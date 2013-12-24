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
 ************************************************************************/

#ifndef PLABCONSTRUCTIONOVERVIEW_H
#define PLABCONSTRUCTIONOVERVIEW_H

#include <QtGui>
#include <QtSql>
#include <QTreeWidgetItem>

#include "./pMolError/pMolError.h"
#include "./pLabTools/pLabUtils.h"
#include "./pMolSql/pMolSql.h"
#include "./pMolStackObject.h"
#include "./pLabTools/pLabConstructionOverviewSlot.h"

///Martin Pule Sequence Viewer is a flexible Qt QGraphicScene based class for
///visualizing dna sequence data. Inherits both <i>QGraphicsView</i> and also
/// <i>pMolStackObject. 
class pLabConstructionOverview : public QWidget, public pMolStackObject
{
  Q_OBJECT

  public:
    ///constructor - pass the pMolElement you want bound to the object
    pLabConstructionOverview(pMolKernelInterface* p_interface);

    ///desctructor.
    ~pLabConstructionOverview();

    ///static creator
    static pLabConstructionOverview* create(pMolKernelInterface* interface, pMolCmd* cmd);

   public slots:
     void itemDoubleClicked(QTreeWidgetItem *item, int column);

  private :
    void populateList();

    QTreeWidget* itemsTable;

    pMolKernelInterface* interface;
};

#endif
