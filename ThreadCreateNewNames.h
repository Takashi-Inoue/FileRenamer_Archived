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

#ifndef THREADCREATENEWNAMES_H
#define THREADCREATENEWNAMES_H

#include <QThread>

#include <QReadWriteLock>
#include <QWeakPointer>

namespace Path {
class PathRoot;
}

namespace StringBuilderOnFile {
class BuilderChainOnFile;
}

class ThreadCreateNewNames : public QThread
{
    Q_OBJECT
public:
    ThreadCreateNewNames(QWeakPointer<Path::PathRoot> pathRoot, QObject *parent = nullptr);

    void setStringBuilderOnFile(QSharedPointer<StringBuilderOnFile::BuilderChainOnFile> builderChain);
    void stop();

signals:
    void newNameCreated(int index);

protected:
    void run() override;

    bool isStopRequested() const;

private:
    mutable QReadWriteLock m_lock;

    bool m_isStopRequested = false;
    QWeakPointer<Path::PathRoot> m_pathRoot;
    QSharedPointer<StringBuilderOnFile::BuilderChainOnFile> m_builderChain;
};

#endif // THREADCREATENEWNAMES_H
