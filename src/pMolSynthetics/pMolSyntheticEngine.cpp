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

#include <QApplication>

#include "pMolSynthetics/pMolSyntheticEngine.h"
#include "pMolRestrictionEndonuclease.h"

//--------------------------------------------------------------------------------------------------------------------
///Constructors and destructors
pMolSyntheticEngine::pMolSyntheticEngine() : pMolStackObject(NULL)
{
  useCodonBadness = 		false;
  useHairpinBadness = 		false;
  useRepeatBaseBadness = 	false;
  useRepeatBaseTypeBadness = 	false;
  useRepeatBadness = 		false;
  useBaseContentBadness = 	false;
  useCrypticSpliceBadness =	false;

  complement[base_a] = base_t;
  complement[base_c] = base_g;
  complement[base_g] = base_c;
  complement[base_t] = base_a;

  marking = false;
}

pMolSyntheticEngine::pMolSyntheticEngine(pMolXMLbase *p_pMolXMLbaseParent) : pMolStackObject(p_pMolXMLbaseParent)
{
  useCodonBadness = 		false;
  useHairpinBadness = 		false;
  useRepeatBaseBadness = 	false;
  useRepeatBaseTypeBadness = 	false;
  useRepeatBadness = 		false;
  useBaseContentBadness = 	false;
  useCrypticSpliceBadness =	false;

  complement[base_a] = base_t;
  complement[base_c] = base_g;
  complement[base_g] = base_c;
  complement[base_t] = base_a;

  marking = false;
}

pMolSyntheticEngine::~pMolSyntheticEngine()
{
};

//--------------------------------------------------------------------------------------------------------------------
///Load up XML data
pMolXMLbase* pMolSyntheticEngine::XMLclose(const QString &tag, const QString &data, QString &error)
{
  if (tag=="pcrEngine")			pcrEngineName=data;
  else if (tag=="codonCodex")		codonCodexName=data;

  //wobbling parameters
  else if (tag=="wobbleDifferenceThreshold")	XMLnumeric(&wobbleDifferenceThreshold, data, error);

  //parameters for codon usage penalty scoring
  else if (tag=="useCodonBadness") 		XMLboolean(&useCodonBadness, data, error);
  else if (tag=="codonPenaltyLin")		XMLdouble(&codonPenaltyLin, data, error);
  else if (tag=="codonPenaltyExp")		XMLdouble(&codonPenaltyExp, data, error);

  //parameters for hairpin penalty scoring
  else if (tag=="useHairpinBadness") 		XMLboolean(&useHairpinBadness, data, error);
  else if (tag=="hairpinPenaltyLin")		XMLdouble(&hairpinPenaltyLin, data, error);
  else if (tag=="hairpinPenaltyExp")		XMLdouble(&hairpinPenaltyExp, data, error);
  else if (tag=="hairpinStemMin")		XMLnumeric(&hairpinStemMin, data, error);
  else if (tag=="hairpinStemMax")		XMLnumeric(&hairpinStemMax, data, error);
  else if (tag=="hairpinLengthThreshold")	XMLnumeric(&hairpinLengthThreshold, data, error);
  else if (tag=="hairpinAbortThreshold")	XMLnumeric(&hairpinAbortThreshold, data, error);

  //parameters for repeated base penalty scoring
  else if (tag=="useRepeatBaseBadness") 	XMLboolean(&useRepeatBaseBadness, data, error);
  else if (tag=="repeatBaseThreshold")		XMLnumeric(&repeatBaseThreshold, data, error);
  else if (tag=="repeatBaseAbortThreshold")	XMLnumeric(&repeatBaseAbortThreshold, data, error);
  else if (tag=="repeatBasePenaltyLin")	XMLdouble(&repeatBasePenaltyLin, data, error);
  else if (tag=="repeatBasePenaltyExp")	XMLdouble(&repeatBasePenaltyExp, data, error);

  else if (tag=="useRepeatBaseTypeBadness")	XMLboolean(&useRepeatBaseTypeBadness, data, error);
  else if (tag=="repeatBaseTypeThreshold")	XMLnumeric(&repeatBaseTypeThreshold, data, error);
  else if (tag=="repeatBaseTypeAbortThreshold")	XMLnumeric(&repeatBaseTypeAbortThreshold, data, error);
  else if (tag=="repeatBaseTypePenaltyLin")	XMLdouble(&repeatBaseTypePenaltyLin, data, error);
  else if (tag=="repeatBaseTypePenaltyExp")	XMLdouble(&repeatBaseTypePenaltyExp, data, error);

  //parameters for base content penalty scoring
  else if (tag=="useBaseContentBadness") 	XMLboolean(&useBaseContentBadness, data, error);
  else if (tag=="idealGCpercent")		XMLnumeric(&idealGCpercent, data, error);
  else if (tag=="GCpercentDeltaThreshold")	XMLnumeric(&GCpercentDeltaThreshold, data, error);
  else if (tag=="GCpercentAbortThreshold")	XMLnumeric(&GCpercentAbortThreshold, data, error);
  else if (tag=="baseContentPenaltyLin")	XMLdouble(&baseContentPenaltyLin, data, error);
  else if (tag=="baseContentPenaltyExp")	XMLdouble(&baseContentPenaltyExp, data, error);

  //parameters for cryptic splicing penalty score
  else if (tag=="useCrypticSpliceBadness") 	XMLboolean(&useCrypticSpliceBadness, data, error);
  else if (tag=="splicingPredictor") 		splicingPredictorName = data;
  else if (tag=="crypticSpliceThreshold")    	XMLdouble(&crypticSpliceThreshold, data, error);
  else if (tag=="crypticSpliceAbortThreshold")	XMLdouble(&crypticSpliceAbortThreshold, data, error);
  else if (tag=="crypticSplicePenaltyLin")	XMLdouble(&crypticSplicePenaltyLin, data, error);
  else if (tag=="crypticSplicePenaltyExp")	XMLdouble(&crypticSplicePenaltyExp, data, error);

  //paramters for repeat penalty scoring
  else if (tag=="useRepeatBadness")		XMLboolean(&useRepeatBadness, data, error);
  else if (tag=="repeatThreshold")		XMLnumeric(&repeatThreshold, data, error);
  else if (tag=="repeatAbortThreshold")	XMLnumeric(&repeatAbortThreshold, data, error);
  else if (tag=="repeatPenaltyLin")		XMLdouble(&repeatPenaltyLin, data, error);
  else if (tag=="repeatPenaltyExp")		XMLdouble(&repeatPenaltyExp, data, error);

  else return pMolXMLbase::XMLclose(tag, data,error);

  return (pMolXMLbase*) this; 
};


//--------------------------------------------------------------------------------------------------
///output XML data as a tree structure
void pMolSyntheticEngine::treeOut(QTreeWidgetItem *tree)
{
  pMolXMLbase::treeOut(tree);
  treeItem(tree,"pcrEngine",pcrEngineName);
  treeItem(tree,"codonCodex",codonCodexName);

  qDebug() << useBaseContentBadness << useCrypticSpliceBadness << useRepeatBadness;

  treeItem(tree, "wobbleDifferenceThreshold",wobbleDifferenceThreshold);

  if (useCodonBadness)
  { QTreeWidgetItem* branch = treeItem(tree, "useCodonBadness", "true");
    treeItem(branch,"codonPenaltyLin",codonPenaltyLin);
    treeItem(branch,"codonPenaltyExp",codonPenaltyExp);
  };

  if (useHairpinBadness)
  { QTreeWidgetItem *branch = treeItem(tree, "useHairpinBadness", "true");
    treeItem(branch,"hairpinPenaltyLin",hairpinPenaltyLin);
    treeItem(branch,"hairpinPenaltyExp",hairpinPenaltyExp);
    treeItem(branch,"hairpinStemMin",hairpinStemMin);
    treeItem(branch,"hairpinStemMax",hairpinStemMax);
    treeItem(branch,"hairpinLengthThreshold",hairpinLengthThreshold);
    treeItem(branch,"hairpinAbortThreshold",hairpinAbortThreshold);
  };

  if (useRepeatBaseBadness)
  { QTreeWidgetItem* branch = treeItem(tree, "useRepeatBaseBadness", "true");
    treeItem(branch,"repeatBaseThreshold",repeatBaseThreshold);
    treeItem(branch,"repeatBaseAbortThreshold",repeatBaseAbortThreshold);
    treeItem(branch,"repeatBasePenaltyLin",repeatBasePenaltyLin);
    treeItem(branch,"repeatBasePenaltyExp",repeatBasePenaltyExp);
  }

  if (useRepeatBaseTypeBadness)
  { QTreeWidgetItem* branch = treeItem(tree, "useRepeatBasTypeBadness", "true");
    treeItem(branch,"repeatBaseTypeThreshold",repeatBaseTypeThreshold);
    treeItem(branch,"repeatBaseTypeAbortThreshold",repeatBaseTypeAbortThreshold);
    treeItem(branch,"repeatBaseTypePenaltyLin",repeatBaseTypePenaltyLin);
    treeItem(branch,"repeatBaseTypePenaltyExp",repeatBaseTypePenaltyExp);
  };

  if (useBaseContentBadness)
  { QTreeWidgetItem* branch = treeItem(tree, "useBaseContentBadness", "true");
    treeItem(branch,"idealGCpercent",idealGCpercent);
    treeItem(branch,"GCpercentDeltaThreshold",GCpercentDeltaThreshold);
    treeItem(branch,"GCpercentAbortThreshold",GCpercentAbortThreshold);
    treeItem(branch,"baseContentPenaltyLin",baseContentPenaltyLin);
    treeItem(branch,"baseContentPenaltyExp",baseContentPenaltyExp);
  };

  if (useCrypticSpliceBadness)
  { QTreeWidgetItem* branch = treeItem(tree, "useCrypticSpliceBadness", "true");
    treeItem(branch,"splicingPredictor",splicingPredictorName);
    treeItem(branch,"crypticSpliceThreshold",crypticSpliceThreshold);
    treeItem(branch,"crypticSpliceAbortThreshold",crypticSpliceAbortThreshold);
    treeItem(branch,"crypticSplicePenaltyLin",crypticSplicePenaltyLin);
    treeItem(branch,"crypticSplicePenaltyExp",crypticSplicePenaltyExp);
  };

  if (useRepeatBadness)
  { QTreeWidgetItem* branch = treeItem(tree, "useRepeatBadness", "true");
    treeItem(branch,"repeatThreshold",repeatThreshold);
    treeItem(branch,"repeatAbortThreshold",repeatAbortThreshold);
    treeItem(branch,"repeatPenaltyLin",repeatPenaltyLin);
    treeItem(branch,"repeatPenaltyExp",repeatPenaltyExp);

  };
};

//--------------------------------------------------------------------------------------------------
///returns type information for execution, dynamic casting etc
bool pMolSyntheticEngine :: type(int i)
{
  //if (i==pMolObject::CODONCODEX) 	return true;	else
  if (i==pMolObject::EXEC) 		return true; 	else
  if (i==pMolObject::ENGINE) 		return true; 	else
  return pMolStackObject::type(i);
}

//-----------------------------------------------------------------------------------------------
///this makes all the look-up tables which saves floating point operations during process intensive
///codon-optimization
void pMolSyntheticEngine::makeLookupTables(int segLength)
{
    qDebug() << "making Lookup Table";
  //lookup table for codon badness
  for (int i=1; i <64; i++)
    codonUsageBadnessTable[i] = codonPenaltyLin/pow(((double)i / 63.0), codonPenaltyExp) - codonPenaltyLin;	//penalty calculation
  codonUsageBadnessTable[0] = codonUsageBadnessTable[1]*100;		//avoid divide by zero problem

  //repeat base badness table
  repeatBaseBadnessTable = new double[segLength];
  for (int i=0; i<segLength; i++) 
  { repeatBaseBadnessTable[i] = repeatBasePenaltyLin*pow((double)i,repeatBasePenaltyExp);
  };

  //repeat base Type badness table
  repeatBaseTypeBadnessTable = new double[segLength];
  for (int i=0; i<segLength; i++) 
  { repeatBaseTypeBadnessTable[i] = repeatBaseTypePenaltyLin*pow((double)i,repeatBaseTypePenaltyExp);

  };

  //base content badness - note need to ingrate bad = p_lin*i^p_log which becomes (p_lin*x^(1 + p_log))/(1 + p_log)
  //we need to do this because 20 extra g' or c' in a 100 base pair stretch must be in proportion to 2 extra g' or c'
  //in a 10 base pair stretch. Then we have to correct it for the length of the segment. Look in notes date 2006-11-02
  //for more details. The input here is going to be deltaPercent
  baseContentBadnessTable = new double[100];
  for (int i=0; i<segLength; i++)
  { double temp = (baseContentPenaltyLin*pow(i,baseContentPenaltyExp+1))/(baseContentPenaltyExp+1);
    baseContentBadnessTable[i] = temp*segLength/100;
  };

};

//-----------------------------------------------------------------------------------------------
///
void pMolSyntheticEngine::deleteLookupTables()
{
  delete []repeatBaseBadnessTable;
  delete []baseContentBadnessTable;
};

//-----------------------------------------------------------------------------------------------
///calculates penalty score for repeats. If a base is repeated more than repeatThreshold, a penalty is incurred based on 
///values in repeatBadnessPenalty, . If a base is repeated more than repeatAbortThreshold, the sequence is failed.
///if a sequence fails, it returns false, otherwise all badness is added to double *badness passed to this method.
bool pMolSyntheticEngine::repeatBaseBadness(pMolSynthArray* seq, double* badness)
{
  int count;
  int length = seq->getFrameLength()*3;				//get length of the current frame
  short int* raw = seq->getFrameStart();			//get pointer to first base in frame

  int counts[16];						//store counts in an array for speed
  short int lastBase  = 0xf;					//remembers what last base was, this has a dummy initial value
  counts[lastBase] = 0;
  counts[base_c] = 0;  counts[base_g] = 0;			//set all the counters to 0
  counts[base_a] = 0;  counts[base_t] = 0;

  for (int i=0; i<length; i++)					//loop up until the length of the frame
  {
    int currentBase = *(raw+i);					//what's the base here?
    if (currentBase==lastBase)					//if it's the same as the last one
    { counts[currentBase]++;
    }
    else
    { count = counts[lastBase];
      if (marking && (count > repeatBaseThreshold))
         mark(seq->getFrameIndex() + i, count, "repeat");
      if (count > repeatBaseAbortThreshold) return false;
      if (count > repeatBaseThreshold) (*badness)+= repeatBaseBadnessTable[count-repeatBaseThreshold];
      counts[lastBase] = 0;					//reset to 0
      lastBase = currentBase;					//this base is now the last base 
      counts[currentBase] = 1;					//we oount one of this new base
    };
  };
  if (count > repeatBaseAbortThreshold) return false;
  count = counts[lastBase];
  if (count > repeatBaseThreshold) (*badness)+= repeatBaseBadnessTable[count-repeatBaseThreshold];
  return true;
};


//-----------------------------------------------------------------------------------------------
///calculates penalty score for repeats. If a base is repeated more than repeatThreshold, a penalty is incurred based on 
///values in repeatBadnessPenalty, . If a base is repeated more than repeatAbortThreshold, the sequence is failed.
///if a sequence fails, it returns false, otherwise all badness is added to double *badness passed to this method.
///groups g and c together
bool pMolSyntheticEngine::repeatBaseBadness2(pMolSynthArray* seq, double* badness)
{
  int count;
  int length = seq->getFrameLength()*3;				//get length of the current frame
  short int* raw = seq->getFrameStart();			//get pointer to first base in frame

  int counts[256];						//store counts in an array for speed
  short int lastBase  = 0x0f;					//remembers what last base was, this has a dummy initial value
  short int lastBaseType = 0x10;
  counts[lastBase] = 0;
  counts[lastBaseType] = 0;
  counts[base_c] = 0;  counts[base_g] = 0;			//set all the counters to 0
  counts[base_a] = 0;  counts[base_t] = 0;
  counts[base_a | base_t] = 0;
  counts[base_c | base_g] = 0;

  for (int i=0; i<length; i++)					//loop up until the length of the frame
  {
    int currentBase = *(raw+i);					//what's the base here?

    if (currentBase==lastBase)					//if it's the same as the last one
      counts[currentBase]++; 
    else
    { count = counts[lastBase];
      if (marking && (count > repeatBaseThreshold))
         mark(seq->getFrameIndex() + i, count, "repeat");
      if (count > repeatBaseAbortThreshold) return false;
      if (count > repeatBaseThreshold) (*badness)+= repeatBaseBadnessTable[count-repeatBaseThreshold];
      counts[lastBase] = 0;					//reset to 0
      lastBase = currentBase;					//this base is now the last base 
      counts[lastBase] = 1;					//we oount one of this new base
    };

    if (currentBase && lastBaseType)
      counts[lastBaseType] ++;
    else
    { count = counts[lastBaseType];
      if (count > repeatBaseTypeAbortThreshold) return false;
      if (count > repeatBaseTypeThreshold) (*badness)+= repeatBaseTypeBadnessTable[count-repeatBaseTypeThreshold];
      counts[lastBase] = 0;
      if ((currentBase==base_a) || (currentBase==base_t)) lastBaseType = base_a | base_t;
      if ((currentBase==base_c) || (currentBase==base_g)) lastBaseType = base_c | base_g;
      counts[lastBaseType] = 1;
    };


  };
  if (count > repeatBaseAbortThreshold) return false;
  count = counts[lastBase];
  if (count > repeatBaseThreshold) (*badness)+= repeatBaseBadnessTable[count-repeatBaseThreshold];
  return true;
};

//-----------------------------------------------------------------------------------------------
///calculates penalty score for base content. Ideally, gcPercent should be around 70%, any less than that and the RNA has a short half life 
///any more and has strong secondary structure. Penalty is calculated as follows: idealGCPercentage is what is desired (e.g. 65%). 
///baseContentDeviationThreshold is what deviation above and below is acceptable (e.g. 7%). BaseContentDeviationAbortThreshold is what is 
///unacceptable (e.g. 30%). Otherwise, the penalty is worked out based on
bool pMolSyntheticEngine::baseContentBadness(pMolSynthArray* seq, double* badness)
{
  int length = seq->getFrameLength() * 3;			//get length of the current frame
  short int* raw = seq->getFrameStart();			//get pointer to first base in frame

  int counts[16];						//store counts in an array for speed
  counts[base_c] = 0;  counts[base_g] = 0;			//set all the counters to 0
  counts[base_a] = 0;  counts[base_t] = 0;

  for (int i=0; i<length; i++)					//accumulate all the base counts in the array
    counts[(*(raw+i)) & 0x000f]++;				//the bitwise & stops buffer overflow

  int GCcontent = counts[base_c] + counts[base_g];		//work out gc content for this frame
  int GCpercent = (100 * GCcontent) / length;			//must adjust to compensate for the length otherwise

  int GCpercentDelta = abs(idealGCpercent - GCpercent);
//  qDebug() << counts[base_c] << counts[base_a] << counts[base_g] << counts[base_t] << GCcontent << GCpercent << GCpercentDelta << badBaseCount;

  if (GCpercentDelta < GCpercentDeltaThreshold) return true;
  if (GCpercentDelta > GCpercentAbortThreshold) return false;
  (*badness)+= baseContentBadnessTable[GCpercentDelta];// baseContentPenaltyLin*pow((badBaseCount),baseContentPenaltyExp);
							
  return true;
};

//-----------------------------------------------------------------------------------------------
///calculates the penalty score for local hairpins. Since the ribosome is quite good at untangling RNA, we only look for local hairpins - i.e. 
///where the head of the loop is quite short. These are the only structures that might stall a ribosome (v. little experimental evidence for this 
///however) but it probably makes sense if you can imagine the overflowing strand of a non-local RNA self-interaction. Local hairpins may be 
///digested by dicer too, another reason to avoid them. HairpinMinLoop 
bool pMolSyntheticEngine::hairpinBadness(pMolSynthArray* seq, double* badness)
{

     short int flatten[] = {0,-1, 2,-1,-1,-1, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 1,-1};



  int length = seq->getFrameLength();				//get length of the current frame
  short int* raw = seq->getFrameStart();			//get pointer to first base in frame

  int* hairpins_length = new int[length];
  int* hairpins_index  = new int[length];

  for (int i=hairpinLengthThreshold; i<length; i++)			//ok lets look for hairpins
  { 
    int longestHairpin = 0;					//longest hairpin has been 0 i.e. none yet
    hairpins_length[i] = 0;					//clear the buffer here too
    int j,k;				//start looking for hairpins minLoop from i as far as max loop
    for (j = i+hairpinStemMin; j<i+hairpinStemMin+hairpinStemMax; j++)
    {
      if (j>length) break; 			//if over the end, break out
      				//now look to see if sequence till i is a hairpin from j
      for (k=0; k<length-j; k++)
      {  if (i-k<0) break;					//if gone back too far, break out
         if (*(raw+i-k)!=complement[*(raw+j+k)]) break;			//if base not equal to complement backwards-forwards, no more hairpin, break out
      };
      if ((k>hairpinLengthThreshold) && (k>longestHairpin))		//ok we've found a longer hairpin
      { hairpins_length[i] = k;
        hairpins_index[i] = j;
        longestHairpin = k;					//if it's too long, abort
        if (longestHairpin >= hairpinAbortThreshold) 
        { delete []hairpins_index;
          delete []hairpins_length;
          return false;
        };
      };
    };
  };

  int currentHairpinIndex = 0;
  int currentHairpinLength = 0;

  for (int i=length; i>0; --i)					//now clean up any overlapping hairpins
  {								//if this is an overlapping hairpin, clean it up
    if ((currentHairpinIndex>0) && (hairpins_index[i] == currentHairpinIndex))
     hairpins_index[i] = 0;

    if (hairpins_length[i] > currentHairpinLength)		//if there is another longer hairpin here, use this one to test
    { currentHairpinIndex = hairpins_index[i];
      currentHairpinLength = hairpins_length[i];
    }

    if (currentHairpinIndex > 0)				//decrease currentHairpin to prepare for the next one
    { currentHairpinIndex--;
      currentHairpinLength--;
    };

    if (hairpins_index[i] > 0)					//work out badness
    {
      if (marking)						//mark the badness, if we're doing that
      { int index=seq->getFrameIndex();
        mark(i+index, hairpins_length[i], "hairpin");
        mark(hairpins_index[i]+index, hairpins_length[i], "hairpin");
      };
      (*badness)+=hairpinPenaltyLin*pow((double)hairpins_length[i]-hairpinLengthThreshold, hairpinPenaltyExp);
     };
  };

  delete []hairpins_index;
  delete []hairpins_length;
  return true;
};

//-----------------------------------------------------------------------------------------------
///
bool pMolSyntheticEngine::getTotalBadness(pMolSynthArray* array, bool virgin, double *badness, double *bestBadness)
{
  *badness = 0.0;

  // double codonBadness, hairpinBadness, repeatBaseBadness, baseContentBadness;
//debug

double temp;
/*
temp = 0.0;
QString debug_repeatBaseBadness;  
if (repeatBaseBadness(array, &temp)) debug_repeatBaseBadness.setNum(temp); else debug_repeatBaseBadness = "x";

temp = 0.0;
QString debug_baseContentBadness;
if (baseContentBadness(array,&temp)) debug_baseContentBadness.setNum(temp); else debug_baseContentBadness = "x";

qDebug() << array->getSequence() << array->getBadness() << debug_repeatBaseBadness << debug_baseContentBadness;
*/

  if (useCodonBadness)							//are we going to count penalties for codon useage?
  { *badness = array->getBadness();
    //if (!codonBadness(seq,badness)) return false;			//penalty for codon useage, if above failure threshld fail sequence
    if ((!virgin) && (*badness>*bestBadness)) return false;		//already worst than best, return failed
  };

/*
  if (useHairpinBadness)						//are we going to count penalties for hairpins
  { if (!hairpinBadness(array,badness)) return false;			//add penalty for hairpins, if above failure threshold returns false, fail
    if ((!virgin) && (*badness>*bestBadness)) return false;		//already worse than best, return failed
  };
*/

  if (useRepeatBaseBadness)	
    if (useRepeatBaseTypeBadness)
    { if (!repeatBaseBadness2(array,badness)) return false;		//add penalty for repeated base
      if ((!virgin) && (*badness>*bestBadness)) return false;		//already worse than best, return false
    }
    else				//are we going to count penalty for repeated bases?
    { if (!repeatBaseBadness(array,badness)) return false;		//add penalty for repeated base
      if ((!virgin) && (*badness>*bestBadness)) return false;		//already worse than best, return false
    };

/*
  if (useRepeatsBadness)						//are we going to count penalties for repeats
  { if (!repeatsBadness(seq,badness)) return false;			//add penalty for short repeats
    if ((!virgin) && (*badness>*bestBadness)) return false;
  };
*/

  if (useBaseContentBadness)						//are we going to count penalty for GC / AT content?
  { if (!baseContentBadness(array,badness)) return false;
    if ((!virgin) && (*badness>*bestBadness)) return false;
  };

/*
  if (useCrypticSpliceBadness)						//are we going to count penalties for cryptic splice sites
  { if (!crypticSpliceBadness(seq,badness)) return false;
    if ((!virgin) && (*badness>*badBadness)) return false;  
  };
*/
 // *badness = array->getBadness();
 // if (virgin) return true;
   return true; 
};

void pMolSyntheticEngine::mark(int index, int length, const QString &name)
{ 
  qDebug() << index << length << name;
};


void pMolSyntheticEngine::report
( pMolKernelInterface* interface, long unsigned int superCounter, 
  int i, const QString &seq, double bestBadness)
{
  QString temp1, temp4, temp2, temp3, temp5;
  temp2 = seq;
  temp4 = name+"::synth - [codon:"+temp1.setNum(i)+"][seg:"+temp2+"]";
  temp4+= "[badness:"+temp3.setNum(bestBadness)+"][iters:"+temp5.setNum(superCounter/5)+"/sec]";
  interface->msg(temp4);
}


//--------------------------------------------------------------------------------------------------------
///handles simple codon optimization of coding for a single peptide sequence
pMolStackObject* pMolSyntheticEngine::synth(pMolKernelInterface* interface, pMolPeptide* peptide, pMolCodonCodex* codonCodex, pMolStackList* list = NULL)
{
  pMolSynthFlatArray* forbidden;

  try
  { if (peptide->getLength() < segLength) 
      throw pMolError("peptide is too short for optimization - use ->rtrans instead");
    forbidden = new pMolSynthFlatArray(list);
  }
  catch (pMolError& error)
  {
    return makeError(interface, "synth<peptide>",error.details());
  };

  double bestBadness, nextBadness;
  makeLookupTables(segLength);
  pMolSynthArray* array1 = new pMolSynthArray(codonCodex, peptide, codonUsageBadnessTable, forbidden);
  int length = array1->getLength();
  pMolSynthArray* bestArray = NULL;  
  bool solved = false;
  array1->reset();

  int counter = 0;
  long unsigned int superCounter = 0;
  QTime time = QTime();
  time.start();

  for (int i = 0; i < length; i+=segLength/2)
  {
    array1->setFrame(i - segLength/2, i + segLength);
    array1->setMask(i - segLength/2, i, false); 
    bestArray = array1->copy(bestArray);
 
    array1->suffixTreeBuild();				//build the suffix tree
    if (!array1->suffixTreeFind()) solved = getTotalBadness(array1, true, &bestBadness, &bestBadness); else solved = false;

    try
    {
      while (array1->flip())
      {
        counter++;
        superCounter++;

        array1->suffixTreeBuild();				
        if (!array1->suffixTreeFind())
        {
          //qDebug() << array1->getSequence() << "accepted";
          if (getTotalBadness(array1, false, &nextBadness, &bestBadness))
          {
            bestArray = array1->copy(bestArray);
            bestBadness = nextBadness;
            solved = true;
          };

         //free up processing resources for a moment and give progress output
          if (counter++>20000)
          { 
            counter = 0;
            if (time.elapsed()>150) QApplication::processEvents();
            if (time.elapsed()>5000)
            { 
              time.restart();
              report(interface, superCounter, i, array1->getSequence().mid(i*3, segLength*3), bestBadness); 
              superCounter = 0;
            }
          }

     } 

     };

     if (!solved)	interface->msg("not good my friend, not good.");
     //  throw pMolError("could not mind an acceptable sequence");					//no solution found for this segment 

     array1 = bestArray->copy(array1);
   }
   catch (pMolError& error)
   {
     delete array1;
     delete bestArray;
     deleteLookupTables();
     return makeError(interface, "::synth - ", error.details());
   };
 };


  //make a new molecule and fill in sequence and ORF data
  interface->msg(name+"::synth - success!");
  pMolDNA* newDna = new pMolDNA();
  newDna->sequence = bestArray->getSequence().toUpper();
  newDna->addComment("reverse translation of "+peptide->name);
  newDna->setIndex(0);   newDna->getLength();
  pMolORF* newOrf = new pMolORF();
  newOrf->name = "ORF";
  newOrf->setIndex(0);
  newOrf->setLength(newDna->getLength());
  newOrf->codonCodex = codonCodex;
  newOrf->codonCodexName = codonCodex->name;
  newDna->append((pMolElement*) newOrf);

  //clean up used arrays etc
  delete array1;  
  delete bestArray;
  deleteLookupTables();

  //return with the finished product!
  return (pMolStackObject*) newDna;
};


//------------------------------------------------------------------------------------------------------
///handles codon optimization and wobble - here we pass synth a coding sequence ORF and synth generates
///the optimal sequence coding for the same peptide but differing by at least <i> wobbleDifferenceThreshogld </i>
///from the original sequence. Used for putting in two similar peptide codings in a retroviral vector for instance
pMolStackObject* pMolSyntheticEngine::synth(pMolKernelInterface* interface, pMolORF* orf, pMolCodonCodex* codonCodex, pMolLayer* layer)
{
  //prepare everything for codon optimization
  double bestBadness, nextBadness;
  makeLookupTables(segLength);
  pMolSynthArray* array0 = new pMolSynthArray(codonCodex, (pMolElement*) orf, codonUsageBadnessTable);
  pMolSynthArray* array1 = array0->copy(NULL);
  int length = array1->getLength();
  pMolSynthArray* bestArray = NULL;
  bool solved = false;
  array1->reset();

  //if there is a layer, mask the codons which fall under the layer
  if (layer!=NULL)
  {
    pMolElement* element; int i = 0;
    while ((element=layer->getElement(&i))!=NULL)
    { int index = element->getIndex();
      int length = element->getLength();
      if (length>0) 
        array1->setMask(index/3, (index+length)/3, false);
      else
        array1->setMask((index+length)/3, index/3, false);
    };
  };

  int counter = 0;
  long unsigned int superCounter = 0;
  QTime time = QTime();
  time.start();
  //search segmentally for optimal sequence
  for (int i = 0; i < length; i+=segLength/2)
  {
    array0->setFrame(i - segLength/2, i + segLength);
    array1->setFrame(i - segLength/2, i + segLength);
    array1->setMask(i - segLength/2, i, false); 
    bestArray = array1->copy(bestArray);
    solved = getTotalBadness(array1, true, &bestBadness, &bestBadness);

    while (array1->flip())
    {
      superCounter++;
      if (counter++>20000)
      { counter = 0;
        if (time.elapsed()>150) QApplication::processEvents();
        if (time.elapsed()>5000)
        { 
          time.restart();
          QString temp1, temp4, temp2, temp3, temp5;
          temp2 = bestArray->getSequence().mid(i*3, segLength*3);
          temp4 = name+"::synth - [codon:"+temp1.setNum(i)+"][seg:"+temp2+"]";
          temp4+= "[badness:"+temp3.setNum(bestBadness)+"][iters:"+temp5.setNum(superCounter/5)+"/sec]";
          interface->msg(temp4);
          superCounter = 0;
        }
      };

//    qDebug() << array0->getSequence() << array1->getSequence() << array1->compare(array0);
      if (array1->compare(array0) < wobbleDifferenceThreshold)
        if (getTotalBadness(array1, false, &nextBadness, &bestBadness))
        {
          bestArray = array1->copy(bestArray);
          bestBadness = nextBadness;
          solved = true;
        };
      };
    if (!solved)						//no solution found for this segment 
    { pMolPeptide* peptide = new pMolPeptide();
      peptide->sequence = codonCodex->translate(array1->getSequence().mid(i*3, segLength*3));
      delete array1;						//so clean up and exit with an error message
      delete bestArray;
      deleteLookupTables();
      interface->msg(name+"::synth - failed to find acceptable sequence");
      interface->msg(name+"::synth - failed segment returned as peptide");
      return (pMolStackObject*) peptide;
    };
    array1 = bestArray->copy(array1);
  };

  //make a new molecule and fill in sequence and ORF data
  interface->msg(name+"::synth - success!");
  pMolDNA* newDna = new pMolDNA();
  newDna->sequence = bestArray->getSequence().toUpper();
  newDna->addComment("reverse translation of "+orf->name);
  newDna->setIndex(0);   newDna->getLength();
  pMolORF* newOrf = new pMolORF();
  newOrf->name = "ORF";
  newOrf->setIndex(0);
  newOrf->setLength(newDna->getLength());
  newOrf->codonCodex = codonCodex;
  newOrf->codonCodexName = codonCodex->name;
  newDna->append((pMolElement*) newOrf);

  //clean up used arrays etc
  delete array0;
  delete array1;  
  delete bestArray;
  deleteLookupTables();

  //return with the finished product!
  return (pMolStackObject*) newDna;
};




//----------------------------------------------------------------------------------------------
///ORs the restriction recognition sequence with DNA sequence and 
int pMolSyntheticEngine::searchRestriction(short int* s, short int*r, int I, int slength, int rlength)
{
  int i,j;
  for (i=I; i<slength-rlength; i++)
  {
   // qDebug() << "s" << slength << rlength << s[i];
    for (j=0; j<rlength; j++)
      if (((*(s+i+j)) & (*(r+j)))==0) break;
    if (j==rlength) return i;
  };
  return -1;
};

//--------------------
///
pMolStackObject* pMolSyntheticEngine::c_restrict(pMolCmd* cmd, pMolKernelInterface* interface)
{
  QString error;

  //if we didn't get any parameters, then send an error and return
  pMolStackList* list = pMolStackList::interpretParameter(cmd, interface, (pMolObject*) this, error);
  if (list==NULL) 
    return makeError(interface, "restrict", error);

  //if there's one number parameter, cut bit off front or back depending if pos or neg
  if (!list->checkParameters(pMolObject::DNA, pMolObject::CODONCODEX, pMolObject::LIST, error))
    return makeError(interface, "restrict", error);

  //extract all the bits!
  pMolElement* orf = dynamic_cast <pMolElement*> (list->at(0));
  pMolCodonCodex* codonCodex = dynamic_cast <pMolCodonCodex*> (list->at(1));
  pMolStackList* reListRaw = dynamic_cast <pMolStackList*> (list->at(2));

  int reMaxLength = -1;
  //check that list only contains restriction endonucleases, cast them and put them into a list structure
  QList <pMolRestrictionEndonuclease*> reList;
  foreach (pMolStackObject* object, reListRaw->list) 
  { if (object==NULL) return makeError(interface, "restrict", "list of restriction endonucleases is fcuked");
    if (!object->type(pMolObject::RESTRICTIONENDONUCLEASE)) return makeError(interface, "restrict", "item in list wrong");
    pMolRestrictionEndonuclease* re = dynamic_cast<pMolRestrictionEndonuclease*> (object);
    if (re->getLength() > reMaxLength) reMaxLength = re->getLength();
    reList.append(re);
    interface->msg(re->getName());
  };

  if (reMaxLength < 0) 
    return makeError(interface, "restrict", "something was wrong with the list of restriction endonucleases");

  //prepare everything for codon optimization
  double bestBadness, nextBadness;
  makeLookupTables(segLength);
  interface->msg(orf->getSequence());
  pMolSynthArray* array0 = new pMolSynthArray(codonCodex, orf, codonUsageBadnessTable);
  pMolSynthArray* array1 = array0->copy(NULL);
  qDebug() << array0->getSequence();
  qDebug() << array1->getSequence();
  int length = array1->getLength();
  pMolSynthArray* bestArray = NULL;
  bool solved = false;


  array1->reset();
  qDebug() << array1->getSequence();
  int counter = 0;

  bestArray = array1->copy(bestArray);
  solved = false;

  interface->msg(array1->getSequence());
  for (int i = 0; i < array1->getLength(); i++)
  {
    array1->setFrame(i, i+reMaxLength/3); 
    array1->reset(i, i+reMaxLength/3);
    while (array1->flip())
    {

//    qDebug() << array0->getSequence() << array1->getSequence() << array1->compare(array0);
      int reSite;
      bool found = false;
      pMolRestrictionEndonuclease* reFound;
      foreach (pMolRestrictionEndonuclease* re, reList)
      {
        int r = searchRestriction(array1->getFrameStart(), re->getSeqbuff(), 0, array1->getFrameLength()*3, re->getLength());
        if (r > 0) 
        {
          found = true;
          reFound = re;
          reSite = r+i*3;
        }
      }

      if (found) 
      {
       // getTotalBadness(array1, false, &nextBadness, &bestBadness);
        bestArray = array1->copy(bestArray);
        bestBadness = nextBadness;
        solved = true;

        QString temp1, temp4, temp2, temp3;
        temp2 = array1->getSequence();
        temp4 = name+"::restrict - found "+reFound->getName()+" "+temp1.setNum(reSite)+" "+temp2+" "+temp3.setNum(bestBadness);
        interface->msg(temp4);
        QApplication::processEvents();
      };
    };
   QString temp1, temp2;
   interface->msg("break"+temp1.setNum(array1->getFrameIndex())+","+temp2.setNum(array1->getFrameLength()));
   array1 = array0->copy(array1);
  }
  if (!solved)						//no solution found for this segment 
  { delete array0;						//so clean up and exit with an error
    delete array1;						//message
    delete bestArray;
    deleteLookupTables();
    return makeError(interface, "restrict<dna>", "could not find acceptable sequence");
  };
  array1 = bestArray->copy(array1);
  

  //make a new molecule and fill in sequence and ORF data
  pMolDNA* newDna = new pMolDNA();
  newDna->sequence = bestArray->getSequence().toUpper();
  newDna->addComment("reverse translation of "+orf->name);
  newDna->setIndex(0);   newDna->getLength();
  pMolORF* newOrf = new pMolORF();
  newOrf->name = "ORF";
  newOrf->setIndex(0);
  newOrf->setLength(newDna->getLength());
  newOrf->codonCodex = codonCodex;
  newOrf->codonCodexName = codonCodex->name;
  newDna->append((pMolElement*) newOrf);

  //clean up used arrays etc
  delete array0;
  delete array1;  
  delete bestArray;
  deleteLookupTables();

  //return with the finished product!
  return (pMolStackObject*) newDna;
};


//------------------------------------------------------------------------------------------------------------------
///this handles synthesis commands
pMolStackObject* pMolSyntheticEngine::synth(pMolCmd* cmd, pMolKernelInterface* interface)
{
  QString error, sequence, name;
  pMolCodonCodex* codonCodex;

  //if we didn't get any parameters, then send an error and return
  pMolStackList* list = pMolStackList::interpretParameter(cmd, interface, (pMolObject*) this, error);
  if (list==NULL) return makeError(interface, "synth", error);

  //if there's one number parameter, cut bit off front or back depending if pos or neg
  if (list->checkParameters(pMolObject::PEPTIDE, pMolObject::CODONCODEX, error))
  {
    pMolPeptide* peptide = dynamic_cast <pMolPeptide*> (list->at(0));
    codonCodex = dynamic_cast <pMolCodonCodex*> (list->at(1));
    return synth(interface, peptide, codonCodex);
  }

  //if there's one number parameter, cut bit off front or back depending if pos or neg
  if (list->checkParameters(pMolObject::PEPTIDE, pMolObject::CODONCODEX, pMolObject::LIST, error))
  {
    qDebug() << "ok found the list";
    pMolPeptide* peptide = dynamic_cast <pMolPeptide*> (list->at(0));
    codonCodex = dynamic_cast <pMolCodonCodex*> (list->at(1));
    pMolStackList* forbiddenList = dynamic_cast <pMolStackList*> (list->at(2));
    return synth(interface, peptide, codonCodex, forbiddenList);
  }

  //if there's and ORF then go to wobble
  if (list->checkParameters(pMolObject::ORF, pMolObject::CODONCODEX, error))
  {
    pMolORF* dna = dynamic_cast <pMolORF*> (list->at(0));
    pMolCodonCodex* codonCodex = dynamic_cast <pMolCodonCodex*> (list->at(1));
    return synth(interface, dna, codonCodex, NULL);
  }

  //if there's and orf and a layer, wobble but mask the layer
  if (list->checkParameters(pMolObject::ORF, pMolObject::CODONCODEX, pMolObject::LAYER, error))
  {
    pMolORF* dna = dynamic_cast <pMolORF*> (list->at(0));
    pMolCodonCodex* codonCodex = dynamic_cast <pMolCodonCodex*> (list->at(1));
    pMolLayer* layer = dynamic_cast <pMolLayer*> (list->at(2));
    return synth(interface, dna, codonCodex, layer);
  }

  return makeError(interface, "synth", "wrong paramters");
};


//------------------------------------------------------------------------------------------------------------------
///this reverse translates peptide sequences
pMolStackObject* pMolSyntheticEngine::c_rtrans(pMolCmd* cmd, pMolKernelInterface* interface)
{
  QString error, sequence, name;
  pMolCodonCodex* codonCodex;

  //if we didn't get any parameters, then send an error and return
  pMolStackList* list = pMolStackList::interpretParameter(cmd, interface, (pMolObject*) this, error);
  if (list==NULL) return makeError(interface, "rtrans", error);

  //if there's one number parameter, cut bit off front or back depending if pos or neg
  if (!list->checkParameters(pMolObject::PEPTIDE, pMolObject::CODONCODEX, error)) 
    return makeError(interface, "transt", error);

  pMolPeptide* peptide = dynamic_cast <pMolPeptide*> (list->at(0));
  codonCodex = dynamic_cast <pMolCodonCodex*> (list->at(1));

  pMolCodon* baseCodon = codonCodex->getBaseCodon();
  qDebug() << "ok!!! about to get sequence";
  sequence = peptide->getSequence().toUpper();

  qDebug() << "ok!!" << sequence;

  //find simple best reverse translation of each 
  QString dnaSequence = "";
  for (int i = 0; i < peptide->getLength(); i++)
  {
    bool ok;
    pMolCodon* codon=baseCodon->searchTranslation(sequence.at(i));
    codon = codon->reset();
    pMolCodon* tempCodon = codon->flip(&ok);
    while (ok)
    { if (tempCodon->badness < codon->badness) codon = tempCodon;
      tempCodon = tempCodon->flip(&ok);
    };
    dnaSequence += codon->code;
    qDebug() << "ok!!!" << codon->code;
  };

  //make new dna molecule and annotate with orf
  pMolDNA* newDna = new pMolDNA();
  newDna->sequence = dnaSequence.toUpper();
  newDna->addComment("reverse translation of "+peptide->sequence);
  newDna->setIndex(0);   newDna->getLength();
  pMolORF* newOrf = new pMolORF();
  newOrf->name = "orf";
  newOrf->setIndex(0);
  newOrf->setLength(newDna->getLength());
  newOrf->codonCodex = codonCodex;
  newOrf->codonCodexName = codonCodex->name;
  newDna->append((pMolElement*) newOrf);

  return (pMolStackObject*) newDna;
};

//------------------------------------------------------------------------------------------------------------------
///This handles command directed at this oject
pMolStackObject* pMolSyntheticEngine::exec(pMolCmd* cmd, pMolKernelInterface* interface)
{
  //no paramaters, return this as object
  if (cmd==NULL) return (pMolStackObject*) this;				//no paramaters, return me

  //if no command but not empty, something is wrong (needs to be empty or have subcommand) so return with error
  if (cmd->type() != pMolCmd::NAME) return makeError(interface,"exec","empty field or parameter list expected");
  pMolCmdName* cmdName = dynamic_cast<pMolCmdName*>(cmd);			//safe to assume this is a pMolCmdName

  if (cmdName->name == "rtrans")	return c_rtrans(cmdName->child, interface);
  if (cmdName->name == "synth")  	return synth(cmdName->child, interface);
  if (cmdName->name == "restrict") 	return c_restrict(cmdName->child, interface);
  return makeError(interface, "->","command not understood");
};
