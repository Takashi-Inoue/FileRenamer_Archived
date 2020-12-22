/*
 * Copyright 2020 Takashi Inoue
 *
 * This file is part of FileRenamer.
 *
 * FileRenamer is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FileRenamer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with FileRenamer.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "PathModel.h"
#include "PathRoot.h"
#include "PathEntity.h"
#include "ThreadCreateNewNames.h"
#include "ThreadRename.h"
#include "ThreadUndoRenaming.h"

#include <QIcon>

PathModel::PathModel(QObject *parent)
    : QAbstractTableModel(parent)
    , m_dataRoot(QSharedPointer<Path::PathRoot>::create())
    , m_threadCreateNewNames(new ThreadCreateNewNames(m_dataRoot, this))
    , m_threadRename(new ThreadRename(m_dataRoot, this))
    , m_threadUndoRenaming(new ThreadUndoRenaming(m_dataRoot, this))
{
    connect(m_threadCreateNewNames, &ThreadCreateNewNames::newNameCreated
          , this, &PathModel::onNewNameCreated);

    connect(m_threadCreateNewNames, &ThreadCreateNewNames::newNameCollisionDetected
          , this, &PathModel::onNewNameCollisionDetected);

    connect(m_threadCreateNewNames, &QThread::finished, this, &PathModel::onCreateNameCompleted);

    connect(m_threadRename, &ThreadRename::renamed, this, &PathModel::onNewNameStateChanged);
    connect(m_threadRename, &ThreadRename::stopped, this, &PathModel::renameStopped);
    connect(m_threadRename, &ThreadRename::completed, this, &PathModel::renameFinished);

    connect(m_threadUndoRenaming, &ThreadUndoRenaming::renamed, this, &PathModel::onNewNameStateChanged);
    connect(m_threadUndoRenaming, &ThreadUndoRenaming::stopped, this, &PathModel::renameStopped);
    connect(m_threadUndoRenaming, &ThreadUndoRenaming::completed, this, &PathModel::readyToRename);
}

QVariant PathModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical)
        return QVariant();

    if (role == Qt::DecorationRole) {
        static const QMap<HSection, QVariant> m_iconMap = {
            {HSection::originalName, QIcon(QStringLiteral(":/res/images/rename_old.svg"))},
            {HSection::newName,      QIcon(QStringLiteral(":/res/images/rename_new.svg"))},
            {HSection::path,         QIcon(QStringLiteral(":/res/icons/folder-3.ico"))},
        };

        return m_iconMap[HSection(section)];
    }

    if (role == Qt::DisplayRole) {
        static const QMap<HSection, QString> m_textMap = {
            {HSection::originalName, QStringLiteral("Original name")},
            {HSection::newName,      QStringLiteral("New name")},
            {HSection::path,         QStringLiteral("Path")},
        };

        return m_textMap[HSection(section)];
    }

    return QVariant();
}

int PathModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return int(m_dataRoot->entityCount());
}

int PathModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 3;
}

QVariant PathModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    QSharedPointer<Path::PathEntity> entity = m_dataRoot->entity(index.row());

    HSection hSection = HSection(index.column());

    if (role == Qt::DisplayRole) {
        if (hSection == HSection::originalName)
            return entity->name();

        if (hSection == HSection::newName)
            return entity->newName();

        if (hSection == HSection::path)
            return entity->parentPath();
    }

    if (role == Qt::DecorationRole) {
        if (hSection == HSection::originalName)
            return entity->typeIcon();

        if (hSection == HSection::newName)
            return entity->stateIcon();
    }

    return QVariant();
}

void PathModel::sort(int column, Qt::SortOrder order)
{
    if (m_dataRoot->isEmpty())
        return;

    stopThreadToCreateNames();

    beginResetModel();

    column == int(HSection::originalName) ? m_dataRoot->sortByEntityName(order)
                                          : m_dataRoot->sortByParentDir(order);

    endResetModel();

    emit internalDataChanged();
}

void PathModel::addPaths(QList<PathModel::ParentChildrenPair> dirs
                       , QList<PathModel::ParentChildrenPair> files)
{
    if (dirs.isEmpty() && files.isEmpty())
        return;

    stopThreadToCreateNames();

    beginResetModel();

    m_dataRoot->addPathsAsDirs(dirs);
    m_dataRoot->addPathsAsFiles(files);

    endResetModel();

    emit internalDataChanged();
}

void PathModel::removeSpecifiedRows(QList<int> rows)
{
    stopThreadToCreateNames();

    beginResetModel();
    m_dataRoot->removeSpecifiedRows(rows);
    endResetModel();

    m_dataRoot->entityCount() != 0 ? emit internalDataChanged()
                                   : emit itemCleared();
}

void PathModel::copyOriginalNameToClipboard(int row) const
{
    m_dataRoot->entity(row)->copyOriginalNameToClipboard();
}

void PathModel::clear()
{
    stopThreadToCreateNames();

    beginResetModel();
    m_dataRoot->clear();
    endResetModel();

    emit itemCleared();
}

// Start/Stop threads
void PathModel::startCreateNewNames(QSharedPointer<StringBuilderOnFile::BuilderChainOnFile> builderChain)
{
    if (m_dataRoot->isEmpty())
        return;

    m_threadCreateNewNames->stop();
    m_threadCreateNewNames->wait();
    m_threadCreateNewNames->setStringBuilderOnFile(builderChain);
    m_threadCreateNewNames->start();
}

void PathModel::startRename()
{
    emit renameStarted();
    m_threadRename->start();
}

void PathModel::stopRename()
{
    if (m_threadRename->isRunning()) {
        m_threadRename->stop();
        m_threadRename->wait();
    }

    if (m_threadUndoRenaming->isRunning()) {
        m_threadUndoRenaming->stop();
        m_threadUndoRenaming->wait();
    }
}

void PathModel::undoRename()
{
    emit undoStarted();
    m_threadUndoRenaming->start();
}

// private slots //
void PathModel::onCreateNameCompleted()
{
    if (m_dataRoot->isEmpty())
        return;

    QModelIndex tl = index(0, int(HSection::newName));
    QModelIndex br = index(int(m_dataRoot->entityCount() - 1), int(HSection::newName));

    emit dataChanged(tl, br, {Qt::DecorationRole});
    emit readyToRename();
}

void PathModel::onNewNameCollisionDetected(QPair<int, int> indices)
{
    QModelIndex lIndex = index(indices.first,  int(HSection::newName));
    QModelIndex rIndex = index(indices.second, int(HSection::newName));

    emit dataChanged(lIndex, lIndex, {Qt::DecorationRole});
    emit dataChanged(rIndex, rIndex, {Qt::DecorationRole});
}

void PathModel::onNewNameCreated(int row)
{
    QModelIndex modelIndex = index(row, int(HSection::newName));

    emit dataChanged(modelIndex, modelIndex, {Qt::DisplayRole});
}

void PathModel::onNewNameStateChanged(int row)
{
    QModelIndex modelIndex = index(row, int(HSection::newName));

    emit dataChanged(modelIndex, modelIndex, {Qt::DecorationRole});
}

// private //
void PathModel::stopThreadToCreateNames()
{
    m_threadCreateNewNames->stop();
    m_threadCreateNewNames->wait();
}
