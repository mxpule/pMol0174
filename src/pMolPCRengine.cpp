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

#include "pMolPCRengine.h"


    pMolPCRengine::pMolPCRengine() : pMolStackObject(NULL)
    {
      entropyTarget  = &dummy;				//cellotape	
      enthalpyTarget = &dummy;				//cellotape
      aquiringHybridationData = false;
    };


    pMolPCRengine::pMolPCRengine(pMolXMLbase *p_pMolXMLbaseParent) : pMolStackObject(p_pMolXMLbaseParent)
    {
      entropyTarget  = &dummy;				//cellotape	
      enthalpyTarget = &dummy;				//cellotape
      aquiringHybridationData = false;
    };


    pMolXMLbase* pMolPCRengine::XMLopen(const QString &tag, QString &error)
    {
      if (tag=="hybridationTable") {aquiringHybridationData=true; qDebug() << "found opening tag hybridationTable"; };
      return pMolXMLbase::XMLopen(tag, error);
    };

//--------------------------------------------------------------------------------------------------------------------
bool pMolPCRengine::type(int i)
{ 
  if (i == pMolStackObject::EXEC) return true;
  return pMolStackObject::type(i);
};

//--------------------------------------------------------------------------------------------------------------------
///I know this is ugly but I wanted all the data to fit into one class
    pMolXMLbase* pMolPCRengine::aquireHybridationData(const QString &tag, const QString &data, QString &error)
    {
      short int buffer[16];
      if ((tag=="pair") && (data.length()==2))
      { if (convertToByte(data, buffer))
        {
          short int i = (buffer[0]<<2) + buffer[1];
          entropyTarget  = &entropyDuplex[i];
          enthalpyTarget = &enthalpyDuplex[i];
        };
      }
      else if ((tag=="base") && (data.length()==1))
      { if (convertToByte(data, buffer))
        {
          short int i = buffer[0];
          entropyTarget  = &entropyEnd[i];
          enthalpyTarget = &enthalpyEnd[i];
        };
      }
      else if (tag=="entropy")  XMLdouble(entropyTarget,  data, error); 
      else if (tag=="enthalpy") XMLdouble(enthalpyTarget, data, error); 
      else if (tag=="name")     hybridationTableName = data;
      else if (tag=="comment")  hybridationTableComment = data;
      else if (tag=="hybridationTable") aquiringHybridationData = false;

      return (pMolXMLbase*) this; 
    };
 
//--------------------------------------------------------------------------------------------------------------------
    pMolXMLbase* pMolPCRengine::XMLclose(const QString &tag, const QString &data, QString &error)
    {
      if (aquiringHybridationData)
        return aquireHybridationData(tag, data, error);

      else if (tag=="hybridationType") 		hybridationType = data;
      else if (tag=="saltConcentration") 		XMLdouble(&saltConcentration, data, error);
      else if (tag=="saltCorrectionMethod")		saltCorrectionMethod = data;
      else if (tag=="nucleicAcidConcentration") 	XMLdouble(&nucleicAcidConcentration, data, error);
      else if (tag=="maxPrimerLength")           	XMLnumeric(&maxPrimerLength, data, error);
      else if (tag=="minPrimerLength")			XMLnumeric(&minPrimerLength, data, error);
      else if (tag=="cloningPrimerTm")			XMLdouble(&cloningPrimerTm, data, error);
      else if (tag=="sequencingPrimerTm")		XMLdouble(&sequencingPrimerTm, data, error);
      else if (tag=="sequencerLength")			XMLnumeric(&sequencerLength, data, error);
      else if (tag=="maxTmWobble")			XMLdouble(&maxTmWobble, data, error);
      else if (tag=="sequenceWobble")         		XMLnumeric(&sequenceWobble, data, error);
      else if (tag=="maxSyntheticLength")   		XMLnumeric(&maxSyntheticLength, data, error);
      else if (tag=="cloningPrimerPadding")		XMLnumeric(&cloningPrimerPadding, data, error);
      else if (tag=="fragmentMinLength")		XMLnumeric(&fragmentMinLength, data, error);
      else if (tag=="fragmentMaxLength")		XMLnumeric(&fragmentMaxLength, data, error);
      else if (tag=="fragmentPrimerTm")		XMLdouble(&fragmentPrimerTm, data, error);
      else if (tag=="fragmentSegLength")		XMLnumeric(&fragmentSegLength, data, error);

      else return pMolXMLbase::XMLclose(tag, data,error);

      return (pMolXMLbase*) this; 
    }

//--------------------------------------------------------------------------------------------------------------------
///returns tree structure of this class
 void pMolPCRengine::treeOut(QTreeWidgetItem *tree)
 {
    QString temp, duplexes;
    //send out the basic stuff
    pMolXMLbase::treeOut(tree);
    treeItem(tree,"hybridationType",hybridationType);
    treeItem(tree,"saltConcentration",saltConcentration);
    treeItem(tree,"saltCorrectionMethod",saltCorrectionMethod);
    treeItem(tree,"nucleicAcidConcentration",nucleicAcidConcentration);
    treeItem(tree,"maxPrimerLength",maxPrimerLength);
    treeItem(tree,"minPrimerLength",minPrimerLength);
    treeItem(tree,"cloningPrimerTm",cloningPrimerTm);
    treeItem(tree,"cloningPrimerPadding", cloningPrimerPadding);
    treeItem(tree,"sequencingPrimerTm",sequencingPrimerTm);
    treeItem(tree,"sequencerLength",sequencerLength);	
    treeItem(tree,"maxTmWobble",maxTmWobble);
    treeItem(tree,"sequenceWobble",sequenceWobble);
    treeItem(tree,"maxSyntheticLength",maxSyntheticLength);
    treeItem(tree,"fragmentMinLength",fragmentMinLength);
    treeItem(tree,"fragmentMaxLength",fragmentMaxLength);
    treeItem(tree,"fragmentPrimerTm",fragmentPrimerTm);
    treeItem(tree,"fragmentSegLength",fragmentSegLength);

    //make the hybridationTable holder
    QTreeWidgetItem* table = treeItem(tree,"hybridationTable", hybridationTableName);
    treeItem(table,"comment",hybridationTableComment);

    //send out the ends first
    QTreeWidgetItem* base;
    duplexes = "ATCG";
    for (int i=0; i<4; i++)
    {  base = treeItem(table,"end",duplexes.mid(i,1));
       treeItem(base,"enthalpy",enthalpyEnd[i]);
       treeItem(base,"entropy" ,entropyEnd[i]);
    }

    //now send out the duplexes, just skip through this string and you have all 16 possibilities!
    short int buffer[32];
    duplexes = "AAACAGATCACCCGCTGAGCGGGTTATCTGTT";
    convertToByte(duplexes, buffer);
    for (int i=0; i<duplexes.length(); i+=2)
    {
      int j = (buffer[i]<<2) + buffer[i+1];
      base = treeItem(table,"duplex",duplexes.mid(i,2));
      treeItem(base,"enthalpy",enthalpyDuplex[j]);
      treeItem(base,"entropy" ,entropyDuplex[j]);
    };
  };

//--------------------------------------------------------------------------------------------------------------------
///go through the string for Tm calculations and convert it so that duplexes can easily be changed into
///a look-up table index. So 'A' becomes 0, 'C' becomes 2, 'G' becomes 3, 'T' becomes 2.
///done this way so easy to see if C or G
   bool pMolPCRengine::convertToByte(const QString &s, short int* buffer)
   {
     //                     A  1  C  3  4  5  G  7  8  9 10 11 12 13 14 15 16 17 18  T 20
     short int flatten[] = {0,-1, 2,-1,-1,-1, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 1,-1};
     for (int i=0; i<s.length(); i++)
     { int t;
       int j = s.at(i).unicode() - 65;
       if ((j<0) || (j>20)) return false;
       if ((t=flatten[j])<0) return false;
       *(buffer+i) = t;
     };
     return true;
   };



//--------------------------------------------------------------------------------------------------------------------
///this is the business method, calculates the Tm of a DNA sequence
  double pMolPCRengine::melting(const QString &s)
  {
    //prepare for calculation
    int length = s.length();			// for speed
    short int* buffer = new short int[length+16];
    if (!convertToByte(s, buffer))
    { //interface->msg("pMolDNAengine::non-standard character in sequence, can't calculate Tm");
      delete []buffer;
      return NULL;
    }

   //get the index of the ends in the look up table
    short int i = buffer[0];
    short int j = buffer[length-1];

    double totalEntropy  = 0.0;
    totalEntropy += entropyEnd[i];
    totalEntropy += entropyEnd[j];

  //calculate the enthalpy from the ends
    double totalEnthalpy = 0.0;
    totalEnthalpy+= enthalpyEnd[i];
    totalEnthalpy+= enthalpyEnd[j];

 qDebug() << totalEntropy << totalEnthalpy;

  //calculate both for each duplex
    for (i=0; i<length-1; i++)
    { j = (buffer[i]<<2) + buffer[i+1];
      totalEntropy += entropyDuplex[j];
      totalEnthalpy+= enthalpyDuplex[j];
    }

  //work out salt correction
  double saltCorrection;

 qDebug() << totalEntropy << totalEnthalpy;

  if (saltCorrectionMethod=="wet91a") 
  { saltCorrection = 16.6 * log10 (saltConcentration/(1.0 + 0.7 * saltConcentration)) - 269.32; } 

  else if (saltCorrectionMethod=="san96a") 
  { saltCorrection = 12.5 * log10 (saltConcentration) - 273.15; }  

  else if (saltCorrectionMethod=="san98a") 
  { saltCorrection = -273.15; totalEntropy += 0.368 * (length-1) * log (saltConcentration); } 

  else 
  { //interface->msg("pMolPCRengine::Salt Correction method not known"); 
    delete buffer; return NULL; };

  //calculate Tm
  double Tm = totalEnthalpy/(totalEntropy + 1.987 * log(nucleicAcidConcentration/d_gnat)) + saltCorrection;

  delete []buffer;
  return Tm;
};

//--------------------------------------------------------------------------------------------------------------------
///this is the business method, calculates the Tm of a DNA sequence
///doesn't allocated memory
  double pMolPCRengine::melting(short int* buffer, int i0, int i1)
  {
   //get the index of the ends in the look up table
    short int i = *(buffer+i0);
    short int j = *(buffer+i1-1);

    double totalEntropy  = 0.0;
    totalEntropy += entropyEnd[i];
    totalEntropy += entropyEnd[j];

  //calculate the enthalpy from the ends
    double totalEnthalpy = 0.0;
    totalEnthalpy+= enthalpyEnd[i];
    totalEnthalpy+= enthalpyEnd[j];

  //calculate both for each duplex
    for (i=i0; i<i1-1; i++)
    { j = (buffer[i]<<2) + buffer[i+1];
      totalEntropy += entropyDuplex[j];
      totalEnthalpy+= enthalpyDuplex[j];
    }

    int length = i1-i0;
  //work out salt correction
  double saltCorrection;

  if (saltCorrectionMethod=="wet91a") 
  { saltCorrection = 16.6 * log10 (saltConcentration/(1.0 + 0.7 * saltConcentration)) - 269.32; } 

  else if (saltCorrectionMethod=="san96a") 
  { saltCorrection = 12.5 * log10 (saltConcentration) - 273.15; }  

  else if (saltCorrectionMethod=="san98a") 
  { saltCorrection = -273.15; totalEntropy += 0.368 * (length-1) * log (saltConcentration); } 

  else 
  { //interface->msg("pMolPCRengine::Salt Correction method not known"); 
   delete buffer; return NULL; };

  //calculate Tm
  double Tm = totalEnthalpy/(totalEntropy + 1.987 * log(nucleicAcidConcentration/d_gnat)) + saltCorrection;

  return Tm;
};


//--------------------------------------------------------------------------------------------------------------------
///works out a badness score for an oligo. I have concentrated mostly on the 3' end,
///need to work more on homodimerization etc etc but couldn't be bothered right now
///note this is not based on any science but my experience of what made a good primer!
///if you don't like this, write your own badness. Should take into account homodimerization at least
double pMolPCRengine::getBadness(short int* b, int length)
{
  double badness = 0.0;
  //3' end not a G or C is very bad!!! so bad almost certainly will be rejected
  if (!(*(b+length-1) & 2)) badness+=100;

  //-1 position it is good to be an A or T
  if ((*(b+length-2) & 2)) badness+=30;

  //-1 position it is good to be an A or T but not as good as -1
  if ((*(b+length-3) & 2)) badness+=10;

  //work out how many repeats present and penalize accordingly
  int repeat = 0;					//number of repeats
  short int prevBase = -1;				//what the repeated base is
  for (int i=0;i<length;i++)				//loop through sequence
  { if ((*(b+i))==prevBase) repeat++;
    else
    { if (repeat>3) badness+=(prevBase+1)*repeat*2;		//penalize (needs to be better, a GGGG is really bad)
      repeat=1; prevBase=*(b+i);
    };
  };

  return badness;
}

//--------------------------------------------------------------------------------------------------------------------
///works out a badness score for an oligo. I have concentrated mostly on the 3' end,
///need to work more on homodimerization etc etc but couldn't be bothered right now
///note this is not based on any science but my experience of what made a good primer!
///if you don't like this, write your own badness. Should take into account homodimerization at least
double pMolPCRengine::getDoubleBadness(short int* b, int length)
{
  double badness = 0.0;
  //3' end not a G or C is very bad!!! so bad almost certainly will be rejected
  if (!(*(b+length-1) & 2)) badness+=100;

  //-1 position it is good to be an A or T
  if ((*(b+length-2) & 2)) badness+=30;

  //-1 position it is good to be an A or T but not as good as -1
  if ((*(b+length-3) & 2)) badness+=10;

  if (!(*(b+0) & 2)) badness+=100;

  //-1 position it is good to be an A or T
  if ((*(b+1) & 2)) badness+=30;

  //-1 position it is good to be an A or T but not as good as -1
  if ((*(b+2) & 2)) badness+=10;
  //work out how many repeats present and penalize accordingly
  int repeat = 0;					//number of repeats
  short int prevBase = -1;				//what the repeated base is
  for (int i=0;i<length;i++)				//loop through sequence
  { if ((*(b+i))==prevBase) repeat++;
    else
    { if (repeat>3) badness+=(prevBase+1)*repeat*2;		//penalize (needs to be better, a GGGG is really bad)
      repeat=1; prevBase=*(b+i);
    };
  };

  return badness;
}

//--------------------------------------------------------------------------------------------------------------------
///gives badness of Tm for fragmentation - exponential!
double pMolPCRengine::fragTmBadness(short int* dna, int i, int j)
{
  double Tm = melting(dna, i, j);
  double delta = abs(fragmentPrimerTm - Tm);
  if (delta < maxTmWobble) return 0.0;
  return pow(3,delta);
};

//--------------------------------------------------------------------------------------------------------------------
///recursive function searching to find the best fragmentation path
///Business end of this process. No weighting is given to the oligo other than its Tm (very little choice)
void pMolPCRengine::fragTry(pMolKernelInterface* interface, short int* dna, int segment, int i, int* path, double badness, int* bestPath, int *bestI, double* bestBadness)
{
  //timer so the punter knows what's going on
  superCounter++;
  if (counter++>20000)
  { counter = 0;
    if (timer.elapsed()>150) QApplication::processEvents();
    if (timer.elapsed()>5000)
    { 
      timer.restart();
      QString temp1, temp4;
      temp4 = name+"::frag - exploring path ";
      temp4 = temp4+"[base="+temp1.setNum(path[i])+"]";
      temp4 = temp4+"[bestBadness="+temp1.setNum(*bestBadness)+"]";
      temp4 = temp4+"[iteration="+temp1.setNum(superCounter/5)+"/sec]";
      interface->msg(temp4);
      superCounter = 0;
     }
  };

  //have we reached our segmentation limit? then if this is the best so far, copy it into the best
  if ((path[i] > segment) && ((i & 0x01) == 0))
  {
    if (badness >= *bestBadness) return;				//not the best, simply leave
    for (int j = 0; j <= i+1; j++) bestPath[j] = path[j];		//best so far so copy over into the bestPath
    *bestBadness = badness;						//copy over the score
    *bestI = i;								//and the index
    return;
  };

  //otherwise, spawn paths around where we can without spanning oligo to short or too long
  for (int j = path[i] + fragmentMinLength; j - path[i-1] <= fragmentMaxLength; j++)
  {
    path[i+1] = j;							//mark next putative break site
    double tempBadness = badness + fragTmBadness(dna, path[i], j);	//melting temperature badness
    if (tempBadness < *bestBadness)					//don't bother if already sucks!
      fragTry(interface, dna, segment, i+1, path, tempBadness, bestPath, bestI, bestBadness);
  };

  return;
};

//--------------------------------------------------------------------------------------------------------------------
/// this member takes a dna object and fragments it into overlapping oligos. It uses this object's
/// <i>fragmentMaxLength</i> and <i>fragmentMinLength</i> to determine the range of sizes allowed 
/// *Note if you don't want this to take all day, make sure that the difference between the two <br>
/// shouldn't be too much. Also if the minimum is greated than maximum / 2, can't work! d'ohhhh<br>
/// for longer sequences, the problem is split into fragments max size is<i>fragmentSegLength</i><br>
/// *Note, computation time increases exponentially with this length! About 300 is a good number<br>
/// *Note, some padding may be introduced into the 3' end <br>
/// *Note, I've termed 'runt' the shorter non-spanning oligos that occur at either end<br>
pMolStackObject* pMolPCRengine::frag(pMolCmd* cmd, pMolKernelInterface* interface)
{
  QString error;

  //make sure that oligo lengths allowed are do-able 
  if (fragmentMinLength >= fragmentMaxLength / 2) return makeError(interface, "frag", "fragment Lengths specified impossible");

  //if we didn't get any parameters, then send an error and return
  pMolStackList* list = pMolStackList::interpretParameter(cmd, interface, (pMolObject*) this, error);
  if (list==NULL) return makeError(interface, "frag", error);

  //see if parameter fits expected - a dna object, otherwise return an error and process sequence
  if (!list->checkParameter(0, pMolObject::RAW, error)) return makeError(interface, "frag", "need a raw sequence for now");
  pMolDNA* p_dna = dynamic_cast <pMolDNA*> (list->at(0));	//extract the dna object
  int length = p_dna->getLength();			 		//length of the sequence

  //duplicate the dna
  pMolDNA* dna = dynamic_cast <pMolDNA*> (p_dna->duplicate());
  dna->name = p_dna->getName() + "_frag";

  //make 3' random padding with good gc content
  QString padding3;
  srand((unsigned)time(0)); 
  for (int n = 0; n < fragmentMaxLength * 3; n++)
  { int random = int(100.0*rand()/(RAND_MAX + 1.0));
    if (random < 20) padding3 += 'A';
    else if (random < 40) padding3 +='T';
    else if (random < 70) padding3 +='C';
    else padding3 += 'G';
  };
  pMolAnnotation* padding = dna->extend3("padding3", "random bases introduced by"+name, padding3);

  //convert to raw format
  qDebug() << "frag" << dna->getSequence() << dna->getLength();
  short int* raw = new short int[dna->getLength()+100];				//allocate space for the raw sequence
  if (!convertToByte(dna->getSequence(), raw))				//convert to raw byte sequence
  { delete []raw; 	delete dna;					//something went wrong so free up space
    return makeError(interface, "frag", "couldn't convert sequence");	//and return error
  }

  qDebug() << "got to here.0";
  //set everything up for the recursive search
  int i = 1;  int oldI = 1;						//index for starting is 1. Dummy value at 0 (5' runt)
  int longestPossiblePath = 4 + (dna->getLength() / fragmentMinLength);	//longest possible path for space allocation
  int* path = new int[longestPossiblePath];				//allocate space for the searching path
  int* bestPath = new int[longestPossiblePath];				//allocate space for storage of the best path found

  double bestBadness = impossiblyBad;					//set the best badness for huge value
  path[0] = -fragmentMinLength;                        			//set the initial condition to hide existence of 5'runt
  path[1] = 0;								//set the starting to the first base pair

  float divisions = round((float) length / (float) fragmentSegLength);	//intelligently fragment long sequences
  int deltaSegment;							//impossible processing times. 
  if (divisions < 1.0) deltaSegment = length;				//if it's short can do it in one go
  else deltaSegment = 1 + length / (int) divisions;			//otherwise split into sensible
  int segment = deltaSegment;						//segments

  //start the recursive searching
  timer.restart();
  counter = 0;
  superCounter = 0;

  qDebug() << "got to here..";
  while (path[i] < length)						//as long as we haven't reached the end
  {
    oldI = i;								//remember current position to see if we moved any
    fragTry(interface, raw, segment, i, path, 0, bestPath, &i, &bestBadness);	
    if (i == oldI) break;						//didn't move - failed!
    for (int j = 0; j <= i; j++) path[j] = bestPath[j];			//copy over best for this segment
    segment+=deltaSegment;						//increment to the next segment
    bestBadness = impossiblyBad;					//reset badness for next segment
  };

  qDebug() << "got to here!!!";
  //if failed leave send an error message
  if (i == oldI)  
  { makeError(interface, "frag", "could not find an acceptable fragmentation path");
    for (int j = 0; j <= i; j++)  bestPath[j]= path[j];			//even if partial failure, can see what we have
  };

  qDebug() << "got to here 004";
  //create a layer to store the fragmentation information
  pMolLayer* layer = new pMolLayer();					//make a new layer
  layer->pMolParent = dna;						//set its derivation from input dna
  layer->addComment("fragmentation layer");
  layer->name = "frag";
  dna->append(layer);
  QString oligoComment = "Fragmentation of "+dna->getName();

  bestPath[0] = 0;							//hide the 5' runt
  bestPath[i+1] = bestPath[i];  					//hide the 3' runt;

  qDebug() << "got to here005";
  //annotate in the hybridation units and work out max and min Tms
  double minTm = 100.0;							//impossibly high Tm
  double maxTm = 0.0;							//impossibly low Tm
  QString temp1, temp2, temp3;
  for (int j=2; j<=i; j++)						
  {
    double Tm = melting(raw, bestPath[j-1], bestPath[j]);
    if (Tm < minTm) minTm = Tm;
    if (Tm > maxTm) maxTm = Tm;

    double bad = getDoubleBadness(raw + bestPath[j], bestPath[j+1] - bestPath[j]);
    temp3 = "Tm:" + temp1.setNum(Tm) + "; Badness:" + temp2.setNum(bad);
    annotate((pMolElement*) layer, bestPath[j-1], bestPath[j]-bestPath[j-1], "hu:"+temp1.setNum(Tm), temp3);
  };
  
  padding->setLength(padding->getLength() - (dna->getLength()-bestPath[i]));
  dna->crop5(bestPath[i]);

  qDebug() << "got to here006";
  //annotate the layer with the primers
  for (int j=2; j<=i+1; j++)						//annotate the primers onto the layer
    if (!(j & 1))								//alternate direction if index is odd or even
      annotatePrimer((pMolElement*) layer, bestPath[j-2], bestPath[j]-bestPath[j-2], "Ffrag_"+p_dna->getName()+"_"+temp1.setNum(bestPath[j-2]), oligoComment);
    else
      annotatePrimer((pMolElement*) layer, bestPath[j], bestPath[j-2]-bestPath[j], "Rfrag_"+p_dna->getName()+"_"+temp1.setNum(bestPath[j]), oligoComment);

  qDebug() << "got to here007";
  //report the results as well as annotate them onto the layer
  interface->msg(name+"::frag>"+dna->getName()+" **complete**");
  temp3 = temp1.setNum(i)+" hybridization units"; 
  layer->addComment(temp3);   interface->msg(name+"::frag>"+temp3);
  temp3 = "Minimum Tm:" + temp1.setNum(minTm) + "; Maximum Tm:" + temp2.setNum(maxTm);
  layer->addComment(temp3);   interface->msg(name+"::frag>"+temp3);

  qDebug() << "got to here008";
  //clean up and return
  delete []raw;							//clean up space used for the sequence
  delete []path;    delete []bestPath; 				//clean up the space used for the path
  return (pMolStackObject*) dna;					//return the annotated layer
};


//--------------------------------------------------------------------------------------------------------------------
///make nice primer freely withing a sequence
double pMolPCRengine::nicePrimerFree(QString s, double p_Tm, int* i0, int* i1)
{
  //prepare for search
  int length = s.length();			// for speed
  short int* buffer = new short int[length];
  if (!convertToByte(s, buffer))
  { delete []buffer; 
    qDebug() << "failed converstion"; return -1.0;  }

  double badness = 64000.0;				//very very bad!
  double bestTm  = 100000.0;				//very very hot
  bool found = false;					//haven't found a primer matching Tm range yet

  for (int i=0; i<length-minPrimerLength; i++)
  { for (int j=minPrimerLength; j<maxPrimerLength; j++)
    { 
      if (i+j>=length) break;

      double Tm = melting(buffer,i,i+j);
      double currentBadness = getBadness(buffer+i,j);

     //if within thermal range, try and get the best one 
      if ((Tm>p_Tm-maxTmWobble) && (Tm<p_Tm+maxTmWobble)&& (currentBadness<badness))
      { found = true;
        badness = currentBadness;
        *i0 = i; *i1 = j;
      } else

      //try and return a primer at all costs, even if not within Tm range, get the best one at least
      if ((!found) && (abs(bestTm-p_Tm)>abs(Tm-p_Tm)))
      { bestTm = Tm;
        *i0 = i; *i1 = j;
      };

    };
  };

  delete []buffer;
  return badness;
};


//--------------------------------------------------------------------------------------------------------------------
///make nice primer anchored left withing a sequence
double pMolPCRengine::nicePrimerLeft(QString s, double p_Tm, int* i1)
{
  qDebug() << "niceprimerleft1" ;
  //prepare for search
  int length = s.length();			// for speed
  int tempMaxPrimerLength = maxPrimerLength;
  if (length<maxPrimerLength) tempMaxPrimerLength = length;	//stops out of range if very short sequence
  short int* buffer = new short int[length];
  if (!convertToByte(s, buffer))
  { delete []buffer;  return -1.0;  }

  double badness = 64000.0;				//very very bad!
  double bestTm  = 100000.0;				//very very hot
  bool found = false;					//haven't found a primer matching Tm range yet


  for (int j=minPrimerLength; j<tempMaxPrimerLength; j++)
  { 
    double Tm = melting(buffer,0,j);
    double currentBadness = getBadness(buffer,j);
    qDebug() << "niceprimerleft2" << currentBadness << Tm;

    //if within thermal range, try and get the best one 
    if ((Tm>p_Tm-maxTmWobble) && (Tm<p_Tm+maxTmWobble)&& (currentBadness<badness))
    { found = true;
      badness = currentBadness;
      *i1 = j;
    } else

    //try and return a primer at all costs, even if not within Tm range, get the best one at least
    if ((!found) && (abs(bestTm-p_Tm)>abs(Tm-p_Tm)))
    { bestTm = Tm;
      *i1 = j;
    };
  };
  qDebug() << "niceprimerleftz" ;
  delete []buffer;
  return badness;
};


//--------------------------------------------------------------------------------------------------------------------
///make nice primer anchored to the center of a sequence
double pMolPCRengine::nicePrimerMid(QString s, double p_Tm, int* i0, int* i1)
{
  //prepare for search
  int length = s.length();			// for speed
  int wobble = 4;				// how much we wobble backwards and forwards around the middle

  short int* buffer = new short int[length];
  if (!convertToByte(s, buffer))
  { delete []buffer;  return -1.0;  }

  double badness = -64000.0;				//very very bad!
  double bestTm  = 100000.0;				//very very hot
  bool found = false;					//haven't found a primer matching Tm range yet


  for (int j=minPrimerLength; j<maxPrimerLength; j++)
  { 
    for (int w=-wobble;w<wobble;w++)
    {
      int i = w+((length-j)/2);
      if ((i>=0) && (i+j < length))
      { 
        double Tm = melting(buffer,i,i+j);
        double currentBadness = getDoubleBadness(buffer+i,j);

        //if within thermal range, try and get the best one 
        if ((Tm>p_Tm-maxTmWobble) && (Tm<p_Tm+maxTmWobble)&& (currentBadness<badness))
        { found = true;
          badness = currentBadness;
          *i0 = i; *i1 = j;
        } else

        //try and return a primer at all costs, even if not within Tm range, get the best one at least
        if ((!found) && (abs(bestTm-p_Tm)>abs(Tm-p_Tm)))
        { bestTm = Tm;
         *i0 = i; *i1 = j;
        };
      };
    };
  };

  delete []buffer;
  return badness;
};
//--------------------------------------------------------------------------------------------------------------------
///returns the melting temperature of a DNA sequence
pMolStackObject* pMolPCRengine::melting(pMolCmd* cmd, pMolKernelInterface* interface)
{
  //if we didn't get any parameters, then send an error and return
  if (cmd==NULL) return NULL;
  if (cmd->type() != pMolCmd::LIST) 
  { interface->msg(name+"::melting - expecting parameter list");return NULL;
  };

  //interpret out paramaters, bailing if interpretation fails
  pMolObject* object = interface->interpret(cmd);
  if (object==NULL) {interface->msg(name+"::melting - something went wrong with interpreting a list"); return NULL; };
  qDebug() << "melting::ok got to here2";

  //I hate doing this but cast up to an object list
  pMolStackList* objectList = dynamic_cast <pMolStackList*>(object);

  //only one parameter - no good
  if (objectList->length()!=1) {interface->msg(name+"::melting - expected one parameter"); return NULL; };
  if (!objectList->at(0)->type(pMolStackObject::DNA)) return NULL;  
  pMolElement* dna = dynamic_cast <pMolElement*> (objectList->at(0));

  //create the new DNA and package for stack
  double Tm = melting(dna->getSequence());
  pMolStackDouble* TmStack = new pMolStackDouble(Tm);
  
  QString s;
  interface->msg(s.setNum(Tm));
  delete objectList;
  return (pMolStackObject*) TmStack;
};


//--------------------------------------------------------------------------------------------------------------------
///returns the badness of a DNA sequence
pMolStackObject* pMolPCRengine::badness(pMolCmd* cmd, pMolKernelInterface* interface)
{
  //if we didn't get any parameters, then send an error and return
  if (cmd==NULL) return NULL;
  if (cmd->type() != pMolCmd::LIST) 
  { interface->msg(name+"::melting - expecting parameter list");return NULL;
  };

  //interpret out paramaters, bailing if interpretation fails
  pMolObject* object = interface->interpret(cmd);
  if (object==NULL) {interface->msg(name+"::melting - something went wrong with interpreting a list"); return NULL; };
  qDebug() << "melting::ok got to here2";

  //I hate doing this but cast up to an object list
  pMolStackList* objectList = dynamic_cast <pMolStackList*>(object);

  //only one parameter - no good
  if (objectList->length()!=1) {interface->msg(name+"::melting - expected one parameter"); return NULL; };
  if (!objectList->at(0)->type(pMolStackObject::DNA)) return NULL;  
  pMolElement* dna = dynamic_cast <pMolElement*> (objectList->at(0));

  QString s = dna->getSequence();
  int length = s.length();			// for speed
  short int* buffer = new short int[length];
  if (!convertToByte(s, buffer))
  { interface->msg("pMolDNAengine::non-standard character in sequence, can't calculate Tm");
      delete []buffer;
      return NULL;
  }

  //create the new DNA and package for stack
  double badness = getBadness(buffer, length);
  pMolStackDouble* TmStack = new pMolStackDouble(badness);

  interface->msg(s.setNum(badness));
  delete objectList;
  delete []buffer;
  return (pMolStackObject*) TmStack;
};


//------------------------------------------------------------------------------------------------------------------
///this handles synthesis commands
pMolStackObject* pMolPCRengine::c_prime(pMolCmd* cmd, pMolKernelInterface* interface)
{
  QString error, sequence, name;

  //if we didn't get any parameters, then send an error and return
  pMolStackList* list = pMolStackList::interpretParameter(cmd, interface, (pMolObject*) this, error);
  if (list==NULL) return makeError(interface, "prime", error);

  //if there's one number parameter, cut bit off front or back depending if pos or neg
  if (!list->checkParameters(pMolObject::DNA, pMolObject::NUMBER, pMolObject::NUMBER, error))
  return makeError(interface, "prime", error);

  pMolElement* dna = dynamic_cast <pMolElement*> (list->at(0));
  pMolStackNumber* p_i0 = dynamic_cast <pMolStackNumber*> (list->at(1));
  pMolStackNumber* p_i1 = dynamic_cast <pMolStackNumber*> (list->at(2));

  int j0, j1;
  int i0 = p_i0->n;
  int i1 = p_i1->n;
  QString temp;
  qDebug() << "primer" << i0 << i1;
  if (i1>i0)
  {
    nicePrimerFree(dna->getSequence().mid(i0, i1-i0), cloningPrimerTm, &j0, &j1);
    pMolLayer* layer = new pMolLayer();
    layer->pMolParent = dna;
    qDebug() << "primer" << j0 << j1;
    annotatePrimer(layer, i0 + j0, j1,"F_prim:"+dna->name+temp.setNum(i0+j0), "made by "+name);
    return layer;
  }
  else
  {
    nicePrimerFree(dna->getSequence().mid(i1, i0-i1), cloningPrimerTm, &j0, &j1);
    pMolLayer* layer = new pMolLayer();
    layer->pMolParent = dna;
    annotatePrimer(layer, i1 + j0 + j1, -j1,"R_prim:"+dna->name+temp.setNum(i0+j0), "made by "+name);
    return layer;
  }
  return makeError(interface, "primer", "not enough space to make primer");
};

/*
//--------------------------------------------------------------------------------------------------------------------
///designs a good primer in a given sequence either anchored to the left, to the right or free
pMolStackObject* pMolPCRengine::c_prime(pMolCmd* cmd, pMolKernelInterface* interface)
{
  QString error;

  //if we didn't get any parameters, then send an error and return
  pMolStackList* list = pMolStackList::interpretParameter(cmd, interface,error);
  if (list==NULL) return makeError(interface, "sequence", error);

  //if there's one number parameter, cut bit off front or back depending if pos or neg
  if (!list->checkParameters(pMolObject::DNA,  error))
       return makeError(interface, "not enough paramaters", error);

  int primerLength;
  pMolElement* dna = dynamic_cast <pMolElement*> (list->at(0));
  pMolStackString* string = dynamic_cast <pMolStackString*> (list-at(1));
  QString command = string->string;
  float badness = 0.0;
  if (command == "left")
      badness = nicePrimerLeft(dna->getSequence(),sequencingPrimerTm, &primerLength);
  elseif (command == "right")
      badness = nicePrimerRight(dna->getSequence(), sequencingPrimerTm, &primerLength);
    if (badness<0.0) return makeError(interface, "primer", "::Tm calculation failed");

*/

//--------------------------------------------------------------------------------------------------------------------
///looks for a sequence and parameters - sequencingPrimerTm, sequencingPadding, sequencingLength and makes nice primers 
///for overlapping sequencing, how to output this nicely??
pMolStackObject* pMolPCRengine::sequence(pMolCmd* cmd, pMolKernelInterface* interface)
{
  QString error;

  //if we didn't get any parameters, then send an error and return
  pMolStackList* list = pMolStackList::interpretParameter(cmd, interface,error);
  if (list==NULL) return makeError(interface, "sequence", error);

  //if there's one number parameter, cut bit off front or back depending if pos or neg
  if (list->checkParameters(pMolObject::DNA, pMolObject::NUMBER, error))
  {
    pMolElement* dna = dynamic_cast <pMolElement*> (list->at(0));
    pMolStackNumber* overSequence = dynamic_cast <pMolStackNumber*> (list->at(1));

  //calulate distances from primers
    QString s = dna->getSequence();
    int length = s.length();			// for speed
    int passes = overSequence->n;
    int delta = 2*length/((2*passes)+1);
    short int* buffer = new short int[length];

    pMolLayer* layer = new pMolLayer();
    layer->pMolParent = dna;
    //prepare sequence for Tm calculations for forward sequencing

    QString temp, oligoComment, oligoName;
    oligoName = "seq-"+dna->name+":";
    oligoComment = "Oligo for sequencing\nTemplate:"+dna->getHierarchialName()+"\nCreated by pMolPCRengine:"+name+"\n";


    int i0,i1;
    for (int i=0; i<length; i+=delta)
    {  double badness = nicePrimerFree(s.mid(i,sequenceWobble+minPrimerLength),sequencingPrimerTm, &i0, &i1);	
       if (badness<0.0) 
         return makeError(interface, "sequence", "::sequence - Tm calculation failed in "+s.mid(0,sequenceWobble));
       i0+=i; 
       temp=temp.setNum(i0);
       annotatePrimer((pMolElement*) layer, i0, i1, "F-"+oligoName+temp, oligoComment+"Forward direction starts at:"+temp);
       interface->msg(badness);
       interface->msg(i0);
       interface->msg(i1);
       interface->msg(s.mid(i0,i1));
    };

  //prepare sequence for Tm calculations for reverse sequencing
  s = dna->getComplement();
  for (int i=0; i<length; i+=delta)
  {  double badness = nicePrimerFree(s.mid(i,sequenceWobble+minPrimerLength),sequencingPrimerTm, &i0, &i1);	
     if (badness<0.0) 
       return makeError(interface, "sequence", "::sequence - Tm calculation failed in "+s.mid(0,sequenceWobble));
     i0+=i; 
     temp=temp.setNum(length-i0);
     annotatePrimer((pMolElement*) layer, length-i0, -i1, "R-"+oligoName+temp, oligoComment+"Reverse direction starts at:"+temp);
     interface->msg(badness);
     interface->msg(length-i0);
     interface->msg(i1);
     interface->msg(s.mid(i0,i1));
  };

  delete []buffer;
  return (pMolStackObject*) layer;

  };
  return makeError(interface, "add", error);

};


//--------------------------------------------------------------------------------------------------------------------
///amplifies sections of a region for fragmentation display
///This needs refinement to accept 5' and 3' tails
pMolStackObject* pMolPCRengine::display(pMolCmd* cmd, pMolKernelInterface* interface)
{
  QString error;

  //if we didn't get any parameters, then send an error and return
  pMolStackList* list = pMolStackList::interpretParameter(cmd, interface,error);
  if (list==NULL) return makeError(interface, "sequence", error);

  //if there's one number parameter, cut bit off front or back depending if pos or neg
  if (list->checkParameters(pMolObject::DNA, pMolObject::NUMBER, pMolObject::NUMBER,  error))
  {
    QString tail5;
    QString tail3;

    pMolElement* dna = dynamic_cast <pMolElement*> (list->at(0));
    pMolStackNumber* span = dynamic_cast <pMolStackNumber*> (list->at(1));
    pMolStackNumber* skip = dynamic_cast <pMolStackNumber*> (list->at(2));

    if (list->checkParameters(pMolObject::DNA, pMolObject::NUMBER, pMolObject::NUMBER, pMolObject::DNA, error))
    { pMolElement *tail = dynamic_cast <pMolElement*> (list->at(3));
      tail5 = tail->getSequence();
    }
    else tail5 = "";

    if (list->checkParameters(pMolObject::DNA, pMolObject::NUMBER, pMolObject::NUMBER, pMolObject::DNA, pMolObject::DNA, error))
    { pMolElement *tail = dynamic_cast <pMolElement*> (list->at(4));
      tail3 = tail->getSequence();
    }
    else tail3 = "";

  //calulate distances from primers
    QString s = dna->getSequence();
    QString c = dna->getComplement();
    int length = s.length();			// for speed

    pMolLayer* layer = new pMolLayer();
    layer->pMolParent = dna;
    //prepare sequence for Tm calculations for forward sequencing

    QString temp, oligoComment, oligoName;
    oligoName = "display-"+dna->name+":";
    oligoComment = "Oligo for fragmentation display\nTemplate:"+dna->getHierarchialName()+"\nCreated by pMolPCRengine:"+name+"\n";

    int primerLength;
    double badness;
    int count = 1;

    for (int i=0; i+span->n<length; i+=skip->n)
    {
       QString dname;
       pMolPrimer* primer;
       dname = "DisplayFrag:"+temp.setNum(count++);

       QString ss = s.mid(i, span->n);
       QString sc = c.mid(length-i-span->n, span->n);
       interface->msg(ss);
       interface->msg(sc);

       //annotate this segment
       annotate((pMolElement*) layer, i, span->n, dname, "Frag");

       //forward Primer
       badness = nicePrimerLeft(ss,sequencingPrimerTm, &primerLength);
       if (badness<0.0)
         return makeError(interface, "sequence", "::sequence - Tm calculation failed in "+s.mid(0,sequenceWobble));
       temp=temp.setNum(primerLength);
       primer = annotatePrimer((pMolElement*) layer, i, primerLength, "F-"+oligoName+":"+dname, oligoComment+"Forward direction starts at:"+temp);
       primer->setTail(tail5);
       interface->msg(badness);
       interface->msg(i);
       interface->msg(primerLength);

       //reverse primer
       badness = nicePrimerLeft(sc, sequencingPrimerTm, &primerLength);
       if (badness<0.0)
         return makeError(interface, "sequence", "::sequence - Tm calculation failed in "+s.mid(0,sequenceWobble));
       temp=temp.setNum(primerLength);
       primer=annotatePrimer((pMolElement*) layer, i+span->n, -primerLength, "R-"+oligoName+":"+dname, oligoComment+"Reverse direction starts at:"+temp);
       primer->setTail(tail3);
       interface->msg(badness);
       interface->msg(i);
       interface->msg(primerLength);

       interface->msg("----");
    };

    return (pMolStackObject*) layer;

  };
  return makeError(interface, "add", error);

};





//--------------------------------------------------------------------------------------------------------------------
///This member for convenience sticks in a primer element
pMolPrimer* pMolPCRengine::annotatePrimer(pMolElement* dna, int index, int length, const QString &name, const QString &comment)
{
   pMolPrimer* primer = new pMolPrimer();
   primer->name = name;
   primer->addComment(comment);
   primer->setIndex(index);
   primer->setLength(length);
   dna->append((pMolElement*) primer);
   return primer;
};

//--------------------------------------------------------------------------------------------------------------------
///This member for convenience sticks in an annotation element
pMolAnnotation* pMolPCRengine::annotate(pMolElement* dna, int index, int length, const QString &name, const QString &comment)
{
   pMolAnnotation* annotation = new pMolAnnotation();
   annotation->name = name;
   annotation->addComment(comment);
   annotation->setIndex(index);
   annotation->setLength(length);
   dna->append((pMolElement*) annotation);
   return annotation;
};

///this does a quite complex and fuzy job of trying to make a good PCR fusion of a number of different segments of DNA
///that are passed to it. It accepts restriction endonucleases, and DNA segments. It decides which segments are short
///enough to just be oligos, and decides which are to be PCRed. At the sites of joining it creates annealing primers of
///correct melting temperature and also fusion regions of correct melting temperature. If dumps out the fusion DNA 
///annotated with primers-> and the annealing primers. I want it do dump out a nice visual representation too! Complex.
pMolStackObject* pMolPCRengine::fuse(pMolCmd* cmd, pMolKernelInterface* interface)
{
  //if we didn't get any parameters, then send an error and return
  if (cmd==NULL) return NULL;
  if (cmd->type() != pMolCmd::LIST) 
  { interface->msg(name+"::fuse - expecting parameter list");return NULL;
  };
  qDebug() << "fuse0";
  //interpret out paramaters, bailing if interpretation fails
  pMolObject* object = interface->interpret(cmd);
  if (object==NULL) {interface->msg(name+"::fuse - something went wrong with interpreting a list"); return NULL; };
  qDebug() << "fuse1";
  //I hate doing this but cast up to an object list
  pMolStackList* listObject = dynamic_cast <pMolStackList*>(object);
  qDebug() << "fuse2";
  pMolDNA* dna = new pMolDNA();
  dna->circular = false;
  dna->overhangAtEnd = 0;
  dna->overhangAtStart = 0;
  //dna->pMolParent = NULL;
  dna->sequence="";
  pMolLayer* layer = new pMolLayer();
  layer->pMolParent = dna;
  layer->name = "construction_layer";
  layer->addComment("pMol::fuse");
  dna->append((pMolElement*) layer);

  pMolAnnotation* lastBlock;
 pMolElement* newdna;
  int i=0;
  int i1,i0;
  int fpi0[64], fpi1[64], rpi0[64], rpi1[64];
  QString padding;
  double badness;
  bool explicit5end = false;
  bool explicit3end = false;
  qDebug() << "fuse3";

  QString rName = "";
  QString fName = "";
  QString rComment = "";
  QString fComment = "";
  QString rNameName = "";
  QString primerFComment = "oligo for DNA fusion\nForward direction\nGenerated by pMolEngine:"+name+"\n";
  QString primerRComment = "oligo for DNA fusion\nReverse direction\nGenerated by pMolEngine:"+name+"\n";


  foreach (pMolStackObject* param, listObject->list)
  {  qDebug() << "fuse4";
  qDebug() << "fuse5" << param->name;
  if (param->type(pMolStackObject::RAW) && (i==0)) explicit5end = true;

  //just a restriction endonuclease sequence - stick it on!
    if (param->type(pMolStackObject::RESTRICTIONENDONUCLEASE))
    {  pMolRestrictionEndonuclease* ren = dynamic_cast <pMolRestrictionEndonuclease*>(param);
       padding+=ren->getSequence();
       dna->extend3(ren->name, "added by PCRengine::fuse",ren->getSequence().toUpper());
       explicit5end= true;
       explicit3end = true;
       fName=fName+"."+param->name;
       rName=param->name+"."+rName;
       fComment=fComment+"Tail: restriction endonuclease: "+param->name+"\n";
       rComment="Tail: restriction endonuclease: "+param->name+rComment+"\n";
    } else
  //some DNA sequence
    if (param->type(pMolStackObject::DNA))
    {  newdna = dynamic_cast <pMolElement*>(param);
       qDebug() << "fuse6" << newdna->name;
  //if nice and short, can just stick it on, but if at the start and not explicit 5' end 
  //implies that user would like some padding, so don't synthesize
       if (newdna->getLength()<maxSyntheticLength && (!((i==0) && (!explicit5end))) )
       { qDebug() << "fuse7a" << newdna->name;
         padding+=newdna->getSequence();
         dna->extend3(newdna->name, "added by PCRengine::fuse",newdna->getSequence());
         explicit5end = true;
         explicit3end = true;
         fName=fName+"."+newdna->name;
         rName=newdna->name+"."+rName;
         fComment="Tail: "+newdna->getHierarchialName()+fComment+"\n";
         rComment=rComment+"Tail: "+newdna->getHierarchialName()+"\n";
       }
  //otherwise we need to design primers
       else
       { 
         if ((i==0) && (!explicit5end))
         {
           badness = nicePrimerFree(newdna->getSequence(-cloningPrimerPadding, cloningPrimerPadding),cloningPrimerTm,&i0, &i1);
           lastBlock = dna->extend3(newdna->name, "added by PCRengine::fuse", 
            newdna->getSequence(i0-cloningPrimerPadding, cloningPrimerPadding-i0+newdna->getLength()));
           layer->annotate("padding","padding added by PCRengine:fuse",0,cloningPrimerPadding-i0);
           fpi0[0]=0;
           fpi1[0]=i1;
         }
         else
         {
           i0 = dna->getLength();
           lastBlock = dna->extend3(newdna->name, "added by PCRengine::fuse",newdna->getSequence());
           qDebug() << "fuse7bm2" << newdna->name;
           badness = nicePrimerLeft(newdna->getSequence(),cloningPrimerTm, &i1);
           fpi0[i] = i0; fpi1[i] = +i1;
         }
         i0 = dna->getLength();
         badness = nicePrimerLeft(newdna->getComplement(), cloningPrimerTm, &i1);
         rpi0[i] = i0; rpi1[i] = -i1;
         explicit3end = false;			//whatever is last a block or padding determines this!
  //this is the first primer pair so just output the forward one (no padding info on reverse so have to wait
         if (i==0) 
         { 
  qDebug() << "fuse7ba" << newdna->name;
           fpi1[0] += fpi0[0];			//add padding to length
           fpi0[0] = 0;				//first primer starts at start!
           fName="F-fuse:"+fName.right(fName.length()-1)+"#"+newdna->name;
           fComment = primerFComment+"template: "+newdna->getHierarchialName()+"\n"+fComment;
           annotatePrimer(layer, fpi0[0], fpi1[0], fName, fComment);
           fName = newdna->name; 
           fComment = "Tail: "+newdna->getHierarchialName()+"\n";
           rName = ""; rComment = "template: "+newdna->getHierarchialName()+"\n";
           rNameName = newdna->name;
          //dna->annotate("F-primer","fuse!",0,fpi1[0]);
           interface->msg(dna->getSequence(fpi0[0],fpi1[0]));
         }
  //second pair of primers or onwards, we have paddin info so can go backwards to finish earlier reverse primers
         else
         { 
 qDebug() << "fuse7bb" << newdna->name;
           int flank;
           int mid = (rpi0[i-1]+fpi0[i])/2;
           qDebug() << "HERE" << mid << rpi0[i-1] << fpi0[i];
           if ((i0-mid)<mid) flank = i0-mid; else flank = mid;
           badness = nicePrimerMid(dna->getSequence().mid(mid-flank, flank*2), cloningPrimerTm, &i0, &i1);

           int delta;

           delta = mid -flank + i0 + i1 - rpi0[i-1];
           qDebug() << "HERE" << i0 << i1 << mid << delta;
           rpi0[i-1]+= delta;
           rpi1[i-1]-= delta;

           delta = fpi0[i] - ( mid - flank + i0);
           fpi0[i]   = mid -flank + i0;
           fpi1[i]  += delta;

           fName="F-fuse:"+fName.left(fName.length())+"#"+newdna->name;
           fComment = primerFComment+"template: "+newdna->getHierarchialName()+fComment;
           rName="R-fuse:"+newdna->name+"."+rName.left(rName.length())+"#"+rNameName;
           rComment=rComment+"Tail: dna sequence: "+newdna->getHierarchialName()+"\n";
           rComment = primerRComment+"template: "+rComment;

           annotatePrimer(layer, rpi0[i-1],rpi1[i-1], rName, rComment);
            annotatePrimer(layer, fpi0[i],fpi1[i], fName, rComment);
           interface->msg(dna->getSequence(rpi0[i-1],rpi1[i-1]));
           interface->msg("  ");
           interface->msg(dna->getSequence(fpi0[i],fpi1[i]));

           fName = newdna->name; 
           fComment = "Tail: "+newdna->getHierarchialName()+"\n";
           rName = ""; 
           rComment = "template: "+newdna->getHierarchialName()+"/n";
           rNameName = newdna->name;

         }
         i++;
         padding = "";
       }
     }
  //don't know what the object in the parameter is!
     else
     { interface->msg("oh boollocks don't know what this is: "+param->name);
       delete dna;
       return NULL;
     };
  }
  if (explicit3end)
  {
    rpi0[i-1] += padding.length();
    rpi1[i-1] -= padding.length();
  }
  else
  //we have to change the reverse primers!
  { int newdna_length = newdna->getLength();
    int builddna_length = dna->getLength();
    QString primerPadding = newdna->getSequence(newdna_length + cloningPrimerPadding, -cloningPrimerPadding);
    badness = nicePrimerFree(primerPadding,cloningPrimerTm,&i0, &i1);
    i0 = cloningPrimerPadding - i0;
    qDebug() << primerPadding << i0 << i1;
    layer->annotate("padding","padding added by PCRengine:fuse",builddna_length,i0);
    dna->sequence+=newdna->getSequence(newdna_length,i0);	//this is bad!
    dna->getLength();
    lastBlock->setLength(lastBlock->getLength()+i0);
    rpi0[i-1]=builddna_length+i0;
    rpi1[i-1]=-i1;
  }
  rName="R-fuse:"+rName.left(rName.length())+"#"+rNameName;
  rComment = primerRComment+"template: "+rComment;
  annotatePrimer(layer, rpi0[i-1],rpi1[i-1], rName, rComment);
  interface->msg(dna->getSequence(rpi0[i-1],rpi1[i-1]));
  return dna;
}


//---------------------------------------------------------------------------------------------------------------------
///executes commands referred to this object
pMolStackObject* pMolPCRengine::exec(pMolCmd* cmd, pMolKernelInterface* interface)
{
  //no paramaters, return this as object
  if (cmd==NULL) return (pMolStackObject*) this;				//no paramaters, return me

  //if no command but not empty, something is wrong (needs to be empty or have subcommand) so return with error
  if (cmd->type() != pMolCmd::NAME)
  { interface->msg("pMolecule::exec - empty field or subcommand expected");
    return NULL;
  };
  //first let's look in our own little internal namespace
  pMolCmdName* cmdName = dynamic_cast<pMolCmdName*>(cmd);					//safe to assume this is a pMolCmdName

  if (cmdName->name=="melting") 	return melting(cmdName->child, interface);	else
  if (cmdName->name=="badness") 	return badness(cmdName->child, interface);    	else
  if (cmdName->name=="sequence") 	return sequence(cmdName->child, interface);	else
  if (cmdName->name=="fuse") 		return fuse(cmdName->child, interface);         else
  if (cmdName->name=="frag")		return frag(cmdName->child, interface);         else
  if (cmdName->name=="prime")		return c_prime(cmdName->child, interface);	else
  if (cmdName->name=="display")		return display(cmdName->child, interface);	else
  /*if (cmdName->name=="prime")		return c_primer(cmdName->child, interface);	else */

  interface->msg(name+"::command not understood");
  return NULL;
};
