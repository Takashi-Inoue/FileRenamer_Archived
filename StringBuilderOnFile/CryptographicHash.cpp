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

#include "CryptographicHash.h"
#include "IFileInfo.h"

#include <QCryptographicHash>
#include <QFile>

namespace StringBuilderOnFile {

CryptographicHash::CryptographicHash(QCryptographicHash::Algorithm algorithm, int pos
                                   , QObject *parent)
    : AbstractNeedFileInfo(pos, parent)
    , m_algorithm(algorithm)
{
}

void CryptographicHash::build(QString &result)
{
    emit needFileInfo(this);

    QString hashHex = m_fileInfo->hashHex(m_algorithm);

    if (hashHex.isEmpty()) {
        QFile file(m_fileInfo->fullPath());

        if (!file.open(QFile::ReadOnly))
            return;

        QCryptographicHash hash(m_algorithm);

        if (!hash.addData(&file))
            return;

        hashHex = hash.result().toHex();

        m_fileInfo->setHashHex(m_algorithm, hashHex);
    }

    result.insert(posToInsert(result.size()), hashHex);
}

} // namespace StringBuilderOnFile
