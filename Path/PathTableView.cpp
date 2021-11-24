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
#include <QGuiApplication>
#include <QMenu>
#include <QDebug>

PathTableView::PathTableView(QWidget *parent)
    : QTableView(parent)
    , m_actionRemoveItem(new QAction(QIcon(":/res/images/x.svg"), "Remove from list(&R)", this))
{
    auto actionCopyName = new QAction(QIcon(":/icons/text"), "Copy original name(&C)", this);

    actionCopyName->setToolTip("Copy original name (current item)");
    actionCopyName->setShortcutContext(Qt::WidgetShortcut);
    actionCopyName->setShortcut(QKeySequence::Copy);

    m_actionRemoveItem->setToolTip("Remove selected items from list");
    m_actionRemoveItem->setShortcutContext(Qt::WidgetShortcut);
    m_actionRemoveItem->setShortcut(QKeySequence::Delete);

    addAction(actionCopyName);
    addAction(m_actionRemoveItem);

    connect(actionCopyName, &QAction::triggered, this, &PathTableView::onActionCopyNameTriggered);
    connect(m_actionRemoveItem, &QAction::triggered, this, &PathTableView::onActionRemoveTriggered);
}

void PathTableView::setEnableToChangeItems(bool isEnable)
{
    setDragEnabled(isEnable);
    m_actionRemoveItem->setEnabled(isEnable);
}

void PathTableView::contextMenuEvent(QContextMenuEvent *event)
{
    if (!selectionModel()->hasSelection())
        return;

    auto menu = findChild<QMenu *>();

    if (menu == nullptr) {
        menu = new QMenu(this);
        menu->addActions(actions());

        connect(m_actionRemoveItem, &QAction::triggered, menu, &QMenu::close);
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

    if (selectedCount == 1) {
        statusText = selectedRows.first().data(Qt::StatusTipRole).toString();
        statusIcon = selectedRows.first().data(Qt::DecorationRole).value<QIcon>();
    }

    emit statusTextChanged(statusIcon, statusText);
}

void PathTableView::onActionCopyNameTriggered()
{
    QModelIndex index = model()->index(currentIndex().row(), int(PathModel::HSection::originalName));

    QString name = index.data().toString();

    QGuiApplication::clipboard()->setText(name);

    QString statusText = QStringLiteral("Copied to clipboard : <i>%1</i>").arg(name);

    emit statusTextChanged(QIcon(), statusText);
}

void PathTableView::onActionRemoveTriggered()
{
    auto pathModel = qobject_cast<PathModel *>(model());

    if (pathModel == nullptr)
        return;

    QList<int> rows;

    for (const QModelIndex &modelIndex: selectionModel()->selectedRows())
        rows << modelIndex.row();

    pathModel->removeSpecifiedRows(rows);
}
