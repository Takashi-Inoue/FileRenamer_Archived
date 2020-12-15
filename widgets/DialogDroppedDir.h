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

#ifndef DIALOGDROPPEDDIR_H
#define DIALOGDROPPEDDIR_H

#include "Settings/SearchSettings.h"

#include <QDialog>
#include "QStringVector.h"

namespace Ui {
class DialogDroppedDir;
}

class DialogDroppedDir : public QDialog
{
    Q_OBJECT

public:
    using ParentChildrenPair = QPair<QString, QStringVector>;

    explicit DialogDroppedDir(const QVector<ParentChildrenPair> &dirs, QWidget *parent = nullptr);
    ~DialogDroppedDir();

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
