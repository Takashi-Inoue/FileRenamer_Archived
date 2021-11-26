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

#ifndef RENAMESTATE_INITIAL_H
#define RENAMESTATE_INITIAL_H

#include "RenameStateIState.h"

namespace RenameState {

class Initial : public RenameState::IState
{
public:
    Initial();

    QIcon stateIcon() const override;
    QString stateText() const override;

    bool isAfterRenaming() const override;
    bool readyToRename() const override;
    bool isRenameSucceeded() const override;
    bool isRenameFailed() const override;
};

} // namespace RenameState

#endif // RENAMESTATE_INITIAL_H
