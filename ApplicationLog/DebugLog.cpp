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

#include "DebugLog.h"
#include "LogData.h"

#include <QApplication>
#include <QDebug>

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

std::vector<DebugLog::LogDataPtr> DebugLog::m_debugLogs;

void DebugLog::init()
{
    qInstallMessageHandler(debugMessageHandler);
}

void DebugLog::writeFile()
{
    if (m_debugLogs.size() == 0)
        return;

    QFile file(QApplication::applicationDirPath() + "/debug.log");

    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "failed to write log to" << file.fileName() << ":" << file.errorString();
        return;
    }

    for (const LogDataPtr &logData : m_debugLogs)
        logData->write(file);
}

void DebugLog::debugMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    LogData *data = nullptr;

    switch (type) {
    case QtDebugMsg:
        data = new LogData(QStringLiteral("Debug"), QString("%1 (%2)").arg(msg).arg(context.function));
        break;

    case QtInfoMsg:
        data = new LogData(QStringLiteral("Info"), msg);
        break;

    case QtWarningMsg:
        data = new LogData(QStringLiteral("Warning"), QString("%1 (%2:%3, %4)")
                          .arg(msg).arg(context.file).arg(context.line).arg(context.function));
        break;

    case QtCriticalMsg:
        data = new LogData(QStringLiteral("Critical"), QString("%1 (%2:%3, %4)")
                          .arg(msg).arg(context.file).arg(context.line).arg(context.function));
        break;

    case QtFatalMsg:
        outputMessage(QString("Fatal: %1 (%2:%3, %4)").arg(msg).arg(context.file)
                     .arg(context.line).arg(context.function));
        abort();
    }

    outputMessage(QString("[%1] %2").arg(data->group(), data->log()));

    m_debugLogs.emplace_back(LogDataPtr(data));
}

QString DebugLog::outputMessage(const QString &msg)
{
#ifdef Q_OS_WIN
    #ifdef UNICODE
        OutputDebugString((msg + "\r\n").toStdWString().c_str());
    #else
        OutputDebugString((msg + "\r\n").toStdString().c_str());
    #endif
#else
    fprintf(stderr, qPrintable(msg + "\n"));
#endif

    return msg;
}
