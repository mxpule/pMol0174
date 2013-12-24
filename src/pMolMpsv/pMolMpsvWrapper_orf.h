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
#ifndef PMOLMPSVWRAPPER_ORF_H
#define PMOLMPSVWRAPPER_ORF_H

#include <QtGui>

#include "pMolORF.h"
#include "pMolMpsv/pMolMpsvWrapper.h"
#include "pMolMpsv/pMolMpsvGlob.h"
#include "pMolMpsv/pMolMpsvStyle.h"
#include "pMolMpsv/pMolMpsvWrapper_layer.h"

///pMolMpsv wrapper object wrapping a pMolOrf
class pMolMpsvWrapper_orf : public pMolMpsvWrapper
{

  public:
    pMolMpsvWrapper_orf(pMolORF* p_orf);
    virtual ~pMolMpsvWrapper_orf();

    virtual int getIndex();
    virtual int getLength();
    virtual QString getName();
    virtual pMolMpsvGlob* makeGlob(int i0, int i1, pMolMpsvStyle* style, bool root);
    virtual pMolMpsvWrapper* getElement(int i);

    ///propogates user event messages
    virtual void userEvent(int type, int portion);

    void setHighlightMode(int mode);

    enum highlightModes {BLANK, PLAIN, BADNESS, POLARITY, CHARGE, RESIDUE};

//  public slots:
//    void flip(QGraphicsSceneMouseEvent* event);

  private:
    int highlightMode;
    pMolORF* orf;
    //static const short int flatten[];
    //static const float polarityArray[];
    //static const float chargeArray[];



    bool getResidueNumber(QString code, int index, short int* number);
    QColor getCodonColor(int codon);

    QList <pMolMpsvWrapper*> elements;			///store the child elements
    QList <pMolMpsvWrapper*> layers;			///store the child elements

};

#endif
