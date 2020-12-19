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

#ifndef PATHMODEL_H
#define PATHMODEL_H

#include <QAbstractTableModel>
#include <QSharedPointer>
#include "QStringVector.h"

namespace Path {
//class ParentDir;
class PathRoot;
}

namespace StringBuilderOnFile{
class BuilderChainOnFile;
}

class ThreadCreateNewNames;

class PathModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    using ParentChildrenPair = QPair<QString, QStringVector>;

    explicit PathModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation
                      , int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    void removeSpecifiedRows(QVector<int> rows);

    // Add data:
    void addPathsAsDirs(QVector<ParentChildrenPair> dirs);
    void addPathsAsFiles(QVector<ParentChildrenPair> files);

    void copyOriginalNameToClipboard(int row) const;
    void startCreateNewNames(QSharedPointer<StringBuilderOnFile::BuilderChainOnFile> builderChain);

private:
    enum class HSection : int {
        originalName, newName, path
    };

    QSharedPointer<Path::PathRoot> m_dataRoot;
    ThreadCreateNewNames *m_threadCreateNewNames;
};

#endif // PATHMODEL_H
