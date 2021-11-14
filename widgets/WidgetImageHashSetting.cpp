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

#include "WidgetImageHashSetting.h"

#include "Settings/ImageHashSettings.h"
#include "StringBuilderOnFile/ImageHash.h"

WidgetImageHashSetting::WidgetImageHashSetting(QWidget *parent)
    : WidgetOnlyPositionFixer(parent)
{
    setTitle(QStringLiteral("Image Hash"));
}

QSharedPointer<StringBuilder::AbstractStringBuilder> WidgetImageHashSetting::StringBuilder() const
{
    return QSharedPointer<StringBuilderOnFile::ImageHash>::create(positionToInsert());
}

void WidgetImageHashSetting::loadSettings(QSharedPointer<QSettings> qSettings)
{
    ImageHashSettings settings;

    settings.read(qSettings);

    setPositionToInsert(settings.position());
}

void WidgetImageHashSetting::saveSettings(QSharedPointer<QSettings> qSettings) const
{
    ImageHashSettings settings;

    settings.setValue(ImageHashSettings::positionEntry, positionToInsert());

    settings.write(qSettings);
}
