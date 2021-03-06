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
#ifndef PMOLCODON_H
#define PMOLCODON_H

#include <QString>
#include "pMolXMLbase.h"

class pMolCodon : public pMolXMLbase
{
  public:
    pMolCodon();
    pMolCodon(pMolXMLbase * p_p_MolXMLbaseParent);
    virtual ~pMolCodon();

    virtual pMolXMLbase* XMLopen(const QString &tag, QString &error);
    virtual pMolXMLbase* XMLclose(const QString &tag, const QString &data, QString &error);
    virtual void treeOut(QTreeWidgetItem *tree);

    void linkCodon(pMolCodon* newCodon);
    void linkSimilar(pMolCodon* p_rootTranslation);
    void linkSimilar(pMolCodon* p_rootTranslation, pMolCodon* newCodon);
    void linkTranslation(pMolCodon* newCodon);
    QString searchCode(const QString &p_code, bool* p_start, bool* p_stop);
    pMolCodon* getCodon(const QString &p_sequence);
    pMolCodon* searchTranslation(const QString &p_code);
    pMolCodon* similarTranslation();

    pMolCodon* reset();			///returns baseCodon
    pMolCodon* flip(bool* ok);		///tries to flip to an alternative codon
    void setBytes(short int* buffer, int i);

    int frequency;			///useage frequency of this codon in the organism specified
    int codeCount;			///how many codons representing the same code exist
    int codeFrequency;			///the useage frequency of this code (sum of all codons same code's frequency)
    int codeBestFrequency;		///the best useagefrequency 
    double badness;			///penalty incurred for using in codon optimization
    double usagePercent;		///percentage amongst all the codons for this code
    bool start, stop;			///is this codon a start or stop codon?
    unsigned int fractionIndex;		///look up table value = 0 to 63 for fast codon optimization
    double fraction;
    QString code, translation;		///what is the code and what does it translate to?
    short int* bytes;


  protected:
    static const int maxFrameLength = 64;
    short int coding[maxFrameLength];

    pMolCodon* upCodon;
    pMolCodon* dnCodon;
    pMolCodon* upTranslation;
    pMolCodon* dnTranslation;
    pMolCodon* eqTranslation;
    pMolCodon* rootTranslation;

    short int getCode(QChar c);
    void setCodonStats(int* sum, int* count, int* best);
    double calculateBadnessExp(int frequency, int codeFrequency, int codeBestFrequency, int codeCount);
};

#endif
