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

#ifndef DIALOGLOADRENAMESETTINGS_H
#define DIALOGLOADRENAMESETTINGS_H

#include <QDialog>

namespace Ui {
class DialogLoadRenameSettings;
}

class DialogLoadRenameSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DialogLoadRenameSettings(QWidget *parent = nullptr);
    ~DialogLoadRenameSettings() override;

    QString settingFullPath() const;

private slots:
    void onListWidgetItemSelectionChanged();

private:
    Ui::DialogLoadRenameSettings *ui;
};

#endif // DIALOGLOADRENAMESETTINGS_H
