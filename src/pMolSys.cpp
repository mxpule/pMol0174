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
#include "pMolSys.h"
#include <QApplication>

pMolSys::pMolSys(): pMolStackObject()
{
}



//---------------------------------------------------------------------
///copy sequence to the clipboard!

pMolStackObject* pMolSys::c_copy(pMolKernelInterface* interface, pMolCmd* cmd)
{
  QString error, sequence, name;

  //if we didn't get any parameters, then send an error and return
  pMolStackList* list = pMolStackList::interpretParameter(cmd, interface, error);
  if (list==NULL) return makeError(interface, "pMolSys", "prime", error);

  //if there's one number parameter, cut bit off front or back depending if pos or neg
  if (list->checkParameters(pMolObject::DNA, error))
  {
    pMolElement* dna = dynamic_cast <pMolElement*> (list->at(0)); 
    QClipboard *clipboard = QApplication::clipboard();
    QString copy=">"+dna->getName()+"\n"+dna->getSequence();
    clipboard->setText(copy);
    return NULL;
  }

  if (list->checkParameters(pMolObject::PEPTIDE, error))
  {
    pMolPeptide* pep = dynamic_cast <pMolPeptide*> (list->at(0)); 
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(pep->getSequence());
    return NULL;
  }

  return makeError(interface, "pMolSys", "copy", "wrong parameter type");
};


//---------------------------------------------------------------------
///find pMol Objects!

pMolStackObject* pMolSys::c_find(pMolKernelInterface* interface, pMolCmd* cmd)
{
  QString error, sequence, name;

  //if we didn't get any parameters, then send an error and return
  pMolStackList* list = pMolStackList::interpretParameter(cmd, interface, error);
  if (list==NULL) return makeError(interface, "pMolSys", "find", error);

  //if there's one number parameter, cut bit off front or back depending if pos or neg
  if (!list->checkParameters(pMolObject::STRING, error))
    return makeError(interface, "pMolSys", "find", error);

  pMolStackString* p_sstring = dynamic_cast <pMolStackString*> (list->at(0));
  QString sstring = p_sstring->string;
  sstring = "*"+sstring+"*.pMol";

  QDir directory("/home/mxpule/Dropbox/data/pMolSynch");
  QStringList filters;
  filters << sstring;
  directory.setNameFilters(filters);
  QStringList files = directory.entryList();

  for (int i=0; i < files.size(); ++i)
  {
     QString s = files.at(i);
     interface->msg(s.left(s.length()-5));
};
  return NULL;
};

//---------------------------------------------------------------------
///sys->fasta; this imports from FASTA format

pMolStackObject* pMolSys::c_fasta(pMolKernelInterface* interface, pMolCmd* cmd)
{
  QString error, name;
  QString sequence = "";

  //if we didn't get any parameters, then send an error and return
  pMolStackList* list = pMolStackList::interpretParameter(cmd, interface, error);
  if (list==NULL) return makeError(interface, "pMolSys", "find", error);

  //if there's one number parameter, cut bit off front or back depending if pos or neg
  if (!list->checkParameters(pMolObject::STRING, error))
    return makeError(interface, "pMolSys", "find", error);

  pMolStackString* p_sstring = dynamic_cast <pMolStackString*> (list->at(0));
  QString filename = p_sstring->string;
  QString directory = "/home/mxpule/Dropbox/data/molecules/";

  QFile file(directory+filename);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    interface->msg("couldn't open file:"+directory+filename);
    return NULL;
  };

  pMolDNA* dna = new pMolDNA("");

  QTextStream stream(&file);
  while (!stream.atEnd())
  {
     QString line = stream.readLine();
     QString rawSequence = "";
     QString filter = "CAGTN";

     //file accession name
     if (line.contains("ACCESSION"))
     {
       QString name = line.right(10);
       name = name.trimmed();
       dna->name = name;
       interface->msg(":NAME:"+name);
     }

     //sequence data
     else if (line.contains("ORIGIN"))
     {
       line = stream.readLine();
       while (!line.contains("//") && !stream.atEnd())
       {
         rawSequence.append(line);
         line = stream.readLine();
       };
       rawSequence = rawSequence.toUpper();
       for (int n=0;n<rawSequence.length();n++)
       {
          QChar c = rawSequence.at(n);
          if (filter.contains(c)) sequence.append(c);
       };
       dna->extend3("","",sequence);
     }

     //open reading frame
     else if (line.contains("CDS"))
     {
       int i = line.indexOf("CDS");
       int j = line.indexOf("..");
       int l = line.length();
       QString org = line.mid(i+4,j-i-4);
       QString end = line.mid(j+2,l-j-2);
       line = stream.readLine();
       i = line.indexOf("gene=");
       l = line.length();
       QString name = line.mid(i+6, l-i-7);

       i = org.toInt()-1;
       l = end.toInt()-i;
       pMolORF* orf = new pMolORF();
       orf->pMolParent = dna;
       orf->setIndex(i);
       orf->setLength(l);
       orf->name = name;
       orf->codonCodexName = "HomoSapiensCodonCodex";
       orf->addComment("FASTA IMPORT");
       dna->append((pMolElement*) orf);
     }

     //annotation
     else if (line.contains("misc_feature"))
     {
       int i = line.indexOf("misc_feature");
       int j = line.indexOf("..");
       int l = line.length();
       QString org = line.mid(i+15,j-i-15);
       QString end = line.mid(j+2,l-j-2);
       line = stream.readLine();
       i = line.indexOf("gene=");
       l = line.length();
       QString name = line.mid(i+6, l-i-7);
       i = org.toInt()-1;
       l = end.toInt()-i;
       dna->annotate(name,"FASTA IMPORT",i,l);
     }

     else
     {
       dna->addComment(line);
     };
  }

  return dna;

/*

  QString temp;
  int l = ORFlength3(dna,n0);
  interface->msg(name+"::ORF:"+temp.setNum(l)+";"+dna->getSequence(n0,24)+";"+translate(dna->getSequence(n0,24)));
  pMolORF* orf = new pMolORF();
  orf->pMolParent = dna;
  orf->setIndex(n0);
  orf->setLength(l);
  orf->name = "ORF:"+temp.setNum(n0);
  orf->codonCodexName = name;

  layer->append((pMolElement*) orf);
  return layer;
*/

  return NULL;
};


/*
pMolStackObject* pMolSys::c_find
{
  if (p1=="plasmid")

  if (p1=="oligo")
  {
     QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

  db.setHostName("db87a.pair.com");
  db.setDatabaseName("mpule2_pMol");
  db.setUserName("mpule2_3_w");
  db.setPassword("a4SzFTDJ");
  bool ok = db.open();

  if (!ok) return makeError(interface, "pMolPrimerOrder", "create", "couldn't open db");

  QSqlQuery query;
  sql = "SELECT ixix, sequence, name FROM oligos WHERE "+sql;
  ok = query.exec(sql);
  if (!ok) return makeError(interface, "pMolPrimerOrder", "create", "SELECT command failed: "+sql);

  QString a = "";

  if (tp=="label")
  {
    a+="<html><body><table>\n";
    int i = 0;
    while (query.next()) 
    { 
      if (i % 6 == 0) a+="<tr>\n";
      QString temp;
      int ixix = query.value(0).toInt();
      QString sequence = query.value(1).toString();
      QString name = query.value(2).toString();
      a+="<td><B>MP"+temp.setNum(ixix)+"</B>100uM<br>"+name+"<br>"+sequence.left(8)+"..."+sequence.right(8)+"</td>\n";
      if (++i % 6==0) a+="</tr>\n";
    };
    a+="</table></body></html>\n";
  }


  if (p1=="location")
*/
//----------------------------------------------------------------------------------------------------
///executes commands referred to this object
pMolStackObject* pMolSys::sexec(pMolKernelInterface* interface, pMolCmd* cmd )
{
  //no paramaters, return this as object
  if (cmd==NULL) 
    return makeError(interface, "pMolSys", "exec", "no command");	//no paramaters, return me

  //if no command but not empty, something is wrong (needs to be empty or have subcommand) so return with error
  if (cmd->type() != pMolCmd::NAME) 
    return makeError(interface, "pMolSys", "exec", "command expected");

  //first let's look in our own little internal namespace
  pMolCmdName* cmdName = dynamic_cast<pMolCmdName*>(cmd);					//safe to assume this is a pMolCmdName

  if (cmdName->name=="copy") 		return c_copy(interface, cmdName->child);	else
  if (cmdName->name=="find")            return c_find(interface, cmdName->child);       else
  if (cmdName->name=="fasta")           return c_fasta(interface, cmdName->child);       else
    return makeError(interface, "pMolSys", "exec", "command not understood");
};
