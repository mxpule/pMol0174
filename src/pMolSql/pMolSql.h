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

#ifndef PMOLSQL_H
#define PMOLSQL_H

#include <QtSql>

///pMolSql class for handling sql input and output
///makes sure only a certain amount of simultaneous connections are made
///disconnects and cleans up as appopriate
///handles errors
class pMolSql
{
  public :
    pMolSql();
    ~pMolSql();

    static QSqlDatabase getConnection(const QString &name);
    static void exec(QSqlDatabase &db, QSqlQuery &query, const QString &name);

  private :

};

#endif
