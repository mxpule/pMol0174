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
#ifndef PMOLANNOTATION_H
#define PMOLANNOTATION_H

#include <QString>
#include <QtGui>

#include "pMolElement.h"
#include "pMolLayer.h"
#include "pMolPrimer.h"
#include "pMolKernelInterface.h"

class pMolAnnotation : public pMolElement
{
  public :
    pMolAnnotation();
    pMolAnnotation(pMolXMLbase *p_pMolXMLbase);
    virtual pMolXMLbase* XMLopen(const QString &tag, QString &error);
    virtual pMolXMLbase* XMLclose(const QString &tag, const QString &data, QString &error);
    virtual bool type(int i);
    virtual bool save(QTextStream* stream);
    virtual bool saveContents(QTextStream* stream);
    virtual void treeOut(QTreeWidgetItem *tree);
    virtual pMolElement* getElement(int* i);
    virtual pMolElement* getElement(pMolCmdName* cmdName);
    virtual pMolElement* duplicate();
    virtual bool loadDependencies(pMolKernelInterface* interface);
    virtual void annotate(const QString &name, const QString &comment, int index, int length);
    virtual void append(pMolElement* element);
    virtual pMolStackObject* exec(pMolCmd* cmd, pMolKernelInterface* interface);

    pMolStackObject* command(pMolCmdName* cmdName, pMolKernelInterface* interface);
    pMolElement* duplicate(pMolAnnotation* annotation);

    pMolElement* n_getTransparentElement(int i);
    pMolElement* n_getElement(int i);
    pMolElement* n_getLayer(int i);

  protected :
    QList <pMolElement*> pMolLayers;
    pMolStackObject* seg(int index, int length);
    pMolStackObject* seg(pMolCmd* cmd, pMolKernelInterface* interface);
    pMolStackObject* c_seg5(pMolCmd* cmd, pMolKernelInterface* interface);
    pMolStackObject* c_seg3(pMolCmd* cmd, pMolKernelInterface* interface);
};

#endif
