

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

#include "./pLabConstructionOverview.h"

//------------------------------------------------------------------------------
///constructors and destructors

pLabConstructionOverview::pLabConstructionOverview(pMolKernelInterface* p_interface) 
{
  interface = p_interface;

  //make the tree widget
  QStringList labels;
  labels << tr("group/construct") << tr("status") << tr("operator") << tr("notes");
  itemsTable = new QTreeWidget;
  itemsTable->header()->setResizeMode(QHeaderView::Interactive);
  itemsTable->setHeaderLabels(labels);
  itemsTable->setSortingEnabled(true);
  itemsTable->setColumnWidth(0,255);

  QGridLayout *layout = new QGridLayout;
  layout->addWidget(itemsTable);
  resize(800,400);
  populateList();

  setLayout(layout);
  setWindowTitle(tr("construction trace"));

  qDebug() << "here";

  connect(itemsTable, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)),
            this, SLOT(itemDoubleClicked(QTreeWidgetItem *, int)));
};

pLabConstructionOverview::~pLabConstructionOverview()
{
};


void pLabConstructionOverview::itemDoubleClicked(QTreeWidgetItem *item, int column)
{
  try
  {
    pLabConstructionOverviewSlot* slot = 
       dynamic_cast <pLabConstructionOverviewSlot*> (item);
    slot->doubleClicked(column);
  }
  //ERROR HANDLING
  catch (pMolError &error)
  {
    error.detail(interface);		//simply dump trace to terminal
  }
};

//------------------------------------------------------------------------------
///assembles the graphical units vertically and places them on the scene
void pLabConstructionOverview::populateList()
{
  QSqlDatabase db = pMolSql::getConnection("blah");
  QSqlQuery query = QSqlQuery(db);
  QString sqls;
  sqls="SELECT constructionGroups.ix, constructionGroups.name, "
       "constructions.plasmidIx, plasmids.name, constructionStatus.name, "
       "minions.name, constructionNotes.note, constructionStatus.color, "
       "plasmids.ix "
       "FROM constructions, minions, plasmids, constructionStatus, "
       "constructionGroups "
       "LEFT JOIN constructionNotes ON constructionNotes.constructionIx = constructions.ix "
       "WHERE constructions.minionIx = minions.ix AND plasmids.ixix = constructions.plasmidIx "
       "AND constructionStatus.ix = constructions.statusIx AND "
       "constructionGroups.ix = constructions.groupIx "
       "ORDER BY constructions.groupIx, constructions.plasmidIx, constructionNotes.ix DESC;";
  QClipboard *clipboard = QApplication::clipboard();
  clipboard->setText(sqls);

  bool ok = query.exec(sqls);
  if (!ok)
  {  pMolError error = pMolError("pMolSql::exec - failed querying database");
    error.append("command: "+sqls);
    QSqlError sqlError = db.lastError();
    if (sqlError.isValid())
    {
      error.append("driverText: "+sqlError.driverText());
      error.append("databaseText: "+sqlError.databaseText());
    };
    throw error;
  };

  QBrush col1(QColor(0xff,0xff,0xd9,255));
  QBrush col2(QColor(0xdf,0xdf,0xdf,255));

  int oldGroupIx = -1;
  int oldPlasmidsIx = -1;

  while (query.next()) 
  { 

  pLabConstructionOverviewSlot* group;
    int groupIx = query.value(0).toInt();
    if (groupIx!=oldGroupIx)
    { 
      oldGroupIx = groupIx;
      QString groupName = query.value(1).toString();
//      group = new QTreeWidgetItem(itemsTable);
      group = new pLabConstructionOverviewSlot(interface, itemsTable, groupIx, 0);
      group->setText(0, groupName);
      group->setBackground(0,col2);
      group->setBackground(1,col2);
      group->setBackground(2,col2);
      group->setBackground(3,col2);
      group->setExpanded(true);
    };

    int plasmidsIx = query.value(8).toInt();			//only put in the most recent note
    if (plasmidsIx!=oldPlasmidsIx)
    {
      int plasmidIx = query.value(2).toInt();
      QString plasmidName = query.value(3).toString();
      pLabConstructionOverviewSlot* plasmid = 
         new pLabConstructionOverviewSlot(interface, group, groupIx, plasmidIx);
      plasmid->setText(0, QString().setNum(plasmidIx)+":"+plasmidName);
      plasmid->setText(1, query.value(4).toString());
      plasmid->setText(2, query.value(5).toString());
      plasmid->setText(3, query.value(6).toString());
      int constructionStatusColor = query.value(7).toInt();
      plasmid->setBackground(1,QBrush(QColor(QRgb(constructionStatusColor))));
      oldPlasmidsIx = plasmidsIx; 

  };


 };
};
//-------------------------------------------------------------------------------
///static member that creates thus c
pLabConstructionOverview* pLabConstructionOverview::create(pMolKernelInterface* interface, pMolCmd* cmd)
{
  pLabConstructionOverview* overview = new pLabConstructionOverview(interface);
  overview->show();
  return overview;
};
