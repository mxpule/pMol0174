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
#include "pMolLayer.h"
#include "pMolORF.h"
#include "pMolAnnotation.h"
#include "pMolPrimer.h"

pMolLayer::pMolLayer(): pMolElement(NULL)    { orphan = true;    };

pMolLayer::pMolLayer(pMolXMLbase *p_pMolXMLbase) : pMolElement(p_pMolXMLbase)   {  orphan = true;    }


//-------------------------------------------------------------------------------------------------------------
///handle XML open command. Children can be another annotation or a layer
pMolXMLbase* pMolLayer::XMLopen(const QString &tag, QString &error)
{
  if (tag=="pMolAnnotation")
  { pMolAnnotation *pMolObjectChild = new pMolAnnotation((pMolObject*) this);
    append((pMolElement *) pMolObjectChild);
    return (pMolXMLbase*) pMolObjectChild; 
  }; 

  if (tag=="pMolLayer")
  { pMolLayer *pMolObjectChild = new pMolLayer((pMolObject*) this);
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

  return pMolObject::XMLopen(tag, error);
};


pMolXMLbase* pMolLayer::XMLclose(const QString &tag, const QString &data, QString &error)
{
  if (tag=="pMolLayer")     			return pMolXMLbaseParent; 
  else return pMolElement::XMLclose(tag, data,error);
  return (pMolXMLbase*) this; 
};

QString pMolLayer::getSequence()
{
   return pMolParent->getSequence(); 
};

QString pMolLayer::getComplement()
{ 
   return complementString(getSequence());
};

QString pMolLayer::getSequence(int p_index, int p_length)
{
  return pMolParent->getSequence(p_index, p_length); 
};

int pMolLayer::getLength()
{
  return pMolParent->getLength();
};

int pMolLayer::getIndex()
{
  return pMolParent->getIndex();
}

//-------------------------------------------------------------------------------------------------------------
///return type information
bool pMolLayer::type(int i)
{ 
  if (i == pMolStackObject::LAYER)  return true;
  //return pMolElement::type(i);
  return pMolParent->type(i);
};

//-------------------------------------------------------------------------------------------------------------
///XML save contents
bool pMolLayer::saveContents(QTextStream* stream)
{
  return pMolElement::saveContents(stream);
};

//-------------------------------------------------------------------------------------------------------------
///XML save entire object
bool pMolLayer::save(QTextStream* stream)
{
  XMLsaveOpen(stream, "pMolLayer");
  saveContents(stream);
  XMLsaveClose(stream,"pMolLayer");
  return true;
};

//----------------------------------------------------------------------------
///get element from Elements or from layers!
pMolElement* pMolLayer::getElement(int* i)
{
  int j = *i;
  pMolElement* element;

  if (orphan)
  {
    element = pMolParent->getElement(i);
    if (element!=NULL) return element;
  }

  element = pMolElement::getElement(i);
  if (element!=NULL) *i = j+1;
  return element;
};

pMolElement* pMolLayer::n_getElement(int i)
{
  return pMolElement::getElement(&i);
}

//-------------------------------------------------------------------------------------------------------------
///Makes a deep copy
pMolElement* pMolLayer::duplicate()
{
  pMolLayer* layer = new pMolLayer();	
  layer->name=name;
  layer->setLength(length);
  layer->setIndex(index);
  layer->addComment(getComments());

  foreach (pMolElement* element, pMolElements)
    layer->append(element->duplicate());

  return (pMolElement*) layer;
};

//-------------------------------------------------------------------------------
///convenience member for quickly creating an annotation
void pMolLayer::annotate(const QString &name, const QString &comment, int index, int length)
{
    pMolAnnotation* annotation = new pMolAnnotation();
    annotation->pMolParent = this;
    annotation->setIndex(index);
    annotation->setLength(length);
    annotation->name = name;
    annotation->addComment(getComments());
    pMolElements.append(annotation);
};

QString pMolLayer::getHierarchialName()
{
  if (pMolParent==NULL) return name;
  if (!orphan) return pMolParent->getHierarchialName();
  return pMolParent->getHierarchialName()+"->*"+name;
};
