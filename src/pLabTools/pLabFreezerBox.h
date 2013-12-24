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
#ifndef PMOLBOX_H
#define PMOLBOX_H

#include <QtSql>
#include <QtGui>

#include "pMolXMLbase.h"
#include "pMolInterface.h"
#include "pMolCmd.h"
#include "pMolKernelInterface.h"
#include "pMolStackObject.h"
#include "pLabTools/pLabToolsSlotWidget.h"

class pLabFreezerBox : public QWidget, public pMolObject, public pMolInterface
{
  Q_OBJECT


  public :
    pLabFreezerBox(pMolKernelInterface* p_interface, int box, int highlightSlotIx = -1);
    ~pLabFreezerBox();

    virtual void paintEvent(QPaintEvent *event);
    virtual void talk(const QString &msg);
    static pMolStackObject* create(pMolKernelInterface* interface, pMolCmd* cmd);
    static pMolStackObject* create(pMolKernelInterface* interface, int box, int highlightSlotIs = -1);

  public slots:
    void searchComboChanged(const QString &text);
    void searchComboActivated(int index);
    void slotClicked();
    void addItemPressed();
    void removeItemPressed();

  protected:
    virtual void keyPressEvent(QKeyEvent * event);
    virtual void keyReleaseEvent(QKeyEvent* event);
    virtual void mousePressEvent(QMouseEvent * event);

  private:
    bool shiftDown;
    int boxType, boxIx, foundIxix;
    QString foundName, foundDetails;
    QString boxNum, boxName, boxTypeName;

    QComboBox *boxCombo;
    QComboBox *searchCombo;
    QTextBrowser *label;
    QGridLayout *layout;

    QHash<QString, pLabToolsSlotWidget* > slotWidgets;

    static const int squareWidth = 24;
    static const int p = 5;
    static const int radius = 20;



    pMolKernelInterface* interface;

    void parseMap(const QString &map, int *max_x, int* max_y);
    void slotsPopulateOligos(QSqlQuery &db, int highlightIx);
    void slotsPopulatePlasmids(QSqlQuery &db, int highlightIx);
    void slotsPopulateCells(QSqlQuery &db, int highlightIx);
    void slotsPopulateSupes(QSqlQuery &db, int highlightIx);
};


#endif
