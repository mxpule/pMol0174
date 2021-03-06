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
#include "pMolNameSpace.h"
#include <QtSql>
#include <QtGui/QMessageBox>

#include "pLabTools/pLabFreezerBox.h"
#include "pMolBatch.h"
#include "pMolPrimerOrder.h"
#include "pMolRea.h"
#include "staden/pMolTraceAbi.h"
#include "pMolSynthetics/pMolSyntheticEngine.h"
//#include "pMolSynthetics/geneSplicer.h"
#include "pMolSynthetics/pMolGeneSplicer.h"
#include "pMolEnsembl/pMolEnsembl.h"
#include "pMolSys.h"
#include "pMolTerminal.h"
#include "pMolMpsv/pMolMpsv.h"
#include "pLabTools/pLabToolsFinder.h"
#include "pLabTools/pLabPlasmidEntry.h"
#include "pLabTools/pLabSupesEntry.h"
#include "pLabTools/pLabCellEntry.h"
#include "pLabTools/pLabConstructionOverview.h"
#include "pLabTools/pLabBoxEntry.h"
#include "pLabTools/pLabConstructionTrace.h"
#include "pLabTools/pLabConstruction.h"
#include "pLabFacs/pLabFacs3Dplot.h"


//-------------------------------------------------------------------------------------------------

///constructor - makes sure we have access to the Kernel's interface
pMolNameSpace::pMolNameSpace(pMolKernelInterface* p_interface) 
{ 
  interface = p_interface;
};

///destructor - iterates through the hashtable and deletes objects
pMolNameSpace::~pMolNameSpace()
{
  foreach (pMolObject* object, hashTable) delete object;
};

//-------------------------------------------------------------------------------------------------
///directs creation of loaded objects correctly
pMolXMLbase* pMolNameSpace::XMLopen(const QString &tag, QString &error)
{
  if (tag == "pMolDNA") 
  {
    nascentObject = (pMolStackObject*) new pMolDNA((pMolObject*) this);
    return nascentObject;
  };

  if (tag == "pMolRea") 
  {
    nascentObject = (pMolStackObject*) new pMolRea((pMolObject*) this);
    return nascentObject;
  };


  if (tag == "pMolBatch") 
  {
    nascentObject = (pMolStackObject*) new pMolBatch((pMolObject*) this);
    return nascentObject;
  };

  if (tag == "pMolCodonCodex")
  {
    nascentObject = (pMolStackObject*) new pMolCodonCodex((pMolXMLbase*) this);
    return nascentObject;
  };

  if (tag == "pMolPCRengine")
  {
    nascentObject = (pMolStackObject*) new pMolPCRengine((pMolXMLbase*) this);
    return nascentObject;
  };

  if (tag == "pMolRestrictionEndonuclease")
  {
    nascentObject = (pMolStackObject*) new pMolRestrictionEndonuclease((pMolXMLbase*) this);
    return nascentObject;
  };

  if (tag == "pMolSyntheticEngine")
  {
    nascentObject = (pMolStackObject*) new pMolSyntheticEngine((pMolXMLbase*) this);
    return nascentObject;
  };

  if (tag == "pMolGeneSplicer")
  {
    nascentObject = (pMolStackObject*) new pMolGeneSplicer((pMolXMLbase*) this);
    return nascentObject;
  };

  if (tag == "pMolEnsembl")
  {
    nascentObject = (pMolStackObject*) new pMolEnsembl((pMolXMLbase*) this);
    return nascentObject;
  };

  if (tag == "pMolPeptide")
  {
    nascentObject = (pMolStackObject*) new pMolPeptide((pMolXMLbase*) this);
    return nascentObject;
  };


  return (pMolXMLbase*) this;
};


//-------------------------------------------------------------------------------------------------
///very disinterested here - does nothing other than keeps making sure we get back safe
pMolXMLbase* pMolNameSpace::XMLclose(const QString &tag, const QString &data, QString &error)
{
  return (pMolXMLbase*) this;
};


//-------------------------------------------------------------------------------------------------
///Trys to find corresponding object from the name space
pMolStackObject* pMolNameSpace::lookUp(const QString &name)
{
  //first, tries and looks in the hashTable
  if (hashTable.contains(name)) return hashTable.value(name);

  //next tries and loads corresponding file...needs work, i.e. a path
  //interface->msg("not found in namespace, trying to load from disk...");

  //QString fileName = QDir::currentPath() + "/home/mxpule/pMol/" + name + ".pMol";
  QString fileName = "/home/mxpule/Dropbox/data/pMolSynch/" + name + ".pMol";

  QFile file(fileName);
  if (!file.open(QFile::ReadOnly | QFile::Text)) 
  { interface->msg("failed to open "+fileName);
    return NULL;
  };

  nascentObject = NULL;
  XmolHandler handler((pMolXMLbase *)this);

  QXmlSimpleReader reader;
  reader.setContentHandler(&handler);
  reader.setErrorHandler(&handler);
  QXmlInputSource xmlInputSource(&file);
  reader.parse(xmlInputSource);

  if (nascentObject == NULL)
  { interface->msg("loading XML failed");
    return NULL;
  };


//  hashTable.insert(nascentObject->name, nascentObject);
  hashTable.insert(nascentObject->name, nascentObject);
 // interface->msg("loading XML succeeded");
  return nascentObject;

};


void pMolNameSpace::enter(QString name, pMolStackObject* object)
{
   mutex.lock();
   if (object->name.isNull()) object->name = name;
   hashTable.insert(name, object);
   mutex.unlock();
};

pMolStackObject* pMolNameSpace::error(pMolKernelInterface* interface, QString e)
{
  interface->msg(e);
  return NULL;
};

pMolStackObject* pMolNameSpace::member(pMolKernelInterface* interface, pMolCmd* cmd)
{

  qDebug()<< "reached member";
  //initial check of parameters
  if (cmd==NULL) return error(interface,"member::expecting parameters");//no parameters sent at all
  if (cmd->type()!=pMolCmd::LIST) return error(interface,"member::expecting a list of parameters");
   	//parameter isn't a list
  //cast up as a list

  pMolObject* object = interface->interpret(cmd, (pMolObject*) this);
  if (object==NULL) return error(interface,"member::interpreting command failed");
  //interpret the parameters
  if (!object->type(pMolObject::LIST)) return error(interface,"member::interpreted command is not a list");
  pMolStackList* list = dynamic_cast <pMolStackList*> (object);	

  //get the length and decide what to do next
  if (list->length()!=2) return error(interface,"member::wrong number of parameters");				//leave if no members

  object = list->at(0);
  if (!object->type(pMolObject::LIST)) return error(interface,"member::first parameter is not a list");
  pMolStackList* array = dynamic_cast <pMolStackList*> (object);

  object = list->at(1);
  if (!object->type(pMolObject::NUMBER)) return error(interface,"member::second parameter is not a number");
  pMolStackNumber* index = dynamic_cast <pMolStackNumber*> (object);

  if (index->n < 0) return error(interface,"member::index is negative");
  if (index->n >= array->length()) return error(interface,"member::index out of bounds error");
  interface->msg("member::releasing object");
  return array->at(index->n);
};

bool pMolNameSpace::createConnections()
{
    // create the default database connection
        qDebug() << QSqlDatabase::drivers();
        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
   // if ( db. ) {
   //     qWarning( "Failed to connect to driver" );
   //     return FALSE;
   // }
qDebug() << "sql";
        db.setHostName("db85b.pair.com");
        db.setDatabaseName("mpule2_childhope");
        db.setUserName("mpule2_2");
        db.setPassword("j5Nwduc7");
        bool ok = db.open();
qDebug() << ok;

        QSqlQuery query;
        query.exec("SELECT * FROM members");

        while (query.next()) {
        int ix = query.value(0).toInt();
        QString something = query.value(1).toString();
        qDebug() << ix << something;
        };



}
//-------------------------------------------------------------------------------------------------
///directs creation of loaded objects correctly
pMolStackObject* pMolNameSpace::exec(pMolCmdName* cmd)
{
  //first, let's go through the built-in C++ classes we can execute
  try
  {
  if (cmd->name == "hInsp") 
  {
    vInspector* insp = vInspector::create(interface, cmd->child);
    return NULL; // to be changed to returning the inspector!
  };

  if (cmd->name == "rawSeq")
  {
    qDebug()<< "namespace rawSeq";
    rawSeq* raw = rawSeq::create(interface, cmd->child);
    return NULL;
  };

  if (cmd->name == "sys")
  {
    return pMolSys::sexec(interface, cmd->child);
  };

  if (cmd->name == "mpSV")
  {
    vSequenceMap* map = vSequenceMap::create(interface, cmd->child);
    return NULL;
  }; 

  if (cmd->name == "mp")
  {
    pMolStackObject* map = pMolMpsv::create(interface, cmd->child);
    return map;
  }; 

  if (cmd->name == "dna")
  {
    pMolStackObject* dna = pMolDNA::create(interface, cmd->child);
    return dna;
  };

  if (cmd->name == "peptide")
  {
    pMolPeptide* peptide = pMolPeptide::create(interface, cmd->child);
    return (pMolStackObject*) peptide;
  };


  if (cmd->name == "member")
  {
    return member(interface, cmd->child);

  };

  if (cmd->name == "engine")
  { return pMolMolecularEngine::exec(interface, cmd->child); };


  if (cmd->name == "exit")
  { QCoreApplication::quit();
    return NULL;
  };

  if (cmd->name == "sql")
  { createConnections();
    return NULL;
  };


  if (cmd->name == "primerOrder")
  { return pMolPrimerOrder::create(interface, cmd->child);
  };

  if (cmd->name == "reset")
  { //foreach (pMolObject* object, hashTable) delete object;
    hashTable.clear();
    return NULL;
  };

  //if (cmd->name == "facs3DPlot")
  if (cmd->name == "facs")
  {
    return pLabFacs3DPlot::create(interface, cmd->child);
  };

/*
  if (cmd->name == "new")
  { //foreach (pMolObject* object, hashTable) delete object;
    pLabPlasmidEntry* entry = new pLabPlasmidEntry();
    entry->show();
    return NULL;
  };
*/
  if (cmd->name == "traceAbi")
  { //foreach (pMolObject* object, hashTable) delete object;
    return (pMolStackObject*) pMolTraceAbi::create(interface, cmd->child);
    //pMolTraceAbi* trace = new pMolTraceAbi("./pMol/abi.ab1");
    //return NULL;
  };

  if (cmd->name == "terminal")
  { //foreach (pMolObject* object, hashTable) delete object;

    pMolTerminal* terminal = new pMolTerminal(interface);
    terminal->show();
    terminal->msg("hello, this is pMol version 0.79 -");
    terminal->msg("let's create something together.");
    return NULL;
  };

/*
  if (cmd->name == "geneSplicer")
  {
    pMolGeneSplicerB* splicer = new pMolGeneSplicerB("/home/mxpule/bin/GeneSplicer/sources/fasta.example","/home/mxpule/bin/GeneSplicer/arabidopsis");
    splicer->findSplicing();
    delete splicer;
    return NULL;
  };
*/
/*
  if (cmd->name == "pMolGeneSplicer")
  {
    pMolGeneSplicer* splicer = new pMolGeneSplicer("/home/mxpule/bin/GeneSplicer/sources/fasta.example","/home/mxpule/bin/GeneSplicer/arabidopsis");
    //splicer->findSplicing("test");
    //delete splicer;
    return (pMolStackObject*) splicer;
  };
*/

  if (cmd->name == "about")
  {
  //  kernel->
    QString title = "About pMol";
    QString text  = "pMol started seeping slowly from my mind in mid 2006.\nEnjoy!\n<center>Martin Pule!</center>";
    QMessageBox::about(NULL, title, text);
    return NULL;
  };

  if (cmd->name == "aboutQt")
  {
  //  kernel->
    QString title = "Qt version used with this pMol";
    //QString text  = "pMol started seeped slowly from the brain of Martin Pule in mid 2006.";
    QMessageBox::aboutQt(NULL, title);
    return NULL;
  };

  pMolStackObject* object = lookUp(cmd->name);
  if (object==NULL) return NULL;
  object->loadDependencies(interface);
  if (object->type(pMolStackObject::EXEC))
  {
    pMolStackObject* objectExec = dynamic_cast<pMolStackObject*>(object);
    return objectExec->exec(cmd->child, interface);
  }
  return object;
  }
  catch(pMolError &error)
  {
    error.detail(interface);
    return NULL;
  };
};

//-------------------------------------------------------------------------------------------------

