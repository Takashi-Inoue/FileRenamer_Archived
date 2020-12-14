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

#include "PathsAnalyzer.h"

#include <QFileInfo>

void PathsAnalyzer::analyze(const QVector<QString> &paths)
{
    m_isAllDir = false;
    m_pathsMap.clear();

    int dirCount = 0;
    int itemCount = paths.size();

    if (itemCount == 0)
        return;

    for (const QString &path : paths) {
        QFileInfo fileInfo(path);

        if (fileInfo.isRelative() || !fileInfo.exists()) {
            --itemCount;
            continue;
        }

        if (fileInfo.isDir())
            ++dirCount;

        m_pathsMap[fileInfo.absolutePath()] << fileInfo.fileName();
    }

    m_isAllDir = ((itemCount != 0) & (dirCount == itemCount));
}

bool PathsAnalyzer::isAllDir() const
{
    return m_isAllDir;
}

const QMap<QString, QVector<QString>> &PathsAnalyzer::analyzedMap() const
{
    return m_pathsMap;
}
