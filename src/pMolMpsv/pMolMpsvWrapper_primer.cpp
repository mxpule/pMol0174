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

#include "pMolMpsvWrapper_primer.h"

//--------------------------------------------------------------------------------------------
///constructors and desctructors
pMolMpsvWrapper_primer::pMolMpsvWrapper_primer(pMolPrimer* p_primer) : pMolMpsvWrapper()
{
  primer = p_primer;
  mutateKeyObject = primer->getMutateKey();
  fillColor = QColor(172, 204, 222, 0xff);	//default color for primer
  textColor = QColor(0x00, 0x00, 0x00, 0xff); 	//default text color for primer
};

pMolMpsvWrapper_primer::~pMolMpsvWrapper_primer()
{
};

//--------------------------------------------------------------------------------------------
///return length and index of wrapped pMol object
int pMolMpsvWrapper_primer::getIndex()
{
  return primer->getIndex();
};

int pMolMpsvWrapper_primer::getLength()
{
  return primer->getLength();
};

QString pMolMpsvWrapper_primer::getName()
{
  return primer->getName();
};

bool pMolMpsvWrapper_primer::refresh()
{
   //have we changed? in this case we have to rebuild everything
   if (!primer->mutated(mutateKeyObject)) return false;

   //otherwise reset mutation
   mutateKeyObject = primer->getMutateKey(); //reset key
   return true;
};

//--------------------------------------------------------------------------------------------
///makes the graphical object 
pMolMpsvGlob* pMolMpsvWrapper_primer::makeGlob(int i0, int i1, pMolMpsvStyle* style, bool root)
{

  //qDebug() << "primerwarpper staring to make glob";
  int i, l, ii, ll;
  bool clip5, clip3, minor;

  //are we within range? if so have we been clipped and how?
  if (!clip(i0,i1, primer->getIndex(), primer->getLength(), &i, &l, &clip5, &clip3, &minor)) 
    return NULL;

  float uc = style->uc;
  QPen pen(textColor);
  QBrush brush(fillColor);
  pMolMpsvGlob* glob = new pMolMpsvGlob(this, style, i0, i, 2);	

  //do the 5' end if not clipped
  if (!clip5) 
  { ii = 1;
    glob->addEllipseItem(0,-1.25*uc, 1.25*uc, 1.5*uc, brush);	//if 5' end not clipped, draw
  } else ii = 0;
  
  //do the 3' end if not clipped
  if (!clip3)
  { ll = l-ii-1;
    glob->addRightArrow(l*uc-uc,-1.25*uc, uc,1.5*uc, brush); //if 3' end not clipped, draw
  } else ll = l - ii;

  //the main sequence bar
  glob->addRectItem(ii * uc, -uc, ll*uc, uc, brush);

  //annotate the primer
  int w = glob->getAnnotationTextWidth(primer->getName());	//to see how wide the text is
  if (w < l*uc) 				//if it fits, render it on the bar
    glob->addAnnotationText((l*uc-w)/2.0, -0.8*uc, primer->getName(), textColor);
  else if (!minor)				//if doesn't fit and this isn't a minor fragment
    glob->addAnnotationText((l*uc-w)/2.0, -1.5*uc, primer->getName(), textColor); 


  glob->normalize();
//  qDebug() << "primerwarpper finished to make glob";
  return glob;
};
