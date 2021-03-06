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
#include "pMolCodon.h"
#include <QtGui>
#include <math.h>

//--------------------------------------------------------------------------------------------------
///constructors and destructors
pMolCodon::pMolCodon() : pMolXMLbase(NULL) 
{ 
  upCodon = NULL;
  dnCodon = NULL;
  upTranslation = NULL;
  dnTranslation = NULL;
  eqTranslation = NULL;
  rootTranslation = this;
  start = false; stop = false;;
};

pMolCodon::pMolCodon(pMolXMLbase *p_pMolXMLbaseParent) : pMolXMLbase(p_pMolXMLbaseParent)
{
  upCodon = NULL;
  dnCodon = NULL;
  upTranslation = NULL;
  dnTranslation = NULL;
  eqTranslation = NULL;
  rootTranslation = this;
  start = false; stop = false;
};

pMolCodon::~pMolCodon() { };

//--------------------------------------------------------------------------------------------------
///get opening data from XML stream
pMolXMLbase* pMolCodon::XMLopen(const QString &tag, QString &error)
{
  if (tag=="start") {start=true; return (pMolXMLbase*) this;}	//may change this to true or false rather than present
  if (tag=="stop")  {stop=true;  return (pMolXMLbase*) this;}

  return pMolXMLbase::XMLopen(tag, error);
};

//--------------------------------------------------------------------------------------------------
///get data from XML stream
pMolXMLbase* pMolCodon::XMLclose(const QString &tag, const QString &data, QString &error)
{
  if (tag=="codon") { return pMolXMLbaseParent; }
  else if (tag== "code")    		code = data;  				//need error handling here if returns false
  else if (tag=="translation") 	translation = data; 
  else if (tag=="frequency")       	XMLnumeric(&frequency, data, error); 
  else return pMolXMLbase::XMLclose(tag, data,error);
  return (pMolXMLbase*) this;
};

//--------------------------------------------------------------------------------------------------
///XML tree out
void pMolCodon::treeOut(QTreeWidgetItem *tree)
{
  QTreeWidgetItem* branch = treeItem(tree,"codon");
  treeItem(branch,"code",code);
  treeItem(branch,"translation",translation);
  treeItem(branch,"frequency",frequency);
  treeItem(branch,"codeCount", codeCount);
  treeItem(branch,"codeFrequency", codeFrequency);
  treeItem(branch,"codeBestFrequency", codeBestFrequency);
  treeItem(branch,"usagePercent", usagePercent);
  treeItem(branch,"fractionIndex", (int) fractionIndex);
  treeItem(branch,"fraction", fraction);
  treeItem(branch,"badness", badness);
  treeItem(branch,"start",start);
  treeItem(branch,"stop",stop);
};

//--------------------------------------------------------------------------------------------------
///link up codons with code for fast binary tree search
void pMolCodon::linkCodon(pMolCodon* newCodon)
{
  if (newCodon->code < code) if (upCodon == NULL) upCodon = newCodon; else upCodon->linkCodon(newCodon);
  if (newCodon->code > code) if (dnCodon == NULL) dnCodon = newCodon; else dnCodon->linkCodon(newCodon);
}

double pMolCodon::calculateBadnessExp(int frequency, int codeFrequency, int codeBestFrequency, int codeCount)
{
  double me = (double)frequency / (double)codeFrequency;
  double best = (double)codeBestFrequency / (double)codeFrequency;
  double badness = (best/me);
  return pow(badness,4)-1.0;
};


//--------------------------------------------------------------------------------------------------
///Some sticky tape here until we stop using Qstring to represent sequences 
short int pMolCodon::getCode(QChar c)
{
  c = c.toLower();
  if (c=='a') return 1;
  if (c=='t') return 2;
  if (c=='c') return 4;
  if (c=='g') return 8;
  return 0;
};

//--------------------------------------------------------------------------------------------------
///calculates sum of frequency of similar translations and penalty score during codon optimization
void pMolCodon::setCodonStats(int* sum, int* count, int* best)
{
  (*count) ++;
  (*sum) += frequency;
  if ((*best) < frequency) *best=frequency;
  if (eqTranslation!=NULL) eqTranslation->setCodonStats(sum, count, best);
  codeCount = *count;
  codeFrequency = *sum;
  codeBestFrequency = *best;
  usagePercent = round(100*((double)frequency)/((double)codeFrequency)); 
  badness = calculateBadnessExp(frequency, codeFrequency, codeBestFrequency, codeCount);
  fraction = ((double)frequency) / ((double) codeBestFrequency);
  fractionIndex = (unsigned int) round(63.0 * fraction); 
  coding[0] = getCode(code.at(0));
  coding[1] = getCode(code.at(1));
  coding[2] = getCode(code.at(2));
};

//--------------------------------------------------------------------------------------------------
///Link up the codon with similar codes at the end of this branch
void pMolCodon::linkSimilar(pMolCodon* p_rootTranslation)
{
  rootTranslation = p_rootTranslation;
};

//--------------------------------------------------------------------------------------------------
///Link up codons with similar codes
void pMolCodon::linkSimilar(pMolCodon* p_rootTranslation, pMolCodon* newCodon)
{
  if (eqTranslation == NULL) 
  { 
    eqTranslation = newCodon;
    rootTranslation = p_rootTranslation;
    newCodon->linkSimilar(p_rootTranslation);
  }
  else eqTranslation->linkSimilar(p_rootTranslation, newCodon);
};

//--------------------------------------------------------------------------------------------------
///link up codons translations for a fast trinary tree search
void pMolCodon::linkTranslation(pMolCodon* newCodon)
{
  if (newCodon->translation < translation) 
  { if (upTranslation == NULL) upTranslation = newCodon; else  upTranslation->linkTranslation(newCodon);
  }
  else if (newCodon->translation > translation) 
  { if (dnTranslation == NULL) dnTranslation = newCodon; else dnTranslation->linkTranslation(newCodon); 
  }
  else linkSimilar(this, newCodon); 

  int sum=0; int count=0; int best=0;
  setCodonStats(&sum, &count, &best);
}; 

//--------------------------------------------------------------------------------------------------
///search the binary tree for a translation
QString pMolCodon::searchCode(const QString &p_code, bool* p_start, bool* p_stop)
{
  if (p_code < code) if (upCodon == NULL) return NULL; else return upCodon->searchCode(p_code,p_start,p_stop);
  if (p_code > code) if (dnCodon == NULL) return NULL; else return dnCodon->searchCode(p_code,p_start,p_stop);
  *p_start = start; *p_stop = stop; return translation;			//must be equal
}

pMolCodon* pMolCodon::getCodon(const QString &p_code)
{
  if (p_code < code) if (upCodon == NULL) return NULL; else return upCodon->getCodon(p_code);
  if (p_code > code) if (dnCodon == NULL) return NULL; else return dnCodon->getCodon(p_code);
  return this;			//must be equal
}

//--------------------------------------------------------------------------------------------------
///search the trinary tree for a codon (Reverse translate)
pMolCodon* pMolCodon::searchTranslation(const QString &p_translation)
{
  if (p_translation<translation) if (upTranslation==NULL) return NULL; else return upTranslation->searchTranslation(p_translation);
  if (p_translation>translation) if (dnTranslation==NULL) return NULL; else return dnTranslation->searchTranslation(p_translation);
  return this;
}

//--------------------------------------------------------------------------------------------------
///are there any more codons coding for the same translation?
pMolCodon* pMolCodon::similarTranslation()
{
  return eqTranslation;
};

//--------------------------------------------------------------------------------------------------
void pMolCodon::setBytes(short int* buffer, int i)
{
 *(buffer + i + 0) = coding[0];
 *(buffer + i + 1) = coding[1];
 *(buffer + i + 2) = coding[2];
};

pMolCodon* pMolCodon::flip(bool* ok)
{
  if (eqTranslation!=NULL) 
  { *ok = true;
    return eqTranslation; 
  }
  else
  { *ok = false;
    return rootTranslation;
  };
};

pMolCodon* pMolCodon::reset()
{
  return rootTranslation;
};
