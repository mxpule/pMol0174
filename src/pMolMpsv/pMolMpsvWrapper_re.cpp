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

#include "pMolMpsv/pMolMpsvWrapper_re.h"

pMolMpsvWrapper_re::pMolMpsvWrapper_re(pMolAnnotation* p_re) : pMolMpsvWrapper()
{
  re = p_re;
  fillColor = QColor(0x80, 0xff, 0xff, 0x80);	//default color for primer
  textColor = QColor(0x00, 0x00, 0x00, 0x80); 	//default text color for primer
};

pMolMpsvWrapper_re::~pMolMpsvWrapper_re()
{
};

//--------------------------------------------------------------------------------------------
///return length and index of wrapped pMol object
int pMolMpsvWrapper_re::getIndex()
{
  return re->getIndex();
};

int pMolMpsvWrapper_re::getLength()
{
  return re->getLength();
};

QString pMolMpsvWrapper_re::getName()
{
  return re->getName();
};

pMolMpsvGlob* pMolMpsvWrapper_re::makeGlob(int i0, int i1,pMolMpsvStyle* style, bool root)
{
  int i, l;
  bool clip5, clip3, minor;

  //are we within range? if so have we been clipped and how?
  if (!clip(i0,i1, re->getIndex(), re->getLength(), &i, &l, &clip5, &clip3, &minor)) 
    return NULL;

  float uc = style->uc;
  pMolMpsvGlob* glob = new pMolMpsvGlob(this, style, i0, i, 2);	//a glob to store the drawing instructions
  QBrush brush(fillColor);
  QPen pen(textColor);

  //the main sequence bar
  glob->addRectItem(0.0, -0.25*uc, l*uc, -1.25*uc, brush);

  //annotate the primer
  int w = glob->getAnnotationTextWidth(re->getName());	//to see how wide the text is
  if (w < l*uc) 				//if it fits, render it on the bar
    glob->addAnnotationText((l*uc-w)/2.0, -1.25*uc, re->getName(), textColor);
  else if (!minor)				//if doesn't fit and this isn't a minor fragment
    glob->addAnnotationText((l*uc-w)/2.0, -2.25*uc, re->getName(), textColor); 

  glob->normalize();
  return glob;
};