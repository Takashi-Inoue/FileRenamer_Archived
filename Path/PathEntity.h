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

#ifndef PATHENTITY_H
#define PATHENTITY_H

#include <QCryptographicHash>
#include <QFileInfo>
#include <QIcon>
#include <QReadWriteLock>
#include <QSharedPointer>
#include <QString>

namespace Path {

class ParentDir;

class PathEntity
{
public:
    PathEntity(QWeakPointer<ParentDir> parent, QStringView name, bool isDir);

    bool isDir() const;
    QString fullPath() const;
    QString parentPath() const;

    QString name() const;
    QString newName() const;

    QWeakPointer<ParentDir> parent() const;

    QString hashHex(QCryptographicHash::Algorithm algorithm) const;
    QString imageHash() const;

    void setHashHex(QCryptographicHash::Algorithm algorithm, QStringView hashHex);
    void setImageHash(QStringView imageHash);
    void setNewName(QStringView newName);

    bool checkForNewNameCollisions(QSharedPointer<PathEntity> other);
    bool checkSelfNewName();

    QIcon stateIcon() const;
    QString stateText() const;

    QIcon typeIcon() const;
    QString statusText() const;

    bool rename();
    bool undoRename();

private:
    enum class State : int {
        initial, ready, sameNewName, success, failure
    };

    enum class ErrorCode : int {
        noError, alreadyExist, sourceNotFound, unknown
    };

    void setState(State state);
    State state() const;

    void findErrorCause();

    mutable QReadWriteLock m_lock;

    const bool m_isDir;

    State m_state = State::initial;
    ErrorCode m_errorCode = ErrorCode::noError;

    QWeakPointer<ParentDir> m_parent;
    QString m_name;
    QString m_newName;
    QIcon m_fileIcon;
    QHash<QCryptographicHash::Algorithm, QString> m_fileHashs;
    QString m_imageHash;
};

} // namespace Path

#endif // PATHENTITY_H
