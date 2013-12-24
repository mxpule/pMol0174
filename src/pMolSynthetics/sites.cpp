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

#include "pMolSynthetics/sites.h"

int Is_Acceptor(const int *B, double *Return_Score)
{
  FILE  * Infile;
  double Score,S1,S2;
  static tree *tacc;
  static int readtacc=FALSE;
  char line[5000];
  int i,ind;
  int T[100];
  double score1,score2,score3;
  char treefile[PATH_MAX+1];

  if(!readtacc && acceptor_tree)
    {
   
      /* read the structure of the acceptor tree */

      sprintf(treefile,"%s/%s",TRAIN_DIR,ACCEPTOR_TREE_FILE);
      Infile = fopen (treefile, "r");
      if  (Infile == NULL)
	{
	  fprintf (stderr, "ERROR:  Unable to open file %s\n", ACCEPTOR_TREE_FILE);
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

int Is_Donor(const int *B, double *Return_Score)
{
  FILE  * Infile;
  double Score,S1,S2;
  static tree *tdon;
  static int readtdon=FALSE;
  char line[5000];
  int ind,i;
  int T[100];
  double score1,score2,score3;
  char treefile[PATH_MAX+1];

  if(!readtdon && donor_tree)
    {
   
      /* read the structure of the donor tree */

      sprintf(treefile,"%s/%s",TRAIN_DIR,DONOR_TREE_FILE);
      Infile = fopen (treefile, "r");
      if  (Infile == NULL)
	{
	  fprintf (stderr, "ERROR:  Unable to open file %s\n", DONOR_TREE_FILE);
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


    
void readtree(char *line, tree *t, int start)
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

int find(char *line, int start)
{
 int stop,i;

 i=start;

 while(line[i]!=')')
 	if(line[i]=='(') i=find(line,i+1);
 	else i++;
 stop=i+1;
 return(stop);
}
 	

int comp(const void *a, const void *b)
{ 
  if(*(double *)a > *(double *)b) return(1);
  else if (*(double *)a==*(double *)b) return(0);
  else return(0);

}  
  

int findfile(const int * S, tree *t)
{
	int val, cons, poz;
	val=t->val;

	cons=t->consens;
	if( cons !=-1)
	{ 
		poz=t->poz;
	    if(S[poz]==cons)
	    	val=findfile(S,t->left);
	    else val=findfile(S, t->right);
	}

	return(val);
}


int  Acc  (const int * S, double * Return_Score, tree *t,int ind)

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
   char accname[PATH_MAX+1];
#if  RETURN_TRUE_PROB
   double  X, Y;
#endif
   int  i, j, k, Ct, Sub, no;

/* see which acceptor you should use */

if(ind) 
  {
	no=findfile(S,t);
	sprintf(accname,"%s/%s%d",TRAIN_DIR,ACCEPTOR_FILE_NAME,no);

  }
else 
  {
    strcpy(accname,TRAIN_DIR);
    strcat(accname,"/acc1.mar");
    no=0;
  }

   if  (! Tables_Loaded[no])
       {
        Infile = fopen (accname, "r");
        if  (Infile == NULL)
            {
             fprintf (stderr, "ERROR:  Unable to open acceptor file \"%s\"\n",
                        accname);
             exit (EXIT_FAILURE);
            }

        for  (i = MARKOV_DEGREE - 1;  i < ACCEPTOR_LEN;  i ++)
          for  (k = 0;  k < MARKOV_LEN;  k ++)
            for  (j = 0;  j < ALPHABET_SIZE;  j ++)
              {
               Ct = fscanf (Infile, "%f", & Positive_Table [no][i] [j] [k]);
               if  (Ct != 1)
                   {
                    fprintf (stderr, "ERROR reading acceptor file \"%s\"\n", 
                                ACCEPTOR_FILE_NAME);
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
                    fprintf (stderr, "ERROR reading acceptor file \"%s\"\n", 
                                ACCEPTOR_FILE_NAME);
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



int  Don  (const int * S, double * Return_Score, tree *t,int ind)

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
   char donname[PATH_MAX+1];
   int no;

#if  RETURN_TRUE_PROB
   double  X, Y;
#endif
   int  i, j, k, Ct, Sub;

   /* see which donor file you should use */
if(ind)
   { no=findfile(S,t);
    sprintf(donname,"%s/%s%d",TRAIN_DIR,DONOR_FILE_NAME,no);
   }
else 
{
  strcpy(donname,TRAIN_DIR);
  strcat(donname,"/don1.mar");
  no=0;
}

   if  (! Tables_Loaded[no] )
       {

        Infile = fopen (donname, "r");
        if  (Infile == NULL)
            {
             fprintf (stderr, "ERROR:  Unable to open donor file \"%s\"\n",
                        donname);
             exit (EXIT_FAILURE);
            }

        for  (i = MARKOV_DEGREE - 1;  i < DONOR_LEN;  i ++)
          for  (k = 0;  k < MARKOV_LEN;  k ++)
            for  (j = 0;  j < ALPHABET_SIZE;  j ++)
              {
               Ct = fscanf (Infile, "%f", & Positive_Table [no] [i] [j] [k]);
               if  (Ct != 1)
                   {
                    fprintf (stderr, "ERROR reading donor file \"%s\"\n", 
                                DONOR_FILE_NAME);
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
                    fprintf (stderr, "ERROR reading donor file \"%s\"\n", 
                                DONOR_FILE_NAME);
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

  
int  Is_Cod_NonCod  (const int * S, double * Return_Score, int ind)

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
   char filename[PATH_MAX+1];
   int no;


#if  RETURN_TRUE_PROB
   double  X, Y;
#endif
   int  i, j, k, Ct, Sub;

   no=ind;

   switch (no) {
   case 0: // case of exon in acceptor
     strcpy(filename,TRAIN_DIR);
     strcat(filename,"/score_ex.acc");
     break;
   case 1: // case of intron in acceptor
     strcpy(filename,TRAIN_DIR);
     strcat(filename,"/score_in.acc");
     break;
   case 2: // case of exon in donor
     strcpy(filename,TRAIN_DIR);
     strcat(filename,"/score_ex.don");
     break;
   case 3: // case of intron in donor
     strcpy(filename,TRAIN_DIR);
     strcat(filename,"/score_in.don");
     break;
   }

   if  (! Tables_Loaded[no] )
       {
        Infile = fopen (filename, "r");
        if  (Infile == NULL)
            {
             fprintf (stderr, "ERROR:  Unable to open donor file \"%s\"\n",
                        filename);
             exit (EXIT_FAILURE);
            }

        for  (i = MARKOV_DEGREE - 1;  i < CODING_LEN;  i ++)
          for  (k = 0;  k < MARKOV_LEN;  k ++)
            for  (j = 0;  j < ALPHABET_SIZE;  j ++)
              {
               Ct = fscanf (Infile, "%f", & Positive_Table [no] [i] [j] [k]);
               if  (Ct != 1)
                   {
                    fprintf (stderr, "ERROR reading donor file \"%s\"\n", 
                                DONOR_FILE_NAME);
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
                    fprintf (stderr, "ERROR reading donor file \"%s\"\n", 
                                DONOR_FILE_NAME);
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




