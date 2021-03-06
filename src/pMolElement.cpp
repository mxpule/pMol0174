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
#include <QtSql>
#include <QString>

#include "pMolElement.h"
#include "pMolLayer.h"
#include "pMolPrimer.h"

    pMolElement::pMolElement() : pMolStackObject(NULL) {};


    pMolElement::pMolElement(pMolXMLbase *p_pMolXMLbaseParent) : pMolStackObject(p_pMolXMLbaseParent)
    {
      pMolParent = dynamic_cast<pMolElement*>(p_pMolXMLbaseParent); 
    };


 void pMolElement::deleteElements()
 {
   foreach (pMolElement* element, pMolElements) delete element;
 };

bool pMolElement::is(pMolCmdName* cmdName)
{
  if (cmdName->mp>0)
  { qDebug() << "is::number" << number << cmdName->mp;
    if (number == cmdName->mp) return true; else return false; }
  else
  { qDebug() << "is::name" << name << cmdName->name;
    if (name == cmdName->name) return true; else return false;  }
};

//------------------------------------------------------------------------------------------------------------------
///Loads up the XML data
pMolXMLbase* pMolElement::XMLclose(const QString &tag, const QString &data, QString &error)
{
  if (tag=="pMolElement")     			return pMolXMLbaseParent; 
  else if (tag=="index")               	XMLnumeric(&index, data, error);
  else if (tag=="length") 	      		XMLnumeric(&length, data, error);
  else return pMolXMLbase::XMLclose(tag, data,error);
  return (pMolXMLbase*) this; 
};


//------------------------------------------------------------------------------------------------------------------
///returns infromation type. This is executable, represents DNA but is an annotation
bool pMolElement::type(int i)
{ 
  if (i == pMolStackObject::EXEC) return true;
  if (i == pMolStackObject::DNA)  return true;
  if (i == pMolStackObject::ANNOTATION)  return true;
  return pMolStackObject::type(i);
};

//------------------------------------------------------------------------------------------------------------------
///virtual tree out class ouputs hierarchial data of class
void pMolElement::treeOut(QTreeWidgetItem *tree)
{
  pMolXMLbase::treeOut(tree);
  treeItem(tree,"index",index);
  treeItem(tree,"length", length);

  //output annotations and their subsequent children
  for (int i = 0; i < pMolElements.size(); ++i) 			//recursively paint
  { QTreeWidgetItem* branch = treeItem(tree,"annotation");
    pMolElements.at(i)->treeOut(branch);
  };
}

bool pMolElement::saveContents(QTextStream* stream)
{
  XMLsave(stream, "index", index);
  XMLsave(stream, "length", length);
  foreach (pMolElement* element, pMolElements) element->save(stream);
  return pMolStackObject::saveContents(stream);
};



//------------------------------------------------------------------------------------------------------------------
///cleans up sequence string ignoring any characters not standard coding
QString pMolElement::cleanString(const QString &s)
{
  QString c = "";
  QString bases = "cgatrykmswnbvdh";
  for (int i=0; i<s.length(); i++)
  {
    QChar x = s.at(i).toLower();
    if (bases.indexOf(x)>=0) c+= x;
  }
  return c.toUpper();
}

QString pMolElement::complementString(const QString &s)
{
   QString c = "";
   QString bases = "cgatrykmswnbvdh";
   QString complement = "gctayrmkswnvbhd";

   for (int i=s.length()-1; i>=0; i--)
   {
     QChar x = s.at(i).toLower();
     int j = bases.indexOf(x);
     if (j>=0) c+= complement.at(j); else c+= "?";
   }
   return c.toUpper();
}

//------------------------------------------------------------------------------------------------------------------
/// returns appropriate sequence segment of the DNA sequence.
/// not as simple as might first appear since I want it to accept negative
/// indices and indices over the length of the plasmid if it is circular
/// By convention the direction is ascribed by the order of i0 and i1. 
/// i.e. if i0 less than i1 then complementary. To boundary backwards 
    /// e.g. plasmid of 1000 say (100,-100) or (1100,900) is complementary
    /// while (100,900) is sense. If plasmid is not circular, this doesn't hold

QString pMolElement::getSequence()
{
   return getSequence(0, length); 
};

QString pMolElement::getComplement()
{ 
   return complementString(getSequence());
};

QString pMolElement::getSequence(int p_index, int p_length)
{
  //qDebug() << "pMolELement::getSequence" << name << p_index << p_length << pMolParent->getSequence(p_index + index, p_length);
  if (p_length>=0) 
    return pMolParent->getSequence(p_index + index, p_length); 
  else
    return pMolParent->getSequence(p_index + index, p_length);
};


int pMolElement::getLength()
{
  return length;
};

int pMolElement::getIndex()
{
  return index;
};

void pMolElement::setLength(int p_length)
{
  length = p_length;
};

void pMolElement::setIndex(int p_index)
{
  index = p_index;
};


bool pMolElement::isCircular()
{
  return false;
};

int pMolElement::childCount()
{
  return pMolElements.size();
};


QString pMolElement::getHierarchialName()
{
  if (pMolParent==NULL) return name;
  return pMolParent->getHierarchialName()+"->"+name;
};

//-------------------------------------------------------------------------------------------------------------
///get element from Elements or from layers!
pMolElement* pMolElement::getElement(int* i)
{
  if (*i<pMolElements.size()) return pMolElements.at((*i)++);
  *i-=pMolElements.size();
  return NULL;
};

pMolElement* pMolElement::n_getElement(int i)
{
  if (i >= pMolElements.count()) return NULL;
  return pMolElements.at(i);
};

//------------------------------------------------------------------------------------------------------------------
///finds a child element in this object by name
pMolElement* pMolElement::getElement(pMolCmdName* cmdName)
{
  foreach (pMolElement* element, pMolElements)
    if (element->is(cmdName)) return element;

  return NULL;
};

//------------------------------------------------------------------------------------------------------------------
///
bool pMolElement::getRelativeIndex(pMolElement* dna, int* index, int* length, QString &error)
{
  *index = 0;
  *length = dna->getLength();

  while (dna!=this)
  {
    if (dna->pMolParent==NULL)
    { error+= "element in list didn't refer back";
      return false;
    }
    //always convert to the coordinates of the parent object
    int i = dna->getIndex();
    int l = dna->getLength();
    dna = dna->pMolParent;
    if (l>0) (*index)+=i;  else { (*index)-=i; *length = -*length; }
  };

  return true;
};

//------------------------------------------------------------------------------------------------------------------
///cuts an element leftwards of index parameter. If uneffected, returns a copy, if excluded by cut, returns NULL
///otherwise returns a left cut segment of the element with a corrected index. This function is further complicated
///by the fact that the direction of the element can be positive or negative
pMolElement* pMolElement::cutLeftElement(int index, pMolElement* element)
{
  pMolElement* temp;
  int element_length = element->getLength();
  int element_index = element->getIndex();

  if (element_length>0)
  {
    if (element_index > index) return NULL;
    if (element_index + element_length < index) return element->duplicate();
    return element->cutLeftSub(index - element_index);
  }
  else
  {
    if (element_index + element_length > index) return NULL;
    if (element_index < index) return element->duplicate();
    pMolElement* temp = element->cutRightSub(element_index - index);
    temp->setIndex(element_index);
    return temp;
  }
};

//------------------------------------------------------------------------------------------------------------------
///returns a left cut copy of this DNA molecule with annotations either included, excluded or correctly cut.
pMolElement* pMolElement::cutLeftSub(int index)
{
    pMolElement* dna = duplicate(); 
    dna->name = name+"-cutLeft";
    dna->setLength(index);
    dna->setIndex(0);

    foreach (pMolElement* element, pMolElements)
      dna->append(cutLeftElement(index, element));

    return dna;
};

//------------------------------------------------------------------------------------------------------------------
pMolElement* pMolElement::cutRightElement(int index, pMolElement* element)
{
  pMolElement* temp;
  int element_length = element->getLength();
  int element_index = element->getIndex();

  if (element_length>0)
  {
     if (element_index + element_length < index) return NULL;
     if (element_index>index) 
     { temp = element->duplicate();
       temp->setIndex(element_index-index);
       return temp;
     };
     temp = element->cutRightSub(index-element_index);
     temp->setIndex(0);
     return temp;
  }
  else 
  {  if (element_index < index) return NULL;
     if (element_index + element_length > index)
     { temp = element->duplicate();
       temp->setIndex(element_index-index);
       return temp;
     }
     temp = element->cutLeftSub(element_index - index);
     temp->setIndex(element_index-index);
     return temp;
  };
};

//------------------------------------------------------------------------------------------------------------------
pMolElement* pMolElement::cutRightSub(int index)
{
    pMolElement* dna = duplicate(); 
    dna->name = name+"-cutRight";
    dna->setLength(length - index);
    dna->setIndex(index);

    foreach (pMolElement* element, pMolElements)
      dna->append(cutRightElement(index, element));

    return dna;
};

//------------------------------------------------------------------------------------------------------------------



void pMolElement::append(pMolElement* element)
{
  if (element == NULL) return;
  element->pMolParent = this;
  pMolElements.append(element);
  mutate();
};

//------------------------------------------------------------------------------------------------------------------
///sellotape, lots of good'ol sellotape
int pMolElement::getIndex(pMolStackObject* object)
{
  qDebug() << "getIndex::ok got to here6";
  if (object==NULL) qDebug() << "getIndex::woops this is NULL";
  if (object->type(pMolObject::NUMBER)==true)
  {   qDebug() << "seg::ok got to here7";
    pMolStackNumber* number = dynamic_cast< pMolStackNumber*>(object);
    return number->n; 
  };

  if (object->type(pMolObject::DNA))
  { pMolElement* element = dynamic_cast<pMolElement*>(object);
    return element->index;
  };

  return -1;
};


//------------------------------------------------------------------------------------------------------------------
///finds a DNA sequence and returns a layer list
pMolStackObject* pMolElement::findDNASequence(pMolElement* dna)
{
  QString p = dna->getSequence().toLower();
  QString s = getSequence().toLower();

  pMolLayer* layer = new pMolLayer();
  layer->pMolParent = this;
  QString name = dna->name;
  if (name.isEmpty()) name = p;
  QString temp;

  int i=0;

  while ((i=s.indexOf(p,i))>0)
  {
    layer->annotate(name+temp.setNum(i),"pMolElement::find",i, p.length());
    i++;
  };

  return (pMolStackObject*) layer;
};

//------------------------------------------------------------------------------------------------------------------
///we need to make this very sophisticated - able to take multiple inputs and outputs
///aswell as output lists of possibilities
pMolStackObject* pMolElement::find(pMolCmd* cmd, pMolKernelInterface* interface)
{

  QString error;

  //if we didn't get any parameters, then send an error and return
  pMolStackList* list = pMolStackList::interpretParameter(cmd, interface,error);
  if (list==NULL) return makeError(interface, "find", error);

  //if there's one number parameter, cut bit off front or back depending if pos or neg
  if (list->checkParameters(pMolObject::DNA, error))
  {
    pMolElement* dna = dynamic_cast <pMolElement*> (list->at(0));
    return findDNASequence(dna);
  };

  return makeError(interface, "add", error);
};

//------------------------------------------------------------------------------------------------------------------
///this adds an element as an annotation
pMolStackObject* pMolElement::add(pMolCmd* cmd, pMolKernelInterface* interface)
{
  QString error;

  //if we didn't get any parameters, then send an error and return
  pMolStackList* list = pMolStackList::interpretParameter(cmd, interface,error);
  if (list==NULL) return makeError(interface, "add", error);

  if (list->checkParameters(pMolObject::DNA, error))
  { 
    pMolStackObject* object = list->at(0);
    pMolElement* element = dynamic_cast <pMolElement*> (object);
    append(element);
    mutate();
    return NULL;
  };

  return makeError(interface, "add", error);
};

//------------------------------------------------------------------------------------------------------------------
///This saves the object
pMolStackObject* pMolElement::save(pMolCmd* cmd, pMolKernelInterface* interface)
{
  pMolStackObject::save(); 
  interface->msg(name+"::save - complete");
  return NULL;
};

//------------------------------------------------------------------------------------------------------------------
///This renames the object 
pMolStackObject* pMolElement::c_rename(pMolCmd* cmd, pMolKernelInterface* interface)
{
  QString error;

  //if we didn't get any parameters, then send an error and return
  pMolStackList* list = pMolStackList::interpretParameter(cmd, interface,error);
  if (list==NULL) return makeError(interface, "rename", error);

  if (list->checkParameters(pMolObject::STRING, error))
  { 
    pMolStackString* string = dynamic_cast <pMolStackString*> (list->at(0));
    name = string->string;
    mutate();
    return (pMolStackObject*) this;
  };

  return makeError(interface, "add", error);
};

//------------------------------------------------------------------------------------------------------------------
///This renames the object 
pMolStackObject* pMolElement::c_comment(pMolCmd* cmd, pMolKernelInterface* interface)
{
  QString error;

  //if we didn't get any parameters, then send an error and return
  pMolStackList* list = pMolStackList::interpretParameter(cmd, interface,error);
  if (list==NULL) return makeError(interface, "rename", error);

  if (list->checkParameters(pMolObject::STRING, error))
  { 
    pMolStackString* string = dynamic_cast <pMolStackString*> (list->at(0));
    addComment(string->string);
    mutate();
    return (pMolStackObject*) this;
  };

  return makeError(interface, "add", error);
};

//------------------------------------------------------------------------------------------------------------------
///This dumps sequence list to the terminal
pMolStackObject* pMolElement::dumpSequences(pMolCmd* cmd, pMolKernelInterface* interface)
{
  int i=0;
  pMolElement* element;

  while ((element=getElement(&i))!=NULL)
    interface->msg(element->name+" "+element->getSequence());

  return NULL;
}

//------------------------------------------------------------------------------------------------------------------
///returns length of the object
pMolStackObject* pMolElement::c_length(pMolCmd* cmd, pMolKernelInterface* interface)
{
  QString temp;
  pMolStackNumber* number = new pMolStackNumber(getLength());
  interface->msg(temp.setNum(getLength()));
  return (pMolStackObject*) number;
}

//-------------
///This dumps sequence list to the terminal
pMolStackObject* pMolElement::dumpPrimers(pMolCmd* cmd, pMolKernelInterface* interface)
{
  int i=0;
  pMolElement* element;

  while ((element=getElement(&i))!=NULL)
    if (element->type(pMolObject::PRIMER)) interface->msg(element->getName()+" "+element->getSequence());

  return NULL;
}

//-------------
///This dumps sequence list to the terminal
pMolStackObject* pMolElement::c_complement(pMolCmd* cmd, pMolKernelInterface* interface)
{
  interface->msg(getComplement());
  return NULL;
};
//------------------------------------------------------------------------------------------------------------------
///This dumps sequence list to the terminal
pMolStackObject* pMolElement::uploadPrimers(pMolCmd* cmd, pMolKernelInterface* interface)
{
  int i=0;
  pMolElement* element;
  bool sequential = false;
  QString error;
  qDebug() << "about to get List";

  //if we didn't get any parameters, then send an error and return
  if (cmd!=NULL)
  {
    if (cmd->type() != pMolCmd::LIST)
    { interface->msg(name+"::translate - expecting parameter list");			return NULL;}

    //if we didn't get any parameters, then send an error and return
    pMolStackList* list = pMolStackList::interpretParameter(cmd, interface,error);
    qDebug() << "got List";
    if (list==NULL) return makeError(interface, "rename", error);
    qDebug() << "past error";
    if (list->checkParameters(pMolObject::STRING, error))
    {
      pMolStackString* string = dynamic_cast <pMolStackString*> (list->at(0));
      if (string->string=="sequential") sequential = true;
    };
  };

  QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", "db");


  db.setHostName("db142e.pair.com");
  db.setDatabaseName("mpule2_PMOLV04");
  db.setUserName("mpule2_20_w");
  db.setPassword("a4SzFTDJ");

  bool ok = db.open();
  if (!ok) return makeError(interface, "dumpPrimers", "sql: failed to open database");

  while ((element=getElement(&i))!=NULL)
    if (element->type(pMolObject::PRIMER))
    { 
      QString error;
      pMolPrimer* primer = dynamic_cast <pMolPrimer*> (element);
      ok = primer->upload(db, sequential, error);
      if (!ok) return makeError(interface, "dumpPrimers", error);

      interface->msg(element->getName()+", "+element->getSequence());
    };

  db.close();
  QSqlDatabase::removeDatabase("db");
  
}

//------------------------------------------------------------------------------------------------------------------
///This executes commands directed at this object
pMolStackObject* pMolElement::command(pMolCmdName* cmdName, pMolKernelInterface* interface)
{
  if (cmdName->name == "add")  return add(cmdName->child, interface);
  if (cmdName->name == "find") return find(cmdName->child, interface);
  if (cmdName->name == "save") return save(cmdName->child, interface);
  if (cmdName->name == "length") return c_length(cmdName->child, interface);
  if (cmdName->name == "rename") return c_rename(cmdName->child, interface);
  if (cmdName->name == "comment") return c_comment(cmdName->child, interface);
  if (cmdName->name == "dumpSequences") return dumpSequences(cmdName->child, interface);
  if (cmdName->name == "uploadPrimers") return uploadPrimers(cmdName->child, interface);
  if (cmdName->name == "dumpPrimers") return dumpPrimers(cmdName->child, interface);
  if (cmdName->name == "complement") return c_complement(cmdName->child, interface);
  return makeError(interface, "exec", "command '"+cmdName->name+"' not understood");
};

//------------------------------------------------------------------------------------------------------------------
///This handles command directed at this oject
pMolStackObject* pMolElement::exec(pMolCmd* cmd, pMolKernelInterface* interface)
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

