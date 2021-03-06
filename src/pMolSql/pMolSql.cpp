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

#include "pMolSql/pMolSql.h"
#include "pMolError/pMolError.h"


pMolSql::pMolSql()
{
};

pMolSql::~pMolSql()
{
};

QSqlDatabase pMolSql::getConnection(const QString &name)
{
   // create the default database connection
    static bool connected = false;
    static QSqlDatabase db;  

/*  
if (connected)
  {  if (db.isValid() && db.isOpen()) return db;
     if (db.open()) return db;
     pMolError error = pMolError("pMolSql::getConnection - failed opening database mpule2_pMol");
     QSqlError sqlError = db.lastError();
     if (sqlError.isValid())
     {
       error.append("driverText: "+sqlError.driverText());
       error.append("databaseText: "+sqlError.databaseText());
     }; 
     connected = false;
     throw(error);
  }
  else
  {
*/  if (connected) db.close();
    connected = true;
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("db142e.pair.com");
    db.setDatabaseName("mpule2_PMOLV04");
    db.setUserName("mpule2_20_w");
    db.setPassword("a4SzFTDJ");
    qDebug() << "here db142e";
    if (db.open()) return db;;

    pMolError error = pMolError("pMolSql::getConnection - failed opening database mpule2_pMol; ");
    QSqlError sqlError = db.lastError();
    if (sqlError.isValid())
    {
      error.append("driverText: "+sqlError.driverText());
      error.append("databaseText: "+sqlError.databaseText());
    };
    connected = false;
    throw(error);

};

void pMolSql::exec(QSqlDatabase &db, QSqlQuery &query, const QString &sql)
{
  bool ok;
  ok = query.exec(sql);
  if (ok) return;

    pMolError error = pMolError("pMolSql::exec - failed querying database");
    error.append("command: "+sql);
    QSqlError sqlError = db.lastError();
    if (sqlError.isValid())
    {
      error.append("driverText: "+sqlError.driverText());
      error.append("databaseText: "+sqlError.databaseText());
    };
    throw(error); 
};


