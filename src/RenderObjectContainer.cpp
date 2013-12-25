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

#include "RenderObjectContainer.h"
#include "RenderTranslatedWithBadness.h"

//-----------------------------------------------------------------------------------------

//! Constructor. Gives basic offset, also sets a parent molecular object
//! so when you click on this on the screen, we know what it is!
RenderObjectContainer::RenderObjectContainer(int x, int y, pMolObject* p_pMolObjectParent) : RenderObject()
{ 
  rect.setRect(x,y,0,0);
  basex=x; basey = y;							//since member render objects can have negative x, we need\

  fillRect.setRect(43,0,0,0);
  filly = false;
  rotated = false;
									//basex and basey as offsets that don't change as new objects fit
  pMolObjectParent = p_pMolObjectParent;				//so we know who this belongs to if we click on it
}

RenderObjectContainer::~RenderObjectContainer()
{
  while (!renderObjects.isEmpty()) delete renderObjects.takeFirst();	//recursive destructor
}

//-----------------------------------------------------------------------------------------

//! Recursive function which actually paints to the screen
void RenderObjectContainer::Render(QPainter *painter, int x, int y)
{
 
 // if (filly)
 // {
    QBrush brush(fillColor);
fillRect.setRect(basex, rect.y(), rect.width()+rect.x(), rect.height());

    if (filly)     painter->fillRect(fillRect.translated(x, y), brush);
 // };



  painter->save();
  painter->translate(x+basex, y+basey);
  if (rotated)
  {
    painter->translate(0,charWidth/2);
    painter->rotate(180.0);
  };
  for (int i = 0; i < renderObjects.size(); ++i) 			//recursively paint
    renderObjects.at(i)->Render(painter, 0, 0);
  painter->restore();
}


void RenderObjectContainer::translate(int x, int y)
{
  basex+=x;   basey+=y;
  rect.translate(x,y);
}
//-----------------------------------------------------------------------------------------

//! Add a Render Object, fit it so it doesnt't overlap
//! This fitting can be made much more sophisticated - some other day
void RenderObjectContainer::addRenderObject(RenderObject* object)
{
  if (object==NULL) return;						//if this is a dud, bail

  for (int i = 0; i < renderObjects.size(); ++i)			//make sure it doesn't overlap any 
    if (renderObjects.at(i)->rect.intersects(object->rect))		//pre-existing render objects
    {  //QRect irect = renderObjects.at(i)->rect.intersect(object->rect);
       int y0 = renderObjects.at(i)->rect.y();
       int y1 = object->rect.y();
       int h1 = object->rect.height();
       object->translate(0,y0-y1-h1);
    };

  rect = rect.united(object->rect.translated(basex,basey));		//merge this new box with outline of parent

  renderObjects.append(object);						//add it to our list.
}

//-----------------------------------------------------------------------------------------

//!Convenience function, adds centered text
void RenderObjectContainer::addCenteredText(const QString &p_s, int x, int y, int charWidth, int charSize)
{
  RenderObjectCenteredText* object = new RenderObjectCenteredText(p_s,x,y,charWidth, charSize);
  addRenderObject((RenderObject*) object);
}

//!Convenience function, adds annotation text
void RenderObjectContainer::addAnnotatedText(const QString &p_s, int x, int y, int charWidth, int charSize)
{
  RenderObjectAnnotationText* object = new RenderObjectAnnotationText(p_s,x,y,charWidth, charSize);
  addRenderObject((RenderObject*) object);
}

//!Convenience function, adds annotation text
void RenderObjectContainer::addAnnotation(const QString &p_text, int x, int y, int w, int charWidth, int charSize)
{
  renderObjectAnnotation* object = new renderObjectAnnotation(p_text,x,y,w,charWidth,charSize);
  addRenderObject((RenderObject*) object);
}

//!Convenience function, adds left justified text
void RenderObjectContainer::addLeftText(const QString &p_s, int x, int y, int charWidth, int charSize)
{
  RenderObjectLeftText* object = new RenderObjectLeftText(p_s,x,y,charWidth, charSize);
  addRenderObject((RenderObject*) object);
}

//!Convenience function, adds left justified text
void RenderObjectContainer::addTranslatedText(const QString &p_s, int x, int y, int charWidth, int charSize)
{
  RenderTranslatedText* object = new RenderTranslatedText(p_s,x,y,charWidth, charSize);
  addRenderObject((RenderObject*) object);
}

//!Convenience function, adds right justified text
void RenderObjectContainer::addRightText(const QString &p_s, int x, int y, int charWidth, int charSize)
{
  RenderObjectRightText* object = new RenderObjectRightText(p_s,x,y,charWidth, charSize);
  addRenderObject((RenderObject*) object);
}

//!Convenience function, adds a square brace
void RenderObjectContainer::addSquareBrace(int x, int y, int w)
{
  RenderObjectSquareBrace* object = new RenderObjectSquareBrace(x,y,w);
  addRenderObject((RenderObject*) object);
}

//!Convenience function, add a line
void RenderObjectContainer::addLine(int x0, int y0, int x1, int y1)
{
  RenderLine* object = new RenderLine(x0,y0,x1,y1);
  addRenderObject((RenderObject*) object);
}

void RenderObjectContainer::addTranslatedWithBadness
(const QString &p_s, int index, int length, pMolORF* orf, int x, int y, int p_charWidth, int p_charSize) 
{
  RenderTranslatedWithBadness* object = new RenderTranslatedWithBadness
  (p_s, index, length, orf, x,y,p_charWidth,p_charSize);
  addRenderObject((RenderObject*) object);
}


void RenderObjectContainer::fill(int x, int y, int w, int h, QColor p_c)
{
  filly = true;
  fillColor = p_c;
  fillRect.setRect(x,y,w,h);
  return;
};

void RenderObjectContainer::rotate(int p_charWidth)
{
  charWidth = p_charWidth;
  rotated = true;
  int x = rect.x();
  int y = rect.y();
  int h = rect.height();
  int w = rect.width();
  rect.setRect(x-w, -y-charWidth, w, h);
};
//-----------------------------------------------------------------------------------------
