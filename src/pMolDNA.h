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
#ifndef PMOLDNA_H
#define PMOLDNA_H

#include <QString>
#include <QtGui>

#include "pMolAnnotation.h"
#include "pMolStackObject.h"
#include "RenderObjectContainer.h"
#include "pMolKernelInterface.h"
#include "pMolPrimer.h"
#include "pMolCmd.h"
#include "pMolORF.h"

///final pMolecule class - hopefully we can remove this soon for the unified DNA class
class pMolDNA : public pMolAnnotation
{
  public:
    pMolDNA();
    pMolDNA(pMolXMLbase *p_pMolXMLbaseParent);
    pMolDNA(const QString &p_sequence);
    pMolDNA(const QString &p_name, const QString &p_sequence);
    static pMolStackObject* create(pMolKernelInterface* interface, pMolCmd* cmd);

    virtual pMolXMLbase* XMLopen(const QString &tag, QString &error);
    virtual pMolXMLbase* XMLclose(const QString &tag, const QString &data, QString &error);
    virtual void treeOut(QTreeWidgetItem *tree);
    virtual bool type(int i);
    virtual bool isCircular();
    virtual bool save(QTextStream* stream);
    virtual bool saveContents(QTextStream* stream);
    virtual pMolDNA* complement();
    virtual int getLength();
    virtual QString getSequence();
    virtual QString getSequence(int p_index, int p_length);
    virtual pMolElement* duplicate();
    //virtual bool loadDependencies(pMolKernelInterface* interface);

    pMolDNA* cutRight(int p_index);
    pMolDNA* cutLeft(int p_index);
    pMolDNA* cutOpen(int p_index);
    void crop5(int index);
    pMolAnnotation* extend3(const QString &name, const QString &comment, const QString &addSequence);

    bool circular;			// if DNA is circular
    int overhangAtStart;		// over or underhang (0 means blunt)
    int overhangAtEnd;			// over or underhang (0 means blunt)

    QString sequence;
 protected:
   static pMolStackObject* s_createFromList(pMolStackList* list);
   static pMolStackObject* s_createFromRandom(pMolStackNumber* number);
   static pMolStackObject* s_createFromString(pMolStackString* s);
   static pMolStackObject* s_createFromElement(pMolElement* dna);

};

#endif
