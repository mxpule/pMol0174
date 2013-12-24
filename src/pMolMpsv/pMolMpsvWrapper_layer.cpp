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

#include "pMolMpsv/pMolMpsvWrapper_layer.h"

//-------------------------------------------------------------------------------
///constructors and destructors
pMolMpsvWrapper_layer::pMolMpsvWrapper_layer(pMolLayer* p_layer)
{
  int i = 0;
  layer = p_layer;

//  parent = pMolMpsvWrapper::bind(layer->pMolParent);

  pMolElement* element;

  //duplicate pMolLayer's list of elements as wrappers
  while ((element = layer->n_getElement(i++))!=NULL)
  { pMolMpsvWrapper* wrapper = pMolMpsvWrapper::bind(element);
    if (wrapper!=NULL) elements.append(wrapper);
  }; 
};

pMolMpsvWrapper_layer::~ pMolMpsvWrapper_layer()
{
  while (!elements.isEmpty()) 
  { pMolMpsvWrapper* wrapper = elements.takeFirst();
    delete wrapper;
  };
};

//--------------------------------------------------------------------------------------------
///return length and index of wrapped pMol object
int pMolMpsvWrapper_layer::getIndex()
{
  return layer->getIndex();				//doesn't mean anything for a layer
};

int pMolMpsvWrapper_layer::getLength()
{
  return layer->getLength();				//doesn't mean anything for a layer
};

QString pMolMpsvWrapper_layer::getName()
{
  return layer->getName();
};

//-------------------------------------------------------------------------------
///get element from Elements or from layers!
pMolMpsvWrapper* pMolMpsvWrapper_layer::getElement(int i)
{
  if (i<elements.count()) return elements.at(i++);
  return NULL;
};

int pMolMpsvWrapper_layer::getElementCount()
{
  return elements.count();
};

//--------------------------------------------------------------------------------
///make the glob 
pMolMpsvGlob* pMolMpsvWrapper_layer::makeGlob(int i0, int i1, pMolMpsvStyle* style, bool root)
{
  //if not the root, transparent, doesn't do anything
  if (!root) return NULL;

  //if the root then pretends it is its parent
  return parent->makeGlob(i0, i1, style, root);
};
