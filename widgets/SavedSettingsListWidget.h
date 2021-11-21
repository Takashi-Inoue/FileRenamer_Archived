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

#ifndef SAVEDSETTINGSLISTWIDGET_H
#define SAVEDSETTINGSLISTWIDGET_H

#include <QListWidget>

class SavedSettingsListWidget : public QListWidget
{
    Q_OBJECT

public:
    explicit SavedSettingsListWidget(QWidget *parent = nullptr);
    ~SavedSettingsListWidget() override = default;

    QString settingsDir() const;

public slots:
    void load();

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
    void showEvent(QShowEvent *event) override;

private slots:
    void onActionDeleteTriggered();
    void onItemSelectionChanged();

private:
    QAction *m_deleteAction;
};

#endif // SAVEDSETTINGSLISTWIDGET_H
