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

#include "EntityName.h"
#include "ParentDir.h"

namespace Path {

EntityName::EntityName(QWeakPointer<ParentDir> parent, QStringView name)
    : m_parent(parent)
    , m_name(name.toString())
{
    Q_ASSERT(parent != nullptr);
    Q_ASSERT(!name.isEmpty());
}

QFileInfo EntityName::fileInfo() const
{
    return QFileInfo(path());
}

QStringView EntityName::name() const
{
    return m_name;
}

QString EntityName::path() const
{
    QReadLocker locker(&m_lock);

    return QString("%1/%2").arg(m_parent.lock()->path(), m_name);
}

} // namespace Path

