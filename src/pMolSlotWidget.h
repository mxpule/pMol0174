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
#ifndef PMOLSLOTWIDGET_H
#define PMOLSLOTWIDGET_H

#include <QWidget>
#include <QtGui>

#include <QString>

class pMolSlotWidget  : public QWidget
{
  public:
    pMolSlotWidget(int p_r, const QString &p_name, const QColor &p_color, bool p_empty);
    ~pMolSlotWidget();
    void reset(int p_r, const QString &p_name, const QColor &p_color, bool p_empty, bool p_clicked);
    virtual void draw(QPainter* painter);

  protected:
   QString name;
   bool clicked, empty, changed;
   int r;
   QColor color;

   virtual void paintEvent(QPaintEvent *event);
   virtual void mousePressEvent(QMouseEvent *event);
};

#endif
