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

#include "MainWindow.h"
#include "ApplicationLog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFont font = QApplication::font("QMenu");
    font.setPointSize(10);

    QApplication::setFont(font);

    QCoreApplication::setApplicationName("File Renamer");
    QCoreApplication::setApplicationVersion("0.1.0.0");

    ApplicationLog::instance().init();

    MainWindow w;
    w.show();

    int result = a.exec();

    ApplicationLog::instance().writeFile();

    return result;
}
