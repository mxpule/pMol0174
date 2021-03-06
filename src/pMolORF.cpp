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

#include "pMolORF.h"
#include "pMolPeptide.h"

pMolORF::pMolORF(): pMolAnnotation(NULL)    {    codonCodex = NULL;   };

pMolORF::pMolORF(pMolXMLbase *p_pMolXMLbase) : pMolAnnotation(p_pMolXMLbase)   {   codonCodex = NULL;    }

pMolORF::pMolORF(const QString &p_name, int p_index, int p_length, const QString &p_codonCodexName)
{
  name = p_name;
  index = p_index;
  length = p_length;
  codonCodexName = p_codonCodexName;
};

pMolXMLbase* pMolORF::XMLclose(const QString &tag, const QString &data, QString &error)
{
  if (tag=="pMolORF")     			return pMolXMLbaseParent; 
  else if (tag=="codonCodex")               	codonCodexName = data; 
  else return pMolElement::XMLclose(tag, data,error);
  return (pMolXMLbase*) this; 
};


//-------------------------------------------------------------------------------------------------------------
///return type information
bool pMolORF::type(int i)
{ 
  if (i == pMolStackObject::ORF)  return true;
  return pMolElement::type(i);
};

//-------------------------------------------------------------------------------------------------------------
///XML save contents
bool pMolORF::saveContents(QTextStream* stream)
{
  XMLsave(stream, "codonCodex", codonCodexName);
  return pMolAnnotation::saveContents(stream);
};

//-------------------------------------------------------------------------------------------------------------
///XML save entire object
bool pMolORF::save(QTextStream* stream)
{
  XMLsaveOpen(stream, "pMolORF");
  saveContents(stream);
  XMLsaveClose(stream,"pMolORF");
  return true;
};

//----------------------------------------------------------------------
///load up any dependencies in child elements
bool pMolORF::loadDependencies(pMolKernelInterface* interface)
{
  int i = 0;
  pMolElement* element;
  bool accumulator = true;

  while((element=n_getTransparentElement(i++))!=NULL)
    accumulator &= element->loadDependencies(interface);

  if (codonCodex!=NULL) return accumulator;

  pMolObject* object = interface->lookup(codonCodexName);
  if (object != NULL) if (object->type(pMolStackObject::CODONCODEX)) 
  { codonCodex = dynamic_cast<pMolCodonCodex*>(object);
    qDebug() << "loaded up codonCodex" << codonCodex->getName() << getComments();
    return accumulator;
  };

  return false;
};

//-------------------------------------------------------------------------------------------------------------
///translates a segment of content
QString pMolORF::translate(int p_index, int p_length)
{
  //get frame
  int frameWidth = codonCodex->frameLength;
  int frameLeft = (p_index) % frameWidth;
  int frameRight = frameWidth - frameLeft;

   QString s2 = "";
   QString s1 = codonCodex->translate(getSequence(p_index-frameLeft,p_length+6));
   QChar le = QChar(32);
   QChar ri = QChar(32);

   QString padLeft;   padLeft.fill(le,frameWidth/2);
   QString padRight;  padRight.fill(ri,(frameWidth/2)-1+(frameWidth%2));

   for (int i=0; i<s1.length(); i++) s2+=padLeft+s1.mid(i,1)+padRight;
   return s2.mid(frameLeft,p_length);
};


//-------------------------------------------------------------------------------------------------------
///create a deep copy of the molecule
pMolElement* pMolORF::duplicate()
{
  pMolORF* orf = new pMolORF();	
  orf->codonCodex = codonCodex;
  orf->codonCodexName = codonCodexName;
  return pMolAnnotation::duplicate((pMolAnnotation*) orf);
};

//-------------------------------------------------------------------------------------------------------
///finds a peptide sequence and returns a layer list
pMolStackObject* pMolORF::findPeptideSequence(pMolPeptide* peptide)
{
  QString p = peptide->getSequence().toLower();
  QString s = codonCodex->translate(getSequence()).toLower();

  pMolLayer* layer = new pMolLayer();
  layer->pMolParent = this;
  QString name = peptide->name;
  if (name.isEmpty()) name = p.left(5);
  QString temp;
  qDebug() << p << s;
  int i=0;
  int frameLength = codonCodex->frameLength;
  while ((i=s.indexOf(p,i))>=0)
  {
    layer->annotate(name+temp.setNum(i),"pMolORF::seg",i*frameLength,p.length()*frameLength);
    i++;
  };

  return (pMolStackObject*) layer;
};



//-----------------------------------------------------------------------------------------------------
///we need to make this very sophisticated - able to take multiple inputs and outputs
///aswell as output lists of possibilities
pMolStackObject* pMolORF::find(pMolCmd* cmd, pMolKernelInterface* interface)
{

  QString error;

  //if we didn't get any parameters, then send an error and return
  pMolStackList* list = pMolStackList::interpretParameter(cmd, interface,error);
  if (list==NULL) return makeError(interface, "find", error);

  //if there's one number parameter, cut bit off front or back depending if pos or neg
  if (list->checkParameters(pMolObject::PEPTIDE, error))
  {
    pMolPeptide* peptide = dynamic_cast <pMolPeptide*> (list->at(0));
    qDebug() << "pMolORF::find peptide" << peptide->getSequence();
    return findPeptideSequence(peptide);
  };

  //if there's one number parameter, cut bit off front or back depending if pos or neg
  if (list->checkParameters(pMolObject::DNA, error))
  {
    pMolElement* dna = dynamic_cast <pMolElement*> (list->at(0));
    return findDNASequence(dna);
  };

  return makeError(interface, "add", error);
};

//----------------------------------------------------------------------------------
///cuts off the 3' bit according to a variety of different inputs
///accepts number, dna sequence, peptide sequence or child object
pMolStackObject* pMolORF::c_seg3(pMolCmd* cmd, pMolKernelInterface* interface)
{
  QString error;

  //if we didn't get any parameters, then send an error and return
  pMolStackList* list = pMolStackList::interpretParameter(cmd, interface, (pMolObject*) this, error);
  if (list==NULL) return makeError(interface, "seg3", error);

  int n0;

  //is it a peptide sequence?
  if (list->checkParameter(0, pMolObject::PEPTIDE, error))
  {
    pMolPeptide* peptide = dynamic_cast <pMolPeptide*> (list->at(0));
    QString p = peptide->getSequence().toLower();
    QString s = codonCodex->translate(getSequence()).toLower();
    n0=s.indexOf(p)*codonCodex->frameLength;
    if (n0<0) return makeError(interface, "seg", "sequence param 1 not found");
  } else

  //is it a number?
  if (list->checkParameter(0, pMolObject::NUMBER, error))
  { pMolStackNumber* number = dynamic_cast <pMolStackNumber*> (list->at(0));
    n0=number->n;
  } else

  //is it a raw dna sequence 
  if (list->checkParameter(0, pMolObject::RAW, error))
  { pMolElement* dna = dynamic_cast <pMolElement*> (list->at(0));
    n0=getSequence().indexOf(dna->getSequence());
    if (n0<0) return makeError(interface, "seg3", "sequence param 1 not found");
  } else

  //is it a child object?
  if (list->checkParameter(0, pMolObject::DNA, error))
  { pMolElement* dna = dynamic_cast <pMolElement*> (list->at(0));
    int i,l;
    if (!getRelativeIndex(dna, &i, &l ,error)) return makeError(interface, "seg3",error);
    if (l<0) n0=i+l; else n0 = i;
    //need to have something here to make sure the element is in this element
    if ((n0<0)||(n0>getLength())) return makeError(interface, "seg3", "sequence param 1 out of range");
  } else
  makeError(interface, "seg3", "param 1 not understood");

  return seg(n0, getLength()-n0); 
}


//-------------------------------------------------------------------------------------
///this takes a pair of a variety of different inputs as reference points and cuts the sequence to flank 
///these two points. Accepts number, dna sequence, peptide sequence, child objects
pMolStackObject* pMolORF::c_seg(pMolCmd* cmd, pMolKernelInterface* interface)
{
  QString error;

  //if we didn't get any parameters, then send an error and return
  pMolStackList* list = pMolStackList::interpretParameter(cmd, interface, (pMolObject*) this, error);
  if (list==NULL) return makeError(interface, "seg", error);

  int n0, n1;
  //Let's handle the first parameter
  //is it a peptide?
  if (list->checkParameter(0, pMolObject::PEPTIDE, error))
  {
    pMolPeptide* peptide = dynamic_cast <pMolPeptide*> (list->at(0));
    QString p = peptide->getSequence().toLower();
    QString s = codonCodex->translate(getSequence()).toLower();
    n0=s.indexOf(p)*codonCodex->frameLength;
    if (n0<0) return makeError(interface, "seg", "sequence param 1 not found");
    interface->msg(n0);
  } else

  //is it a number?
  if (list->checkParameter(0, pMolObject::NUMBER, error))
  { pMolStackNumber* number = dynamic_cast <pMolStackNumber*> (list->at(0));
    n0=number->n;
  } else

  //is it a raw sequence?
  if (list->checkParameter(0, pMolObject::RAW, error))
  { pMolElement* dna = dynamic_cast <pMolElement*> (list->at(0));
    n0=getSequence().indexOf(dna->getSequence());
    if (n0<0) return makeError(interface, "seg", "sequence param 1 not found");
  } else

  //is it a child object
  if (list->checkParameter(0, pMolObject::DNA, error))
  { pMolElement* dna = dynamic_cast <pMolElement*> (list->at(0));
    int i,l;
    if (!getRelativeIndex(dna, &i, &l ,error)) return makeError(interface, "seg",error);
    if (l<0) n0=i+l; else n0 = i;
    //need to have something here to make sure the element is in this element
    QString temp1, temp2;
    if ((n0<0)||(n0>getLength())) return makeError(interface, "seg", "sequence param 1 out of range "+temp1.setNum(i)+" "+temp2.setNum(l));
  } else

  makeError(interface, "seg", "param 1 not understood");



  //Let's handle the second parameter
  if (list->checkParameter(1, pMolObject::PEPTIDE, error))
  {
    pMolPeptide* peptide = dynamic_cast <pMolPeptide*> (list->at(1));
    QString p = peptide->getSequence().toLower();
    QString s = codonCodex->translate(getSequence()).toLower();
    n1=(s.indexOf(p)+p.length())*codonCodex->frameLength;
    if (n1<0) return makeError(interface, "seg", "sequence param 1 not found");
    interface->msg(n1);
  } else 

  if (list->checkParameter(1, pMolObject::NUMBER, error))
  { pMolStackNumber* number = dynamic_cast <pMolStackNumber*> (list->at(1));
    n1=number->n;
  } else

  if (list->checkParameter(1, pMolObject::RAW, error))
  { pMolElement* dna = dynamic_cast <pMolElement*> (list->at(1));
    n1=getSequence().indexOf(dna->getSequence());
    if (n1<0) return makeError(interface, "seg", "sequence param 2 not found");
    n1+=dna->getLength();
  } else

  if (list->checkParameter(1, pMolObject::DNA, error))
  { pMolElement* dna = dynamic_cast <pMolElement*> (list->at(1));
    int i,l;
    if (!getRelativeIndex(dna, &i, &l,error)) return makeError(interface, "seg",error);
    if (l>0) n1=i+l; else n1=i;
    //need to have something here to make sure the element is in this element
    if ((n1<0)||(n1>getLength())) return makeError(interface, "seg", "sequence param 2 out of range");
  } else

  makeError(interface, "seg", "param 2 not understood");

  if (n1>n0) 	return seg(n0, n1-n0); 
	else	return seg(n1, n1-n0);
 
  return makeError(interface, "add", error);
};



//-------------------------------------------------------------------------------------------------------------
///This executes commands directed at this object
pMolStackObject* pMolORF::command(pMolCmdName* cmdName, pMolKernelInterface* interface)
{
  if (cmdName->name == "find") 	return find(cmdName->child, interface);
  if (cmdName->name == "seg")	return c_seg(cmdName->child, interface);
  if (cmdName->name == "seg3") 	return c_seg3(cmdName->child, interface);
  return pMolAnnotation::command(cmdName, interface);
};



//------------------------------------------------------------------------------------------------------------------
///This handles command directed at this oject
pMolStackObject* pMolORF::exec(pMolCmd* cmd, pMolKernelInterface* interface)
{
  //no paramaters, return this as object
  if (cmd==NULL) return (pMolStackObject*) this;				//no paramaters, return me

  //if no command but not empty, something is wrong (needs to be empty or have subcommand) so return with error
  if (cmd->type() != pMolCmd::NAME) return makeError(interface,"exec","empty field or parameter list expected");
  pMolCmdName* cmdName = dynamic_cast<pMolCmdName*>(cmd);			//safe to assume this is a pMolCmdName
  pMolElement* element = getElement(cmdName);
  if (element!=NULL) return element->exec(cmdName->child, interface);

  return command(cmdName, interface);
};