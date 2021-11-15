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

#ifndef FORMSTRINGBUILDERCHAIN_H
#define FORMSTRINGBUILDERCHAIN_H

#include <QWidget>

#include "StringBuilderOnFile/BuilderChainOnFile.h"

class QSettings;
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
    void loadBuilderSettings(QSharedPointer<QSettings>);
    void saveCurrentBuilderSettings(QSharedPointer<QSettings>) const;

signals:
    void builderCleared();
    void changeStarted();
    void settingsChanged(QSharedPointer<StringBuilderOnFile::BuilderChainOnFile>);
    void settingsIndicesChanged();

public slots:
    void createNewSetting(int builderIndex = 0);

private slots:
    void startTimer();
    void onBuilderRequestedDown();
    void onBuilderRequestedUp();
    void onBuilderRequestedRemove();
    void onSettingsIndicesChanged();

private:
    int settingsCount() const;

    Ui::FormStringBuilderChain *ui;
    QTimer *m_timer;
};

#endif // FORMSTRINGBUILDERCHAIN_H
