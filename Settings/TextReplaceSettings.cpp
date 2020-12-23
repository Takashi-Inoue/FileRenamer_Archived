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

#include "TextReplaceSettings.h"

TextReplaceSettings::TextReplaceSettings()
    : AbstractSettings(
          QStringLiteral("TextReplace")
        , QHash<int, QPair<QString, QVariant>>
          {
              {beforeEntry,        {QStringLiteral("Before"),        QString("")}},
              {afterEntry,         {QStringLiteral("After"),         QString("")}},
              {useRegexpEntry,     {QStringLiteral("UseRegexp"),     false}},
              {caseSensitiveEntry, {QStringLiteral("CaseSensitive"), true}},
          })
{
}

QString TextReplaceSettings::before() const
{
    return m_valuesHash[beforeEntry].second.toString();
}

QString TextReplaceSettings::after() const
{
    return m_valuesHash[afterEntry].second.toString();
}

bool TextReplaceSettings::isUseRegexp() const
{
    return m_valuesHash[useRegexpEntry].second.toBool();
}

bool TextReplaceSettings::isCaseSensitive() const
{
    return m_valuesHash[caseSensitiveEntry].second.toBool();
}
