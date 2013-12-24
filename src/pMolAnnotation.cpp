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

#include "pMolRea.h"
#include "pMolAnnotation.h"

pMolAnnotation::pMolAnnotation(): pMolElement(NULL)    {   };

pMolAnnotation::pMolAnnotation(pMolXMLbase *p_pMolXMLbase) : pMolElement(p_pMolXMLbase)   {   };

//-------------------------------------------------------------------------------------------------------------
///handle XML open command. Children can be another annotation or a layer
pMolXMLbase* pMolAnnotation::XMLopen(const QString &tag, QString &error)
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

  if (tag=="pMolPrimer")
  { pMolPrimer *pMolObjectChild = new pMolPrimer((pMolObject*) this);
    append((pMolElement *) pMolObjectChild);
    return (pMolXMLbase*) pMolObjectChild;
 };

  if (tag=="pMolRea")
  { pMolRea* pMolObjectChild = new pMolRea((pMolObject*) this);
    append((pMolElement *) pMolObjectChild);
    return (pMolXMLbase*) pMolObjectChild;
  };

  return pMolObject::XMLopen(tag, error);
};

//-------------------------------------------------------------------------------------------------------------
///handle XML close command
pMolXMLbase* pMolAnnotation::XMLclose(const QString &tag, const QString &data, QString &error)
{
  if (tag=="pMolAnnotation")     			return pMolXMLbaseParent; 
  else return pMolElement::XMLclose(tag, data,error);
  return (pMolXMLbase*) this; 
};

//-------------------------------------------------------------------------------------------------------------
///return type information
bool pMolAnnotation::type(int i)
{ 
  if (i == pMolStackObject::ANNOTATION)  return true;
  return pMolElement::type(i);
};

//-------------------------------------------------------------------------------------------------------------
///XML save contents
bool pMolAnnotation::saveContents(QTextStream* stream)
{
  foreach (pMolElement* layer, pMolLayers)
    layer->save(stream);

  return pMolElement::saveContents(stream);
};

//-------------------------------------------------------------------------------------------------------------
///XML save entire object
bool pMolAnnotation::save(QTextStream* stream)
{
  XMLsaveOpen(stream, "pMolAnnotation");
  saveContents(stream);
  XMLsaveClose(stream,"pMolAnnotation");
  return true;
};


//----------------------------------------------------------------------
///load up any dependencies in child elements
bool pMolAnnotation::loadDependencies(pMolKernelInterface* interface)
{
  int i = 0;
  bool accumulator = true;
  pMolElement* element;

  qDebug() << "pMolAnnotation::loadDependencies" << getName() << pMolElements.count();

  while((element=n_getTransparentElement(i++))!=NULL)
    accumulator = accumulator & element->loadDependencies(interface);

  return accumulator;
};

//----------------------------------------------------------------------
///new-style get functions, eventually make them as standard 
pMolElement* pMolAnnotation::n_getTransparentElement(int i)
{
  if (i<pMolElements.size()) return pMolElements.at(i);
  i-= pMolElements.size();

  foreach (pMolElement* layer, pMolLayers)
  {
    int cc = layer->childCount();
    if (i < cc) return layer->n_getElement(i);
    i-= cc;
  };

  return NULL;
};

pMolElement* pMolAnnotation::n_getElement(int i)
{
  if (i<pMolElements.size()) return pMolElements.at(i);
  return NULL;
};

pMolElement* pMolAnnotation::n_getLayer(int i)
{
  if (i<pMolLayers.size()) return pMolLayers.at(i);
  return NULL;
};

//------------------------------------------------------------------------------
///get element from Elements or from layers!
pMolElement* pMolAnnotation::getElement(int* i)
{
  if (*i<pMolElements.size()) return pMolElements.at((*i)++);
  int j=*i;
  *i-=pMolElements.size();

  foreach (pMolElement* layer, pMolLayers)
  { pMolElement* element = layer->getElement(i);
    if (element!=NULL)
    { *i = j+1; return element; };
  };
  return NULL;
};


//------------------------------------------------------------------------------
///finds a child element in this object by name
pMolElement* pMolAnnotation::getElement(pMolCmdName* cmdName)
{
  foreach (pMolElement* element, pMolElements)
    if (element->is(cmdName)) return element;

  foreach (pMolElement* layer, pMolLayers)
  { if (layer->is(cmdName)) return layer;
    else
    { pMolElement* element = layer->getElement(cmdName);
      if (element!=NULL) return element;
    }
  }

  return NULL;
};

//------------------------------------------------------------------------------------------------------------------
///virtual tree out class ouputs hierarchial data of class
void pMolAnnotation::treeOut(QTreeWidgetItem *tree)
{
  pMolElement::treeOut(tree);

  //output annotations and their subsequent children
  for (int i = 0; i < pMolLayers.size(); ++i) 			//recursively paint
  { QTreeWidgetItem* branch = treeItem(tree,"Layer");
    pMolLayers.at(i)->treeOut(branch);
  };
}


void pMolAnnotation::append(pMolElement* element)
{
  if (element == NULL) return;
  element->pMolParent = this;

  if (element->type(pMolObject::LAYER))
  {
    pMolLayers.append(element);
    pMolLayer* layer = dynamic_cast <pMolLayer*> (element);
    layer->orphan = false;
  }
  else
  {
    pMolElements.append(element);
  }
};


//-------------------------------------------------------------------------------------------------------------
///Makes a deep copy
pMolElement* pMolAnnotation::duplicate()
{
  pMolAnnotation* annotation = new pMolAnnotation();	


  return duplicate(annotation);
};

//-------------------------------------------------------------------------------------------------------------
///Makes a deep copy of innards
pMolElement* pMolAnnotation::duplicate(pMolAnnotation* annotation)
{	
  annotation->setLength(length);
  annotation->setIndex(index);
  annotation->name = name;

  foreach (pMolElement* element, pMolElements)
    annotation->append(element->duplicate());

  foreach (pMolElement* layer, pMolLayers)
    annotation->append(layer->duplicate());

  return (pMolElement*) annotation;
};



//-------------------------------------------------------------------------------
///convenience member for quickly creating an annotation
void pMolAnnotation::annotate(const QString &name, const QString &comment, int index, int length)
{
    pMolAnnotation* annotation = new pMolAnnotation();
    annotation->pMolParent = this;
    annotation->setIndex(index);
    annotation->setLength(length);
    annotation->name = name;
    annotation->addComment(comment);
    pMolElements.append(annotation);
};

//------------------------------------------------------------------------------------------------------------------
///simple chop up sequence
pMolStackObject* pMolAnnotation::seg(int index, int length)
{
  pMolAnnotation* element = new pMolAnnotation();
  element->pMolParent = this;
  element->setIndex(index);
  element->setLength(length);
  return (pMolStackObject*) element;
};




//----------------------------------
///cuts off the 5' bit
pMolStackObject* pMolAnnotation::c_seg5(pMolCmd* cmd, pMolKernelInterface* interface)
{
  QString error;

  //if we didn't get any parameters, then send an error and return
  pMolStackList* list = pMolStackList::interpretParameter(cmd, interface, (pMolObject*) this, error);
  if (list==NULL) return makeError(interface, "seg5", error);

  int n0;
  //if there's one number parameter, cut bit off front or back depending if pos or neg
  if (list->checkParameter(0, pMolObject::NUMBER, error))
  { pMolStackNumber* number = dynamic_cast <pMolStackNumber*> (list->at(0));
    n0=number->n;
  } else
  if (list->checkParameter(0, pMolObject::RAW, error))
  { pMolElement* dna = dynamic_cast <pMolElement*> (list->at(0));
    n0=getSequence().indexOf(dna->getSequence())+dna->getSequence().length();
    if (n0<0) return makeError(interface, "seg5", "sequence param 1 not found");
  } else
  if (list->checkParameter(0, pMolObject::DNA, error))
  { pMolElement* dna = dynamic_cast <pMolElement*> (list->at(0));
    int i,l;
    if (!getRelativeIndex(dna, &i, &l ,error)) return makeError(interface, "seg5",error);
    if (l<0) n0=i - l; else n0 = i + l;
    //need to have something here to make sure the element is in this element
    if ((n0<0)||(n0>getLength())) return makeError(interface, "seg5", "sequence param 1 out of range");
  } else
  makeError(interface, "seg5", "param 1 not understood");

  return seg(0, n0); 
}

//----------------------------------
///cuts off the 3' bit
pMolStackObject* pMolAnnotation::c_seg3(pMolCmd* cmd, pMolKernelInterface* interface)
{
  QString error;

  //if we didn't get any parameters, then send an error and return
  pMolStackList* list = pMolStackList::interpretParameter(cmd, interface, (pMolObject*) this, error);
  if (list==NULL) return makeError(interface, "seg3", error);

  int n0;
  //if there's one number parameter, cut bit off front or back depending if pos or neg
  if (list->checkParameter(0, pMolObject::NUMBER, error))
  { pMolStackNumber* number = dynamic_cast <pMolStackNumber*> (list->at(0));
    n0=number->n;
  } else
  if (list->checkParameter(0, pMolObject::RAW, error))
  { pMolElement* dna = dynamic_cast <pMolElement*> (list->at(0));
    n0=getSequence().indexOf(dna->getSequence());
    if (n0<0) return makeError(interface, "seg3", "sequence param 1 not found");
  } else
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

//------------------------------------------------------------------------------------------------------------------
///we need to make this very sophisticated - able to take multiple inputs and outputs
///aswell as output lists of possibilities
pMolStackObject* pMolAnnotation::seg(pMolCmd* cmd, pMolKernelInterface* interface)
{
  QString error;

  //if we didn't get any parameters, then send an error and return
  pMolStackList* list = pMolStackList::interpretParameter(cmd, interface, (pMolObject*) this, error);
  if (list==NULL) return makeError(interface, "seg", error);

  int n0, n1;
  //if there's one number parameter, cut bit off front or back depending if pos or neg
  if (list->checkParameter(0, pMolObject::NUMBER, error))
  { pMolStackNumber* number = dynamic_cast <pMolStackNumber*> (list->at(0));
    n0=number->n;
  } else
  if (list->checkParameter(0, pMolObject::RAW, error))
  { pMolElement* dna = dynamic_cast <pMolElement*> (list->at(0));
    n0=getSequence().indexOf(dna->getSequence());
    if (n0<0) return makeError(interface, "seg", "sequence param 1 not found");
  } else
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

  //if there's one number parameter, cut bit off front or back depending if pos or neg
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

//------------------------------------------------------------------------------------------------------------------
///This executes commands directed at this object
pMolStackObject* pMolAnnotation::command(pMolCmdName* cmdName, pMolKernelInterface* interface)
{
  if (cmdName->name == "seg")  return seg(cmdName->child, interface);
  if (cmdName->name == "seg5") return c_seg5(cmdName->child, interface);
  if (cmdName->name == "seg3") return c_seg3(cmdName->child, interface);
  return pMolElement::command(cmdName, interface);
};

//------------------------------------------------------------------------------------------------------------------
///This handles command directed at this oject
pMolStackObject* pMolAnnotation::exec(pMolCmd* cmd, pMolKernelInterface* interface)
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
