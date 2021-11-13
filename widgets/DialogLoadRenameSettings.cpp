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

#include "DialogLoadRenameSettings.h"
#include "ui_DialogLoadRenameSettings.h"

#include "Application.h"

#include <QPushButton>

DialogLoadRenameSettings::DialogLoadRenameSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogLoadRenameSettings)
{
    ui->setupUi(this);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

DialogLoadRenameSettings::~DialogLoadRenameSettings()
{
    delete ui;
}

QString DialogLoadRenameSettings::settingFullPath() const
{
    QListWidgetItem *currentItem = ui->listWidget->currentItem();

    if (currentItem == nullptr)
        return QString();

    return QStringLiteral("%1/%2.ini").arg(Application::renameSettingsDirPath(), currentItem->text());
}

void DialogLoadRenameSettings::onListWidgetItemSelectionChanged()
{
    bool hasSelection = ui->listWidget->selectionModel()->hasSelection();

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(hasSelection);
}
