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
#ifndef PMOLMPSVMENU_H
#define PMOLMPSVMENU_H

#include <QtGui>
#include <QObject>

#include "pMolMpsv/pMolMpsvWrapper_orf.h"


class pMolMpsvMenu : public QObject
{
  Q_OBJECT

  public:
    pMolMpsvMenu(pMolMpsvWrapper_orf* p_parent);
    ~pMolMpsvMenu();

  public slots:
    void pickedSnake();
    void pickedBadness();
    void pickedPlain();
    void pickedPolar();
    void pickedWater();
    void pickedResidue();

  private:
    QMenu* menu;

    pMolMpsvWrapper_orf* parent;

    int highlightMode;
    QAction *actSnake, *actBadness, *actPlain;
    QAction *actPolar, *actWater, *actResidue;

    void cleanup();
};


#endif
