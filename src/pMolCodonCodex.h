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
#ifndef PMOLCODONCODEX_H
#define PMOLCODONCODEX_H

#include <QString>

#include "pMolStackObject.h"
#include "pMolElement.h"
#include "pMolKernelInterface.h"
#include "pMolCodon.h"
#include "pMolCodonArray.h"

class pMolCodonCodex : public pMolStackObject
{
  public:
    pMolCodonCodex();
    pMolCodonCodex(pMolXMLbase * p_p_MolXMLbaseParent);
    virtual ~pMolCodonCodex();

    virtual pMolXMLbase* XMLopen(const QString &tag, QString &error);
    virtual pMolXMLbase* XMLclose(const QString &tag, const QString &data, QString &error);
    virtual bool type(int i);
    virtual void treeOut(QTreeWidgetItem *tree);
    virtual pMolStackObject* exec(pMolCmd* cmd, pMolKernelInterface* interface);
    QString translate(const QString &p_source);			///translate string
    pMolCodon* getCodon(const QString &p_source);		///get a single codon
    pMolCodon* getBaseCodon();

    int frameLength;
    bool functional;
    int totalFrequency;
    int iterations;

  protected:
    void linkCodons();
    bool isStart(QString s);
    bool isStop(QString s);
    int ORFlength(pMolElement* dna, int index);
    int ORFlength3(pMolElement* dna, int index);
    QString code(const QString &sequence);
    pMolCodonArray* codeWobble(pMolCodonArray* codons1);
    pMolCodonArray* codeOptimizeSeqment(QString translation);
    pMolStackObject* c_findORF(pMolCmd* cmd, pMolKernelInterface* interface);
    pMolStackObject* c_findORFs(pMolCmd* cmd, pMolKernelInterface* interface);
    pMolStackObject* translate(pMolCmd* cmd, pMolKernelInterface* interface);
    pMolStackObject* code(pMolCmd* cmd, pMolKernelInterface* interface);
    pMolStackObject* debug(pMolCmd* cmd, pMolKernelInterface* interface);
    pMolStackObject* wobble(pMolCmd* cmd, pMolKernelInterface* interface);

    bool currentCodonExists;
    QString translateFrom;
    QString translateTo;
    pMolCodon* currentCodon;
    pMolCodon* baseCodon;


    QList <pMolCodon*> codons;
};

#endif
