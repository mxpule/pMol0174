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
#include "pMolSlotWidget.h"

pMolSlotWidget::pMolSlotWidget(int p_r,const QString &p_name, const QColor &p_color, bool p_empty)
{
  reset(p_r, p_name,p_color,p_empty, false); 
};

pMolSlotWidget::~pMolSlotWidget()
{
};

void pMolSlotWidget::reset(int p_r, const QString &p_name, const QColor &p_color, bool p_empty, bool p_clicked)
{
  r = p_r;
  name = p_name;
  color = p_color;
  empty = p_empty;
  clicked = p_clicked;
  changed = true;
};

void pMolSlotWidget::draw(QPainter* painter)
{
  painter->translate(r,r);

  QColor slotColor;
  QColor backColor;
  QColor textColor;

  //first, set the colors to use depending if clicked or not
  if (clicked)
  {
    backColor.setRgb(0x00,0x00,0x00,0xff);
    textColor.setRgb(0xff,0xff,0xff,0xff);
    slotColor.setRgb(0xff-color.red(),0xff-color.green(),0xff-color.blue(),0xff);
  }
  else
  {
    backColor.setRgb(0xff,0xff,0xff,0xff);
    textColor.setRgb(0x00,0x00,0x00,0xff);
    slotColor = color;
  };

  //the background square is the same for all
   QBrush brush(backColor);
   painter->fillRect(-r,-r,r*2,r*2,brush);

  //now draw slot if filled
  if (!empty)
  {
    painter->setBrush(slotColor);
    painter->drawEllipse(-r,-r,r*2,r*2);

    int l = name.length();
    int n;
    double scale;

    QFont font("Lucida Sans", -1+r/2);
    font.setFixedPitch(true);
    painter->setFont(font);
    QFontMetrics fm(font);
    int textLength = fm.width(name);	
    int charWidth = textLength/l;
    if (l>5) scale = (3.14*r/2) / (double)textLength;

    if (l>5) painter->scale(scale,scale);
    painter->setPen(textColor);
    painter->drawText(QRect(-r,-r,2*r,2*r), Qt::AlignCenter, name);
  }
};

void pMolSlotWidget::paintEvent(QPaintEvent *event)
{
  if (changed)
  {
    QPainter painter(this);
    draw(&painter);
  }
  else QWidget::paintEvent(event);
};

void pMolSlotWidget::mousePressEvent(QMouseEvent *event)
{
  clicked  = !clicked;
  changed = true;
  update();
};