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
#include "Path/PathHeaderView.h"
#include "Path/PathModel.h"
#include "SearchInDirs.h"
#include "widgets/DialogDroppedDir.h"

#include <QDropEvent>
#include <QMimeData>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_pathModel(new PathModel(this))
{
    ui->setupUi(this);

    setState(State::initial);

    ui->splitter->setSizes({350, 450});

    QStringList paths = QApplication::arguments();

    paths.pop_front();

    registerPaths(paths);

    ui->tableView->setHorizontalHeader(new PathHeaderView(ui->tableView));
    ui->tableView->setModel(m_pathModel);

    connect(ui->formStringBuilderChain, &FormStringBuilderChain::settingsChanged
          , m_pathModel, &PathModel::startCreateNewNames);

    connect(ui->formStringBuilderChain, &FormStringBuilderChain::changeStarted
          , this, &MainWindow::adaptorToChangeState);
    connect(m_pathModel, &PathModel::readyToRename,  this, &MainWindow::adaptorToChangeState);
    connect(m_pathModel, &PathModel::renameStarted,  this, &MainWindow::adaptorToChangeState);
    connect(m_pathModel, &PathModel::renameStopped,  this, &MainWindow::adaptorToChangeState);
    connect(m_pathModel, &PathModel::renameFinished, this, &MainWindow::adaptorToChangeState);
    connect(m_pathModel, &PathModel::undoStarted,    this, &MainWindow::adaptorToChangeState);

    connect(m_pathModel, &PathModel::internalDataChanged, this, &MainWindow::onPathsDataChanged);

    connect(ui->actionRename, &QAction::triggered, m_pathModel, &PathModel::startRename);
    connect(ui->actionStop, &QAction::triggered, m_pathModel, &PathModel::stopRename);
    connect(ui->actionUndo, &QAction::triggered, m_pathModel, &PathModel::undoRename);

    m_pathModel->startCreateNewNames(ui->formStringBuilderChain->builderChain());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->setAccepted(ui->actionExit->isEnabled());
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

    QStringList paths;

    for (const QUrl &url : event->mimeData()->urls())
        paths << url.toLocalFile();

    registerPaths(paths);
}

void MainWindow::onPathsDataChanged()
{
    m_pathModel->startCreateNewNames(ui->formStringBuilderChain->builderChain());
}

void MainWindow::adaptorToChangeState()
{
    static const QHash<int, State> hashSignalToState = {
        {ui->formStringBuilderChain->metaObject()->indexOfSignal("changeStarted()")
       , State::changingSettings},
        {m_pathModel->metaObject()->indexOfSignal("readyToRename()"),  State::ready},
        {m_pathModel->metaObject()->indexOfSignal("renameStarted()"),  State::renaming},
        {m_pathModel->metaObject()->indexOfSignal("renameStopped()"),  State::stopped},
        {m_pathModel->metaObject()->indexOfSignal("renameFinished()"), State::finishedRenaming},
        {m_pathModel->metaObject()->indexOfSignal("undoStarted()"),    State::undoing},
    };

    auto itr = hashSignalToState.find(senderSignalIndex());

    if (itr == hashSignalToState.end())
        return;

    setState(itr.value());
}

void MainWindow::setState(MainWindow::State state)
{
    m_state = state;

    QHash<int, QList<bool>> hashForUI = {
        {int(State::initial),          {false, false, false, true}},
        {int(State::changingSettings), {false, false, false, true}},
        {int(State::ready),            {true,  false, false, true}},
        {int(State::renaming),         {false, true,  false, false}},
        {int(State::stopped),          {true,  false, true,  true}},
        {int(State::undoing),          {false, true,  false, false}},
        {int(State::finishedRenaming), {false, false, true,  true}},
    };

    enum Actions {rename, stop, undo, exit};

    ui->actionRename->setEnabled(hashForUI[int(state)][rename]);
    ui->actionStop->setEnabled(hashForUI[int(state)][stop]);
    ui->actionUndo->setEnabled(hashForUI[int(state)][undo]);
    ui->actionExit->setEnabled(hashForUI[int(state)][exit]);
}

void MainWindow::onPathsAdded()
{
    QHeaderView *header = ui->tableView->horizontalHeader();

    m_pathModel->sort(header->sortIndicatorSection(), header->sortIndicatorOrder());
}

void MainWindow::registerPaths(const QStringList &paths)
{
    PathsAnalyzer analyzer;

    analyzer.analyze(paths);

    if (!analyzer.isAllDir()) {
        m_pathModel->addPaths(analyzer.dirs(), analyzer.files());
        onPathsAdded();

        return;
    }

    DialogDroppedDir dlg(analyzer.dirs(), this);

    if (dlg.exec() == QDialog::Rejected)
        return;

    if (dlg.isRegisterDroppedDir()) {
        m_pathModel->addPaths(analyzer.dirs(), {});
        onPathsAdded();

        return;
    }

    SearchInDirs searchInDirs(dlg.searchSettings());

    searchInDirs.exec(analyzer.dirs());

    m_pathModel->addPaths(searchInDirs.dirs(), searchInDirs.files());

    onPathsAdded();
}
