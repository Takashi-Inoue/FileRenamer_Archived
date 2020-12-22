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

#ifndef PATHROOT_H
#define PATHROOT_H

#include <QReadWriteLock>
#include <QSharedPointer>

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
    using ParentChildrenPair = QPair<QString, QStringList>;

    PathRoot() = default;
    ~PathRoot() = default;

    // Add / Remove Data
    void addDir(QSharedPointer<ParentDir> dir);
    void addPathsAsDirs(QList<ParentChildrenPair> dirs);
    void addPathsAsFiles(QList<ParentChildrenPair> files);
    void remove(int index, int count = 1);
    void removeSpecifiedRows(QList<int> rows);

    QSharedPointer<ParentDir> dir(QStringView path) const;
    QSharedPointer<PathEntity> entity(qsizetype index) const;
    qsizetype entityCount() const;
    void sortByEntityName(Qt::SortOrder order);
    void sortByParentDir(Qt::SortOrder order);

private:
    enum class EntityType {dirs, files};

    void addPaths(const QList<ParentChildrenPair> &paths, EntityType entityType);

    mutable QReadWriteLock m_lock;

    QList<QSharedPointer<PathEntity>> m_entities;
    QList<QSharedPointer<ParentDir>> m_dirs;
};

} // namespace Path

#endif // PATHROOT_H
