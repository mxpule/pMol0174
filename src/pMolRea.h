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
#ifndef PMOLREA_H
#define PMOLREA_H

#include <QString>
#include <QtGui>
#include <QtSql>

#include "pMolElement.h"
#include "pMolCodonCodex.h"
#include "pMolKernelInterface.h"
#include "pMolRestrictionEndonuclease.h"

class pMolRea : public pMolElement
{
  public :
    pMolRea();
    pMolRea(pMolXMLbase *p_pMolXMLbase);
    pMolRea(pMolRestrictionEndonuclease* p_re);
    virtual pMolXMLbase* XMLopen(const QString &tag, QString &error);
    virtual pMolXMLbase* XMLclose(const QString &tag, const QString &data, QString &error);
    virtual bool type(int i);
    virtual bool save(QTextStream* stream);
    virtual bool saveContents(QTextStream* stream);
    virtual pMolElement* getElement(int* i);
    virtual pMolElement* getElement(const QString &p_name);
    virtual pMolElement* duplicate();
    virtual void annotate(const QString &name, const QString &comment, int index, int length);
    virtual bool loadDependencies(pMolKernelInterface* interface);

  protected :
    QString reName;
    pMolRestrictionEndonuclease* re;
};

#endif
