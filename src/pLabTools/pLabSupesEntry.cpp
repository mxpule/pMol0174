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
#include "pLabTools/pLabSupesEntry.h"

pLabSupesEntry::pLabSupesEntry(pMolKernelInterface* p_interface)
{
  interface = p_interface;
  vectorIxix = 0;
  allocatedIx = 0;


  mpLabel = new QLabel("<h1><b>MPxxxx</b></h1>", this);
  mpLabel->setTextFormat(Qt::RichText);

  QLabel* vectorLabel = new QLabel("vector", this);
  QLabel* commentLabel = new QLabel("comment", this);
  QLabel* typeLabel = new QLabel("source", this);

  pseudotype = new QComboBox;
  QSqlDatabase db = pMolSql::getConnection("blaj");
  QSqlQuery query;
  bool ok = query.exec("SELECT name FROM supeTypes ORDER BY name");
  while (query.next())
    pseudotype->addItem(query.value(0).toString());
  db.close();

  vectorCombo = new QComboBox;
  vectorCombo->setEditable(true);
  vectorCombo->setInsertPolicy(QComboBox::InsertAtTop);
  vectorCombo->setFocus();


  commentString = new QLineEdit;
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

  connect(commentString, SIGNAL(editingFinished()), 
            this, SLOT(commentEditingFinished()));

  connect(finishButton, SIGNAL(clicked(bool)),
            this, SLOT(finishButtonClicked(bool)));

  connect(cancelButton, SIGNAL(clicked(bool)),
            this, SLOT(cancelButtonClicked(bool)));

  connect(allocateButton, SIGNAL(clicked(bool)),
            this, SLOT(allocateButtonClicked(bool)));

  QGridLayout *layout1 = new QGridLayout;
  layout1->addWidget(mpLabel,0,0,1,1);

  QGridLayout *layout2 = new QGridLayout;
  layout2->addWidget(vectorLabel, 1, 0,1,1);
  layout2->addWidget(vectorCombo, 1, 1, 1, 6);
  layout2->addWidget(typeLabel, 2,0,1,1);
  layout2->addWidget(pseudotype, 2, 1,1,6);
  layout2->addWidget(commentLabel, 3, 0,1,1);
  layout2->addWidget(commentString, 3, 1, 1, 6);
  layout2->setColumnMinimumWidth(1, 388);

  QGridLayout *layout3 = new QGridLayout;
  layout3->addWidget(allocateButton, 0, 0 , 1,1);
  layout3->addWidget(finishButton, 0, 1 , 1,1);
  layout3->addWidget(cancelButton, 0, 2, 1,1);

  QGridLayout *layout = new QGridLayout;
  layout->addLayout(layout1,0,0,1,2);
  layout->addLayout(layout2,1,0,1,2);
  layout->addLayout(layout3,2,0,1,1);

  setLayout(layout);
  setWindowTitle(tr("Enter Supes Details!"));
};

//---------------------------------------------------------
///
void pLabSupesEntry::cancelButtonClicked(bool)
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
void pLabSupesEntry::allocateButtonClicked(bool)
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
void pLabSupesEntry::finishButtonClicked(bool)
{
  try
  {
    QString temp1, temp2;

    QSqlDatabase db = pMolSql::getConnection("blah");			//connect to database
    QSqlQuery query;
    QString sql;
    sql = "INSERT INTO supes (ixix, name, comment, vectorIxix, supeTypeIx) "
          "VALUES ("+temp1.setNum(allocatedIx)+",'"+vectorName+"','"+commentString->text()+"'"
          ","+temp2.setNum(vectorIxix)+",1)";
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

void pLabSupesEntry::setStatus()
{
  if (allocatedIx)
  {
    allocateButton->setEnabled(false);
    if (vectorIxix && (!commentString->displayText().isEmpty()))
      finishButton->setEnabled(true);
    else
      finishButton->setEnabled(false);
  }
  else
  {
    if (vectorIxix && (!commentString->displayText().isEmpty()))
      allocateButton->setEnabled(true);
    else
      allocateButton->setEnabled(false);
  };
};

void pLabSupesEntry::commentEditingFinished()
{
  setStatus();
};

//----------------------------------------------------------------------
///search for item has been activated, let's look for something
void pLabSupesEntry::vectorComboActivated(int index)
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

pLabSupesEntry* pLabSupesEntry::create(pMolKernelInterface* interface, pMolCmd* cmd)
{
  pLabSupesEntry* entry = new pLabSupesEntry(interface);
  entry->show();
  return entry;
};
