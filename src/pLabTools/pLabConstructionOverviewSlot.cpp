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
#include "./pLabTools/pLabConstructionTrace.h"
#include "./pLabTools/pLabConstructionOverviewSlot.h"

pLabConstructionOverviewSlot::pLabConstructionOverviewSlot(pMolKernelInterface* p_interface,QTreeWidget *parent, int p_constructionGroupIx, int p_constructionIx) : QTreeWidgetItem(parent)
{
  interface = p_interface;
  constructionIx = p_constructionIx;
  constructionGroupIx = p_constructionGroupIx;
};


pLabConstructionOverviewSlot::pLabConstructionOverviewSlot(pMolKernelInterface* p_interface,QTreeWidgetItem *parent, int p_constructionGroupIx, int p_constructionIx) : QTreeWidgetItem(parent)
{
  interface = p_interface;
  constructionIx = p_constructionIx;
  constructionGroupIx = p_constructionGroupIx;
};

void pLabConstructionOverviewSlot::doubleClicked(int column)
{
  if (constructionIx < 1) return;
  qDebug() << "here we are" << constructionGroupIx << constructionIx;
  pLabConstructionTrace* trace = new pLabConstructionTrace(interface, constructionGroupIx);

  trace->show();
};
