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

#include "FormStringBuilderChain.h"
#include "ui_FormStringBuilderChain.h"

#include "FormStringBuilder.h"

#include <QApplication>
#include <QTimer>

FormStringBuilderChain::FormStringBuilderChain(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FormStringBuilderChain)
    , m_timer(new QTimer(this))
{
    ui->setupUi(this);

    m_timer->setSingleShot(true);
    m_timer->setInterval(QApplication::keyboardInputInterval());

    createNewSetting();

    connect(m_timer, &QTimer::timeout, this, [this]() {
        emit settingsChanged(builderChain());
    });
}

FormStringBuilderChain::~FormStringBuilderChain()
{
    int count = ui->vLayout->count();

    for (int i = 0; i < count; ++i) {
        QLayoutItem *item = ui->vLayout->itemAt(i);
        auto widget = qobject_cast<FormStringBuilder *>(item->widget());

        if (widget != nullptr) {
            disconnect(widget, &FormStringBuilder::destroyed
                     , this, &FormStringBuilderChain::startTimer);
        }
    }

    delete ui;
}

QSharedPointer<StringBuilderOnFile::BuilderChainOnFile> FormStringBuilderChain::builderChain() const
{
    auto builderChain = QSharedPointer<StringBuilderOnFile::BuilderChainOnFile>::create();

    int count = ui->vLayout->count();

    for (int i = 0; i < count; ++i) {
        QLayoutItem *item = ui->vLayout->itemAt(i);
        auto widget = qobject_cast<FormStringBuilder *>(item->widget());

        if (widget != nullptr)
            builderChain->addCreator(widget->stringBuilder());
    }

    return builderChain;
}

void FormStringBuilderChain::createNewSetting()
{
    auto widget = new FormStringBuilder(this);

    widget->setAttribute(Qt::WA_DeleteOnClose, true);

    connect(widget, &FormStringBuilder::changeStarted, this, &FormStringBuilderChain::startTimer);
    connect(widget, &FormStringBuilder::destroyed,     this, &FormStringBuilderChain::startTimer);

    ui->vLayout->insertWidget(ui->vLayout->count() - 2, widget);

    startTimer();
}

void FormStringBuilderChain::startTimer()
{
    if (findChildren<FormStringBuilder *>().isEmpty()) {
        emit builderCleared();
        return;
    }

    m_timer->start();
    emit changeStarted();
}
