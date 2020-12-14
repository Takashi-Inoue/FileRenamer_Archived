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

#ifndef ENTITYNAME_H
#define ENTITYNAME_H

#include <QFileInfo>
#include <QReadWriteLock>
#include <QSharedPointer>
#include <QString>

namespace Path {

class ParentDir;

class EntityName
{
public:
    EntityName(QWeakPointer<ParentDir> parent, QStringView name);

    QFileInfo fileInfo() const;
    QStringView name() const;
    QString path() const;

private:
    mutable QReadWriteLock m_lock;

    QWeakPointer<ParentDir> m_parent;
    QString m_name;
};

} // namespace Path

#endif // ENTITYNAME_H
