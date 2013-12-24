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
#ifndef XBELHANDLER_H
#define XBELHANDLER_H

#include <QIcon>
#include <QXmlDefaultHandler>

#include "pMolObject.h"

class QTreeWidget;
class QTreeWidgetItem;

class XbelHandler : public QXmlDefaultHandler
{
public:
 //   XbelHandler(QTreeWidget *treeWidget);
    XbelHandler(const QString &p_filename);

    bool startElement(const QString &namespaceURI, const QString &localName,
                      const QString &qName, const QXmlAttributes &attributes);
    bool endElement(const QString &namespaceURI, const QString &localName,
                    const QString &qName);
    bool characters(const QString &str);
    bool fatalError(const QXmlParseException &exception);
    QString errorString() const;

private:
    QTreeWidgetItem *createChildItem(const QString &tagName);

    QTreeWidget *treeWidget;
    QTreeWidgetItem *item;
    QString currentText;
    QString errorStr;
    QString filename;
    bool metXbelTag;

    QIcon folderIcon;
    QIcon bookmarkIcon;
};

#endif
