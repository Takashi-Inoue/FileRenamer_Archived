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

#include "MainWindow.h"
#include "Application.h"
#include "ApplicationLog/ApplicationLog.h"
#include "ApplicationLog/DebugLog.h"

#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setStyle(QStyleFactory::create("Fusion"));

    Application::loadMainSettings();

    QFont font = QApplication::font("QMenu");
    font.setPointSize(10);

    QApplication::setFont(font);

    QCoreApplication::setApplicationName("FileRenamer");
    QCoreApplication::setApplicationVersion("1.2.1.1");

    DebugLog::init();

    MainWindow w;
    w.show();

    int result = a.exec();

    Application::saveMainSettings();

    ApplicationLog::instance().writeFile();
    DebugLog::writeFile();

    return result;
}
