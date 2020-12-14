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

#include "BuilderChainOnFile.h"
#include "AbstractNeedFileInfo.h"
#include "Path/EntityName.h"

namespace StringBuilder {
namespace File {

void BuilderChainOnFile::addCreator(QSharedPointer<AbstractStringBuilder> creator)
{
    auto needFileInfo = qobject_cast<AbstractNeedFileInfo *>(creator.get());

    if (needFileInfo != nullptr) {
        connect(needFileInfo, &AbstractNeedFileInfo::needFileInfo
              , this, &BuilderChainOnFile::onNeedFileInfo, Qt::DirectConnection);
    }

    BuilderChain::addCreator(creator);
}

void BuilderChainOnFile::setPathEntityName(QWeakPointer<Path::EntityName> pathEntityName)
{
    m_pathEntityName = pathEntityName;
}

void BuilderChainOnFile::onNeedFileInfo(AbstractNeedFileInfo *stringBuilder)
{
    QSharedPointer<Path::EntityName> entityName = m_pathEntityName.lock();

    if (entityName != nullptr)
        stringBuilder->setFileInfo(entityName->fileInfo());
}

} // namespace File
} // namespace StringBuilder
