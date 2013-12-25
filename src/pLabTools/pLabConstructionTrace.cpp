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

#include "pLabTools/pLabUtils.h"
#include "pLabTools/pLabConstructionTrace.h"

pLabConstructionTrace::pLabConstructionTrace(pMolKernelInterface* p_interface, int p_groupIx)
{
  interface = p_interface;
  groupIx = p_groupIx;
  qDebug() << "past sql"; 
  QSqlDatabase db = pMolSql::getConnection("blaj");
  QSqlQuery query;
  QString sqls;

  //get all the status types into a QStringList
  sqls="SELECT name, color FROM constructionStatus "
       "ORDER BY ix;";
  bool ok = query.exec(sqls);
  if (!ok) throw pMolError("sql:"+sqls);
  while (query.next())
    statusTypes << query.value(0).toString();

  //get the actual data for the construct
  sqls="SELECT constructionGroups.name, minions.name, "
       "constructionGroups.make "
       "FROM constructionGroups, minions "
       "WHERE constructionGroups.minionIx = minions.Ix "
       "AND constructionGroups.ix = "+QString().setNum(groupIx);
  ok = query.exec(sqls);
  if (!ok) throw pMolError("sql:"+sqls);
  if (!query.next()) throw pMolError("sql: construction trace not found!"+sqls);
  QString group = query.value(0).toString();	//name of the construction
  QString minion = query.value(1).toString();	//who is doing it
  make = query.value(2).toString();		//the make instructions

  //label for the construction group we are making
  groupLabel = new QLabel("<h2><b>"+group+"</b></h2>", this);
  groupLabel->setTextFormat(Qt::RichText);

  //label for the minion name 
  minionLabel = new QLabel("<h3><b>"+minion+"</b></h3>", this);
  minionLabel->setTextFormat(Qt::RichText);

  //this is the tab everything fits onto
  QTabWidget* tab = new QTabWidget(this);

  //make text box and tab
  QWidget* makeTab = new QWidget();
  makeText = new QTextEdit(this);
  makeText->setPlainText(make);
  makeText->setReadOnly(false);
  QGridLayout *makeTabLayout = new QGridLayout;
  makeTabLayout->addWidget(makeText);
  makeTab->setLayout(makeTabLayout);
  tab->addTab(makeTab, "make");
  qDebug() << "finished make tab";

  //finder details and tab
  QWidget* findTab = new QWidget();
  QGridLayout *findTabLayout = new QGridLayout;
  findList = new QTreeWidget();
  QStringList labels;
  labels << tr("MP") << tr("box") << tr("slot");
  findList->header()->setResizeMode(QHeaderView::Interactive);
  findList->setHeaderLabels(labels);
  findList->setSortingEnabled(true);
  findTabLayout->addWidget(findList);
  findTab->setLayout(findTabLayout);
  tab->addTab(findTab, "finder");
  findComponents(query);			//finds the components and adds them
  qDebug() << "finished finder section";

  //make the individual plasmid tabs
  sqls="SELECT constructions.ix, plasmids.ixix, plasmids.name, "
       "constructionNotes.stamp, constructionNotes.note, statusIx "
       "FROM plasmids, constructions LEFT JOIN constructionNotes "
       "ON constructionNotes.constructionIx = constructions.ix "
       "WHERE constructions.groupIx = "+QString().setNum(groupIx)+" "
       "AND plasmids.ixix = constructions.plasmidIx "
       "ORDER BY plasmids.ixix ASC, constructionNotes.ix;";
  bool ok2 = query.exec(sqls);
  if (!ok2) throw pMolError(sqls);
  int constructionIx = -1;		//impossible value
  constructionCount = 1;		//first tab with plasmid should be 2
  qDebug() << "finished construction trace query";

  //loop for each construction note entry, grouping by plasmid
  while (query.next())
  {
    QTreeWidget* traceText;
    int newConstructionIx = query.value(0).toInt();
    if (newConstructionIx != constructionIx)
    {

   //group a plasmid here
      int plasmidIx = query.value(1).toInt();
      QString plasmidName = query.value(2).toString();
      int statusIx = query.value(5).toInt();
      pLabConstructionTraceTab* traceTab =  
        new pLabConstructionTraceTab(newConstructionIx, plasmidIx, plasmidName, statusTypes, statusIx);
      constructionTabs[++constructionCount] = traceTab;
      tab->addTab(constructionTabs[constructionCount], traceTab->getName());
      constructionIx = newConstructionIx;

      connect(traceTab->statusCombo, SIGNAL(currentIndexChanged(int)),
              this, SLOT(statusChanged( int )));
    };
    //adds trace records
     qDebug() << "testing out of loop" << constructionCount << constructionTabs[constructionCount]->getName();
    constructionTabs[constructionCount]->
      addTraceItem(query.value(3).toDate().toString("yyyy-MM-dd"), query.value(4).toString());
     qDebug() << "just added" << query.value(4).toString();
  };

  //nameString->setFocus();

  updateButton = new QPushButton("update");
  updateButton->setEnabled(true);
  finishButton = new QPushButton("finish");
  finishButton->setEnabled(false);


  connect(tab, SIGNAL(currentChanged(int)),
           this, SLOT(tabChanged(int)));

  connect(updateButton, SIGNAL(clicked(bool)),
           this, SLOT(updateButtonClicked(bool)));


  QGridLayout *layout1 = new QGridLayout;
  layout1->addWidget(groupLabel,  1, 0);
  layout1->addWidget(minionLabel, 2, 0);
  layout1->addWidget(tab, 3, 0);

  QGridLayout *layout3 = new QGridLayout;
  layout3->addWidget(updateButton, 0, 0 , 1,1);
  layout3->addWidget(finishButton, 0, 1 , 1,1);

  QGridLayout *layout = new QGridLayout;
  layout->addLayout(layout1,0,0,1,2);
  layout->addLayout(layout3,1,0,1,1);

  setLayout(layout);
  setWindowTitle(tr("Enter new construction trace!"));
  qDebug() << "constructor finished";
};

//---------------------------------------------------------
///
void pLabConstructionTrace::updateButtonClicked(bool)
{
  qDebug() << "update!" << constructionCount;

  if (tab>1)
  {
    qDebug() << constructionTabs[tab]->getName() << constructionTabs[tab]->getEntryText();
    QString entryText = constructionTabs[tab]->getEntryText();
    QString constructionIx = QString().setNum(constructionTabs[tab]->getConstructionIx());
    QString date = QDate::currentDate().toString("yyyy-MM-dd");
    QString stamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    constructionTabs[tab]->addTraceItem(date,entryText);
    for (int n=2; n<=constructionCount; n++)
      if (n == tab) 
        constructionTabs[n]->setEntryText("");
      else
        constructionTabs[n]->setEntryText(entryText);
    QString sql = "INSERT INTO constructionNotes (constructionIx, note, stamp) VALUES "
                  "("+constructionIx+",'"+entryText+"','"+stamp+"');";
    update(sql);
  };

};

void pLabConstructionTrace::statusChanged(int p_status)
{
if (tab>1)
  {
    QString status = QString().setNum(p_status+1);
    QString constructionIx = QString().setNum(constructionTabs[tab]->getConstructionIx());
    QString sql = "UPDATE constructions SET statusIx = "+status+" "
                  "WHERE ix = "+constructionIx;
    update(sql);
  };
};


void pLabConstructionTrace::update(const QString &sqls)
{
  interface->msg(sqls);
  QSqlDatabase db = pMolSql::getConnection("blaj");
  QSqlQuery query;
  bool ok = query.exec(sqls);
  if (!ok) throw pMolError("sql:"+sqls);
};
//-------------------------------------------------------------------
///used by findComponents, finds the MP numbers in the construction 
///instructions and places them in an SQL format (1112,1144,1122) etc
QString pLabConstructionTrace::findNumbers(const QString &source)
{
  int i = 0;
  bool virgin = true;
  QString accumulator = "(";
  qDebug() << source << source.indexOf("MP",i);
  while ((i=source.indexOf("MP", i))>0)
  {
    qDebug() << i;
    i+=2; int j=i;						//skip the MP bit
    while (source.at(j).isDigit()) j++;				//loop to get numbers
    if (j>i) 							//if we got something
    { if (virgin) virgin=false; else accumulator +=", ";	//add comma if needed
      accumulator+=source.mid(i,j-i);				//accumulate the number
    };
  };
  accumulator+=")";
  return accumulator;
};


//-----------------------------------------------------------------
///find components needed for construction
void pLabConstructionTrace::findComponents(QSqlQuery &query)
{
  bool ok;

  findList->setColumnWidth(0,96);
  findList->setColumnWidth(1,148);
  findList->setColumnWidth(2,52);
  QString numbers = findNumbers(make);
  QTreeWidgetItem* plasmids = new QTreeWidgetItem(findList);
  plasmids->setExpanded(true);
  plasmids->setText(0, "plasmids");
  QTreeWidgetItem* oligos = new QTreeWidgetItem(findList);
  oligos->setText(0, "oligos");
  oligos->setExpanded(true);

  if (numbers == "()") return;

  QString sqls;
  sqls="SELECT plasmids.ixix, boxes.name, samples.slot "
       "FROM plasmids, boxes, samples "
       "WHERE plasmids.ixix IN "+numbers+" "
       "AND plasmids.ixix = samples.ixix "
       "AND samples.box = boxes.ix AND samples.present = 1 "
       "AND boxes.disaster_recovery = 0 " 
       "ORDER BY plasmids.ixix;";
  ok = query.exec(sqls);
  if (!ok) throw pMolError(sqls);

  while (query.next())
  {
    QTreeWidgetItem* plasmid = new QTreeWidgetItem(plasmids);
    plasmid->setText(0, "MP"+query.value(0).toString());
    plasmid->setText(1, query.value(1).toString());
    plasmid->setText(2, query.value(2).toString());
  };

  sqls="SELECT oligos.ixix, boxes.name, samples.slot "
       "FROM oligos, boxes, samples "
       " WHERE oligos.ixix IN "+numbers+" "
       "AND oligos.ixix = samples.ixix "
       "AND samples.box = boxes.ix AND samples.present = 1 "
       "AND boxes.disaster_recovery = 0 "
       "ORDER BY oligos.ixix;";
  ok = query.exec(sqls);
  if (!ok) throw pMolError(sqls);

  while (query.next())
  {
    QTreeWidgetItem* oligo = new QTreeWidgetItem(oligos);
    oligo->setText(0, "MP"+query.value(0).toString());
    oligo->setText(1, query.value(1).toString());
    oligo->setText(2, query.value(2).toString());
  };
};

//---------------------------------------------------------
///allocate a number on the database
/*
void pLabConstructionTrace::allocateButtonClicked(bool)
{

  try
  {
    //initial connections
    QSqlDatabase db = pMolSql::getConnection("blah");			//connect to database
    QSqlQuery query;
    QString sql = "INSERT INTO ix (typeIx) VALUES (1);";
    bool ok = query.exec(sql);
//    allocatedIx = query.lastInsertId().toInt();
    QString temp;
//    mpLabel->setText("<h1><b>MP"+temp.setNum(allocatedIx)+"</b></h1>");
    setStatus();
    db.close();
  }

  catch(pMolError &error)
  {
    error.append("pMolFreezerBox::trying to search");
    error.detail(interface);
  }

};
*/
/*
//---------------------------------------------------------
///


/*
void pLabConstructionTrace::tabberChanged(int t)
{
  if 


};
*/

/*
void pLabConstructionTrace::tabChanged(int p_tab)
{
  tab = p_tab;
 if (tab>2) qDebug() << "tab changed" << constructionTabs[p_tab]->getName() << constructionTabs[tab]->getEntryText();
};




pMolStackObject* pLabConstructionTrace::create(pMolKernelInterface* interface, pMolCmd* cmd)
{
  if (cmd==NULL) 
    return makeError(interface, "pMolDNA", "constructor", "no parameters");			
  
  if (cmd->type()!=pMolCmd::LIST) 
    return makeError(interface, "pMolDNA", "constructor", "want parameters not command");

  pMolStackList* objectList = dynamic_cast <pMolStackList*> (interface->interpret(cmd));

  if (!objectList->at(0)->type(pMolStackObject::NUMBER))
    return makeError(interface, "pMolDNA", "constructor", "wrong parameter type");

  pMolStackNumber* n = dynamic_cast <pMolStackNumber*> (objectList->at(0));

  pLabConstructionTrace* entry = new pLabConstructionTrace(interface,   n->n);
  qDebug() << "about to show";
  entry->show();
  qDebug() << "shown@";
  return entry;
};
?*/
