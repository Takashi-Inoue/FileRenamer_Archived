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

#ifndef FORMSTRINGBUILDERCHAIN_H
#define FORMSTRINGBUILDERCHAIN_H

#include <QWidget>

#include "StringBuilderOnFile/BuilderChainOnFile.h"

class QTimer;

namespace Ui {
class FormStringBuilderChain;
}

class FormStringBuilderChain : public QWidget
{
    Q_OBJECT

public:
    explicit FormStringBuilderChain(QWidget *parent = nullptr);
    ~FormStringBuilderChain() override;

    QSharedPointer<StringBuilderOnFile::BuilderChainOnFile> builderChain() const;

signals:
    void changeStarted();
    void settingsChanged(QSharedPointer<StringBuilderOnFile::BuilderChainOnFile>);

public slots:
    void createNewSetting();

private slots:
    void startTimer();

private:
    Ui::FormStringBuilderChain *ui;
    QTimer *m_timer;
};

#endif // FORMSTRINGBUILDERCHAIN_H
