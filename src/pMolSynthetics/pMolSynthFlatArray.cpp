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
#include "pMolSynthetics/pMolSynthFlatArray.h"  

//----------------------------------------------------------------------
///constructors and destructors
pMolSynthFlatArray::pMolSynthFlatArray(pMolStackList* list)
{
  if (list==NULL) 					//if the list is NULL
  { array = NULL;					//pointer is NULL
    count = 0;						//make this object empty
    return;						//finish constructing
  };

  int length = 0;					//length of bases to store
  QStringList seqList;					//list of strings to store

  foreach (pMolStackObject* object, list->list) 	//iterate through list of sequences
  {
    QString seq;

    if (object==NULL) 					//make sure the object isn't NULL
      throw pMolError("pMolSynthFlatArray::create - list of forbidden objects contains a NULL");

    if (object->type(pMolObject::DNA))			//if a regular sequence
    {
      pMolElement* element = dynamic_cast <pMolElement*> (object);
      seq = element->getSequence().toLower();
    }
    else						//if an endonuclease sequence
    if (object->type(pMolObject::RESTRICTIONENDONUCLEASE))
    {
      pMolRestrictionEndonuclease* re = dynamic_cast <pMolRestrictionEndonuclease*> (object);
      seq = re->getSequence().toLower();
    }							//something fishy, throw an error
    else throw pMolError("pMolSynthFlatArray::create - list contains a fish: "+ object->name);

    seqList.append(seq);				//hold on to the sequence
    length += seq.length() + 1;				//remember how much storage we need
  };

  indices = new unsigned int[seqList.count()];		//allocated space for array of indexes
  array = new short unsigned int[length+1];		//allocated space for the sequences
  if ((array==NULL) || (list==NULL))			//memory allocation failed
    throw pMolError("pMolSynthFlatArray::create - couldn't allocate a few meagre bytes");

  max = 0;
  count = 0;						//prepare to loop
  int index = 0;
  foreach (QString seq, seqList)
  {
    indices[count++] = index;				//store the indexes of each sequence in this
							//array
    if (seq.length() > max) max = seq.length();		//maximum length of an element

    for (int i=0; i<seq.length(); i++)			//loop through the characters and flatten
    {
      char c = seq.at(i).toLower().toAscii(); 		//make it flat
      switch (c)
      { 
        case 'a'	 : array[index++] = 0; break;	//
      	case 't'	 : array[index++] = 1; break;	//
        case 'c'	 : array[index++] = 2; break;	//
     	case 'g'	 : array[index++] = 3; break;	//
        default	 : 				//funny base found
        { delete []array;				//note, degenerate bases not yet supported
          delete []indices;				//clean up
          throw pMolError("pMolSynthFlatArray::pMolSynthFlatArray - not a,c,g or t base in forbidden");
        };
      };
    };
    array[index++] = 4;					//stop signal
  };
  for (int i = 0; i<length; i++) qDebug() << array[i];
};


pMolSynthFlatArray::~pMolSynthFlatArray()
{
  if (array!=NULL) 
  { delete []array;
    delete []indices;
  };
};

//----------------------------------------------------------------------
bool pMolSynthFlatArray::isEmpty()
{
  if (count==0) return true;
  return false;
};

//----------------------------------------------------------------------
///returns the array containing the flat sequence
short unsigned int *pMolSynthFlatArray::getArray(int index)
{
  if (index >= count) return NULL;
  return array + indices[index];
};


//----------------------------------------------------------------------
///returns the maximum length of stored sequences
int pMolSynthFlatArray::getMaxLength()
{
  return max;
};