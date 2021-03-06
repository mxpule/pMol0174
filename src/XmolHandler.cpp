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
#include <QtGui>

#include "XmolHandler.h"

XmolHandler::XmolHandler(const QString &p_filename)
{
  filename = p_filename;
}

XmolHandler::XmolHandler(pMolXMLbase *pMolRootObject)
{
  pMolXMLbaseFocus = pMolRootObject;
}

bool XmolHandler::startElement(const QString & /* namespaceURI */,
                               const QString & /* localName */,
                               const QString &qName,
                               const QXmlAttributes &attributes)
{
    pMolXMLbaseFocus = pMolXMLbaseFocus->XMLopen(qName, error);

    return true;
}

bool XmolHandler::endElement(const QString & /* namespaceURI */,
                             const QString & /* localName */,
                             const QString &qName)
{
    pMolXMLbaseFocus = pMolXMLbaseFocus->XMLclose(qName, data.trimmed(), error);
    data = ""; 

    return true;
}

bool XmolHandler::characters(const QString &str)
{
    data += str;
    return true;
}

bool XmolHandler::fatalError(const QXmlParseException &exception)
{
    return false;
}

QString XmolHandler::errorString() const
{
    return error;
}

