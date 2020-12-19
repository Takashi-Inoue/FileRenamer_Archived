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

#ifndef PATHROOT_H
#define PATHROOT_H

#include <QReadWriteLock>
#include <QSharedPointer>
#include "QStringVector.h"

namespace Path {

class PathEntity;
class ParentDir;

class PathRoot final
{
    PathRoot(const PathRoot &) = delete;
    PathRoot(PathRoot &&) = delete;
    PathRoot &operator=(const PathRoot &) = delete;
    PathRoot &operator=(PathRoot &&) = delete;

public:
    using ParentChildrenPair = QPair<QString, QStringVector>;

    PathRoot() = default;
    ~PathRoot() = default;

    // Add / Remove Data
    void addDir(QSharedPointer<ParentDir> dir);
    void addPathsAsDirs(QVector<ParentChildrenPair> dirs);
    void addPathsAsFiles(QVector<ParentChildrenPair> files);
    void remove(int index, int count = 1);
    void removeSpecifiedRows(QVector<int> rows);

    QSharedPointer<ParentDir> dir(QStringView path) const;
    QSharedPointer<PathEntity> entity(int index) const;
    qsizetype entityCount() const;
    void sortByEntityName(Qt::SortOrder order);
    void sortByParentDir(Qt::SortOrder order);

private:
    enum class EntityType {dirs, files};

    void addPaths(const QVector<ParentChildrenPair> &paths, EntityType entityType);

    mutable QReadWriteLock m_lock;

    QVector<QSharedPointer<PathEntity>> m_entities;
    QVector<QSharedPointer<ParentDir>> m_dirs;
};

} // namespace Path

#endif // PATHROOT_H
