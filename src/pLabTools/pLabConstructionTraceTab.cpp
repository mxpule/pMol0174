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

#include "./pLabTools/pLabConstructionTraceTab.h"


pLabConstructionTraceTab::pLabConstructionTraceTab(int p_constructionIx, int p_plasmidIx, const QString &p_plasmidName, const QStringList &statusTypes, int statusIx)
{
  plasmidIx = p_plasmidIx;
  plasmidName = p_plasmidName;
  constructionIx = p_constructionIx;

  name = QString().setNum(plasmidIx);

  //draw the title
  QString title = QString().setNum(plasmidIx)+":"+plasmidName;
  QLabel* nameLabel = new QLabel("<h3><b>"+plasmidName+"</b></h3>", this);
  nameLabel->setTextFormat(Qt::RichText);

  //setup the trace text QTreeWidget
  traceText = new QTreeWidget();
  traceText->header()->setResizeMode(QHeaderView::Interactive);
  QStringList labels;
  labels << tr("date") << tr("note");
  traceText->setHeaderLabels(labels);
  traceText->setSortingEnabled(true);
  traceText->setColumnWidth(0,96);

  //set up the status Combo box and its label
  QLabel* statusLabel = new QLabel("status", this);
  statusCombo = new QComboBox;
  statusCombo->addItems(statusTypes);
  statusCombo->setCurrentIndex(statusIx-1);

  //set up the label text entry and its label
  QLabel* updateLabel = new QLabel("update", this);
  traceEntry = new QLineEdit;

  //set up the grid so everything looks ok
  QGridLayout *traceTabLayout = new QGridLayout;
  traceTabLayout->addWidget(nameLabel, 0, 0, 1, 12);
  traceTabLayout->addWidget(statusLabel,1 , 0, 1, 1);
  traceTabLayout->addWidget(statusCombo, 1, 1, 1, 11);
  traceTabLayout->addWidget(traceText, 2, 0, 1, 12);
  traceTabLayout->addWidget(updateLabel, 3, 0, 1, 1);
  traceTabLayout->addWidget(traceEntry, 3, 1, 1, 11);
  setLayout(traceTabLayout);
};


void pLabConstructionTraceTab::setEntryText(const QString &text)
{
  traceEntry->setText(text);
  traceEntry->selectAll();
};

QString pLabConstructionTraceTab::getEntryText()
{
  return traceEntry->text();
};


QString pLabConstructionTraceTab::getName()
{
  return name;
};


int pLabConstructionTraceTab::getConstructionIx()
{
  return constructionIx;
};

void pLabConstructionTraceTab::addTraceItem(const QString &stamp, const QString &text)
{
  qDebug() << "whats going on1?" << stamp << text << traceText;
  QTreeWidgetItem* traceItem = new QTreeWidgetItem(traceText);
  qDebug() << "whats going on2?" << stamp << text << traceText;
  traceItem->setText(0,stamp);
  traceItem->setText(1,text);
  qDebug() << "whats going on31?" << stamp << text << traceText;
};

*/
