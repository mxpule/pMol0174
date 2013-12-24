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
///This is the pMol terminal programme.
///It is essentially a bastardized version of QT qTextEdit

#ifndef PMOLPRIMERORDER_H
#define PMOLPRIMERORDER_H

#include <QtGui>
#include <QTextEdit>

#include "pMolKernelInterface.h"
#include "pMolCmd.h"
#include "pMolStackObject.h"

class pMolPrimerOrder : public QTextEdit, pMolStackObject
{
    Q_OBJECT

public:
    pMolPrimerOrder(const QString &sql, const QString &a);
    static pMolStackObject* create(pMolKernelInterface* interface, pMolCmd* cmd);
    virtual bool type(int);

protected:
    void closeEvent(QCloseEvent *event);

private slots:

private:

};

#endif