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
    if (isDir() && newName().isEmpty()) {
        if (state() != State::initial)
            setState(State::initial);

        return false;
    }

    if (newName() == other->newName()) {
        setState(State::sameNewName);
        other->setState(State::sameNewName);

        return false;
    }

    if (state() == State::initial)
        setState(State::ready);

    if (other->state() == State::initial)
        other->setState(State::ready);

    return true;
}

bool PathEntity::checkSelfNewName()
{
    const QString newname = newName();

    if (newname.isEmpty()/* || newname == name()*/) {
        if (state() != State::initial)
            setState(State::initial);

        return false;
    }

    setState(State::ready);

    return true;
}

QIcon PathEntity::stateIcon() const
{
    static const QHash<int, QIcon> icons = {
        {int(State::initial),     QIcon(QStringLiteral(":/res/images/circlegray.svg"))},
        {int(State::ready),       QIcon(QStringLiteral(":/res/images/circlegreen.svg"))},
        {int(State::sameNewName), QIcon(QStringLiteral(":/res/images/collision.svg"))},
        {int(State::success),     QIcon(QStringLiteral(":/res/images/success.svg"))},
        {int(State::failure),     QIcon(QStringLiteral(":/res/images/failure.svg"))},
    };

    QReadLocker locker(&m_lock);

    return icons[int(m_state)];
}

QString PathEntity::stateText() const
{
    static const QHash<int, QString> texts = {
        {int(State::initial),     QStringLiteral("Waiting")},
        {int(State::ready),       QStringLiteral("Ready")},
        {int(State::sameNewName), QStringLiteral("Same new name")},
        {int(State::success),     QStringLiteral("Succeeded")},
        {int(State::failure),     QStringLiteral("Failed")},
    };

    QReadLocker locker(&m_lock);

    return texts[int(m_state)];
}

QIcon PathEntity::typeIcon() const
{
    return m_fileIcon;
}

QString PathEntity::statusText() const
{
    switch (state()) {
    case State::initial:
        return fullPath();

    case State::ready:
        return QStringLiteral("%1 -> %2").arg(fullPath(), newName());

    case State::sameNewName:
        return QStringLiteral("New name <b>%1</b> is duplicated.").arg(newName());

    case State::success:
        return QStringLiteral("New path: %1%2").arg(parentPath(), newName());

    case State::failure:
        if (m_errorCode == ErrorCode::alreadyExist)
            return QStringLiteral("<b>%1%2</b> already exeists.").arg(parentPath(), newName());

        if (m_errorCode == ErrorCode::sourceNotFound)
            return QStringLiteral("<b>%1</b> does not exeists.").arg(fullPath());

        if (m_errorCode == ErrorCode::unknown)
            return QStringLiteral("Unknown error occered. Please confirm that file is not opened.");
    }

    return QString();
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

    QString log = QStringLiteral("%1 - %2/%3 -> %4").arg(result, dir.absolutePath(), m_name, m_newName);

    ApplicationLog::instance().log(log, QStringLiteral("Rename"));

    m_lock.unlock();

    if (isOk) {
        setState(State::success);
    } else {
        setState(State::failure);
        findErrorCause();
    }

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

    QString log = QStringLiteral("%1 - %2/%3 -> %4").arg(result, dir.absolutePath(), m_newName, m_name);

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

void PathEntity::findErrorCause()
{
    Q_ASSERT(state() == State::failure);

    ErrorCode errorCode = ErrorCode::unknown;

    if (!QFileInfo::exists(fullPath()))
        errorCode = ErrorCode::sourceNotFound;
    else if (QFileInfo::exists(QStringLiteral("%1%2").arg(parentPath(), newName())))
        errorCode = ErrorCode::alreadyExist;

    QWriteLocker locker(&m_lock);

    m_errorCode = errorCode;
}

} // namespace Path

