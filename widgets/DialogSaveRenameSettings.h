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

#ifndef DIALOGSAVERENAMESETTINGS_H
#define DIALOGSAVERENAMESETTINGS_H

#include <QDialog>

class QListWidgetItem;

namespace Ui {
class DialogSaveRenameSettings;
}

class DialogSaveRenameSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSaveRenameSettings(QWidget *parent = nullptr);
    ~DialogSaveRenameSettings() override;

    QString newSettingFullpath() const;

public slots:
    void accept() override;

private slots:
    void onLineEditTextChanged(const QString &text);
    void onListWidgetCurrentTextChanged(const QString &currentText);
    void onListWidgetItemPressed(QListWidgetItem *item);

private:
    Ui::DialogSaveRenameSettings *ui;

    QString m_newSettingName;
};

#endif // DIALOGSAVERENAMESETTINGS_H
