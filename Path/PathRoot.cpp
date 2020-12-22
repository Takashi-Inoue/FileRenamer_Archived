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

#include "PathRoot.h"
#include "ParentDir.h"
#include "PathEntity.h"

#include <QCollator>

namespace Path {

void PathRoot::addDir(QSharedPointer<ParentDir> dir)
{
    Q_ASSERT(!m_dirs.contains(dir));

    QWriteLocker locker(&m_lock);
    m_dirs << dir;
}

void PathRoot::addPathsAsDirs(QList<PathRoot::ParentChildrenPair> dirs)
{
    addPaths(dirs, EntityType::dirs);
}

void PathRoot::addPathsAsFiles(QList<PathRoot::ParentChildrenPair> files)
{
    addPaths(files, EntityType::files);
}

void PathRoot::remove(int index, int count)
{
    QWriteLocker locker(&m_lock);

    for (int i = 0; i < count; ++i) {
        QSharedPointer<PathEntity> entity = m_entities.takeAt(index);

        entity->parent().lock()->removeEntity(entity);
    }
}

void PathRoot::removeSpecifiedRows(QList<int> rows)
{
    std::sort(rows.begin(), rows.end(), std::greater<int>());

    QWriteLocker locker(&m_lock);

    for (int index : rows) {
        QSharedPointer<PathEntity> entity = m_entities.takeAt(index);

        entity->parent().lock()->removeEntity(entity);
    }
}

QSharedPointer<ParentDir> PathRoot::dir(QStringView path) const
{
    auto itr = std::find_if(m_dirs.begin(), m_dirs.end(), [&](const QSharedPointer<ParentDir> &dir) {
        return dir->path() == path;
    });

    if (itr == m_dirs.end())
        return nullptr;

    return *itr;
}

QSharedPointer<PathEntity> PathRoot::entity(qsizetype index) const
{
    Q_ASSERT(uint(index) < uint(m_entities.size()));

    return m_entities[index];
}

qsizetype PathRoot::entityCount() const
{
    return m_entities.size();
}

bool PathRoot::isEmpty() const
{
    return m_entities.size() == 0;
}

void PathRoot::sortByEntityName(Qt::SortOrder order)
{
    QWriteLocker locker(&m_lock);

    QCollator collator;
    collator.setNumericMode(true);

    for (QSharedPointer<ParentDir> &dir : m_dirs)
        dir->sort(collator, order);

    m_entities.clear();

    for (const QSharedPointer<ParentDir> &dir : m_dirs)
        m_entities << dir->allEntities();
}

void PathRoot::sortByParentDir(Qt::SortOrder order)
{
    QWriteLocker locker(&m_lock);

    QCollator collator;
    collator.setNumericMode(true);

    using DirPtr = QSharedPointer<ParentDir>;

    std::sort(m_dirs.begin(), m_dirs.end(), [&](const DirPtr &lhs, const DirPtr &rhs) {
        return order == Qt::AscendingOrder ? collator.compare(lhs->path(), rhs->path()) < 0
                                           : collator.compare(lhs->path(), rhs->path()) > 0;
    });

    m_entities.clear();

    for (const DirPtr &dir : m_dirs)
        m_entities << dir->allEntities();
}

void PathRoot::addPaths(const QList<ParentChildrenPair> &paths, EntityType entityType)
{
    for (const ParentChildrenPair &path : paths) {
        QSharedPointer<ParentDir> parentDir = dir(path.first);

        if (parentDir == nullptr) {
            parentDir = QSharedPointer<ParentDir>::create(path.first);
            addDir(parentDir);
        }

        QWriteLocker locker(&m_lock);

        for (QStringView name : path.second) {
            auto entity = QSharedPointer<PathEntity>::create(
                              parentDir, name, entityType == EntityType::dirs);
            parentDir->addEntity(entity);

            m_entities << entity;
        }
    }
}

} // namespace Path
