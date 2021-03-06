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
 ///returns tree structure of this class
#include "pMolRea.h"
#include "pMolRestrictionEndonuclease.h"


pMolRestrictionEndonuclease::pMolRestrictionEndonuclease(): pMolStackObject(NULL) 
{

};


pMolRestrictionEndonuclease::pMolRestrictionEndonuclease(pMolXMLbase *p_pMolXMLbaseParent) : pMolStackObject(p_pMolXMLbaseParent) { };


pMolRestrictionEndonuclease::~pMolRestrictionEndonuclease()
{
  foreach (pMolRestrictionEndonucleaseBuffer* buffer, buffers) delete buffer;
  foreach (pMolRestrictionEndonucleaseVendor* vendor, vendors) delete vendor;
  delete []seqbuff;
};

void pMolRestrictionEndonuclease::init()
{
  length = getSequence().length();
  seqbuff = new short int[length];
  convertToByteSoftExternal(getSequence(), seqbuff);
}

pMolXMLbase* pMolRestrictionEndonuclease::XMLopen(const QString &tag, QString &error)
{
  if (tag=="bufferActivity") 
  { pMolRestrictionEndonucleaseBuffer* buffer = new pMolRestrictionEndonucleaseBuffer((pMolXMLbase*) this); 
    buffers.append(buffer);
    return (pMolXMLbase*) buffer;
  } else
  if (tag=="vendor")
  { pMolRestrictionEndonucleaseVendor* vendor = new pMolRestrictionEndonucleaseVendor((pMolXMLbase*) this); 
    vendors.append(vendor);
    return (pMolXMLbase*) vendor;
  } else 
  return pMolXMLbase::XMLopen(tag, error);
};

pMolXMLbase* pMolRestrictionEndonuclease::XMLclose(const QString &tag, const QString &data, QString &error)
{
  qDebug() << name << data;
   if (tag=="sourceOrganism")		sourceOrganism = data;  			else
   if (tag=="reference") 		reference = data;				else
   if (tag=="recognitionSite")		recognitionSite = data;				else
   if (tag=="cleavesPositive")		XMLnumeric(&cleavesPositive, data, error); 	else
   if (tag=="cleavesNegative")		XMLnumeric(&cleavesNegative, data, error); 	else
   if (tag=="dam")			XMLboolean(&dam, data, error); 			else
   if (tag=="dcm")			XMLboolean(&dcm, data, error);			else
   if (tag=="CpG")			XMLboolean(&CpG, data, error);			else
   if (tag=="pMolRestrictionEndonuclease") init();
   return pMolXMLbase::XMLclose(tag, data,error);
}


void pMolRestrictionEndonuclease::treeOut(QTreeWidgetItem *tree)
{
  pMolXMLbase::treeOut(tree);

  treeItem(tree,"sourceOrganism",sourceOrganism);
  treeItem(tree,"reference",reference);
  treeItem(tree,"recognitionSite", recognitionSite);
  treeItem(tree,"cleavesPositive",cleavesPositive);
  treeItem(tree,"cleavesNegative",cleavesNegative);
  treeItem(tree,"dam",dam);
  treeItem(tree,"dcm",dcm);
  treeItem(tree,"CpG",CpG);

  foreach (pMolRestrictionEndonucleaseBuffer* buffer, buffers) buffer->treeOut(tree);
  foreach (pMolRestrictionEndonucleaseVendor* vendor, vendors) vendor->treeOut(tree);
};

short int* pMolRestrictionEndonuclease::getSeqbuff()
{ 
  return seqbuff;
};

int pMolRestrictionEndonuclease::getLength()
{
  return length;
};

bool pMolRestrictionEndonuclease::type(int i)
{ 
  if (i == pMolStackObject::RESTRICTIONENDONUCLEASE) return true;
  if (i == pMolStackObject::EXEC) return true;
  return pMolStackObject::type(i);
};

//----------------------------------------------------------------------------------------------
//allows degenerate bases
bool pMolRestrictionEndonuclease::convertToByteSoft(const QString &s, short int* buffer)
{
  /*C G T A  dec ascii index
  A 0 0 0 1   1    65    0  x
  T 0 0 1 0   2    84   19  x
  G 0 1 0 0   4    71    6  x
  C 1 0 0 0   8    67    2  x
  R 0 1 0 1   5    82   17  x
  Y 1 0 1 0  10    89   24
  W 0 0 1 1   3    87   22  x
  S 1 1 0 0  12    83   18  x
  M 1 0 0 1   9    77   12  x
  K 0 1 1 0   6    75   10  x
  H 1 0 1 1  11    72    7  x
  B 1 1 1 0  14    66    1  x
  V 1 1 0 1  13    86   21  x
  D 0 1 1 1   7    68    3  x
  N 1 1 1 1  15    78   13  x  */
  //                        0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24
     short int flatten[] = {1,14, 8, 7,-1,-1, 4,11,-1,-1, 6,-1, 9,15,-1,-1,-1, 5,12, 2,-1,13, 3,-1,10};
     //short int flatten[] = {0,-1, 2,-1,-1,-1, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 1,-1};
  for (int i=0; i<s.length(); i++)
  { int t;
    int j = s.at(i).unicode() - 65;
    if ((j<0) || (j>24)) return false;
    if ((t=flatten[j])<0) return false;
    *(buffer+i) = t;
  };
  return true;
};

//----------------------------------------------------------------------------------------------
///disallows degenerate bases
bool pMolRestrictionEndonuclease::convertToByteHard(const QString &s, short int* buffer)
{
  /*C G T A  dec ascii index soft hard
  A 0 0 0 1   1    65    0    x    x
  T 0 0 1 0   2    84   19    x    x
  G 0 1 0 0   4    71    6    x    x
  C 1 0 0 0   8    67    2    x    x
  R 0 1 0 1   5    82   17    x
  Y 1 0 1 0  10    89   24    x
  W 0 0 1 1   3    87   22    x
  S 1 1 0 0  12    83   18    x
  M 1 0 0 1   9    77   12    x
  K 0 1 1 0   6    75   10    x
  H 1 0 1 1  11    72    7    x
  B 1 1 1 0  14    66    1    x
  V 1 1 0 1  13    86   21    x
  D 0 1 1 1   7    68    3    x
  N 1 1 1 1  15    78   13    x  */
  //                      0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24
  short int flatten[] = {1, 0, 8, 0,-1,-1, 4, 0,-1,-1, 0,-1, 0, 0,-1,-1,-1, 0, 0, 2,-1, 0, 0,-1, 0};

  for (int i=0; i<s.length(); i++)
  { int t;
    int j = s.at(i).unicode() - 65;
    if ((j<0) || (j>24)) return false;
    if ((t=flatten[j])<0) return false;
    *(buffer+i) = t;
  };
  return true;
};

//----------------------------------------------------------------------------------------------
///I know I am not a good programmer. I have used different conventions to represent dna
///One day, I will make a universal one and stop using QString...
///unfortunately, when writing pMolSyntheticEngine, I used different numbers for C and G 
///hence this bit of tape!
bool pMolRestrictionEndonuclease::convertToByteSoftExternal(const QString &s, short int* buffer)
{

  /*G C T A  dec ascii index
  A 0 0 0 1   1    65    0  x  x
  T 0 0 1 0   2    84   19  x  x
  C 0 1 0 0   4    67    2  x  x
  G 1 0 0 0   8    71    6  x  x
  R 1 0 0 1   5    77   12  x  x
  Y 0 1 1 0  10    75   10  x  x
  W 0 0 1 1   3    87   22  x  x
  S 1 1 0 0  12    83   18  x  x
  M 1 0 0 1   9    82   17  x  x
  K 1 0 1 0   6    89   24  x  x
  H 1 0 1 1  11    68    3  x  x
  B 1 1 1 0  14    66    1  x  x
  V 1 1 0 1  13    86   21  x  x
  D 0 1 1 1   7    72    7  x  x
  N 1 1 1 1  15    78   13  x  x*/
  //                     0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24
  short int flatten[] = {1,14, 4,11,-1,-1, 8, 3,-1,-1,10,-1, 5,15,-1,-1,-1, 9,12, 2,-1,13, 3,-1, 6};
  qDebug() << "convertToByteSoftExternal" << name << getSequence();
  for (int i=0; i<s.length(); i++)
  { int t;
    int j = s.at(i).unicode() - 65;
    if ((j<0) || (j>24)) return false;
    if ((t=flatten[j])<0) return false;
    *(buffer+i) = t;
    qDebug() << name << t;
  };
  return true;
};

//----------------------------------------------------------------------------------------------
///ORs the restriction recognition sequence with DNA sequence and 
int pMolRestrictionEndonuclease::search(short int* s, short int*r, int I, int slength, int rlength)
{
  int i,j;
  for (i=I; i<slength-rlength; i++)
  {
    for (j=0; j<rlength; j++)
      if (((*(s+i+j)) & (*(r+j)))==0) break;
    if (j==rlength) return i;
  };
  return -1;
};

//----------------------------------------------------------------------------------------------
///returns the recognition sequence
QString pMolRestrictionEndonuclease::getSequence()
{
  return recognitionSite;
};

//----------------------------------------------------------------------------------------------
///finds restriction site in molecule and sends back a list of sites
pMolLayer* pMolRestrictionEndonuclease::find(pMolElement* dna)
{
  QString s = dna->getSequence();				//get the target sequence
  int rlength = getSequence().length();					//recognition sequence length
  if (dna->isCircular()) s += s.right(rlength-1);			//if circular, remember can cut over junction
  int slength = s.length();

  short int* sbuffer = new short int[slength];
  if (!convertToByteHard(s, sbuffer)) 
  { delete []sbuffer; return NULL;}				//ok we need better error handling here

  short int* rbuffer = new short int[rlength];
  if (!convertToByteSoft(getSequence(), rbuffer))
  { delete []sbuffer; delete []rbuffer; return NULL;}	//better error handling here

  int i=0;
  pMolLayer* layer = new pMolLayer();
  layer->pMolParent = dna;
  while ((i=search(sbuffer,rbuffer,i,slength,rlength))>=0)
  {
    qDebug() << "pMolRestrictionEndonuclease: " << i << slength << rlength;
    pMolRea* site = new pMolRea(this);
    site->pMolParent = (pMolElement*) layer;
    site->setIndex(i);
    site->setLength(rlength);
    site->name = name;
    site->addComment(name + " restriction site");
    layer->append((pMolElement*) site);
    i++;							//so we won't keep finding the same one over and over
  };

  delete []sbuffer;
  delete []rbuffer;
  return layer;
};

pMolStackList* pMolRestrictionEndonuclease::cut(pMolDNA* dna)
{
  QString s = dna->getSequence();				//get the target sequence
  int rlength = getSequence().length();					//recognition sequence length
  if (dna->circular) s += s.right(rlength-1);			//if circular, remember can cut over junction
  int slength = s.length();

  short int* sbuffer = new short int[slength];
  if (!convertToByteHard(s, sbuffer)) 
  { delete []sbuffer; return NULL;}				//ok we need better error handling here

  short int* rbuffer = new short int[rlength];
  if (!convertToByteSoft(getSequence(), rbuffer))
  { delete []sbuffer; delete []rbuffer; return NULL;}	//better error handling here

  int i=0;
  pMolStackList* list = new pMolStackList();

  pMolDNA* dna0;
  pMolDNA* dna1;
  dna0 = dynamic_cast <pMolDNA*> (dna->duplicate());						//make a memory copy

  int min, max;							//we need to leave the cleaveage site
  if (cleavesPositive > cleavesNegative)			//overhand or underhang with the 
  { max = cleavesPositive; min = cleavesNegative; }		//sequence, hence we need to know
  else								//how much to leave extra on either end
  { max = cleavesNegative; min = cleavesPositive; }


  while ((i=search(sbuffer,rbuffer,i,slength,rlength))>=0)
  {
    if (dna0->circular)						//if DNA is circular, we just open it up
    {  dna1 = dna0->cutOpen(i+min);
       delete dna0;						//delete the circular memory copy
       dna0 = dna1;
       dna0->sequence = dna0->sequence + dna0->sequence.mid(0,max-min);
       dna0->setLength(dna->getLength());
       dna0->overhangAtStart = cleavesNegative - cleavesPositive;    	//how much sense strand overhangs
       dna0->overhangAtEnd   = cleavesPositive - cleavesNegative;	//is what this value means
    }
    else
    {
      dna1 = dna0->cutLeft(i+max);
      dna1->overhangAtEnd    = cleavesPositive - cleavesNegative;
      list->append((pMolStackObject*) dna1);		 	// left half

      dna1 = dna0->cutRight(i+min);
      dna1->overhangAtStart  = cleavesNegative = cleavesPositive;
      delete dna0;
      dna0 = dna1; 
    };

    i++;							//so we won't keep finding the same one o
  };

  list->append((pMolStackObject*) dna0);
  delete []sbuffer;
  delete []rbuffer;
  return list;
};



//----------------------------------------------------------------------------------------------
///finds and cuts at the restriction site in a single molecule
pMolStackObject* pMolRestrictionEndonuclease::cut(pMolCmd* cmd, pMolKernelInterface* interface)
{
  //if we didn't get any parameters, then send an error and return
  if (cmd==NULL) return NULL;
  if (cmd->type() != pMolCmd::LIST) 
  { interface->msg(name+"::cut - expecting parameter list");				return NULL;};

  //interpret out paramaters, bailing if interpretation fails
  pMolObject* object = interface->interpret(cmd);
  if (object==NULL)
  { interface->msg(name+"::cut - something went wrong with interpreting a list"); 	return NULL; };

  //I hate doing this but cast up to an object list
  pMolStackList* objectList = dynamic_cast <pMolStackList*>(object);

  //check we have the correct number of parameters
  if (objectList->length()!=1) 
  { interface->msg(name+"::cut - expected one parameter"); 				return NULL; };

  //extract first parameter
  if (!objectList->at(0)->type(pMolStackObject::RAW))
  { interface->msg(name+"::cut - expected a dna object as parameter"); 		return NULL; };
  pMolDNA* dna = dynamic_cast <pMolDNA*> (objectList->at(0));

  delete objectList;

  pMolStackList* list = cut(dna);
  if (list==NULL)
  { interface->msg(name+"::cut - search failed - likely bad sequence"); 		return NULL; };

  interface->msg(list->length());

  return (pMolStackObject*) list;
};

//----------------------------------------------------------------------------------------------
///finds the restriction site in a single molecule
pMolStackObject* pMolRestrictionEndonuclease::find(pMolCmd* cmd, pMolKernelInterface* interface)
{
  //if we didn't get any parameters, then send an error and return
  if (cmd==NULL) return NULL;
  if (cmd->type() != pMolCmd::LIST) 
  { interface->msg(name+"::find - expecting parameter list");				return NULL;};

  //interpret out paramaters, bailing if interpretation fails
  pMolObject* object = interface->interpret(cmd);
  if (object==NULL)
  { interface->msg(name+"::find - something went wrong with interpreting a list"); 	return NULL; };

  //I hate doing this but cast up to an object list
  pMolStackList* objectList = dynamic_cast <pMolStackList*>(object);

  //check we have the correct number of parameters
  if (objectList->length()!=1) 
  { interface->msg(name+"::find - expected one parameter"); 				return NULL; };

  //extract first parameter
  if (!objectList->at(0)->type(pMolStackObject::DNA))
  { interface->msg(name+"::find - expected a dna object as parameter"); 		return NULL; };
  pMolElement* dna = dynamic_cast <pMolElement*> (objectList->at(0));

  delete objectList;

  pMolLayer* layer = find(dna);
  if (layer==NULL)
  { interface->msg(name+"::find - search failed - likely bad sequence"); 		return NULL; };

  interface->msg(layer->childCount());

  return (pMolStackObject*) layer;
};



//---------------------------------------------------------------------------------------------------------------------
///executes commands referred to this object
pMolStackObject* pMolRestrictionEndonuclease::exec(pMolCmd* cmd, pMolKernelInterface* interface)
{
  //no paramaters, return this as object
  if (cmd==NULL) return (pMolStackObject*) this;				//no paramaters, return me

  //if no command but not empty, something is wrong (needs to be empty or have subcommand) so return with error
  if (cmd->type() != pMolCmd::NAME)
  { interface->msg(name+"::exec - empty field or subcommand expected");
    return NULL;
  };
  //first let's look in our own little internal namespace
  pMolCmdName* cmdName = dynamic_cast<pMolCmdName*>(cmd);					//safe to assume this is a pMolCmdName

  if (cmdName->name=="find") return find(cmdName->child, interface);	//else
  if (cmdName->name=="cut")  return cut(cmdName->child,interface);
 // if (cmdName->name=="badness") return badness(cmdName->child, interface);

  interface->msg(name+"::command not understood");
  return NULL;
};



//----------------------------------------------------------------------------------------------
///this is a small class which contains restrictionEndonuclease buffer information
pMolRestrictionEndonucleaseBuffer::pMolRestrictionEndonucleaseBuffer(): pMolXMLbase(NULL) { };

pMolRestrictionEndonucleaseBuffer::pMolRestrictionEndonucleaseBuffer(pMolXMLbase *p_pMolXMLbaseParent) : pMolXMLbase(p_pMolXMLbaseParent) { };

pMolXMLbase* pMolRestrictionEndonucleaseBuffer::XMLclose(const QString &tag, const QString &data, QString &error)
{
  if (tag=="bufferActivity")		return pMolXMLbaseParent; 			else
  if (tag=="activity")			XMLnumeric(&activity, data, error); 		
  return pMolXMLbase::XMLclose(tag, data,error);
}

void pMolRestrictionEndonucleaseBuffer::treeOut(QTreeWidgetItem *tree)
{
  QTreeWidgetItem* branch = treeItem(tree,"bufferActivity");
  treeItem(branch,"name",name);
  treeItem(branch,"activity",activity);
}
//-----------------------------------------------------------------------------------------------

pMolRestrictionEndonucleaseVendor::pMolRestrictionEndonucleaseVendor(): pMolXMLbase(NULL) { }

pMolRestrictionEndonucleaseVendor::pMolRestrictionEndonucleaseVendor(pMolXMLbase *p_pMolXMLbaseParent) :  pMolXMLbase(p_pMolXMLbaseParent) { }

pMolXMLbase* pMolRestrictionEndonucleaseVendor::XMLclose(const QString &tag, const QString &data, QString &error)
{
  if (tag=="vendor")			return pMolXMLbaseParent; 			else
  if (tag=="catalogueReference")	catalogueReference = data;	 		
  return pMolXMLbase::XMLclose(tag, data,error);
}

void pMolRestrictionEndonucleaseVendor::treeOut(QTreeWidgetItem *tree)
{
  QTreeWidgetItem* branch = treeItem(tree,"vendor");
  treeItem(branch,"name",name);
  treeItem(branch,"catalogueReference",catalogueReference);
}
