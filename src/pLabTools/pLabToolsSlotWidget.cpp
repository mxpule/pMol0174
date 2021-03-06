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
/*
#include "pLabToolsSlotWidget.h"

pLabToolsSlotWidget::pLabToolsSlotWidget(const QString &p_slot, int p_radius, pMolInterface* p_parent)
{
  slot = p_slot;
  parent = p_parent;
  r = p_radius;
  reset(0,"",false);
  setFixedSize(r*2+1, r*2+1);
};


pLabToolsSlotWidget::~pLabToolsSlotWidget()
{
};

//-----------------------------------------------------------------------
///makes nice colors for slots
QColor pLabToolsSlotWidget::niceColor(int ixix)
{
  int r = ((ixix * 1771) % 101) + 155;
  int g = ((ixix *  67) % 128) + 128;
  int b = ((ixix * 711) % 128) + 128;
  return QColor(r,g,b,255);
};


void pLabToolsSlotWidget::reset(int p_ixix, const QString &p_details, bool p_clicked)
{
  if (p_ixix)
  {
    ixix = p_ixix;
    name.setNum(ixix);
    color = niceColor(ixix);
    empty = false;
    clicked = p_clicked;
    details = "[MP"+name+"]"+p_details;
  }
  else
  {
    empty = true;
    clicked = p_clicked;
  };
  changed = true;
};

void pLabToolsSlotWidget::draw(QPainter* painter)
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
   painter->fillRect(-r,-r,r*2+1,r*2+1,brush);

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

void pLabToolsSlotWidget::paintEvent(QPaintEvent *event)
{
  if (changed)
  {
    QPainter painter(this);
    draw(&painter);
  }
  else QWidget::paintEvent(event);
};

void pLabToolsSlotWidget::highlight()
{
  qDebug() << "highlight: about to talk";
  parent->talk(details);
  clicked = true;
  changed = true;
  update();
};

void pLabToolsSlotWidget::unHighlight()
{
  if (clicked) 
  { clicked = false;
    changed = true;
    update();
  };
};

void pLabToolsSlotWidget::mousePressEvent(QMouseEvent *event)
{
  bool cacheClicked = clicked;
  if (!clicked) parent->talk(details); else parent->talk("\n\n\n");
  clicked = !cacheClicked;
  changed = true;
  update();
};
*/
