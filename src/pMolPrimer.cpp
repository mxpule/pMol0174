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
#include "pMolPrimer.h"

pMolPrimer::pMolPrimer(): pMolElement(NULL) {
  tail = "";
};

pMolPrimer::pMolPrimer(pMolXMLbase *p_pMolXMLbase) : pMolElement(p_pMolXMLbase)   {    }

//-------------------------------------------------------------------------------------------------------------
///handle XML open command. Children can be another annotation or a layer
pMolXMLbase* pMolPrimer::XMLopen(const QString &tag, QString &error)
{
  return pMolObject::XMLopen(tag, error);
};

pMolXMLbase* pMolPrimer::XMLclose(const QString &tag, const QString &data, QString &error)
{
  if (tag=="pMolPrimer") return pMolXMLbaseParent;
  else if (tag=="tail")
  { qDebug() << "tail"; tail = data; }
  else return pMolElement::XMLclose(tag, data, error);
  return (pMolXMLbase*) this;
};

bool pMolPrimer::type(int i)
{ 
  if (i == pMolStackObject::PRIMER)  return true;
  return pMolElement::type(i);
};

///XML save contents
bool pMolPrimer::saveContents(QTextStream* stream)
{
  if (tail.length()) XMLsave(stream, "tail", tail);
  return pMolElement::saveContents(stream);
};

bool pMolPrimer::save(QTextStream* stream)
{
  XMLsaveOpen(stream, "pMolPrimer");
  saveContents(stream);
  XMLsaveClose(stream,"pMolPrimer");
  return true;
};

QString pMolPrimer::getSequence()
{
    return tail + pMolElement::getSequence();
};

pMolElement* pMolPrimer::getElement(int* i)
{
  return NULL;
};

pMolElement* pMolPrimer::getElement(const QString &p_name)
{
  return NULL;
};

pMolElement* pMolPrimer::duplicate()
{
  pMolPrimer* primer = new pMolPrimer();
  primer->name = name;
  primer->addComment(getComments());
  primer->setIndex(index);
  primer->setLength(length);

  return (pMolElement*) primer;
};

void pMolPrimer::setTail(const QString &p_tail)
{
  tail = p_tail;
}

void pMolPrimer::annotate(const QString &name, const QString &comment, int index, int length)
{
};

bool pMolPrimer::upload(QSqlDatabase &db, bool sequential, QString &error)
{
  bool ok;
  int ixix;
  bool makenew;
  QString temp;
  QSqlQuery query(db);
  QString sequence = getSequence().toLower();

  //first see if the primer exists in the database already
  if (!sequential)
  {
  ok = query.exec("SELECT ixix FROM oligos WHERE sequence='"+sequence+"'");
  if (!ok || (query.size()<0))
    { error ="pMolPrimer::upload - query into oligose failed\n";
      error+=query.lastError().text();
      return false;
    }
  if (query.size()==0) makenew = true;  else makenew = false;
  } else makenew = true;

  //if doesn't exist, make new oligo linked to ix table
  if (makenew)
  { query.exec("INSERT INTO ix SET typeIx = 9");
    QVariant v = query.lastInsertId();
    if (!v.isValid())
    { error="pMolPrimer::upload - Insert into ix returned invalid insert id";
      return false;
    };
    ixix = v.toInt(&ok);
    if (!ok)
    { error="pMolPrimer::upload - inssert into ix insert id failed conversion to int";
      return false;
    };
    QString sql;
    sql = "INSERT INTO oligos SET ";
    sql+= "sequence ='"+sequence+"', ";
    sql+= "name ='"+name+"', ";
    sql+= "comment ='"+getComments()+"', ";
    sql+= "ixix ="+temp.setNum(ixix);
    qDebug() << "pMolPrimer::dumpPrimers " << sql;
    ok = query.exec(sql);
    if (!ok)
    { error ="pMolPrimer::upload - Insert primer sequence failed\n";
      error+=query.lastError().text();
      return false;
    }
  }
  //if it does exist, get its details!
  else
  {
    query.next();
    QVariant v = query.value(0);
    if (!v.isValid())
    { error="pMolPrimer::upload - extracting ixix from table oligos failed";
      return false;
    };
    ixix = v.toInt(&ok);
    if (!ok)
    { error="pMolPrimer::upload - ixix from table oligos failed conversion to int";
      return false;
    };
  }

  number = ixix;
  mutate();
  return true;
};

/*
bool pMolPrimer::scan(pMolKernelInterface* interface)
{
  

  bool ok;
  int ixix;
  QString temp;
  QSqlQuery query(db);
  QString sequence = getSequence().toLower();

  //first see if the primer exists in the database already
  ok = query.exec("SELECT ixix FROM oligos "+ com);
  if (!ok || (query.size()<0)
  throw pMolError("pMolOligo::scan - database error:"+query.lastError().text()));

  while not eof
  {
    seq=getSequence();
    element->find(sequence);
  //if doesn't exist, make new oligo linked to ix table
  if (query.size()==0)
  { query.exec("INSERT INTO ix SET typeIx = 9");
    QVariant v = query.lastInsertId();
    if (!v.isValid())
    { error="pMolPrimer::upload - Insert into ix returned invalid insert id";
      return false;
    };
    ixix = v.toInt(&ok);
    if (!ok)
    { error="pMolPrimer::upload - inssert into ix insert id failed conversion to int";
      return false;
    };
    QString sql;
    sql = "INSERT INTO oligos SET ";
    sql+= "sequence ='"+sequence+"', ";
    sql+= "name ='"+name+"', ";
    sql+= "comment ='"+getComments()+"', ";
    sql+= "ixix ="+temp.setNum(ixix);
    qDebug() << "pMolPrimer::dumpPrimers " << sql;
    ok = query.exec(sql);
    if (!ok)
    { error ="pMolPrimer::upload - Insert primer sequence failed\n";
      error+=query.lastError().text();
      return false;
    }
  }
  //if it does exist, get its details!
  else
  {
    query.next();
    QVariant v = query.value(0);
    if (!v.isValid())
    { error="pMolPrimer::upload - extracting ixix from table oligos failed";
      return false;
    };
    ixix = v.toInt(&ok);
    if (!ok)
    { error="pMolPrimer::upload - ixix from table oligos failed conversion to int";
      return false;
    };
  }

  number = ixix;
  return true;
};
*/
