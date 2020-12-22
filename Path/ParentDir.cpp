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

#include "ParentDir.h"
#include "PathEntity.h"

#include <QCollator>

namespace Path {

ParentDir::ParentDir(QStringView path)
    : m_path(path.toString())
{
    Q_ASSERT(!path.isEmpty());
}

void ParentDir::addEntity(QSharedPointer<PathEntity> entity)
{
    QWriteLocker locker(&m_lock);

    m_children << entity;
}

void ParentDir::removeEntity(QWeakPointer<PathEntity> entity)
{
    QWriteLocker locker(&m_lock);

    m_children.removeOne(entity);
}

const QList<QSharedPointer<PathEntity>> &ParentDir::allEntities() const
{
    return m_children;
}

QSharedPointer<PathEntity> ParentDir::entity(int index) const
{
    Q_ASSERT(index < m_children.size());

    return m_children[index];
}

qsizetype ParentDir::entityCount() const
{
    return m_children.size();
}

QString ParentDir::path() const
{
    return m_path;
}

void ParentDir::sort(QCollator &collator, Qt::SortOrder order)
{
    QWriteLocker locker(&m_lock);

    using EntityPtr = QSharedPointer<PathEntity>;

    std::sort(m_children.begin(), m_children.end(), [&](const EntityPtr &lhs, const EntityPtr &rhs) {
        return order == Qt::AscendingOrder ? collator.compare(lhs->name(), rhs->name()) < 0
                                           : collator.compare(lhs->name(), rhs->name()) > 0;
    });
}

} // namespace Path
