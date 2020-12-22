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

#ifndef PATHTABLEVIEW_H
#define PATHTABLEVIEW_H

#include <QTableView>

class QAction;

class PathTableView : public QTableView
{
    Q_OBJECT
public:
    PathTableView(QWidget *parent = nullptr);

    QList<QAction *> actionsToChangeItem() const;
    void setEnableToChangeItems(bool isEnable);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

private slots:
    void onActionCopyNameTriggered();
    void onActionDeleteTriggered();

private:
    QAction *m_actionCopyName;
    QAction *m_actionDeleteItem;
};

#endif // PATHTABLEVIEW_H
