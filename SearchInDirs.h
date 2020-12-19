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

#ifndef SEARCHINDIRS_H
#define SEARCHINDIRS_H

#include "Settings/SearchSettings.h"

#include <QDir>
#include "QStringVector.h"

class SearchInDirs
{
public:
    using ParentChildrenPair = QPair<QString, QStringVector>;

    SearchInDirs(const SearchSettings &settings);

    QVector<ParentChildrenPair> dirs() const;
    QVector<ParentChildrenPair> files() const;

    void exec(QVector<ParentChildrenPair> targetDirs);

private:
    void searchForDirs(const QDir &parentDir, QVector<ParentChildrenPair> &targetDirs);
    void searchForFiles(const QDir &parentDir);
    void searchOneLayer(QVector<ParentChildrenPair> &targetDirs, const QStringList &nameFilters);
    QString addSeparator(QStringView dirPath);

    const SearchSettings m_settings;

    QVector<ParentChildrenPair> m_dirs;
    QVector<ParentChildrenPair> m_files;
};

#endif // SEARCHINDIRS_H
