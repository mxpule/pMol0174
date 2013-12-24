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

#ifndef PMOLGENESPLICERTABLE_H
#define PMOLGENESPLICERTABLE_H

#include <QString>
#include <QtGui>

#include "pMolStackObject.h"

class pMolGeneSplicerTable : public pMolStackObject
{
  public:
    pMolGeneSplicerTable(int p_acceptorLen, int p_alphabetSize, 
      int p_markovLen, pMolXMLbase* p_pMolXMLbaseParent, const QString &p_name);
    ~pMolGeneSplicerTable();

    virtual pMolXMLbase* XMLclose(const QString &tag, const QString &data, QString &error);
    virtual void treeOut(QTreeWidgetItem* tree);

    double getPositiveTableValue(int baseNumber, int base, int markov, bool* ok);
    double getNegativeTableValue(int baseNumber, int base, int markov, bool* ok);
    void setPositiveTableValue(int baseNumber, int base, int markov, double value);
    void setNegativeTableValue(int baseNumber, int base, int markov, double value);

  protected:
   double value;
   int baseNumber, base, markov;
   int length, alphabetSize, markovLen;
   bool baseNumberSet, baseSet, markovSet, valueSet;

   double*  Positive_Table;
   double*  Negative_Table;
};

#endif
