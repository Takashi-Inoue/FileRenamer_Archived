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

#ifndef NUMBERSETTINGS_H
#define NUMBERSETTINGS_H

#include "AbstractSettings.h"

class NumberSettings : public AbstractSettings
{
public:
    enum Entry {
        startEntry, stepEntry, digitEntry, prefixEntry, suffixEntry, positionEntry
    };

    NumberSettings();

    int start() const;
    int step() const;
    int digit() const;
    QString prefix() const;
    QString suffix() const;
    int position() const;
};

#endif // NUMBERSETTINGS_H