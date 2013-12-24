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
 

#include "./pLabTools/pLabRectTextItem.h"

pLabRectTextItem::pLabRectTextItem(int p_w, int p_h, int p_color, const QString &p_text)
{
  w=p_w;
  h=p_h;
  text = p_text;
  color = p_color;
};

QRectF pLabRectTextItem::boundingRect() const
{
  qreal penWidth = 1;
  return QRectF(-10 - penWidth / 2, -10 - penWidth / 2,
  20 + penWidth / 2, 20 + penWidth / 2);
}


void pLabRectTextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget=0) 
{
  painter->fillRect(0,0,w,h, QBrush(QColor(color)));
  painter->drawText(0,0,text);
}


void pLabRectTextItem::create(QGraphicsScene* scene, int x, int y, int w, int h, int color, const QString &text)
{
  qDebug() << x << y << w << h;
  pLabRectTextItem* item = new pLabRectTextItem(w, h, color, text);
  item->setPos(x,y);
  scene->addItem(item);
};