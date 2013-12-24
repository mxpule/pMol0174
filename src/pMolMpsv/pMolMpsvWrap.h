/***************************************************************************
 *   Copyright (C) 2006 by Martin Pule   *
 *   martin@pulecyte.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef PMOLMPSVWRAP_H
#define PMOLMPSVWRAP_H


#include <QtGui>
#include <QObject>

///abstract root class of wrappers providing an interface to basic functions and
///handling of user events
class pMolMpsvWrap : public QObject
{
  public:
    ///virtual desctructor
    virtual ~pMolMpsvWrap() { };

    ///handle an event
    virtual void userEvent(int type, int portion) = 0;

    ///get the index of the wrapped class
    virtual int getIndex() = 0;

    ///get the length of the wrapped class
    virtual int getLength() = 0;

    ///get the name of the wrapped class
    virtual QString getName() = 0;
};

#endif
