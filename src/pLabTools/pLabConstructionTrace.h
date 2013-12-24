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
#ifndef PLABCONSTRUCTIONTRACE_H
#define PLABCONSTRUCTIONTRACE_H

#include <QWidget>
#include <QTreeWidget>

#include "pMolSql/pMolSql.h"
#include "pMolStackObject.h"
#include "pMolKernelInterface.h"
#include "./pLabTools/pLabConstructionTraceTab.h"

class QComboBox;
class QLineEdit;
class QPushButton;

class pLabConstructionTrace : public QWidget, public pMolStackObject
{
    Q_OBJECT

public:
    pLabConstructionTrace(pMolKernelInterface* p_interface, int p_GroupIx);
    static pMolStackObject* create(pMolKernelInterface* interface, pMolCmd* cmd);


public slots:
    void tabChanged(int p_tab);
    void updateButtonClicked(bool);
    void statusChanged(int p_constructionIx);

private:
    QString vectorName;
    void searchSql(const QString &p_sql);
    void update(const QString &sqls);
    int tab;
    int groupIx;

    QComboBox *statusCombo;

    QPushButton *updateButton;// = new QPushButton("allocate");
    QPushButton *finishButton;// = new QPushButton("cancel");
    QLabel* groupLabel;// = new QLabel("vector", this);
    QLabel* plasmidLabel;
    QLabel* minionLabel;
    QTextEdit* makeText;
    QTextEdit* traceText;
    QLineEdit* entryText;
    QTreeWidget* findList;
    pMolKernelInterface* interface;
    QString make;

    QString findNumbers(const QString &source);
    void findComponents(QSqlQuery &query);

    static const int maxTabs = 256;
    pLabConstructionTraceTab* constructionTabs[maxTabs];

    QStringList sqlsCache;

    QStringList statusTypes;
    int constructionCount;
};

#endif