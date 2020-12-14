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

namespace Path {

class EntityName;
class ParentDir;

class PathRoot final
{
    PathRoot(const PathRoot &) = delete;
    PathRoot(PathRoot &&) = delete;
    PathRoot &operator=(const PathRoot &) = delete;
    PathRoot &operator=(PathRoot &&) = delete;

public:
    PathRoot() = default;
    ~PathRoot() = default;

    void addDir(QSharedPointer<ParentDir> dir);
    QSharedPointer<ParentDir> dir(QStringView path) const;
    QSharedPointer<EntityName> entity(int index) const;
    int entityCount() const;
    void sort(Qt::SortOrder order);

private:
    mutable QReadWriteLock m_lock;

    QVector<QSharedPointer<ParentDir>> m_dirs;
};

} // namespace Path

#endif // PATHROOT_H
