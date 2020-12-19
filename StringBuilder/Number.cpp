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

#include "Number.h"

namespace StringBuilder {

Number::Number(int pos, int start, int step, int digit, QStringView prefix, QStringView suffix, QObject *parent)
    : AbstractInsertString(pos, parent)
    , m_step(step)
    , m_digit(digit)
    , m_prefix(prefix.toString())
    , m_suffix(suffix.toString())
    , m_currentNumber(start)
{
}

void Number::build(QString &result)
{
    QString numberString = QString("%1%2%3")
                           .arg(m_prefix)
                           .arg(m_currentNumber, m_digit, 10, QLatin1Char('0'))
                           .arg(m_suffix);

    result.insert(posToInsert(result.size()), numberString);

    m_currentNumber += m_step;
}

} // namespace StringBuilder

