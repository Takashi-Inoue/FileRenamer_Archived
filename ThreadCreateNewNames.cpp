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

    QSharedPointer<Path::PathRoot> root = m_pathRoot.lock();

    for (int i = 0, count = int(root->entityCount()); i < count; ++i) {
        QSharedPointer<Path::PathEntity>entity = root->entity(i);

        locker.relock();

        m_builderChain->setFileInfo(QSharedPointer<Path::PathEntityInfo>::create(entity));
        entity->setNewName(m_builderChain->build());

        locker.unlock();

        emit newNameCreated(i);

        if (isStopRequested())
            return;
    }
}

bool ThreadCreateNewNames::isStopRequested() const
{
    QReadLocker locker(&m_lock);

    return m_isStopRequested;
}
