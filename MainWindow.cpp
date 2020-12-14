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

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "PathsAnalyzer.h"
#include "widgets/DialogDroppedDir.h"

#include <QDropEvent>
#include <QMimeData>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->splitter->setSizes({350, 450});

    QStringList paths = QApplication::arguments();

    paths.pop_front();

    registerPaths({paths.begin(), paths.end()});
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
//    if (!renameManager->canChangeRenameSettings())
//        return;

    if (event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event)
{
//    if (!renameManager->canChangeRenameSettings())
//        return;

    QVector<QString> paths;

    for (const QUrl &url : event->mimeData()->urls())
        paths << url.toLocalFile();

    registerPaths(paths);
}

void MainWindow::registerPaths(const QVector<QString> &paths)
{
    PathsAnalyzer analyzer;

    analyzer.analyze(paths);

    if (analyzer.isAllDir()) {
        DialogDroppedDir dlg(analyzer.analyzedMap(), this);

        if (dlg.exec() == QDialog::Rejected)
            return;
    }
}
