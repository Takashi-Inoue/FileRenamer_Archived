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

#ifndef PATHTABLEVIEW_H
#define PATHTABLEVIEW_H

#include <QTableView>

class QAction;

class PathTableView : public QTableView
{
    Q_OBJECT
public:
    PathTableView(QWidget *parent = nullptr);

    void setEnableToChangeItems(bool isEnable);

    enum Actions {
        RemoveItem, CopyName, DeletePath, OpenPath, OpenMulti
    };

    Q_ENUM(Actions)

signals:
    void selectedCountChanged(qsizetype);
    void statusTextChanged(QIcon, QString);
    void stateTextChanged(QIcon, QString);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) override;

private slots:
    void copyName();
    void openFile();
    void deleteFile();
    void openBothFiles();
    void removeSelectedRows();

private:
    void createContextMenu();

    QList<QAction *> m_actions;

    QAction *m_menuSection = nullptr;
};

#endif // PATHTABLEVIEW_H
