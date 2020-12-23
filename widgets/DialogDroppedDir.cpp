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

#include "DialogDroppedDir.h"
#include "ui_DialogDroppedDir.h"

DialogDroppedDir::DialogDroppedDir(const QList<ParentChildrenPair> &dirs, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogDroppedDir)
{
    ui->setupUi(this);

    ui->pushButtonBack->setVisible(false);
    ui->pushButtonOk->setVisible(false);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeaderItem(0)->setIcon(QIcon(":/res/icons/folder-3.ico"));

    for (const ParentChildrenPair &parentChildren : dirs) {
        auto parentDirItem = new QTableWidgetItem(parentChildren.first);

        for (QStringView childName : parentChildren.second) {
            auto childNameItem = new QTableWidgetItem(childName.toString());
            int row = ui->tableWidget->rowCount();

            ui->tableWidget->insertRow(row);
            ui->tableWidget->setItem(row, 0, childNameItem);
            ui->tableWidget->setItem(row, 1, parentDirItem);
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

    m_settings.read();

    QStringList filtersHistory = m_settings.value(SearchSettings::filterHistory, QStringList());

    for (const QString &filtersString : filtersHistory)
        ui->comboBoxFilter->addItem(filtersString);

    ui->checkBoxDirs->setChecked(m_settings.value(SearchSettings::searchDirs, false));
    ui->checkBoxFiles->setChecked(m_settings.value(SearchSettings::searchFiles, true));
    ui->comboBoxFilter->setCurrentText(m_settings.value(SearchSettings::filter, QString("")));
    ui->spinBoxHierarchy->setValue(m_settings.value(SearchSettings::hierarchy, 0));
}

DialogDroppedDir::~DialogDroppedDir()
{
    delete ui;
}

bool DialogDroppedDir::isRegisterDroppedDir() const
{
    return m_isRegisterDir;
}

const SearchSettings &DialogDroppedDir::searchSettings() const
{
    return m_settings;
}

void DialogDroppedDir::on_pushButtonBack_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);

    ui->pushButtonBack->setVisible(false);
    ui->pushButtonOk->setVisible(false);
}

void DialogDroppedDir::on_pushButtonOk_clicked()
{
    QString filtersString = ui->comboBoxFilter->currentText();

    filtersString = fixFiltersString(filtersString);

    m_settings.setValue(SearchSettings::searchDirs, ui->checkBoxDirs->isChecked());
    m_settings.setValue(SearchSettings::searchFiles, ui->checkBoxFiles->isChecked());
    m_settings.setValue(SearchSettings::filter, filtersString);
    m_settings.setValue(SearchSettings::hierarchy, ui->spinBoxHierarchy->value());

    QStringList filtersList;

    for (int i = 0, count = ui->comboBoxFilter->count(); i < count; ++i)
        filtersList << fixFiltersString(ui->comboBoxFilter->itemText(i));

    filtersList.prepend(filtersString);
    filtersList.removeDuplicates();

    m_settings.setValue(SearchSettings::filterHistory, filtersList);

    m_settings.write();

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

QString DialogDroppedDir::fixFiltersString(QStringView filtersString)
{
    QStringList filters = filtersString.toString().split(';', Qt::SkipEmptyParts);

    for (QString &filter : filters)
        filter = fixOneFilter(filter);

    return filters.join(';');
}

QString DialogDroppedDir::fixOneFilter(QStringView filter)
{
    if (!filter.contains('*') && !filter.contains('?'))
        return QString("*%1*").arg(filter);

    return filter.toString();
}
