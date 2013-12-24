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

#include "pMolKernel.h"
//-----------------------------------------------------------------------
///Constructor and Destructor 

///Constructor
pMolKernel::pMolKernel()
{
  stack = new pMolStack((pMolKernelInterface *)this);
  terminal = new pMolTerminal((pMolKernelInterface *)this);
  nameSpace = new pMolNameSpace((pMolKernelInterface *)this);

  terminal->show();
  terminal->msg("hello, this is pMol version 0.149 -");
  terminal->msg("let's create something together.");

  //get terminal to switch on
  //initialize nameSpace with built in functions
};

///Destructor
pMolKernel::~pMolKernel()
{
  delete terminal;
  delete nameSpace;
};

//-----------------------------------------------------------------------

///Interface classes

///Send output to terminal
void pMolKernel::msg(const QString &p_message)
{
  terminal->msg(p_message);
};


///Look something up in the namespace
pMolStackObject* pMolKernel::lookup(const QString &p_name)
{
  pMolStackObject* object = nameSpace->lookUp(p_name);
  if (object!=NULL) object->loadDependencies(this);
  return object;
};


///Execute a command line entry
void pMolKernel::command(QString &p_inputString)
{
  QString inputString = p_inputString;
  inputString = inputString.trimmed();
  int equalsIndex = inputString.indexOf(":=");
  if (equalsIndex > 0)
  {
    QString name = inputString.mid(0,equalsIndex).trimmed();
    inputString = inputString.mid(equalsIndex+2,-1).trimmed();
    qDebug() << "defined" << name << inputString;
    pMolCmd* cmd = parse(&inputString);
    pMolObject* object = interpret(cmd, NULL);
    delete cmd;
    if (object==NULL) return;
    nameSpace->enter(name, dynamic_cast <pMolStackObject*> (object));
  }
  else
  {
    pMolCmd* cmd = parse(&inputString);
    interpret(cmd, NULL);
    delete cmd;
  };
};

//------------------------------------------------------------------------

  void pMolKernel::push(pMolObject* object)
  {
    stack->push(object);
  };

  pMolObject* pMolKernel::peek(int i)
  {
    return stack->peek(i);
  };

  void pMolKernel::pop(int i)
  {
    stack->pop(i);
  };

//-------interpreter methods live here ----------------------------------------------

  ///interprets everything it contains and packages it in a stackList
  pMolObject* pMolKernel::interpretList(pMolCmdList* cmdList, pMolObject* parent)
  {
    pMolStackList* stackList = new pMolStackList();
    foreach (pMolCmd* cmd, cmdList->list) 
    {  pMolStackObject* object = dynamic_cast <pMolStackObject*> (interpret(cmd, parent));
       if (object==NULL) { delete stackList; return NULL; }
       stackList->append(object);
    };
    return (pMolObject*) stackList;
  };

  ///this is slightly more complex. If the name is found in the name space, then this means that
  ///we have a "live" object - i.e. one that is a living object dynamically generated. If it
  ///isn't found, it might be a static function - so look there too! Not very elegent I know 
  ///the first clause handles situations e.g. doblah->blah(->x,->y) handles it so that ->x and ->y are 
  ///children of blah
  pMolObject* pMolKernel::interpretName(pMolCmdName* cmdName, pMolObject* parent)
  {
    if (cmdName->type(pMolCmd::CHILD) && (parent!=NULL))
    { pMolStackObject* object = dynamic_cast <pMolStackObject*> (parent);
      return object->exec((pMolCmd*) cmdName, (pMolKernelInterface*) this);
    };

    return nameSpace->exec(cmdName);
  };

  ///simply interprets a number and packages it in a stackobject how dumb
  pMolObject* pMolKernel::interpretNumber(pMolCmdNumber* cmdNumber, pMolObject* parent)
  {
    pMolObject* object = new pMolStackNumber(cmdNumber->n);
    return (pMolObject*) object;
  };

  ///simply interprets a string and packages it in a stackobject
  pMolObject* pMolKernel::interpretString(pMolCmdString* cmdString, pMolObject* parent)
  {
    pMolObject* object = new pMolStackString(cmdString->string);
    return (pMolObject*) object;
  }; 

  ///generic interpreter of all types
  pMolObject* pMolKernel::interpret(pMolCmd* cmd, pMolObject* parent)
  {
    switch (cmd->type())
    { case pMolCmd::ERROR   : return NULL; break;
      case pMolCmd::NAME    : return interpretName(dynamic_cast<pMolCmdName*>(cmd), parent); break;
      case pMolCmd::NUMBER  : return interpretNumber(dynamic_cast<pMolCmdNumber*>(cmd), parent); break;
      case pMolCmd::STRING  : return interpretString(dynamic_cast<pMolCmdString*>(cmd), parent); break;
      case pMolCmd::LIST    : return interpretList(dynamic_cast<pMolCmdList*>(cmd), parent); break;
      default : return NULL;
    };
  };

  pMolObject* pMolKernel::interpret(pMolCmd* cmd)
  {
    return interpret(cmd, NULL);
  };

