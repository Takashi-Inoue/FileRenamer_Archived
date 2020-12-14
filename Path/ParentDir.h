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

#ifndef PARENTDIR_H
#define PARENTDIR_H

#include <QReadWriteLock>
#include <QSharedPointer>
#include <QVector>

namespace Path {

class EntityName;

class ParentDir
{
public:
    ParentDir(QStringView path);

    void addChild(QSharedPointer<EntityName> child);
    QSharedPointer<EntityName> entity(int index) const;
    int entityCount() const;
    QStringView path() const;
    void sort(QCollator &collator, Qt::SortOrder order);

private:
    mutable QReadWriteLock m_lock;

    const QString m_path;
    QVector<QSharedPointer<EntityName>> m_children;
};

} // namespace Path

#endif // PARENTDIR_H
