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

#include <QPrinter>
#include <QWidget>
#include <QGraphicsView>
#include "pMolMpsv.h"

//------------------------------------------------------------------------------
///constructors and destructors
pMolMpsv::pMolMpsv(pMolElement* p_element) : QGraphicsView()
{
  //gets all the base wrapper set up
  element = p_element;
  rootWrapper = pMolMpsvWrapper::bind(element);

  //get the viewport setup
  //style = new pMolMpsvStyle(12, "Efont Fixed", 8, "Lucidia Sans");
  style = new pMolMpsvStyle(12, "Courier", 8, "Lucidia Sans");
  setWindowTitle(element->getHierarchialName());
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setAlignment(Qt::AlignCenter);
  setResizeAnchor(QGraphicsView::AnchorViewCenter);
  resize(1200,400);

  //render the QGraphicsScence

  makeGlobs();			//add globs to the scene
  setScene(&scene);		//set the scene
  show();
  widthOld = width();

  //start the timer thread that checks for updates and triggers re-draws asynchronously
  thread = new pMolMpsvThread();
  QObject::connect(thread, SIGNAL(tickSignal()), this, SLOT(checkUpdates()));
  thread->start();
};

pMolMpsv::~pMolMpsv()
{
  deleteGlobs();
  delete style;
  delete rootWrapper;
};

bool pMolMpsv::type(int t)
{
  if (t==EXEC) return true;
  if (t==VIEWABLE) return true;
  return pMolStackObject::type(t);
};

//------------------------------------------------------------------------------
///Marks the object has having been resized then goes about the usual Qt resize business
void pMolMpsv::resizeEvent(QResizeEvent *event)
{
  updated = true;	
  widthOld = width();		
  QGraphicsView::resizeEvent(event);
}


//------------------------------------------------------------------------------
///if resized, re-render before painting
void pMolMpsv::paintEvent(QPaintEvent *event)
{
  if (updated)
    {
    //    delete rootWrapper;
    //    rootWrapper = pMolMpsvWrapper::bind(element);
        makeGlobs();
    };
  updated = false;
  QGraphicsView::paintEvent(event);
}

///the threading needs to make more sense
void pMolMpsv::checkUpdates()
{
  if (!mutex.tryLock()) return;			//if already busy, not point putting anything into
  updated = updated || rootWrapper->refresh();	//have any of the wrappers have changed?
  mutex.unlock();		
  if (updated) qDebug() << "chjecl updates" << updated;
  if (updated) viewport()->update();
};


//------------------------------------------------------------------------------
///empty the scene of graphics Items and delete them
void pMolMpsv::deleteGlobs()
{
  while (!globs.isEmpty())
  {pMolMpsvGlob* glob = globs.takeFirst();
   delete glob;
  };
};


//------------------------------------------------------------------------------
///assembles the graphical units vertically and places them on the scene
void pMolMpsv::makeGlobs()
{
  //delete any older globs now we have changed
  if (!mutex.tryLock()) return;
  deleteGlobs();				
  int delta_y = 12;

  //setup the parameters for the operation
  int basesPerLine = ((width()-2*sideMargin)/style->uc);	//how many bases will we fit per line
  int index = rootWrapper->getIndex();			//the index
  int length = rootWrapper->getLength();		//the length
  float y = topMargin;						//cummulative y location on the canvas
  int i = index;					//base counter
  int grey = 0;						//alternating grey and white background

  //go through the sequence and assemble the globs vertically
  do 
  {
    pMolMpsvGlob* glob = rootWrapper->makeGlob(i, i+basesPerLine, style, true);

    if (glob!=NULL)
    {
      glob->compact();
      scene.addItem(glob);
      globs.append(glob);
      y+=(glob->getBoundingRect()->height() + delta_y);
      glob->moveBy(sideMargin,y);

    //if ((grey++) & 1) glob->setBackGround(lightGray);
    };
    i+=basesPerLine;
  } 
  while (i < index + length);
  //QRectF rect = scene.sceneRect();
  //rect.setWidth(widthOld);
  scene.setSceneRect(0,0,widthOld, y+botMargin);

  updated = false;
  mutex.unlock();
 }

//------------------------------------------------------------------------------------------------
///prints the scene as a pdf
pMolStackObject* pMolMpsv::c_pdf(pMolCmd* cmd, pMolKernelInterface* interface)
{
  QString error;

  //if we didn't get any parameters, then send an error and return
  pMolStackList* list = pMolStackList::interpretParameter(cmd, interface,error);
  if (list==NULL) return makeError(interface, "mpsv", "pdf", error);

  //if there's one number parameter, cut bit off front or back depending if pos or neg
  if (!list->checkParameters(pMolObject::STRING, error))
    return makeError(interface, "pMolPrimerOrder", "create", error);

  pMolStackString* stackString = dynamic_cast <pMolStackString*> (list->at(0));
  QString filename = stackString->string;

  QPrinter printer;
  printer.setPageSize(QPrinter::A4);
  printer.setOutputFormat(QPrinter::PdfFormat);
  printer.setOutputFileName(filename+".pdf");
  QPainter painter(&printer);
  scene.render(&painter);

  return NULL;
};

pMolStackObject* pMolMpsv::c_del(pMolCmd* cmd, pMolKernelInterface* interface)
{
  qDebug() << "pMolMpsv::del" << "about to delete";
  deleteGlobs();
  return NULL;
};

//--------------------------------------------------------------------------------------------------------
///This handles command directed at this oject
pMolStackObject* pMolMpsv::exec(pMolCmd* cmd, pMolKernelInterface* interface)
{
  //no paramaters, return this as object
  if (cmd==NULL) return (pMolStackObject*) this;				//no paramaters, return me

  //if no command but not empty, something is wrong (needs to be empty or have subcommand) so return with error
  if (cmd->type() != pMolCmd::NAME) return makeError(interface,"exec","empty field or parameter list expected");
  pMolCmdName* cmdName = dynamic_cast<pMolCmdName*>(cmd);			//safe to assume this is a pMolCmdName

  if (cmdName->name == "pdf")	return c_pdf(cmdName->child, interface);
  if (cmdName->name == "del")	return c_del(cmdName->child, interface);
  return makeError(interface, "->","command not understood");
};


//-------------------------------------------------------------------------------
///static member that creates thus c
pMolStackObject* pMolMpsv::create(pMolKernelInterface* interface, pMolCmd* cmd)
{
  QString error;

  //initial check of parameters
  if (cmd==NULL) return NULL;				//no parameters sent at all

  pMolMpsv* map = NULL;

  pMolStackList* list = pMolStackList::interpretParameter(cmd, interface,error);
  if (list==NULL) return makeError(interface, "mpSV", "find", error);

  for (int i=0; i<list->length(); i++)
  {
    pMolStackObject* object = list->at(i);
    if (object->type(pMolStackObject::DNA))
    { map = new pMolMpsv(dynamic_cast<pMolElement*> (object));
      map->show();
    }
  };
  return map;
};
