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

#include "PathTableView.h"
#include "PathModel.h"

#include <QContextMenuEvent>
#include <QMenu>
#include <QDebug>

PathTableView::PathTableView(QWidget *parent)
    : QTableView(parent)
    , m_actionCopyName(new QAction(QIcon(":/res/icons/text-x-preview.ico"), "Copy name(&C)", this))
    , m_actionDeleteItem(new QAction(QIcon(":/res/images/x.svg"), "Delete(&D)", this))
{
    m_actionCopyName->setToolTip("Copy original name (topmost selected item)");
    m_actionCopyName->setShortcutContext(Qt::WidgetShortcut);
    m_actionCopyName->setShortcut(QKeySequence::Copy);

    m_actionDeleteItem->setToolTip("Delete selected items");
    m_actionDeleteItem->setShortcutContext(Qt::WidgetShortcut);
    m_actionDeleteItem->setShortcut(QKeySequence::Delete);

    addAction(m_actionCopyName);
    addAction(m_actionDeleteItem);

    connect(m_actionCopyName, &QAction::triggered, this, &PathTableView::onActionCopyNameTriggered);
    connect(m_actionDeleteItem, &QAction::triggered, this, &PathTableView::onActionDeleteTriggered);
}

QVector<QAction *> PathTableView::actionsToChangeItem() const
{
    return {m_actionCopyName, m_actionDeleteItem};
}

void PathTableView::contextMenuEvent(QContextMenuEvent *event)
{
    if (!selectionModel()->hasSelection())
        return;

    auto menu = findChild<QMenu *>();

    if (menu == nullptr) {
        menu = new QMenu(this);
        menu->addActions(actions());

        connect(m_actionDeleteItem, &QAction::triggered, menu, &QMenu::close);
    }

    menu->popup(viewport()->mapToGlobal(event->pos()));
}

void PathTableView::onActionCopyNameTriggered()
{
    auto pathModel = qobject_cast<PathModel *>(model());

    if (pathModel == nullptr)
        return;

    QModelIndexList indices = selectionModel()->selectedRows();

    std::sort(indices.begin(), indices.end(), [](const QModelIndex &lhs, const QModelIndex &rhs) {
        return lhs.row() < rhs.row();
    });

    pathModel->copyOriginalNameToClipboard(indices.first().row());
}

void PathTableView::onActionDeleteTriggered()
{
    auto pathModel = qobject_cast<PathModel *>(model());

    if (pathModel == nullptr)
        return;

    QVector<int> rows;

    for (const QModelIndex &modelIndex: selectionModel()->selectedRows())
        rows << modelIndex.row();

    pathModel->removeSpecifiedRows(rows);
}
