#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "stapel.h"

static char UNUSED rcsid[] = "$Id: moves.c,v 1.1 2001/04/05 08:00:57 ivo Exp $";

void SPIN_move_it(char *string) {
  /* generate 1-point error mutants */
   int i,length;
   char *s;
   length = strlen(string);
   s = (char *) space((length+1)*sizeof(char));

   for (i=0;i<length;i++) { 
     strcpy(s,string);
     if( s[i]=='+') s[i]='-'; else s[i]='+';
     push(s);
   }
   free(s);
}

/* Move-sets on Permutations */
static int *String2Perm(char *perm) {
  int i,k,n;
  int *P;
  char *s,*line;
  line = strdup(perm);
  for (k=i=0;i<strlen(line);i++) if (line[i]==',') k++;
  n=k+1;
  P = (int *) space(sizeof(int)*(n+1));
  P[0]=n;
  s = strtok(line, ",\n");
  for (i=1;i<=n;i++) {
    (void)sscanf(s,"%d",&k);
    P[i]=k;
    s=strtok(NULL,",\n");
  }
  free(line);
  return P;
}

static char *Perm2String(int *P) {
  char tmp[1000];
  char digit[10];
  char *string;
  int i,n;
  n = P[0];
  tmp[0]='\0';
  for (i=1;i<n;i++) {
    sprintf(digit,"%d,",P[i]); strcat(tmp,digit);
  }
  sprintf(digit,"%d",P[i]); strcat(tmp,digit);
  string = strdup(tmp);
  return string;
}


void Transpos_move_it(char *perm) {
  int i,j,jj,n;
  int *P;
  char *s;
  
  P = String2Perm(perm);
  n = P[0];
  
  for (i=1;i<n;i++)
    for (j=i+1;j<=n;j++) {
      jj = P[i];P[i]=P[j];P[j]=jj;  /* transpose i,j */
      s = Perm2String(P);
      jj = P[i];P[i]=P[j];P[j]=jj;  /* transpose i,j again to restore P */
      push(s);
      free(s);
    }
  free(P);
}

void CTranspos_move_it(char *perm) {
  int i,j,jj,n;
  int *P;
  char *s;
  
  P = String2Perm(perm);
  n = P[0];
  
  for (i=1;i<n;i++) {
    j=i+1;
    jj = P[i];P[i]=P[j];P[j]=jj;  /* transpose i,j */
    s = Perm2String(P);
    jj = P[i];P[i]=P[j];P[j]=jj;  /* transpose i,j again to restore P */
    push(s);
    free(s);
  }
  free(P);
}

void Reversal_move_it(char *perm) {
  int i,j,k,n;
  int *P, *P1;
  char *s;
  
  P  = String2Perm(perm);  
  n  = P[0];
  P1 = space(sizeof(int)*(n+1));
  
  for (i=1;i<n;i++)
    for (j=i+1;j<=n;j++) {
      for (k=1;k<i;k++) P1[k]=P[k];
      for (k=0;k<j-i;k++) P1[i+k] = P[j-k];
      for (k=j+1;k<=n;k++) P1[k] = P[k];
      s = Perm2String(P);
      push(s);
      free(s);
    }
  free(P);
  free(P1);
}

/* Move-sets for Trees */
#include "trees.h"

void NNI_move_it(char *string) {
  /* Nearest neighbor Interchange moves */
   int i,nl,nneigh;
   char *s;
   Tree *T; Tree *T_NNI;

   /* printf("%s\n",string); */

   nl = number_of_leaves(string);
   nneigh = 2*(nl-3);
   T  = string2Tree(string);
   T_NNI = Make_all_NNI(T[0]);

   for (i=0;i<nneigh;i++) {
     s = Tree2string(T_NNI[i]);
     push(s);
     free(s);
   }

   FreeTree(T,1);
   FreeTree(T_NNI,nneigh);
}