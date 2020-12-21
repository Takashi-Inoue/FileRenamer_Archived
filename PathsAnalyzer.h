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

#ifndef PATHSANALYZER_H
#define PATHSANALYZER_H

#include <QMap>

class PathsAnalyzer
{
public:
    using ParentChildrenPair = QPair<QString, QStringList>;

    PathsAnalyzer() = default;
    ~PathsAnalyzer() = default;

    void analyze(const QStringList &paths);

    QList<ParentChildrenPair> dirs() const;
    QList<ParentChildrenPair> files() const;

    bool isAllDir() const;

private:
    QList<ParentChildrenPair> m_dirs;
    QList<ParentChildrenPair> m_files;
};

#endif // PATHSANALYZER_H
