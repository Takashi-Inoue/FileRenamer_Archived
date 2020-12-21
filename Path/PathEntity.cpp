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

#include "PathEntity.h"
#include "ParentDir.h"

#include <QClipboard>
#include <QDir>
#include <QFileIconProvider>
#include <QGuiApplication>

namespace Path {

PathEntity::PathEntity(QWeakPointer<ParentDir> parent, QStringView name, bool isDir)
    : m_isDir(isDir)
    , m_parent(parent)
    , m_name(name.toString())
{
    Q_ASSERT(parent != nullptr);
    Q_ASSERT(!name.isEmpty());
}

void PathEntity::copyOriginalNameToClipboard() const
{
    QReadLocker locker(&m_lock);

    QGuiApplication::clipboard()->setText(m_name);
}

bool PathEntity::isDir() const
{
    return m_isDir;
}

QString PathEntity::fullPath() const
{
    QReadLocker locker(&m_lock);

    return QString("%1%2").arg(m_parent.lock()->path(), m_name);
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

void PathEntity::setHashHex(QCryptographicHash::Algorithm algorithm, QStringView hashHex)
{
    m_fileHashs[algorithm] = hashHex.toString();
}

void PathEntity::setNewName(QStringView newName)
{
    QWriteLocker locker(&m_lock);

    m_newName = m_isDir ? newName.toString()
                        : QString("%1%2").arg(newName, m_name.mid(m_name.indexOf('.')));

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
    static const QIcon icons[] = {
        QFileIconProvider().icon(QAbstractFileIconProvider::File),
        QFileIconProvider().icon(QAbstractFileIconProvider::Folder),
    };

    return icons[m_isDir];
}

bool PathEntity::rename()
{
    if (state() != State::ready)
        return false;

    m_lock.lockForRead();

    QDir dir(m_parent.lock()->path());

    bool isOk = dir.rename(m_name, m_newName);

    QString log = isOk ? "rename SUCCEEDED : "
                       : "rename ---FAILED : ";

//    Log::log(log + dir.absolutePath() + "/" + m_name + " > " + m_newName);

    m_lock.unlock();

    isOk ? setState(State::success)
         : setState(State::failure);

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

