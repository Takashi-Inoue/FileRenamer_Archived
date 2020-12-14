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

#include "FormTextFormatSetting.h"
#include "ui_FormTextFormatSetting.h"

#include <QPushButton>
#include <QResizeEvent>
#include <QDebug>

FormTextFormatSetting::FormTextFormatSetting(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::FormTextFormatSetting)
    , m_buttonClose(new QPushButton(QIcon(":/res/images/x.svg"), QString(), this))
{
    ui->setupUi(this);

    m_buttonClose->setVisible(false);
    m_buttonClose->raise();
    m_buttonClose->setToolTip(QStringLiteral("Remove this"));
    m_buttonClose->setIconSize({12, 12});
    m_buttonClose->resize(20, 20);

    connect(m_buttonClose, &QPushButton::clicked, this, &QWidget::close);
}

FormTextFormatSetting::~FormTextFormatSetting()
{
    delete ui;
}

QSharedPointer<StringBuilder::AbstractStringBuilder> FormTextFormatSetting::StringBuilder() const
{
    auto widget = qobject_cast<AbstractStringBuilderWidget *>(ui->stackedWidget->currentWidget());

    Q_ASSERT(widget != nullptr);

    return widget->StringBuilder();
}

void FormTextFormatSetting::enterEvent(QEvent *)
{
    m_buttonClose->setVisible(true);
}

void FormTextFormatSetting::leaveEvent(QEvent *)
{
    m_buttonClose->setVisible(false);
}

void FormTextFormatSetting::resizeEvent(QResizeEvent *event)
{
    m_buttonClose->move(event->size().width() - 4 - 20, 2);
}
