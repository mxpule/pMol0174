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
#ifndef PLABTOOLSSLOTWIDGET_H
#define PLABTOOLSSLOTWIDGET_H

#include <QTextBrowser>
#include <QWidget>
#include <QtGui>
#include <QString>

#include "pMolInterface.h"

class pLabToolsSlotWidget  : public QWidget
{
  public:
    pLabToolsSlotWidget(const QString &p_slot, int p_radius, pMolInterface* p_parent);
    virtual ~pLabToolsSlotWidget();

    void reset(int p_ixix, const QString &p_details, bool p_clicked);
    virtual void draw(QPainter* painter);
    virtual void paintEvent(QPaintEvent *event);
 
    void highlight();
    void unHighlight();
    bool clicked, empty;
    QString name, details, slot;
    bool changed;
    int ixix;

  protected:
    pMolInterface* parent;
    QColor niceColor(int ixix);

    int r;
    QColor color;

    virtual void mousePressEvent(QMouseEvent *event);
};

#endif
*/
