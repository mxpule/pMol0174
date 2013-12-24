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

#ifndef PMOLMPSV_H
#define PMOLMPSV_H

#include <QtGui>
#include <QMutex>

#include "pMolStackObject.h"
#include "pMolMpsv/pMolMpsvWrapper.h"
#include "pMolMpsv/pMolMpsvGlob.h"
#include "pMolMpsv/pMolMpsvStyle.h"
#include "pMolMpsv/pMolMpsvThread.h"

///Martin Pule Sequence Viewer is a flexible Qt QGraphicScene based class for
///visualizing dna sequence data. Inherits both <i>QGraphicsView</i> and also
/// <i>pMolStackObject. 
class pMolMpsv : public QGraphicsView, pMolStackObject
{
  Q_OBJECT

  public:
    ///constructor - pass the pMolElement you want bound to the object
    pMolMpsv(pMolElement* p_element);

    ///virtual desctructor.
    virtual ~pMolMpsv();

   ///returns type as VIEWABLE and EXEC
   virtual bool type(int t);

   ///handles commands passed to it
   virtual pMolStackObject* exec(pMolCmd* cmd, pMolKernelInterface* interface);

   ///static member for creating this object
   static pMolStackObject* create(pMolKernelInterface* interface, pMolCmd* cmd);


   public slots:
     void checkUpdates();

  private :
    QMutex mutex;

    int widthOld;			//store the last width
    bool updated;			//have we been re-sized
    pMolMpsvStyle* style;		//the style to use
    QGraphicsScene scene;		//the scene
    pMolElement* element;		//the element we are bound to
    pMolMpsvWrapper* rootWrapper;	//the wrapper binding to this element
    pMolMpsvThread* thread;

    virtual void resizeEvent(QResizeEvent *event);
    virtual void paintEvent(QPaintEvent *event);

    void makeGlobs();
    void deleteGlobs();
    pMolStackObject* c_del(pMolCmd* cmd, pMolKernelInterface* interface);
    pMolStackObject* c_pdf(pMolCmd* cmd, pMolKernelInterface* interface);
    QList <pMolMpsvGlob*> globs;	

    static const int topMargin = 8;
    static const int botMargin = 24;
    static const int sideMargin = 32;
};

#endif
