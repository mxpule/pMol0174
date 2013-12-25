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

#include "pMolMpsv/pMolMpsvGlobPath.h"

//----------------------------------------------------
///Constructors and destructors
pMolMpsvGlobPath::pMolMpsvGlobPath()
{
  i = 0;
};

pMolMpsvGlobPath::~pMolMpsvGlobPath()
{
};

//----------------------------------------------------
///resets a path
void pMolMpsvGlobPath::reset()
{
  i = 0;
};

//----------------------------------------------------
///Deep copy functions 
pMolMpsvGlobPath* pMolMpsvGlobPath::copy(pMolMpsvGlobPath* path)
{
  path->reset();
  for (int j=0; j<i; j++)
    path->add(globs[j], y_array[j]);
  return path;
};

pMolMpsvGlobPath* pMolMpsvGlobPath::copy()
{
  pMolMpsvGlobPath* path = new pMolMpsvGlobPath();
  return copy(path);
};

//----------------------------------------------------
///add a strip to mask
void pMolMpsvGlobPath::add(pMolMpsvGlob* glob, float y)
{
  if (i>=max) return;
  globs[i] = glob;
  y_array[i] = y;
  i++;
};

//----------------------------------------------------
///bind globs to parent glob 
void pMolMpsvGlobPath::bind(pMolMpsvGlob* parent)
{
  for (int j=0; j<i; j++)
  {
    pMolMpsvGlob* glob = globs[j];
    float y = y_array[j];

    if (glob->getDir() > 0)
    { 
      glob->moveBy(glob->getX(), -y);		//positive direction, so simply
    //  parent->addToGroup(glob);				//transform by y and register parent
      glob->setParentItem(parent);
      glob->compact();
    }
    else
    {
      glob->setRotation(180);
      glob->moveBy(-glob->getX(), -y);
      //parent->addToGroup(glob);
      glob->setParentItem(parent);
      glob->compact();
    };
  };
};
