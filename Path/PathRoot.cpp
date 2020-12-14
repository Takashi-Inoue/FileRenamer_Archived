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

#include "PathRoot.h"
#include "ParentDir.h"

#include <QCollator>

namespace Path {

void PathRoot::addDir(QSharedPointer<ParentDir> dir)
{
    Q_ASSERT(m_dirs.contains(dir));

    m_dirs << dir;
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

QSharedPointer<EntityName> PathRoot::entity(int index) const
{
    Q_ASSERT(index >= 0);

    for (const QSharedPointer<ParentDir> &dir : m_dirs) {
        const int entityCount = dir->entityCount();

        if (index >= entityCount)
            index -= entityCount;
        else
            return dir->entity(index);
    }

    return nullptr;
}

int PathRoot::entityCount() const
{
    int count = 0;

    for (const QSharedPointer<ParentDir> &dir : m_dirs)
        count += dir->entityCount();

    return count;
}

void PathRoot::sort(Qt::SortOrder order)
{
    QWriteLocker locker(&m_lock);

    QCollator collator;
    collator.setNumericMode(true);

    using DirPtr = QSharedPointer<ParentDir>;

    std::sort(m_dirs.begin(), m_dirs.end(), [&](const DirPtr &lhs, const DirPtr &rhs) {
        return order == Qt::AscendingOrder ? collator.compare(lhs->path(), rhs->path()) < 0
                                           : collator.compare(lhs->path(), rhs->path()) > 0;
    });

    for (DirPtr &dir : m_dirs)
        dir->sort(collator, order);
}

} // namespace Path
