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

#include "SearchInDirs.h"

#include <QDir>
#include <QRegularExpression>

SearchInDirs::SearchInDirs(const SearchSettings &settings)
    : m_settings(settings)
{
}

QVector<SearchInDirs::ParentChildrenPair> SearchInDirs::exec(QVector<ParentChildrenPair> targetDirs)
{
    m_resultPaths.clear();

    const QStringList nameFilters = m_settings.filters();
    int hierarchy = m_settings.searchHierarchy();

    while (!targetDirs.isEmpty()) {
        searchOneLayer(targetDirs, nameFilters);

        if (--hierarchy == 0)
            break;
    }

    return m_resultPaths;
}

void SearchInDirs::searchForDirs(const QDir &parentDir
                               , QVector<SearchInDirs::ParentChildrenPair> &targetDirs)
{
    QStringList childrenNames = parentDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Hidden);

    if (childrenNames.isEmpty())
        return;

    targetDirs << ParentChildrenPair(parentDir.path(), {childrenNames.begin(), childrenNames.end()});

    if (!m_settings.isSearchDirs())
        return;

    if (parentDir.nameFilters().isEmpty()) {
        m_resultPaths << targetDirs.last();
        return;
    }

    QStringList patterns = parentDir.nameFilters();

    for (QString &pattern : patterns)
        pattern = QRegularExpression::wildcardToRegularExpression(pattern);

    QRegularExpression regexp(patterns.join('|'));

    childrenNames = childrenNames.filter(regexp);

    if (childrenNames.isEmpty())
        return;

    m_resultPaths << ParentChildrenPair(parentDir.path()
                                      , QStringVector(childrenNames.begin(), childrenNames.end()));
}

void SearchInDirs::searchForFiles(const QDir &parentDir)
{
    if (!m_settings.isSearchFiles())
        return;

    QStringList childrenNames = parentDir.entryList(QDir::Files | QDir::Hidden);

    if (childrenNames.isEmpty())
        return;

    m_resultPaths << ParentChildrenPair(parentDir.path()
                                      , QStringVector(childrenNames.begin(), childrenNames.end()));
}

void SearchInDirs::searchOneLayer(QVector<ParentChildrenPair> &targetDirs
                                , const QStringList &nameFilters)
{
    for (int i = 0, count = targetDirs.size(); i < count; ++i) {
        const ParentChildrenPair &parentChildren = targetDirs.takeFirst();

        for (QStringView childName : parentChildren.second) {
            QDir targetDir(QString("%1/%2").arg(parentChildren.first, childName));

            targetDir.setNameFilters(nameFilters);

            searchForFiles(targetDir);
            searchForDirs(targetDir, targetDirs);
        }
    }
}
