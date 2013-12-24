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
#ifndef PLABUTILS_H
#define PLABUTILS_H

#include <QtSql>
#include <QString>

#include "pMolError/pMolError.h"

class pLabUtils
{
  public:
    static QString parseToSql(const QString &search, const QString &table);
    static QString getSqlDetails(const QSqlQuery &query, int type);
    static QString parseTableSql(const QString &query, int type);

    static const int type_SUPES = 1;
    static const int type_PLASMIDS = 8;
    static const int type_OLIGOS = 9;
    static const int type_CELLS = 3;

    static const int BOXTYPE_OLIGOS = 9;
    static const int BOXTYPE_PLASMIDS = 8;
    static const int BOXTYPE_SUPES = 1;
    static const int BOXTYPE_CELLS = 3;


  private:
    static int parseNextType(const QString &s, int *i);
    static QString parseGetString(const QString &s, int *i, bool *ok);
    static int parseGetNumber(const QString &s, int *i, bool *ok);

    enum parseTypes {NUMBER, STRING, GT, LT, AND, OR, NOT, TO, OPEN, CLOSE, BRACE};
};

#endif
