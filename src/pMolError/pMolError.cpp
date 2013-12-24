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

#include "pMolError/pMolError.h"

pMolError::pMolError(const QString &p_error)
{
  errors << p_error;
};

pMolError::pMolError(const pMolError &p_error)
{
  errors << p_error.errors;
};

void pMolError::append(const QString &p_error)
{
  errors << p_error;
};

void pMolError::append(const pMolError &p_error)
{
  errors << p_error.errors;
};

pMolError::~pMolError()
{
};

QString pMolError::details()
{
  return errors.join("\n");
};

void pMolError::detail(pMolKernelInterface* interface)
{
  for (int i=0; i<errors.size(); ++i)
    interface->msg(errors.at(i));
};
