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

#ifndef ABSTRACTNEEDFILEINFO_H
#define ABSTRACTNEEDFILEINFO_H

#include "StringBuilder/AbstractInsertString.h"

#include <QFileInfo>
#include <QMetaType>
#include <QObject>

namespace StringBuilder {
namespace File {

class AbstractNeedFileInfo : public AbstractInsertString
{
    Q_OBJECT
public:
    AbstractNeedFileInfo(int pos, QObject *parent = nullptr)
        : AbstractInsertString(pos, parent)
    {
    }

    void setFileInfo(const QFileInfo &fileInfo)
    {
        m_fileInfo = fileInfo;
    }

signals:
    void needFileInfo(StringBuilder::File::AbstractNeedFileInfo *stringBuilder);

protected:
    QFileInfo m_fileInfo;
};

} // File StringBuilder
} // namespace StringBuilder

Q_DECLARE_METATYPE(StringBuilder::File::AbstractNeedFileInfo *)

#endif // ABSTRACTNEEDFILEINFO_H
