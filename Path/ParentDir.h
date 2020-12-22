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

#ifndef PARENTDIR_H
#define PARENTDIR_H

#include <QReadWriteLock>
#include <QSharedPointer>
#include <QList>

namespace Path {

class PathEntity;

class ParentDir
{
public:
    ParentDir(QStringView path);

    // Add / Remove entity;
    void addEntity(QSharedPointer<PathEntity> entity);
    void clear();
    void removeEntity(QWeakPointer<PathEntity> entity);

    const QList<QSharedPointer<PathEntity>> &allEntities() const;
    QSharedPointer<PathEntity> entity(int index) const;
    qsizetype entityCount() const;
    QString path() const;
    void sort(QCollator &collator, Qt::SortOrder order);

private:
    mutable QReadWriteLock m_lock;

    const QString m_path;
    QList<QSharedPointer<PathEntity>> m_children;
};

} // namespace Path

#endif // PARENTDIR_H
