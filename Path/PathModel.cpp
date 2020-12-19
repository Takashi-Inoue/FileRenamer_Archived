/*
 * Copyright YEAR Takashi Inoue
 *
 * This file is part of APPNAME.
 *
 * APPNAME is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * APPNAME is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with APPNAME.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "PathModel.h"
#include "PathRoot.h"
#include "PathEntity.h"
#include "ThreadCreateNewNames.h"

#include <QFileIconProvider>
#include <QIcon>

PathModel::PathModel(QObject *parent)
    : QAbstractTableModel(parent)
    , m_dataRoot(QSharedPointer<Path::PathRoot>::create())
    , m_threadCreateNewNames(new ThreadCreateNewNames(m_dataRoot, this))
{
    connect (m_threadCreateNewNames, &ThreadCreateNewNames::newNameCreated, this, [this](int row) {
        emit dataChanged(index(row, int(HSection::newName)), index(row, int(HSection::newName)));
    });
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

    if (role == Qt::DecorationRole && hSection == HSection::originalName) {
        static const QIcon icons[] = {
            QFileIconProvider().icon(QAbstractFileIconProvider::File),
            QFileIconProvider().icon(QAbstractFileIconProvider::Folder),
        };

        return icons[entity->isDir()];
    }

    return QVariant();
}

void PathModel::sort(int column, Qt::SortOrder order)
{
    beginResetModel();

    column == int(HSection::originalName) ? m_dataRoot->sortByEntityName(order)
                                          : m_dataRoot->sortByParentDir(order);

    endResetModel();
}

bool PathModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_ASSERT(uint(count) > 0);

    beginRemoveRows(parent, row, row + count - 1);
    m_dataRoot->remove(row, count);
    endRemoveRows();

    return true;
}

void PathModel::removeSpecifiedRows(QVector<int> rows)
{
    beginResetModel();
    m_dataRoot->removeSpecifiedRows(rows);
    endResetModel();
}

void PathModel::addPathsAsDirs(QVector<ParentChildrenPair> dirs)
{
    beginResetModel();
    m_dataRoot->addPathsAsDirs(dirs);
    endResetModel();
}

void PathModel::addPathsAsFiles(QVector<ParentChildrenPair> files)
{
    beginResetModel();
    m_dataRoot->addPathsAsFiles(files);
    endResetModel();
}

void PathModel::copyOriginalNameToClipboard(int row) const
{
    m_dataRoot->entity(row)->copyOriginalNameToClipboard();
}

void PathModel::startCreateNewNames(QSharedPointer<StringBuilderOnFile::BuilderChainOnFile> builderChain)
{
    m_threadCreateNewNames->stop();
    m_threadCreateNewNames->wait();
    m_threadCreateNewNames->setStringBuilderOnFile(builderChain);
    m_threadCreateNewNames->start();
}
