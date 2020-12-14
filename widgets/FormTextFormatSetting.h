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

#ifndef FORMTEXTFORMATSETTING_H
#define FORMTEXTFORMATSETTING_H

#include <QFrame>
#include <QSharedPointer>

namespace StringBuilder {class AbstractStringBuilder;}

class QPushButton;

namespace Ui {
class FormTextFormatSetting;
}

class FormTextFormatSetting : public QFrame
{
    Q_OBJECT

public:
    explicit FormTextFormatSetting(QWidget *parent = nullptr);
    ~FormTextFormatSetting() override;

    QSharedPointer<StringBuilder::AbstractStringBuilder> StringBuilder() const;

protected:
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    static constexpr char m_closeButtonName[] = "pushButtonClose";

    Ui::FormTextFormatSetting *ui;
    QPushButton *m_buttonClose;
};

#endif // FORMTEXTFORMATSETTING_H
