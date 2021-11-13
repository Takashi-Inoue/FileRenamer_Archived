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

#include "AbstractSettings.h"

#include <QMetaEnum>

AbstractSettings::AbstractSettings(QStringView settingsName
                                 , QHash<int, QPair<QString, QVariant>> hash)
    :m_settingsName(settingsName.toString())
    , m_valuesHash(hash)
{
}

void AbstractSettings::read(QSharedPointer<QSettings> qSettings)
{
    qInfo() << "Settings:" << m_settingsName << "read using"
            << QMetaEnum::fromType<QSettings::Format>().valueToKey(qSettings->format());

    qSettings->beginGroup(m_settingsName);

    for (QPair<QString, QVariant> &keyValue : m_valuesHash) {
        keyValue.second = qSettings->value(keyValue.first, keyValue.second);
        qDebug() << "read..." << keyValue.first << ":" << keyValue.second;
    }

    qSettings->endGroup();
}

void AbstractSettings::write(QSharedPointer<QSettings> qSettings) const
{
    qInfo() << "Settings:" << m_settingsName << "write using"
            << QMetaEnum::fromType<QSettings::Format>().valueToKey(qSettings->format());

    qSettings->beginGroup(m_settingsName);

    for (const QPair<QString, QVariant> &keyValue : m_valuesHash) {
        qSettings->setValue(keyValue.first, keyValue.second);
        qDebug() << "wrote..." << keyValue.first << ":" << keyValue.second;
    }

    qSettings->endGroup();
}

void AbstractSettings::setValue(int entry, QVariant value)
{
    auto itr = m_valuesHash.find(entry);

    Q_ASSERT(itr != m_valuesHash.end());

    itr->second = value;
}
