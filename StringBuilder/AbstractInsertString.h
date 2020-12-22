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

#ifndef ABSTRACTINSERTSTRING_H
#define ABSTRACTINSERTSTRING_H

#include "AbstractStringBuilder.h"

#include <QtGlobal>

namespace StringBuilder {

class AbstractInsertString : public AbstractStringBuilder
{
    Q_OBJECT
public:
    AbstractInsertString(int pos, QObject *parent = nullptr)
        : AbstractStringBuilder(parent)
        , m_pos(pos)
    {}

protected:
    qsizetype posToInsert(qsizetype targetLength)
    {
        qsizetype pos = qMin<qsizetype>(m_pos, targetLength);

        if (m_pos < 0)
            pos = qMax<qsizetype>(0, targetLength + m_pos);

        return pos;
    }

    const int m_pos;
};

} // namespace StringBuilder

#endif // ABSTRACTINSERTSTRING_H
