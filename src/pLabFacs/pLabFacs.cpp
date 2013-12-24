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


#include "./pLabFacs/pLabFacs.h"

pLabFacs::pLabFacs()
{
  buffer = new quint16[64];
  qDebug() << "pLabFacs::constructor";
  qDebug() << buffer;
}

int pLabFacs::getTotalEvents()
{
  return 300;
};

int pLabFacs::getParameters()
{
  return 3;
};

quint16* pLabFacs::getEvent(int n)
{
  buffer[0] = (quint16) (rand() & 0xffff);
  buffer[1] = (quint16) (rand() & 0xffff);
  buffer[2] = (quint16) (rand() & 0xffff);
  return buffer;
};

int pLabFacs::getChannelMax(int n)
{
  return 0x10000;
};


/*
pLabFacs::pLabFacs(pMolInterface* p_interface, const QString &p_name)
{
  name = p_name;
  interface = p_interface;

  getHeaderData();
};

//-------------------------------------------------
///throws a pMolError, adds some detail to it
void pLabFacs::error(const QString &member, const QString &errorText)
{
  throw pMolError("pMolFACS["+name+"]::"+member+":"+errorText);
};


//------------------------------------------------
///free up any memory used to cache
void pLabFacs::shrink()
{
  //free up the memory and mark as freed
  delete buffer[];
  buffer = NULL;
};

//------------------------------------------------
///load the Data as integer 
void pLabFacs::loadDataInt(File)
{
  for (int n = 0; n < );
};


//------------------------------------------------
///loads the data
quint16* pLabFacs::getData(int n)
{
  //is this data in our memory cache? quint16
  if ((buffer!=NULL) && (n>bufferIndex) && (n<bufferIndex+bufferLength)) 
    return &(buffer + n*PAR);			//return address of data

  //allocate memory space if not allocated
  if (buffer==NULL) 
    buffer = new quint16[bufferLength];

  //make sure memory allocation has been ok 
  if (buffer==NULL)
    throwError("getData","memory allocation failed");

  //load the data into 16 bit unsigned integer buffer
  switch (MODE)
  {
    case pLabFacs::MODE_I : {loadDataInteger(n, bufferSize);  break;}
    case pLabFacs::MODE_F : {loadDataFloat(n, bufferSize); break; }
    case pLabFacs::MODE_D : {loadDataDouble(n, bufferSize); break; }
    case pLabFacs::MODE_A : {loadDataAscii(); break;  };
    default : throwError("getData","unkown data time");
  };
};


//-------------------------------------------------
///converts a number from text in the HEADER. Needs to remove '***' 
///throws an error if number mangled and is criticial
int pLabFacs::getHeaderNumber(const QString &p_HEADER, int i0, int i1, bool criticial)
{
  //remove any '*****;
  QString HEADER = p_HEADER.mid(i0, i1).replace("*", "");

  //try to convert to number
  bool ok;
  int number = pHeader.setNum(ok);
  if (ok) return number; 		//if conversion ok, return the number
  else if (!critical) return 0;		//if not critical return 0, otherwise throw an error
  else throwError("getHeaderNumber","could not convert HEADER: "+HEADER+" to integer");
  return 0;
};

//-------------------------------------------------
void pLabFacs::getHeaderData()
{
  //open the file 

  //load the HEADER 
  QByteArray array();
  array.resize(64);


  //check to see that the first 6 bytes read 'FCS3.0'

  //load up the offset data 
  TEXT_start = getHeaderNumber(HEADER, 10, 17, true);
  TEXT_end   = getHeaderNumber(HEADER, 18, 25, true);
  DATA_start = getHeaderNumber(HEADER, 26, 33, true);
  DATA_end   = getHeaderNumber(HEADER, 23, 41, true);
  ANALYSIS_start = getHeaderNumber(HEADER, 42, 49, false);
  ANALYSIS_end   = getHeaderNumber(HEADER, 50, 57, false);
  OTHER_start = getHeaderNumber(HEADER, 58, 63, false);

  //catch some crazy sizes
  if (TEXT_end-TEXT_start > crazyTextSegmentSize)
    throwError("getHeaderData", "TEXT SEGMENT is a crazy size");

  //load the TEXT SEGMENT 
  array.resize(TEXT_end-TEXT_start);			//should check can assign memory 
  
  //start plucking out the required keywords`
  BEGINANALYSIS = getKeyInt(TEXT, "$BEGINANALYSIS", true);
  BEGINDATA     = getKeyInt(TEXT, "$BEGINDATA", true);
  BEGINSTEXT    = getKeyInt(TEXT, "$BEGINSTEXT", true);
  BYTEORD       = getKeyMulti
   (TEXT,"$BYTEORD", QStringList() << "1,2,3,4" << "4,3,2,1" << "3,4,1,2", true);
  DATATYPE      = getKeyMulti
   (TEXT,"DATATYPE", QStringList() << "I" << "F" << "D" << "A", true);
  ENDANALYSIS   = getKeyInt(TEXT,"$ENDANALYSIS", true);
  ENDDATA       = getKeyInt(TEXT,"$ENDDATA", true);
  ENDSDATA      = getKeyInt(TEXT,"$ENDSDATA", true);
  MODE          = getKeyMult
   (TEXT,"$MODE", QStringList() << "C" << "L" << "U", true);
  NEXTDATA	= getKeyInt(TEXT,"$NEXTDATA", true);
  PAR           = getKeyInt(TEXT,"$PAR", true);

  //let's catch some errors 
  if (MODE !=1)
   throwError("getHeaderData", "Can only handle listmode");

  if (PAR > maxParameter)
   throwError("getHeaderData", "phew, too many parameters I'm bailing");

  //load up the parameter data
  for (int p = 0; p < PAR; p++)
  {
    QString ps = QString().setNum(p);
    PnB[p] = getKeyInt(TEXT, "$P"+ps+"B", true);
    PnE[p] = get
    PnR[p] = getKeyInt(TEXT, "$P"+ps+"R", true);
    PnF[p] = getKeyWord(TEXT, "$P"+ps+"F", false);
    PnG[p] = getKeyDouble[TEXT, "$P"+ps+"G", false);
  };

  


  }; 
  
  
*/