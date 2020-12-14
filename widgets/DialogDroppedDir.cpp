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

#include "DialogDroppedDir.h"
#include "ui_DialogDroppedDir.h"

#include "Settings/SearchSettings.h"

DialogDroppedDir::DialogDroppedDir(const QMap<QString, QVector<QString>> &dirMap, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogDroppedDir)
{
    ui->setupUi(this);

    ui->pushButtonBack->setVisible(false);
    ui->pushButtonOk->setVisible(false);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeaderItem(0)->setIcon(QIcon(":/res/icons/folder-3.ico"));

    for (auto itr = dirMap.begin(), end = dirMap.end(); itr != end; ++itr) {
        auto pathItem = new QTableWidgetItem(itr.key());

        for (QStringView name : itr.value()) {
            auto nameItem = new QTableWidgetItem(name.toString());
            int row = ui->tableWidget->rowCount();

            ui->tableWidget->insertRow(row);
            ui->tableWidget->setItem(row, 0, nameItem);
            ui->tableWidget->setItem(row, 1, pathItem);
        }
    }

    const int itemCount = ui->tableWidget->rowCount();

    ui->pushButtonRegisterDir->setEnabled(itemCount != 0);
    ui->pushButtonSearch->setEnabled(itemCount != 0);

    const QString registerDirText[2] = {
        QStringLiteral("Register these directories"),
        QStringLiteral("Register this directory"),
    };

    const QString searchText[2] = {
        QStringLiteral("Search in these directories (Go to settings)"),
        QStringLiteral("Search in this directory (Go to settings)"),
    };

    ui->pushButtonRegisterDir->setText(registerDirText[itemCount == 1]);
    ui->pushButtonSearch->setText(searchText[itemCount == 1]);

    SearchSettings settings;
    settings.read();

    ui->checkBoxDirs->setChecked(settings.value(SearchSettings::searchDirs, false));
    ui->checkBoxFiles->setChecked(settings.value(SearchSettings::searchFiles, true));
    ui->lineEditFilter->setText(settings.value(SearchSettings::filter, QString("")));
    ui->spinBoxHierarchy->setValue(settings.value(SearchSettings::hierarchy, 0));
}

DialogDroppedDir::~DialogDroppedDir()
{
    delete ui;
}

bool DialogDroppedDir::isRegisterDir() const
{
    return m_isRegisterDir;
}

void DialogDroppedDir::on_pushButtonBack_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);

    ui->pushButtonBack->setVisible(false);
    ui->pushButtonOk->setVisible(false);
}

void DialogDroppedDir::on_pushButtonOk_clicked()
{
    SearchSettings settings;

    settings.setValue(SearchSettings::searchDirs, ui->checkBoxDirs->isChecked());
    settings.setValue(SearchSettings::searchFiles, ui->checkBoxFiles->isChecked());
    settings.setValue(SearchSettings::filter, ui->lineEditFilter->text());
    settings.setValue(SearchSettings::hierarchy, ui->spinBoxHierarchy->value());

    settings.write();

    accept();
}

void DialogDroppedDir::on_pushButtonRegisterDir_clicked()
{
    m_isRegisterDir = true;

    accept();
}

void DialogDroppedDir::on_pushButtonSearch_clicked()
{
    m_isRegisterDir = false;

    ui->stackedWidget->setCurrentIndex(1);

    ui->pushButtonBack->setVisible(true);
    ui->pushButtonOk->setVisible(true);
}
