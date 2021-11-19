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

#include "WidgetOriginalNameSetting.h"

#include "StringBuilderOnFile/OriginalName.h"

namespace {
constexpr char settingsGroupName[] = "OriginalName";
}

WidgetOriginalNameSetting::WidgetOriginalNameSetting(QWidget *parent)
    : WidgetOnlyPositionFixer(parent)
{
    setTitle(QStringLiteral("Original name"));
}

QSharedPointer<StringBuilder::AbstractStringBuilder> WidgetOriginalNameSetting::StringBuilder() const
{
    return QSharedPointer<StringBuilderOnFile::OriginalName>::create(positionToInsert());
}

void WidgetOriginalNameSetting::loadSettings(QSharedPointer<QSettings> qSettings)
{
    qSettings->beginGroup(settingsGroupName);
    WidgetOnlyPositionFixer::loadSettings(qSettings);
    qSettings->endGroup();
}

void WidgetOriginalNameSetting::saveSettings(QSharedPointer<QSettings> qSettings) const
{
    qSettings->beginGroup(settingsGroupName);
    WidgetOnlyPositionFixer::saveSettings(qSettings);
    qSettings->endGroup();
}
