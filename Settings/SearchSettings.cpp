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

#include "SearchSettings.h"


SearchSettings::SearchSettings()
    : AbstractSettings(
          QStringLiteral("Search")
        , QHash<int, QPair<QString, QVariant>>
          {
              {searchFiles,   {QStringLiteral("SearchFiles"),   true}},
              {searchDirs,    {QStringLiteral("SearchDirs"),    false}},
              {filter,        {QStringLiteral("Filter"),        QString("")}},
              {filterHistory, {QStringLiteral("FilterHistory"), QStringList()}},
              {hierarchy,     {QStringLiteral("Hierarchy"),     0}},
          })
{
}

QStringList SearchSettings::filtersHistory() const
{
    return m_valuesHash[filterHistory].second.toStringList();
}

QStringList SearchSettings::filters() const
{
    return m_valuesHash[filter].second.toString().split(';', Qt::SkipEmptyParts);
}

int SearchSettings::searchHierarchy() const
{
    return m_valuesHash[hierarchy].second.toInt();
}

bool SearchSettings::isSearchDirs() const
{
    return m_valuesHash[searchDirs].second.toBool();
}

bool SearchSettings::isSearchFiles() const
{
    return m_valuesHash[searchFiles].second.toBool();
}
