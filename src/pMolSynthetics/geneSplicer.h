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

#ifndef GENESPLICER_H
#define GENESPLICER_H

#include <QString>
#include <QtGui>

#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

//#include "pMolSynthetics/sites.h"

#ifndef PATH_MAX
#define PATH_MAX 255
#endif

//-------------------
#include  <stdio.h>
#include  <math.h>
#include  <stdlib.h>
#include  <string.h>
#include  <ctype.h>
#include  <limits.h>
#include  <float.h>
#include  <time.h>
#include  <assert.h>

#ifndef PATH_MAX
#define PATH_MAX 255
#endif

extern char TRAIN_DIR[PATH_MAX+1];
extern unsigned char donor_tree;
extern unsigned char acceptor_tree;
extern unsigned char use_cod_noncod;

#define  TRUE  1
#define  FALSE  0
#define  ALPHABET_SIZE  4
#define  ACCEPTOR_LEN  29                    /* Positions +44,72 in a80 */
#define  ACCEPTOR_FILE_NAME "acc"
#define  ACCEPTOR_TREE_FILE "outex"
#define  ACCEPTOR_SIGNAL_OFFSET  24          /* Start of  AG  */
extern double  ACCEPTOR_THRESHOLD;


#define  DONOR_LEN  16                        /* Positions +5,20 in d80 */
#define  DONOR_FILE_NAME "don"
#define  DONOR_TREE_FILE "outin"
#define  DONOR_SIGNAL_OFFSET  5               /* Start of  GT  */
extern double  DONOR_THRESHOLD;

#define  MARKOV_DEGREE  1
#define  MARKOV_LEN  4                     /* ALPHABET_SIZE ^ MARKOV_DEGREE */
#define  LOG_PROB_ACCEPTOR  log (1682.0 / 30191.0)
#define  LOG_PROB_NONACCEPTOR  log (28509.0 / 30191.0)
#define  LOG_PROB_DONOR  log (1682.0 / 30191.0)         /* Change this */
#define  LOG_PROB_NONDONOR  log (28509.0 / 30191.0)     /* Change this */
#define  LOW_SCORE  -99.0  /* Score if pattern does not have GT or AG signal */
#define  RETURN_TRUE_PROB  0

#define CODING_LEN 80

#ifndef  EXIT_FAILURE
  #define  EXIT_FAILURE  -1
#endif
#ifndef  EXIT_SUCCESS
  #define  EXIT_SUCCESS  0
#endif

typedef struct tree {
	int val;
   	int consens;
   	int poz;
   	int no;
   	struct tree *left;
   	struct tree *right;
   } tree;

typedef unsigned int word;



#define  Start_PosEx 56
#define  Stop_PosEx 84

#define  Start_PosIn 75
#define  Stop_PosIn 90

#define  Start_Cod 0
#define  Stop_Cod 79

#define Start_NoCod 82
#define Stop_NoCod 161


//------------------



struct Site
{
  int type;
  long pos;
  double score;
  Site *prev;
  Site *forw;
  int dir;
};

class pMolGeneSplicerB
{

public:

  QString fastaName;
  QString trainingDirectory;

char FILE_NAME[100];
int UseFile;

int DISTA;  // represents the distance within I choose the best acceptor score
int DISTD;  // represents the distance within I choose the best donor score
double ACCEPTOR_HIGH_LIMIT;
double DONOR_HIGH_LIMIT;
double ACCEPTOR_THRESHOLD;
double DONOR_THRESHOLD;
char Path[PATH_MAX+1];
//char TRAIN_DIR[PATH_MAX+1];
unsigned char donor_tree;
unsigned char acceptor_tree;
unsigned char use_cod_noncod;


pMolGeneSplicerB(const QString &p_fastaName, const QString &p_trainingDirectory);

bool findSplicing();
char  Filter  (char );
void printsites(Site *, Site *, FILE *);
int  Is_Acceptor  (const int *, double *);
int  Is_Donor  (const int *, double *);
FILE *  File_Open  (const char *, const char *);
void LoadData(FILE *,char *);
void  Process_Options  (int, char * []);

int fileFind(const int * S, tree *t);

//---------------------------------------------

//int  Is_Acceptor_sites  (const int *, double *);
//int  Is_Donor_sites  (const int *, double *);
int  Acc  (const int *, double *, tree *t,int ind);
int  Don  (const int *, double *, tree *t,int ind);
int comp(const void *a, const void *b);





//
void readtree(char *line, tree *t, int start);
int find(char *line, int start);
int  Is_Cod_NonCod  (const int * , double *, int ind);

};

#endif
