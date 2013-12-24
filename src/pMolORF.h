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
#ifndef PMOLORF_H
#define PMOLORF_H

#include <QString>
#include <QtGui>

#include "pMolPeptide.h"
#include "pMolAnnotation.h"
#include "pMolCodonCodex.h"
#include "pMolKernelInterface.h"

class pMolORF : public pMolAnnotation
{
  public :
    pMolORF();
    pMolORF(pMolXMLbase *p_pMolXMLbase);
    pMolORF(const QString &p_name, int p_index, int p_length, const QString &p_codonCodexName);

    virtual pMolXMLbase* XMLclose(const QString &tag, const QString &data, QString &error);
    virtual bool type(int i);
    virtual bool save(QTextStream* stream);
    virtual bool saveContents(QTextStream* stream);
    virtual pMolElement* duplicate();
    virtual bool loadDependencies(pMolKernelInterface* interface);
    virtual pMolStackObject* command(pMolCmdName* cmdName, pMolKernelInterface* interface);
    virtual pMolStackObject* exec(pMolCmd* cmd, pMolKernelInterface* interface);

    pMolStackObject* findPeptideSequence(pMolPeptide* peptide);
    pMolStackObject* find(pMolCmd* cmd, pMolKernelInterface* interface);

    QString translate(int i0, int i1);
    QString codonCodexName;
    pMolCodonCodex* codonCodex;

  protected :
    pMolStackObject* c_seg(pMolCmd* cmd, pMolKernelInterface* interface);
    pMolStackObject* c_seg3(pMolCmd* cmd, pMolKernelInterface* interface);

};

#endif
