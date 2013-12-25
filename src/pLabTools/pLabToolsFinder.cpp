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

#include <QtGui>
#include <QtSql>

#include "pMolError/pMolError.h"
#include "pLabTools/pLabUtils.h"
#include "pMolSql/pMolSql.h"
#include "pLabTools/pLabToolsFinder.h"
#include "pLabTools/pLabToolsFinderSlot.h"

pLabToolsFinder::pLabToolsFinder(pMolKernelInterface* p_interface)
{
  interface = p_interface;

  sampleType = new QComboBox;
  sampleType->addItem(tr("oligo"));
  sampleType->addItem(tr("plasmid"));
  sampleType->addItem(tr("supes"));
  sampleType->addItem(tr("cells"));
  sampleType->addItem(tr("boxes"));

  searchString = new QLineEdit;
  searchString->setFocus();

  QPushButton *filterButton = new QPushButton("search");
  QPushButton *sqlButton = new QPushButton("SQL");

  QStringList labels;
  labels << tr("ix") << tr("name") << tr("summary");
  itemsTable = new QTreeWidget;
  itemsTable->header()->setResizeMode(QHeaderView::Interactive);
  itemsTable->setHeaderLabels(labels);
  itemsTable->setSortingEnabled(true);


  connect(filterButton, SIGNAL(clicked(bool)),
            this, SLOT(filterButtonClicked(bool)));

  connect(sqlButton, SIGNAL(clicked(bool)),
            this, SLOT(sqlButtonClicked(bool)));

//void QListWidget::itemDoubleClicked ( QListWidgetItem * item )  

  connect(itemsTable, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)),
            this, SLOT(itemDoubleClicked(QTreeWidgetItem *, int)));

  connect(searchString, SIGNAL(returnPressed()), 
            this, SLOT(searchStringEditingFinished()));


  QGridLayout *layout = new QGridLayout;
  layout->addWidget(searchString, 1, 0);
  layout->addWidget(sampleType, 1, 1);
  layout->addWidget(filterButton, 1, 2);
  layout->addWidget(sqlButton, 1,3);
  layout->addWidget(itemsTable, 0,0,1,4);

  setLayout(layout);
  setWindowTitle(tr("finder!"));
};

void pLabToolsFinder::searchStringEditingFinished()
{
  filterButtonClicked(true);
};

void pLabToolsFinder::itemDoubleClicked(QTreeWidgetItem *item, int column)
{
  try
  {
    pLabToolsFinderSlot* slot = dynamic_cast <pLabToolsFinderSlot*> (item);
    slot->doubleClicked(column);
  }
  //ERROR HANDLING
  catch (pMolError &error)
  {
    error.detail(interface);		//simply dump trace to terminal
  }
};


void pLabToolsFinder::filterButtonClicked(bool checked)
{
  try
  {
    QString table;
    QString search = searchString->displayText();
    if (sampleType->currentIndex()==0) table = "oligos";
    if (sampleType->currentIndex()==1) table = "plasmids";
    if (sampleType->currentIndex()==2) table = "supes";
    if (sampleType->currentIndex()==3) table = "cells";
    if (sampleType->currentIndex()==4) table = "boxes";
    QString sql = pLabUtils::parseToSql(search,table);
    searchSql(sql);
  }
  catch (pMolError &error)
  {
    error.detail(interface);
  };
};

void pLabToolsFinder::sqlButtonClicked(bool checked)
{
  QString sql = searchString->displayText();
  searchSql(sql);
};

void pLabToolsFinder::searchSql(const QString &p_sql)
{
  try
  {
  qDebug() << "query" << p_sql;
  QSqlDatabase db = pMolSql::getConnection("blaj");
  QSqlQuery query;
  QString sql;

  if (sampleType->currentIndex()==0)
  {
  sql  = "SELECT oligos.ixix, oligos.name, oligos.comment, ";
  sql += "bosamples.name, bosamples.slot, bosamples.box ";
  sql += "FROM oligos LEFT JOIN ";
  sql += "(SELECT * FROM samples, boxes WHERE boxes.ix = samples.box AND samples.present = 1)";
  sql += " AS bosamples ";
  sql += "ON oligos.ixix = bosamples.ixix ";
  sql += "WHERE "+p_sql+" ORDER BY oligos.ixix";
  }
  else if (sampleType->currentIndex()==1)
  {
  sql  = "SELECT plasmids.ixix, plasmids.name, plasmids.comment, ";
  sql += "bosamples.name, bosamples.slot, bosamples.box ";
  sql += "FROM plasmids LEFT JOIN ";
  sql += "(SELECT * FROM samples, boxes WHERE boxes.ix = samples.box AND samples.present = 1)";
  sql += " AS bosamples ";
  sql += "ON plasmids.ixix = bosamples.ixix ";
  sql += "WHERE "+p_sql+" ORDER BY plasmids.ixix";
  }
  else if (sampleType->currentIndex()==2)
  {
  sql="SELECT supes.ixix, supes.name, supes.comment, "
      "bosamples.name, bosamples.slot, bosamples.box "
      "FROM supes LEFT JOIN "
      "(SELECT * FROM samples, boxes WHERE boxes.ix = samples.box AND samples.present = 1)"
      " AS bosamples "
      "ON supes.ixix = bosamples.ixix "
      "WHERE "+p_sql+" ORDER BY supes.ixix";
  }

  else if (sampleType->currentIndex()==3)
  {
  sql="SELECT cells.ixix, cells.name, cells.comment, "
      "bosamples.name, bosamples.slot, bosamples.box "
      "FROM cells LEFT JOIN "
      "(SELECT * FROM samples, boxes WHERE boxes.ix = samples.box AND samples.present = 1)"
      " AS bosamples "
      "ON cells.ixix = bosamples.ixix "
      "WHERE "+p_sql+" ORDER BY cells.ixix";
  }

  else if (sampleType->currentIndex()==4)
  {
  sql="SELECT boxes.ixix, boxes.name, boxes.details, "
      "boxes.ix FROM boxes, ix "
      "WHERE "+p_sql+" AND boxes.ixix = ix.ix ORDER BY boxes.ixix";
  }

  else return;

  qDebug() << sql;
  bool ok;
  ok = query.exec(sql);


  itemsTable->clear();
  pLabToolsFinderSlot *newItem, *newSample;
  int oldIxix = 0;
  int sampleCount = 0;
  QBrush sampleBrush(QColor(0xff,0xff,0xd9,255));
  QBrush itemBrush(QColor(0,127,127,255));

  while (query.next()) 
  {

    int fudgeBoxIx;
    if (sampleType->currentIndex()==4) 
      fudgeBoxIx = query.value(3).toInt();
    else
      fudgeBoxIx = 0;

    QString ixString;
    int ixix = query.value(0).toInt();
    QString comment = query.value(2).toString();
    QString name = query.value(1).toString();
    ixString.setNum(ixix);
    if (ixix!=oldIxix)
    {
      oldIxix = ixix; 
      sampleCount = 0;
      newItem = new pLabToolsFinderSlot(interface, itemsTable, ixix, fudgeBoxIx);
      newItem->setText(0,ixString);
      newItem->setText(1,name);
      comment = comment.replace('\n', "; ");
      newItem->setText(2,comment.left(64));
    }
    if (!query.value(3).isNull() && !fudgeBoxIx)
    {
      sampleCount++;
      QString count, box, slot;
      count.setNum(sampleCount);
      box = query.value(3).toString();
      slot = query.value(4).toString();
      int boxNumber = query.value(5).toInt();
      newSample = new pLabToolsFinderSlot(interface, newItem, ixix, boxNumber);
      newSample->setText(0, "[sample:"+count+"]");
      newSample->setText(1, "[BOX:"+box+"][SLOT:"+slot+"]");
      newSample->setBackground(0,sampleBrush);
      newSample->setBackground(1,sampleBrush);
    };
  //  qDebug() << ixString << name << sequence;
  };
  }
  //ERROR HANDLING
  catch(pMolError &error)
  {
    error.append("pLabToolsFinder::error while searching database");
    error.detail(interface);		//simply dump trace to terminal
  };
};

pLabToolsFinder* pLabToolsFinder::create(pMolKernelInterface* interface, pMolCmd* cmd)
{
  pLabToolsFinder* finder = new pLabToolsFinder(interface);
  finder->show();
  return finder;
};

*/
