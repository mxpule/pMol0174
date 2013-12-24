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
#ifndef PMOLMPSVGLOBPATH_H
#define PMOLMPSVGLOBPATH_H

#include "pMolMpsv/pMolMpsvGlob.h"

///represents a search path - i.e. the order / position of a sequence of child
///globs added to a parent glob. These child globs can be bound to the parent
///glob's QGraphicsItem by calling bind. Limited to 1024 child globs.
class pMolMpsvGlobPath
{
  public :
    ///constructor
    pMolMpsvGlobPath();

    ///destructor
    ~pMolMpsvGlobPath();

     ///clear all the date so we can re-use
     void reset();

     ///create new object and copy our data into it
     pMolMpsvGlobPath* copy();


     ///copy our data into an existing pMolMpsvGlobPath object
     pMolMpsvGlobPath* copy(pMolMpsvGlobPath* path);

     ///add a child glob to the path at position y
     void add(pMolMpsvGlob* glob, float y);

     ///bind the child globs to the parent glob provided
     void bind(pMolMpsvGlob* parent);

  private :
    int i;
    static const int max = 1024;
    pMolMpsvGlob* globs[max];
    float y_array[max];
};

#endif
