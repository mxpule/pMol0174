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

#include "pMolXMLbase.h"

    pMolXMLbase::pMolXMLbase()
    {
      pMolXMLbaseParent = NULL;
      number = -1;
    };


    pMolXMLbase::pMolXMLbase(pMolXMLbase *p_pMolXMLbaseParent)
    {
      pMolXMLbaseParent = p_pMolXMLbaseParent;
      number = -1;
    };




//--------------------------------------------------------------------------
///base member handles opening XML tag by returning pointer to itself
pMolXMLbase* pMolXMLbase::XMLopen(const QString &tag, QString &error)
{
  return this;
};

//--------------------------------------------------------------------------
///base member handles basic data of object - name, number and comments
pMolXMLbase* pMolXMLbase::XMLclose(const QString &tag, const QString &data, QString &error)
{
  if (tag=="name")	name = data; 				else
  if (tag=="number") 	XMLnumeric(&number, data, error);	else
  if (tag=="comment")	comments.append(data);			else
  error+="XML tag:'"+tag+"' not known\n";

  return this;
};

//--------------------------------------------------------------------------
///base member of making nice tree structure
void pMolXMLbase::treeOut(QTreeWidgetItem *tree)
{
  QTreeWidgetItem* w = treeItem(tree,"name",getName());
  treeItem(tree,"comment",getComments());
};

//--------------------------------------------------------------------------
///gets the name with id if has been assigned
QString pMolXMLbase::getName()
{
  if (number<0) return name;

  QString temp;
  return "MP"+temp.setNum(number)+":"+name;
};

//--------------------------------------------------------------------------
///accumulates comments into a single string separated by /n
QString pMolXMLbase::getComments()
{
  QString accumulation;

  foreach (QString comment, comments) 
  { if (!accumulation.isEmpty()) accumulation+="\n";
    accumulation+=comment;
  }
  return accumulation;
};

//--------------------------------------------------------------------------
///adds a comment line
void pMolXMLbase::addComment(const QString &comment)
{
  comments.append(comment);
};


    bool pMolXMLbase::XMLdouble(double *x, const QString &data, QString &error)
    {
        bool ok;
        *x = data.toDouble(&ok);
        if (ok) return true;
        error.append("malformed Base Start" + data + "/n");
        return false;
    };

    bool pMolXMLbase::XMLnumeric(int *x, const QString &data, QString &error)
    {
        bool ok;
        *x = data.toLong(&ok, 10);
        if (ok) return true;
        error.append("malformed Base Start" + data + "/n");
        return false;
    };

    bool pMolXMLbase::XMLboolean(bool* x, const QString &data, QString &error)
    {
      QString data_uppercase = data.toUpper();
      if (data_uppercase=="TRUE")  { *x = true;  return true;};
      if (data_uppercase=="FALSE") { *x = false; return true;};
      error.append("couldn't understand boolean" + data +"/n");
      return false;
    };




    QTreeWidgetItem* pMolXMLbase::treeItem(QTreeWidgetItem* parent, const QString &name)
    {
       QTreeWidgetItem* newItem = new QTreeWidgetItem(parent);
       newItem->setText(0,name);
       return newItem;
    };

    QTreeWidgetItem* pMolXMLbase::treeItem(QTreeWidgetItem* parent, const QString &name, const QString &value)
    {
       QTreeWidgetItem* newItem = new QTreeWidgetItem(parent);
       newItem->setText(0,name);
       newItem->setText(1,value);
       return newItem;
    };

    QTreeWidgetItem* pMolXMLbase::treeItem(QTreeWidgetItem* parent, const QString &name, int value)
    {
       QString temp;
       QTreeWidgetItem* newItem = new QTreeWidgetItem(parent);
       newItem->setText(0,name);
       newItem->setText(1,temp.setNum(value));
       return newItem;
    };

    QTreeWidgetItem* pMolXMLbase::treeItem(QTreeWidgetItem* parent, const QString &name, double value)
    {
       QString temp;
       QTreeWidgetItem* newItem = new QTreeWidgetItem(parent);
       newItem->setText(0,name);
       newItem->setText(1,temp.setNum(value));
       return newItem;
    };

    QTreeWidgetItem* pMolXMLbase::treeItem(QTreeWidgetItem* parent, const QString &name, bool value)
    {
       QString temp;
       QTreeWidgetItem* newItem = new QTreeWidgetItem(parent);
       newItem->setText(0,name);
       if (value) 
          newItem->setText(1,"true");
       else
          newItem->setText(1,"false");
       return newItem;
    };

//----------------------------------------------------------------------------------------------
///outputs a string list in XML format
bool pMolXMLbase::XMLsave(QTextStream* stream, const QString &name, const QStringList &list)
{
  foreach (QString string, list)
    if (!XMLsave(stream, name, string)) return false;
  return true;
};

//----------------------------------------------------------------------------------------------
///outputs a string in XML format
bool pMolXMLbase::XMLsave(QTextStream* stream, const QString &name, const QString &data)
{
  (*stream) << "<"+name+">" << data << "</"+name+">" << endl;
  return true;
};

//----------------------------------------------------------------------------------------------
///outputs a double value in XML format
bool pMolXMLbase::XMLsave(QTextStream* stream, const QString &name, double d)
{
  (*stream) << "<"+name+">" << d << "</"+name+">" << endl;
  return true;
};

//----------------------------------------------------------------------------------------------
///outputs a boolean in XML format
bool pMolXMLbase::XMLsave(QTextStream* stream, const QString &name, bool b)
{
  if (b) 
    (*stream) << "<"+name+">true</"+name+">" << endl;
  else
    (*stream) << "<"+name+">false</"+name+">" << endl;
  return true;
};

//----------------------------------------------------------------------------------------------
///outputs a integer in XML format
bool pMolXMLbase::XMLsave(QTextStream* stream, const QString &name, int i)
{
  (*stream) << "<"+name+">" << i << "</"+name+">" << endl;
  return true;
};

//----------------------------------------------------------------------------------------------
///outputs an opening tag in XML format
bool pMolXMLbase::XMLsaveOpen(QTextStream* stream, const QString &name)
{
  (*stream) << "<"+name+">" << endl;
  return true;
};

//----------------------------------------------------------------------------------------------
///outputs a closing tag in XML format
bool pMolXMLbase::XMLsaveClose(QTextStream* stream, const QString &name)
{
  (*stream) << "</"+name+">" << endl;
  return true;
};
