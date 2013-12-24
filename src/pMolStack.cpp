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
#include "pMolStack.h"

pMolStack::pMolStack(pMolKernelInterface* p_interface) : pMolXMLbase()
{
  interface = p_interface;
  name = "pMolObjectStack";
};

pMolStack::~pMolStack()
{
  pop(stack.size());
};

void pMolStack::push(pMolObject* object)
{
  stack.append(object);
  qDebug() << "push:stack size is" << stack.size();
};

pMolObject* pMolStack::peek(int i)
{
  qDebug() << "peek:stack size is" << stack.size();
  i=stack.size()-i;
  if (i>=0) return stack.at(i); 
  return NULL;
};

void pMolStack::pop(int i)
{
   while ((!stack.empty()) || ((i--)>0))
   { pMolObject* object = stack.takeLast();
     if (object->name.isEmpty()) delete object;
   };
};
