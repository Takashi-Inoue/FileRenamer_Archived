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

#include "StringBuilderWidgetFactory.h"

#include "widgets/WidgetOriginalNameSetting.h"
#include "widgets/WidgetNumberSetting.h"
#include "widgets/WidgetReplaceSetting.h"
#include "widgets/WidgetInsertTextSetting.h"
#include "widgets/WidgetFileHashSetting.h"
#include "widgets/WidgetImageHashSetting.h"

AbstractStringBuilderWidget *StringBuilderWidgetFactory::createWidget(
        int builderIndex, QWidget *parent) const
{
    // builderIndex links index of combobox in FormStringBuilder.
    switch (builderIndex) {
    case 0:
        return new WidgetOriginalNameSetting(parent);

    case 1:
        return new WidgetNumberSetting(parent);

    case 2:
        return new WidgetReplaceSetting(parent);

    case 3:
        return new WidgetInsertTextSetting(parent);

    case 4:
        return new WidgetFileHashSetting(parent);

    case 5:
        return new WidgetImageHashSetting(parent);
    }

    Q_ASSERT(false);

    return nullptr;
}
