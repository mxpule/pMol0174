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

#ifndef PMOLENSEMBL_H
#define PMOLENSEMBL_H

#include <QString>
#include <math.h>

#include "pMolStackObject.h"
#include "pMolDNA.h"
#include "pMolPrimer.h"
#include "pMolKernelInterface.h"
#include "pMolRestrictionEndonuclease.h"
#include "pMolCmd.h"

//ensembl allows anonymous access to its data structure.
//transcript table provides basic data on the transcipt, i.e. does it exist, protein coding
//exon table linked back to trascript_stable_id table and transcript gives list of exons
//exon sequences refer to a seq_region_id (with start and end locations and orientation)
//the basic structure of ensembl is that "seq_regions"  start off abstract  and large and are fragmented into
//progressively smaller bits until a seq_region_id can be found in the table dna which has real raw sequence
//so we first look up seq_region_id in table dna, if it's not here we look it up in assembly table
//we next adjust the sequence we're looking for relative to assembly region we've found and
//recurse (i.e. look in dna, if not there keep going further down the assembly)
//this is a bit painful because the assembly bits have their own offset (cmp_start) and can be in
//different orientations (ori). Finally, a long sequence can be found in several seq_regions in dna
//table. So far (since exons are quite short), this class only supports breakage into a 5' and 3' bit.
//If you try to extract a very long piece of dna which spans more than two seq_regions not supported

///Ensembl is a joint project between EMBL and the Wellcome Trust Sanger Institute to develop a software
///system which produces and maintains automatic annotation on eukaryotic genomes. It is mySQL based,
///making it very easy to interface with, especially since ensembl has annonymously open mySQL servers.
///If you are using this feature heavily, please consider installing ensembl mySQL server locally.
///Currently this class only pulls out transcript data x:=ensembl->get('ENST000???????')
///exons, reading frame and some basic annotations are loaded automatically with the sequence
///remember to keep checking ensembl, as they often put up new builds! Change your dbName accordingly
///typical data base names are e.g. mus_musculus_core_42_36c or homo_sapiens_core_42_36d
class pMolEnsembl : public pMolStackObject
{
  public:
    pMolEnsembl();
    pMolEnsembl(pMolXMLbase * p_p_MolXMLbaseParent);

    virtual pMolXMLbase* XMLclose(const QString &tag, const QString &data, QString &error);
    virtual pMolStackObject* exec(pMolCmd* cmd, pMolKernelInterface* interface);
    virtual void treeOut(QTreeWidgetItem *tree);
    virtual bool type(int i);

    pMolStackObject* command(pMolCmdName* cmdName, pMolKernelInterface* interface);

  private:
    ///the ensembl database address you want to access. Public one is 'sql.ensembl.org'
    QString dbAddress;

    ///the database user one, if using public database, set to 'anonymous'
    QString dbUser;

    ///this is important, this is the database you want to access. e.g. homo_sapiens_core_42_36d.
    ///this will determine if you are accessing a human, mouse or duck-billed platybus genome.
    ///Also, as ensembl refine their build, they come out with new databases - so keep checking
    QString dbName;

    ///password
    QString dbPass;

    ///This is the codon codex you want used for translation. If human - use a human one etc.
    QString codonCodexName;

    pMolStackObject* c_get(pMolCmd* cmd, pMolKernelInterface* interface);
    QString getSequence(QSqlDatabase* db, qulonglong seq_region_id, qulonglong seq_region_start, qulonglong seq_region_end, short int strand);
    QString getSequenceRecurse(QSqlDatabase* db, qulonglong seq_region_id, qulonglong seq_region_start, qulonglong seq_region_end, short int strand);
};

#endif
