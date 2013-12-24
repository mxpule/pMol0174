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
#include "pMolEnsembl.h"


pMolEnsembl::pMolEnsembl() : pMolStackObject(NULL)
{
};


pMolEnsembl::pMolEnsembl(pMolXMLbase *p_pMolXMLbaseParent) : pMolStackObject(p_pMolXMLbaseParent)
{
};


//--------------------------------------------------------------------------------------------------------------------
bool pMolEnsembl::type(int i)
{
  if (i == pMolStackObject::EXEC) return true;
  return pMolStackObject::type(i);
};


//--------------------------------------------------------------------------------------------------------------------
pMolXMLbase* pMolEnsembl::XMLclose(const QString &tag, const QString &data, QString &error)
{
  if (tag=="dbAddress")			dbAddress = data;
  else if (tag=="dbUser") 		dbUser = data;
  else if (tag=="dbPass")		dbPass = data;
  else if (tag=="dbName")		dbName = data;
  else if (tag=="codonCodex")		codonCodexName = data;
  else return pMolXMLbase::XMLclose(tag, data,error);

  return (pMolXMLbase*) this;
}

//--------------------------------------------------------------------------------------------------------------------
///returns tree structure of this class
 void pMolEnsembl::treeOut(QTreeWidgetItem *tree)
{
  //send out the basic stuff
  pMolXMLbase::treeOut(tree);
  treeItem(tree,"dbAddress",dbAddress);
  treeItem(tree,"dbUser",dbUser);
  treeItem(tree,"dbPass",dbPass);
  treeItem(tree,"dbName",dbName);
  treeItem(tree,"codonCodex", codonCodexName);
};


//--------------------------------------------------------------------------------------------------------------------
///Recurses through the ensembl structure to find the raw sequence data
QString pMolEnsembl::getSequenceRecurse(QSqlDatabase* db, qulonglong seq_region_id, qulonglong seq_region_start, qulonglong seq_region_end, short int strand)
{
  bool ok;
  QString sql;
  short int ori;
  QSqlQuery query;
  qlonglong cmp_start;
  QString temp1, temp2, temp3;
  qulonglong cmp_seq_region_id, asm_start, asm_end;

  //look in ensembl assembly where there is a region id segment containing the sequence
  sql = "SELECT * FROM assembly WHERE assembly.asm_seq_region_id="+temp1.setNum(seq_region_id) + " ";
  sql+= "AND asm_start < "+temp2.setNum(seq_region_start) + " ";
  sql+= "AND asm_end > "+temp3.setNum(seq_region_end)+ " ";
  sql+= "ORDER BY cmp_seq_region_id;";
  ok = query.exec(sql);
  if (!ok) {qDebug() << "error"; return NULL; }

  //is there a sequence we can get?
  if (query.size() > 0)
  {
    query.next();
    cmp_seq_region_id = query.value(1).toULongLong();		//cmp_seq_region_id
    asm_start = query.value(2).toULongLong();			//asm_start
    asm_end = query.value(3).toULongLong();
    cmp_start = query.value(4).toLongLong();
    ori = query.value(6).toInt();

    //now go and get the raw sequence (converted to correct relative coordinates to what we want)
    if (ori>0)
      return getSequence(db,
        cmp_seq_region_id, seq_region_start-asm_start+cmp_start,
        seq_region_end-asm_start+ cmp_start+1, strand * ori);
    else
      return getSequence(db,
        cmp_seq_region_id, asm_end-seq_region_end+cmp_start,
        asm_end-seq_region_start+cmp_start+1, strand * ori);
  }

  else
  //likely explanation if nothing found is where the sequence we want is broken into two or more dna segments
  //lets get the 5' end first, then the 3' end next. Situations where we try and get such a long sequence that
  //spans more than two ensembl seqments is not supported yet.
  {
    QString seq5, seq3;

    //lets get the 5' bit
    sql = "SELECT * FROM assembly WHERE assembly.asm_seq_region_id="+temp1.setNum(seq_region_id) + " ";
    sql+= "AND asm_start < "+temp2.setNum(seq_region_start) + " ";
    sql+= "AND asm_end > "+temp3.setNum(seq_region_start)+ " ";		//limit to segments just at the point we want
    sql+= "ORDER BY cmp_seq_region_id;";
    ok = query.exec(sql);
    if (!ok) {qDebug() << "error in 5' split"; return NULL; };
    if (query.size() < 1) { qDebug() << "5' end not found"; return NULL; };
    query.next();
    cmp_seq_region_id = query.value(1).toULongLong();
    asm_start = query.value(2).toULongLong();
    asm_end = query.value(3).toULongLong();
    cmp_start = query.value(4).toLongLong();
    ori = query.value(6).toInt();

    if (ori>0)
      seq5 = getSequence(db, cmp_seq_region_id, seq_region_start-asm_start+cmp_start, asm_end-asm_start+ cmp_start+1, strand * ori);
    else
      seq5 = getSequence(db, cmp_seq_region_id, cmp_start, asm_end-seq_region_start+cmp_start+1, strand * ori);
    if (seq5.isNull()) { qDebug() << "failed to get raw sequence for 5' end"; return NULL; };

    //lets get the 3' bit
    sql = "SELECT * FROM assembly WHERE assembly.asm_seq_region_id="+temp1.setNum(seq_region_id) + " ";
    sql+= "AND asm_end > "+temp2.setNum(seq_region_end) + " ";
    sql+= "AND asm_start < "+temp3.setNum(seq_region_end)+ " ";
    sql+= "ORDER BY cmp_seq_region_id;";
    ok = query.exec(sql);
    if (!ok) {qDebug() << "error in 3' split"; return NULL; };
    if (query.size() < 1) { qDebug() << "3' end not found"; return NULL; };
    query.next();
    cmp_seq_region_id = query.value(1).toULongLong();
    asm_start = query.value(2).toULongLong();
    asm_end = query.value(3).toULongLong();
    cmp_start = query.value(4).toLongLong();
    ori = query.value(6).toInt();

    if (ori>0)
      seq3 = getSequence(db, cmp_seq_region_id, cmp_start, seq_region_end-asm_start+ cmp_start+1, strand * ori);
    else
      seq3 = getSequence(db, cmp_seq_region_id, asm_end-seq_region_end+cmp_start, asm_end-asm_end+cmp_start+1, strand * ori);
    if (seq3.isNull()) { qDebug() << "failed to get raw sequence for 3' end"; return NULL; };

    //done, return the two seqments
    return seq5 + seq3;
  };
};


//--------------------------------------------------------------------------------------------------------------------
///tries to get raw sequence, if it doesn't find it calls above sister function to recurse until found
QString pMolEnsembl::getSequence(QSqlDatabase* db, qulonglong seq_region_id, qulonglong seq_region_start, qulonglong seq_region_end, short int strand)
{
  bool ok;
  QString sql,temp1, temp2, temp3;

  //is this sequence present in raw dna table?
  qDebug() << "searching seq_id" << seq_region_id << seq_region_start << seq_region_end << strand;
  sql = "SELECT * FROM dna WHERE seq_region_id=" + temp1.setNum(seq_region_id) + ";";
  QSqlQuery query;
  ok = query.exec(sql);
  if (!ok) return NULL;

  //if this sequence needs to be referred downwards
  if (query.size()<1) return getSequenceRecurse(db,seq_region_id, seq_region_start, seq_region_end, strand);

  qDebug() << "raw reached";
  sql = "SELECT SUBSTRING(sequence,";
  sql+= temp1.setNum(seq_region_start)+",";
  sql+= temp2.setNum(seq_region_end - seq_region_start)+") ";
  sql+= "from dna ";
  sql+= " WHERE seq_region_id = "+ temp3.setNum(seq_region_id)+";";
  qDebug() << sql;
  ok = query.exec(sql);
  if (!ok) {qDebug() << "error trying to get sequence"+query.lastError().text(); return NULL; }
  if (query.size() < 1) { qDebug() << "not found sequence"; return NULL; }
  query.next();
  qDebug() << query.value(0).toString();
  if (strand > 0)
    return query.value(0).toString();
  else
    return pMolElement::complementString(query.value(0).toString());
};


//------------------------------------------------------------------------------------------------------------------
///
pMolStackObject* pMolEnsembl::c_get(pMolCmd* cmd, pMolKernelInterface* interface)
{
  QString error, temp1, temp2, temp3;

  //if we didn't get any parameters, then send an error and return
  pMolStackList* list = pMolStackList::interpretParameter(cmd, interface,error);
  if (list==NULL) return makeError(interface, "pMolPrimerOrder", "create", error);

  //if there's one number parameter, cut bit off front or back depending if pos or neg
  if (!list->checkParameters(pMolObject::STRING, error))
    return makeError(interface, "pMolEnsembl", "create", error);

  pMolStackString* s = dynamic_cast <pMolStackString*> (list->at(0));

  QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

  db.setHostName(dbAddress);
  db.setDatabaseName(dbName);
  db.setUserName(dbUser);
  db.setPassword(dbPass);
  bool ok = db.open();

  if (!ok) return makeError(interface, "pMolPrimerOrder", "create", "couldn't open db");

  QString sql;
  QSqlQuery query;

  //let's get the basics about this transcript: does it exist? what is it's volatile id? Is it protein coding?
  sql = "SELECT transcript.transcript_id, transcript.biotype FROM transcript, transcript_stable_id ";
  sql+= "WHERE transcript_stable_id.stable_id = '" + s->string +"' ";
  sql+= "AND transcript.transcript_id = transcript_stable_id.transcript_id;";
  ok = query.exec(sql);
  if (!ok) return makeError(interface, "pMolEnsembl", "open", "SQL command failed: "+sql);
  if (!query.next()) return makeError(interface, "pMolEnsembl", "open", "couldn't find transcript: "+s->string);
  unsigned long int transcript_id = query.value(0).toULongLong();
  bool protein_coding;
  if (query.value(1).toString() == "protein_coding") protein_coding = true; else protein_coding = false;

  //let's go through the exons and assemble their sequences
  sql = "SELECT exon_transcript.rank, exon.* FROM exon, transcript, exon_transcript, transcript_stable_id WHERE transcript_stable_id.stable_id = '"+s->string+"' AND transcript.transcript_id = transcript_stable_id.transcript_id AND exon_transcript.transcript_id = transcript.transcript_id AND exon_transcript.exon_id = exon.exon_id;";
  ok = query.exec(sql);
  if (!ok) return makeError(interface, "pMolEnsembl", "create", "SQL command failed: "+sql);

  QString a = "";
  pMolDNA* dna = new pMolDNA();
  dna->name = s->string;
  dna->setIndex(0);

  while (query.next())
  {
    QString temp;
    unsigned int rank = query.value(0).toInt();
    unsigned long int exon_id = query.value(1).toULongLong();
    unsigned long int seq_region_id = query.value(2).toULongLong();
    unsigned long int seq_region_start = query.value(3).toULongLong();
    unsigned long int seq_region_end = query.value(4).toULongLong();
    short int strand = query.value(5).toInt();
    qDebug() << exon_id << seq_region_id << seq_region_start << seq_region_end;
    temp3 = getSequence(&db, seq_region_id, seq_region_start, seq_region_end, strand);
    if (!temp3.isNull()) dna->extend3("exon_"+temp1.setNum(rank),"exon_id="+temp2.setNum(exon_id), temp3);
  };

  //if it's protein coding transcript, let's annotate the ORF
  if (protein_coding)
  { //find out which exon translation starts and ends
    sql = "SELECT * FROM translation, translation_stable_id WHERE translation.transcript_id = " + temp1.setNum(transcript_id)+" ";
    sql+= "AND translation.translation_id = translation_stable_id.translation_id;";
    ok = query.exec(sql);
    if (!ok) return makeError(interface, "pMolEnsembl", "get translation", "SQL command failed: "+sql);
    if (!query.next()) return makeError(interface, "pMolEnsembl", "open", "couldn't find translation");
    qulonglong translation_id = query.value(0).toULongLong();			//translation_id
    qulonglong seq_start = query.value(2).toULongLong();			//seq_start, start of orf in
    qulonglong start_exon_id = query.value(3).toULongLong();			//start_exon_id, this exon
    qulonglong seq_end = query.value(4).toULongLong();
    qulonglong end_exon_id = query.value(5).toULongLong();
    QString translation_stable_id = query.value(7).toString();


    //get the rank of the starting exon and work out index
    sql = "SELECT rank FROM exon_transcript WHERE ";
    sql+= "exon_id = "+temp1.setNum(start_exon_id) + " AND ";
    sql+= "transcript_id = "+temp2.setNum(transcript_id) +";";
    ok = query.exec(sql);
    if (!ok) return makeError(interface, "pMolEnsembl", "get orf starting exon", "SQL command failed: "+sql);
    if (!query.next()) return makeError(interface, "pMolEnsembl", "get orf starting exon", "couldn't find exon");
    qulonglong start_exon_rank = query.value(0).toULongLong();

    int i = (int) start_exon_rank - 1;
    pMolElement* start_exon = dna->getElement(&i);
    if (start_exon == NULL) return makeError(interface, "pMolEnsembl", "start exon not found in assembled molecule");
    int index = start_exon->getIndex() + (int) seq_start;

    //get the rank of the ending exon and work out length
    sql = "SELECT rank FROM exon_transcript WHERE ";
    sql+= "exon_id = "+temp1.setNum(end_exon_id) + " AND ";
    sql+= "transcript_id = "+temp2.setNum(transcript_id) +";";
    ok = query.exec(sql);
    if (!ok) return makeError(interface, "pMolEnsembl", "get orf ending exon", "SQL command failed: "+sql);
    if (!query.next()) return makeError(interface, "pMolEnsembl", "get orf ending exon", "couldn't find exon");
    qulonglong end_exon_rank = query.value(0).toULongLong();

    int j = (int) end_exon_rank - 1;
    pMolElement* end_exon = dna->getElement(&j);
    if (end_exon == NULL) return makeError(interface, "pMolEnsembl", "end exon not found in assembled molecule");
    int length = end_exon->getIndex() + (int) seq_end - index;

    //annotate in ORF
    pMolORF* orf = new pMolORF();
    orf->setIndex(index-1);
    orf->setLength(length+1);
    orf->codonCodexName = codonCodexName;
    orf->name = "orf";
    orf->addComment("ensembl stable id = "+translation_stable_id);
    dna->append((pMolElement*) orf);

    //put in the protein features
    sql = "SELECT protein_feature.seq_start, protein_feature.seq_end, protein_feature.hit_id,";
    sql+= "analysis_description.display_label,analysis_description.description ";
    sql+= "FROM translation, protein_feature, analysis_description ";
    sql+= "WHERE translation.translation_id = "+temp1.setNum(translation_id)+ " ";
    sql+= "AND protein_feature.translation_id = translation.translation_id ";
    sql+= "AND analysis_description.analysis_id = protein_feature.analysis_id ";
    sql+= "AND (analysis_description.display_label = 'tmhmm' ";
    sql+= "OR analysis_description.display_label = 'Signalp');";
    ok = query.exec(sql);
    if (!ok) return makeError(interface, "pMolEnsembl", "problem getting protein features", "SQL command failed: "+sql);

    while (query.next())
    {
      qulonglong feature_seq_start = query.value(0).toULongLong();
      qulonglong feature_seq_end   = query.value(1).toULongLong();
      QString feature_hit_id       = query.value(2).toString();
      QString feature_label        = query.value(3).toString();
      QString feature_description  = query.value(4).toString();
      orf->annotate(feature_label, feature_description, (int) 3*(feature_seq_start-1), (int) 3*(feature_seq_end - feature_seq_start));
    };


  };

  //clean up database stuff
  db.close();
  QSqlDatabase::removeDatabase("db");

  //return the newly made molecule
  return (pMolStackObject*) dna;
};

/*
USE homo_sapiens_core_41_36c;
SELECT transcript.biotype, exon_transcript.rank, exon.* FROM exon, transcript, exon_transcript, transcript_stable_id WHERE transcript_stable_id.stable_id = 'ENST00000361719' AND transcript.transcript_id = transcript_stable_id.transcript_id AND exon_transcript.transcript_id = transcript.transcript_id AND exon_transcript.exon_id = exon.exon_id;
*/


//------------------------------------------------------------------------------------------------------------------
///This executes commands directed at this object
pMolStackObject* pMolEnsembl::command(pMolCmdName* cmdName, pMolKernelInterface* interface)
{
  if (cmdName->name == "get")  return c_get(cmdName->child, interface);
  return makeError(interface, "exec", "command '"+cmdName->name+"' not understood");
};

//------------------------------------------------------------------------------------------------------------------
///This handles command directed at this oject
pMolStackObject* pMolEnsembl::exec(pMolCmd* cmd, pMolKernelInterface* interface)
{
  //no paramaters, return this as object
  if (cmd==NULL) return (pMolStackObject*) this;				//no paramaters, return me

  //if no command but not empty, something is wrong (needs to be empty or have subcommand) so return with error
  if (cmd->type() != pMolCmd::NAME) return makeError(interface,"exec","empty field or parameter list expected");
  pMolCmdName* cmdName = dynamic_cast<pMolCmdName*>(cmd);			//safe to assume this is a pMolCmdName

  return command(cmdName, interface);
};
