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

namespace Path {
//class ParentDir;
class PathRoot;
}

namespace StringBuilderOnFile{
class BuilderChainOnFile;
}

class ThreadCreateNewNames;
class ThreadRename;

class PathModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    using ParentChildrenPair = QPair<QString, QStringList>;

    explicit PathModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation
                      , int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;

    // Add/Remove data:
    void addPaths(QList<ParentChildrenPair> dirs, QList<ParentChildrenPair> files);
    void removeSpecifiedRows(QList<int> rows);

    void copyOriginalNameToClipboard(int row) const;

    // Start threads
    void startCreateNewNames(QSharedPointer<StringBuilderOnFile::BuilderChainOnFile> builderChain);
    void startRename();

signals:
    void internalDataChanged();

private slots:
    void onCreateNameCompleted();
    void onNewNameCollisionDetected(QPair<int, int> indices);
    void onNewNameCreated(int row);
    void onStateChanged(int row);

private:
    enum class HSection : int {
        originalName, newName, path
    };

    void stopThreadToCreateNames();

    QSharedPointer<Path::PathRoot> m_dataRoot;
    ThreadCreateNewNames *m_threadCreateNewNames;
    ThreadRename *m_threadRename;
};

#endif // PATHMODEL_H