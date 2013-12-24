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
#ifndef PMOLMPSVWRAPPER_RE_H
#define PMOLMPSVWRAPPER_RE_H

#include "pMolAnnotation.h"
#include "pMolMpsv/pMolMpsvStyle.h"
#include "pMolMpsv/pMolMpsvWrapper.h"

class pMolMpsvWrapper_re : public pMolMpsvWrapper
{
  public:
    pMolMpsvWrapper_re(pMolAnnotation* p_re);
    virtual ~pMolMpsvWrapper_re();

    virtual int getIndex();
    virtual int getLength();
    virtual QString getName();
    virtual pMolMpsvGlob* makeGlob(int i0, int i1, pMolMpsvStyle* style, bool root);

  private:
    pMolAnnotation* re;
};

#endif