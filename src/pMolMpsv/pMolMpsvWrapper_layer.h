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


#ifndef PMOLMPSVWRAPPER_LAYER_H
#define PMOLMPSVWRAPPER_LAYER_H

#include "pMolLayer.h"
#include "pMolMpsv/pMolMpsvWrapper.h"

///wrapper class which wraps a pMolLayer
class pMolMpsvWrapper_layer : public pMolMpsvWrapper
{
  public:
    ///constructor is passed the pMolLayer
    pMolMpsvWrapper_layer(pMolLayer* p_layer);

    ///virtual desctructor
    virtual ~pMolMpsvWrapper_layer();

    virtual int getIndex();
    virtual int getLength();
    virtual QString getName();
    virtual int getElementCount();
    virtual pMolMpsvWrapper* getElement(int i);

    virtual pMolMpsvGlob* makeGlob(int i0, int i1, pMolMpsvStyle* style, bool root);

  private:
    pMolLayer* layer;
    pMolMpsvWrapper* parent;
    QList <pMolMpsvWrapper*> elements;			///store the child elements
};

#endif
