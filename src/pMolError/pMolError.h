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

#ifndef PMOLERROR_H
#define PMOLERROR_H

#include <QString>
#include <QStringList>

#include "pMolKernelInterface.h"

class pMolError
{
  public:
    pMolError(const QString &p_error);
    pMolError(const pMolError &p_error);
    ~pMolError();

    void append(const QString &p_error);
    void append(const pMolError &p_error);

    QString details();
    void detail(pMolKernelInterface* interface);

  protected:
    QStringList errors;
};

#endif
