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

#include "pMolSynthetics/pMolSynthArray.h"

//------------------------------------------------------------------------
///constructors and destructors

pMolSynthArray::pMolSynthArray
( int p_length, double* p_codonPenaltyTable, pMolSynthFlatArray* p_forbidden)
{
  length = p_length;
  mask = new bool[length];
  codons = new pMolCodon*[length];
  coding = new short int[length*3];
  codonPenaltyTable = p_codonPenaltyTable;
  suffixTreeAllocate();
  forbidden = p_forbidden;
}

pMolSynthArray::pMolSynthArray(pMolCodonCodex* codonCodex, pMolElement* orf, double* p_codonPenaltyTable)
{
  length = orf->getLength() / 3;
  mask = new bool[length];
  codons = new pMolCodon*[length];
  coding = new short int[length*3];

  pMolCodon* baseCodon = codonCodex->getBaseCodon();
  QString sequence = orf->getSequence().toLower();
  qDebug() << "BBBBBBBB" << sequence << length;

  for (int i = 0; i < length; i++)
    codons[i]=baseCodon->getCodon(sequence.mid(i*3,3));

  setMask(0, length, true);
  setFrame(0, length);
  frameIndex = 0;  frameEnd = length;
  codonPenaltyTable = p_codonPenaltyTable;

  suffixTreeAllocate();
};

pMolSynthArray::pMolSynthArray
( pMolCodonCodex* codonCodex, 
  pMolPeptide* peptide, 
  double* p_codonPenaltyTable, 
  pMolSynthFlatArray* p_forbidden )
{
  length = peptide->getLength();
  mask = new bool[length];
  codons = new pMolCodon*[length];
  coding = new short int[length*3];

  pMolCodon* baseCodon = codonCodex->getBaseCodon();
  QString sequence = peptide->getSequence().toUpper();

  for (int i = 0; i < length; i++)
    codons[i]=baseCodon->searchTranslation(sequence.at(i));

  setMask(0, length, true);
  frameIndex = 0;  frameEnd = 0;
  codonPenaltyTable = p_codonPenaltyTable;

  suffixTreeAllocate();
  forbidden = p_forbidden;
};

pMolSynthArray::~pMolSynthArray()
{
  delete []coding;
  delete []codons;
  delete []mask;
  suffixTreeDelete();
};

//------------------------------------------------------------------------
///
double pMolSynthArray::getCodonBadness(int i)
{
  if (i>63) i = 63;
  return codonPenaltyTable[i];
}

//------------------------------------------------------------------------
///
int pMolSynthArray::getLength()
{
  return length;
};

//------------------------------------------------------------------------
///return the frame's index
int pMolSynthArray::getFrameIndex()
{
  return frameIndex;
};

//------------------------------------------------------------------------
///return the frame's length
int pMolSynthArray::getFrameEnd()
{
  return frameEnd;
};

//------------------------------------------------------------------------
///return the frame's length
int pMolSynthArray::getFrameLength()
{
  return frameEnd - frameIndex;
};

//------------------------------------------------------------------------
///return the frame's address
short int* pMolSynthArray::getFrameStart()
{
  return coding + frameIndex*3;
};


//------------------------------------------------------------------------
///get the badness of the array
double pMolSynthArray::getBadness()
{
  return badness;
}

//------------------------------------------------------------------------
///return sequence - hopefully obolete soon
QString pMolSynthArray::getSequence()
{
  QString sequence;

  for (int i = 0; i < length*3; i++)
    if (coding[i] == 1) sequence = sequence + "a"; else
    if (coding[i] == 2) sequence = sequence + "t"; else
    if (coding[i] == 4) sequence = sequence + "c"; else
    if (coding[i] == 8) sequence = sequence + "g"; else
    sequence = sequence + "n";

  return sequence;
}

//------------------------------------------------------------------------
///set mask at one point 
void pMolSynthArray::setMask(int i, bool state)
{
  if (i < length) mask[i] = state;
};

//------------------------------------------------------------------------
///sets the codon
void pMolSynthArray::setCodon(int i, pMolCodon* codon)
{
  if (i < length) codons[i] = codon;
};

//------------------------------------------------------------------------
///set mask in a range
void pMolSynthArray::setMask(int i, int j, bool state)
{
  if (i > length) i = length-1;		// out of range modulo back within range
  if (j > length) j = length-1;		// out of range modulo back to within range
  if (i < 0) i = 0;			// out of range checking
  if (j < 0) j = 0;			// out of range checking

  for (int n = i; n < j; n++) mask[n] = state;
}

//------------------------------------------------------------------------
///sets the current frame, calulates badness for the frame and 
///updates the raw sequence
bool pMolSynthArray::setFrame(int p_frameIndex, int p_frameEnd)
{
  if (p_frameEnd > length) frameEnd = length; else frameEnd = p_frameEnd;
  if (p_frameIndex > length) frameIndex = length; else frameIndex = p_frameIndex;	
  if (frameEnd < 0) frameEnd = 0;
  if (frameIndex < 0) frameIndex = 0;

  badness = 0.0;

  for (int i = frameIndex; i < frameEnd; i++)
    if (codons[i]!=NULL) 
    {
      badness+=codons[i]->badness;
      codons[i]->setBytes(coding, i*3);
    };

  return true;
};

//--------------------------------------------------------------------------
///tries to flip an individual one. If it can't returns false
///if codon changes, it automatically updates badness and the 
///raw sequence
bool pMolSynthArray::flip(int i)
{
  if (i>length) return false;		//out of range so can't change
  if (!mask[i]) return false;		//masked - so can't change
  if (codons[i]==NULL) return false;		//not defined so can't change

  bool ok;					//has it flipped around?
  badness -= codons[i]->badness;		//reset badness 
  codons[i]=codons[i]->flip(&ok);		//change the state
  codons[i]->setBytes(coding, i * 3);
  badness += codons[i]->badness;

  //qDebug() << getSequence() << ok;



  return ok;					//otherwise send ok
};

void pMolSynthArray::reset()
{
  for (int i=0; i<length; i++)
    if (codons[i]!=NULL) codons[i]=codons[i]->reset();
};

//------------------------------------------------------------------------
///sets the current frame, calulates badness for the frame and 
///updates the raw sequence
void pMolSynthArray::reset(int p_resetIndex, int p_resetEnd)
{
  int resetIndex, resetEnd;

  if (p_resetEnd > length) resetEnd = length; else resetEnd = p_resetEnd;
  if (p_resetIndex > length) resetIndex = length; else resetIndex = p_resetIndex;	

  for (int i = resetIndex; i < resetEnd; i++)
    if (codons[i]!=NULL) 
       codons[i]=codons[i]->reset();

};

//--------------------------------------------------------------------------
///tries to flip the codons, if it can't flip the right-most one 
///because it's masked or if the codon flips over it tries to flip the next one 
///hence moves along like solving a combination lock
///if no codons are flipped returns false
bool pMolSynthArray::flip()
{
  for (int i=frameIndex; i<frameEnd; i++)
  { 
    //qDebug() << "about to flip" << i;
   if (flip(i)) return true;
   // qDebug() << "other side of while" << frameEnd;
  };

  return false;
};

//--------------------------------------------------------------------------
///copies a synthArray 
pMolSynthArray* pMolSynthArray::copy(pMolSynthArray* dest)
{							//if completely new, allocate space
  if (dest==NULL) dest = new pMolSynthArray(length, codonPenaltyTable, forbidden);	
  if (dest->getLength()!=length)			//if an old one for re-cycling, is it similar?
  { delete dest;					//if not delete it
    dest = new pMolSynthArray(length, codonPenaltyTable, forbidden);			//and make a new one
  };

  for (int i = 0; i < length; i++)			//copy the codons and the mask
  {
    dest->setMask(i, mask[i]);
    dest->setCodon(i, codons[i]);
  };

  dest->setFrame(frameIndex, frameEnd);		//copy the frame

  return dest;
};

//--------------------------------------------------------------------------
///
int pMolSynthArray::compare(pMolSynthArray* compareArray)
{
  short int* data0 = getFrameStart();
  int len0 = getFrameLength() * 3;

  short int* data1 = compareArray->getFrameStart();
  int len1 = compareArray->getFrameLength() * 3;

  if (len0!=len1) return -1;

  int count = 0;
  for (int i=0; i<len0; i++) if (data0[i]==data1[i]) count++;

  return (count*100)/len0;
};


//--------------------------------------------------------------------------
///generates the suffix tree. This code should be as efficient as possible
///if memory is exceeded, it tries to allocated more
void pMolSynthArray::suffixTreeBuild()
{
  //establish the frame we will generate the suffice tree. Flank it on both sides
  //with enough bases such that even a change at either end don' generate a forbidden sequence
  int n0 = getFrameIndex()*3 - forbidden->getMaxLength();
  int n1 = n0 + getFrameLength()*3 + forbidden->getMaxLength();
  if (n0<0) n0 = 0;					//if out of bounds either way
  if (n1>getFrameLength()*3) n1 = getFrameLength()*3;	//correct

  n1 = n0+3;
  //Debug() << "starting to build tree" << n0 << n1;

  int offset = 0;					//for looking up nodes
  short int flatten[] = 
  {0,0,1,1, 2,2,2,2, 3,3,3,3, 3,3,3,3};			//array for quick flattening
  short int* rawData = coding;				//where the raw sequence data is kept
  if (suffixArray == NULL) throw
    pMolError("suffixTreeBuild - space for tree has not been allocated");

  suffixArrayNextFreeNode = 5;
  suffixArray[0] = 0;	//initialize A
  suffixArray[1] = 0;	//initialize C
  suffixArray[2] = 0;	//G
  suffixArray[3] = 0;	//T
  suffixArray[4] = 0;	//termination signal

  for (int i = n0; i<n1; i++)				//loop from start to end
  {	
 //  qDebug() << "****suffix******" << i;
   offset = 0;
   for (int j = i; j<n1; j++)				//loop through each suffix
   {
     unsigned int index = flatten[rawData[j]&0x000f];	//index to look up next base
  //   if (offset > suffixArrayMax)			//check for outer bounds
  //     throw pMolError("suffixTreeBuild - tree node pointed to outer space");

     //offset+=index;					//branch to the next offset
     int newOffset = suffixArray[offset+index];		//is there a branch in this direction?
   //  qDebug() << index << offset << newOffset;

     if (newOffset==0)					//no branch in this direction
     {
       //have we exceeded the storage allocated to us?
       if (suffixArrayNextFreeNode>suffixArrayMax)	//no more room left
       {						//so let's allocate more
          qDebug() << "allocated more space";

          int newSuffixArrayMax = suffixArrayMax + 4*5*(n1-n0);
          unsigned int* newSuffixArray;		//temporary pointer to new array
          newSuffixArray = new unsigned int[newSuffixArrayMax];

    //      if (newSuffixArray == NULL)			//allocation failed
    //        throw pMolError("suffixTreeBuild - could not allocate more space");

          for (int n=0; n<suffixArrayMax; n++)		//copy over the old array
            newSuffixArray[n] = suffixArray[n];		//to the new space

          delete []suffixArray;			//free up the old array
          suffixArray = newSuffixArray;			//copy over the pointer
          suffixArrayMax = newSuffixArrayMax;		//and the max 
       };

       suffixArray[offset+index] = suffixArrayNextFreeNode;	//next free node
       offset = suffixArrayNextFreeNode;		//go to the newly created node
       suffixArray[suffixArrayNextFreeNode++] = 0;	//initialize A
       suffixArray[suffixArrayNextFreeNode++] = 0;	//initialize C
       suffixArray[suffixArrayNextFreeNode++] = 0;	//G
       suffixArray[suffixArrayNextFreeNode++] = 0;	//T
       suffixArray[suffixArrayNextFreeNode++] = 0;	//termination signal
     }
     else offset = newOffset;				//otherwise traverse the tree
   };
   suffixArray[offset+4]++;				//add termination signal
  };
};


//-------------------------------------------------------------------------
///searches through the suffix tree to see if there is a similar short sequence
///sequences are sent as a flattened short int string terminated by 4
bool pMolSynthArray::suffixTreeFind()
{
  int index = 0;
  short unsigned int* seq;

  while ((seq = forbidden->getArray(index++))!=NULL)	//loop through the sequences
  {
    int offset = 0;					//points to node in tree 
    bool found = true;

    //qDebug() << "entering search";

    while (*seq<4)					//while there are more bases in the seq
    {
      offset=suffixArray[offset + *(seq++)];		//look up the next node
      if (offset==0)    { found = false; break; };

  //    qDebug() << *(seq-1) << offset;

      if (offset>suffixArrayMax) 			//out of bounds checking
        throw pMolError("suffixTreeFind - search out of bounds");
    };
    //qDebug() << "out of loop" << found;
    if (*seq>4)					//if something funny happened
      throw pMolError("suffixTreeFind - woah there that's a strange code");

    if (found) return true;				//wow this sequence is there!
  };
  return false;					//not found in any of the sequences
};


//--------------------------------------------------------------------------
///allocates memory for a suffix tree
void pMolSynthArray::suffixTreeAllocate()
{
  suffixArrayMax = 5*2444;
  suffixArray = new unsigned int[suffixArrayMax]; 
  if (suffixArray==NULL) 
    throw pMolError("suffixTreeAllocate - could not allocate space for array");
  suffixArrayNextFreeNode = 0;
  suffixArray[suffixArrayNextFreeNode++] = 0;	//A
  suffixArray[suffixArrayNextFreeNode++] = 0;	//C
  suffixArray[suffixArrayNextFreeNode++] = 0;	//G
  suffixArray[suffixArrayNextFreeNode++] = 0;	//T
  suffixArray[suffixArrayNextFreeNode++] = 0;	//termination signal
};


//--------------------------------------------------------------------------
///deletes the suffix Tree
void pMolSynthArray::suffixTreeDelete()
{
  suffixArrayMax = 0;
  if (suffixArray != NULL) delete []suffixArray;
  suffixArray = NULL;
};

