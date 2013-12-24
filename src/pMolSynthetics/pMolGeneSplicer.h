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

/*  Derived from TIGR's gene splicer 
 *  Copyright (c) 2003, The Institute for Genomic Research (TIGR), Rockville,
 *  Maryland, U.S.A.  All rights reserved.
 *   genesplicer.cpp was designed by Mihaela PERTEA to find splice 
 *   sites in a fasta file
*/

#ifndef PMOLGENESPLICER_H
#define PMOLGENESPLICER_H

#include <QString>
#include <QtGui>

#include <math.h>

#include "pMolStackObject.h"
#include "pMolKernelInterface.h"
#include "pMolGeneSplicerTree.h"
#include "pMolGeneSplicerTable.h"

///This is my bastardization of TIGR's splicing HMM. 
///<br>* note, I got rid of the splice searching on the complentary strand - in my mind, this would not be useful for this application, where splicing detection will be used for codon optimization or assessing the stability of mRNA. We will a priori know the direction of transcription. If you want, you could always put through the complementary strand manually
///<br>*I must say that it was very badly written code. When you see something coming from TIGR, you assume to be humbled by the artistry and perfection of the code but actually it was really terrible. Have they never heard of XML? Commenting their code?
///<br>*Also I have translated their training data / trees etc into XML but only for homo sapiens, in their source they give options for other organisms. I will put the source code into pMol notes if you ever want to get their data for another organism. Probably better off writing denovo a script of some kind. I'm very unlikely to need to re-programme another organism except homosapiens at the moment. Sorry!
class pMolGeneSplicer : public pMolStackObject
{
public:
  pMolGeneSplicer(pMolXMLbase * p_pMolXMLbaseParent);
  ~pMolGeneSplicer();

  virtual bool type(int i);
  virtual void treeOut(QTreeWidgetItem *tree);
  virtual pMolStackObject* exec(pMolCmd* cmd, pMolKernelInterface* interface);
  virtual pMolXMLbase* XMLopen(const QString &tag, QString &error);
  virtual pMolXMLbase* XMLclose(const QString &tag, const QString &data, QString &error);
  virtual bool saveContents(QTextStream* stream);
  bool findSplicing(const QString &seq);

private:
  QString error;
  bool suberror(const QString &function, const QString &message);

  struct Site
  {
    int type;
    long pos;
    double score;
    Site *prev;
    Site *forw;
    int dir;
  };
  static const int spliceTablesMax = 300;

  pMolGeneSplicerTable* spliceDonorTable;
  pMolGeneSplicerTable* spliceAcceptorTable;
  pMolGeneSplicerTable* exonDonorTable;
  pMolGeneSplicerTable* exonAcceptorTable;
  pMolGeneSplicerTable* intronDonorTable;
  pMolGeneSplicerTable* intronAcceptorTable;

  pMolGeneSplicerTree* spliceDonorTree;
  pMolGeneSplicerTree* spliceAcceptorTree;

  pMolGeneSplicerTable* spliceDonorTables[spliceTablesMax];
  pMolGeneSplicerTable* spliceAcceptorTables[spliceTablesMax];

  void updateSpliceTables();
  pMolStackObject* findSplicing(pMolCmd* cmd, pMolKernelInterface* interface);


  static const int DONOR_LEN = 16;              	// Positions +5,20 in d80 */
  static const int DONOR_SIGNAL_OFFSET = 5;     	// Start of  GT 
  static const int ALPHABET_SIZE = 4;
  static const int ACCEPTOR_LEN = 29;			// Positions +44,72 in a80
  static const int ACCEPTOR_SIGNAL_OFFSET = 24;		// Start of  AG 
  static const int Start_PosEx = 56;
  static const int Stop_PosEx = 84;
  static const int Start_PosIn = 75;
  static const int Stop_PosIn = 90;
  static const int Start_Cod = 0;
  static const int Stop_Cod = 79;
  static const int Start_NoCod = 82;
  static const int Stop_NoCod = 161;

  static const int RETURN_TRUE_PROB = 0;
  static const int CODING_LEN = 80;


  static const int MARKOV_DEGREE = 1;
  static const int MARKOV_LEN = 4;                     	// ALPHABET_SIZE ^ MARKOV_DEGREE */

  double LOG_PROB_ACCEPTOR;
  double LOG_PROB_NONACCEPTOR;
  double LOG_PROB_DONOR;         			// Change this
  double LOG_PROB_NONDONOR;     			// Change this
  double LOW_SCORE;  					// Score if pattern does not have GT or AG signal */

  double ACCEPTOR_HIGH_LIMIT;
  double DONOR_HIGH_LIMIT;
  double ACCEPTOR_THRESHOLD;
  double DONOR_THRESHOLD;

  bool useCodingNonCoding;		///use presence of coding sequences before and after putative splicing to strenghten prediction
  bool cancelNeighbours;			///Delete donor or acceptor within a certain distance of a more likely donor or acceptor. If analyzing genomic sequences set true, but if analyzing the stability of mRNA set false;
  int acceptorCancelNeighbourhood;	///if cancelNeighbours is true, will delete less probable acceptor within this distance
  int donorCancelNeighbourhood;		///if cancelNeighbours is true, will delete less probable donor within this distance

  unsigned char donor_tree;
  unsigned char acceptor_tree;
  unsigned char use_cod_noncod;

  char  Filter  (char );
  void cancelNeighbouringSpliceSites(Site *List, Site *Cap);
  void outputSites(Site *List, Site *Cap);
  int isDonor(const int *b, double *returnScore);
  int isAcceptor(const int *b, double *returnScore);

  int searchTree(const int *S, pMolGeneSplicerTree *tree);
  bool donor(const int * S, double* Return_Score, int ind);
  bool acceptor(const int * S, double* Return_Score, int ind);
  bool IsCodingNonCoding(const int * S, double * Return_Score, pMolGeneSplicerTable* table);
  int comp(const void *a, const void *b);

  int find(char *line, int start);

};

#endif
