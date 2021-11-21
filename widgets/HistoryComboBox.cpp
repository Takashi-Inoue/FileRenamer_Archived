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

#include "HistoryComboBox.h"

namespace {
constexpr char settingsGroupName[] = "History";
}

QStringList HistoryComboBox::allItemsText() const
{
    QStringList texts;

    for (int i = 0, itemCount = count(); i < itemCount; ++i)
        texts << itemText(i);

    return texts;
}

void HistoryComboBox::addCurrentTextToItem()
{
    QString text = currentText();

    if (findText(text) == -1)
        addItem(currentText());
}

void HistoryComboBox::insertCurrentTextToItem(int index)
{
    QString text = currentText();

    if (findText(text) == -1)
        insertItem(index, currentText());
}

void HistoryComboBox::loadSettings(QSharedPointer<QSettings> qSettings, QString key)
{
    qSettings->beginGroup(settingsGroupName);
    addItems(qSettings->value(key).toStringList());
    qSettings->endGroup();
}

void HistoryComboBox::saveSettings(QSharedPointer<QSettings> qSettings, QString key)
{
    qSettings->beginGroup(settingsGroupName);
    qSettings->setValue(key, allItemsText());
    qSettings->endGroup();
}

void HistoryComboBox::loadSettings(QSharedPointer<QSettings> qSettings, QString subGroup, QString key)
{
    qSettings->beginGroup(settingsGroupName);
    qSettings->beginGroup(subGroup);
    addItems(qSettings->value(key).toStringList());
    qSettings->endGroup();
    qSettings->endGroup();
}

void HistoryComboBox::saveSettings(QSharedPointer<QSettings> qSettings, QString subGroup, QString key)
{
    qSettings->beginGroup(settingsGroupName);
    qSettings->beginGroup(subGroup);
    qSettings->setValue(key, allItemsText());
    qSettings->endGroup();
    qSettings->endGroup();
}
