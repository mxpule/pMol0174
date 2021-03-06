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

#include <QTextBrowser>

#include "pMolError/pMolError.h"
#include "pMolSql/pMolSql.h"
#include "pLabTools/pLabUtils.h"
#include "pLabTools/pLabFreezerBox.h"
#include "pLabTools/pLabToolsSlotWidget.h"
/*


//--------------------------------------------------------------------------------
///constructor
pLabFreezerBox::pLabFreezerBox(pMolKernelInterface* p_interface, int box, int highlightSlotIx)
{
  boxIx = box;
  interface = p_interface;

  shiftDown = false;
  boxNum.setNum(box);
  QString sql, map;
  QString description;
  int max_x, max_y;

  try
  {
    // create the default database connection
    QSqlDatabase db = pMolSql::getConnection("blah");
    QSqlQuery query = QSqlQuery(db);

    //get the box name and set as the title on the window
    sql = "SELECT boxes.name, boxes.ix, boxes.type, boxShapes.map, "
          "boxes.details, boxes.location, types.description FROM boxes, boxShapes, types  "
          "WHERE boxes.shape = boxShapes.ix AND boxes.type = types.ix AND boxes.ix = "+boxNum;
    qDebug() << sql;
    query.exec(sql);
    if (query.next()) 
    { boxName = query.value(0).toString();		//name of the box
      boxType = query.value(2).toInt();			//type of box
      map = query.value(3).toString();			//map of the box
      setWindowTitle("MP"+boxNum+":"+boxName);
      description = "["+query.value(4).toString()+"]";
      description+= "["+query.value(5).toString()+"]";
      description+= "["+query.value(6).toString()+"]";
    }
    else throw pMolError("box not found");

    //make the map of slots
    layout = new QGridLayout;
    parseMap(map, &max_x, &max_y);

    //populate the box slots
    switch(boxType)
    {
      case pLabUtils::BOXTYPE_OLIGOS  	: slotsPopulateOligos(query, highlightSlotIx);
  				 	  boxTypeName = "oligos";	break;
      case pLabUtils::BOXTYPE_PLASMIDS : slotsPopulatePlasmids(query, highlightSlotIx);
				  	  boxTypeName = "plasmids"; 	break;
      case pLabUtils::BOXTYPE_SUPES    : slotsPopulateSupes(query, highlightSlotIx); 
				  	  boxTypeName = "supes";	break;
      case pLabUtils::BOXTYPE_CELLS    : slotsPopulateCells(query, highlightSlotIx); 
				  	  boxTypeName = "cells";	break;
      default 				: throw("box type not known");
    };

   qDebug() << "got to here" << boxTypeName;

  //put in the slots into the frame
  for (int x=0; x<max_x; x++)
  {
    QString temp;
    QLabel* axisLabel;
    axisLabel = new QLabel(temp.setNum(x+1));
    layout->addWidget(axisLabel, 1, p*x+1, 1, p);
  };

  for (int y=0; y<max_y; y++)
  {
    QString temp;
    QLabel* axisLabel;
    axisLabel = new QLabel(QChar(65+y));
    layout->addWidget(axisLabel, p*y+2, 0, p, 1);
  };

  //general layout stuff
  max_y+=2;
  setLayout(layout);
  QSizePolicy policy = QSizePolicy();
  policy.setHorizontalStretch(0);
  policy.setVerticalStretch(0);
  setSizePolicy(policy);

  //labels and buttons
  QLabel* boxLabel = new QLabel(description, this);
  layout->addWidget(boxLabel, 0, 0, 1, max_x*p+1);
 // layout->addWidget(boxCombo, 0, p*1.5, 1, (int)(7.5*p+1));

   qDebug() << "got to here now -2" << boxTypeName;
  QPushButton *addButton = new QPushButton("add");
  layout->addWidget(addButton, max_y*p, 0, 1, 2*p);

  QPushButton *removeButton = new QPushButton("remove");
  layout->addWidget(removeButton, max_y*p, 2*p, 1, 2*p);

  QPushButton *refreshButton = new QPushButton("refresh");
  layout->addWidget(refreshButton, max_y*p, 4*p, 1, 2*p);

  QPushButton *emptyButton = new QPushButton("empty");
  layout->addWidget(emptyButton, max_y*p, 6*p, 1, 2*p);

  QLabel *searchLabel = new QLabel("search");
  layout->addWidget(searchLabel, max_y*p+1, 0, 1, p);

  searchCombo = new QComboBox(this);
  searchCombo->insertItem(1,"blah blah");
  searchCombo->insertItem(2,"more more");
  searchCombo->setEditable(true);
  searchCombo->setInsertPolicy(QComboBox::InsertAtTop);
  layout->addWidget(searchCombo, max_y*p+1, p, 1, (max_x-1)*p+1);

  label = new QTextBrowser(this);
  label->setFrameStyle(QFrame::Panel | QFrame::Sunken);
  label->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
  label->setMaximumHeight(64);
  layout->addWidget(label, max_y*p+2, 0, 2, max_x*p+1);

  //connect up the signals and slots
  connect(searchCombo, SIGNAL(editTextChanged(const QString &)),
            this, SLOT(searchComboChanged(const QString &)));

  connect(searchCombo, SIGNAL(activated(int)),
            this, SLOT(searchComboActivated(int)));

  connect(addButton, SIGNAL(pressed()),
            this, SLOT(addItemPressed()));

  connect(removeButton, SIGNAL(pressed()),
            this, SLOT(removeItemPressed()));
  }

  //ERROR HANDLING
  catch(pMolError &error)
  {
    error.append("pLabFreezerBox::constructor");
    error.detail(interface);
  };

  show();
};

//----------------------------------------------------------------------------------
///destructor
pLabFreezerBox::~ pLabFreezerBox() 
{ 
  foreach (pLabToolsSlotWidget* slotWidget, slotWidgets)
    delete slotWidget;
  slotWidgets.empty();
};


//-----------------------------------------------------------------------
///populates slots with oligos
void pLabFreezerBox::slotsPopulateOligos(QSqlQuery &query, int highlightIx)
{
  int c;
  QString sql, details;
  sql = "SELECT samples.ixix, samples.slot, ";
  sql += "oligos.sequence, oligos.name, oligos.comment ";
  sql += "FROM samples, oligos ";
  sql += "WHERE samples.present=TRUE AND samples.box = "+boxNum+" ";
  sql += "AND oligos.ixix = samples.ixix;";
  query.exec(sql);

  while (query.next()) 
  {
    int ixix = query.value(0).toInt();
    details  = "["+query.value(3).toString()+"]\n";			//name
    details += "["+query.value(2).toString()+"]\n";			//sequence
    details += "["+query.value(4).toString()+"] ";			//comment
    QString slot = query.value(1).toString().toLower().trimmed();

    pLabToolsSlotWidget* slotWidget = slotWidgets.value(slot, NULL);
    if (slotWidget!=NULL) slotWidget->reset(ixix, details, (ixix == highlightIx));	
  };
};
//-----------------------------------------------------------------------
///populates slots with plasmids
void pLabFreezerBox::slotsPopulatePlasmids(QSqlQuery &query, int highlightIx)
{
  int c;
  QString sql, details;

  sql = "SELECT samples.ixix, samples.slot, ";
  sql += "plasmids.name, plasmids.comment ";
  sql += "FROM samples, plasmids ";
  sql += "WHERE samples.present=TRUE AND samples.box = "+boxNum+" ";
  sql += "AND plasmids.ixix = samples.ixix;";
  query.exec(sql);

  while (query.next()) 
  {
    int ixix = query.value(0).toInt();
    details  = "["+query.value(2).toString()+"]\n";
    details += "["+query.value(3).toString().left(64)+"] ";
    QString slot = query.value(1).toString().toLower().trimmed();

    pLabToolsSlotWidget* slotWidget = slotWidgets.value(slot, NULL);		//look up hash table
    qDebug() << "searching for" << slot;
    if (slotWidget==NULL) qDebug() << "not found"; else qDebug() << "found";
    if (slotWidget!=NULL) slotWidget->reset(ixix, details, (ixix == highlightIx));
  };
};
//-----------------------------------------------------------------------
///populates slots with plasmids
void pLabFreezerBox::slotsPopulateCells(QSqlQuery &query, int highlightIx)
{
  int c;
  QString sql, details;

  sql = "SELECT samples.ixix, samples.slot, ";
  sql += "cells.name, cells.comment ";
  sql += "FROM samples, cells ";
  sql += "WHERE samples.present=TRUE AND samples.box = "+boxNum+" ";
  sql += "AND cells.ixix = samples.ixix;";
  query.exec(sql);

  while (query.next()) 
  {
    int ixix = query.value(0).toInt();
    details  = "["+query.value(2).toString()+"]\n";
    details += "["+query.value(3).toString().left(64)+"] ";
    QString slot = query.value(1).toString().toLower().trimmed();

    pLabToolsSlotWidget* slotWidget = slotWidgets.value(slot, NULL);		//look up hash table
    if (slotWidget!=NULL) slotWidget->reset(ixix, details, (ixix == highlightIx));
  };
};

//-----------------------------------------------------------------------
///populates slots with plasmids
void pLabFreezerBox::slotsPopulateSupes(QSqlQuery &query, int highlightIx)
{
  int c;
  QString sql, details;

  sql = "SELECT samples.ixix, samples.slot, ";
  sql += "plasmids.name, supeTypes.name, supes.comment ";
  sql += "FROM samples, supes, plasmids, supeTypes ";
  sql += "WHERE samples.present=TRUE AND samples.box = "+boxNum+" ";
  sql += "AND plasmids.ixix = supes.vectorIxix AND supeTypes.ix = supes.supeTypeIx ";
  sql += "AND supes.ixix = samples.ixix;";
  query.exec(sql);
  qDebug() << sql;

  while (query.next()) 
  {
    int ixix = query.value(0).toInt();
    details  = "["+query.value(2).toString()+"]\n";
    details += "["+query.value(3).toString()+"]\n";
    details += "["+query.value(4).toString().left(64)+"] ";
    QString slot = query.value(1).toString().toLower().trimmed();

    pLabToolsSlotWidget* slotWidget = slotWidgets.value(slot, NULL);
    qDebug() << "searching for" << slot;
    if (slotWidget==NULL) qDebug() << "not found"; else qDebug() << "found";
    if (slotWidget!=NULL) slotWidget->reset(ixix, details, (ixix == highlightIx));
  };
};


//-------------
///format is label:x,y; 
void pLabFreezerBox::parseMap(const QString &map, int* max_x, int* max_y)
{
  int i = 0;						//index in map we've got to
  *max_x = 0; *max_y=0;
  qDebug() << "parseMap" << map;
  qDebug() << map.mid(i) << map.mid(i).trimmed() << map.mid(i).trimmed().length();

  while (map.mid(i).trimmed().length() > 0)	
  {
    //get the first label and coordinates 
    int i_a = map.indexOf(":",i);			//find the ":" before x
    int i_b = map.indexOf(",",i);			//find the "," before y
    int i_c = map.indexOf(";",i);			//find the ";" before end
    if ((i_a < i) || (i_b < i_a) || (i_c < i_b))
      throw pMolError("map notation garbled");

    //get the subsequent substrings
    QString s_a = map.mid(    i, i_a-    i);		//label	
    QString s_b = map.mid(i_a+1, i_b-i_a-1);		//x coordinate
    QString s_c = map.mid(i_b+1, i_c-i_b-1);		//y coordinate

    //get the label
    QString slot = s_a.trimmed().toLower();		//get label
    if (slot.length()<1)				//if not there
      throw pMolError("map label not understood"+map.mid(i,i_c+1));

    //get the coordinates
    bool ok1, ok2;					//for error checking
    int x = s_b.toInt(&ok1);				//get x
    int y = s_c.toInt(&ok2);				//get y
    if ((!ok1) || (!ok2)) 				//not a number
      throw pMolError("couldn't understand map numeral: "+map.mid(i,i_c+1-i));
    if ((x>128) || (y>128))				//too big
      throw pMolError("map coordinates too large: "+map.mid(i,i_c+1-i));
    if ((x<1) || (y<1))					//negative
      throw pMolError("map coordinates are zero or negative: "+map.mid(i,i_c+1-i)); 

    //add the slot
    pLabToolsSlotWidget* slotWidget = new pLabToolsSlotWidget(slot, radius, this);
    if (x>*max_x) *max_x = x;
    if (y>*max_y) *max_y = y;
    slotWidgets.insert(slot, slotWidget);
    layout->addWidget(slotWidget, (y-1)*p+2, (x-1)*p+1, p, p);

    //prepare for the next one
    i = i_c+1;
  };	
};


//----------------------------------------------------------------------------------
///product of a sick mind
void pLabFreezerBox::talk(const QString &msg1)
{
  qDebug() << "someone is saying hi" << msg1 << shiftDown;
  label->setPlainText(msg1);
  label->update();
  slotClicked();
};


void pLabFreezerBox::keyPressEvent ( QKeyEvent * event )
{
  if (event->key() == Qt::Key_Shift) 
  { shiftDown = true; 
    grabKeyboard();
    qDebug() << "press esc"; 
  };
};

void pLabFreezerBox::keyReleaseEvent ( QKeyEvent * event )
{
  if (shiftDown)
  { shiftDown = false; 
    releaseKeyboard();
    qDebug() << "release" << event->key(); 
  };
};

void pLabFreezerBox::slotClicked()
{
  if (!shiftDown)
  foreach (pLabToolsSlotWidget* slotWidget, slotWidgets)
    slotWidget->unHighlight();
};

void pLabFreezerBox::mousePressEvent ( QMouseEvent * event )
{
  foreach (pLabToolsSlotWidget* slotWidget, slotWidgets)
    slotWidget->unHighlight();
};


void pLabFreezerBox::searchComboChanged(const QString &text)
{
  qDebug() << text;
};


//----------------------------------------------------------------------
///adds samples to location database
void pLabFreezerBox::removeItemPressed()
{
  try
  {
    QSqlDatabase db = pMolSql::getConnection("blah");	//get a database connection
    QSqlQuery query;

    foreach (pLabToolsSlotWidget* slotWidget, slotWidgets)
    {
      if ((slotWidget->clicked) && (!slotWidget->empty))
      {
        QString ixix, box, sql;
        sql = "UPDATE samples SET ";
        sql+= "present=0 WHERE ";
        sql+= "ixix=" +ixix.setNum(slotWidget->ixix)+" AND ";
        sql+= "box=" + box.setNum(boxIx)+" AND ";
        sql+= "slot='" + slotWidget->slot+"' "; 
        pMolSql::exec(db, query, sql);

        slotWidget->reset(0, "", false);
        slotWidget->update(0, 0, radius*2+1, radius*2+1);
      };
    };
  }
  //ERROR HANDLING
  catch(pMolError &error)
  {
    error.append("pLabFreezerBox::trying to add samples and somehow failed");
    error.detail(interface);
  };
};

//----------------------------------------------------------------------
///remove item(s)
void pLabFreezerBox::addItemPressed()
{
  qDebug() << "pressed" << foundIxix;
  if (!foundIxix) return;	

  try
  { QSqlDatabase db = pMolSql::getConnection("blah");	//get a database connection
    QSqlQuery query;

    foreach (pLabToolsSlotWidget* slotWidget, slotWidgets)
    { 
      if ((slotWidget->clicked) && (slotWidget->empty))
      { QString ixix, box, sql;
        sql = "INSERT INTO samples SET ";
        sql+= "ixix=" +ixix.setNum(foundIxix)+", ";
        sql+= "box=" + box.setNum(boxIx)+", ";
        sql+= "slot='" + slotWidget->slot+"', "; 
        sql+= "present=1";
        pMolSql::exec(db, query, sql);

        slotWidget->reset(foundIxix,"[MP"+sql+"]["+foundName+"]\n["+foundDetails+"]" ,true);
        slotWidget->update(0, 0, radius*2+1, radius*2+1);
      };
    };
  }
  //ERROR HANDLING
  catch(pMolError &error)
  { error.append("pLabFreezerBox::trying to add samples");
    error.detail(interface);
  };
  return;
};


//----------------------------------------------------------------------
///search for item has been activated, let's look for something
void pLabFreezerBox::searchComboActivated(int index)
{
  try
  { QSqlDatabase db = pMolSql::getConnection("blah");			//connect to database
    QString search = pLabUtils::parseToSql(searchCombo->currentText(), boxTypeName);
    if (search.isNull()) throw pMolError("you searched for nonsense");
    QString sql = "SELECT ixix,name FROM "+boxTypeName+" WHERE "+search+" ORDER BY ixix";

    QSqlQuery query;
    query.exec(sql);

    int i = 0;
    foundIxix = 0;
    searchCombo->clear();

    while (query.next()) 
    {
      QString s1;
      int ixix = query.value(0).toInt();
      QString name = query.value(1).toString();
      if (foundIxix == 0) 
      { foundIxix = ixix;
        foundName = name;
        foundDetails = "some details coming to a cinema near you";
      };
      searchCombo->insertItem(i++,"[MP"+s1.setNum(ixix)+"]["+name+"]");
    }
  }
  catch(pMolError &error)
  {
    error.append("pMolFreezerBox::trying to search");
    error.detail(interface);
  }
};


void pLabFreezerBox::paintEvent(QPaintEvent *)
{
};

pMolStackObject* pLabFreezerBox::create(pMolKernelInterface* interface, pMolCmd* cmd)
{
  QString error;
  pMolStackList* list = pMolStackList::interpretParameter(cmd, interface,error);
  if (list==NULL) return NULL;
  //if there's one number parameter, cut bit off front or back depending if pos or neg
  if (list->checkParameters(pMolObject::NUMBER, error))
  {
    pMolStackNumber* number = dynamic_cast <pMolStackNumber*> ( list->at(0));
    int n=number->n;
    pLabFreezerBox* box = new pLabFreezerBox(interface, n);

  };

  return NULL;
};

*/
