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

#include "WidgetReplaceSetting.h"
#include "ui_WidgetReplaceSetting.h"

#include "StringBuilder/RegExpReplace.h"
#include "StringBuilder/ReplaceString.h"

namespace {
    constexpr char settingsGroupName[] = "TextReplace";
    constexpr char settingsKeyAfter[]  = "After";
    constexpr char settingsKeyBefore[] = "Before";
    constexpr char settingsKeyUserRegexp[] = "UseRegexp";
    constexpr char settingsKeyCaseSensitive[] = "CaseSensitive";
}

WidgetReplaceSetting::WidgetReplaceSetting(QWidget *parent) :
    AbstractStringBuilderWidget(parent),
    ui(new Ui::WidgetReplaceSetting)
{
    ui->setupUi(this);

    connect(ui->checkBoxCaseSensitive, &QCheckBox::clicked
          , this, &AbstractStringBuilderWidget::changeStarted);

    connect(ui->checkBoxUseRegex, &QCheckBox::clicked
          , this, &AbstractStringBuilderWidget::changeStarted);

    connect(ui->lineEditAfter, &QLineEdit::textChanged
          , this, &AbstractStringBuilderWidget::changeStarted);

    connect(ui->lineEditBefore, &QLineEdit::textChanged
          , this, &AbstractStringBuilderWidget::changeStarted);
}

WidgetReplaceSetting::~WidgetReplaceSetting()
{
    delete ui;
}

QSharedPointer<StringBuilder::AbstractStringBuilder> WidgetReplaceSetting::StringBuilder() const
{
    QString before = ui->lineEditBefore->text();
    QString after = ui->lineEditAfter->text();
    bool isCaseSensitive = ui->checkBoxCaseSensitive->isChecked();

    if (ui->checkBoxUseRegex->isChecked())
        return QSharedPointer<StringBuilder::RegExpReplace>::create(before, after, isCaseSensitive);

    return QSharedPointer<StringBuilder::ReplaceString>::create(before, after, isCaseSensitive);
}

void WidgetReplaceSetting::loadSettings(QSharedPointer<QSettings> qSettings)
{
    qSettings->beginGroup(settingsGroupName);

    ui->lineEditAfter->setText(qSettings->value(settingsKeyAfter).toString());
    ui->lineEditBefore->setText(qSettings->value(settingsKeyBefore).toString());
    ui->checkBoxUseRegex->setChecked(qSettings->value(settingsKeyUserRegexp).toBool());
    ui->checkBoxCaseSensitive->setChecked(qSettings->value(settingsKeyCaseSensitive, true).toBool());

    qSettings->endGroup();
}

void WidgetReplaceSetting::saveSettings(QSharedPointer<QSettings> qSettings) const
{
    qSettings->beginGroup(settingsGroupName);

    qSettings->setValue(settingsKeyAfter, ui->lineEditAfter->text());
    qSettings->setValue(settingsKeyBefore, ui->lineEditBefore->text());
    qSettings->setValue(settingsKeyUserRegexp, ui->checkBoxUseRegex->isChecked());
    qSettings->setValue(settingsKeyCaseSensitive, ui->checkBoxCaseSensitive->isChecked());

    qSettings->endGroup();
}
