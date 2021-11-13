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

#ifndef APPLICATIONLOG_H
#define APPLICATIONLOG_H

#include "LogData.h"

#include <QAbstractTableModel>
#include <QString>
#include <QReadWriteLock>
#include <memory>

class ApplicationLog : public QAbstractTableModel
{
    ApplicationLog(const ApplicationLog &) = delete;
    ApplicationLog(ApplicationLog &&) = delete;
    ApplicationLog &operator=(const ApplicationLog &) = delete;
    ApplicationLog &operator=(ApplicationLog &&) = delete;

    Q_OBJECT
public:
    static ApplicationLog &instance();

    // implementation for QAbstractTableModel
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void clear();
    bool isEmpty() const;
    void log(QStringView log, QStringView groupName = QStringLiteral("General"));

    void writeFile() const;
    void writeSpecifiedLogs(QStringView groupName) const;

private:
    using QAbstractTableModel::QAbstractTableModel;

    QString logDir() const;
    void removeOldestLogFile() const;

    using LogDataPtr = std::unique_ptr<LogData>;

    mutable QReadWriteLock m_lock;
    std::vector<LogDataPtr> m_applicationLogs;
};

#endif // APPLICATIONLOG_H
