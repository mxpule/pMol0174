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

#include "/pLabTools/pLabFreezerCryoBox.h"

pLabFreezerCryoBox::pLabFreezerCryoBox(pMolKernelInterface* p_interface, int box, int highlightSlotIx = -1)
{
  boxIx = box;
  interface = p_interface;
  shiftDown = false;
  boxNum.setNum(box);

  try
  {
    for (int i=0; i<82; i++) ixArray[i] = new pLabToolsSlotWidget(radius, this);

    // create the default database connection
    QSqlDatabase db = pMolSql::getConnection("blah");
    QSqlQuery query = QSqlQuery(db);

    //get the box name and set as the title on the window
    query.exec("SELECT name, ix, type FROM boxes WHERE ix = "+boxNum);
    if (query.next()) 
    { boxName = query.value(0).toString();
      boxType = query.value(2).toInt();
      setWindowTitle("MP"+boxNum+":"+boxName);
    }
    else throw pMolError("box not found");

    //populate the box slots
    switch(boxType)
    {
      case BOXTYPE_OLIGOS   	: slotsPopulateOligos(query, highlightSlotIx);
  				  boxTypeName = "oligos";	break;
      case BOXTYPE_PLASMIDS 	: slotsPopulatePlasmids(query, highlightSlotIx);
				  boxTypeName = "plasmids"; 	break;
      case BOXTYPE_SUPES    	: slotsPopulateSupes(query, highlightSlotIx); 
				  boxTypeName = "supes";	break;
      default 		: throw("box type not known");
    };

   qDebug() << "got to here" << boxTypeName;

  //put in the slots into the frame
  QGridLayout *layout = new QGridLayout;
  for (int x=0; x<9; x++)
  {
    QString temp;
    QLabel* axisLabel;
    axisLabel = new QLabel(temp.setNum(x+1));
    layout->addWidget(axisLabel, 1, p*x+1, 1, p);
    axisLabel = new QLabel(QChar(65+x));
    layout->addWidget(axisLabel, p*x+2, 0, p, 1);

    for (int y=0; y<9; y++)
      layout->addWidget(ixArray[x+y*9], p*y+2,p*x+1, p, p);
  }

   qDebug() << "got to here now" << boxTypeName;

  //general layout stuff
  setLayout(layout);
  QSizePolicy policy = QSizePolicy();
  policy.setHorizontalStretch(0);
  policy.setVerticalStretch(0);
  setSizePolicy(policy);

   qDebug() << "got to here now -1" << boxTypeName;

  //labels and buttons
  QLabel* boxLabel = new QLabel("Select box: ", this);
  layout->addWidget(boxLabel, 0, 0, 1, (int)p*1.5);
 // layout->addWidget(boxCombo, 0, p*1.5, 1, (int)(7.5*p+1));

   qDebug() << "got to here now -2" << boxTypeName;
  QPushButton *addButton = new QPushButton("add");
  layout->addWidget(addButton, 11*p, 0, 1, 2*p);

  QPushButton *removeButton = new QPushButton("remove");
  layout->addWidget(removeButton, 11*p, 2*p, 1, 2*p);

  QPushButton *refreshButton = new QPushButton("refresh");
  layout->addWidget(refreshButton, 11*p, 4*p, 1, 2*p);

  QPushButton *emptyButton = new QPushButton("empty");
  layout->addWidget(emptyButton, 11*p, 6*p, 1, 2*p);

  QLabel *searchLabel = new QLabel("search");
  layout->addWidget(searchLabel, 11*p+1, 0, 1, p);

   qDebug() << "got to here now -3" << boxTypeName;
  searchCombo = new QComboBox(this);
  searchCombo->insertItem(1,"blah blah");
  searchCombo->insertItem(2,"more more");
  searchCombo->setEditable(true);
  searchCombo->setInsertPolicy(QComboBox::InsertAtTop);
  layout->addWidget(searchCombo, 11*p+1, p, 1, 8*p+1);

   qDebug() << "got to here now -4" << boxTypeName;
  label = new QTextBrowser(this);
  label->setFrameStyle(QFrame::Panel | QFrame::Sunken);
  label->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
  label->setMaximumHeight(64);
  layout->addWidget(label, 11*p+2, 0, 2, 9*p+1);

   qDebug() << "got to here then" << boxTypeName;

  //connect up the signals and slots
  connect(searchCombo, SIGNAL(editTextChanged(const QString &)),
            this, SLOT(searchComboChanged(const QString &)));

  connect(searchCombo, SIGNAL(activated(int)),
            this, SLOT(searchComboActivated(int)));

  connect(addButton, SIGNAL(pressed()),
            this, SLOT(addItemPressed()));

  connect(removeButton, SIGNAL(pressed()),
            this, SLOT(removeItemPressed()));
  }

  //ERROR HANDLING
  catch(pMolError &error)
  {
    error.append("pLabFreezerBox::constructor");
    error.detail(interface);
  };

  show();

};