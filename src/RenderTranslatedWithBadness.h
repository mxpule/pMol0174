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
#ifndef RENDERTRANSLATEDWITHBADNESS_H
#define RENDERTRANSLATEDWITHBADNESS_H

#include <QtGui>
#include <QString>

#include "pMolORF.h"
#include "RenderObject.h"

class RenderTranslatedWithBadness : public RenderObject
{
  public:
    QString s;

    RenderTranslatedWithBadness(const QString &p_s, int index, int length, pMolORF* orf, int x, int y, int p_charWidth, int p_charSize);
    ~RenderTranslatedWithBadness();

    virtual void Render(QPainter *painter, int x, int y);

  protected:
    short int* buffer;
};

#endif
