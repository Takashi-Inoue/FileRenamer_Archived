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

class QTimer;

namespace Ui {
class FormStringBuilderChain;
}

namespace StringBuilderOnFile {
class BuilderChainOnFile;
}

class FormStringBuilderChain : public QWidget
{
    Q_OBJECT

public:
    explicit FormStringBuilderChain(QWidget *parent = nullptr);
    ~FormStringBuilderChain() override;

    QSharedPointer<StringBuilderOnFile::BuilderChainOnFile> builderChain() const;

signals:
    void settingsChanged();

public slots:
    void createNewSetting();

private:
    Ui::FormStringBuilderChain *ui;
    QTimer *m_timer;
};

#endif // FORMSTRINGBUILDERCHAIN_H
