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

#ifndef PMOLKERNEL_H
#define PMOLKERNEL_H

#include <QString>

#include "pMolKernelInterface.h"
#include "pMolLanguageEngine.h"
#include "pMolNameSpace.h"
#include "pMolTerminal.h"
#include "pMolObject.h"
#include "pMolStack.h"

class pMolKernel : public pMolKernelInterface, pMolLanguageEngine
{
  public :
    pMolKernel();
    virtual ~pMolKernel();

    virtual void msg(const QString &p_message);
    virtual void command(QString &p_inputString);
    virtual pMolStackObject* lookup(const QString &p_name);

    virtual void push(pMolObject* object);
    virtual pMolObject* peek(int i);
    virtual void pop(int i);

    virtual pMolObject* interpret(pMolCmd* cmd);
    virtual pMolObject* interpret(pMolCmd* cmd, pMolObject* parent);

  private :
    pMolStack* stack;
    pMolTerminal* terminal;
    pMolNameSpace* nameSpace;

    pMolObject* interpretName(pMolCmdName* cmdName, pMolObject* parent);
    pMolObject* interpretList(pMolCmdList* cmdList, pMolObject* parent);
    pMolObject* interpretNumber(pMolCmdNumber* cmdNumber, pMolObject* parent);
    pMolObject* interpretString(pMolCmdString* cmdString, pMolObject* parent);
};

#endif
