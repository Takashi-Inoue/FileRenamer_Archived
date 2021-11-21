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

#ifndef ABSTRACTNEEDFILEINFO_H
#define ABSTRACTNEEDFILEINFO_H

#include "StringBuilder/AbstractInsertString.h"

#include <QMetaType>
#include <QObject>
#include <QSharedPointer>

namespace StringBuilderOnFile {

class IFileInfo;

class AbstractNeedFileInfo : public StringBuilder::AbstractInsertString
{
    Q_OBJECT
public:
    using AbstractInsertString::AbstractInsertString;

    void setFileInfo(QSharedPointer<IFileInfo> fileInfo)
    {
        if (m_fileInfo !=fileInfo)
            m_fileInfo = fileInfo;
    }

signals:
    void needFileInfo(StringBuilderOnFile::AbstractNeedFileInfo *stringBuilder);

protected:
    QSharedPointer<IFileInfo> m_fileInfo;
};

} // namespace StringBuilderOnFile

Q_DECLARE_METATYPE(StringBuilderOnFile::AbstractNeedFileInfo *)

#endif // ABSTRACTNEEDFILEINFO_H
