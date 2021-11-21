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

#ifndef DEBUGLOG_H
#define DEBUGLOG_H

#include <QString>

#include <memory>

class LogData;

class DebugLog final
{
    DebugLog(const DebugLog &) = delete;
    DebugLog(DebugLog &&) = delete;
    DebugLog &operator=(const DebugLog &) = delete;
    DebugLog &operator=(DebugLog &&) = delete;
public:
    DebugLog() = default;
    ~DebugLog() = default;

    static void init();
    static void writeFile();

private:
    using LogDataPtr = std::unique_ptr<LogData>;

    static void debugMessageHandler(QtMsgType type, const QMessageLogContext &context
                                  , const QString &msg);
    static QString outputMessage(const QString &msg);

    static std::vector<LogDataPtr> m_debugLogs;
};

#endif // DEBUGLOG_H
