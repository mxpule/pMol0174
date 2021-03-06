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
#ifndef PMOLOBJECT2_H
#define PMOLOBJECT2_H

#include <QtGui>
#include <QString>

#include "pMolXMLbase.h"

///next in hierarchy of pMolObject, this one is executable
///this is probably in the wrong place
///the main reason I made such a big hierarchy was the problem
///of layered definitions
class pMolObject : public pMolXMLbase
{
  public:
    pMolObject();
    pMolObject(pMolXMLbase *p_pMolXMLbaseParent);


    virtual void exec(const QString &p_params);
    virtual bool type(int);

    void mutate();
    bool mutated(long unsigned int mutateKey);
    long unsigned int getMutateKey();

    enum childTypes {NUMBER, LIST, DNA, OBJECT, STACK_OBJECT, STRING, EXEC, CODONCODEX, LAYER,
                      DOUBLE, RESTRICTIONENDONUCLEASE, ORF, ANNOTATION, RAW, PEPTIDE, PRIMER,
                      BATCH, WINDOW, ENGINE, REA, ERROR, VIEWABLE};

   private:
     long unsigned int mutateCount;
};

#endif
