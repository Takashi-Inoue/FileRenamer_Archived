/*
 * Copyright 2021 Takashi Inoue
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

#include "PathEntity.h"
#include "ParentDir.h"

#include "ApplicationLog/ApplicationLog.h"

#include <QDir>
#include <QFileIconProvider>

namespace Path {

PathEntity::PathEntity(QWeakPointer<ParentDir> parent, QStringView name, bool isDir)
    : m_isDir(isDir)
    , m_parent(parent)
    , m_name(name.toString())
{
    static QFileIconProvider iconProvider; // static for one-time creation.

    Q_ASSERT(parent != nullptr);
    Q_ASSERT(!name.isEmpty());

    m_fileIcon = iconProvider.icon(QFileInfo(fullPath()));
}

bool PathEntity::isDir() const
{
    return m_isDir;
}

QString PathEntity::fullPath() const
{
    QReadLocker locker(&m_lock);

    return QStringLiteral("%1%2").arg(m_parent.lock()->path(), m_name);
}

QString PathEntity::parentPath() const
{
    return m_parent.lock()->path();
}

QString PathEntity::name() const
{
    QReadLocker locker(&m_lock);

    return m_name;
}

QString PathEntity::newName() const
{
    QReadLocker locker(&m_lock);

    return m_newName;
}

QWeakPointer<ParentDir> PathEntity::parent() const
{
    return m_parent;
}

QString PathEntity::hashHex(QCryptographicHash::Algorithm algorithm) const
{
    return m_fileHashs[algorithm];
}

QString PathEntity::imageHash() const
{
    return m_imageHash;
}

void PathEntity::setHashHex(QCryptographicHash::Algorithm algorithm, QStringView hashHex)
{
    m_fileHashs[algorithm] = hashHex.toString();
}

void PathEntity::setImageHash(QStringView imageHash)
{
    m_imageHash = imageHash.toString();
}

void PathEntity::setNewName(QStringView newName)
{
    QWriteLocker locker(&m_lock);

    m_newName = m_isDir ? newName.toString()
                        : QStringLiteral("%1%2").arg(newName, m_name.mid(m_name.indexOf('.')));

    m_state = State::initial;
}

bool PathEntity::checkForNewNameCollisions(QSharedPointer<PathEntity> other)
{
    if (newName() == other->newName()) {
        setState(State::hasCollision);
        other->setState(State::hasCollision);

        return false;
    }

    if (state() == State::initial)
        setState(State::ready);

    if (other->state() == State::initial)
        other->setState(State::ready);

    return true;
}

void PathEntity::notNeedToCheckNewName()
{
    setState(State::ready);
}

QIcon PathEntity::stateIcon() const
{
    static const QHash<int, QIcon> icons = {
        {int(State::initial),      QIcon(QStringLiteral(":/res/images/circlegray.svg"))},
        {int(State::ready),        QIcon(QStringLiteral(":/res/images/circlegreen.svg"))},
        {int(State::hasCollision), QIcon(QStringLiteral(":/res/images/collision.svg"))},
        {int(State::success),      QIcon(QStringLiteral(":/res/images/success.svg"))},
        {int(State::failure),      QIcon(QStringLiteral(":/res/images/failure.svg"))},
    };

    QReadLocker locker(&m_lock);

    return icons[int(m_state)];
}

QIcon PathEntity::typeIcon() const
{
    return m_fileIcon;
}

QString PathEntity::statusText() const
{
    if (state() == State::initial)
        return name();

    if (state() == State::ready)
        return QStringLiteral("%1 -> %2").arg(fullPath(), newName());

    return "";
}

bool PathEntity::rename()
{
    if (state() == State::success)
        return true;

    if (state() != State::ready)
        return false;

    m_lock.lockForRead();

    QDir dir(m_parent.lock()->path());

    bool isOk = dir.rename(m_name, m_newName);

    QString result = isOk ? QStringLiteral("SUCCEEDED")
                          : QStringLiteral("---FAILED");

    QString log = QStringLiteral("%1 - %2/%3 > %4").arg(result, dir.absolutePath(), m_name, m_newName);

    ApplicationLog::instance().log(log, QStringLiteral("Rename"));

    m_lock.unlock();

    isOk ? setState(State::success)
         : setState(State::failure);

    return isOk;
}

bool PathEntity::undoRename()
{
    if (state() != State::success)
        return false;

    m_lock.lockForRead();

    QDir dir(m_parent.lock()->path());

    bool isOk = dir.rename(m_newName, m_name);

    QString result = isOk ? QStringLiteral("SUCCEEDED")
                          : QStringLiteral("---FAILED");

    QString log = QStringLiteral("%1 - %2/%3 > %4").arg(result, dir.absolutePath(), m_newName, m_name);

    ApplicationLog::instance().log(log, QStringLiteral("Undo"));

    m_lock.unlock();

    isOk ? setState(State::ready)
         : setState(State::success);

    return isOk;
}

void PathEntity::setState(PathEntity::State state)
{
    QWriteLocker locker(&m_lock);

    m_state = state;
}

PathEntity::State PathEntity::state() const
{
    QReadLocker locker(&m_lock);

    return m_state;
}

} // namespace Path

