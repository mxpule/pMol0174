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
 *  Copyright (c) 2003, The Institute for Genomic Research (TIGR), Rockville,
 *  Maryland, U.S.A.  All rights reserved.

 *   genesplicer.cpp was designed by Mihaela PERTEA to find splice 
 *   sites in a fasta file

*/



//enum SpliceType { ACC, DON, FALSE};
//enum Direction {F,R};

#include "pMolSynthetics/geneSplicer.h"

pMolGeneSplicerB::pMolGeneSplicerB(const QString &p_fastaName, const QString &p_trainingDirectory)
{
  UseFile=0;

  fastaName = p_fastaName;
  trainingDirectory = p_trainingDirectory;
};


bool pMolGeneSplicerB::findSplicing()
{
  FILE  * fp, *ofp;
  char  Data[1000001];
  long int  Data_Len;
  Site *List,*nod, *site, *Cap;
  int B[200],k;
  double score;
  long offset,i,j;
  char filename[PATH_MAX+1];
  float readdouble;



  //strcpy(TRAIN_DIR,trainingDir..toAscii().data());
  QString configFilename = trainingDirectory+"/config_file";
  // read config file
  //sprintf(filename,"%s/config_file",TRAIN_DIR);
  fp = fopen (configFilename.toAscii().data(), "r");
  if  (fp == NULL) {
    fprintf (stderr, "ERROR:  Could not open file  %s \n", configFilename.toAscii().data());
    exit (0);
  }

  fscanf(fp,"%f",&readdouble);ACCEPTOR_HIGH_LIMIT=(double)readdouble;
  fscanf(fp,"%f",&readdouble);DONOR_HIGH_LIMIT=(double)readdouble;
  fscanf(fp,"%f",&readdouble);ACCEPTOR_THRESHOLD=(double)readdouble;
  fscanf(fp,"%f",&readdouble);DONOR_THRESHOLD=(double)readdouble;
  fscanf(fp,"%d",&k); donor_tree=(unsigned char)k;
  fscanf(fp,"%d",&k); acceptor_tree=(unsigned char)k;
  fscanf(fp,"%d",&DISTA);
  fscanf(fp,"%d",&DISTD);
  //  fscanf(fp,"%u",&use_cod_noncod);
  fclose(fp);

/*
  Process_Options (argc, argv);   // Set global variables to reflect status of
                                  // command-line options.
*/


  fp = File_Open (fastaName.toAscii().data(), "r");

  if(UseFile) {
    ofp= File_Open (FILE_NAME, "w");
  }

  offset=1;


  while(!feof(fp)) {

    List=NULL;
    nod=List;
    Cap=List;

    LoadData(fp,Data);

    Data_Len = strlen (Data);    

    for  (i = 0;  i < Data_Len;  i ++) {

      // Converts all characters to  acgt
      Data [i] = Filter (Data [i]);
    }

    for  (i = 80;  i <= Data_Len-82;  i ++) {

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

	  if(Is_Acceptor(B,&score)) {

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

	  if(Is_Donor(B,&score)) {

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

      // reversed direction
      if(Data[i]=='c' && Data[i+1]=='t') { // Deal with acceptors

	  k=0;
	  for(j=i+81;j>=i-80;j--){
	    switch (Data[j]){
	    case 'a': B[k]=3;break;
	    case 'c': B[k]=2;break;
	    case 'g': B[k]=1;break;
	    case 't': B[k]=0;break;
	    default: B[k]=1;
	    }
	    k++;
	  }

	  if(Is_Acceptor(B,&score)) {

	    //	    printf("ag: %ld\n",i+offset+1);

	    // add acceptor to list;
	    site=(Site *) malloc(sizeof(Site));
	    if (site == NULL) {
	      fprintf(stderr,"Memory allocation for Site position failure.\n"); 
	      abort();
	    }
	    
	    site->type=1;
	    site->score=score;
	    site->pos=i+offset;
	    site->dir=-1;
	    site->forw=NULL;
	    site->prev=nod;

	    if(List == NULL) List=site;
	    Cap=site;
	    if(nod != NULL) nod->forw=site;
	    
	    nod=site;

	  }
      }

      if(Data[i]=='a' && Data[i+1]=='c') { // Deal with donors

	  k=0;
	  for(j=i+81;j>=i-80;j--){
	    switch (Data[j]){
	    case 'a': B[k]=3;break;
	    case 'c': B[k]=2;break;
	    case 'g': B[k]=1;break;
	    case 't': B[k]=0;break;
	    default: B[k]=1;
	    }
	    k++;
	  }

	  if(Is_Donor(B,&score)) {

	    //	    printf("gt: %ld\n",i+offset+1);

	    // add donor to list;
	    site=(Site *) malloc(sizeof(Site));
	    if (site == NULL) {
	      fprintf(stderr,"Memory allocation for Site position failure.\n"); 
	      abort();
	    }
	    
	    site->type=2;
	    site->score=score;
	    site->pos=i+offset;
	    site->dir=-1;
	    site->forw=NULL;
	    site->prev=nod;

	    if(List == NULL) List=site;
	    Cap=site;
	    if(nod !=NULL) nod->forw=site;
	    
	    nod=site;
	  }

      }

    }

    // print splice sites found so far

    if(UseFile) printsites(List,Cap,ofp);    
    else printsites(List,Cap,stderr);
    offset+=Data_Len-161;

    fprintf(stderr,"Done %ldbp..............\n",offset+160);
  }

  fclose(fp);
  if(UseFile) fclose(ofp);
}

void pMolGeneSplicerB::LoadData(FILE *fp,char *Data) 
{
  char ch;
  long length,lcopy,back;
  char line[1001];
  
  // find the start
  length=161;
  while(length) {
    fseek(fp,-1,SEEK_CUR); 
    ch=fgetc(fp);
    fseek(fp,-1,SEEK_CUR); 
    if(ch != '\n') length--;
  }
   
  // copy the data
  length=0;
  Data[0]='\0';


  while(length<1000000) {
    if(!fgets(line,1000,fp)) break;
    
    lcopy=strlen(line);
    if(line[lcopy-1]=='\n') line[--lcopy]='\0';
    if(length+lcopy>1000000) {
      back=lcopy;
      lcopy=1000000-length;
      back-=lcopy;
      while(back) {
	fseek(fp,-1,SEEK_CUR); 
	ch=fgetc(fp);
	fseek(fp,-1,SEEK_CUR); 
	if(ch != '\n') back--;
      }
    }
    length+=lcopy;
    strncat(Data,line,lcopy);
    Data[length]='\0';

  }

}


void pMolGeneSplicerB::printsites(Site *List, Site *Cap,FILE *ofp) 
{
  char confidence[15];
  Site *nod, *scan;

  nod=List;

  while(nod!=NULL) {
    scan=nod->prev;

    if(nod->type==1) {
      while(scan && nod->pos-scan->pos<=DISTA) {
	if(scan->type==1 && scan->score<nod->score) {
	  scan->type=0;
	}
	scan=scan->prev;
      }
    }
      
    if(nod->type==2) {
      while(scan && nod->pos-scan->pos<=DISTD) {
	if(scan->type==2 && scan->score<nod->score) {
	  scan->type=0;
	}
	scan=scan->prev;
      }
    }

    nod=nod->forw;
  }

  nod=Cap;

  while(nod!=NULL) {
    scan=nod->forw;
      
    if(nod->type==1) {
      while(scan && scan->pos-nod->pos<=DISTA) {
	if(scan->type==1 && scan->score<nod->score) {
	  scan->type=0;
	}
	scan=scan->forw;
      }
    }  

    if(nod->type==2) {
      while(scan && scan->pos-nod->pos<=DISTD) {
	if(scan->type==2 && scan->score<nod->score) {
	  scan->type=0;
	}
	scan=scan->forw;
      }
    }
      
    nod=nod->prev;
  }
  

  nod=List;

  while(nod!=NULL) {
    if(nod->type == 1) {
      if(nod->score>=ACCEPTOR_HIGH_LIMIT) strcpy(confidence,"High");
      else strcpy(confidence,"Medium");
      if(nod->dir == 1) fprintf(ofp,"%ld %ld %.6f %s acceptor\n",nod->pos,nod->pos+1,nod->score,confidence);
      if(nod->dir == -1) fprintf(ofp,"%ld %ld %.6f %s acceptor\n",nod->pos+1,nod->pos,nod->score,confidence);
    }
    if(nod->type == 2) {
      if(nod->score>=DONOR_HIGH_LIMIT) strcpy(confidence,"High");
      else strcpy(confidence,"Medium");
      if(nod->dir == 1) fprintf(ofp,"%ld %ld %.6f %s donor\n",nod->pos,nod->pos+1,nod->score,confidence);
      if(nod->dir == -1) fprintf(ofp,"%ld %ld %.6f %s donor\n",nod->pos+1,nod->pos,nod->score,confidence);
    }
    List=nod;
    nod=nod->forw;
    free(List);
  }
}

char pMolGeneSplicerB::Filter  (char Ch)

//  Return a single  a, c, g or t  for  Ch .  Choice is to minimize likelihood
//  of a stop codon on the primary strand.

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

FILE* pMolGeneSplicerB::File_Open  (const char * Filename, const char * Mode)

/* Open  Filename  in  Mode  and return a pointer to its control
*  block.  If fail, print a message and exit. */

{
  FILE  *  fp;
  char line[1000];
  int length;
  
  fp = fopen (Filename, Mode);
  if  (fp == NULL)
    {
      fprintf (stderr, "ERROR:  Could not open file  %s \n", Filename);
      exit (0);
    }

  if(Mode[0] == 'r') {
    fgets(line,1000,fp);
  
    if(line[0] != '>') {
      fprintf (stderr,"ERROR: File %s is not a fasta file\n",Filename);
      exit (0);
    }

    length=0;

    while(length<161) {
      if(fgetc(fp) != '\n') length++;
    }
    
  }
  
  return  fp;
}


//---------------------------------------------


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
/*  *  Copyright (c) 2003, The Institute for Genomic Research (TIGR), Rockville,
 *  Maryland, U.S.A.  All rights reserved.

 *  sitesk.c compute a score for splice sites based on karlin's paper.
 */


int pMolGeneSplicerB::Is_Acceptor(const int *B, double *Return_Score)
{
  FILE  * Infile;
  double Score,S1,S2;
  static tree *tacc;
  static int readtacc=FALSE;
  char line[5000];
  int i,ind;
  int T[100];
  double score1,score2,score3;
  //char treefile[PATH_MAX+1];
  QString treefile;

  if(!readtacc && acceptor_tree)
    {
   
      /* read the structure of the acceptor tree */

      //sprintf(treefile,"%s/%s",TRAIN_DIR,ACCEPTOR_TREE_FILE);
      treefile = trainingDirectory + "/" + ACCEPTOR_TREE_FILE;
      Infile = fopen (treefile.toAscii().data(), "r");
      if  (Infile == NULL)
	{
	  qDebug() << "ERROR:  Unable to open file " << treefile;
	  exit (EXIT_FAILURE);
	}

      tacc = (tree *) malloc(sizeof(tree));
      if (tacc == NULL) {fprintf(stderr," Memory allocation for tree failure.\n"); abort();}
      fgets(line, 5000, Infile);
      i=strlen(line);
      line[i-1]='\0';
      fclose(Infile);

      readtree(line, tacc, 0);
      readtacc=TRUE;
    }
   
  for(i=0;i<=Stop_PosEx-Start_PosEx;i++)
    T[i]=B[i+Start_PosEx];

  ind=Acc(T, &S1, tacc,0);
  if(ind==0) return(0);
  if(acceptor_tree) {
    Acc(T, &S2, tacc,1);
    score1=(S1+S2)/2;
  }
  else { score1=S1; }

  for(i=0;i<=Stop_NoCod-Start_NoCod;i++)
    T[i]=B[i+Start_NoCod];

  Is_Cod_NonCod(T,&score2,0);

  for(i=0;i<=Stop_Cod-Start_Cod;i++)
    T[i]=B[i+Start_Cod];

  Is_Cod_NonCod(T,&score3,1);


  Score=score1+score2+score3;

  *Return_Score=Score;
  
  
  return Score >= ACCEPTOR_THRESHOLD;
	  
      
}  

int pMolGeneSplicerB::Is_Donor(const int *B, double *Return_Score)
{
  FILE  * Infile;
  double Score,S1,S2;
  static tree *tdon;
  static int readtdon=FALSE;
  char line[5000];
  int ind,i;
  int T[100];
  double score1,score2,score3;
  QString treefile;

  if(!readtdon && donor_tree)
    {
   
      /* read the structure of the donor tree */
      
      //sprintf(treefile,"%s/%s",TRAIN_DIR,DONOR_TREE_FILE);
      treefile = trainingDirectory + "/" + DONOR_TREE_FILE;
      Infile = fopen (treefile.toAscii().data(), "r");
      if  (Infile == NULL)
	{
	  qDebug() << "ERROR:  Unable to open file " << treefile;
	  exit (EXIT_FAILURE);
	}

      tdon = (tree *) malloc(sizeof(tree));
      if (tdon == NULL) {fprintf(stderr,"Memory allocation for tree failure.\n"); abort();}
      fgets(line, 5000, Infile);
      i=strlen(line);
      line[i-1]='\0';
      fclose(Infile);

      readtree(line, tdon, 0);
      readtdon=TRUE;
    }

  for(i=0;i<=Stop_PosIn-Start_PosIn;i++)
    T[i]=B[i+Start_PosIn];

  ind=Don(T, &S1, tdon,0);
  if(ind==0) return(0);
  if(donor_tree) {
    Don(T, &S2, tdon,1);
    score1=(S1+S2)/2;
  }
  else {
    score1=S1;
  }

  //  if(score1<=THR_DON) score1=-99;

  for(i=0;i<=Stop_Cod-Start_Cod;i++)
    T[i]=B[i+Start_Cod];

  Is_Cod_NonCod(T,&score2,2);

  //  if(score2<=THR_DON_EX) score2=-99;
  
  for(i=0;i<=Stop_NoCod-Start_NoCod;i++)
    T[i]=B[i+Start_NoCod];

  Is_Cod_NonCod(T,&score3,3);

  //  if(score3<=THR_DON_IN) score3=-99;

  Score=score1+score2+score3;

  *Return_Score=Score;
  
  return Score >= DONOR_THRESHOLD;
	  
      
}  


    
void pMolGeneSplicerB::readtree(char *line, tree *t, int start)
{
 int len;
 int i,n;
 char part[10];
 len=strlen(line);

 i=start;
 while((line[i]=='(')||(line[i]==' ')) i++;
 n=i;
 while(line[i]!=' ')
 {
	part[i-n]=line[i];
	i++;
 }
 part[i-n]='\0';
 t->val=atoi(part);

 i++;
 n=i;
 while(line[i]!=' ')
 { 
	part[i-n]=line[i];
	i++;
 }
 part[i-n]='\0';
 t->consens=atoi(part);

 i++;
 n=i;
 while(line[i]!=' ')
 { 
	part[i-n]=line[i];
	i++;
 }
 part[i-n]='\0';
 t->poz=atoi(part);

 i++;
 n=i;
 while(line[i]!=' ')
 { 
	part[i-n]=line[i];
	i++;
 }
 part[i-n]='\0';
 t->no=atoi(part);

 t->left=NULL;
 t->right=NULL;

 i+=2;n=i;
 if(line[i]=='(') 
 	{
 		i=find(line,i+1);
		t->left = (tree *) malloc(sizeof(tree));
   		if (t->left == NULL) {fprintf(stderr,"Memory allocation for tree failure.\n"); abort();}
        readtree(line,t->left,n);
     }
	
 i+=2;n=i;
 if(line[i]=='(') 
 	{
 		i=find(line,i+1);
		t->right = (tree *) malloc(sizeof(tree));
   		if (t->right == NULL) {fprintf(stderr,"Memory allocation for tree failure.\n"); abort();}
        readtree(line,t->right,n);
     }
}

int pMolGeneSplicerB::find(char *line, int start)
{
 int stop,i;

 i=start;

 while(line[i]!=')')
 	if(line[i]=='(') i=find(line,i+1);
 	else i++;
 stop=i+1;
 return(stop);
}
 	

int pMolGeneSplicerB::comp(const void *a, const void *b)
{ 
  if(*(double *)a > *(double *)b) return(1);
  else if (*(double *)a==*(double *)b) return(0);
  else return(0);

}  
  

int pMolGeneSplicerB::fileFind(const int * S, tree *t)
{
	int val, cons, poz;
	val=t->val;

	cons=t->consens;
	if( cons !=-1)
	{ 
		poz=t->poz;
	    if(S[poz]==cons)
	    	val=fileFind(S,t->left);
	    else val=fileFind(S, t->right);
	}

	return(val);
}


int  pMolGeneSplicerB::Acc  (const int * S, double * Return_Score, tree *t,int ind)

/* Evaluate string  S [0 .. (ACCEPTOR_LEN -1)] and
*  return  TRUE  or  FALSE  as to whether it is a likely acceptor
*  site.  Also set  Return_Score  to the probability that it is an acceptor
*  site. */

  {
   FILE  * Infile;
   static float  Positive_Table[300][ACCEPTOR_LEN] [ALPHABET_SIZE] [MARKOV_LEN];
   static float  Negative_Table[300][ACCEPTOR_LEN] [ALPHABET_SIZE] [MARKOV_LEN];
   static int Tables_Loaded[300]={FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE};
   double  Positive_Sum, Negative_Sum, Score;
   QString accname, temp;
#if  RETURN_TRUE_PROB
   double  X, Y;
#endif
   int  i, j, k, Ct, Sub, no;

/* see which acceptor you should use */

/*
  //strcpy(TRAIN_DIR,trainingDir..toAscii().data());
  QString configFilename = trainingDirectory+"/config_file";
  // read config file
  //sprintf(filename,"%s/config_file",TRAIN_DIR);
  fp = fopen (configFilename.toAscii().data(), "r");
*/

if(ind) 
  {
	no=fileFind(S,t);
	//sprintf(accname,"%s/%s%d",TRAIN_DIR,ACCEPTOR_FILE_NAME,no);
       accname = trainingDirectory + "/" + ACCEPTOR_FILE_NAME + temp.setNum(no);

  }
else 
  {
    //strcpy(accname,TRAIN_DIR);
    //strcat(accname,"/acc1.mar");
     accname = trainingDirectory + "/acc1.mar";
    no=0;
  }

   if  (! Tables_Loaded[no])
       {
        Infile = fopen (accname.toAscii().data(), "r");
        if  (Infile == NULL)
            {
             qDebug() << "ERROR:  Unable to open acceptor file " <<  accname;
             exit (EXIT_FAILURE);
            }

        for  (i = MARKOV_DEGREE - 1;  i < ACCEPTOR_LEN;  i ++)
          for  (k = 0;  k < MARKOV_LEN;  k ++)
            for  (j = 0;  j < ALPHABET_SIZE;  j ++)
              {
               Ct = fscanf (Infile, "%f", & Positive_Table [no][i] [j] [k]);
               if  (Ct != 1)
                   {
                    qDebug() << "ERROR reading acceptor file" << accname;
                    exit (EXIT_FAILURE);
                   }
              }

        for  (i = MARKOV_DEGREE - 1;  i < ACCEPTOR_LEN;  i ++)
          for  (k = 0;  k < MARKOV_LEN;  k ++)
            for  (j = 0;  j < ALPHABET_SIZE;  j ++)
              {
               Ct = fscanf (Infile, "%f", & Negative_Table [no][i] [j] [k]);
               if  (Ct != 1)
                   {
                    qDebug() << "ERROR reading acceptor file" << accname;
                    exit (EXIT_FAILURE);
                   }
              }

        fclose (Infile);

        Tables_Loaded[no]  = TRUE;
       }

   if  (S [ACCEPTOR_SIGNAL_OFFSET] != 0
           || S [ACCEPTOR_SIGNAL_OFFSET + 1] != 2)    /* AG */
       {
        * Return_Score = LOW_SCORE;
        return  FALSE;
       }

   Sub = 0;
   for  (i = 0;  i < MARKOV_DEGREE;  i ++)
     Sub = ALPHABET_SIZE * Sub + S [i];

   Positive_Sum = Positive_Table [no][MARKOV_DEGREE - 1] [0] [Sub];
   Negative_Sum = Negative_Table [no][MARKOV_DEGREE - 1] [0] [Sub];

   for  (i = MARKOV_DEGREE;  i < ACCEPTOR_LEN;  i ++)
     {
      j = S [i];
      Positive_Sum += Positive_Table [no] [i] [j] [Sub];
      Negative_Sum += Negative_Table [no] [i] [j] [Sub];
      Sub = ALPHABET_SIZE * (Sub % (MARKOV_LEN / ALPHABET_SIZE)) + j;
     }
  


   Score = Positive_Sum - Negative_Sum;

#if  RETURN_TRUE_PROB
   X = exp (Positive_Sum + LOG_PROB_ACCEPTOR);
   Y = exp (Negative_Sum + LOG_PROB_NONACCEPTOR);
   * Return_Score = log (X / (X + Y));
#else
   * Return_Score = Score;
#endif

   return(1);
  }



int  pMolGeneSplicerB::Don  (const int * S, double * Return_Score, tree *t,int ind)

/* Evaluate string  S [0 .. (DONOR_LEN -1)] and
*  return  TRUE  or  FALSE  as to whether it is a likely donor
*  site.  Also set  Return_Score  to the probability that it is an donor
*  site. */
{
   FILE  * Infile;
   static float  Positive_Table [300][DONOR_LEN] [ALPHABET_SIZE] [MARKOV_LEN];
   static float  Negative_Table [300][DONOR_LEN] [ALPHABET_SIZE] [MARKOV_LEN];
   static int Tables_Loaded[300]={FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE};
   double  Positive_Sum, Negative_Sum, Score;
   int no;

#if  RETURN_TRUE_PROB
   double  X, Y;
#endif
   int  i, j, k, Ct, Sub;
QString donname, temp;
   /* see which donor file you should use */
/*
  //strcpy(TRAIN_DIR,trainingDir..toAscii().data());
  QString configFilename = trainingDirectory+"/config_file";
  // read config file
  //sprintf(filename,"%s/config_file",TRAIN_DIR);
  fp = fopen (configFilename.toAscii().data(), "r");
*/

if(ind)
   { no=fileFind(S,t);
     donname=trainingDirectory+"/"+DONOR_FILE_NAME+temp.setNum(no);
    //sprintf(donname,"%s/%s%d",TRAIN_DIR,DONOR_FILE_NAME,no);
   }
else 
{
  //strcpy(donname,TRAIN_DIR);
  //strcat(donname,"/don1.mar");
   donname = trainingDirectory+"/don1.mar";
  no=0;
}

   if  (! Tables_Loaded[no] )
       {

        Infile = fopen (donname.toAscii().data(), "r");
        if  (Infile == NULL)
            {
             qDebug() << "ERROR:  Unable to open donor file " <<  donname;
             exit (EXIT_FAILURE);
            }

        for  (i = MARKOV_DEGREE - 1;  i < DONOR_LEN;  i ++)
          for  (k = 0;  k < MARKOV_LEN;  k ++)
            for  (j = 0;  j < ALPHABET_SIZE;  j ++)
              {
               Ct = fscanf (Infile, "%f", & Positive_Table [no] [i] [j] [k]);
               if  (Ct != 1)
                   {
                    qDebug() << "ERROR reading donor file" << donname;
                    exit (EXIT_FAILURE);
                   }
              }

        for  (i = MARKOV_DEGREE - 1;  i < DONOR_LEN;  i ++)
          for  (k = 0;  k < MARKOV_LEN;  k ++)
            for  (j = 0;  j < ALPHABET_SIZE;  j ++)
              {
               Ct = fscanf (Infile, "%f", & Negative_Table [no] [i] [j] [k]);
               if  (Ct != 1)
                   {
                    qDebug() << "ERROR reading donor file" << DONOR_FILE_NAME;
                    exit (EXIT_FAILURE);
                   }
              }

        fclose (Infile);

        Tables_Loaded [no] = TRUE;
       }


   if  (S [DONOR_SIGNAL_OFFSET] != 2
           || S [DONOR_SIGNAL_OFFSET + 1] != 3)    /* GT */
       {
        * Return_Score = LOW_SCORE;
        return  FALSE;
       }

   Sub = 0;
   for  (i = 0;  i < MARKOV_DEGREE;  i ++)
     Sub = ALPHABET_SIZE * Sub + S [i];

   Positive_Sum = Positive_Table [no] [MARKOV_DEGREE - 1] [0] [Sub];
   Negative_Sum = Negative_Table [no] [MARKOV_DEGREE - 1] [0] [Sub];

   for  (i = MARKOV_DEGREE;  i < DONOR_LEN;  i ++)
     {
      j = S [i];
      Positive_Sum += Positive_Table [no] [i] [j] [Sub];
      Negative_Sum += Negative_Table [no] [i] [j] [Sub];
      Sub = ALPHABET_SIZE * (Sub % (MARKOV_LEN / ALPHABET_SIZE)) + j;
     }
 
   Score = Positive_Sum - Negative_Sum;

#if  RETURN_TRUE_PROB
   X = exp (Positive_Sum + LOG_PROB_DONOR);
   Y = exp (Negative_Sum + LOG_PROB_NONDONOR);
   * Return_Score = log (X / (X + Y));
#else
   * Return_Score = Score;
#endif

	if(Score==-99) printf("look one\n");

   return(1);
  }

  
int  pMolGeneSplicerB::Is_Cod_NonCod  (const int * S, double * Return_Score, int ind)

/* Evaluate string  S [0 .. (CODING_LEN -1)] and
*  return  TRUE  or  FALSE  as to whether it is a likely donor
*  site.  Also set  Return_Score  to the probability that it is an donor
*  site. */

  {
   FILE  * Infile;
   static float  Positive_Table [4][CODING_LEN] [ALPHABET_SIZE] [MARKOV_LEN];
   static float  Negative_Table [4][CODING_LEN] [ALPHABET_SIZE] [MARKOV_LEN];
   static  int  Tables_Loaded[4] = {FALSE,FALSE,FALSE,FALSE};
   double  Positive_Sum, Negative_Sum, Score, Threshold;
   QString filename;
   int no;


#if  RETURN_TRUE_PROB
   double  X, Y;
#endif
   int  i, j, k, Ct, Sub;

   no=ind;
/*
  //strcpy(TRAIN_DIR,trainingDir..toAscii().data());
  QString configFilename = trainingDirectory+"/config_file";
  // read config file
  //sprintf(filename,"%s/config_file",TRAIN_DIR);
  fp = fopen (configFilename.toAscii().data(), "r");
*/

   switch (no) {
   case 0: // case of exon in acceptor
     filename = trainingDirectory+"/score_ex.acc";
     break;
   case 1: // case of intron in acceptor
     filename = trainingDirectory+"/score_in.acc";
     break;
   case 2: // case of exon in donor
     filename = trainingDirectory+"/score_ex.don";
     break;
   case 3: // case of intron in donor
     filename = trainingDirectory+"/score_in.don";
     break;
   }

   if  (! Tables_Loaded[no] )
       {
        Infile = fopen (filename.toAscii().data(), "r");
        if  (Infile == NULL)
            {
             qDebug() << "ERROR:  Unable to open donor file" << filename;
             exit (EXIT_FAILURE);
            }

        for  (i = MARKOV_DEGREE - 1;  i < CODING_LEN;  i ++)
          for  (k = 0;  k < MARKOV_LEN;  k ++)
            for  (j = 0;  j < ALPHABET_SIZE;  j ++)
              {
               Ct = fscanf (Infile, "%f", & Positive_Table [no] [i] [j] [k]);
               if  (Ct != 1)
                   {
                    qDebug() <<"ERROR reading donor file" << filename;
                    exit (EXIT_FAILURE);
                   }
              }

        for  (i = MARKOV_DEGREE - 1;  i < CODING_LEN;  i ++)
          for  (k = 0;  k < MARKOV_LEN;  k ++)
            for  (j = 0;  j < ALPHABET_SIZE;  j ++)
              {
               Ct = fscanf (Infile, "%f", & Negative_Table [no] [i] [j] [k]);
               if  (Ct != 1)
                   {
                    qDebug() << "ERROR reading donor file" << DONOR_FILE_NAME;
                    exit (EXIT_FAILURE);
                   }
              }

        fclose (Infile);

        Tables_Loaded [no] = TRUE;
       }

   Sub = 0;
   for  (i = 0;  i < MARKOV_DEGREE;  i ++)
     Sub = ALPHABET_SIZE * Sub + S [i];

   Positive_Sum = Positive_Table [no] [MARKOV_DEGREE - 1] [0] [Sub];
   Negative_Sum = Negative_Table [no] [MARKOV_DEGREE - 1] [0] [Sub];

   for  (i = MARKOV_DEGREE;  i < CODING_LEN;  i ++)
     {
      j = S [i];
      Positive_Sum += Positive_Table [no] [i] [j] [Sub];
      Negative_Sum += Negative_Table [no] [i] [j] [Sub];
      Sub = ALPHABET_SIZE * (Sub % (MARKOV_LEN / ALPHABET_SIZE)) + j;
     }
 


   Score = Positive_Sum - Negative_Sum;

#if  RETURN_TRUE_PROB
   X = exp (Positive_Sum + LOG_PROB_DONOR);
   Y = exp (Negative_Sum + LOG_PROB_NONDONOR);
   * Return_Score = log (X / (X + Y));
#else
   * Return_Score = Score;
#endif

   return (1);
  }





