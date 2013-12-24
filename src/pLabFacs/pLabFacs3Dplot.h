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

#ifndef PLABFACS3DPLOT_H
#define PLABFACS3DPLOT_H

#include <math.h>

#include <QGLWidget>
#include <QMouseEvent>

#include "./pMolStackObject.h"
#include "./pMolKernelInterface.h"
#include "./pLabFacs/pLabFacs.h"
#include "./pMolError/pMolError.h"

class pLabFacs3DPlot : public QGLWidget, public pMolStackObject
{
    Q_OBJECT

public:
    pLabFacs3DPlot(pMolKernelInterface* p_interface);
    ~pLabFacs3DPlot();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    static pMolStackObject* create(pMolKernelInterface* p_interface, pMolCmd* cmd);

protected:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);

    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    pMolKernelInterface* interface;

    void throwError(const QString &member, const QString &error);
    void fillVoxels();
    void resetVoxels(int n);
    void normalizeVoxels(int n);
    int* channelFilter(unsigned int max, int *shift, unsigned int *mask);

    GLuint makeObject();
    void quad(int x, int y, int z, int c);
    void drawCube(int x, int y, int z, int c);
    void extrude(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);
    void normalizeAngle(int *angle);

    GLuint object;
    int xRot;
    int yRot;
    int zRot;
    QPoint lastPos;
    QColor trolltechGreen;
    QColor trolltechPurple;

    pLabFacs* facsData;


    
    int res;			//resolution of voxel map
    quint8 voxelMax;		//maximum voxel density scored
    quint8* voxels;		//voxel map
    int channel_x;
    int channel_y;
    int channel_z;

    bool doNormalizeVoxels;

// vertex coords array
    static GLfloat vertices[];

// normal array
    static GLfloat normals[] ;
 
// color array
    static GLfloat colors[] ;

// index array of vertex array for glDrawElements()
    static GLubyte indices[] ;
};

#endif
