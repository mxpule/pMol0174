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
#ifndef PMOLLAYER_H
#define PMOLLAYER_H

#include <QString>
#include <QtGui>

#include "pMolElement.h"


class pMolLayer : public pMolElement
{
  public :
    pMolLayer();
    pMolLayer(pMolXMLbase *p_pMolXMLbase);
    virtual pMolXMLbase* XMLopen(const QString &tag, QString &error);
    virtual pMolXMLbase* XMLclose(const QString &tag, const QString &data, QString &error);
    virtual bool type(int i);
    virtual bool save(QTextStream* stream);
    virtual bool saveContents(QTextStream* stream);
    virtual pMolElement* getElement(int* i);
    virtual QString getSequence();
    virtual QString getComplement();
    virtual QString getSequence(int p_index, int p_length);
    virtual int getLength();
    virtual int getIndex();
    virtual pMolElement* duplicate();
    virtual void annotate(const QString &name, const QString &comment, int index, int length);
    virtual QString getHierarchialName();

    int n_getChildCount();
    pMolElement* n_getElement(int i);

    bool orphan;

  protected :

};


#endif
