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

#include <cstdlib>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsItem>
#include "pMolMpsv/pMolMpsvGlob.h"
#include "pMolMpsv/pMolMpsvGlobPath.h"

//---------------------------------------------------------------------------------------
///Constructors and destructors
pMolMpsvGlob::pMolMpsvGlob(
pMolMpsvWrap* p_parentWrapper, pMolMpsvStyle* p_style, int i0, int i, int p_priority)
{
  style = p_style;
  uc = style->uc;
  uca = style->uca;
  priority = p_priority;
  parentWrapper = p_parentWrapper;

  //set the direction of the glob
  if (parentWrapper->getLength() < 0) 
    dir = -1; else dir = 1;

  //set the x position of the glob (relative to parent)
  if (dir>0)
    x = uc * (float) ( parentWrapper->getIndex() -i0 + i);
  //  x = uc * (float) (parentWrapper->getIndex() - i);
  else
    x = uc * (float) ( parentWrapper->getIndex() -i0 - i);

  //make a new mask
  mask = new pMolMpsvMask(uc);
  setHandlesChildEvents(false);
};

pMolMpsvGlob::~pMolMpsvGlob()
{
  if (mask!=NULL) delete mask;
};

//---------------------------------------------------------------------------------------
///basic functions
void pMolMpsvGlob::addGlob(pMolMpsvGlob* glob)
{
  if (glob!=NULL) globs.append(glob);
};

int pMolMpsvGlob::getPriority()
{
  return priority;
};

int pMolMpsvGlob::getX()
{
  return x;
};

int pMolMpsvGlob::getDir()
{
  return dir;
};

pMolMpsvMask* pMolMpsvGlob::getMask()
{
  return mask;
};

QRectF* pMolMpsvGlob::getBoundingRect()
{
  return &boundingRectValue;
};


void pMolMpsvGlob::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  qDebug() << "glob::mousePressEvent says hello from "+parentWrapper->getName()+", about to emit signal";
  qDebug() << handlesChildEvents();
  parentWrapper->userEvent(0,0);
  //emit mouseEvent(event);
};

void pMolMpsvGlob::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
//  QList<QGraphicsItem*> childList = children();
//  for (int i = 0; i < childList.size(); ++i) childList.at(i)->paint(painter, option, widget);
};

QRectF pMolMpsvGlob::boundingRect() const 
{ 
  return childrenBoundingRect ();
};
//---------------------------------------------------------------------------------------
///returns width of text rendered using sequence font
int pMolMpsvGlob::getSequenceTextWidth(const QString &text)
{
  QFontMetrics fm(style->sequenceFont);			//need metrics
  return fm.width(text);	
};


//---------------------------------------------------------------------------------------
///returns width of text rendered with annotation font
int pMolMpsvGlob::getAnnotationTextWidth(const QString &text)
{
  QFontMetrics fm(style->annotationFont);		//need metrics
  return fm.width(text);	
};

//---------------------------------------------------------------------------------------
///at sequence text (fixed-width font)
void pMolMpsvGlob::addSequenceText(int index, qreal x, qreal y, const QString &text, const QColor &color)
{	 

  pMolMpsvGlobSequence* sequence = new pMolMpsvGlobSequence(parentWrapper, index, this);
  sequence->setFont(style->sequenceFont);
  sequence->setPlainText(text);
  sequence->setDefaultTextColor(color);
  sequence->setPos(x, y);
  sequence->setZValue(Z_sequenceText);
  mask->addMask(x, y+0.6*uc, getSequenceTextWidth(text), 1.6*uc);

/*
  QGraphicsSimpleTextItem* textItem = new QGraphicsSimpleTextItem(this);
  textItem->setFont(style->sequenceFont);
  textItem->setText(text);
  textItem->setPen(QPen(color));
  textItem->setPos(x, y);
  textItem->setZValue(Z_sequenceText);
  mask->addMask(x, y+0.6*uc, getSequenceTextWidth(text), 1.6*uc);
*/
};

//---------------------------------------------------------------------------------------
///add text for annotation (easy to read font)
void pMolMpsvGlob::addAnnotationText(qreal x, qreal y, const QString &text, const QColor &color)
{	 
  QGraphicsTextItem* textItem = new QGraphicsTextItem(this); 
  textItem->setFont(style->annotationFont);
  textItem->setPlainText(text);
  textItem->setDefaultTextColor(color);
  textItem->setPos(x, y-uca/2);
 // qDebug() << "uca" << uc << uca;
  textItem->setZValue(Z_annotationText);
  mask->addMask(x, y-uc/2.2, getAnnotationTextWidth(text), uc*1.2);
};

//---------------------------------------------------------------------------------------
///add an ellipse
void pMolMpsvGlob::addEllipseItem(qreal x, qreal y, qreal w, qreal h, const QBrush &brush)
{
  QGraphicsEllipseItem* ellipseItem =
    new QGraphicsEllipseItem(x,y,w,h,this);
  ellipseItem->setBrush(brush);
  ellipseItem->setPen(QPen(Qt::NoPen));
  mask->addMask(x, y, w, h);
};

//---------------------------------------------------------------------------------------
///Right pointing arrow
void pMolMpsvGlob::addRightArrow(qreal x, qreal y, qreal w, qreal h, const QBrush &brush)
{
  QPolygonF polygon;				//polygon to store it
  polygon << QPointF(x, y)			//point 1
          << QPointF(x + w, y + h/2)		//point 2
          << QPointF(x, y + h);			//point 3

  QGraphicsPolygonItem* polygonItem = 
    new QGraphicsPolygonItem(polygon, this);
  polygonItem->setBrush(brush);
  polygonItem->setPen(QPen(Qt::NoPen));
  mask->addMask(x, y, w, h);			//mask the shape in
};

//---------------------------------------------------------------------------------------
///adds a rectangle filled with brush
void pMolMpsvGlob::addRectItem(qreal x, qreal y, qreal w, qreal h, const QBrush &brush)
{
  QGraphicsRectItem* rectItem = 
    new QGraphicsRectItem(x,y,w,h,this);
  rectItem->setBrush(brush);
   rectItem->setPen(QPen(Qt::NoPen));
  mask->addMask(x, y, w, h);
};

//--------------------------------------------------------------------------------------
///frees up memory used by mask but first stores its bounding Rect
void pMolMpsvGlob::compact()
{
  mask->setRect(boundingRectValue);		//remember the bounding rectangle from the mask
  if (mask!=NULL) delete mask;	//we don't need the mask anymore so delete it
  mask = NULL;				//tag as deleted
};

//---------------------------------------------------------------------------------------
///transform glob so that most bottom left of bounding rect is (0,0)
void pMolMpsvGlob::normalize()
{
  float y = mask->normalize();		//normalize the mask and get y offset to do this
//  translate(0.0, y);			//translate by the appropriate amount
  moveBy(0.0, y);
};

//---------------------------------------------------------------------------------------
///basic random function
int pMolMpsvGlob::rnd(int max)
{
  return int(((float) max)*rand()/(RAND_MAX + 1.0));
};

//---------------------------------------------------------------------------------------
///minimizes total height of glob by shifting around child globs and
///frees up as much memory as possible when done
void pMolMpsvGlob::minimize()
{
  //if there are no child globs, just normalize and return
  if (globs.count() == 0)
  { 
    normalize();
    return;
  };

  //otherwise get ready to optimize
  QList <pMolMpsvGlob*> globs1, globs1_temp;
  QList <pMolMpsvGlob*> globs2, globs2_temp;

  foreach (pMolMpsvGlob* glob, globs)
  { if (glob->getPriority() == 1) globs1.append(glob);
    if (glob->getPriority() == 2) globs2.append(glob);
  };

  pMolMpsvMask* bestMask = new pMolMpsvMask(uc);
  pMolMpsvMask* tempMask = new pMolMpsvMask(uc);
  pMolMpsvGlobPath* bestPath = new pMolMpsvGlobPath();
  pMolMpsvGlobPath* tempPath = new pMolMpsvGlobPath();
  bool virgin = true;

  //this is the optimization loop
  for (int i=0; i<iterateMax; i++)
  {
    //re-set everything
    tempPath->reset();
    globs1_temp = globs1;
    globs2_temp = globs2;
    tempMask = mask->copy(tempMask);

    //get everything ready
    int y;
    pMolMpsvGlob* glob;

    //search path with high priority ones first
    while (!globs1_temp.empty())
    {
      glob = globs1_temp.takeAt(rnd(globs1_temp.count()));
      if (glob->getDir() > 0)
        y = tempMask->fitPos(glob->getX()/uc, glob->getMask());
      else
        y = tempMask->fitNeg(glob->getX()/uc, glob->getMask());
      tempPath->add(glob, y);
    };

    //low priority ones now
    while (!globs2_temp.empty())
    {
      glob = globs2_temp.takeAt(rnd(globs2_temp.count()));
      if (glob->getDir() > 0)
        y = tempMask->fitPos(glob->getX()/uc, glob->getMask());
      else
        y = tempMask->fitNeg(glob->getX()/uc, glob->getMask());
      tempPath->add(glob, y);
    };

    //if we have found a better solution, copy as the best path and mask
    if (virgin || (tempMask->getHeight() < bestMask->getHeight()))
    {
      virgin = false;			//no longer virginal
      bestMask = tempMask->copy(bestMask); 
      bestPath = tempPath->copy(bestPath);
    };
  };

  //bind down to the best path
  mask = bestMask->copy(mask);		//copy the best mask to this glob's mask
  bestPath->bind(this);		//bind globs to this glob's QGraphicsItem
  normalize();				//normalize this glob

  //clean up operation
  delete tempMask;
  delete bestMask;
  delete bestPath;
  delete tempPath; 
};
