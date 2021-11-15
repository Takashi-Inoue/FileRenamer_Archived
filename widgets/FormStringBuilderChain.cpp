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

#include "Application.h"
#include "FormStringBuilder.h"
#include "Settings/BuilderChainSettings.h"

#include <QTimer>

FormStringBuilderChain::FormStringBuilderChain(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FormStringBuilderChain)
    , m_timer(new QTimer(this))
{
    ui->setupUi(this);

    m_timer->setSingleShot(true);
    m_timer->setInterval(QApplication::keyboardInputInterval());

    connect(this, &FormStringBuilderChain::settingsIndicesChanged
          , this, &FormStringBuilderChain::onSettingsIndicesChanged);

    BuilderChainSettings settings;

    settings.read(Application::mainQSettings());

    for (int builderIndex : settings.indices())
        createNewSetting(builderIndex);

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

void FormStringBuilderChain::loadBuilderSettings(QSharedPointer<QSettings> qSettings)
{
    int count = ui->vLayout->count();

    for (int i = 0; i < count; ++i) {
        QLayoutItem *item = ui->vLayout->itemAt(i);
        auto widget = qobject_cast<FormStringBuilder *>(item->widget());

        if (widget != nullptr)
            widget->close();
    }

    BuilderChainSettings settings;

    settings.read(qSettings);

    for (int builderIndex : settings.indices())
        createNewSetting(builderIndex);

    for (auto formBuilder : findChildren<FormStringBuilder *>())
        formBuilder->loadBuilderSettings(qSettings);
}

void FormStringBuilderChain::saveCurrentBuilderSettings(QSharedPointer<QSettings> qSettings) const
{
    QList<int> builderIndices;

    for (auto formBuilder : findChildren<FormStringBuilder *>()) {
        formBuilder->saveCurrentBuilderSettings(qSettings);
        builderIndices << formBuilder->currentBuilderIndex();
    }

    BuilderChainSettings settings;

    settings.setValue(BuilderChainSettings::indicesEntry
                    , QVariant::fromValue<QList<int>>(builderIndices));

    settings.write(qSettings);
}

void FormStringBuilderChain::createNewSetting(int builderIndex)
{
    auto widget = new FormStringBuilder(this);

    widget->setCurrentBuilderIndex(builderIndex);
    widget->setAttribute(Qt::WA_DeleteOnClose, true);

    connect(widget, &FormStringBuilder::changeStarted, this, &FormStringBuilderChain::startTimer);
    connect(widget, &FormStringBuilder::destroyed, this, &FormStringBuilderChain::startTimer);
    connect(widget, &FormStringBuilder::requestDown, this, &FormStringBuilderChain::onBuilderRequestedDown);
    connect(widget, &FormStringBuilder::requestUp, this, &FormStringBuilderChain::onBuilderRequestedUp);
    connect(widget, &FormStringBuilder::requestRemove, this, &FormStringBuilderChain::onBuilderRequestedRemove);

    int settingCount = settingsCount();

    ui->vLayout->insertWidget(settingCount, widget);

    emit settingsIndicesChanged();

    startTimer();
}

void FormStringBuilderChain::startTimer()
{
    if (findChildren<FormStringBuilder *>().isEmpty())
        emit builderCleared();

    m_timer->start();
    emit changeStarted();
}

void FormStringBuilderChain::onBuilderRequestedDown()
{
    QWidget *widget = qobject_cast<QWidget *>(sender());

    int layoutIndex = ui->vLayout->indexOf(widget);

    if (layoutIndex >= ui->vLayout->count() - 3)
        return;

    ui->vLayout->removeWidget(widget);
    ui->vLayout->insertWidget(layoutIndex + 1, widget);

    emit settingsIndicesChanged();
}

void FormStringBuilderChain::onBuilderRequestedUp()
{
    QWidget *widget = qobject_cast<QWidget *>(sender());

    int layoutIndex = ui->vLayout->indexOf(widget);

    if (layoutIndex == 0)
        return;

    ui->vLayout->removeWidget(widget);
    ui->vLayout->insertWidget(layoutIndex - 1, widget);

    emit settingsIndicesChanged();
}

void FormStringBuilderChain::onBuilderRequestedRemove()
{
    auto widget = qobject_cast<QWidget *>(sender());

    if (widget != nullptr)
        ui->vLayout->removeWidget(widget);

    emit settingsIndicesChanged();
}

void FormStringBuilderChain::onSettingsIndicesChanged()
{
    for (int i = 0, count = settingsCount(); i < count; ++i) {
        QLayoutItem *item = ui->vLayout->itemAt(i);
        auto widget = qobject_cast<FormStringBuilder *>(item->widget());

        if (widget != nullptr)
            widget->notifySettingIndexChanged(i, count);
    }

    startTimer();
}

int FormStringBuilderChain::settingsCount() const
{
    return ui->vLayout->count() - 2; // vLayout has "Plus" button and Spacer item.
}
