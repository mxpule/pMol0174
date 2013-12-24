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

#ifndef PMOLMPSVWRAPPER_ANNOTATION_H
#define PMOLMPSVWRAPPER_ANNOTATION_H

#include "pMolAnnotation.h"
#include "pMolMpsv/pMolMpsvWrapper.h"

///pMolMpsv Wrapper for pMolAnnotation
class pMolMpsvWrapper_annotation : public pMolMpsvWrapper
{
  public:
    ///constructor is passed pMolAnnotation to be wrapped
    pMolMpsvWrapper_annotation(pMolAnnotation* p_annotation);

    ///virtual destructor
    virtual ~pMolMpsvWrapper_annotation();

    //has anything been updated?
    virtual bool refresh();

    ///get the index of the wrapped class
    virtual int getIndex();

    ///get the length of the wrapped class
    virtual int getLength();

    ///get the name of the wrapped class
    virtual QString getName();

    ///transparently get the child element <i>i<i>. Return NULL if out of range
    virtual pMolMpsvWrapper* getElement(int i);

    ///make the graphical object (glob) to represent this pMolAnnotation between
    ///base <i>io</i> and <i>i1</i> parent coorodinates. Also given style and
    ///if the root class (i.e. the first annotation off the sequence) passed true
    ///in root (so that it knows to draw the actual sequence data)
    virtual pMolMpsvGlob* makeGlob(int i0, int i1, pMolMpsvStyle* style, bool root);

  private:
    pMolAnnotation* annotation;
    QList <pMolMpsvWrapper*> elements;		///store the child elements
    QList <pMolMpsvWrapper*> layers;		///store the layers

};

#endif
