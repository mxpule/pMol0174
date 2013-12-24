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
#ifndef PMOLCODONARRAY_H
#define PMOLCODONARRAY_H

#include <QString>

#include "pMolCodon.h"

class pMolCodonArray
{
  public:
    pMolCodonArray(pMolCodon* p_baseCodon, const QString &translation);
    ~pMolCodonArray();

    bool flip(int i);			///tries to flip codon, true if successfull
    void cflip(int i);			///flips and if at end goes on around
    void reset(int i);			///resets codon to first one
    double badness();			///calculates badness;
    QString sequence();
    pMolCodon* at(int i);
    void copy(pMolCodonArray* array);
    double similarity(pMolCodonArray* codons2, int repeatsAllowed, int segLength);
    int length;

    static const int maxLength = 64;
    static const double repeatPenalty = 100.0;

  protected:
    pMolCodon* baseCodon;
    pMolCodon* codons[maxLength];
};


#endif

