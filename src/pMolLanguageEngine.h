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
#ifndef PMOLLANGUAGEENGINE_H
#define PMOLLANGUAGEENGINE_H

#include <QString>
#include <QList>
#include <QtGui>

#include "pMolCmd.h"

class pMolLanguageEngine
{
  protected :
    int getCue(QString* cmdStr);
    QString getNext(QString *cmdStr);
    pMolCmd* parseError(const QString &errorMsg, QString* cmdStr);
    pMolCmd* parseString(QString* cmdStr);
    pMolCmd* parseNumber(QString* cmdStr);
    pMolCmd* parseName(QString* cmdStr);
    pMolCmd* parseChild(QString* cmdStr);
    pMolCmd* parsePostName(QString* cmdStr);
    pMolCmd* parseList(QString *cmdStr);
    pMolCmd* parse(QString *cmdStr);

    enum cueTypes 
    {CUE_TERM, CUE_OPEN_ROUND, CUE_CLOSE_ROUND, CUE_EMPTY,
     CUE_CHILD, CUE_COMMA, CUE_STRING, CUE_NUMBER, CUE_NAME, CUE_UNKNOWN};
};

#endif
