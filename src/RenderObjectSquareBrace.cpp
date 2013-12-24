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
#include "RenderObjectSquareBrace.h"

RenderObjectSquareBrace::RenderObjectSquareBrace(int x, int y, int w) : RenderObject()
{
  //qDebug() << "RenderObjectSquareBrace::constructor"<<x<<y<<rect;
  rect.setRect(x,y,w,6);
}

void RenderObjectSquareBrace::Render(QPainter *painter, int x, int y)
{
  //qDebug() << "RenderObjectSquareBase::Render"<<x<<y<<rect.translated(x,y);

  QBrush brush(QColor(128, 255, 128,128));
    painter->fillRect(rect.translated(x,y), brush);
}
