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
///This is the pMol terminal programme.
///It is essentially a bastardized version of QT qTextEdit

#ifndef PMOLSYS_H
#define PMOLSYS_H

#include <QtGui>
#include <QTextEdit>

#include "pMolElement.h"
#include "pMolKernelInterface.h"
#include "pMolCmd.h"
#include "pMolDNA.h"
#include "pMolElement.h"
#include "pMolORF.h"
#include "pMolPeptide.h"
#include "pMolStackObject.h"

class pMolSys : public pMolStackObject
{

public:
    pMolSys();
    static pMolStackObject* c_copy(pMolKernelInterface* interface, pMolCmd* cmd);
    static pMolStackObject* c_find(pMolKernelInterface* interface, pMolCmd* cmd);
    static pMolStackObject* c_fasta(pMolKernelInterface* interface, pMolCmd* cmd);
    static pMolStackObject* sexec(pMolKernelInterface* interface, pMolCmd* cmd);

};

#endif
