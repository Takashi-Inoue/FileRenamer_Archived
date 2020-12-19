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

#include "WidgetNumberSetting.h"
#include "ui_WidgetNumberSetting.h"

#include "StringBuilder/Number.h"

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
