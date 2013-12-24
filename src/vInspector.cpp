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
#include <QtGui>
#include "vInspector.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

vInspector::vInspector(pMolObject* object) :  QObject(), pMolStackObject()
{
   if (object==NULL) return;

   treeWidget = new QTreeWidget();

   QStringList labels;
   labels << "name" << "value";
   treeWidget->setColumnCount(2);
   treeWidget->header()->setResizeMode(QHeaderView::Stretch);
   treeWidget->setHeaderLabels(labels);
   treeWidget->setWindowTitle(object->name);

   //tree = new QTreeItem(QTreeWidgetItem::Type);
   QTreeWidgetItem* branch = new QTreeWidgetItem(QTreeWidgetItem::Type);
   object->treeOut(branch);
   treeWidget->insertTopLevelItem(0, branch);
   treeWidget->expandItem(branch);

   connect(
     treeWidget, 
     SIGNAL(itemClicked(QTreeWidgetItem*, int)), 
     this, 
     SLOT(handleItemPressed(QTreeWidgetItem*, int))
   );

   treeWidget->show();
};


bool vInspector:: type(int i)
{
  if (i==pMolObject::STACK_OBJECT) return true; else return pMolObject::type(i);
}


void vInspector::handleItemPressed(QTreeWidgetItem * item, int column)
{
  qDebug() << "hInsp" << "item pressed" << column;
  if (column==1) treeWidget->openPersistentEditor( item, 1);
};

vInspector* vInspector::create(pMolKernelInterface* interface, pMolCmd* cmd)
{
  //initial check of parameters
  if (cmd==NULL) return NULL;				//no parameters sent at all
  if (cmd->type()!=pMolCmd::LIST) return NULL;	//parameter isn't a list

  //interpret the parameters
  pMolStackList* objectList = dynamic_cast <pMolStackList*> (interface->interpret(cmd));	
  if (objectList==NULL) return NULL;
  //get the length and decide what to do next
  int length = objectList->length();			//get the lenght once
  if (length==0) return NULL;				//leave if no members

  //if more than one ultimately, I want it to return an object list 
  //opens multiple windows - should we put them on a single widget? 
  for (int i=0; i<length; i++)
    if ((objectList->at(i))!=NULL)
    {
      vInspector* inspector = new vInspector((pMolObject*) objectList->at(i));
    }

  //clean up and return
  delete objectList;	
  return NULL;
};

