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

#include "PathsAnalyzer.h"

#include <QFileInfo>

void PathsAnalyzer::analyze(const QStringList &paths)
{
    m_dirs.clear();
    m_files.clear();

    if (paths.size() == 0)
        return;

    for (const QString &path : paths) {
        QFileInfo fileInfo(path);

        if (fileInfo.isRoot() || fileInfo.isRelative() || !fileInfo.exists())
            continue;

        QList<ParentChildrenPair> &paths = fileInfo.isDir() ? m_dirs : m_files;
        QString parentDir = fileInfo.absolutePath();

        if (!parentDir.endsWith('/'))
            parentDir += '/';

        auto itr = std::find_if(paths.begin(), paths.end(), [&](ParentChildrenPair &path) {
            return path.first == parentDir;
        });

        if (itr == paths.end())
            itr = paths.insert(itr, ParentChildrenPair(parentDir, QStringList()));

        itr->second << fileInfo.fileName();
    }
}

QList<PathsAnalyzer::ParentChildrenPair> PathsAnalyzer::dirs() const
{
    return m_dirs;
}

QList<PathsAnalyzer::ParentChildrenPair> PathsAnalyzer::files() const
{
    return m_files;
}

bool PathsAnalyzer::isAllDir() const
{
    return !m_dirs.isEmpty() & m_files.isEmpty();
}
