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

#include <QApplication>
#include <QSettings>

AbstractSettings::AbstractSettings(QStringView settingsName
                                 , QHash<int, QPair<QString, QVariant>> hash)
    : m_iniPath(QApplication::applicationFilePath().replace(".exe", ".ini"))
    , m_settingsName(settingsName.toString())
    , m_valuesHash(hash)
{
}

void AbstractSettings::read()
{
    qInfo() << "Settings:" << m_settingsName << "read from ini.";

    QSettings settings(m_iniPath, QSettings::IniFormat);

    settings.beginGroup(m_settingsName);

    for (QPair<QString, QVariant> &keyValue : m_valuesHash) {
        keyValue.second = settings.value(keyValue.first, keyValue.second);
        qDebug() << "read..." << keyValue.first << ":" << keyValue.second;
    }

    settings.endGroup();
}

void AbstractSettings::write() const
{
    qInfo() << "Settings:" << m_settingsName << "write to ini.";

    QSettings settings(m_iniPath, QSettings::IniFormat);

    settings.beginGroup(m_settingsName);

    for (const QPair<QString, QVariant> &keyValue : m_valuesHash) {
        settings.setValue(keyValue.first, keyValue.second);
        qDebug() << "wrote..." << keyValue.first << ":" << keyValue.second;
    }

    settings.endGroup();
}

void AbstractSettings::setValue(int entry, QVariant value)
{
    auto itr = m_valuesHash.find(entry);

    Q_ASSERT(itr != m_valuesHash.end());

    itr->second = value;
}
