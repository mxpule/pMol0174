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
#ifndef PMOLECULARENGINE_H
#define PMOLECULARENGINE_H

#include <QString>

#include "pMolStackObject.h"
#include "pMolDNA.h"

class pMolMolecularEngine
{
  public :
    static pMolDNA* ligate(pMolDNA* m1, pMolDNA* m2);
    static void ligatePermutations(pMolStackList* list,pMolStackObject* o1, pMolStackObject* o2);
    static pMolStackList* ligate(pMolStackList* inlist);
    static pMolStackObject* ligate(pMolCmd* cmd, pMolKernelInterface* interface);
    static pMolStackObject* exec(pMolKernelInterface* interface, pMolCmd* cmd);
};

#endif
