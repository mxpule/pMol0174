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
#include "pMolPeptide.h"

pMolPeptide::pMolPeptide() : pMolStackObject(NULL) {};

pMolPeptide::pMolPeptide(pMolXMLbase *p_pMolXMLbaseParent) : pMolStackObject(p_pMolXMLbaseParent) {};

//------------------------------------------------------------------------------------------------------------------

///cleans up sequence string ignoring any characters not standard coding
QString pMolPeptide::cleanString(const QString &s)
{
  QString c = "";
  QString aminoacids = "acdefghiklmnpqrstvwy?";

  for (int i=0; i<s.length(); i++)
  {
    QChar x = s.at(i).toLower();
    if (aminoacids.indexOf(x)>=0) c+=x;
  };
  return c.toUpper();
}



///XML close data signal parser
pMolXMLbase* pMolPeptide::XMLclose(const QString &tag, const QString &data, QString &error)
{
  if (tag=="sequence") 
  { sequence = cleanString(data);
      }
  else return pMolXMLbase::XMLclose(tag, data,error);
  return (pMolXMLbase*) this; 
};
///returns infromation type. This is executable, represents DNA but is an annotation
bool pMolPeptide::type(int i)
{ 
  if (i == pMolStackObject::PEPTIDE) return true;
  return pMolStackObject::type(i);
};


QString pMolPeptide::getSequence()
{ 
  return sequence;
};

///kernel command peptide('ygryyppp') generates new peptide molecule
pMolPeptide* pMolPeptide::create(pMolKernelInterface* interface, pMolCmd* cmd)
{
  //initial check of parameters
  if (cmd==NULL) return NULL;				//no parameters sent at all
  if (cmd->type()!=pMolCmd::LIST) 
  { interface->msg("dna::create - was expecting a string");
    return NULL;	//parameter isn't a list
  };

  pMolStackList* objectList = dynamic_cast <pMolStackList*> (interface->interpret(cmd));	
  if (objectList->length()!=1) return NULL;				//leave if no members

  //interpret the parameters
  if (!objectList->at(0)->type(pMolStackObject::STRING)) return NULL;  
  pMolStackString* string = dynamic_cast <pMolStackString*> (objectList->at(0));

  //create the new DNA
  pMolPeptide* peptide = new pMolPeptide();
  peptide->sequence = string->string.toLower();
  //dna->getLength();

  delete objectList;

  return peptide;
};

int pMolPeptide::getLength()
{
  qDebug() << "pMolPeptide::length" << sequence << sequence.length();
  return sequence.length();
}
