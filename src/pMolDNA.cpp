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
#include <ctime> 
#include <cstdlib>

#include "pMolDNA.h"
#include "pMolRestrictionEndonuclease.h"

//------------------------------------------------------------------------------------------------------------------
///constructors and destructors
pMolDNA::pMolDNA() : pMolAnnotation(NULL)  {  };

pMolDNA::pMolDNA(pMolXMLbase *p_pMolXMLbase) : pMolAnnotation(p_pMolXMLbase)    {    };

pMolDNA::pMolDNA(const QString &p_sequence)
{
  sequence = p_sequence.toUpper();
  circular = false;
  getLength();
  overhangAtEnd = 0;
  overhangAtStart = 0;
}

pMolDNA::pMolDNA(const QString &p_name, const QString &p_sequence) 
{
  name = p_name;
  sequence = p_sequence;
  circular = false;
  index = 0;
  getLength();
  overhangAtEnd = 0;
  overhangAtStart = 0;
};

//------------------------------------------------------------------------------------------------------------------
///XML open data signal parser
pMolXMLbase* pMolDNA::XMLopen(const QString &tag, QString &error)
{
  if (tag=="pMolAnnotation")
  { pMolAnnotation *pMolObjectChild = new pMolAnnotation((pMolObject*) this);
    append((pMolElement *) pMolObjectChild);
    return (pMolXMLbase*) pMolObjectChild; 
  }; 

  if (tag=="pMolORF")
  { pMolORF *pMolObjectChild = new pMolORF((pMolObject*) this);
    append((pMolElement *) pMolObjectChild);
    return (pMolXMLbase*) pMolObjectChild; 
  }; 

  if (tag=="pMolPrimer")
  { pMolPrimer *pMolObjectChild = new pMolPrimer((pMolObject*) this);
    append((pMolElement *) pMolObjectChild);
    return (pMolXMLbase*) pMolObjectChild; 
  }; 

  return pMolAnnotation::XMLopen(tag, error);
};

//------------------------------------------------------------------------------------------------------------------
///XML close data signal parser
pMolXMLbase* pMolDNA::XMLclose(const QString &tag, const QString &data, QString &error)
{
  if (tag=="sequence") 
  { sequence = cleanString(data);
     index = 0; length = sequence.length(); }
  else if (tag=="circular")      			XMLboolean(&circular, data, error);
  else if (tag=="overhangAtStart")			XMLnumeric(&overhangAtStart, data, error);
  else if (tag=="overhangAtEnd")			XMLnumeric(&overhangAtEnd, data, error);
  else return pMolXMLbase::XMLclose(tag, data,error);
  return (pMolXMLbase*) this; 
};

//-------------------------------------------------------------------------------------------------
///virtual tree out class ouputs hierarchial data of class
void pMolDNA::treeOut(QTreeWidgetItem *tree)
{
  treeItem(tree,"circular", circular);
  //if linear, give descriptions of ends
  if (!circular) 
  { treeItem(tree,"overhangAtStart", overhangAtStart);
    treeItem(tree,"overhangAtEnd", overhangAtEnd);
  }
  pMolAnnotation::treeOut(tree);
}

//-------------------------------------------------------------------------------------------------
///returns type data indicating it contains raw sequence data
bool pMolDNA::type(int i)
{ 
  if (i == pMolStackObject::RAW)  return true;
  return pMolAnnotation::type(i);
};

//-------------------------------------------------------------------------------------------------
///XML save contents
bool pMolDNA::saveContents(QTextStream* stream)
{
  qDebug() << "pMolDNA about to save contents";
  XMLsave(stream, "sequence", sequence);
  XMLsave(stream, "circular", circular);
  if (!circular) 
  { XMLsave(stream,"overhangAtStart", overhangAtStart);
    XMLsave(stream,"overhangAtEnd", overhangAtEnd);
  }
  return pMolAnnotation::saveContents(stream);
};

//-------------------------------------------------------------------------------------------------
///XML save entire object
bool pMolDNA::save(QTextStream* stream)
{
qDebug() << "got back to pMolDNA!";
  XMLsaveOpen(stream, "pMolDNA");
  saveContents(stream);
  XMLsaveClose(stream,"pMolDNA");
  return true;
};


//-------------------------------------------------------------------------------------------------
///return the length of the DNA molecule
int pMolDNA::getLength()
{
  length = sequence.length();
  return length;
};

bool pMolDNA::isCircular()
{ return circular;
};

//-------------------------------------------------------------------------------------------------
///return the raw sequence data of the DNA molecule
QString pMolDNA::getSequence()
{
  return sequence;
};


//------------------------------------------------------------------------------------------------------------------
///create a deep copy of the molecule
pMolElement* pMolDNA::duplicate()
{
  pMolDNA* dna = new pMolDNA();	
  dna->sequence = getSequence();
  dna->circular = circular;
  dna->overhangAtEnd = overhangAtEnd;
  dna->overhangAtStart = overhangAtStart;

  return pMolAnnotation::duplicate((pMolAnnotation*) dna);
};

//------------------------------------------------------------------------------------------------------------------
///create a deep copy duplicate of inverse complement of the molecule
pMolDNA* pMolDNA::complement()
{
  pMolDNA* dna = new pMolDNA();
  dna->sequence = complementString(getSequence());
  dna->name=name+"_complement";
  dna->setLength(length);
  dna->circular = circular;
  dna->setIndex(index);
  dna->overhangAtEnd = overhangAtEnd;
  dna->overhangAtStart = overhangAtStart;

  foreach (pMolElement* element, pMolElements)
  {  pMolElement * temp = element->duplicate();
     temp->setIndex(length - temp->getIndex());
     temp->setLength(-temp->getLength());
     dna->append(temp);
  };

  return dna;
};


//------------------------------------------------------------------------------------------------------------------
///get sequence starting at p_index for p_length bases. Take into account that negative length means inverse
///complement. Also take into account, possible circular nature of this molecule
QString pMolDNA::getSequence(int p_index, int p_length)
{
  getLength();
  if (!circular)
  //not circular, this makes it a bit easier!
  { if (p_length>0) 	return sequence.mid(p_index,p_length);
    else            	return complementString(sequence.mid(p_index+p_length,-p_length));
  }
  else 

  //circular structure - ok a little more complex
  { p_index = p_index % length;					//correct for negative or values > length
    if (p_length>0)						//positive length - sense sequence requested
    {
      if (p_index+p_length < length)				//does it loop around positive
        return sequence.mid(p_index,p_length);			//no - simple, just return the raw sequence
      else							//otherwise add loop around
        return sequence.mid(p_index,length-p_index) + sequence.mid(0,p_index + p_length - length);
    }
    else							//complementary segment requested
    { 
      if (p_index+p_length >= 0)				//does it loop backward around?
        return complementString(sequence.mid(p_index+p_length,-p_length));
      else							//yes so stick the bits together and return
        return complementString(sequence.mid(p_index-p_length+length, -p_index-p_length)+sequence.mid(0,p_index));
    }
  };
};


//------------------------------------------------------------------------------------------------------------------
///cut the plasmid open and return a deep copy of the opened plasmid.
pMolDNA* pMolDNA::cutOpen(int index)
{
  pMolElement* temp;
  pMolDNA* dna = new pMolDNA(); 
  dna->sequence = getSequence(index, length);			//should work since we designed getSequence to handle circle
  dna->name = name+"-cutOpen";
  dna->length = length;
  dna->circular = false;
  dna->overhangAtEnd = 0;
  dna->overhangAtStart = 0;

  foreach (pMolElement* element, pMolElements)
  {
    int c_index = element->getIndex();
    int c_length = element->getLength();
    int c_sum = c_index + c_length;

    if ((c_sum>0) && (c_sum<length))				//is the annotation over the origin? (case D)
    {
      if ((c_index<index) && (c_sum<index))			//is the annotation before the cut site? (case A)
      { temp = element->duplicate();
        temp->setIndex(c_index+length-index);
        append(temp);
      }
      else if((c_index>index) && (c_sum>index))		//is the annotation after the cut site? (case C)
      { temp = element->duplicate();				//duplicate and simply adjust index
        temp->setIndex(c_index - index);
        append(temp);
      }
      else							//annotation is right over the cut site!
      { temp = cutLeftElement(index, element);			//since the element will be duplicated
        temp->setIndex(length - index);				//into a left and right end
        dna->append(temp);					//the index of the left fragment will
        dna->append(cutRightElement(index, element));		//need correction by looping around
      };
    }
    else							//ok now I have a headache!
    {
      if (index > c_index)					//does it start before or after cut site?
      {								//cuts afterwards
        if (c_length > 0)					//positive direction
        {							// status RA
          temp = element->cutLeftSub(index-c_index);
          temp->setIndex(c_index+length-index);
          dna->append(temp);
          temp = element->cutRightSub(index-c_index);
          temp->setIndex(0);
          dna->append(temp);
        }
        else if ((c_sum % length) < index) 			//does it loop right around over origin and cutsite
 	{							//yes it does, shit case RC
          temp = element->cutRightSub(c_length- index + (c_sum % length));
          temp->setIndex(length-c_index);
          dna->append(temp);
          temp = element->cutLeftSub(c_length- index + (c_sum % length));
          temp->setIndex(length - temp->getLength());
          dna->append(temp);
        }
        else
        {							//loops negative over origin but isn't cut
          temp = element->duplicate();				//case RB. Simply duplicate and re-zero`
          temp->setIndex(c_index+length-index);
          dna->append(temp);
        };
      }
      else
      {								//annotation starts after cut site
        if (c_length < 0)					//negative direction do condition RD
        {
          temp = element->cutLeftSub(c_index-index);
          temp->setIndex(c_index-index);
          dna->append(temp);
          temp = element->cutRightSub(c_index-index);
          temp->setIndex(length-1);
          dna->append(temp);
        }
        else if ((c_sum % length) > index)			//does it loop right around midline and cutsite?
        {							//yes it does - shit! Case RF
          temp = element->cutLeftSub(c_length-(c_sum % length) + index);
          temp->setIndex(c_index-index);
          dna->append(temp);
          temp = element->cutRightSub(c_length-(c_sum % length) + index);
          temp->setIndex(0);
          dna->append(temp);
        }
        else 
        {							//loops positive over midline but doesn't get cut
          temp = element->duplicate();				//case RE. Duplicate and re-zero
          temp->setIndex(c_index - index);
          dna->append(temp);
        };
      }
    };
  };								//for each loop
  return dna;							//ok that's all folks
};


//------------------------------------------------------------------------------------------------------------------
///cut the plasmid and return the right segment
pMolDNA* pMolDNA::cutRight(int index)
{
    pMolDNA* dna = new pMolDNA(); 
    dna->sequence = getSequence(index, length-index);
    dna->name = name+"-cutRight";
    dna->setLength(length - index);
    dna->setIndex(0);
    dna->circular = false;
    dna->overhangAtEnd = 0;
    dna->overhangAtStart = 0;

    foreach (pMolElement* element, pMolElements)
      dna->append(cutRightElement(index, element));

    return dna;
};


//------------------------------------------------------------------------------------------------------------------
///cut the plasmid and return the right segment
void pMolDNA::crop5(int index)
{
  sequence = sequence.mid(0,index);
  getLength();
};

//------------------------------------------------------------------------------------------------------------------
///cut the plasmid and return the left segment
pMolDNA* pMolDNA::cutLeft(int index)
{
    pMolDNA* dna = new pMolDNA(); 
    dna->sequence = getSequence(0, index);
    dna->name = name+"-cutLeft";
    dna->setLength(index);
    dna->circular = false;
    dna->setIndex(0);
    dna->overhangAtEnd = 0;
    dna->overhangAtStart = 0;

    foreach (pMolElement* element, pMolElements)
      dna->append(cutLeftElement(index, element));

    return dna;
};

///convenience member for quickly creating an annotation
pMolAnnotation* pMolDNA::extend3(const QString &name, const QString &comment, const QString &addSequence)
{
    int i0 = getLength();
    sequence+=addSequence;
    if (name.isEmpty()) return NULL;

    int i1 = getLength()-i0;

    pMolAnnotation* annotation = new pMolAnnotation();
    annotation->pMolParent = this;
    annotation->setIndex(i0);
    annotation->setLength(i1);
    annotation->name = name;
    annotation->addComment(comment);
    pMolElements.append(annotation);
    return annotation;
};

/*
pMolAnnotation *pMolLayer::splat(const QString &query)
{
  QSqlDatabase db = pMolSql::getConnection("blaj");
  QSqlQuery query;
  bool ok = query.exec("SELECT name FROM supeTypes ORDER BY name");

  QString sequence = getSequence();
  pMolLayer* layer = new pMolLayer(this);

  while (query.next())
    seq_p=query.value(1).toString();
    seq_n=complementString(seq_p);

    if (
      layer->annotate(query.value(0).toString(), i0, l);

  db.close();
};
*/


pMolStackObject* pMolDNA::s_createFromElement(pMolElement* element)
{
  if (element->type(pMolObject::RAW))
  {
    pMolDNA* raw = dynamic_cast <pMolDNA*> (element);
    return (pMolStackObject*) raw->duplicate();
  };

  QString sequence = element->getSequence();
  pMolDNA* dna = new pMolDNA(sequence);
  return (pMolStackObject*) dna;
};

pMolStackObject* pMolDNA::s_createFromString(pMolStackString* s)
{
  QString sequence = s->string;
  pMolDNA* dna = new pMolDNA(sequence);
  return (pMolStackObject*) dna;
};


pMolStackObject* pMolDNA::s_createFromRandom(pMolStackNumber* number)
{
  QString sequence;   
  //srand((unsigned)time(0)); 
  for (int n = 0; n < number->n; n++)
  { int random = int(100.0*rand()/(RAND_MAX + 1.0));
     if (random < 20) sequence += 'a';
     else if (random < 40) sequence +='t';
     else if (random < 70) sequence +='c';
     else sequence += 'g';
  };
  pMolDNA* dna = new pMolDNA(sequence);
  return (pMolStackObject*) dna;
};

pMolStackObject* pMolDNA::s_createFromList(pMolStackList* list)
{
  pMolDNA* dna = new pMolDNA("");
  bool start = true;

  foreach (pMolStackObject* object, list->list)
  { 
    if (object==NULL) 
    { delete dna; return makeError("pMolDNA::","create","incorrect parameters");}

    else if (object->type(pMolObject::DNA))
    {
      pMolElement* element = dynamic_cast <pMolElement*> (object);
      dna->extend3(element->getName(), "", element->getSequence());
    }

    else if (object->type(pMolObject::RESTRICTIONENDONUCLEASE))
    {
      pMolRestrictionEndonuclease* ren = dynamic_cast <pMolRestrictionEndonuclease*> (object);
      dna->extend3(ren->getName(), "", ren->getSequence());
    }

    else 
    { delete dna; return makeError("pMolDNA::","create","paramater "+object->getName()+" contains no sequence data");}
  }
  return (pMolStackObject*) dna;
};

///kernel command dna('CAGTCAGGGGGTTTC') generates new dna molecule
pMolStackObject* pMolDNA::create(pMolKernelInterface* interface, pMolCmd* cmd)
{
  //initial check of parameters
  if (cmd==NULL) 
    return makeError(interface, "pMolDNA", "constructor", "no parameters");				//no parameters sent at all

  if (cmd->type()!=pMolCmd::LIST) 
    return makeError(interface, "pMolDNA", "constructor", "want parameters not command");

  pMolStackList* objectList = dynamic_cast <pMolStackList*> (interface->interpret(cmd));	
  pMolStackObject* product;

  if ((objectList==NULL) || (objectList->length()<1)) 
    product = makeError(interface, "pMolDNA", "constructor", "couldn't understand paramters");

  else if (objectList->length()>1) 
    product = s_createFromList(objectList);

  else if (objectList->at(0)->type(pMolStackObject::DNA))
    product = s_createFromElement(dynamic_cast <pMolElement*> (objectList->at(0)));

  else if (objectList->at(0)->type(pMolStackObject::STRING)) 
    product = s_createFromString(dynamic_cast <pMolStackString*> (objectList->at(0)));

  else if (objectList->at(0)->type(pMolStackObject::NUMBER))
    product = s_createFromRandom(dynamic_cast <pMolStackNumber*> (objectList->at(0)));

  else product = makeError(interface, "pMolDNA::", "constructor", "couldn't understand parameters");

  delete objectList;
  return product;
};
