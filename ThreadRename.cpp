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

#include "ThreadRename.h"

#include "Path/PathRoot.h"
#include "Path/PathEntity.h"

ThreadRename::ThreadRename(QWeakPointer<Path::PathRoot> pathRoot, QObject *parent)
    : QThread(parent)
    , m_pathRoot(pathRoot)
{
    Q_ASSERT(m_pathRoot != nullptr);
}

void ThreadRename::pause()
{
    QWriteLocker locker(&m_lockForPause);

    m_isPauseRequested = true;
}

void ThreadRename::stop()
{
    QWriteLocker locker(&m_lock);

    m_isStopRequested = true;
}

void ThreadRename::run()
{
    m_lock.lockForWrite();
    m_isStopRequested = false;
    m_lock.unlock();

    QList<EntityToIndex> dirs;
    QList<EntityToIndex> files;

    QSharedPointer<Path::PathRoot> root = m_pathRoot.lock();

    for (qsizetype i = 0, count = root->entityCount(); i < count; ++i) {
        QSharedPointer<Path::PathEntity> entity = root->entity(i);

        entity->isDir() ? dirs  << EntityToIndex(entity, i)
                        : files << EntityToIndex(entity, i);
    }

    renameEntities(files);

    if (isStopRequested())
        return;

    std::sort(dirs.begin(), dirs.end(), [](const EntityToIndex &lhs, const EntityToIndex &rhs) {
        return lhs.first->fullPath().count('/') > rhs.first->fullPath().count('/');
    });

    renameEntities(dirs);
}

bool ThreadRename::isStopRequested() const
{
    QReadLocker locker(&m_lock);

    return m_isStopRequested;
}

void ThreadRename::renameEntities(const QList<EntityToIndex> &entityToIndexList)
{
    for (const EntityToIndex &entityToIndex : entityToIndexList) {
        entityToIndex.first->rename();

        emit renamed(entityToIndex.second);

        m_lockForPause.lockForRead();

        if (m_isPauseRequested)
            m_waitConditionPause.wait(&m_lockForPause);

        m_lockForPause.unlock();


        if (isStopRequested())
            return;
    }
}
