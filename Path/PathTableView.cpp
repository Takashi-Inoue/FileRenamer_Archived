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
    , m_actions(QMetaEnum::fromType<Actions>().keyCount(), nullptr)
{
    createContextMenu();
}

void PathTableView::setEnableToChangeItems(bool isEnable)
{
    setDragEnabled(isEnable);

    Q_CHECK_PTR(m_actions[RemoveItem]);

    m_actions[RemoveItem]->setEnabled(isEnable);
}

void PathTableView::contextMenuEvent(QContextMenuEvent *event)
{
    if (!selectionModel()->hasSelection())
        return;

    auto menu = findChild<QMenu *>();
    auto pathModel = qobject_cast<PathModel*>(model());

    Q_CHECK_PTR(menu);
    Q_CHECK_PTR(pathModel);
    Q_CHECK_PTR(m_menuSection);

    QModelIndex index = currentIndex();

    m_menuSection->setText(index.data().toString());

    if (index.column() == int(PathModel::HSection::path) || pathModel->isDir(index)) {
        m_actions[OpenPath]->setText(QStringLiteral("Open directory(&O)"));
        m_actions[DeletePath]->setText(QStringLiteral("Delete directory(&D)"));
    }

    auto hSection = PathModel::HSection(index.column());

    if (hSection == PathModel::HSection::newName) {
        bool isExist = QFileInfo::exists(pathModel->fullPath(index));

        m_actions[OpenPath]->setEnabled(isExist);
        m_actions[DeletePath]->setEnabled(isExist);
    } else {
        m_actions[OpenPath]->setEnabled(true);
        m_actions[DeletePath]->setEnabled(index.column() != int(PathModel::HSection::path));
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

void PathTableView::createContextMenu()
{
    // RemoveItem, CopyName, DeletePath, OpenPath, OpenMulti
    const QStringList texts = {
        QStringLiteral("Remove from list(&R)")
      , QStringLiteral("Copy name(&C)")
      , QStringLiteral("Delete file(&D)")
      , QStringLiteral("Open file(&O)")
      , QStringLiteral("Open both files(&B)")
    };

    const QList<QIcon> icons = {
        QIcon(QStringLiteral(":/res/images/x.svg"))
      , QIcon(QStringLiteral(":/res/images/file.svg"))
      , QIcon(QStringLiteral(":/res/images/delete_file.svg"))
      , QIcon(QStringLiteral(":/res/images/exec.svg"))
      , QIcon(QStringLiteral(":/res/images/exec.svg"))
    };

    const QKeySequence shortcuts[] = {
        QKeySequence::Delete
      , QKeySequence::Copy
      , QKeySequence::DeleteEndOfWord
      , QKeySequence::Open
      , QStringLiteral("Ctrl+Shift+O")
    };

    const QStringList slotNames = {
        QStringLiteral("removeSelectedRows()")
      , QStringLiteral("copyName()")
      , QStringLiteral("deleteFile()")
      , QStringLiteral("openFile()")
      , QStringLiteral("openBothFiles()")
    };

    auto menu = new QMenu(this);

    for (int i = 0, count = QMetaEnum::fromType<Actions>().keyCount(); i < count; ++i) {
        m_actions[i] = new QAction(icons[i], texts[i], menu);
        m_actions[i]->setShortcutContext(Qt::WidgetShortcut);
        m_actions[i]->setShortcut(shortcuts[i]);

        int slotIndex = PathTableView::metaObject()->indexOfSlot(qPrintable(slotNames[i]));
        int signalIndex = m_actions[i]->metaObject()->indexOfSignal("triggered(bool)");

        Q_ASSERT(slotIndex != -1 && signalIndex != -1);

        QMetaMethod signal = m_actions[i]->metaObject()->method(signalIndex);
        QMetaMethod slot = PathTableView::metaObject()->method(slotIndex);

        connect(m_actions[i], signal, this, slot);
    }

    QAction *sectionHeader = menu->addSection(QStringLiteral("List"));
    menu->addAction(m_actions[RemoveItem]);
    m_menuSection = menu->addSection(QStringLiteral("Original file"));
    menu->addAction(m_actions[CopyName]);
    menu->addAction(m_actions[OpenPath]);
    menu->addAction(m_actions[DeletePath]);
    menu->addSeparator();
    menu->addAction(m_actions[OpenMulti]);

    QFont font(sectionHeader->font());

    font.setBold(true);
    font.setPointSizeF(font.pointSizeF() + 0.5);
    sectionHeader->setFont(font);
    m_menuSection->setFont(font);

    addActions(m_actions);
}
