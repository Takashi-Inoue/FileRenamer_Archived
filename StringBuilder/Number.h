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

#ifndef NUMBER_H
#define NUMBER_H

#include "AbstractInsertString.h"

namespace StringBuilder {

class Number : public AbstractInsertString
{
    Q_OBJECT
public:
    Number(int pos, int start, int step, int digit, QObject *parent = nullptr);

    void create(QString &result) override;

private:
    const int m_step;
    const int m_digit;

    int m_currentNumber;
};

} // namespace StringBuilder

#endif // NUMBER_H
