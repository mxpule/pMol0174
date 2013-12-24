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

#include "pMolMpsv/pMolMpsvWrapper.h"

#include "pMolElement.h"

#include "pMolLayer.h"
#include "pMolMpsv/pMolMpsvWrapper_layer.h"

#include "pMolRea.h"
#include "pMolMpsv/pMolMpsvWrapper_rea.h"

#include "pMolPrimer.h"
#include "pMolMpsv/pMolMpsvWrapper_primer.h"

#include "pMolORF.h"
#include "pMolMpsv/pMolMpsvWrapper_orf.h"

#include "pMolAnnotation.h"
#include "pMolMpsv/pMolMpsvWrapper_annotation.h"

//-----------------------------------------------------------------
///constructors and desctructors
pMolMpsvWrapper::pMolMpsvWrapper()
{
  visible = true;
  recurse = true;
  updated = false;
};

pMolMpsvWrapper::~pMolMpsvWrapper()
{
};

//-----------------------------------------------------------------
bool pMolMpsvWrapper::refresh()
{
  return false;
};

//-----------------------------------------------------------------
///handle user events
void pMolMpsvWrapper::userEvent(int type, int portion)
{
   QString s;
   s = "something happened at the wrapper" + getName();
    qDebug() << s << type << portion;
};

int pMolMpsvWrapper::getElementCount()
{
  return 0;
};

pMolMpsvWrapper* pMolMpsvWrapper::getElement(int i)
{
  return NULL;
};

//-----------------------------------------------------------------
///clips child object in a frame of parent space bounded by i0, i1. 
bool pMolMpsvWrapper::clip(int i0, int i1, int index, int length, int* i, int* l, bool* clip5, bool* clip3, bool* minor)
{
  if (length>0)
  { //positive sense
    //out of frame
    if (index + length <= i0) return false;		//out of bounds left 
    if (index >= i1) return false;			//out of bounds right 

    //have we clipped off the 5' end
    if (index < i0) 
    { *clip5 = true; *i = i0 - index; }
    else
    { *clip5 = false; *i = 0; };

    //have we clipped of the 3' end?
    if (index + length > i1)
    { *clip3 = true; *l = i1 - index - (*i); }
    else
    { *clip3 = false; *l = length - (*i); };

    //if short fragment, then a minor fragment
    if (*l < length / 2) *minor = true; else *minor = false;
  }
  else
  { //negative sense
    //out of frame tests
    if (index + length  >= i1) return false;
    if (index <= i0) return false;

    //have we clipped the 5' end?
    if (index > i1)
    { *clip5 = true; *i = index - i1; }
    else
    { *clip5 = false; *i = 0; };

    //have we clipped the 3' end?
    if (index + length < i0)
    { *clip3 = true; *l = index - i0 - (*i);}
    else
    { *clip3 = false; *l = -(*i) - length; };

    //if shorter than half -- need a better way to do this!
    if (*l < -length/2) *minor = true; else *minor = false;
  };

  return true;
};

//-----------------------------------------------------------------
///static functions to bind elements with wrappers
pMolMpsvWrapper* pMolMpsvWrapper::bind(pMolElement* element)
{
  //bind a layer
  if (element->type(pMolStackObject::LAYER))
  { pMolMpsvWrapper_layer* wrapper = 
      new pMolMpsvWrapper_layer(dynamic_cast <pMolLayer*> (element));
    return (pMolMpsvWrapper*) wrapper;
  };

  //bind a restriction endonuclease annotation
  if (element->type(pMolStackObject::REA))
  { pMolMpsvWrapper_rea* wrapper = 
      new pMolMpsvWrapper_rea(dynamic_cast <pMolRea*> (element));
    return (pMolMpsvWrapper*) wrapper;
  };

  //bind a primer
  if (element->type(pMolStackObject::PRIMER))
  { pMolMpsvWrapper_primer* wrapper = 
      new pMolMpsvWrapper_primer(dynamic_cast <pMolPrimer*> (element));
    return (pMolMpsvWrapper*) wrapper;
  };

  //bind an orf annotation
  if (element->type(pMolStackObject::ORF))
  { pMolMpsvWrapper_orf* wrapper = 
      new pMolMpsvWrapper_orf(dynamic_cast <pMolORF*> (element));
    return (pMolMpsvWrapper*) wrapper;
  };

  //bind an annotation
  if (element->type(pMolStackObject::ANNOTATION))
  { pMolMpsvWrapper_annotation* wrapper = 
      new pMolMpsvWrapper_annotation(dynamic_cast <pMolAnnotation*> (element));
    return (pMolMpsvWrapper*) wrapper;
  };

  //don't know what it is, send NULL back
  return NULL;
}
