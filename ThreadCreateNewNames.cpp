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

#include "ThreadCreateNewNames.h"

#include "Path/PathRoot.h"
#include "Path/PathEntity.h"
#include "Path/PathEntityInfo.h"
#include "StringBuilderOnFile/BuilderChainOnFile.h"

ThreadCreateNewNames::ThreadCreateNewNames(QWeakPointer<Path::PathRoot> pathRoot, QObject *parent)
    : QThread(parent)
    , m_pathRoot(pathRoot)
{
    Q_ASSERT(m_pathRoot != nullptr);
}

void ThreadCreateNewNames::setStringBuilderOnFile(
        QSharedPointer<StringBuilderOnFile::BuilderChainOnFile> builderChain)
{
    QWriteLocker locker(&m_lock);

    if (m_builderChain != builderChain)
        m_builderChain = builderChain;
}

void ThreadCreateNewNames::stop()
{
    QWriteLocker locker(&m_lock);

    m_isStopRequested = true;
}

void ThreadCreateNewNames::run()
{
    QReadLocker locker(&m_lock);

    if(m_builderChain == nullptr)
        return;

    locker.unlock();

    m_lock.lockForWrite();
    m_isStopRequested = false;
    m_lock.unlock();

    HashToCheckEntities hashToCheckNames;

    if (!createNewNames(hashToCheckNames))
        return;

    if (!checkNewNames(hashToCheckNames))
        return;

    if (m_builderChain->isEmpty())
        return;

    emit completed();
}

bool ThreadCreateNewNames::isStopRequested() const
{
    QReadLocker locker(&m_lock);

    return m_isStopRequested;
}

bool ThreadCreateNewNames::checkNewNames(HashToCheckEntities &hashToCheckNames)
{
    bool isOk = true;

    for (QList<EntityToIndex> &entityToIndexList : hashToCheckNames.values()) {
        std::sort(entityToIndexList.begin(), entityToIndexList.end()
                , [](const EntityToIndex &lhs, const EntityToIndex &rhs)
        {
            return lhs.first->newName() < rhs.first->newName();
        });

        const qsizetype count = entityToIndexList.size();

        if (count == 1)
            entityToIndexList[0].first->notNeedToCheckNewName();

        for (qsizetype i = 0; i < count - 1; ++i) {
            EntityToIndex &lhs = entityToIndexList[i];
            EntityToIndex &rhs = entityToIndexList[i + 1];

            if (!lhs.first->checkForNewNameCollisions(rhs.first)) {
                isOk = false;
                emit newNameCollisionDetected({lhs.second, rhs.second});
            }

            if (isStopRequested())
                return false;
        }
    }

    if (isOk)
        emit newNameCollisionNotDetected();

    return true;
}

bool ThreadCreateNewNames::createNewNames(HashToCheckEntities &hashToCheckNames)
{
    QSharedPointer<Path::PathRoot> root = m_pathRoot.lock();

    for (int i = 0, count = int(root->entityCount()); i < count; ++i) {
        createOneNewName({root->entity(i), i}, hashToCheckNames);

        if (isStopRequested())
            return false;
    }

    return true;
}

void ThreadCreateNewNames::createOneNewName(EntityToIndex entityToIndex
                                          , HashToCheckEntities &hashToCheckNames)
{
    QSharedPointer<Path::PathEntity> &entity = entityToIndex.first;

    m_lock.lockForRead();

    m_builderChain->setFileInfo(QSharedPointer<Path::PathEntityInfo>::create(entity));
    entity->setNewName(m_builderChain->build());

    hashToCheckNames[quintptr(entity->parent().lock().get())] << entityToIndex;

    m_lock.unlock();

    emit newNameCreated(entityToIndex.second);
}
