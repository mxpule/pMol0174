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

#ifndef PMOLMPSVWRAPPER_PRIMER_H
#define PMOLMPSVWRAPPER_PRIMER_H

#include "pMolPrimer.h"
#include "pMolMpsvStyle.h"
#include "pMolMpsvWrapper.h"

///a pMolMpsv wrapper object to wrap a primer
class pMolMpsvWrapper_primer : public pMolMpsvWrapper
{
  public:
    pMolMpsvWrapper_primer(pMolPrimer* p_primer);
    virtual ~pMolMpsvWrapper_primer();

    virtual int getIndex();
    virtual int getLength();
    virtual QString getName();
    virtual pMolMpsvGlob* makeGlob(int i0, int i1, pMolMpsvStyle* style, bool root);
    virtual bool refresh();

  private:
    pMolPrimer* primer;
};

#endif
