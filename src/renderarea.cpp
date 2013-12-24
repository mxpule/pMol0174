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

#include "renderarea.h"
#include <QtGui>

///constructor
RenderArea::RenderArea(QWidget *parent, pMolecule *p_pMol) : QAbstractScrollArea(parent)
{
    setBackgroundRole(QPalette::Base);

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    //setViewportMargins(0,0,0,0);
    //setFrameStyle(QFrame::NoFrame);
    pMol = p_pMol;
    reSized = true;
    setWindowTitle(pMol->name);
    totalHeight = 1;
}

///I've no clue what this is for
QSize RenderArea::minimumSizeHint() const
{
   return QSize(400, 400);
}

///I've no clue what this is for either
QSize RenderArea::sizeHint() const
{
   return QSize(400, 200);
}

///Marks the object has having been resized
///then goes about the usual Qt resize business
void RenderArea::resizeEvent(QResizeEvent *event)
{
   reSized = true;			
   QAbstractScrollArea::resizeEvent(event);
}

///Generates renderObjectCollections when required, i.e.
///when re-sized, molecule altered; adjusts the vertical scrollbar,
///and paints all visible renderOjbectCollections
void RenderArea::paintEvent(QPaintEvent *)
{
  //First, let's see how big the window is and how many bases we can spanIdRole
    QFont font("Courier", 14, QFont::Normal);						//need fixed width font
    QPainter painter(viewport());
    painter.setFont(font);
    QFontMetrics fm(font);
    int charWidth   = fm.width("cagtnrlpyt") / 10;	//width of each cha
    int lineHeight  = charWidth*3;            //fm.height("cagtnrlpyt");
    int numberWidth = round(log10(pMol->sequence.length())) * charWidth + charWidth ;
    int basePerLine = (width()-numberWidth-32)/charWidth;

    int currentBase = 0;
    QString number = QString();
    QScrollBar* scrollbar = verticalScrollBar();

    //we go here and re-virtual-render everything if the window has been re-sized 
    //as well as adjusting the vertical scroll bar
    if (reSized)
    {
       float r = ((float) scrollbar->sliderPosition())/((float) totalHeight);           //get relative position of where we are
       while (!renderObjects.isEmpty()) delete renderObjects.takeFirst();		//clean up previous render Elements

       totalHeight = 0;									//clear cumulative height
       do {
         qDebug() << "---------Line----------" << currentBase;
         RenderObjectContainer* container = pMol->renderSequenceDump(NULL, currentBase,currentBase+basePerLine, charWidth);
         renderObjects.append(container);
         currentBase += basePerLine;
         totalHeight+=container->rect.height();						//measure total height of render
       } while ((currentBase + basePerLine)<(pMol->sequence.length()));

       scrollbar->setMinimum(1);							//re-set the scrollbar
       scrollbar->setMaximum(totalHeight-height()+100);					//leave a bit of room at the bottom	
       scrollbar->setPageStep(height());						//adjust the page step to the view port's size
       scrollbar->setSliderPosition(round(r*totalHeight));				//set the slider position correctly from the relative value we had stored
       scrollbar->setSingleStep(charWidth);

       reSized = false;								//label as reSized!
    } 

    //This bit paints only the renderObjectContainers above that are visible on this viewport

    int y = 1; //renderObjects.at(1)->rect.height();
    int virtualy = scrollbar->sliderPosition();

    for (int i = 0; i < renderObjects.size(); ++i)					//ok let's paint what is visible in the view-port
    {
      y+= renderObjects.at(i)->rect.height();
      if (y>=virtualy) renderObjects.at(i)->Render(&painter, numberWidth, y-virtualy); //only if visible
      if (y>(virtualy + height())) break;						//if over the edge of the view port, stop looping
    }
}

