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
#ifndef PLABPLASMIDENTRY_H
#define PLABPLASMIDENTRY_H


#include <QWidget>
#include <QTreeWidget>

#include "pMolSql/pMolSql.h"
#include "pMolStackObject.h"
#include "pMolKernelInterface.h"

class QComboBox;
class QLineEdit;
class QPushButton;

class pLabPlasmidEntry : public QWidget, public pMolStackObject
{
    Q_OBJECT

public:
    pLabPlasmidEntry(pMolKernelInterface* p_interface);
    static pLabPlasmidEntry* create(pMolKernelInterface* interface, pMolCmd* cmd);


public slots:
    void allocateButtonClicked(bool);
    void finishButtonClicked(bool);
    void cancelButtonClicked(bool);
    void nameEditingFinished();
    void commentEditingFinished();

private:
    void setStatus();
    int allocatedIx;

    QLineEdit *nameString;
    QLineEdit *commentString;

    QPushButton *allocateButton;// = new QPushButton("allocate");
    QPushButton *cancelButton;// = new QPushButton("cancel");
    QPushButton *finishButton;// =  new QPushButton("finish");
    QLabel* mpLabel;// = new QLabel("vector", this);

    pMolKernelInterface* interface;
};

#endif