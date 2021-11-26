/*
 * Copyright 2021 Takashi Inoue
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

#include "PathTableView.h"
#include "PathModel.h"

#include <QClipboard>
#include <QContextMenuEvent>
#include <QDesktopServices>
#include <QFileInfo>
#include <QGuiApplication>
#include <QMenu>
#include <QMetaEnum>
#include <QDebug>

PathTableView::PathTableView(QWidget *parent)
    : QTableView(parent)
{
    createActions();
    createContextMenu();
}

void PathTableView::setEnableToChangeItems(bool isEnable)
{
    setDragEnabled(isEnable);

    Q_CHECK_PTR(m_actions[Actions::RemoveItem]);

    m_actions[Actions::RemoveItem]->setEnabled(isEnable);
}

void PathTableView::contextMenuEvent(QContextMenuEvent *event)
{
    if (!selectionModel()->hasSelection())
        return;

    auto menu = findChild<QMenu *>();
    auto pathModel = qobject_cast<PathModel*>(model());

    Q_CHECK_PTR(menu);
    Q_CHECK_PTR(pathModel);

    QModelIndex index = currentIndex();

    m_sections[Sections::FileName]->setText(index.data().toString());

    if (index.column() == int(PathModel::HSection::path) || pathModel->isDir(index)) {
        m_actions[Actions::OpenPath]->setText(QStringLiteral("Open directory(&O)"));
        m_actions[Actions::DeletePath]->setText(QStringLiteral("Delete directory(&D)"));
    }

    auto hSection = PathModel::HSection(index.column());

    if (hSection == PathModel::HSection::newName) {
        bool isExist = QFileInfo::exists(pathModel->fullPath(index));

        m_actions[Actions::OpenPath]->setEnabled(isExist);
        m_actions[Actions::DeletePath]->setEnabled(isExist);
    } else {
        m_actions[Actions::OpenPath]->setEnabled(true);
        m_actions[Actions::DeletePath]->setEnabled(index.column() != int(PathModel::HSection::path));
    }

    menu->popup(viewport()->mapToGlobal(event->pos()));
}

void PathTableView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    QTableView::selectionChanged(selected, deselected);

    QModelIndexList selectedRows = selectionModel()->selectedRows();
    qsizetype selectedCount = selectedRows.count();

    emit selectedCountChanged(selectedCount);

    QString statusText;
    QIcon statusIcon;
    QString stateText;
    QIcon stateIcon;

    if (selectedCount == 1) {
        QModelIndex &index = selectedRows.first();

        statusText = index.data(Qt::StatusTipRole).toString();
        statusIcon = index.data(Qt::DecorationRole).value<QIcon>();
        stateText = index.data(PathModel::StateTextRole).toString();
        stateIcon = index.data(PathModel::StateIconRole).value<QIcon>();
    }

    emit statusTextChanged(statusIcon, statusText);
    emit stateTextChanged(stateIcon, stateText);
}

void PathTableView::copyName()
{
    QString text = currentIndex().data().toString();

    QGuiApplication::clipboard()->setText(text);

    QString statusText = QStringLiteral("Copied to clipboard : <b>%1</b>").arg(text);

    emit statusTextChanged(QIcon(), statusText);
}

void PathTableView::openFile()
{
    auto pathModel = qobject_cast<PathModel*>(model());

    Q_CHECK_PTR(pathModel);

    QString fullPath = pathModel->fullPath(currentIndex());

    if (!QFileInfo::exists(fullPath))
        return;

    QDesktopServices::openUrl(QUrl::fromLocalFile(fullPath));
}

void PathTableView::deleteFile()
{
    const QModelIndex index = currentIndex();

    if (index.column() == int(PathModel::HSection::path))
        return;

    auto pathModel = qobject_cast<PathModel*>(model());

    Q_CHECK_PTR(pathModel);

    QString fullPath = pathModel->fullPath(index);

    if (!QFileInfo::exists(fullPath))
        return;

    if (QFile::moveToTrash(fullPath) && index.column() == int(PathModel::HSection::originalName))
        pathModel->removeSpecifiedRows({index.row()});
}

void PathTableView::openBothFiles()
{

}

void PathTableView::removeSelectedRows()
{
    auto pathModel = qobject_cast<PathModel *>(model());

    if (pathModel == nullptr)
        return;

    QList<int> rows;

    for (const QModelIndex &modelIndex: selectionModel()->selectedRows())
        rows << modelIndex.row();

    pathModel->removeSpecifiedRows(rows);
}

void PathTableView::createActions()
{
    const QHash<Actions, QString> texts = {
        {Actions::RemoveItem, QStringLiteral("Remove from list(&R)")}
      , {Actions::CopyName,   QStringLiteral("Copy name(&C)")}
      , {Actions::DeletePath, QStringLiteral("Delete file(&D)")}
      , {Actions::OpenPath,   QStringLiteral("Open file(&O)")}
      , {Actions::OpenMulti , QStringLiteral("Open both files(&B)")}
    };

    const QHash<Actions, QIcon> icons = {
        {Actions::RemoveItem, QIcon(QStringLiteral(":/res/images/x.svg"))}
      , {Actions::CopyName,   QIcon(QStringLiteral(":/res/images/file.svg"))}
      , {Actions::DeletePath, QIcon(QStringLiteral(":/res/images/delete_file.svg"))}
      , {Actions::OpenPath,   QIcon(QStringLiteral(":/res/images/exec.svg"))}
      , {Actions::OpenMulti , QIcon(QStringLiteral(":/res/images/exec_double.svg"))}
    };

    const QHash<Actions, QKeySequence> shortcuts = {
        {Actions::RemoveItem, QKeySequence::Delete}
      , {Actions::CopyName,   QKeySequence::Copy}
      , {Actions::DeletePath, QKeySequence::DeleteEndOfWord}
      , {Actions::OpenPath,   QKeySequence::Open}
      , {Actions::OpenMulti , QStringLiteral("Ctrl+Shift+O")}
    };

    const QHash<Actions, QString> slotNames = {
        {Actions::RemoveItem, QStringLiteral("removeSelectedRows()")}
      , {Actions::CopyName,   QStringLiteral("copyName()")}
      , {Actions::DeletePath, QStringLiteral("deleteFile()")}
      , {Actions::OpenPath,   QStringLiteral("openFile()")}
      , {Actions::OpenMulti , QStringLiteral("openBothFiles()")}
    };

    QMetaEnum enumActions = QMetaEnum::fromType<Actions>();

    for (int i = 0, count = enumActions.keyCount(); i < count; ++i) {
        Actions actionType = Actions(enumActions.value(i));
        QAction *action = new QAction(icons[actionType], texts[actionType], this);

        m_actions[actionType] = action;

        action->setShortcutContext(Qt::WidgetShortcut);
        action->setShortcut(shortcuts[actionType]);

        int slotIndex = PathTableView::metaObject()->indexOfSlot(qPrintable(slotNames[actionType]));
        int signalIndex = action->metaObject()->indexOfSignal("triggered(bool)");

        Q_ASSERT(slotIndex != -1 && signalIndex != -1);

        QMetaMethod signal = action->metaObject()->method(signalIndex);
        QMetaMethod slot = PathTableView::metaObject()->method(slotIndex);

        connect(action, signal, this, slot);
    }
}

void PathTableView::createContextMenu()
{
    Q_ASSERT(m_actions.size() == QMetaEnum::fromType<Actions>().keyCount());

    auto menu = new QMenu(this);

    QAction *sectionHeader = menu->addSection(QStringLiteral("List"));

    menu->addAction(m_actions[Actions::RemoveItem]);
    m_sections[Sections::FileName] = menu->addSection(QStringLiteral("Original file"));
    menu->addAction(m_actions[Actions::CopyName]);
    menu->addAction(m_actions[Actions::OpenPath]);
    menu->addAction(m_actions[Actions::DeletePath]);
    m_sections[Sections::MultiFiles] = menu->addSection(QStringLiteral("Both files"));
    menu->addAction(m_actions[Actions::OpenMulti]);

    QFont font(sectionHeader->font());

    font.setBold(true);
    font.setPointSizeF(font.pointSizeF() + 0.5);
    sectionHeader->setFont(font);
    m_sections[Sections::FileName]->setFont(font);
    m_sections[Sections::MultiFiles]->setFont(font);

    addActions(m_actions.values());
}
