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
#ifndef PMOLBOX_H
#define PMOLBOX_H

#include <QtGui>

#include "pMolSlotWidget.h"
#include "pMolXMLbase.h"
#include "pMolStackObject.h"
#include "pMolCmd.h"
#include "pMolKernelInterface.h"

class pMolBox : public QWidget, pMolStackObject
{
  Q_OBJECT

  public :
    pMolBox(int box);
    ~pMolBox();

    virtual void paintEvent(QPaintEvent *event);
    static pMolStackObject* create(pMolKernelInterface* interface, pMolCmd* cmd);

  private:
    int ixArray[81];
    static const int squareWidth = 24;

    int coords(const QString &slot);
};


#endif
