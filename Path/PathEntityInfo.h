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

#ifndef PATHENTITYINFO_H
#define PATHENTITYINFO_H

#include "StringBuilderOnFile/IFileInfo.h"

#include <QSharedPointer>

namespace Path {

class PathEntity;

class PathEntityInfo : public StringBuilderOnFile::IFileInfo
{
public:
    PathEntityInfo(QSharedPointer<PathEntity> entity);

    bool isDir() const override;
    QString fullPath() const override;
    QString fileName() const override;
    QString completeBaseName() const override;
    QString suffix() const override;
    QString hashHex(QCryptographicHash::Algorithm algorithm) const override;
    QString imageHash() const override;

    void setHashHex(QCryptographicHash::Algorithm algorithm, QString hashHex) override;
    void setImageHash(QString) override;

private:
    QSharedPointer<PathEntity> m_entity;
};

} // namespace Path

#endif // PATHENTITYINFO_H
