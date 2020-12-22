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

#include "SearchInDirs.h"

#include <QDir>
#include <QRegularExpression>

SearchInDirs::SearchInDirs(const SearchSettings &settings)
    : m_settings(settings)
{
}

QList<SearchInDirs::ParentChildrenPair> SearchInDirs::dirs() const
{
    return m_dirs;
}

QList<SearchInDirs::ParentChildrenPair> SearchInDirs::files() const
{
    return m_files;
}

void SearchInDirs::exec(QList<ParentChildrenPair> targetDirs)
{
    m_dirs.clear();
    m_files.clear();

    const QStringList nameFilters = m_settings.filters();
    int hierarchy = m_settings.searchHierarchy();

    while (!targetDirs.isEmpty()) {
        searchOneLayer(targetDirs, nameFilters);

        if (--hierarchy == 0)
            break;
    }
}

void SearchInDirs::searchForDirs(const QDir &parentDir
                               , QList<SearchInDirs::ParentChildrenPair> &targetDirs)
{
    QStringList childrenNames = parentDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Hidden);

    if (childrenNames.isEmpty())
        return;

    targetDirs << ParentChildrenPair(addSeparator(parentDir.path())
                                   , {childrenNames.begin(), childrenNames.end()});

    if (!m_settings.isSearchDirs())
        return;

    if (parentDir.nameFilters().isEmpty()) {
        m_dirs << targetDirs.last();
        return;
    }

    QStringList patterns = parentDir.nameFilters();

    for (QString &pattern : patterns)
        pattern = QRegularExpression::wildcardToRegularExpression(pattern);

    QRegularExpression regexp(patterns.join('|'));

    childrenNames = childrenNames.filter(regexp);

    if (childrenNames.isEmpty())
        return;

    m_dirs << ParentChildrenPair(addSeparator(parentDir.path())
                               , QStringList(childrenNames.begin(), childrenNames.end()));
}

void SearchInDirs::searchForFiles(const QDir &parentDir)
{
    if (!m_settings.isSearchFiles())
        return;

    QStringList childrenNames = parentDir.entryList(QDir::Files | QDir::Hidden);

    if (childrenNames.isEmpty())
        return;

    m_files << ParentChildrenPair(addSeparator(parentDir.path())
                                , QStringList(childrenNames.begin(), childrenNames.end()));
}

void SearchInDirs::searchOneLayer(QList<ParentChildrenPair> &targetDirs
                                , const QStringList &nameFilters)
{
    for (int i = 0, count = int(targetDirs.size()); i < count; ++i) {
        const ParentChildrenPair &parentChildren = targetDirs.takeFirst();

        for (QStringView childName : parentChildren.second) {
            QDir targetDir(QString("%1%2").arg(parentChildren.first, childName));

            targetDir.setNameFilters(nameFilters);

            searchForFiles(targetDir);
            searchForDirs(targetDir, targetDirs);
        }
    }
}

QString SearchInDirs::addSeparator(QStringView dirPath)
{
    if (dirPath.endsWith('/'))
        return dirPath.toString();

    return QString("%1/").arg(dirPath);
}
