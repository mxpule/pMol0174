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

#ifndef PMOLMPSVWRAPPER_H
#define PMOLMPSVWRAPPER_H

#include "pMolMpsvStyle.h"
#include "pMolMpsvWrap.h"
#include "pMolElement.h"
#include "pMolMpsvGlob.h"

///next abstract class up to wrap a sequence representation object. Provides a static member
///which creates appropriate wrapper types.
class pMolMpsvWrapper : public pMolMpsvWrap
{
  public:
    ///constructor
    pMolMpsvWrapper();

    ///virtual desctructor
    virtual ~pMolMpsvWrapper();

    ///returns true if the wrapper has been updated in some way
    virtual bool refresh();

    ///makes the graphical object representing this dna annotation from parent coordinates <i>
    ///i0</i> and <i>i1</i>. Also given style and told if root annotation or not.
    virtual pMolMpsvGlob* makeGlob(int i0, int i1, pMolMpsvStyle* style, bool root) = 0;

    ///propogates user event messages
    virtual void userEvent(int type, int portion);

    ///how many child elements does the annotation have?
    virtual int getElementCount();

    ///transparently get child annotation <i>i<i>. Return NULL if <i>i</i> is out of bounds
    virtual pMolMpsvWrapper* getElement(int i);

    ///static member to allow binding and creation of wrappers
    static pMolMpsvWrapper* bind(pMolElement* element);


    long unsigned int mutateKeyObject;

  protected:
    bool visible;
    bool recurse;
    bool updated;
    QColor textColor;			///colour to draw annotation text in
    QColor fillColor;			///colour to draw stuff in
    QColor backColor;			///background color


    bool clip
    ( int i0, int i1, int index, int length, 
      int* i, int* l, bool *clip5, bool* clip3, bool* minor);

};

#endif
