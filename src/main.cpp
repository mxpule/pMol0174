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
#include <QApplication>
#include <ctime> 
#include <cstdlib>
#include "pMolKernel.h"
#include "pMolSql/pMolSql.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(mdi);

    pMolSql sql = pMolSql();
    QApplication app(argc, argv);
    //MainWindow mainWin;
    //mainWin.show();
    srand((unsigned)time(0)); 
    pMolKernel* kernel = new pMolKernel();
    return app.exec();
}
