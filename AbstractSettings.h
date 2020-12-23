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

#ifndef ABSTRACTSETTINGS_H
#define ABSTRACTSETTINGS_H

#include <QHash>
#include <QString>
#include <QVariant>

#include <QDebug>

class AbstractSettings
{
public:
    AbstractSettings(QStringView settingsName, QHash<int, QPair<QString, QVariant>> hash);
    virtual ~AbstractSettings() = default;

    void read();
    void write() const;

    void setValue(int entry, QVariant value);

    template<typename T>
    T value(int entry, T defaultValue) const
    {
        auto itr = m_valuesHash.find(entry);

        if (itr == m_valuesHash.end()) {
            qWarning() << "Settings:" << m_settingsName << "- Entry" << entry << "is not found.";
            return defaultValue;
        }

        QVariant val = itr->second;

        if (val.canConvert<T>()) {
            return val.value<T>();
        } else {
            qWarning() << "Settings:" << m_settingsName
                       << "- Value can't be converted to" << typeid(T).name();
        }

        return defaultValue;
    }

protected:
    const QString m_iniPath;
    const QString m_settingsName;

    QHash<int, QPair<QString, QVariant>> m_valuesHash;
};

#endif // ABSTRACTSETTINGS_H
