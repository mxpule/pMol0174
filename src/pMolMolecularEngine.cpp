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
#include "pMolMolecularEngine.h"

pMolDNA* pMolMolecularEngine::ligate(pMolDNA* m1, pMolDNA* m2)
{
  qDebug() << "ligate005";
  int temp;

  if (m1->overhangAtEnd + m2->overhangAtStart != 0) return NULL;
  if (m1->overhangAtEnd > m2->overhangAtStart) temp = m1->overhangAtEnd; else temp = m2->overhangAtStart;
  if (m1->overhangAtEnd!=0) if (m2->getSequence(0,temp)!=m1->getSequence(m1->getLength()-temp, temp)) return NULL;

  pMolDNA* dna1 = m1->cutLeft(m1->getLength()-temp);
  dna1->append(m2);

  //try and circularlize
  if (dna1->overhangAtEnd + dna1->overhangAtStart != 0) return dna1;
  if (dna1->overhangAtEnd > dna1->overhangAtStart) temp = dna1->overhangAtEnd; else temp = dna1->overhangAtStart;
  if (dna1->overhangAtEnd!=0)  if (dna1->getSequence(0,temp)!=dna1->getSequence(dna1->getLength()-temp, temp)) return dna1;

  pMolDNA* dna2 = dna1->cutLeft(dna1->getLength()-temp);

  dna1->deleteElements();
  delete dna1;
  return dna2;
};



void pMolMolecularEngine::ligatePermutations(pMolStackList* list, pMolStackObject* o1, pMolStackObject* o2)
{
  qDebug() << "ligate004";
  if (!o1->type(pMolObject::RAW)) return;
  if (!o2->type(pMolObject::RAW)) return;

  pMolDNA* dna1 = dynamic_cast <pMolDNA*> (o1);
  pMolDNA* dna2 = dynamic_cast <pMolDNA*> (o2);

  list->append(ligate(dna1,dna2));
  list->append(ligate(dna2,dna1));

  pMolDNA* i_dna2 = dna2->complement();
  list->append(ligate(dna1,i_dna2));
  i_dna2->deleteElements();
  delete i_dna2;

  return;
};

pMolStackList* pMolMolecularEngine::ligate(pMolStackList* inList)
{
  qDebug() << "ligate004";
  pMolStackList* outList = new pMolStackList();

  for (int i=0; i<inList->length(); i++)
    for (int j=i; i<inList->length(); j++)
      ligatePermutations(outList, inList->at(i), inList->at(j));

  return outList;
};

pMolStackObject* pMolMolecularEngine::ligate(pMolCmd* cmd, pMolKernelInterface* interface)
{
  qDebug() << "ligate001";
  //if we didn't get any parameters, then send an error and return
  if (cmd==NULL) return NULL;
  if (cmd->type() != pMolCmd::LIST) 
  { interface->msg("pMolecularEngine::ligate - expecting parameter list");return NULL;
  };

  qDebug() << "ligate002";
  //interpret out paramaters, bailing if interpretation fails
  pMolObject* object = interface->interpret(cmd);
  if (object==NULL) {interface->msg("pMolecularEngine::ligate - something went wrong with interpreting a list"); return NULL; };
  if (!object->type(pMolObject::LIST)) {interface->msg("pMolecularEngine::ligate - interpreted parameters are not a list"); return NULL; };

  qDebug() << "ligate003";
  //I hate doing this but cast up to an object list
  pMolStackList* list = dynamic_cast <pMolStackList*>(object);
  if (list->length()<1) {interface->msg("pMolecularEngine::parameter list empty"); return NULL; };

  if (list->at(0)->type(pMolObject::LIST)) 
  {  object = list->at(0);
     list = dynamic_cast <pMolStackList*> (object);
  };

  return ligate(list);
};

//---------------------------------------------------------------------------------------------------------------------
///executes commands referred to this object
pMolStackObject* pMolMolecularEngine::exec(pMolKernelInterface* interface, pMolCmd* cmd)
{
  //no paramaters, return this as object
  if (cmd==NULL) return NULL;				//no paramaters, return me

  //if no command but not empty, something is wrong (needs to be empty or have subcommand) so return with error
  if (cmd->type() != pMolCmd::NAME)
  { interface->msg("pMolecularEngine::exec - empty field or subcommand expected");
    return NULL;
  };
  //first let's look in our own little internal namespace
  pMolCmdName* cmdName = dynamic_cast<pMolCmdName*>(cmd);					//safe to assume this is a pMolCmdName

  if (cmdName->name=="ligate") 		return ligate(cmdName->child, interface);	else
  interface->msg("pMolecularEngine::command not understood");
  return NULL;
};
