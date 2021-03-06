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

#include <QScrollBar>

#include "vSequenceMap.h"

///constructor
vSequenceMap::vSequenceMap(pMolKernelInterface* p_interface) : pMolStackObject(), QAbstractScrollArea()
{
    setBackgroundRole(QPalette::Base);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    totalHeight = 1;
    reSized = true;
    interface = p_interface;

    molecule = dynamic_cast<pMolDNA*> (interface->peek(1));    //setViewportMargins(0,0,0,0);

    if (molecule!=NULL) 
      interface->msg("vSequenceMap: ok bound to "+molecule->name);
    else
      interface->msg("vSequenceMap: failed to bind molecule");
};

vSequenceMap::vSequenceMap(pMolKernelInterface* p_interface, pMolElement* p_molecule) : pMolStackObject(), QAbstractScrollArea()
{
    setBackgroundRole(QPalette::Base);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    interface = p_interface;
    totalHeight = 1;
    reSized = true;
    justBorn = true;
    molecule = p_molecule;    //setViewportMargins(0,0,0,0);
    setWindowTitle(molecule->getHierarchialName());
    badnessHighlighted = true;
};

vSequenceMap::~ vSequenceMap() { };

///I've no clue what this is for
QSize vSequenceMap::minimumSizeHint() const
{
   return QSize(800, 400);
}

///I've no clue what this is for either
QSize vSequenceMap::sizeHint() const
{
   return QSize(800, 600);
}

///Marks the object has having been resized
///then goes about the usual Qt resize business
void vSequenceMap::resizeEvent(QResizeEvent *event)
{
   reSized = true;			
   QAbstractScrollArea::resizeEvent(event);
}

bool vSequenceMap:: type(int i)
{
  if (i==pMolObject::WINDOW) return true; else return pMolStackObject::type(i);
}

int vSequenceMap::clip(pMolElement* dna, int index, int length, int* tempIndex,int * tempLength, bool* ia, bool* la)
{
  int dna_index = dna->getIndex();
  int dna_length = dna->getLength();
  //see if reverse or not
  if (dna_length > 0)
  { //forward direction
    if ((dna_index  > index + length)||(dna_index + dna_length < index)) return 0;
    //lets clip the edges if needed
    if (dna_index < index) 
    { *tempIndex = index;         *ia=true;  }
    else 
    { *tempIndex = dna_index;      *ia = false;  };

    if (dna_index+dna_length < index+length) 
    { *tempLength = dna_length;      *la=false; }
    else
    { *tempLength = index+length-dna_index;      *la=true;};

    *tempLength-= *tempIndex - dna_index;
    return 1;
  }
  else
  { //reverse direction
//  qDebug() << dna->name << index << length << dna->index << dna->length;
    if ((dna_index < index)||(dna_index + dna_length >index + length)) return 0;
    //lets clip the edges if needed
    if (dna_index > index+length)
    { *tempIndex = index+length; *ia = true; }
    else
    { *tempIndex = dna_index; *ia = false; };

    if (dna_index+dna_length < index) 
    { *tempLength = index-dna_index; *la = true; }
    else 
    {*tempLength = dna_length;  *la = false; };

    *tempLength-= *tempIndex - dna_index;
    return -1;
  };
  return 0;
};


RenderObjectContainer* vSequenceMap::renderSequence(pMolElement* dna, int frameIndex, int frameLength, int charWidth, int charSize)
{
  QString number;  
  bool ia, la;
  int tempIndex, tempLength;  
  int dir = clip(dna,frameIndex,frameLength,&tempIndex,&tempLength, &ia, &la);
  RenderObjectContainer* container = new RenderObjectContainer(0,0,(pMolObject*) dna);
  container->addLeftText(dna->getSequence(frameIndex-dna->getIndex(), tempLength).toLower(),0,0, charWidth, charSize);
  container->addRightText(number.setNum(frameIndex-dna->getIndex()),-10,0,charWidth, charSize); 

  //ok this is very messy, but need some transparent way for layer to release parent classes in an elegent way
  if (dna->type(pMolObject::ORF)) 
  {
    pMolElement* potentialORF = dna;
    while ((potentialORF!=NULL) && potentialORF->type(pMolObject::LAYER)) potentialORF=potentialORF->pMolParent;
    pMolORF* orf = dynamic_cast <pMolORF*> (potentialORF);
    QString translated = orf->translate(tempIndex-dna->getIndex(), tempLength).toLower();
    container->addTranslatedWithBadness(translated, tempIndex-dna->getIndex(), tempLength,orf, 0,-charWidth*1-3, charWidth, charSize);
    //container->addRenderObject(renderORF(dynamic_cast <pMolORF*> (potentialORF), frameIndex, frameLength, charWidth, charSize));
  };

  int i=0;
  pMolElement* element;
  RenderObject* object;
 //do the ORFs first;
  while ((element=dna->getElement(&i))!=NULL)
    if (element->type(pMolObject::ORF))
    {
      object = (RenderObject*) render(element, frameIndex-dna->getIndex(), frameLength, charWidth, charSize);
      container->addRenderObject(object);
    }

  //do the annotations next
  while ((element=dna->getElement(&i))!=NULL)
    if (!element->type(pMolObject::ORF))
    {
      object = (RenderObject*) render(element, frameIndex-dna->getIndex(), frameLength, charWidth, charSize);
      container->addRenderObject(object);
    }

  return container;
};

//------------------------------------------------------------------------------------------------------------
///Render an annotation onto the virtual map
RenderObjectContainer* vSequenceMap::renderAnnotation(pMolElement* dna, int frameIndex, int frameLength, int charWidth, int charSize)
{
  QString number;
  bool ia, la;
  int tempIndex, tempLength;
  int dir = clip(dna,frameIndex,frameLength,&tempIndex,&tempLength, &ia, &la);

  if (dir==1) 
  { RenderObjectContainer* container = new RenderObjectContainer(charWidth*(tempIndex-frameIndex),0,(pMolObject*) this); 
//    container->addAnnotatedText(dna->getName(), charWidth*tempLength/2, -charWidth, charWidth, charSize);
//    container->addSquareBrace(0,-charWidth,tempLength*charWidth-1);
    container->addAnnotation(dna->getName(), 0,-charWidth,tempLength*charWidth-1, charWidth, charSize);
    if (!ia) container->addLine(0,-charWidth,0,-charWidth*3/2);
    if (!la) 
      if (dna->type(pMolObject::PRIMER))
        container->addLine(tempLength*charWidth-1, -charWidth, tempLength*charWidth - charWidth/2, -charWidth*3/2);
      else
        container->addLine(tempLength*charWidth-1, -charWidth, tempLength*charWidth-1, -charWidth*3/2);
    return container;
  }
  else if (dir==-1)
  { 
    RenderObjectContainer* container = new RenderObjectContainer(charWidth*(tempIndex-frameIndex),0,(pMolObject*) this); 
  //  container->addAnnotatedText(dna->getName(), -charWidth*tempLength/2, -charWidth, charWidth, charSize);
    container->addSquareBrace(0,-charWidth,-tempLength*charWidth-1);
    container->addAnnotation(dna->getName(), 0, -charWidth, -tempLength*charWidth-1, charWidth, charSize);
    if (!ia) container->addLine(0,-charWidth,0,-charWidth*3/2);
    if (!la) 
      if (dna->type(pMolObject::PRIMER))
        container->addLine(-tempLength*charWidth-1, -charWidth, -tempLength*charWidth - charWidth/2 +1, -charWidth*3/2);
      else
        container->addLine(-tempLength*charWidth-1, -charWidth, -tempLength*charWidth-1, -charWidth*3/2);

    container->rotate(charWidth);
    return container;
  }
  else return NULL;
 ///need to implement negative direction and sub-annotations later
};


//------------------------------------------------------------------------------------------------------------
///Render an ORF onto the virtual map
RenderObjectContainer* vSequenceMap::renderORF(pMolORF* dna, int frameIndex, int frameLength, int charWidth, int charSize)
{
       //qDebug() << "about to render ORF";
   if (dna->codonCodex==NULL)
   {  pMolObject* object = interface->lookup(dna->codonCodexName);
      if (object == NULL) return NULL;
      if (!object->type(pMolStackObject::CODONCODEX)) return NULL;
      dna->codonCodex = dynamic_cast<pMolCodonCodex*>(object);
   };

   int tempIndex, tempLength;
  bool ia, la;
   int dir = clip(dna,frameIndex,frameLength,&tempIndex,&tempLength, &ia, &la);
   if (dir==1) 
   { QString translated = dna->translate(tempIndex-dna->getIndex(), tempLength).toLower();
     RenderObjectContainer* container = new RenderObjectContainer(charWidth*(tempIndex-frameIndex),0,(pMolObject*) this);
     //if (badnessHighlighted)
     container->addTranslatedWithBadness(translated, tempIndex-dna->getIndex(), tempLength,dna, 0,0, charWidth, charSize);
     //else
     //container->addTranslatedText(translated,0,0, charWidth, charSize);
     //container->addTranslatedText(translated,0,-charWidth*1-3, charWidth, charSize);
    // container->addRightText(dna->name,-10,0,-charWidth*1-3, charSize); 
     container->addRightText(dna->name,0,0,charWidth, charSize); 
     //container->addAnnotatedText(dna->name, charWidth*(tempLength)/2, -charWidth*3, charWidth, charSize);
     //container->addSquareBrace(0,-charWidth*2-2,tempLength*charWidth);
     return container;
   } 
   else return NULL;
 ///need to implement negative direction and sub-annotations later
   //foreach (pMolElement* element, dna->pMolElements) 
   //container->addRenderObject((RenderObject*) render(element, tempBaseStart,tempBaseEnd, charWidth));

   //return container;
};

//------------------------------------------------------------------------------------------------------------------
///starts rendering routines for different members
RenderObjectContainer* vSequenceMap::render(pMolElement* dna, int frameIndex, int frameLength, int charWidth, int charSize)
{
  if (dna->type(pMolStackObject::RAW)) return renderSequence(dna, frameIndex, frameLength, charWidth, charSize);  else
  if (dna->type(pMolStackObject::ORF)) return renderORF(dynamic_cast <pMolORF*> (dna), frameIndex, frameLength, charWidth, charSize);  else
  if (dna->type(pMolStackObject::ANNOTATION)) return renderAnnotation(dna, frameIndex, frameLength, charWidth, charSize);  else
  return NULL;
}

//------------------------------------------------------------------------------------------------------------------
///Generates renderObjectCollections when required, i.e.
///when re-sized, molecule altered; adjusts the vertical scrollbar,
///and paints all visible renderOjbectCollections
void vSequenceMap::paintEvent(QPaintEvent *)
{
  //safety first 
  QTime time = QTime();
  time.start();
 
  if (molecule==NULL) return;
  //First, let's see how big the window is and how many bases we can spanIdRole
    QFont font("Courier", charSize);				//need fixed width font
    font.setFixedPitch(true);
    QPainter painter(viewport());
    painter.setFont(font);
    QFontMetrics fm(font);
    int charWidth   = fm.width("cagtnrlpyt") / 10;	//width of each cha
    int lineHeight  = charWidth*3;            //fm.height("cagtnrlpyt");
    int numberWidth = round(log10(molecule->getLength()+molecule->getIndex())) * charWidth + 2*charWidth ;	//how much space for the numbers?
    int basePerLine = (width()-numberWidth-32)/charWidth;

    int currentIndex = molecule->getIndex();
    int currentBase  = 0;
    QString number = QString();
    QScrollBar* scrollbar = verticalScrollBar();

    //we go here and re-virtual-render everything if the window has been re-sized 
    //as well as adjusting the vertical scroll bar
    if (reSized)
    {
       float r = ((float) scrollbar->sliderPosition())/((float) totalHeight);           //get relative position of where we are
       while (!renderObjects.isEmpty()) delete renderObjects.takeFirst();		//clean up previous render Elements
       int grey = 0;
       totalHeight = 0;									//clear cumulative height
       do {
         RenderObjectContainer* container = renderSequence(molecule, currentBase + currentIndex, basePerLine, charWidth, charSize);
         if ((grey++) & 1)
           container->fill(42,-container->rect.height()-container->rect.y(),
           container->rect.width()-container->rect.x(), container->rect.height(), 
           QColor(215,215,200,128));
         renderObjects.append(container);
         currentBase += basePerLine;
         totalHeight+=container->rect.height();						//measure total height of render
       } while (currentBase<molecule->getLength());

       scrollbar->setMinimum(1);							//re-set the scrollbar
       scrollbar->setMaximum(totalHeight-height()+10);					//leave a bit of room at the bottom	
   //     scrollbar->setMaximum(totalHeight);
       scrollbar->setPageStep(height());						//adjust the page step to the view port's size
       scrollbar->setSliderPosition(round(r*totalHeight));				//set the slider position correctly from the relative value we had stored
       scrollbar->setSingleStep(charWidth);

       reSized = false;								//label as reSized!
    } 

    //This bit paints only the renderObjectContainers above that are visible on this viewport

    int y = 1; //renderObjects.at(1)->rect.height();

    int virtualy = scrollbar->sliderPosition();
    if (totalHeight < height()) virtualy = 0;
 
    for (int i = 0; i < renderObjects.size(); ++i)					//ok let's paint what is visible in the view-port
    {
      if (y-renderObjects.at(i)->rect.y() >= virtualy) 
        renderObjects.at(i)->Render(&painter, numberWidth, y-renderObjects.at(i)->rect.y()-virtualy); //only if visible
      y+= renderObjects.at(i)->rect.height();
      if (y>(virtualy + height())) break;						//if over the edge of the view port, stop looping
    }
    qDebug() << "timer" << time.elapsed();
}

vSequenceMap* vSequenceMap::create(pMolKernelInterface* interface, pMolCmd* cmd)
{
  qDebug()<< "reached vInspector::create";
  //initial check of parameters
  if (cmd==NULL) return NULL;				//no parameters sent at all
  if (cmd->type()!=pMolCmd::LIST) return NULL;	//parameter isn't a list
  //cast up as a list

  pMolObject* object = interface->interpret(cmd);
  if (object==NULL) return NULL;
  qDebug()<< "reached rawSeq::second dynamic cast" << object->type(pMolObject::LIST);
  //interpret the parameters
  pMolStackList* objectList = dynamic_cast <pMolStackList*> (interface->interpret(cmd));	
  qDebug()<< "reached rawSeq::second dynamic cast" << objectList->type(pMolObject::LIST);
  //get the length and decide what to do next
  int length = objectList->length();			//get the lenght once
  qDebug()<< "reached rawSeq::length" << length;
  if (length==0) return NULL;				//leave if no members

  //if more than one ultimately, I want it to return an object list 
  //opens multiple windows - should we put them on a single widget? 
  for (int i=0; i<length; i++)
  {
    pMolStackObject* object = objectList->at(i);
    if (object->type(pMolStackObject::DNA))
    { vSequenceMap* map = new vSequenceMap(interface, dynamic_cast<pMolElement*> (object));
      map->show();
    }
  };
  return NULL;
};
