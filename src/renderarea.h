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

#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <math.h>

#include <QtGui>

#include "pMolecule.h"
#include "RenderObjectContainer.h"

class RenderArea : public QAbstractScrollArea
{
    Q_OBJECT

public:

    RenderArea(QWidget *parent, pMolecule *p_pMol);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;


protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void resizeEvent(QResizeEvent *event);

    pMolecule *pMol;

    bool reSized;
    int totalHeight;
    QList <RenderObjectContainer *> renderObjects;


};

#endif
