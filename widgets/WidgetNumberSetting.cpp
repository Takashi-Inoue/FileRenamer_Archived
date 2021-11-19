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

#include "WidgetNumberSetting.h"
#include "ui_WidgetNumberSetting.h"

#include "StringBuilder/Number.h"

namespace {
constexpr char settingsGroupName[] = "Number";
constexpr char settingsKeyStart[] = "Start";
constexpr char settingsKeyIncremental[] = "Incremental";
constexpr char settingsKeyDigit[] = "Digit";
constexpr char settingsKeyPrefix[] = "Prefix";
constexpr char settingsKeySuffix[] = "Suffix";
}

WidgetNumberSetting::WidgetNumberSetting(QWidget *parent) :
    AbstractStringBuilderWidget(parent),
    ui(new Ui::WidgetNumberSetting)
{
    ui->setupUi(this);

    connect(ui->spinBoxStart, &QSpinBox::valueChanged
          , this, &AbstractStringBuilderWidget::changeStarted);

    connect(ui->spinBoxStep, &QSpinBox::valueChanged
          , this, &AbstractStringBuilderWidget::changeStarted);

    connect(ui->spinBoxDigit, &QSpinBox::valueChanged
          , this, &AbstractStringBuilderWidget::changeStarted);

    connect(ui->lineEditPrefix, &QLineEdit::textChanged
          , this, &AbstractStringBuilderWidget::changeStarted);

    connect(ui->lineEditSuffix, &QLineEdit::textChanged
          , this, &AbstractStringBuilderWidget::changeStarted);

    connect(ui->widgetPositionFixer, &WidgetPositionFixer::changeStarted
          , this, &AbstractStringBuilderWidget::changeStarted);
}

WidgetNumberSetting::~WidgetNumberSetting()
{
    delete ui;
}

QSharedPointer<StringBuilder::AbstractStringBuilder> WidgetNumberSetting::StringBuilder() const
{
    return QSharedPointer<StringBuilder::Number>::create(
                ui->widgetPositionFixer->value()
              , ui->spinBoxStart->value(), ui->spinBoxStep->value(), ui->spinBoxDigit->value()
              , ui->lineEditPrefix->text(), ui->lineEditSuffix->text()
                );
}

void WidgetNumberSetting::loadSettings(QSharedPointer<QSettings> qSettings)
{
    qSettings->beginGroup(settingsGroupName);

    ui->spinBoxStart->setValue(qSettings->value(settingsKeyStart, 0).toInt());
    ui->spinBoxStep->setValue(qSettings->value(settingsKeyIncremental, 1).toInt());
    ui->spinBoxDigit->setValue(qSettings->value(settingsKeyDigit, 0).toInt());
    ui->lineEditPrefix->setText(qSettings->value(settingsKeyPrefix).toString());
    ui->lineEditSuffix->setText(qSettings->value(settingsKeySuffix).toString());
    ui->widgetPositionFixer->loadSettings(qSettings);

    qSettings->endGroup();
}

void WidgetNumberSetting::saveSettings(QSharedPointer<QSettings> qSettings) const
{
    qSettings->beginGroup(settingsGroupName);

    qSettings->setValue(settingsKeyStart, ui->spinBoxStart->value());
    qSettings->setValue(settingsKeyIncremental, ui->spinBoxStep->value());
    qSettings->setValue(settingsKeyDigit, ui->spinBoxDigit->value());
    qSettings->setValue(settingsKeyPrefix, ui->lineEditPrefix->text());
    qSettings->setValue(settingsKeySuffix, ui->lineEditSuffix->text());
    ui->widgetPositionFixer->saveSettings(qSettings);

    qSettings->endGroup();
}
