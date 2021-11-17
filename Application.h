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

#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QSettings>

class Application : public QApplication
{
    Application(const Application &) = delete;
    Application(Application &&) = delete;
    Application &operator=(const Application &) = delete;
    Application &operator=(Application &&) = delete;

public:
    using QApplication::QApplication;

    static void applyDarkPalette();
    static void applyDefaultPalette();
    static bool isDarkMode();

    static void loadMainSettings();
    static void saveMainSettings();

    static QString mainSettingsFilePath();
    static QString renameSettingsDirPath();

    static QSharedPointer<QSettings> mainQSettings();

private:
    static constexpr char propertyDarkMode[] = "DarkMode";
};

#endif // APPLICATION_H
