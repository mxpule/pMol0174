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

#include <math.h>
#include "pMolCodon.h"
#include "pMolCodonCodex.h"
#include "pMolMpsv/pMolMpsvWrapper_orf.h"
#include "pMolMpsv/pMolMpsvMenu.h"

//----------------------------------------------------------------------------
///constructors and desctructors
pMolMpsvWrapper_orf::pMolMpsvWrapper_orf(pMolORF* p_orf) : pMolMpsvWrapper()
{
  qDebug() << "pMolMpsvWrapper_orf::constructor starting";
  int i;
  orf = p_orf;
  fillColor = QColor(0x80, 0x80, 0xff, 0x80);	//default color for primer
  textColor = QColor(0x00, 0x00, 0x00, 0x80); 	//default text color for primer
  highlightMode = POLARITY;

  //bind up the elements
  pMolElement* element;  i = 0;
  while ((element = orf->n_getElement(i++))!=NULL)
  { pMolMpsvWrapper* wrapper = pMolMpsvWrapper::bind(element);
    if (wrapper!=NULL) elements.append(wrapper);
  }; 

  //bind up the layers
  pMolElement* layer;  i = 0;
  while ((layer = orf->n_getLayer(i++))!=NULL)
  { pMolMpsvWrapper* wrapper = pMolMpsvWrapper::bind(layer);
    if (wrapper!=NULL) layers.append(wrapper);
  }; 
  qDebug() << "pMolMpsvWrapper_orf::constructor ending";
};

pMolMpsvWrapper_orf::~pMolMpsvWrapper_orf()
{
  //delete all the elements
  while (!elements.isEmpty()) 
  { pMolMpsvWrapper* wrapper = elements.takeFirst();
    delete wrapper;
  };

  //delete all the layers
  while (!layers.isEmpty()) 
  { pMolMpsvWrapper* wrapper = layers.takeFirst();
    delete wrapper;
  };
};

//--------------------------------------------------------------------------------------
/*bool pMolMpsvWrapper_orf::type(int t)
{
  if (t==ORF) return true;
  return false;
};
*/

//--------------------------------------------------------------------------------------------
///return length and index of wrapped pMol object
int pMolMpsvWrapper_orf::getIndex()
{
  return orf->getIndex();
};

int pMolMpsvWrapper_orf::getLength()
{
  return orf->getLength();
};

QString pMolMpsvWrapper_orf::getName()
{
  return orf->getName();
};
//---------------------------------------------------------------------------------------------
///get element from Elements or from layers!
pMolMpsvWrapper* pMolMpsvWrapper_orf::getElement(int i)
{
  int count = elements.count();
  if (i<count) return elements.at(i);

  foreach (pMolMpsvWrapper* layer, layers)
  { i -= count;
    count = layer->getElementCount();
    if (i < count) return layer->getElement(i);
  };
  return NULL;
};



//--------------------------------------------------------------------------------------
///converts a character in a QString to a number representing an amino acid
bool pMolMpsvWrapper_orf::getResidueNumber(QString code, int index, short int* number)
{
const short int flatten[] = 
//convert ascii amino acids to numbers
// A  *   C   D   E   F   G   H   I   *  K  L   M   N  *    P   Q   R   S   T  *    V  W  *   Y
//65, *, 67, 68, 69, 70, 71, 72, 73   * 75 76  77  78  **  80  81  82  83  84  **  86  87  **  89 
  {0, -1,  1,  2,  3,  4,  5,  6,  7, -1, 8, 9, 10, 11, -1, 12, 13, 14, 15, 16, -1, 17, 18, -1, 19};

  short int t;
  int j = code.at(index).unicode() - 65;
  if ((j<0) || (j>25)) return false;
  if ((t=flatten[j])<0) return false;
  *number = t;
  return true;
}



//--------------------------------------------------------------------------------------
///gets the appropriate background of an amino acid depending on the highlight Mode
QColor pMolMpsvWrapper_orf::getCodonColor(int codon)
{

const float polarityArray[] =	//polar nature of residue
//A    C    D    E    F    G    H    I    K    L    M    N    P    Q    R    S    T    V    W    Y
{ 0.5, 0.0, 1.0, 1.0,-1.0, 0.0, 0.5,-1.0, 1.0,-1.0,-1.0, 1.0, 0.0, 1.0, 1.0, 0.5, 0.5,-1.0,-1.0, 0.5};


const int chargeArray[] =	//charged nature of residue
//A    C    D    E    F    G    H    I    K    L    M    N    P    Q    R    S    T    V    W    Y
//{ 0.0,-0.5,-1.0,-1.0, 0.0, 0.0, 0.5, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0};
{   2,   1,   0,   0,   2,   2,   3,   2,   4,   2,   2,   2,   2,   2,   4,   2,   2,   2,   2,   2};

  //plain just alternates between white and fillcolor
  if (highlightMode == PLAIN)
  {
    if (codon & 1) return QColor(0xd0, 0xd0, 0xd0, 0x80);
    return fillColor;
  };

  //highlights according to the usage of a codon
  if (highlightMode == BADNESS)
  {
    int frameLength = orf->codonCodex->frameLength;
    QString code = orf->getSequence(codon*frameLength,frameLength).toLower();
    pMolCodon* codon = orf->codonCodex->getCodon(code);
    if (codon==NULL) return QColor(0xff, 0xff, 0xff, 0x80);
    int intensity = round(255.0*(double)codon->frequency/(double)codon->codeBestFrequency);
    return QColor(0xff, intensity, intensity, 0x80);
  };

  //if phobic orange if philic yellow
  if (highlightMode == POLARITY)
  {
    int frameLength = orf->codonCodex->frameLength;
    QString code = orf->getSequence(codon*frameLength,frameLength).toLower();
    pMolCodon* codon = orf->codonCodex->getCodon(code);
    if (codon==NULL) return QColor(0xff, 0xff, 0xff, 0x80);
    int red, blue;
    short int residue;
    if (!getResidueNumber(codon->translation, 0, &residue))
      return QColor(0xff, 0xff, 0xff, 0x80);
    float polarity = polarityArray[residue];
    int green = 128 + round(127.0*polarity);
    return QColor(0xff, green, 0x00, 0xff);
  }

  //if negative in baby blue, if positive in baby pink!
  if (highlightMode == CHARGE)
  {
    int frameLength = orf->codonCodex->frameLength;
    QString code = orf->getSequence(codon*frameLength,frameLength).toLower();
    pMolCodon* codon = orf->codonCodex->getCodon(code);
    if (codon==NULL) return QColor(0xff, 0xff, 0xff, 0x80);
    short int residue;
    if (!getResidueNumber(codon->translation, 0, &residue))
      return QColor(0xff, 0xff, 0xff, 0x80);
    switch (chargeArray[residue]) {
      case 0 :  return QColor(0x00, 0x34, 0xff, 0x80);	//blue
      case 1 :  return QColor(0x80, 0x90, 0xff, 0x80);	//half-blue
      case 2 :  return QColor(0xe0, 0xe0, 0xe0, 0x80);	//white
      case 3 :  return QColor(0xff, 0x90, 0x80, 0x80);	//half-scarlet
      case 4 :  return QColor(0xff, 0x34, 0x00, 0x80);	//scarlet
    };
    return QColor(0xff, 0xff, 0xff, 0x80);
  }

  //unique color for each residue
  if (highlightMode == RESIDUE)
  {
    int frameLength = orf->codonCodex->frameLength;
    QString code = orf->getSequence(codon*frameLength,frameLength).toLower();
    pMolCodon* codon = orf->codonCodex->getCodon(code);
    if (codon==NULL) return QColor(0xff, 0xff, 0xff, 0x80);
    short int residue;
    if (!getResidueNumber(codon->translation, 0, &residue))
      return QColor(0xff, 0xff, 0xff, 0x80);
    int red   = 127 + (residue * 1771) % 128;
    int green = 55 + (residue *  67) % 200;
    int blue  = 55 + (residue * 711) % 200;
    return QColor(red, green, blue, 0xff);
  };
  //none, just return white background
  return QColor(0xff, 0xff, 0xff, 0x80);
}

///handle user events
void pMolMpsvWrapper_orf::userEvent(int type, int portion)
{
  qDebug() << "in the orf!";
  pMolMpsvMenu* myMenu;
  myMenu = new pMolMpsvMenu(this);
};

void pMolMpsvWrapper_orf::setHighlightMode(int mode)
{
  //qDebug() << mode;
  highlightMode = mode;
  updated = true;
};

//--------------------------------------------------------------------------------------
///makes the glob
pMolMpsvGlob* pMolMpsvWrapper_orf::makeGlob(int i0, int i1, pMolMpsvStyle* style, bool root)
{
  //qDebug() << "pMolMpsvWrapper_orf::makeGlob starting";

  int i, l;
  bool clip5, clip3, minor;

  //are we within range? if so have we been clipped and how?
  if (!clip(i0,i1, getIndex(), getLength(), &i, &l, &clip5, &clip3, &minor)) 
    return NULL;

  int ii = 0;
  int jj = l;
  float uc = style->uc;

  //initialize pens and make a glob ready for construction
  QPen pen(textColor);
  QBrush brush(fillColor);
  pMolMpsvGlob* glob = new pMolMpsvGlob(this, style, i0, i, 1);//a glob to store the drawing 

  //if we are the root object, draw the dna sequence too
           //glob->addSequenceText(0,0,annotation->getSequence(i,l).toLower(), textColor);
  if (root) glob->addSequenceText(i0,0,0,orf->getSequence(i,l).toLower(), textColor);

  //qDebug() << "pMolMpsvWrapper_orf::makeGlob about to access codoncodex!";

  //qDebug() << orf->getHierarchialName();
  //qDebug() << orf->getName();

  int frameLength = orf->codonCodex->frameLength;

  //the start of the sequence may need a codon clipped
  if (clip5)					//doesn't necessarily start in frame
  { int ofs = i % frameLength;
    if (ofs == 0) ii = 0;			//starts in frame, don't need to do anything
    else
    { ii = frameLength - ofs;
      QBrush brush(getCodonColor(i/frameLength));
      glob->addRectItem(0, -1.1*uc, ii * uc, 1.7*uc, brush);
    }
  }

  //qDebug() << "pMolMpsvWrapper_orf::makeGlob section 2";

  //the end of a sequence may need a codon clipped
  if (clip3)					//doesn't necessarily end in frame
  { int ofs = (i+l) % frameLength;
    if (ofs == 0) jj = l - ii;			//ends in frame
    else
    { 
      QBrush brush(getCodonColor((i+l-ofs)/frameLength));
      glob->addRectItem((l-ofs) * uc, -1.1*uc, ofs * uc, 1.7*uc, brush);
      jj = l  - ofs - ii;
    }
  }

  //qDebug() << "pMolMpsvWrapper_orf::makeGlob section 3";

  //do the sequence background itself 
  for (int iii=ii;  iii < jj; iii+=frameLength)
  {
    QBrush brush(getCodonColor((i+iii)/frameLength));
    glob->addRectItem(iii*uc, -1.1*uc, uc * frameLength, 1.7*uc, brush);
  }

  QColor sequenceColor(0x00, 0x00, 0xff, 0xff);
  glob->addSequenceText(i0, 0,-1.6*uc,orf->translate(i, l).toLower(), sequenceColor);

  int n = 0;
  if (recurse)
  {
    pMolMpsvWrapper* wrapper;
    while ((wrapper=getElement(n++))!=NULL)
      glob->addGlob(wrapper->makeGlob(i, i+l, style, false));
  };

  //qDebug() << "pMolMpsvWrapper_orf::makeGlob finishing";

  glob->minimize();
  return glob;
};
