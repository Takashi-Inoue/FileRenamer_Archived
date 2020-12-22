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

#ifndef CRYPTOGRAPHICHASH_H
#define CRYPTOGRAPHICHASH_H

#include "AbstractNeedFileInfo.h"

#include <QCryptographicHash>

namespace StringBuilderOnFile {

class CryptographicHash : public AbstractNeedFileInfo
{
    Q_OBJECT
public:
    CryptographicHash(QCryptographicHash::Algorithm algorithm, int pos, QObject *parent = nullptr);

    void build(QString &result) override;

protected:
    QCryptographicHash::Algorithm m_algorithm;
};

} // namespace StringBuilderOnFile

#endif // CRYPTOGRAPHICHASH_H
