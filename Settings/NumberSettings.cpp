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

#include "NumberSettings.h"


NumberSettings::NumberSettings()
    : AbstractSettings(
          QStringLiteral("Number")
        , QHash<int, QPair<QString, QVariant>>
          {
              {startEntry,    {QStringLiteral("Start"),    0}},
              {stepEntry,     {QStringLiteral("Step"),     1}},
              {digitEntry,    {QStringLiteral("Digit"),    1}},
              {prefixEntry,   {QStringLiteral("Prefix"),   QString("")}},
              {suffixEntry,   {QStringLiteral("Suffix"),   QString("")}},
              {positionEntry, {QStringLiteral("Position"), 0}},
          })
{
}

int NumberSettings::start() const
{
    return m_valuesHash[startEntry].second.toInt();
}

int NumberSettings::step() const
{
    return m_valuesHash[stepEntry].second.toInt();
}

int NumberSettings::digit() const
{
    return m_valuesHash[digitEntry].second.toInt();
}

QString NumberSettings::prefix() const
{
    return m_valuesHash[prefixEntry].second.toString();
}

QString NumberSettings::suffix() const
{
    return m_valuesHash[suffixEntry].second.toString();
}

int NumberSettings::position() const
{
    return m_valuesHash[positionEntry].second.toInt();
}
