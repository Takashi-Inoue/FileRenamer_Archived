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

#ifndef INSERTSTRING_H
#define INSERTSTRING_H

#include "AbstractInsertString.h"

namespace StringBuilder {

class InsertString : public AbstractInsertString
{
    Q_OBJECT
public:
    InsertString(int pos, QStringView string, QObject *parent = nullptr);

    void build(QString &result) override;

protected:
    const QString m_string;
};

} // namespace StringBuilder

#endif // INSERTSTRING_H
