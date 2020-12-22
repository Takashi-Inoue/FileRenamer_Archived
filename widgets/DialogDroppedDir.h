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

#ifndef DIALOGDROPPEDDIR_H
#define DIALOGDROPPEDDIR_H

#include "Settings/SearchSettings.h"

#include <QDialog>

namespace Ui {
class DialogDroppedDir;
}

class DialogDroppedDir : public QDialog
{
    Q_OBJECT

public:
    using ParentChildrenPair = QPair<QString, QStringList>;

    explicit DialogDroppedDir(const QList<ParentChildrenPair> &dirs, QWidget *parent = nullptr);
    ~DialogDroppedDir() override;

    bool isRegisterDroppedDir() const;
    const SearchSettings &searchSettings() const;

private slots:
    void on_pushButtonBack_clicked();
    void on_pushButtonOk_clicked();
    void on_pushButtonRegisterDir_clicked();
    void on_pushButtonSearch_clicked();

private:
    Ui::DialogDroppedDir *ui;

    bool m_isRegisterDir = false;
    SearchSettings m_settings;
};

#endif // DIALOGDROPPEDDIR_H
