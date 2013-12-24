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
#include <math.h>

#include "pMolCodon.h"
#include "pMolCodonCodex.h"
#include "RenderTranslatedWithBadness.h"

RenderTranslatedWithBadness::RenderTranslatedWithBadness
(const QString &p_s, int index, int length, pMolORF* orf, int x, int y, int p_charWidth, int p_charSize) : RenderObject()
{ 
  //may in future use clever things like fontmetrics here
  s = p_s.mid(0,length);
  charSize = p_charSize;
  charWidth = p_charWidth;
  int textw = s.length()*charWidth;
  rect.setRect(x,y-4,textw,charWidth+4);

  //now do the badness!
  pMolCodonCodex* codex = orf->codonCodex;
  int frame = index % codex->frameLength;
  index-=frame;
  buffer = new short int[s.length()+1];
  for (int i=0; i<s.length(); i++)
  {
    QString currentCode = orf->getSequence(index,codex->frameLength).toLower();
    pMolCodon* codon = codex->getCodon(currentCode);
    if (codon==NULL)
      buffer[i] = 255;
    else
      buffer[i] = round(255.0*(double)codon->frequency/(double)codon->codeBestFrequency);
    if (++frame >= codex->frameLength) 
    { frame=0; index+=codex->frameLength;    };			//next frame
  };
}

RenderTranslatedWithBadness::~RenderTranslatedWithBadness()
{
  delete []buffer;
};

void RenderTranslatedWithBadness::Render(QPainter *painter, int x, int y)
{
  QRect fillRect = QRect(rect.x(), rect.y(), charWidth, rect.height());
  fillRect = fillRect.translated(x,y);
  for (int i=0; i<s.length(); i++)
  {
    QBrush brush(QColor(255, buffer[i],buffer[i],128));
    painter->fillRect(fillRect, brush);
    fillRect = fillRect.translated(charWidth, 0);
  };

  QFont font("Courier", charSize);				//need fixed width font
  //QBrush brush(QColor(0,0,255,255));
  font.setFixedPitch(true);
  painter->setPen(QColor(0,0,255,255));
  painter->setFont(font);
  painter->drawText(rect.translated(x,y), Qt::AlignCenter, s);
  painter->setPen(QColor(0,0,0,255));


};