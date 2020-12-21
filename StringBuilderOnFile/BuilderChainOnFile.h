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

#ifndef BUILDERCHAINONFILE_H
#define BUILDERCHAINONFILE_H

#include "StringBuilder/BuilderChain.h"

namespace Path {class PathEntity;}

namespace StringBuilderOnFile {

class AbstractNeedFileInfo;
class IFileInfo;

class BuilderChainOnFile : public StringBuilder::BuilderChain
{
    Q_OBJECT
public:
    using BuilderChain::BuilderChain;

    void addCreator(QSharedPointer<StringBuilder::AbstractStringBuilder> creator) override;
    void setFileInfo(QSharedPointer<IFileInfo> fileInfo);

private slots:
    void onNeedFileInfo(StringBuilderOnFile::AbstractNeedFileInfo *stringBuilder);

private:
    QSharedPointer<IFileInfo> m_fileInfo;
};

} // namespace StringBuilderOnFile

Q_DECLARE_METATYPE(QSharedPointer<StringBuilderOnFile::BuilderChainOnFile>)

#endif // BUILDERCHAINONFILE_H
