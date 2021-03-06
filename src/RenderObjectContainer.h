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
#ifndef RENDEROBJECTCONTAINER2_H
#define RENDEROBJECTCONTAINER2_H

#include <QtGui>
#include <QString>

#include "pMolObject.h"
#include "RenderLine.h"
#include "RenderObject.h"
#include "pMolORF.h"
#include "RenderObjectCenteredText.h"
#include "RenderObjectLeftText.h"
#include "RenderObjectRightText.h"
#include "RenderObjectSquareBrace.h"
#include "renderObjectAnnotation.h"
#include "RenderObjectAnnotationText.h"
#include "RenderTranslatedText.h"

class RenderObjectContainer : public RenderObject
{
  public:
    int basex, basey;

    pMolObject* pMolObjectParent;

    QList <RenderObject *> renderObjects;

    RenderObjectContainer(int x, int y, pMolObject* p_pMolObjectParent);
    void addRenderObject(RenderObject* object);
    virtual ~RenderObjectContainer();
    virtual void Render(QPainter *painter, int x, int y);
    virtual void translate(int x, int y);

    void addCenteredText(const QString &p_s, int x, int y, int charWidth, int charSize);
    void addAnnotation(const QString &p_text, int x, int y, int w, int charWidth, int charSize);
    void addAnnotatedText(const QString &p_s, int x, int y, int charWidth, int charSize);
    void addLeftText(const QString &p_s, int x, int y, int charWidth, int charSize);
    void addRightText(const QString &p_s, int x, int y, int charWidth, int charSize);
    void addTranslatedText(const QString &p_s, int x, int y, int charWidth, int charSize);
    void addTranslatedWithBadness(const QString &p_s, int index, int length, pMolORF* orf, int x, int y, int p_charWidth, int p_charSize);
    void addSquareBrace(int x, int y, int w);
    void addLine(int x0, int y0, int x1, int y1);
    void fill(int x, int y, int w, int h, QColor p_c);
    void rotate(int p_charWidth);

  protected:
   bool filly;
   bool rotated;
   QRect fillRect;
   QColor fillColor;
};


#endif