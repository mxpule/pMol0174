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
#ifndef PMOLPEPTIDE_H
#define PMOLPEPTIDE_H


#include "pMolStackObject.h"
#include "pMolKernelInterface.h"
#include "pMolCmd.h"

class pMolPeptide : public pMolStackObject
{
  public :
    pMolPeptide();
    pMolPeptide(pMolXMLbase *p_pMolXMLbaseParent);
    static pMolPeptide* create(pMolKernelInterface* interface, pMolCmd* cmd);
    virtual pMolXMLbase* XMLclose(const QString &tag, const QString &data, QString &error);

    virtual bool type(int i);
    QString getSequence();
    int getLength();

    QString sequence;

  private:
   QString cleanString(const QString &s);
};

#endif
