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
#include <QtGui>
#include "pMolMpsv/pMolMpsvMask.h"

//-------------------------------------------------
///Constructors and Destructors
pMolMpsvMask::pMolMpsvMask(float p_uc)
{
  uc = p_uc;
  x0=0; x1=0; y0=0; y1=0;

  for (int i=0; i<max; i++)
    use[i] = false;

  virgin = true;
};

pMolMpsvMask::~pMolMpsvMask()
{
};

//-----------------------------------------------------------
///deep copy functions
pMolMpsvMask* pMolMpsvMask::copy(pMolMpsvMask* to)
{
  to->reset();
  for (int x=0; x<max; x++)
   if (use[x]) to->setMask(x, top[x], bot[x]);

  return to;
}

pMolMpsvMask* pMolMpsvMask::copy()
{
  pMolMpsvMask* to = new pMolMpsvMask(uc);
  return copy(to);
};

//-----------------------------------------------------------
///basic access functions
bool pMolMpsvMask::getUse(int x)
{
  if ((x<0) || (x>=max)) return false;
  return use[x];
};

int pMolMpsvMask::getTop(int x)
{
  if ((x<0) || (x>=max)) return 0;
  return top[x];
};

int pMolMpsvMask::getBot(int x)
{
  if ((x<0) || (x>=max)) return 0;
  return bot[x];
};

float pMolMpsvMask::getHeight()
{
  return y1-y0;
};

float pMolMpsvMask::getWidth()
{
  return (int) ((x1-x0) * uc);
};

void pMolMpsvMask::setRect(QRectF &rect)
{
  rect.setRect(x0*uc, y0, getWidth(), getHeight());
};

//----------------------------------------------------------
///resets the class for re-use
void pMolMpsvMask::reset()
{
  virgin = true;
  for (int x=0; x<max; x++) use[x] = false; 
};

//----------------------------------------------------------
///masks in a vertical strip using native coordinate system
void pMolMpsvMask::setMask(int x, int p_top, int p_bot)
{
  //check to see if out of bounds
  if ((x<0) || (x>=max)) return;

  //is this the first strip entered?
  if (virgin)
  {
    virgin = false;
    use[x] = true;
    top[x] = p_top;
    bot[x] = p_bot;
    x0 = x;
    x1 = x + uc;
    y0 = p_top;
    y1 = p_bot;
    return;
  };

  if (use[x])
  {
  //is this strip already in use? then merge in
    if (p_top < top[x]) 
    { top[x] = p_top;
      if (y0>p_top) y0 = p_top;
    };
    if (p_bot > bot[x]) 
    { bot[x] = p_bot;
      if (y1<p_bot) y1 = p_bot;		//correct top right if extended
    };
  }
  else
  //the srip is not already in use, plonk in, no need to merge
  { use[x] = true;
    top[x] = p_top;
    bot[x] = p_bot;
    if (x<x0) x0 = x;
    if (x>x1) x1 = x;
    if (y0>p_top) y0 = p_top;
    if (y1<p_bot) y1 = p_bot;	
  }
};

//----------------------------------------------------------
///masks in a positive rectangle x,y top left with width w and heigh h
void pMolMpsvMask::addMask(float p_x, float p_y, float p_w, float p_h)
{
  int y = (int) p_y;
  int h = (int) p_h;
  int x = (int) p_x/uc;
  int w = (int) p_w/uc;
  if (w==0) w=1;

  for (int i=0; i<w; i++) 
    setMask(x+i, y, y+h);
};


//----------------------------------------------------------
///masks in a positive mask
void pMolMpsvMask::addMaskPos(float p_x, float p_y, pMolMpsvMask* mask)
{
  int y = (int) p_y;
  int x = (int) p_x;

  for (int i=0; i<max-x; i++)
    if (mask->getUse(i)) setMask(i+x, y+mask->getTop(i), y+mask->getBot(i));
};


//----------------------------------------------------------
///masks in a negative mask (rotated around 0,0 180 degrees)
void pMolMpsvMask::addMaskNeg(float p_x, float p_y, pMolMpsvMask* mask)
{
  int y = (int) p_y;
  int x = (int) p_x;

  for (int i=0; i<x; i++)
    if (mask->getUse(i)) setMask(x-i, y-mask->getBot(i), y-mask->getTop(i));
};

//-----------------------------------------------------------
///shifts everything up so that bottom left is (0,0)
float pMolMpsvMask::normalize()
{
  for (int x=0; x<max; x++)
    if (use[x]) 
    { top[x] -= y1;
      bot[x] -= y1;
    };

  float temp = (float) -y1;
  y0 -= y1;
  y1 -= y1;

  return temp;
};



//------------------------------------------------------------
///fits a mask at x positive direction, returns minimum height for fit
float pMolMpsvMask::fitPos(int x, pMolMpsvMask* mask)
{
  int y = 0;
  int temp_y;
  bool virgin = true;

  for (int i=0; i<max-x; i++)
  { 
    if (mask->getUse(i) && getUse(i+x))
    { 
      temp_y =  mask->getBot(i) - top[i+x];
      if (virgin) 
      { virgin = false; y = temp_y; }
      else
      if (temp_y > y) y = temp_y;
    }
  };
  addMaskPos((float) x, (float) -y, mask);
  return (float) y;
}

//------------------------------------------------------------
///fits a mask at x negative direction, returning minimum height for fit
float pMolMpsvMask::fitNeg(int x, pMolMpsvMask* mask)
{
  int y = 0;
  int temp_y;
  bool virgin = true;

  for (int i=0; i<x; i++)
  {
    if (mask->getUse(i) && getUse(x-i))
    {
      temp_y =  getBot(x-i) - mask->getBot(i);
      if (virgin)
      { virgin = false; y = temp_y; }
      else
      if (temp_y > y) y = temp_y;
    };
  };
  addMaskNeg((float) x, (float) y, mask);
  return (float) y;
};

