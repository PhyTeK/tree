#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX 1000
#define MAX2 2000
#define MAXK 4

struct Point{
  double x,y; // coordinates
  int cat; // category
  double dist; // distance
  char lab[5]; // test label
};


double euclide(struct Point,struct Point);
void sortdist(struct Point *,int);


void sortdist(struct Point *p,int n){
  // Simple insertion
  int i,j;
  double d;
  struct Point t;

  for(i=1;i<n;i++){
    t =  p[i];
    for (j=i-1;j >= 0 && t.dist < p[j].dist;j--)
      p[j+1] = p[j];
    p[j+1] = t; 
  }
}

double euclide(struct Point p1,struct Point p2){

  return sqrt(pow((p1.x - p2.x) + (p1.y - p2.y),2));
}

int main(int argc, char* argv){
  FILE *fout;
  int i,j,m,freq=0;
  char str1[5],str2[5];
  
  struct Point p[MAX2],cp[MAX2],q;

  fout = fopen("results","w");

  // Generate random points
  for(i=0;i<MAX2;i++){
    p[i].x = rand()%MAX2;
    p[i].y = rand()%MAX2;
    if((p[i].x < MAX && p[i].y < MAX) ||
       (p[i].x > MAX && p[i].y > MAX))
      p[i].cat = 0;
    else
      p[i].cat = 1;
    strcpy(p[i].lab,"");
  }

  // Backup p[i]
  for(i=0;i<MAX2;i++)
    cp[i]= p[i];
  
  // Test point
  if(argc > 0){
      q.x = (double)argv[1];
      q.y = (double)argv[2];

  }
  
  
  for(m=0;m<MAX2;m++){ // Apply knn to each point

    q.x = cp[m].x;
    q.y = cp[m].y;
    
  for(i=0;i<MAX2;i++)
    p[i].dist = euclide(p[i],q); // Collect distances from test point q

  sortdist(&p[0],MAX2); // sort by increasing distances from q=p[m]


  // Collect the MAXK closest neighboors and get the frequency

  freq = 0;
  for(i=0;i<MAXK && i != m;i++){
    if(p[i].cat == 1)
      freq++; 
    else if(p[i].cat == 0)
      freq--;
  }

  //printf("\nPoint has category %d (freq = %d)\n",(freq > 0 ? 1:0),freq);

  // Create a label typ 01 11 10 00
  sprintf(cp[m].lab,"%d", cp[m].cat);
  sprintf(str2,"%d", (freq > 0 ? 1:0));
  strcat(cp[m].lab,str2);

  } // next point q = cp[m]

  // Write results in file
  
  for(i=0;i<MAX2;i++)
    fprintf(fout,"%d %f %f %d %s\n",i,cp[i].x,cp[i].y,cp[i].cat,cp[i].lab);

  fclose(fout);
  
  return 0;
}
