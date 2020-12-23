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

#ifndef WIDGETONLYPOSITIONFIXER_H
#define WIDGETONLYPOSITIONFIXER_H

#include "AbstractStringBuilderWidget.h"

namespace Ui {
class WidgetOnlyPositionFixer;
}

class WidgetOnlyPositionFixer : public AbstractStringBuilderWidget
{
    Q_OBJECT

public:
    explicit WidgetOnlyPositionFixer(QWidget *parent = nullptr);
    ~WidgetOnlyPositionFixer() override;

protected:
    int positionToInsert() const;
    void setPositionToInsert(int pos);
    void setTitle(QStringView title);

private:
    Ui::WidgetOnlyPositionFixer *ui;
};

#endif // WIDGETONLYPOSITIONFIXER_H
