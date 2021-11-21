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

#ifndef BUILDERCHAIN_H
#define BUILDERCHAIN_H

#include <QObject>
#include <QSharedPointer>
#include <QList>

namespace StringBuilder {

class AbstractStringBuilder;

class BuilderChain : public QObject
{
    Q_OBJECT
public:
    BuilderChain(QObject *parent = nullptr);

    virtual void addCreator(QSharedPointer<AbstractStringBuilder> builder);
    virtual QString build();
    bool isEmpty() const;

protected:
    QList<QSharedPointer<AbstractStringBuilder>> m_builders;
};

} // namespace StringBuilder

#endif // BUILDERCHAIN_H
