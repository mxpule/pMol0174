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
#include "pMolStackObject.h"

pMolStackObject::pMolStackObject() : pMolObject(NULL) {};


pMolStackObject::pMolStackObject(pMolXMLbase *p_pMolXMLbaseParent) : pMolObject(p_pMolXMLbaseParent) {};

void pMolStackObject::renderBase(const QString &p_param) {};
void pMolStackObject::render(QPaintEvent *) {};


bool pMolStackObject :: type(int i)
{
  if (i==pMolObject::STACK_OBJECT) return true; else return pMolObject::type(i);
}


//------------------------------------------------------------------------------------------------------------------
///this checks validity of parameter list
pMolStackList* pMolStackList::interpretParameter(pMolCmd* cmd, pMolKernelInterface* interface, QString &error)
{
  return interpretParameter(cmd, interface, NULL, error);
};


//------------------------------------------------------------------------------------------------------------------
///this checks validity of parameter list
pMolStackList* pMolStackList::interpretParameter(pMolCmd* cmd, pMolKernelInterface* interface, pMolObject* parent, QString &error)
{
if (cmd->type()!= pMolCmd::LIST)
  { error = "Parameter was not a list";
    return NULL;
  };

  //interpret out paramaters, bailing if interpretation fails
  pMolObject* object = interface->interpret(cmd, parent);
  if (object==NULL) 
  { error = "Something went wrong interpreting parameter list";
    return NULL;
  }

  //I hate doing this but cast up to an object list
  pMolStackList* list = dynamic_cast <pMolStackList*>(object);
  return list;
};




pMolStackObject* pMolStackObject::makeError(pMolKernelInterface* interface, const QString &member, const QString &error)
{
   interface->msg(name+"->"+member+"::"+error);
   return NULL;

};

pMolStackObject* pMolStackObject::makeError(pMolKernelInterface* interface, const QString &name, const QString &member, const QString &error)
{
   interface->msg(name+"->"+member+"::"+error);
   return NULL;

};


pMolStackObject* pMolStackObject::makeError(const QString &name, const QString &member, const QString &error)
{
   pMolStackError* errorObject = new pMolStackError(name+"->"+member+"::"+error);
   return (pMolStackObject*) errorObject; 
};
//------------------------------------------------------------------------------------------------------------------
///This handles command directed at this oject
pMolStackObject* pMolStackObject::exec(pMolCmd* cmd, pMolKernelInterface* interface)
{
  //no paramaters, return this as object
  if (cmd==NULL) return (pMolStackObject*) this;				//no paramaters, return me

  //if no command but not empty, something is wrong (needs to be empty or have subcommand) so return with error
  if (cmd->type() != pMolCmd::NAME) return makeError(interface,"exec","empty field or parameter list expected");
  pMolCmdName* cmdName = dynamic_cast<pMolCmdName*>(cmd);			//safe to assume this is a pMolCmdName

  return NULL;
  //return command(cmdName, interface);
};







//---------------------------------------------------

pMolStackString :: pMolStackString(const QString &p_string) : pMolStackObject(NULL) { string = p_string; };

void pMolStackString :: treeOut(QTreeWidgetItem *tree)
{
  treeItem(tree,"pMolStackString",string);
};

bool pMolStackString :: type(int i)
{
  if (i==pMolObject::STRING) return true; else return pMolStackObject::type(i);
}

//---------------------------------------------------


//---------------------------------------------------

pMolStackError :: pMolStackError(const QString &p_error) : pMolStackObject(NULL) { error = p_error; };

void pMolStackError :: treeOut(QTreeWidgetItem *tree)
{
  treeItem(tree,"pMolStackError",error);
};

bool pMolStackError :: type(int i)
{
  if (i==pMolObject::ERROR) return true; else return pMolStackObject::type(i);
}


pMolStackNumber :: pMolStackNumber(int p_n) : pMolStackObject(NULL) 
{ n = p_n; };

void pMolStackNumber :: treeOut(QTreeWidgetItem *tree) 
{ treeItem(tree,"pMolCmd::NUMBER",n); };

bool pMolStackNumber :: type(int i)
{ if (i==pMolObject::NUMBER) return true; else return pMolStackObject::type(i);}

//---------------------------------------------------

pMolStackDouble :: pMolStackDouble(double p_n) : pMolStackObject(NULL) 
{ n = p_n; };

void pMolStackDouble :: treeOut(QTreeWidgetItem *tree) 
{ treeItem(tree,"pMolStack::DOUBLE",n); };

bool pMolStackDouble :: type(int i) 
{ if (i==pMolObject::DOUBLE) return true; else return pMolStackObject::type(i); }


//---------------------------------------------------


pMolStackList :: ~pMolStackList() 
{  };

int pMolStackList :: length() { return list.count(); };

void pMolStackList :: append(pMolStackObject* cmd) { list.append(cmd); };

void pMolStackList :: treeOut(QTreeWidgetItem *tree)
{
  QTreeWidgetItem* branch = treeItem(tree,"pMolCmd::LIST");
  foreach (pMolStackObject* object, list) object->treeOut(branch);
};

pMolStackObject* pMolStackList :: at(int i) 
{
  if (i<length()) return list.at(i); else return NULL; 
};

bool pMolStackList :: type(int i)
{
  if (i==pMolObject::LIST) return true; else return pMolStackObject::type(i);
}

//------------------------------------------------------------------------------------------------------------------
///check and individual parameter against being NULL and being the correct type
bool pMolStackList::checkParameter(pMolStackObject* object, int type)
{
  if (object==NULL) return false;
  return object->type(type);
};

//------------------------------------------------------------------------------------------------------------------
///this checks if 1 parameter is correct
bool pMolStackList::checkParameters(int type0, QString &error)
{
  //only one parameter - no good
  if (length()!=1) 
  { error = "wrong number of parameters";
    return false;
  }

  if (!checkParameter(at(0),type0))
  { error = "paramater 1 - wrong type";
    return false;
  };

  return true;
};


//------------------------------------------------------------------------------------------------------------------
///this checks if 2 parameters are correct
bool pMolStackList::checkParameters(int type0, int type1, QString &error)
{
  //only one parameter - no good
  if (length()<2)
  { error = "wrong number of parameters";
    return false;
  }

  if (!checkParameter(at(0),type0))
  { error = "paramater 1 - wrong type";
    return false;
  };

  if (!checkParameter(at(1),type1))
  { error = "paramater 2 - wrong type";
    return false;
  };

  return true;
};

//------------------------------------------------------------------------------------------------------------------
///this checks if 3 parameters are correct
bool pMolStackList::checkParameters(int type0, int type1, int type2, QString &error)
{
  //only one parameter - no good
  if (length()<3)
  { error = "Not enough parameters";
    return false;
  }

  if (!checkParameter(at(0),type0))
  { error = "paramater 1 - wrong type";
    return false;
  };

  if (!checkParameter(at(1),type1))
  { error = "paramater 2 - wrong type";
    return false;
  };

  if (!checkParameter(at(2),type2))
  { error = "paramater 3 - wrong type";
    return false;
  };

  return true;
};

//------------------------------------------------------------------------------------------------------------------
///this checks if 4 parameters are correct
bool pMolStackList::checkParameters(int type0, int type1, int type2, int type3, QString &error)
{
  //only one parameter - no good
  if (length()<4)
  { error = "Not enough parameters";
    return false;
  }

  if (!checkParameter(at(0),type0))
  { error = "paramater 1 - wrong type";
    return false;
  };

  if (!checkParameter(at(1),type1))
  { error = "paramater 2 - wrong type";
    return false;
  };

  if (!checkParameter(at(2),type2))
  { error = "paramater 3 - wrong type";
    return false;
  };

  if (!checkParameter(at(3),type3))
  { error = "paramater 4 - wrong type";
    return false;
  };

  return true;
};

//------------------------------------------------------------------------------------------------------------------
///this checks if 5 parameters are correct
bool pMolStackList::checkParameters(int type0, int type1, int type2, int type3, int type4, QString &error)
{
  //only one parameter - no good
  if (length()<5)
  { error = "Not Enough parameters";
    return false;
  }

  if (!checkParameter(at(0),type0))
  { error = "paramater 1 - wrong type";
    return false;
  };

  if (!checkParameter(at(1),type1))
  { error = "paramater 2 - wrong type";
    return false;
  };

  if (!checkParameter(at(2),type2))
  { error = "paramater 3 - wrong type";
    return false;
  };

  if (!checkParameter(at(3),type3))
  { error = "paramater 4 - wrong type";
    return false;
  };

  if (!checkParameter(at(4),type4))
  { error = "paramater 5 - wrong type";
    return false;
  };

  return true;
};


bool pMolStackList::checkParameter(int i, int type, QString &error)
{
  if (i>=length()) return false;
  return at(i)->type(type);
};




bool pMolStackObject::saveContents(QTextStream* stream)
{
  XMLsave(stream, "name", name);
  XMLsave(stream, "comment", comments);
  if (number>0) XMLsave(stream,"number",number);
  return true;
};


bool pMolStackObject::save(QTextStream* stream)
{
  XMLsaveOpen(stream, "pMolStackObject");
  saveContents(stream);
  XMLsaveClose(stream,"pMolStackObject");
  return true;
};

bool pMolStackObject::save()
{
  QString filename = "/home/mxpule/Dropbox/data/pMolSynch/" + name + ".pMol";
  QFile file(filename);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))  return false;
  QTextStream stream(&file);
  save(&stream);
  file.close();
  return true;
};


//--------------------------------------------------------------------------
///Virtual member that in higher objects will load up any objects dependent on
bool pMolStackObject::loadDependencies(pMolKernelInterface* interface)
{
  qDebug() << "pMolStackObject::loadDependencies" << getName();
  return true;
};

