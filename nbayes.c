#include <stdio.h>
#include <stdlib.h>

#define NMAX 14
#define RMAX 5

double bayes(int [RMAX][NMAX], int [RMAX],int,int);


double bayes(int Xi[RMAX][NMAX], int y[RMAX], int n, int r){
  /* 

     Naive-Bayes probability

     P(y|x1,x2,...xn) = P(y)*(P(x1|y)P(x2|y)...P(xn|y))

     P(y) = Class probability
     
     + normalization
  */

  int i,j;
  int nb0,nb1,s1,s2;
  double py0,py1,p1,p2,pys;

  double s[r];

  // Sum all P(xi|y)
  
  for(i=0;i<r;i++){
    s[i] = 0;
    for(j=0;j<n;j++){
      if(Xi[i][j] == y[i] && Xi[r-1][j] == y[r-1]){
	s[i]++;
      }
    }
  }

  printf("\n");
  for(i=0;i<r;i++){
      printf("%1.0f ",s[i]);
   }

  printf("\n");
  
  // Normalization

  // Class probabilities pc0=nb0/n,pc1=nb1/n

  for(i=0,nb1=0,nb0=0;i<n;i++){
    if(Xi[r-1][i] == 1) nb1++;
    if(Xi[r-1][i] == 0) nb0++;
  }
  printf("nb0=%d, nb1=%d\n",nb0,nb1);
  
  for(i=0;i<r-1;i++){
    if(y[r-1] == 1) s[i] /= nb1;
    if(y[r-1] == 0) s[i] /= nb0;
  }

  s[r-1] /= n;
  
  for(i=0,pys=1.0;i<r;i++){
    printf("%1.2f ",s[i]);
    pys *= s[i];
  }
  printf("%f\n",pys);
  
  return pys;
}

int main(){
  int i,j;
  int s=0;
  double py1,py0;
  
  // Rain(0), Overcast(1), Sunny(2)
  // Temperature: Hot(0),Mild(1),Cool(2)
  // Humidity: High(0), Normal(1)
  // Wind: False(0), True(1)
  // Play Golf: False(0), True(1)
  // Outcome = {0,0,1,2,2,2,1,0,0,2,0,1,1,2},
  // Sunny <-> Rainy gives identical result with article
  // http://geeksforgeeks.org/naive-bayes-classifiers/
  
  int Xi[RMAX][NMAX] = {
			{0,0,1,2,2,2,1,0,0,2,0,1,1,2},
			{0,0,0,1,2,2,2,1,2,1,1,1,0,1},
			{0,0,0,0,1,1,1,0,1,1,1,0,1,0},
			{0,1,0,0,0,1,1,0,0,0,1,1,0,1},
			{0,0,1,1,1,0,1,0,1,1,1,1,1,0}
		     };

  /*

  test = {sunny,hot,normal,false} 

  -> P(yes) = 0.0141 (67%)
  -> P(no)  = 0.0068 (33%)

  test2 = {rainy,hot,high,false}

  test3 = {overcast,cool,high,true}

  */

  int test1[RMAX] = {0,0,1,0,1};
  int test0[RMAX] = {0,0,1,0,0};
  int test21[RMAX] = {0,0,0,0,1};
  int test20[RMAX] = {0,0,0,0,0};
  int test31[RMAX] = {1,0,0,1,1};
  int test30[RMAX] = {1,0,0,1,0};

  
  // Print normalized probabilities of yes and no

  printf("\nTest1: {sunny,hot,normal,false}\n");
  printf("-------------------------------\n");
  py1 = bayes(Xi,test1,NMAX,RMAX);
  py0 = bayes(Xi,test0,NMAX,RMAX);

  printf("\n");
  printf("P1(y=%d)=%f\n",1,py1/(py1+py0));
  printf("P1(y=%d)=%f\n",0,py0/(py1+py0));

  printf("\nTest2: {rainy,hot,high,false}\n");
  printf("-----------------------------\n");
  py1 = bayes(Xi,test21,NMAX,RMAX);
  py0 = bayes(Xi,test20,NMAX,RMAX);
  printf("\n");
  printf("P2(y=%d)=%f\n",1,py1/(py1+py0));
  printf("P2(y=%d)=%f\n",0,py0/(py1+py0));

  printf("\nTest3: {overcast,cool,high,true}\n");
  printf("-------------------------------\n");
  py1 = bayes(Xi,test31,NMAX,RMAX);
  py0 = bayes(Xi,test30,NMAX,RMAX);
  printf("\n");
  printf("P3(y=%d)=%f\n",1,py1/(py1+py0));
  printf("P3(y=%d)=%f\n",0,py0/(py1+py0));

  printf("\nWarning !! If only one of the conditional probabilities=0\n");
  printf("Naive-Bayes algorythm get wrong as in test3\n");
  
  return 0;
}
  
