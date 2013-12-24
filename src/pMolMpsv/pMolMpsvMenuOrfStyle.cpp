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
/*
#include "pMolMpsv/pMolMpsvMenuOrfStyle.h"

pMolMpsvMenuOrfStyle::pMolMpsvMenuOrfStyle(pMolMpsvWrapper_orf* p_parent)
{
  parent = p_parent;

  menu = new QMenu();
  QAction *actSnake, *actBadness, *actPlain;
  QAction *actPolar, *actWater, *actResidue;
  actSnake = new QAction(tr("&Snake Stripe"), this);
  actBadness = new QAction(tr("&Badness level"), this);
  actPlain = new QAction(tr("&No Markings"), this);
  actPolar = new QAction(tr("&Polarity"), this);
  actWater = new QAction(tr("&Hydrophobicity"), this);
  actResidue = new QAction(tr("&Residue Colour"), this);

  menu->addAction(actSnake);
  menu->addAction(actBadness);
  menu->addAction(actPlain);
  menu->addAction(actPolar);
  menu->addAction(actWater);
  menu->addAction(actResidue);

  connect(actSnake,   SIGNAL(triggered()), this, SLOT(pickedSnake()));
  connect(actBadness, SIGNAL(triggered()), this, SLOT(pickedBadness()));
  connect(actPlain,   SIGNAL(triggered()), this, SLOT(pickedPlain()));
  connect(actPolar,   SIGNAL(triggered()), this, SLOT(pickedPolar()));
  connect(actWater,   SIGNAL(triggered()), this, SLOT(pickedWater()));
  connect(actResidue, SIGNAL(triggered()), this, SLOT(pickedResidue()));

  menu->exec(QCursor::pos());
};

pMolMpsvMenuOrfStyle::~ pMolMpsvMenuOrfStyle()
{
  cleanup();
};


void pMolMpsvMenuOrfStyle::cleanup()
{
  //how does all this get deleted when it looses focus???
  delete menu;
  delete actSnake;
  delete actBadness;
  delete actPlain;
  delete actPolar;
  delete actWater;
  delete actResidue;
};

void pMolMpsvMenuOrfStyle::pickedSnake()
{
  parent->setHighlightMode(pMolMpsvWrapper_orf::PLAIN);
  cleanup();
};

void pMolMpsvMenuOrfStyle::pickedBadness()
{
  parent->setHighlightMode(pMolMpsvWrapper_orf::BADNESS);
  cleanup();
};

void pMolMpsvMenuOrfStyle::pickedPlain()
{
  parent->setHighlightMode(pMolMpsvWrapper_orf::BLANK);
  cleanup();
};

void pMolMpsvMenuOrfStyle::pickedPolar()
{
  parent->setHighlightMode(pMolMpsvWrapper_orf::CHARGE);
  cleanup();
};

void pMolMpsvMenuOrfStyle::pickedWater()
{
  parent->setHighlightMode(pMolMpsvWrapper_orf::POLARITY);
  cleanup();
};

void pMolMpsvMenuOrfStyle::pickedResidue()
{
  parent->setHighlightMode(pMolMpsvWrapper_orf::RESIDUE);
  cleanup();
};
*/