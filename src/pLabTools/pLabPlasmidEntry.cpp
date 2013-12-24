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
#include "pLabTools/pLabPlasmidEntry.h"

pLabPlasmidEntry::pLabPlasmidEntry(pMolKernelInterface* p_interface)
{
  interface = p_interface;
  allocatedIx = 0;


  mpLabel = new QLabel("<h1><b>MPxxxx</b></h1>", this);
  mpLabel->setTextFormat(Qt::RichText);

  QLabel* nameLabel = new QLabel("name", this);
  QLabel* commentLabel = new QLabel("comment", this);
  //QLabel* typeLabel = new QLabel("source", this);

  nameString = new QLineEdit;
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

  connect(nameString, SIGNAL(editingFinished()), 
            this, SLOT(nameEditingFinished()));

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
  layout2->addWidget(nameLabel, 1, 0,1,1);
  layout2->addWidget(nameString, 1, 1, 1, 6);
  layout2->addWidget(commentLabel, 2, 0,1,1);
  layout2->addWidget(commentString, 2, 1, 1, 6);
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
  setWindowTitle(tr("Enter Cell Details!"));
};

//---------------------------------------------------------
///
void pLabPlasmidEntry::cancelButtonClicked(bool)
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
void pLabPlasmidEntry::allocateButtonClicked(bool)
{
  try
  {
    //initial connections
    QSqlDatabase db = pMolSql::getConnection("blah");			//connect to database
    QSqlQuery query;
    QString sql = "INSERT INTO ix (typeIx) VALUES (8);";
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
void pLabPlasmidEntry::finishButtonClicked(bool)
{
  try
  {
    QString temp1, temp2;

    QSqlDatabase db = pMolSql::getConnection("blah");			//connect to database
    QSqlQuery query;
    QString sql;
    sql = "INSERT INTO plasmids (ixix, name, comment) "
          "VALUES ("+temp1.setNum(allocatedIx)+",'"+nameString->text()+"','"+commentString->text()+"')";
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

void pLabPlasmidEntry::setStatus()
{
  if (allocatedIx)
  {
    allocateButton->setEnabled(false);
    if ((!nameString->displayText().isEmpty()) && (!commentString->displayText().isEmpty()))
      finishButton->setEnabled(true);
    else
      finishButton->setEnabled(false);
  }
  else
  {
    if ((!nameString->displayText().isEmpty()) && (!commentString->displayText().isEmpty()))
      allocateButton->setEnabled(true);
    else
      allocateButton->setEnabled(false);
  };
};

void pLabPlasmidEntry::commentEditingFinished()
{
  setStatus();
};

void pLabPlasmidEntry::nameEditingFinished()
{
  setStatus();
};


pLabPlasmidEntry* pLabPlasmidEntry::create(pMolKernelInterface* interface, pMolCmd* cmd)
{
  pLabPlasmidEntry* entry = new pLabPlasmidEntry(interface);
  entry->show();
  return entry;
};