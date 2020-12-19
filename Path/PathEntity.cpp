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

QFileInfo PathEntity::fileInfo() const
{
    return QFileInfo(fullPath());
}

QString PathEntity::fullPath() const
{
    QReadLocker locker(&m_lock);

    return QString("%1%2").arg(m_parent.lock()->path(), m_name);
}

QString PathEntity::parentPath() const
{
    return m_parent.lock()->path().toString();
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
    return m_hashs[algorithm];
}

void PathEntity::setHashHex(QCryptographicHash::Algorithm algorithm, QStringView hashHex)
{
    m_hashs[algorithm] = hashHex.toString();
}

void PathEntity::setNewName(QStringView newName)
{
    QWriteLocker locker(&m_lock);

    m_newName = m_isDir ? newName.toString()
                        : QString("%1%2").arg(newName, m_name.mid(m_name.indexOf('.')));
}

} // namespace Path

