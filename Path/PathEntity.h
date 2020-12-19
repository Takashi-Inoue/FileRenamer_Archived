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

#ifndef PATHENTITY_H
#define PATHENTITY_H

#include <QCryptographicHash>
#include <QFileInfo>
#include <QReadWriteLock>
#include <QSharedPointer>
#include <QString>

namespace Path {

class ParentDir;

class PathEntity
{
public:
    PathEntity(QWeakPointer<ParentDir> parent, QStringView name, bool isDir);

    void copyOriginalNameToClipboard() const;

    bool isDir() const;
    QFileInfo fileInfo() const;
    QString fullPath() const;
    QString parentPath() const;

    QString name() const;
    QString newName() const;

    QWeakPointer<ParentDir> parent() const;

    QString hashHex(QCryptographicHash::Algorithm algorithm) const;

    void setHashHex(QCryptographicHash::Algorithm algorithm, QStringView hashHex);
    void setNewName(QStringView newName);

private:
    mutable QReadWriteLock m_lock;

    const bool m_isDir;

    QWeakPointer<ParentDir> m_parent;
    QString m_name;
    QString m_newName;
    QHash<QCryptographicHash::Algorithm, QString> m_hashs;
};

} // namespace Path

#endif // PATHENTITY_H
