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

#include "pMolGeneSplicerTable.h"

//---------------------------------------------------------------------------------------------------
///Constructors and destructors
pMolGeneSplicerTable::pMolGeneSplicerTable(int p_length, int p_alphabetSize, int p_markovLen, 
  pMolXMLbase* p_pMolXMLbaseParent, const QString &p_name) : pMolStackObject(p_pMolXMLbaseParent)
{
  name = p_name;

  length = p_length;
  alphabetSize = p_alphabetSize;
  markovLen = p_markovLen;

  Positive_Table = new double[length * alphabetSize * markovLen];
  Negative_Table = new double[length * alphabetSize * markovLen];

  baseNumberSet = false;
  baseSet = false;
  markovSet = false;
  valueSet = false;

  number = 0;
};

pMolGeneSplicerTable::~pMolGeneSplicerTable()
{
  delete []Positive_Table;
  delete []Negative_Table;
};

//---------------------------------------------------------------------------------------------------
///Loads up the XML data
pMolXMLbase* pMolGeneSplicerTable::XMLclose(const QString &tag, const QString &data, QString &error)
{
  //qDebug() << name << tag << data;
  //if (name=="pMolExonAcceptorTable") qDebug() << tag << data;

  if (tag=="pMolGeneSplicerAcceptorTable")     	return pMolXMLbaseParent; 
  if (tag=="pMolGeneSplicerDonorTable")		return pMolXMLbaseParent;
  if (tag=="pMolExonDonorTable")		return pMolXMLbaseParent;
  if (tag=="pMolIntronAcceptorTable")		return pMolXMLbaseParent;
  if (tag=="pMolExonAcceptorTable")		return pMolXMLbaseParent;
  if (tag=="pMolIntronAcceptorTable")		return pMolXMLbaseParent;


  if (tag=="pMolGeneSplicerPositiveTableElement")
  {
    if (baseNumberSet && baseSet && markovSet && valueSet)
      if ((baseNumber<length) && (base < alphabetSize) && (markov < markovLen))
        setPositiveTableValue(baseNumber, base, markov, value);
    valueSet = false;		baseNumberSet = false;
    baseSet = false;     	markovSet = false;
    return (pMolXMLbase*) this;
  }

  if (tag=="pMolGeneSplicerNegativeTableElement")
  {
    if (baseNumberSet && baseSet && markovSet && valueSet)
      if ((baseNumber<length) && (base < alphabetSize) && (markov < markovLen))
        setNegativeTableValue(baseNumber, base, markov, value);
    valueSet = false;		baseNumberSet = false;
    baseSet = false;    	markovSet = false;
    return (pMolXMLbase*) this;
  }

  else if (tag=="value")              valueSet = XMLdouble(&value, data, error);
  else if (tag=="baseNumber")         baseNumberSet = XMLnumeric(&baseNumber, data, error);
  else if (tag=="base")               baseSet = XMLnumeric(&base, data, error);
  else if (tag=="markov")		markovSet = XMLnumeric(&markov, data, error);

  else if (tag=="number")		XMLnumeric(&number, data, error);

  else return pMolXMLbase::XMLclose(tag, data,error);
  return (pMolXMLbase*) this; 
};

//---------------------------------------------------------------------------------------------------
///Outputs tree structure
void pMolGeneSplicerTable::treeOut(QTreeWidgetItem* tree)
{
  bool ok;
  int i,j,k;
  QString tempi, tempj, tempk;
  QTreeWidgetItem* branch;

  if (number==0)
     branch = treeItem(tree, name);
   else
     branch = treeItem(tree, name+"["+tempi.setNum(number)+"]");
 
  QTreeWidgetItem* branchPos = treeItem(branch, "PositiveTable","[baseNumber][base][markov]");
  for (i=0; i<length; i++)
    for (j=0; j<alphabetSize; j++)
      for (k=0; k<markovLen; k++)
      { QString a =
        "[baseNumber:"+tempi.setNum(i)+"][base:"+tempj.setNum(j)+"][markov:"+tempk.setNum(k)+"]";
        treeItem(branchPos, a, getPositiveTableValue(i, j, k, &ok));
      };

  QTreeWidgetItem* branchNeg = treeItem(branch, "NegativeTable","[baseNumber][base][markov]");
  for (i=0; i<length; i++)
    for (j=0; j<alphabetSize; j++)
      for (k=0; k<markovLen; k++)
      { QString a =
        "[baseNumber:"+tempi.setNum(i)+"][base:"+tempj.setNum(j)+"][markov:"+tempk.setNum(k)+"]";
        treeItem(branchNeg, a, getNegativeTableValue(i, j, k, &ok));
      };
};

//---------------------------------------------------------------------------------------------------
///Gets a result from positive table
double pMolGeneSplicerTable::getPositiveTableValue(int baseNumber, int base, int markov, bool *ok)
{
  if ((baseNumber < length) && (base < alphabetSize) && (markov < markovLen) && (Negative_Table!=NULL))
  { *ok = true;
    return Positive_Table[baseNumber*alphabetSize*markovLen + base*markovLen + markov];
  }
  else
  { *ok = false;
    return 0.0;
  };
};

//---------------------------------------------------------------------------------------------------
///Gets a result from negative table
double pMolGeneSplicerTable::getNegativeTableValue(int baseNumber, int base, int markov, bool *ok)
{
  if ((baseNumber < length) && (base < alphabetSize) && (markov < markovLen) && (Negative_Table!=NULL))
  { *ok = true;
    return Negative_Table[baseNumber*alphabetSize*markovLen + base*markovLen + markov];
  }
  else
  { *ok = false;
    return 0.0;
  };
};

//---------------------------------------------------------------------------------------------------
///Sets a value in a positive table
void pMolGeneSplicerTable::setPositiveTableValue(int baseNumber, int base, int markov, double value)
{
  if ((baseNumber < length) && (base < alphabetSize) && (markov < markovLen) && (Negative_Table!=NULL))
    Positive_Table[baseNumber*alphabetSize*markovLen + base*markovLen + markov] = value;
};

//---------------------------------------------------------------------------------------------------
///Sets a value in negative table
void pMolGeneSplicerTable::setNegativeTableValue(int baseNumber, int base, int markov, double value)
{
  if ((baseNumber < length) && (base < alphabetSize) && (markov < markovLen) && (Negative_Table!=NULL))
    Negative_Table[baseNumber*alphabetSize*markovLen + base*markovLen + markov] = value;
};
