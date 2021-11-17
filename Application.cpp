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

#include "Application.h"

#include <QFile>
#include <QPalette>
#include <QStyle>

void Application::applyDarkPalette()
{
    QFile file(QApplication::applicationDirPath() + QStringLiteral("/pal/dark.pal"));

    if (!file.open(QIODevice::ReadOnly))
        return;

    QDataStream stream(&file);

    QPalette pal;

    stream >> pal;

    QApplication::setPalette(pal);

    instance()->setProperty(propertyDarkMode, true);
}

void Application::applyDefaultPalette()
{
    QApplication::setPalette(style()->standardPalette());
    instance()->setProperty(propertyDarkMode, false);
}

bool Application::isDarkMode()
{
    return instance()->property(propertyDarkMode).toBool();
}

void Application::loadMainSettings()
{
    QSharedPointer<QSettings> qSettings = mainQSettings();

    bool isDarkMode = qSettings->value(QStringLiteral("Main/DarkMode"), false).toBool();

    isDarkMode ? applyDarkPalette()
               : applyDefaultPalette();
}

void Application::saveMainSettings()
{
    QSharedPointer<QSettings> qSettings = mainQSettings();

    qSettings->setValue(QStringLiteral("Main/DarkMode"), instance()->property(propertyDarkMode));
}

QString Application::mainSettingsFilePath()
{
    return QApplication::applicationFilePath().replace(".exe", ".ini");
}

QString Application::renameSettingsDirPath()
{
    return QApplication::applicationDirPath() + QStringLiteral("/settings");
}

QSharedPointer<QSettings> Application::mainQSettings()
{
    return QSharedPointer<QSettings>::create(mainSettingsFilePath(), QSettings::IniFormat);
}
