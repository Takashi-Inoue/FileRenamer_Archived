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

#include "FormStringBuilder.h"
#include "ui_FormStringBuilder.h"

#include "widgets/WidgetPositionFixer.h"

#include <QAction>
#include <QPushButton>
#include <QResizeEvent>
#include <QDebug>

FormStringBuilder::FormStringBuilder(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::FormStringBuilder)
{
    ui->setupUi(this);

//    ui->widgetButtons->setVisible(false);

    for (auto widget : findChildren<AbstractStringBuilderWidget *>()) {
        connect(widget, &AbstractStringBuilderWidget::changeStarted
              , this, &FormStringBuilder::changeStarted);
    }

    connect(ui->buttonRemove, &QPushButton::clicked, this, &FormStringBuilder::requestRemove);
    connect(ui->buttonRemove, &QPushButton::clicked, this, &FormStringBuilder::close);

    connect(ui->buttonDown, &QPushButton::clicked, this, &FormStringBuilder::requestDown);
    connect(ui->buttonUp,   &QPushButton::clicked, this, &FormStringBuilder::requestUp);

    connect(ui->comboBoxBuilders, &QComboBox::currentIndexChanged
          , this, &FormStringBuilder::changeStarted);
}

FormStringBuilder::~FormStringBuilder()
{
    delete ui;
}

QSharedPointer<StringBuilder::AbstractStringBuilder> FormStringBuilder::stringBuilder() const
{
    auto widget = qobject_cast<AbstractStringBuilderWidget *>(ui->stackedWidget->currentWidget());

    Q_ASSERT(widget != nullptr);

    return widget->StringBuilder();
}

int FormStringBuilder::currentBuilderIndex() const
{
    return ui->comboBoxBuilders->currentIndex();
}

void FormStringBuilder::setCurrentBuilderIndex(int index)
{
    ui->comboBoxBuilders->setCurrentIndex(index);
}

void FormStringBuilder::loadBuilderSettings(QSharedPointer<QSettings> qSettings)
{
    auto widget = qobject_cast<AbstractStringBuilderWidget *>(ui->stackedWidget->currentWidget());

    if (widget != nullptr)
        widget->loadSettings(qSettings);
}

void FormStringBuilder::saveCurrentBuilderSettings(QSharedPointer<QSettings> qSettings) const
{
    auto widget = qobject_cast<AbstractStringBuilderWidget *>(ui->stackedWidget->currentWidget());

    if (widget != nullptr)
        widget->saveSettings(qSettings);
}

void FormStringBuilder::notifySettingIndexChanged(int index, int settingsCount)
{
    ui->buttonDown->setEnabled(index < settingsCount - 1);
    ui->buttonUp->setEnabled(index != 0);

    for (int i = 0, count = ui->stackedWidget->count(); i < count; ++i) {
        auto widget = qobject_cast<AbstractStringBuilderWidget *>(ui->stackedWidget->widget(i));
        auto positionFixer = widget->findChild<WidgetPositionFixer *>();

        if (positionFixer != nullptr)
            positionFixer->setEnabled(index != 0);
    }

    qDebug() << index << settingsCount;
}

//void FormStringBuilder::enterEvent(QEnterEvent *event)
//{
//    ui->widgetButtons->setVisible(true);

//    QFrame::enterEvent(event);
//}

//void FormStringBuilder::leaveEvent(QEvent *event)
//{
//    ui->widgetButtons->setVisible(false);

//    QFrame::leaveEvent(event);
//}

//void FormStringBuilder::moveEvent(QMoveEvent *event)
//{
//    QPoint cursorPos = mapToParent(mapFromGlobal(QCursor::pos()));

//    ui->widgetButtons->setVisible(geometry().contains(cursorPos));

//    QFrame::moveEvent(event);
//}
