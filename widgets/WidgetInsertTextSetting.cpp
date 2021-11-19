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

#include "WidgetInsertTextSetting.h"
#include "ui_WidgetInsertTextSetting.h"

#include "StringBuilder/InsertString.h"

namespace {
constexpr char settingsGroupName[] = "TextInsertion";
constexpr char settingsKeyText[] = "Text";
}

WidgetInsertTextSetting::WidgetInsertTextSetting(QWidget *parent) :
    AbstractStringBuilderWidget(parent),
    ui(new Ui::WidgetInsertTextSetting)
{
    ui->setupUi(this);

    connect(ui->widgetPositionFixer, &WidgetPositionFixer::changeStarted
          , this, &AbstractStringBuilderWidget::changeStarted);

    connect(ui->lineEdit, &QLineEdit::textChanged
          , this, &AbstractStringBuilderWidget::changeStarted);
}

WidgetInsertTextSetting::~WidgetInsertTextSetting()
{
    delete ui;
}

QSharedPointer<StringBuilder::AbstractStringBuilder> WidgetInsertTextSetting::StringBuilder() const
{
    return QSharedPointer<StringBuilder::InsertString>::create(
                ui->widgetPositionFixer->value(), ui->lineEdit->text());
}

void WidgetInsertTextSetting::loadSettings(QSharedPointer<QSettings> qSettings)
{
    qSettings->beginGroup(settingsGroupName);

    ui->lineEdit->setText(qSettings->value(settingsKeyText).toString());
    ui->widgetPositionFixer->loadSettings(qSettings);

    qSettings->endGroup();
}

void WidgetInsertTextSetting::saveSettings(QSharedPointer<QSettings> qSettings) const
{
    qSettings->beginGroup(settingsGroupName);

    qSettings->setValue(settingsKeyText, ui->lineEdit->text());
    ui->widgetPositionFixer->saveSettings(qSettings);

    qSettings->endGroup();
}
