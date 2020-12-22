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

#include "ApplicationLog.h"

#include <QApplication>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QDebug>

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

class ApplicationLog::LogData
{
public:
    LogData(QStringView group, QStringView log)
        : m_time(QDateTime::currentDateTime())
        , m_group(group.toString())
        , m_log(log.toString())
    {
    }

    QString time(QStringView format) const
    {
        return m_time.toString(format);
    }

    QString group() const
    {
        return m_group;
    }

    QString log() const
    {
        return m_log;
    }

    void write(QFile &file) const
    {
        QString line = QStringLiteral("[%1] [%2] : %3")
                       .arg(m_time.toString(QStringLiteral("yyyy-MM-dd hh:mm:ss")), m_group, m_log);
        file.write(line.toUtf8());
        file.write("\r\n");
    }

private:
    const QDateTime m_time;
    const QString m_group;
    const QString m_log;
};

QReadWriteLock ApplicationLog::m_lock;
std::vector<ApplicationLog::LogDataPtr> ApplicationLog::m_applicationLogs;
std::vector<ApplicationLog::LogDataPtr> ApplicationLog::m_debugLogs;

ApplicationLog &ApplicationLog::instance()
{
    static ApplicationLog appLogInstance;
    return appLogInstance;
}

int ApplicationLog::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    QReadLocker locker(&m_lock);

    return int(m_applicationLogs.size());
}

int ApplicationLog::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 3;
}

QVariant ApplicationLog::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical || role != Qt::DisplayRole)
        return QVariant();

    QHash<int, QString> hashData = {
        {0, QStringLiteral("Time")},
        {1, QStringLiteral("Category")},
        {2, QStringLiteral("Content")},
    };

    return hashData[section];
}

QVariant ApplicationLog::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole || !index.isValid())
        return QVariant();

    QReadLocker locker(&m_lock);

    auto row = std::size_t(index.row());
    const LogDataPtr &logData = m_applicationLogs.at(row);

    if (index.column() == 0)
        return logData->time(QStringLiteral("hh:mm:ss"));
    else if (index.column() == 1)
        return logData->group();

    return logData->log();
}

void ApplicationLog::init()
{
    qInstallMessageHandler(debugMessageHandler);
    QDir().mkpath(logDir());
}

void ApplicationLog::clear()
{
    QWriteLocker locker(&m_lock);

    m_applicationLogs.clear();
}

bool ApplicationLog::isEmpty() const
{
    return m_applicationLogs.size() == 0;
}

void ApplicationLog::log(QStringView log, QStringView groupName)
{
    m_lock.lockForRead();
    int row = int(m_applicationLogs.size());
    m_lock.unlock();

    beginInsertRows(QModelIndex(), row, row);

    m_lock.lockForWrite();
    m_applicationLogs.emplace_back(std::make_unique<LogData>(groupName, log));
    m_lock.unlock();

    endInsertRows();
}

void ApplicationLog::writeFile() const
{
    writeApplicationLog();
    writeDebugLog();
}

void ApplicationLog::writeSpecifiedLogs(QStringView groupName) const
{
    QReadLocker locker(&m_lock);

    if (m_applicationLogs.size() == 0)
        return;

    QFile file(logDir() + QStringLiteral("/%1.log").arg(groupName));

    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "failed to write log to" << file.fileName() << ":" << file.errorString();
        return;
    }

    for (const LogDataPtr &logData : m_applicationLogs) {
        if (logData->group() == groupName)
            logData->write(file);
    }
}

QString ApplicationLog::logDir() const
{
    return QApplication::applicationDirPath() + QStringLiteral("/logs");
}

void ApplicationLog::writeApplicationLog() const
{
    QReadLocker locker(&m_lock);

    if (m_applicationLogs.size() == 0)
        return;

    QFile file(logDir() + "/" + QApplication::applicationName() + ".log");

    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "failed to write log to" << file.fileName() << ":" << file.errorString();
        return;
    }

    for (const LogDataPtr &logData : m_applicationLogs)
        logData->write(file);
}

void ApplicationLog::writeDebugLog() const
{
    QReadLocker locker(&m_lock);

    if (m_debugLogs.size() == 0)
        return;

    QFile file(logDir() + "/debug.log");

    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "failed to write log to" << file.fileName() << ":" << file.errorString();
        return;
    }

    for (const LogDataPtr &logData : m_debugLogs)
        logData->write(file);
}

void ApplicationLog::debugMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QWriteLocker locker(&m_lock);

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

QString ApplicationLog::outputMessage(const QString &msg)
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
