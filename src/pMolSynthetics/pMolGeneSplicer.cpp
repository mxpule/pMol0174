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

/*  Derived from:
 *  Copyright (c) 2003, The Institute for Genomic Research (TIGR), Rockville,
 *  Maryland, U.S.A.  All rights reserved.
 *   genesplicer.cp was designed by Mihaela PERTEA to find splice 
 *   sites in a fasta file
 *  and absorbed into pMol code by Martin Pule.
*/


#include "pMolElement.h"
#include "pMolSynthetics/pMolGeneSplicer.h"

//---------------------------------------------------------------------------------------------------
///Constructors and destructors
pMolGeneSplicer::pMolGeneSplicer(pMolXMLbase* p_pMolXMLbaseParent): pMolStackObject(p_pMolXMLbaseParent)
{
  LOG_PROB_ACCEPTOR = log (1682.0 / 30191.0);
  LOG_PROB_NONACCEPTOR = log (28509.0 / 30191.0);
  LOG_PROB_DONOR = log (1682.0 / 30191.0);         	// Change this 
  LOG_PROB_NONDONOR = log (28509.0 / 30191.0);     	// Change this 
  LOW_SCORE = -99.0;  					// Score if pattern does not have GT or AG signal */
  //set all the trees to NULL
  spliceDonorTree = NULL;
  spliceDonorTable = NULL;
  spliceAcceptorTree = NULL;
  spliceAcceptorTable = NULL;

  useCodingNonCoding = false;
  exonDonorTable = NULL;
  exonAcceptorTable = NULL;
  intronDonorTable = NULL;
  intronAcceptorTable = NULL;
  //set the table of tables all to NULL
  for (int i=0; i<spliceTablesMax; i++)
  { spliceDonorTables[i] = NULL;
    spliceAcceptorTables[i] = NULL;
  };
};

pMolGeneSplicer::~pMolGeneSplicer()
{ //delete the trees
  if (spliceDonorTree!=NULL) delete spliceDonorTree;
  if (spliceAcceptorTree!=NULL) delete spliceAcceptorTree;
  //now delete all the donor and acceptor tables
  for (int i=0; i<spliceTablesMax; i++)
  { if (spliceDonorTables[i]!= NULL) delete spliceDonorTables[i];
    if (spliceAcceptorTables[i] != NULL) delete spliceAcceptorTables[i];
  };
  //now delete the intron / exon tables 
  if (exonDonorTable != NULL) delete exonDonorTable;
  if (exonAcceptorTable != NULL) delete exonAcceptorTable;
  if (intronDonorTable != NULL) delete intronDonorTable;
  if (intronAcceptorTable != NULL) delete intronAcceptorTable;
};

//---------------------------------------------------------------------------------------------------
///Enters donor and acceptor tables from XML
void pMolGeneSplicer::updateSpliceTables()
{
  if (spliceDonorTable!=NULL)
  { int n = spliceDonorTable->number;
    if (n < spliceTablesMax) spliceDonorTables[n] = spliceDonorTable;
    spliceDonorTable=NULL;
  };

  if (spliceAcceptorTable!=NULL)
  { int n = spliceAcceptorTable->number;
    if (n < spliceTablesMax) spliceAcceptorTables[n] = spliceAcceptorTable;
    spliceAcceptorTable=NULL;
  };
};

//---------------------------------------------------------------------------------------------------
///direct XML imput into trees and tables
pMolXMLbase* pMolGeneSplicer::XMLopen(const QString &tag, QString &error)
{
  updateSpliceTables();			//see is there a completed table waiting to be entered into table of tables

  if (tag=="pMolGeneSplicerAcceptorTree") 
  { spliceAcceptorTree = new pMolGeneSplicerTree((pMolXMLbase*) this, tag);
    return (pMolXMLbase*)  spliceAcceptorTree;
  };

  if (tag=="pMolGeneSplicerDonorTree")
  { spliceDonorTree = new pMolGeneSplicerTree((pMolXMLbase*) this, tag);
    return (pMolXMLbase*) spliceDonorTree;
  };

  if (tag=="pMolGeneSplicerAcceptorTable")
  { spliceAcceptorTable = 
      new pMolGeneSplicerTable(ACCEPTOR_LEN, ALPHABET_SIZE, MARKOV_LEN, (pMolXMLbase*) this, tag);
    return (pMolXMLbase*) spliceAcceptorTable;
  };

  if (tag=="pMolGeneSplicerDonorTable")
  { spliceDonorTable = 
      new pMolGeneSplicerTable(ACCEPTOR_LEN, ALPHABET_SIZE, MARKOV_LEN, (pMolXMLbase*) this, tag);
    return (pMolXMLbase*) spliceDonorTable;
  };

  if (tag=="pMolExonDonorTable")
  { exonDonorTable = 
      new pMolGeneSplicerTable(CODING_LEN, ALPHABET_SIZE, MARKOV_LEN, (pMolXMLbase*) this, tag);
    return (pMolXMLbase*) exonDonorTable;
  };

  if (tag=="pMolIntronDonorTable")
  { intronDonorTable = 
      new pMolGeneSplicerTable(CODING_LEN, ALPHABET_SIZE, MARKOV_LEN, (pMolXMLbase*) this, tag);
    return (pMolXMLbase*) intronDonorTable;
  };

  if (tag=="pMolExonAcceptorTable")
  { exonAcceptorTable = 
      new pMolGeneSplicerTable(CODING_LEN, ALPHABET_SIZE, MARKOV_LEN, (pMolXMLbase*) this, tag);
    return (pMolXMLbase*) exonAcceptorTable;
  };

  if (tag=="pMolIntronAcceptorTable")
  { intronAcceptorTable = 
      new pMolGeneSplicerTable(CODING_LEN, ALPHABET_SIZE, MARKOV_LEN, (pMolXMLbase*) this, tag);
    return (pMolXMLbase*) intronAcceptorTable;
  };

  return (pMolXMLbase*) this;
};

//---------------------------------------------------------------------------------------------------
///Loads up the XML data
pMolXMLbase* pMolGeneSplicer::XMLclose(const QString &tag, const QString &data, QString &error)
{
  updateSpliceTables();
  if (tag=="pMolGeneSplicer")   			return pMolXMLbaseParent; 

  else if (tag=="acceptorHighLimit")               	XMLdouble(&ACCEPTOR_HIGH_LIMIT, data, error);
  else if (tag=="donorHighLimit")               	XMLdouble(&DONOR_HIGH_LIMIT, data, error);
  else if (tag=="acceptorThreshold")               	XMLdouble(&ACCEPTOR_THRESHOLD, data, error);
  else if (tag=="donorThreshold")               	XMLdouble(&DONOR_THRESHOLD, data, error);
  else if (tag=="cancelNeighbours")               	XMLboolean(&cancelNeighbours, data, error);
  else if (tag=="useCodingNonCoding")			XMLboolean(&useCodingNonCoding, data, error);
  else if (tag=="acceptorCancelNeighbourhood")   	XMLnumeric(&acceptorCancelNeighbourhood, data, error);
  else if (tag=="donorCancelNeighbourhood")   		XMLnumeric(&donorCancelNeighbourhood, data, error);

  else return pMolXMLbase::XMLclose(tag, data,error);
  return (pMolXMLbase*) this; 
};

//--------------------------------------------------------------------------------------------------
///saves the data in XML (partially complete)
bool pMolGeneSplicer::saveContents(QTextStream* stream)
{
  XMLsave(stream, "acceptorHighLimit", ACCEPTOR_HIGH_LIMIT);
  XMLsave(stream, "donorHighLimit", DONOR_HIGH_LIMIT);
  XMLsave(stream, "acceptorThreshold", ACCEPTOR_THRESHOLD);
  XMLsave(stream, "donorThreshold", DONOR_THRESHOLD);
  XMLsave(stream, "cancelNeighbours", cancelNeighbours);
  XMLsave(stream, "acceptorCancelNeighbourhood", acceptorCancelNeighbourhood);
  XMLsave(stream, "donorCancelNeighbourhood", donorCancelNeighbourhood);
};

//--------------------------------------------------------------------------------------------------
///output XML data as a tree structure
void pMolGeneSplicer::treeOut(QTreeWidgetItem *tree)
{
  pMolXMLbase::treeOut(tree);
  treeItem(tree,"acceptorHighLimit",ACCEPTOR_HIGH_LIMIT);
  treeItem(tree,"donorHighLimit",DONOR_HIGH_LIMIT);
  treeItem(tree,"acceptorThreshold",ACCEPTOR_THRESHOLD);
  treeItem(tree,"donorThreshold",DONOR_THRESHOLD);

  if (cancelNeighbours)
  { QTreeWidgetItem* branch = treeItem(tree, "cancelNeighbours", "true");
    treeItem(branch,"acceptorCancelNeighbourhood",acceptorCancelNeighbourhood);
    treeItem(branch,"donorCancelNeighbourhood",donorCancelNeighbourhood);
  }
  else treeItem(tree, "cancelNeighbours", "false");

  if (spliceDonorTree != NULL) spliceDonorTree->treeOut(tree);
  if (spliceAcceptorTree != NULL) spliceAcceptorTree->treeOut(tree);

  if (useCodingNonCoding)
  {
    QTreeWidgetItem* branch = treeItem(tree, "useCodingNonCoding", useCodingNonCoding);
    if (exonDonorTable!=NULL) exonDonorTable->treeOut(branch);
    if (exonAcceptorTable!=NULL) exonAcceptorTable->treeOut(branch);
    if (intronDonorTable!=NULL) intronDonorTable->treeOut(branch);
    if (intronAcceptorTable!=NULL) intronAcceptorTable->treeOut(branch);
  };

  int i;
  for (i=0; i<spliceTablesMax; i++) if (spliceDonorTables[i]!=NULL) spliceDonorTables[i]->treeOut(tree);
  for (i=0; i<spliceTablesMax; i++) if (spliceAcceptorTables[i]!=NULL) spliceAcceptorTables[i]->treeOut(tree);
};

//------------------------------------------------------------------------------------------------------------------
///returns infromation type. This class is executable, represents an engine
bool pMolGeneSplicer::type(int i)
{ 
  if (i == pMolStackObject::EXEC) return true;
  if (i == pMolStackObject::ENGINE)  return true;
  return pMolStackObject::type(i);
};

//------------------------------------------------------------------------------------------------------------------
///Nascent error handling
bool pMolGeneSplicer::suberror(const QString &function, const QString &message)
{
  error+=function+":"+message+"\n";
  return false;
};

//------------------------------------------------------------------------------------------------------------------
///Business bit, directs all splicing searches
bool pMolGeneSplicer::findSplicing(const QString &sequence)
{

  long int  Data_Len;
  Site *List,*nod, *site, *Cap;
  int B[200],k;
  double score;
  long offset,i,j;
  char filename[PATH_MAX+1];
  float readdouble;


  offset=1;
  List=NULL;
  nod=List;
  Cap=List;

  Data_Len = sequence.length();
  char* Data = sequence.toLocal8Bit().data();
  //char* Data = new char[Data_Len];

  //for  (i = 0;  i < Data_Len;  i ++)
  //  Data[i] = Filter(sequence.at(i).toAscii());


  for  (i = 80;  i <= Data_Len-82;  i ++) 
  {
      // look fot gt and ag
      // forward direction
      if(Data[i]=='a' && Data[i+1]=='g') { // Deal with acceptors

	  k=0;
	  for(j=i-80;j<i+82;j++){
	    switch (Data[j]){
	    case 'a': B[k]=0;break;
	    case 'c': B[k]=1;break;
	    case 'g': B[k]=2;break;
	    case 't': B[k]=3;break;
	    default: B[k]=1;
	    }
	    k++;
	  }

	  if(isAcceptor(B,&score)) {

	    //	    printf("ag: %ld\n",i+offset);

	    // add acceptor to list;
	    site=(Site *) malloc(sizeof(Site));
	    if (site == NULL) {
	      fprintf(stderr,"Memory allocation for Site position failure.\n"); 
	      abort();
	    }
	    
	    site->type=1;
	    site->score=score;
	    site->pos=i+offset;
	    site->dir=1;
	    site->forw=NULL;
	    site->prev=nod;

	    if(List == NULL) List=site;
	    Cap=site;
	    if(nod != NULL ) nod->forw =site;
	    
	    nod=site;

	  }
      }


      if(Data[i]=='g' && Data[i+1]=='t') { // Deal with donors

	  k=0;
	  for(j=i-80;j<i+82;j++){
	    switch (Data[j]){
	    case 'a': B[k]=0;break;
	    case 'c': B[k]=1;break;
	    case 'g': B[k]=2;break;
	    case 't': B[k]=3;break;
	    default: B[k]=1;
	    }
	    k++;
	  }

	  if(isDonor(B,&score)) {

	    //printf("gt: %ld\n",i+offset+1);fflush(stdout);

	    // add donor to list;
	    site=(Site *) malloc(sizeof(Site));
	    if (site == NULL) {
	      fprintf(stderr,"Memory allocation for Site position failure.\n"); 
	      abort();
	    }

	    site->type=2;
	    site->score=score;
	    site->pos=i+offset;
	    site->dir=1;
	    site->forw=NULL;
	    site->prev=nod;

	    if(List == NULL) List=site;

	    Cap=site;

	    if(nod != NULL) nod->forw=site;
	    
	    nod=site;

	  }
      }

    }

    // print splice sites found so far

   if (cancelNeighbours) cancelNeighbouringSpliceSites(List,Cap);

    outputSites(List,Cap);
    offset+=Data_Len-161;

    fprintf(stderr,"Done %ldbp..............\n",offset+160);

  delete []Data;
}

//------------------------------------------------------------------------------
///As far as I can gather, this bit sweeps backwards and forwards and deletes any 
///donor or acceptor within a certain distance of a more likely donor or acceptor 
///this would be important for real world processing but if we want to figure out 
///the likely stability of some mRNA, having all the features may be better.
void pMolGeneSplicer::cancelNeighbouringSpliceSites(Site *List, Site *Cap)
{
  Site *nod, *scan;

  nod=List;
  while(nod!=NULL) {
    scan=nod->prev;

    if(nod->type==1) {			//donor
      while(scan && nod->pos-scan->pos<=acceptorCancelNeighbourhood) {		//DISTA
	if(scan->type==1 && scan->score<nod->score) {
	  scan->type=0;			//cancelled
	}
	scan=scan->prev;
      }
    }
      
    if(nod->type==2) {			//acceptor
      while(scan && nod->pos-scan->pos<=donorCancelNeighbourhood) {		//DISTD
	if(scan->type==2 && scan->score<nod->score) {
	  scan->type=0;			//cancelled
	}
	scan=scan->prev;
      }
    }

    nod=nod->forw;
  }

  nod=Cap;

  while(nod!=NULL) {
    scan=nod->forw;
      
    if(nod->type==1) {			//donor
      while(scan && scan->pos-nod->pos<=acceptorCancelNeighbourhood) {		//DISTA
	if(scan->type==1 && scan->score<nod->score) {
	  scan->type=0;			//cancelled
	}
	scan=scan->forw;
      }
    }  

    if(nod->type==2) {			//acceptor
      while(scan && scan->pos-nod->pos<=donorCancelNeighbourhood) {		//DISTD
	if(scan->type==2 && scan->score<nod->score) {
	  scan->type=0;			//cancelled
	}
	scan=scan->forw;
      }
    }
    nod=nod->prev;
  }
}

//------------------------------------------------------------------------------
///Output sites found
void pMolGeneSplicer::outputSites(Site *List, Site *Cap) 
{
  char confidence[15];
  Site *nod, *scan;
  nod=List;

  while(nod!=NULL) {
    if(nod->type == 1) {
      if(nod->score>=ACCEPTOR_HIGH_LIMIT) strcpy(confidence,"High");
      else strcpy(confidence,"Medium");
      if(nod->dir == 1) fprintf(stderr,"%ld %ld %.6f %s acceptor\n",nod->pos,nod->pos+1,nod->score,confidence);
      if(nod->dir == -1) fprintf(stderr,"%ld %ld %.6f %s acceptor\n",nod->pos+1,nod->pos,nod->score,confidence);
    }
    if(nod->type == 2) {
      if(nod->score>=DONOR_HIGH_LIMIT) strcpy(confidence,"High");
      else strcpy(confidence,"Medium");
      if(nod->dir == 1) fprintf(stderr,"%ld %ld %.6f %s donor\n",nod->pos,nod->pos+1,nod->score,confidence);
      if(nod->dir == -1) fprintf(stderr,"%ld %ld %.6f %s donor\n",nod->pos+1,nod->pos,nod->score,confidence);
    }
    List=nod;
    nod=nod->forw;
    free(List);
  }
}

//------------------------------------------------------------------------------
///Return a single  a, c, g or t  for  Ch .  Choice is to minimize likelihood
///of a stop codon on the primary strand.
char pMolGeneSplicer::Filter  (char Ch)
{
  switch  (tolower (Ch))
    {
    case  'a' :
    case  'c' :
    case  'g' :
    case  't' :
      return  tolower(Ch);
    case  'r' :     // a or g
      return  'g';
    case  'y' :     // c or t
      return  'c';
    case  's' :     // c or g
      return  'c';
    case  'w' :     // a or t
      return  't';
    case  'm' :     // a or c
      return  'c';
    case  'k' :     // g or t
      return  't';
    case  'b' :     // c, g or t
      return  'c';
    case  'd' :     // a, g or t
      return  'g';
    case  'h' :     // a, c or t
      return  'c';
    case  'v' :     // a, c or g
      return  'c';
    default :       // anything
      return  'c';
    }
}

//------------------------------------------------------------------------------------
///my version of is_Acceptor
int pMolGeneSplicer::isAcceptor(const int *b, double *returnScore)
{
  double Score,S1,S2;

  int i,ind;
  int T[100];
  double score1,score2,score3;

  for(i=0;i<=Stop_PosEx-Start_PosEx;i++)
    T[i]=b[i+Start_PosEx];

  ind=acceptor(T, &S1,0);
  if(ind==0) return(0);
  if(acceptor_tree) {
    acceptor(T, &S2,1);
    score1=(S1+S2)/2;
  }
  else { score1=S1; }

  if (useCodingNonCoding)
  {
    for(i=0;i<=Stop_NoCod-Start_NoCod;i++)
      T[i]=b[i+Start_NoCod];
    IsCodingNonCoding(T,&score2,exonAcceptorTable);	//   case 0: // case of exon in acceptor

    for(i=0;i<=Stop_Cod-Start_Cod;i++)
      T[i]=b[i+Start_Cod];
    IsCodingNonCoding(T,&score3,intronAcceptorTable);	//   case 1: // case of intron in acceptor

    Score=score1+score2+score3;
  }
  else Score = score1;

  *returnScore=Score;
  return Score >= ACCEPTOR_THRESHOLD;
};


//------------------------------------------------------------------------------------
///my version of is_donor
int pMolGeneSplicer::isDonor(const int *b, double *returnScore)
{
  double Score,S1,S2;

  int ind,i;
  int T[100];
  double score1,score2,score3;

  for(i=0;i<=Stop_PosIn-Start_PosIn;i++)
    T[i]=b[i+Start_PosIn];

  ind=donor(T, &S1,0);
  if(ind==0) return(0);

  if(donor_tree) {
    donor(T, &S2, 1);
    score1=(S1+S2)/2;
  }
  else {
    score1=S1;
  }

  if (useCodingNonCoding)				//If we use weighting by coding / non coding sequences
  {							//lets go looking
    for(i=0;i<=Stop_Cod-Start_Cod;i++)			//TIGR comment: if(score1<=THR_DON) score1=-99;
      T[i]=b[i+Start_Cod];
    IsCodingNonCoding(T,&score2,exonDonorTable);	//TIGR comment: case 2: // case of exon in donor

    for(i=0;i<=Stop_NoCod-Start_NoCod;i++)		//TIGR comment: if(score2<=THR_DON_EX) score2=-99;  
      T[i]=b[i+Start_NoCod];				//TIGR comment: case 3: // case of intron in donor
    IsCodingNonCoding(T,&score3,intronDonorTable);	//TIGR comment:	if(score3<=THR_DON_IN) score3=-99;

    Score=score1+score2+score3;
  }
  else Score = score1;

  *returnScore=Score;
  return Score >= DONOR_THRESHOLD;
};



//------------------------------------------------------------------------------------
///compares
int pMolGeneSplicer::comp(const void *a, const void *b)
{ 
  if(*(double *)a > *(double *)b) return(1);
  else if (*(double *)a==*(double *)b) return(0);
  else return(0);

}

//------------------------------------------------------------------------------------
///my version of TIGR's find file function
int pMolGeneSplicer::searchTree(const int *S, pMolGeneSplicerTree *tree)
{
  int val, cons, poz;

  if (tree==NULL)
  {suberror("searchTree","tried to access a null tree branch"); return -1;}

  val=tree->val;
  cons=tree->consens;

  if (cons!=-1)
  { 
    poz=tree->poz;
    if(S[poz]==cons)
      val=searchTree(S,tree->left);
    else
      val=searchTree(S,tree->right);
  }
  return(val);
}

//------------------------------------------------------------------------------------
///my version of TIGR's function "don"
bool pMolGeneSplicer::donor(const int * S, double* Return_Score, int ind)
{
  int no;

  if (ind) no=searchTree(S,spliceDonorTree); else no=1;

  if ((S[DONOR_SIGNAL_OFFSET]!=2) || (S[DONOR_SIGNAL_OFFSET+1]!=3))  //double check if GT
  { *Return_Score = LOW_SCORE;
     return false;
  }

   int Sub = 0;
   bool okp, okn;
   for  (int i = 0;  i<MARKOV_DEGREE;  i++)
     Sub = ALPHABET_SIZE * Sub + S[i];

   double Positive_Sum = spliceDonorTables[no]->getPositiveTableValue(MARKOV_DEGREE - 1,0,Sub,&okp);
   double Negative_Sum = spliceDonorTables[no]->getNegativeTableValue(MARKOV_DEGREE - 1,0,Sub,&okn);
   if ((!okp) || (!okn)) return suberror("donor","problem trying to access splicerDonorTables");

   for  (int i = MARKOV_DEGREE;  i < DONOR_LEN;  i ++)
   {
     int j = S[i];
     Positive_Sum += spliceDonorTables[no]->getPositiveTableValue(i,j,Sub,&okp);
     Negative_Sum += spliceDonorTables[no]->getNegativeTableValue(i,j,Sub,&okn);
     if ((!okp) || (!okn)) return suberror("donor","problem trying to access splicerDonorTables");
     Sub = ALPHABET_SIZE * (Sub % (MARKOV_LEN / ALPHABET_SIZE)) + j;
   }

   double Score = Positive_Sum - Negative_Sum;

#if  RETURN_TRUE_PROB
   X = exp (Positive_Sum + LOG_PROB_DONOR);
   Y = exp (Negative_Sum + LOG_PROB_NONDONOR);
   *Return_Score = log (X / (X + Y));
#else
   *Return_Score = Score;
#endif

   return true;
};

//------------------------------------------------------------------------------------
///my version of TIGR's function "acc"
bool pMolGeneSplicer::acceptor(const int * S, double* Return_Score, int ind)
{
  int no;
  if (ind) no=searchTree(S,spliceAcceptorTree); else no=1;
  if ((S[ACCEPTOR_SIGNAL_OFFSET]!=0) || (S[ACCEPTOR_SIGNAL_OFFSET+1]!=2))  //double check if AG
  { *Return_Score = LOW_SCORE;
     return false;
  }

   int Sub = 0;
   bool okp, okn;
   for  (int i = 0;  i<MARKOV_DEGREE;  i++)
     Sub = ALPHABET_SIZE * Sub + S[i];

   double Positive_Sum = spliceAcceptorTables[no]->getPositiveTableValue(MARKOV_DEGREE - 1,0,Sub,&okp);
   double Negative_Sum = spliceAcceptorTables[no]->getNegativeTableValue(MARKOV_DEGREE - 1,0,Sub,&okn);
   if ((!okp) || (!okn)) return suberror("donor","problem trying to access splicerDonorTables");

   for  (int i = MARKOV_DEGREE;  i < ACCEPTOR_LEN;  i ++)
   {
     int j = S[i];
     Positive_Sum += spliceAcceptorTables[no]->getPositiveTableValue(i,j,Sub,&okp);
     Negative_Sum += spliceAcceptorTables[no]->getNegativeTableValue(i,j,Sub,&okn);
     if ((!okp) || (!okn)) return suberror("donor","problem trying to access splicerDonorTables");
     Sub = ALPHABET_SIZE * (Sub % (MARKOV_LEN / ALPHABET_SIZE)) + j;
   }

   double Score = Positive_Sum - Negative_Sum;

#if  RETURN_TRUE_PROB
   X = exp (Positive_Sum + LOG_PROB_DONOR);
   Y = exp (Negative_Sum + LOG_PROB_NONDONOR);
   *Return_Score = log (X / (X + Y));
#else
   *Return_Score = Score;
#endif

   return true;
};



//------------------------------------------------------------------------------------
///My version of TIGR's isCod_nonCod
///Evaluate string  S [0 .. (CODING_LEN -1)] and
///return  TRUE  or  FALSE  as to whether it is a likely donor
///site.  Also set  Return_Score  to the probability that it is an donor site. 
bool  pMolGeneSplicer::IsCodingNonCoding(const int * S, double * Return_Score, pMolGeneSplicerTable* table)
{
  double  Positive_Sum, Negative_Sum, Score, Threshold;

#if  RETURN_TRUE_PROB
  double  X, Y;
#endif
  int  i, j, k, Ct, Sub;
  bool okp, okn;

  Sub = 0;
  for  (i = 0;  i < MARKOV_DEGREE;  i ++)
    Sub = ALPHABET_SIZE * Sub + S [i];

  Positive_Sum = table->getPositiveTableValue(MARKOV_DEGREE - 1,0,Sub,&okp);
  Negative_Sum = table->getNegativeTableValue(MARKOV_DEGREE - 1,0,Sub,&okn);
  if ((!okp) || (!okn)) return suberror("IsCodingNonCoding","problem trying to access tables");

  for  (i = MARKOV_DEGREE;  i < CODING_LEN;  i ++)
  {
    j = S [i];
    Positive_Sum += table->getPositiveTableValue(i,j,Sub,&okp);
    Negative_Sum += table->getNegativeTableValue(i,j,Sub,&okn);
    if ((!okp) || (!okn)) return suberror("IsCodingNonCoding","problem trying to access tables");
    Sub = ALPHABET_SIZE * (Sub % (MARKOV_LEN / ALPHABET_SIZE)) + j;
  }

  Score = Positive_Sum - Negative_Sum;

#if  RETURN_TRUE_PROB
  X = exp (Positive_Sum + LOG_PROB_DONOR);
  Y = exp (Negative_Sum + LOG_PROB_NONDONOR);
  *Return_Score = log (X / (X + Y));
#else
  *Return_Score = Score;
#endif

  return true;
};

//--------------------------------------------------------------------------------------------------
///we need to make this very sophisticated - able to take multiple inputs and outputs
///aswell as output lists of possibilities
pMolStackObject* pMolGeneSplicer::findSplicing(pMolCmd* cmd, pMolKernelInterface* interface)
{

  QString error;

  //if we didn't get any parameters, then send an error and return
  pMolStackList* list = pMolStackList::interpretParameter(cmd, interface,error);
  if (list==NULL) return makeError(interface, "find", error);

  //if there's one number parameter, cut bit off front or back depending if pos or neg
  if (list->checkParameters(pMolObject::DNA, error))
  {
    pMolElement* dna = dynamic_cast <pMolElement*> (list->at(0));
    findSplicing(dna->getSequence());
    qDebug() << "got to here";
    return NULL;
  };

  return makeError(interface, "add", error);
};

//-----------------------------------------------------------------------------------------------
///executes commands referred to this object
pMolStackObject* pMolGeneSplicer::exec(pMolCmd* cmd, pMolKernelInterface* interface)
{
  //no paramaters, return this as object
  if (cmd==NULL) return (pMolStackObject*) this;				//no paramaters, return me

  //if no command but not empty, something is wrong (needs to be empty or have subcommand) so return with error
  if (cmd->type() != pMolCmd::NAME)
  { interface->msg(name+"::exec - empty field or subcommand expected");
    return NULL;
  };
  //first let's look in our own little internal namespace
  pMolCmdName* cmdName = dynamic_cast<pMolCmdName*>(cmd);		//safe to assume this is a pMolCmdName

  if (cmdName->name=="findSplicing") 	return findSplicing(cmdName->child, interface);	else
  interface->msg(name+"::command not understood");
  return NULL;
};



