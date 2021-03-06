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
#include <QtSql>

#include "pMolBox.h"

pMolBox::pMolBox(int box)
{
  for (int x=0; x<9; x++)
    for (int y=0; y<9; y++)
      ixArray[x+y*9] = 0;

  QString boxStr;
  boxStr.setNum(box);
  name = "box:"+boxStr;
  QString boxName = name;

   // create the default database connection
  QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

  qDebug() << "sql";
  db.setHostName("db87a.pair.com");
  db.setDatabaseName("mpule2_pMol");
  db.setUserName("mpule2_3_w");
  db.setPassword("a4SzFTDJ");
  bool ok = db.open();
  qDebug() << ok;

  QSqlQuery query;

  query.exec("SELECT name FROM boxes WHERE ix = "+boxStr);
  if (query.next()) 
  {
    boxName =  query.value(0).toString();
  }
  setWindowTitle(boxName);

  query.exec("SELECT ixix, slot FROM samples WHERE present=TRUE AND box = "+boxStr);

  while (query.next()) 
  { 
    int ixix = query.value(0).toInt();
    QString slot = query.value(1).toString();
    if (slot.length()==2) ixArray[coords(slot)] = ixix;
    qDebug() << ixix << slot << slot.length() << coords(slot);
  };

  int radius = 24;
  QGridLayout *layout = new QGridLayout;
  for (int x=0; x<9; x++)
  {
   for (int y=0; y<9; y++)
   { unsigned int r,g,b;
     unsigned int ixix = ixArray[y*9+x];
     if (ixix<=0)
     { pMolSlotWidget* slot = new pMolSlotWidget(radius,"",QColor(0,0,0,0), true);
       layout->addWidget(slot,y,x);
     }
     else 
     { r = ((ixix * 1771) % 101) + 155;
       g = ((ixix *  67) % 128) + 128;
       b = ((ixix * 711) % 128) + 128;
       QColor color(r,g,b,255);
       QString temp;
       pMolSlotWidget* slot = new pMolSlotWidget(radius,temp.setNum(ixix),color, false);
       layout->addWidget(slot, y,x);
     };
    };
  };
  QLabel *label = new QLabel(this);
  label->setFrameStyle(QFrame::Panel | QFrame::Sunken);
  label->setText("first line");
  label->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
  layout->addWidget(label, 9, 2, 1, 7);

  QPushButton *removeButton = new QPushButton("remove");
  layout->addWidget(removeButton, 9, 0, 1, 2);

  setLayout(layout);
/*
  pMolSlotWidget *slot1 = new pMolSlotWidget(23,"1",QColor(127,120,90,255),false);
  pMolSlotWidget *slot2 = new pMolSlotWidget(23,"12",QColor(127,150,40,255),false);
  pMolSlotWidget *slot3 = new pMolSlotWidget(23,"123",QColor(127,130,255,255),false);
  pMolSlotWidget *slot4 = new pMolSlotWidget(23,"1234",QColor(127,200,20,255),false);
  pMolSlotWidget *slot5 = new pMolSlotWidget(23,"12345",QColor(127,100,40,255),false);

    QHBoxLayout *layout;
    layout = new QHBoxLayout;
    layout->addWidget(slot1);
    layout->addWidget(slot2);
    layout->addWidget(slot3);
    layout->addWidget(slot4);
    layout->addWidget(slot5);
    setLayout(layout);

*/
};

pMolBox::~ pMolBox() { };

int pMolBox::coords(const QString &slot)
{
  int x = slot.toUpper().at(0).unicode() - 49;
  int y = slot.toUpper().at(1).unicode() - 65;
  qDebug() << x << y;
  if (x<0) x=0; else x = x % 9;
  if (y<0) y=0; else y = y % 9;
  return x+y*9;
};


void pMolBox::paintEvent(QPaintEvent *)
{
  /*
  QPainter painter(this);
  for (int x=0; x<9; x++)
   for (int y=0; y<9; y++)
   { unsigned int r,g,b;
     unsigned int ixix = ixArray[y*9+x];
     if (ixix<=0)
     { r = 255; g = 255; b = 255; }
     else 
     { r = (ixix * 1771) % 200;
       g = (ixix *  67) % 256;
       b = (ixix * 711) % 256;
     };
     QColor color(r,g,b,255);
     QBrush brush(color);
     QRect rect(x*squareWidth, y*squareWidth, squareWidth, squareWidth);
     painter.fillRect(rect, brush);
   };
*/
};

pMolStackObject* pMolBox::create(pMolKernelInterface* interface, pMolCmd* cmd)
{
  QString error;
  pMolStackList* list = pMolStackList::interpretParameter(cmd, interface,error);
  if (list==NULL) return NULL;
  //if there's one number parameter, cut bit off front or back depending if pos or neg
  if (list->checkParameters(pMolObject::NUMBER, error))
  {
    pMolStackNumber* number = dynamic_cast <pMolStackNumber*> ( list->at(0));
    int n=number->n;
    pMolBox* box = new pMolBox(n);
    box->show();
  };

  return NULL;
};