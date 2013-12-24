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
#include "RenderObjectAnnotationText.h"

RenderObjectAnnotationText::RenderObjectAnnotationText(const QString &p_s, int x, int y, int p_charWidth, int p_charSize) : RenderObject()
{ 
  //may in future use clever things like fontmetrics here
  s = p_s;
  charSize = p_charSize;
  charWidth = p_charWidth;
  int textw = s.length()*charWidth;
  rect.setRect(x - textw/2,y-4,textw,charWidth+4);
  //qDebug() << "RenderOjectCenteredText::constructor-rect" << rect;
}

void RenderObjectAnnotationText::Render(QPainter *painter, int x, int y)
{
  //qDebug() << "RenderObjectCenteredText::Render"<<x<<y<<rect.translated(x,y);
  QFont font("Lucida Sans", charSize-4);				//need fixed width font
  font.setFixedPitch(true);
  painter->setFont(font);
  painter->drawText(rect.translated(x,y), Qt::AlignCenter, s);
};