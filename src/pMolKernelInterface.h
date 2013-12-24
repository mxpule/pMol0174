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
#ifndef PMOLKERNELINTERFACE_H
#define PMOLKERNELINTERFACE_H


#include <QString>

#include "pMolObject.h"
#include "pMolCmd.h"

class pMolKernelInterface
{
  public:

  pMolKernelInterface() {};
  virtual ~pMolKernelInterface() {};

  void msg(double f)  {QString s; msg(s.setNum(f)); };		///for convenience message a float
  void msg(int i)     {QString s; msg(s.setNum(i)); };		///for convenience message an int
  virtual void msg(const QString &p_message) = 0;		///send a message to the terminal

  virtual void command(QString &p_inputString) = 0;
  virtual pMolObject* lookup(const QString &p_name) = 0;

  virtual void push(pMolObject* object)=0;
  virtual pMolObject* peek(int i)=0;
  virtual void pop(int i)=0;

  virtual pMolObject* interpret(pMolCmd* cmd)=0;
  virtual pMolObject* interpret(pMolCmd*, pMolObject* parent)=0;
};

#endif
