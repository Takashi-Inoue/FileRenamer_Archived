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

#include "PathTableViewMenu.h"

#include "PathModel.h"

#include <QAbstractItemView>
#include <QFileInfo>
#include <QMetaEnum>
#include <QModelIndex>

PathTableViewMenu::PathTableViewMenu(QAbstractItemView *view, QWidget *parent)
    : QMenu(parent)
    , m_view(view)
{
    Q_CHECK_PTR(view);

    m_model = qobject_cast<PathModel*>(view->model());

    Q_CHECK_PTR(m_model);

    createActions();
    createMenu();
}

void PathTableViewMenu::addActionsToWidget(QWidget *widget)
{
    widget->addActions(m_actions.values());
}

void PathTableViewMenu::setEnabledActions(Action action, bool isEnable)
{
    m_actions[action]->setEnabled(isEnable);
}

void PathTableViewMenu::updateActions()
{
    const QModelIndex currentIndex = m_view->currentIndex();
    const auto currentSection = PathModel::HSection(currentIndex.column());
    const QString displayedText(m_model->data(currentIndex).toString());

    m_actions[Action::CopyName]->setText(QStringLiteral("Copy name \"%1\"").arg(displayedText));

    QFileInfo newPathInfo(m_model->fullPath(currentIndex.row(), PathModel::HSection::NewName));
    bool isNewPathExists = newPathInfo.exists();

    m_actions[Action::OpenMulti]->setEnabled(isNewPathExists);

    bool isDir = true;

    if (currentSection == PathModel::HSection::NewName) {
        isDir = isNewPathExists ? newPathInfo.isDir()
                                : m_model->isDir(currentIndex.row());
        m_actions[Action::OpenPath]->setEnabled(isNewPathExists);
        m_actions[Action::DeletePath]->setEnabled(isNewPathExists);
    } else {
        m_actions[Action::OpenPath]->setEnabled(true);

        bool isCurrentSectionOriginalName = currentSection == PathModel::HSection::OriginalName;

        m_actions[Action::DeletePath]->setEnabled(isCurrentSectionOriginalName);

        if (isCurrentSectionOriginalName)
            isDir = m_model->isDir(currentIndex.row());
    }

    const QString typeText = isDir ? QStringLiteral("dir")
                                   : QStringLiteral("file");

    m_actions[Action::OpenPath]->setText(QStringLiteral("Open %1 \"%2\"").arg(typeText, displayedText));
    m_actions[Action::DeletePath]->setText(QStringLiteral("Delete %1 \"%2\"").arg(typeText, displayedText));

    const QString originalName = m_model->originalName(currentIndex.row());
    const QString newName = m_model->newName(currentIndex.row());

    m_actions[Action::OpenMulti]->setText(QStringLiteral("Open \"%1\" | \"%2\"").arg(originalName, newName));
}

void PathTableViewMenu::createActions()
{
    const QHash<Action, QString> texts = {
        {Action::RemoveItem, QStringLiteral("Remove from list(&R)")}
      , {Action::CopyName,   QStringLiteral("Copy name(&C)")}
      , {Action::OpenPath,   QStringLiteral("Open file(&O)")}
      , {Action::DeletePath, QStringLiteral("Delete file(&D)")}
      , {Action::OpenMulti , QStringLiteral("Open both files(&B)")}
    };

    const QHash<Action, QIcon> icons = {
        {Action::RemoveItem, QIcon(QStringLiteral(":/res/images/x.svg"))}
      , {Action::CopyName,   QIcon(QStringLiteral(":/res/images/file.svg"))}
      , {Action::OpenPath,   QIcon(QStringLiteral(":/res/images/exec.svg"))}
      , {Action::DeletePath, QIcon(QStringLiteral(":/res/images/delete_file.svg"))}
      , {Action::OpenMulti , QIcon(QStringLiteral(":/res/images/exec_double.svg"))}
    };

    const QHash<Action, QKeySequence> shortcuts = {
        {Action::RemoveItem, QKeySequence::Delete}
      , {Action::CopyName,   QKeySequence::Copy}
      , {Action::OpenPath,   QKeySequence::Open}
      , {Action::DeletePath, QKeySequence::DeleteEndOfWord}
      , {Action::OpenMulti , QStringLiteral("Ctrl+Shift+O")}
    };

    const QHash<Action, QString> signalNames = {
        {Action::RemoveItem, QStringLiteral("requestRemoveItem()")}
      , {Action::CopyName,   QStringLiteral("requestCopyName()")}
      , {Action::OpenPath,   QStringLiteral("requestOpenPath()")}
      , {Action::DeletePath, QStringLiteral("requestDeletePath()")}
      , {Action::OpenMulti , QStringLiteral("requestOpenMulti()")}
    };

    QMetaEnum enumActions = QMetaEnum::fromType<Action>();

    for (int i = 0, count = enumActions.keyCount(); i < count; ++i) {
        Action actionType = Action(enumActions.value(i));
        QAction *action = new QAction(icons[actionType], texts[actionType], this);

        m_actions[actionType] = action;

        action->setShortcutContext(Qt::WidgetShortcut);
        action->setShortcut(shortcuts[actionType]);

        int signalIndex = PathTableViewMenu::metaObject()->indexOfSignal(qPrintable(signalNames[actionType]));
        int actionSignalIndex = action->metaObject()->indexOfSignal("triggered(bool)");

        Q_ASSERT(signalIndex != -1 && actionSignalIndex != -1);

        QMetaMethod actionSignal = action->metaObject()->method(actionSignalIndex);
        QMetaMethod signal = PathTableViewMenu::metaObject()->method(signalIndex);

        connect(action, actionSignal, this, signal);
    }
}

void PathTableViewMenu::createMenu()
{
    addSection(QStringLiteral("List"));

    addAction(m_actions[Action::RemoveItem]);

    m_sections[Section::FileName] = addSection(QStringLiteral("filename"));

    addAction(m_actions[Action::CopyName]);
    addAction(m_actions[Action::OpenPath]);
    addAction(m_actions[Action::DeletePath]);

    m_sections[Section::MultiFiles] = addSection(QStringLiteral("both"));

    addAction(m_actions[Action::OpenMulti]);
}
