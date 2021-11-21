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

#ifndef WIDGETNUMBERSETTING_H
#define WIDGETNUMBERSETTING_H

#include "AbstractStringBuilderWidget.h"

namespace Ui {
class WidgetNumberSetting;
}

class WidgetNumberSetting : public AbstractStringBuilderWidget
{
    Q_OBJECT

public:
    explicit WidgetNumberSetting(QWidget *parent = nullptr);
    ~WidgetNumberSetting() override;

    QSharedPointer<StringBuilder::AbstractStringBuilder> StringBuilder() const override;
    void loadSettings(QSharedPointer<QSettings>) override;
    void saveSettings(QSharedPointer<QSettings>) const override;

protected:
    void changeEvent(QEvent *event) override;

private:
    Ui::WidgetNumberSetting *ui;
};

#endif // WIDGETNUMBERSETTING_H
