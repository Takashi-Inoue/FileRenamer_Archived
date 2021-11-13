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

#include <QAction>
#include <QPushButton>
#include <QResizeEvent>
#include <QDebug>

FormStringBuilder::FormStringBuilder(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::FormStringBuilder)
    , m_buttonClose(new QPushButton(QIcon(":/res/images/x.svg"), QString(), this))
{
    ui->setupUi(this);

    m_buttonClose->setVisible(false);
    m_buttonClose->raise();
    m_buttonClose->setToolTip(QStringLiteral("Remove this (Ctrl+Shift+Del)"));
    m_buttonClose->setIconSize({12, 12});
    m_buttonClose->resize(20, 20);

    auto action = new QAction(QStringLiteral("Remove this"), this);
    action->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    action->setShortcut(QKeySequence(QStringLiteral("Ctrl+Shift+Delete")));

    addAction(action);

    connect(action, &QAction::triggered, this, &QWidget::close);
    connect(m_buttonClose, &QPushButton::clicked, action, &QAction::trigger);

    for (auto widget : findChildren<AbstractStringBuilderWidget *>()) {
        connect(widget, &AbstractStringBuilderWidget::changeStarted
              , this, &FormStringBuilder::changeStarted);
    }

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

void FormStringBuilder::enterEvent(QEnterEvent *)
{
    m_buttonClose->setVisible(true);
}

void FormStringBuilder::leaveEvent(QEvent *)
{
    m_buttonClose->setVisible(false);
}

void FormStringBuilder::resizeEvent(QResizeEvent *event)
{
    m_buttonClose->move(event->size().width() - 4 - 20, 2);
}
