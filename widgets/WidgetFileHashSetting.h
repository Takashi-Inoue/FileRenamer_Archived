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

#ifndef WIDGETFILEHASHSETTING_H
#define WIDGETFILEHASHSETTING_H

#include "AbstractStringBuilderWidget.h"

namespace Ui {
class WidgetFileHashSetting;
}

class WidgetFileHashSetting : public AbstractStringBuilderWidget
{
    Q_OBJECT

public:
    explicit WidgetFileHashSetting(QWidget *parent = nullptr);
    ~WidgetFileHashSetting() override;

    QSharedPointer<StringBuilder::AbstractStringBuilder> StringBuilder() const override;

private:
    Ui::WidgetFileHashSetting *ui;
};

#endif // WIDGETFILEHASHSETTING_H