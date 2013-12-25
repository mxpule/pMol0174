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
#ifndef PLABTOOLSFINDER_H
#define PLABTOOLSFINDER_H

#include <QWidget>
#include <QTreeWidget>

#include "pMolStackObject.h"
#include "pMolKernelInterface.h"

class QComboBox;
class QLineEdit;
class QPushButton;

class pLabToolsFinder : public QWidget, public pMolStackObject
{
    Q_OBJECT

public:
    pLabToolsFinder(pMolKernelInterface* p_interface);
    static pLabToolsFinder* create(pMolKernelInterface* interface, pMolCmd* cmd);


public slots:
    void sqlButtonClicked(bool);
    void filterButtonClicked(bool);
    void itemDoubleClicked(QTreeWidgetItem *item, int column);
    void searchStringEditingFinished();
/*
public slots:
    void echoChanged(int);
    void validatorChanged(int);
    void alignmentChanged(int);
    void inputMaskChanged(int);
    void accessChanged(int);
*/
/*
private:
    void searchSql(const QString &p_sql);

    QComboBox *sampleType;
    QLineEdit *searchString;
    QTreeWidget* itemsTable;

    pMolKernelInterface* interface;
};

#endif
*/
