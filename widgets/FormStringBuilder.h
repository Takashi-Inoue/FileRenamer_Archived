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

#ifndef FORMSTRINGBUILDER_H
#define FORMSTRINGBUILDER_H

#include <QFrame>
#include <QSharedPointer>

namespace StringBuilder {class AbstractStringBuilder;}

class QPushButton;

namespace Ui {
class FormStringBuilder;
}

class FormStringBuilder : public QFrame
{
    Q_OBJECT

public:
    explicit FormStringBuilder(QWidget *parent = nullptr);
    ~FormStringBuilder() override;

    QSharedPointer<StringBuilder::AbstractStringBuilder> stringBuilder() const;

signals:
    void changeStarted();

protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::FormStringBuilder *ui;
    QPushButton *m_buttonClose;
};

#endif // FORMSTRINGBUILDER_H
