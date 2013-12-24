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
#include "pMolCmd.h"

//----------------------------------------

pMolCmd::pMolCmd()  { };

pMolCmd::~pMolCmd() { };

int pMolCmd::type() { return pMolCmd::OBJ; };

bool pMolCmd::type(int code)
{
  if (code == pMolCmd::OBJ) return true; 
  return false;
};

//----------------------------------------

pMolCmdString :: pMolCmdString(const QString &p_string) { string = p_string; };

int pMolCmdString :: type() { return pMolCmd::STRING; };

bool pMolCmdString::type(int code)
{ 
  if (code==pMolCmd::STRING) return true;
  return pMolCmd::type(code);
};

void pMolCmdString :: treeOut(QTreeWidgetItem *tree)
{
  treeItem(tree,"pMolCmd::STRING",string);
};


//----------------------------------------

pMolCmdNumber :: pMolCmdNumber(int p_n) { n = p_n; };

int pMolCmdNumber :: type() { return pMolCmd::NUMBER; };

bool pMolCmdNumber::type(int code)
{ 
  if (code==pMolCmd::NUMBER) return true;
  return pMolCmd::type(code);
};

void pMolCmdNumber :: treeOut(QTreeWidgetItem *tree)
{
  QString str;
  treeItem(tree,"pMolCmd::NUMBER",str.setNum(n));
};



//----------------------------------------

pMolCmdError :: pMolCmdError(const QString &p_error) { error = p_error; };

int pMolCmdError :: type() { return pMolCmd::ERROR; };

bool pMolCmdError::type(int code)
{ 
  if (code==pMolCmd::ERROR) return true;
  return pMolCmd::type(code);
};

void pMolCmdError :: treeOut(QTreeWidgetItem *tree)
{
  treeItem(tree,"pMolCmd::ERROR",error);
};

//----------------------------------------

pMolCmdName :: pMolCmdName(const QString &p_name)
{ 
  child = NULL;

  if (p_name.startsWith("MP"))
  { mp = p_name.mid(2).toInt();
    qDebug() << "constructed name number" << p_name << mp;
    name = p_name;
  }
  else
  { mp = 0;
    name = p_name;
  };
};

pMolCmdName :: ~pMolCmdName() {  if (child!=NULL) delete child;};

int pMolCmdName :: type() { return pMolCmd::NAME; };

bool pMolCmdName::type(int code)
{ 
  if (code==pMolCmd::NAME) return true;
  return pMolCmd::type(code);
};

void pMolCmdName :: treeOut(QTreeWidgetItem *tree)
{
  QTreeWidgetItem* branch = treeItem(tree,"pMolCmd::NAME",name);
  if (child!=NULL) child->treeOut(branch);
};

//----------------------------------------

pMolCmdChild :: pMolCmdChild(const QString &p_name) : pMolCmdName(p_name) {};

int pMolCmdChild :: type() { return pMolCmd::NAME; };

bool pMolCmdChild::type(int code)
{ 
  if (code==pMolCmd::CHILD) return true;
  return pMolCmdName::type(code);
};

void pMolCmdChild :: treeOut(QTreeWidgetItem *tree)
{
  QTreeWidgetItem* branch = treeItem(tree,"pMolCmd::CHILD",name);
  if (child!=NULL) child->treeOut(branch);
};

//----------------------------------------

pMolCmdList :: ~pMolCmdList() { foreach (pMolCmd* cmd, list) delete cmd; };

int pMolCmdList :: type() { return pMolCmd::LIST; };

bool pMolCmdList::type(int code)
{ 
  if (code==pMolCmd::LIST) return true;
  return pMolCmd::type(code);
};

int pMolCmdList :: length() { return list.count(); };

void pMolCmdList :: append(pMolCmd* cmd) { list.append(cmd); };

pMolCmd* pMolCmdList :: at(int i) 
{
  if (i<length()) return list.at(i); else return NULL; 
};

void pMolCmdList :: treeOut(QTreeWidgetItem *tree)
{
  QTreeWidgetItem* branch = treeItem(tree,"pMolCmd::LIST","----");
  foreach (pMolCmd* cmd, list) cmd->treeOut(branch);
};


//----------------------------------------
