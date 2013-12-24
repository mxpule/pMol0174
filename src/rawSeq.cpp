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
#include "rawSeq.h"

rawSeq::rawSeq(pMolElement* element): pMolStackObject()
{
  qDebug()<< "reached rawSeq::constructor";
  setAttribute(Qt::WA_DeleteOnClose);
  setWindowTitle(element->name);
  setReadOnly(true);
  qDebug()<< "reached rawSeq::constructor"<<element->getSequence();
  insertPlainText(element->getSequence());
}

rawSeq::rawSeq(pMolPeptide* peptide): pMolStackObject()
{
  qDebug()<< "reached rawSeq::constructor";
  setAttribute(Qt::WA_DeleteOnClose);
  setWindowTitle(peptide->name);
  setReadOnly(true);
  qDebug()<< "reached rawSeq::constructor"<<peptide->getSequence();
  insertPlainText(peptide->getSequence());
}

void rawSeq::closeEvent(QCloseEvent *event)
{
   event->accept();
}

bool rawSeq:: type(int i)
{
  if (i==pMolObject::WINDOW) return true; else return pMolStackObject::type(i);
}


rawSeq* rawSeq::create(pMolKernelInterface* interface, pMolCmd* cmd)
{
    qDebug()<< "reached rawSeq::create";
  //initial check of parameters
  if (cmd==NULL) return NULL;				//no parameters sent at all
  if (cmd->type()!=pMolCmd::LIST) return NULL;	//parameter isn't a list
  //cast up as a list

  pMolCmdList* list = dynamic_cast<pMolCmdList*> (cmd);			//safe to do this since we've
    qDebug()<< "reached rawSeq::first dynamic cast"<<list->length();

  pMolObject* object = interface->interpret(cmd);
  if (object==NULL) return NULL;
  qDebug()<< "reached rawSeq::second dynamic cast" << object->type(pMolObject::LIST);
  //interpret the parameters
  pMolStackList* objectList = dynamic_cast <pMolStackList*> (object);	
  qDebug()<< "reached rawSeq::second dynamic cast" << objectList->type(pMolObject::LIST);
  //get the length and decide what to do next
  int length = objectList->length();			//get the lenght once
  qDebug()<< "reached rawSeq::length" << length;
  if (length==0) return NULL;				//leave if no members

  //if more than one ultimately, I want it to return an object list 
  //opens multiple windows - should we put them on a single widget? 
  for (int i=0; i<length; i++)
  {
    pMolStackObject* stackObject = objectList->at(i);
    qDebug()<< "reached rawSeq::loop" << stackObject->name;
    if (stackObject->type(pMolStackObject::DNA))
    { rawSeq* raw = new rawSeq(dynamic_cast<pMolElement*> (stackObject));
      raw->show();
    }
    if (stackObject->type(pMolObject::PEPTIDE))
    { rawSeq* raw = new rawSeq(dynamic_cast<pMolPeptide*> (stackObject));
      raw->show();
    }
  };
  return NULL;
};

