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
/*
#ifndef PLABCONSTRUCTIONTRACETAB_H
#define PLABCONSTRUCTIONTRACETAB_H

#include <QWidget>
#include <QTreeWidget>

#include "pMolSql/pMolSql.h"
#include "pMolStackObject.h"
#include "pMolKernelInterface.h"

class QComboBox;
class QLineEdit;
class QPushButton;

class pLabConstructionTraceTab : public QWidget
{
  Q_OBJECT

  public:

    pLabConstructionTraceTab(int p_constructionIx, int plasmidIx, const QString &plasmidName, const QStringList &statusTypes, int statusIx);

    void setEntryText(const QString &text);
    QString getEntryText();
    void addTraceItem(const QString &date, const QString &text);
    int getConstructionIx();
    QString getName();
    QComboBox* statusCombo;

  private:
    int plasmidIx;
    QString plasmidName;
    int constructionIx;


    QTreeWidget* traceText;
    QLineEdit* traceEntry;
    QString name;
};

#endif
*/
