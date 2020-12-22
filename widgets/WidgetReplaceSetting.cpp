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
