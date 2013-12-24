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
#include <QtSql>

#include "pLabTools/pLabUtils.h"
#include "pLabTools/pLabConstruction.h"

pLabConstruction::pLabConstruction(pMolKernelInterface* p_interface)
{
  interface = p_interface;
  vectorIxix = 0;
  allocatedIx = 0;

  QLabel* groupLabel = new QLabel("group", this);
  QLabel* vectorLabel = new QLabel("vector", this);
  QLabel* makeLabel = new QLabel("make", this);
  QLabel* minionLabel = new QLabel("minion", this);

  groupsCombo = new QComboBox;
  QSqlDatabase db = pMolSql::getConnection("blaj");
  QSqlQuery query;

  query.exec("SELECT name FROM constructionGroups ORDER BY ix");
  while (query.next())
   groupsCombo->addItem(query.value(0).toString());

  query.exec("SELECT name FROM minions ORDER BY ix");
  while (query.next())
  minionCombo->addItem(query.value(0).toString());

  db.close();
 
  vectorCombo = new QComboBox;
  vectorCombo->setEditable(true);
  vectorCombo->setInsertPolicy(QComboBox::InsertAtTop);
  vectorCombo->setFocus();


  makeEdit = new QTextEdit;
  //nameString->setFocus();

  allocateButton = new QPushButton("allocate");
  allocateButton->setEnabled(false);
  cancelButton = new QPushButton("cancel");
  finishButton = new QPushButton("finish");
  finishButton->setEnabled(false);

/*
  connect(vectorCombo, SIGNAL(editTextChanged(const QString &)),
            this, SLOT(vectorComboChanged(const QString &)));
*/

  connect(vectorCombo, SIGNAL(activated(int)),
            this, SLOT(vectorComboActivated(int)));

  connect(makeEdit, SIGNAL(editingFinished()), 
            this, SLOT(commentEditingFinished()));

  connect(finishButton, SIGNAL(clicked(bool)),
            this, SLOT(finishButtonClicked(bool)));

  connect(cancelButton, SIGNAL(clicked(bool)),
            this, SLOT(cancelButtonClicked(bool)));

  connect(allocateButton, SIGNAL(clicked(bool)),
            this, SLOT(allocateButtonClicked(bool)));


  QGridLayout *layout2 = new QGridLayout;
  layout2->addWidget(groupLabel, 1,0,1,1);
  layout2->addWidget(groupsCombo, 1,1,1,6);
  layout2->addWidget(vectorLabel, 2, 0,1,1);
  layout2->addWidget(vectorCombo, 2, 1, 1, 6);
  layout2->addWidget(minionLabel, 3 , 0,1,1);
  layout2->addWidget(minionCombo, 3,1,1,6);
  layout2->addWidget(makeLabel, 4,0,1,1);
  layout2->addWidget(makeEdit, 4,1,1,6);
  layout2->setColumnMinimumWidth(1, 388);

  QGridLayout *layout3 = new QGridLayout;
  layout3->addWidget(allocateButton, 0, 0 , 1,1);
  layout3->addWidget(finishButton, 0, 1 , 1,1);
  layout3->addWidget(cancelButton, 0, 2, 1,1);

  QGridLayout *layout = new QGridLayout;
  layout->addLayout(layout2,0,0,1,2);
  layout->addLayout(layout3,1,0,1,1);

  setLayout(layout);
  setWindowTitle(tr("Enter new construction!"));
};

//---------------------------------------------------------
///
void pLabConstruction::cancelButtonClicked(bool)
{
  try
  {
    if (allocatedIx)
    {
      QSqlDatabase db = pMolSql::getConnection("blah");			//connect to database
      QSqlQuery query;
      QString sql, temp;
      sql = "DELETE FROM ix WHERE ix.ix = "+temp.setNum(allocatedIx);
      bool ok = query.exec(sql);
      db.close(); 
    }
    close();
  }
    catch(pMolError &error)
  {
    error.append("pMolFreezerBox::trying to search");
    error.detail(interface);
  }
};

//---------------------------------------------------------
///allocate a number on the database
void pLabConstruction::allocateButtonClicked(bool)
{
  try
  {
    //initial connections
    QSqlDatabase db = pMolSql::getConnection("blah");			//connect to database
    QSqlQuery query;
    QString sql = "INSERT INTO ix (typeIx) VALUES (1);";
    bool ok = query.exec(sql);
    allocatedIx = query.lastInsertId().toInt();
    QString temp;
    mpLabel->setText("<h1><b>MP"+temp.setNum(allocatedIx)+"</b></h1>");
    setStatus();
    db.close();
  }

  catch(pMolError &error)
  {
    error.append("pMolFreezerBox::trying to search");
    error.detail(interface);
  }
};


//---------------------------------------------------------
///
void pLabConstruction::finishButtonClicked(bool)
{
  try
  {
    QString temp1, temp2;

    QSqlDatabase db = pMolSql::getConnection("blah");			//connect to database
    QSqlQuery query;
    QString sql;
    sql = "INSERT INTO constructions (vectorIxix, status, make ) "
          "VALUES ("+QString().setNum(vectorIxix)+",1,'"+makeEdit->toPlainText()+"')";
    query.exec(sql);
    qDebug() << sql;
    close();
  }

  catch(pMolError &error)
  {
    error.append("pMolFreezerBox::trying to search");
    error.detail(interface);
  }
};

void pLabConstruction::setStatus()
{
  if (allocatedIx)
  {
    allocateButton->setEnabled(false);
    if (vectorIxix && (!makeEdit->toPlainText().isEmpty()))
      finishButton->setEnabled(true);
    else
      finishButton->setEnabled(false);
  }
  else
  {
    if (vectorIxix && (!makeEdit->toPlainText().isEmpty()))
      allocateButton->setEnabled(true);
    else
      allocateButton->setEnabled(false);
  };
};

void pLabConstruction::commentEditingFinished()
{
  setStatus();
};

//----------------------------------------------------------------------
///search for item has been activated, let's look for something
void pLabConstruction::vectorComboActivated(int index)
{
  try
  { QSqlDatabase db = pMolSql::getConnection("blah");			//connect to database
    QString boxTypeName = "plasmids";
    QString search = pLabUtils::parseToSql(vectorCombo->currentText(), boxTypeName);
    if (search.isNull()) throw pMolError("you searched for nonsense");
    QString sql = "SELECT ixix,name FROM "+boxTypeName+" WHERE "+search+" ORDER BY ixix";

    QSqlQuery query;
    query.exec(sql);

    QString name;
    int i = 0, ixix;
//    foundIxix = 0;
    vectorCombo->clear();

    while (query.next()) 
    {
      QString s1;
      ixix = query.value(0).toInt();
      name = query.value(1).toString();
      vectorCombo->insertItem(i++,"[MP"+s1.setNum(ixix)+"]["+name+"]");
    }

    if (vectorCombo->count() == 1)
    { vectorIxix = ixix; vectorName = name; }
    else
      vectorIxix = 0;

    setStatus();

  }
  catch(pMolError &error)
  {
    error.append("pMolFreezerBox::trying to search");
    error.detail(interface);
  }
};

pLabConstruction* pLabConstruction::create(pMolKernelInterface* interface, pMolCmd* cmd)
{
  pLabConstruction* entry = new pLabConstruction(interface);
  entry->show();
  return entry;
};
