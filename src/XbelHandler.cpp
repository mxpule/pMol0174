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

#include "XbelHandler.h"
/*
XbelHandler::XbelHandler(QTreeWidget *treeWidget)
    : treeWidget(treeWidget)
{
    item = 0;
    metXbelTag = false;

    QStyle *style = treeWidget->style();

    folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirClosedIcon),
                         QIcon::Normal, QIcon::Off);
    folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirOpenIcon),
                         QIcon::Normal, QIcon::On);
    bookmarkIcon.addPixmap(style->standardPixmap(QStyle::SP_FileIcon));
}*/

XbelHandler::XbelHandler(const QString &p_filename)
{
  filename = p_filename;
}


bool XbelHandler::startElement(const QString & /* namespaceURI */,
                               const QString & /* localName */,
                               const QString &qName,
                               const QXmlAttributes &attributes)
{
    if (!metXbelTag && qName != "xbel") {
        errorStr = QObject::tr("The file is not an XBEL file.");
        return false;
    }

    if (qName == "xbel") {
        QString version = attributes.value("version");
        if (!version.isEmpty() && version != "1.0") {
            errorStr = QObject::tr("The file is not an XBEL version 1.0 file.");
            return false;
        }
        metXbelTag = true;
    } else if (qName == "folder") {
        item = createChildItem(qName);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        item->setIcon(0, folderIcon);
        item->setText(0, QObject::tr("Folder"));
        bool folded = (attributes.value("folded") != "no");
        treeWidget->setItemExpanded(item, !folded);
    } else if (qName == "bookmark") {
        item = createChildItem(qName);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        item->setIcon(0, bookmarkIcon);
        item->setText(0, QObject::tr("Unknown title"));
        item->setText(1, attributes.value("href"));
    } else if (qName == "separator") {
        item = createChildItem(qName);
        item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
        item->setText(0, QString(30, 0xB7));
    }

    currentText.clear();
    return true;
}

bool XbelHandler::endElement(const QString & /* namespaceURI */,
                             const QString & /* localName */,
                             const QString &qName)
{
    if (qName == "title") {
        if (item)
            item->setText(0, currentText);
    } else if (qName == "folder" || qName == "bookmark"
               || qName == "separator") {
        item = item->parent();
    }
    return true;
}

bool XbelHandler::characters(const QString &str)
{
    currentText += str;
    return true;
}

bool XbelHandler::fatalError(const QXmlParseException &exception)
{
    QMessageBox::information(treeWidget->window(), QObject::tr("SAX Bookmarks"),
                             QObject::tr("Parse error at line %1, column %2:\n"
                                         "%3")
                             .arg(exception.lineNumber())
                             .arg(exception.columnNumber())
                             .arg(exception.message()));
    return false;
}

QString XbelHandler::errorString() const
{
    return errorStr;
}

QTreeWidgetItem *XbelHandler::createChildItem(const QString &tagName)
{
    QTreeWidgetItem *childItem;
    if (item) {
        childItem = new QTreeWidgetItem(item);
    } else {
        childItem = new QTreeWidgetItem(treeWidget);
    }
    childItem->setData(0, Qt::UserRole, tagName);
    return childItem;
}

