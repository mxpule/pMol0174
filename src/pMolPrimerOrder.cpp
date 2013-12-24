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

#include "pMolPrimerOrder.h"

pMolPrimerOrder::pMolPrimerOrder(const QString &sql, const QString &a): pMolStackObject()
{
  setAttribute(Qt::WA_DeleteOnClose);
  setWindowTitle(sql);
  setReadOnly(true);
  insertPlainText(a);
}



void pMolPrimerOrder::closeEvent(QCloseEvent *event)
{
   event->accept();
}

bool pMolPrimerOrder::type(int i)
{
  if (i==pMolObject::WINDOW) return true; else return pMolStackObject::type(i);
}


pMolStackObject* pMolPrimerOrder::create(pMolKernelInterface* interface, pMolCmd* cmd)
{
  QString error;

  //if we didn't get any parameters, then send an error and return
  pMolStackList* list = pMolStackList::interpretParameter(cmd, interface,error);
  if (list==NULL) return makeError(interface, "pMolPrimerOrder", "create", error);
  qDebug() << "primer odrer 1";

  //if there's one number parameter, cut bit off front or back depending if pos or neg
  if (!list->checkParameters(pMolObject::STRING, pMolObject::STRING, error))
    return makeError(interface, "pMolPrimerOrder", "create", error);
  qDebug() << "primer odrer 2";

  pMolStackString* t = dynamic_cast <pMolStackString*> (list->at(0));
  QString tp = t->string;

  pMolStackString* s = dynamic_cast <pMolStackString*> (list->at(1));
  QString sql = s->string;

  QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
  db.setHostName("db142e.pair.com");
  db.setDatabaseName("mpule2_PMOLV04");
  db.setUserName("mpule2_20_w");
  db.setPassword("a4SzFTDJ");
  bool ok = db.open();

  if (!ok) return makeError(interface, "pMolPrimerOrder", "create", "couldn't open db");

  QSqlQuery query;

  if (tp=="gblock")
  {   sql = "SELECT ixix, sequence, name FROM gblocks WHERE "+sql;
      tp = "idt";
  }
  else
    sql = "SELECT ixix, sequence, name FROM oligos WHERE "+sql;
  ok = query.exec(sql);
  if (!ok) return makeError(interface, "pMolPrimerOrder", "create", "SELECT command failed: "+sql);

  QString a = "";

  if (tp=="label")
  {
    a+="<html><body><table>\n";
    int i = 0;
    while (query.next()) 
    { 
      if (i % 6 == 0) a+="<tr>\n";
      QString temp;
      int ixix = query.value(0).toInt();
      QString sequence = query.value(1).toString();
      QString name = query.value(2).toString();
      a+="<td><B>MP"+temp.setNum(ixix)+"</B>100uM<br>"+name+"<br>"+sequence.left(8)+"..."+sequence.right(8)+"</td>\n";
      if (++i % 6==0) a+="</tr>\n";
    };
    a+="</table></body></html>\n";
  }
  else if (tp=="label2")
  {
    a+="<html><body><table>\n";
    int i = 0;
    while (query.next()) 
    { 
      if (i % 5 == 0) a+="<tr>\n";
      QString temp;
      int ixix = query.value(0).toInt();
      QString sequence = query.value(1).toString();
      QString name = query.value(2).toString();
      a+="<td><B>MP"+temp.setNum(ixix)+"</B> 100uM<br>"+name+"<br>"+sequence+"</td>\n";
      if (++i % 5==0) a+="</tr>\n";
    };
    a+="</table></body></html>\n";
  }
  else while (query.next()) 
  { 
    QString temp, scale, b;
    int ixix = query.value(0).toInt();
    QString sequence = query.value(1).toString();
    if (tp=="idt") sequence = sequence.toUpper();
    QString name = query.value(2).toString();
    if (sequence.length()<56) scale = "10nM"; else scale = "50nM";
    if (tp=="idt") b="MP"+temp.setNum(ixix)+";"+sequence+";"+name+"\n";
    if (tp=="invitrogen") b=sequence+", MP"+temp.setNum(ixix)+", "+name+"\n";
    if (tp=="obiscum") b="MP"+temp.setNum(ixix)+ ";;;;"+scale+";;Standard;;none;;none;;"+sequence+"\n";
    qDebug() << b;
    a+=b;
  };

  //db.close();
  //QSqlDatabase::removeDatabase("db");
  qDebug() << a;
  pMolPrimerOrder* order = new pMolPrimerOrder(sql, a);
  order->show();
  return (pMolStackObject*) order;
};
