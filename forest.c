#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

//#define NTREE 1+2^1+2^2+2^3+2^4+2^5+2^6
#define NTREE 64
#define N 10

struct node {
  int data[N];
  int ndt;  // number of data
  struct node *left;
  struct node *right;
  struct node *prev;
  struct node *next;
  struct node *parent;
};

double entropy(int *,int *,int,int);
double gain(int *,int *,int,int);
double gini(int *,int);
double prob(int *,int,int);
void printree(struct node **,int, int);

int classes(int *X,int *C, int n){
  //Find all different classes in X
  //Return how many and put classes in C

  int i,j,nc=0;
  
  for(i=0;i<n;i++){
    for(j=0;j<nc;j++){
      if(X[i] == C[j]) break;
    }
    C[++nc] = X[i];
  }
  
  return nc;
}


double entropy(int *p,int *c,int n,int nc){
  // Mesure the non-homogeneity/uncertainly of p probalities
  double h=0,pb;
  int i,j;

  for(i=0;i<nc;i++){
    pb = prob(p,c[i],n);
    h += pb*log2(pb);
  }
  return -h;
}

double gain(int *X,int *A,int nx,int na){
  // Gain = Entropy(X) - Conditional entropy(X|A)
  // CondEntropy(Y|X) = - Sum_{x,y}(p(x,y)*log(p(x,y)/p(x)))
  
  double g=0;
  int class[2] = {1,0};
  double hx = entropy(X,class,nx,2);
  double ha = entropy(A,class,na,2);
  
  for(int i=0;i<na;i++)
    g += ha*A[i]*prob(X,A[i],nx);
  return hx - g;
}

double gini(int X[],int n){
  double g=0;
  for(int i=0;i<n;i++)
    g += X[i]*X[i];

  return 1 - g;
}


double prob(int X[], int x,int n){
  // Return the probability to have x in X

  int i,p=0;

  for(i=0;i<n;i++){
    if(X[i] == x) p++;
  }
    
  return (double)p/n;
}


void printree(struct node **tree,int nd, int ndt){
  // Print all nodes nd from tree
  // nd = number of nodes
  // ndt = number of data in node n
  // 1                 2,4,1,5,3,2,6
  // 2          2,1,3,2             4,5,6
  // 3     2,1,2       3         4,5      6
  // 4    1     2,2             4   5
  // 5            
  
  int i,j,l=1,n;
  
  printf("Tree[%d]\n",NTREE);
  while(1){
    printf("%d",l++); // line number
    for(i=0;i<tree[0]->ndt;i++)
      printf("   ");
    // Print root
    for(i=0;i<tree[0]->ndt;i++)
      printf("%d ",tree[0]->data[i]);
    printf("\n%d",l++);
    for(i=0;i<tree[1]->ndt;i++)
      printf("   ");
    // print node 1
    for(i=0;i<tree[1]->ndt;i++)
      printf("%d ",tree[1]->data[i]);
    printf("               ");
    // print node 2
    for(i=0;i<tree[2]->ndt;i++)
      printf("%d ",tree[2]->data[i]);
    printf("\n%d",l++);
    printf("      ");
    // print node 3
    for(i=0;i<tree[3]->ndt;i++)
      printf("%d ",tree[3]->data[i]);
    printf("        ");
    // print node 4
    for(i=0;i<tree[4]->ndt;i++)
      printf("%d ",tree[4]->data[i]);
    printf("            ");
    // print node 5
    for(i=0;i<tree[5]->ndt;i++)
      printf("%d ",tree[5]->data[i]);
    printf("    ");
    // print node 6
    for(i=0;i<tree[6]->ndt;i++)
      printf("%d ",tree[6]->data[i]);
    printf("\n%d",l++);
    printf("\n");
    
    
    
    break;
       
  }
  
}

int main(){
  int i,j;
  //double X[2]={(double)3/8,(double)5/8};
  
  int X[N]={1,0,1,1,0,1,0,1};
  
  int class[2] = {1,0};
  //double T[4][4]={{1,1,1,1},{1,1,0,1},{0,0,1,2},{1,0,0,2}};
  // T = {{X},{Y},{Z},{C}}
  int T[4][4]={{1,1,0,1},{1,1,0,0},{1,0,1,0},{1,1,2,2}};

  printf("%f=%f ",(double)5/8,prob(X,1,8));
  printf("%f=%f ",(double)3/8,prob(X,0,8));
  
  printf("%f\n",entropy(X,class,8,2));


  struct node *tree[NTREE];

  // From root populate left then right childs
  // Then move root to first left child
  // If more than one class populate left and right childs
  // move root to first rigth child from root
  // If more than one class populate left and right childs
  // move root to first left child ...
  
  int Y[7] = {2,4,1,5,3,2,6};

  struct node *rootL, *rootR, *firstL,*lnd,*rnd;
  
  // Allocate all trees in memory
  for(i=0;i<NTREE;i++){
    tree[i] = (struct node *)malloc(sizeof(struct node));
  }

  // Init all nodes 
  for(i=0;i<NTREE;i++){
    tree[i]->data[0] = i;
    int line = i%2 + 1;
    if(i>2 && i<NTREE-1){
      tree[i]->left = tree[2*i+1];
      tree[i]->right = tree[2*i+2];
      if(i%2 == 0)
	tree[i]->parent = tree[i-1]->parent;
      else
	tree[i]->parent = tree[tree[i-1]->parent->data[0]+1];
      tree[i]->prev = tree[i-1];
      tree[i]->next = tree[i+1];
    }else if(i==0){
      tree[0]->parent = NULL;
      tree[0]->prev = NULL;
      tree[0]->left = tree[1];
      tree[0]->right = tree[2];
      tree[1]->parent = tree[0];
      tree[2]->parent = tree[0];
      tree[0]->next = tree[1];
    }else if(i== NTREE-1){
      if(i%2)
	tree[i]->parent = tree[i-1]->parent;
      else
	tree[i]->parent = tree[tree[i-1]->parent->data[0]+1];
      tree[i]->next = NULL;
      tree[i]->prev = tree[i-1];
      tree[i]->left = NULL;
      tree[i]->right = NULL;
    }
  }
  int n=0,nmax=1,nbe=0,nl=1;
  char s[200];
  int space = 54, intspc=48;
  struct node *root=NULL;
  
  strcpy(s," ");
  for(i=0;i<space;i++)
    strcat(s," ");
  printf("%2d%s",1,s);
  while(1){ // while because not all nodes are used
    root = tree[n];  // Pointer to pointer in struc
    //printf("%x %d %d\n",root,n,root->data[0]);
    //printf("%d ",root->data[0]);
    strcpy(s," ");
    for(j=0;j<intspc;j++)
      strcat(s," ");
    //    printf("%x %x %d%s",root,*root,*(*root)->next->data[0],s);
    //printf("%x %x %d%s",tree[n],root->parent,1,s);
    if(n>0)
      printf("(%d,%d)%s",tree[n]->data[0],tree[n]->parent->data[0],s);
    else
      printf("(%d,%d)%s",tree[n]->data[0],0,s);
    nbe++; // nbr of elements per line
    if(nbe >= nmax){ // nmax max nodes per line
      nl++;
      space -= 10;
      strcpy(s," ");
      for(j=0;j<space;j++)
	strcat(s," ");
      printf("\n%2d%s",nl,s);
      nbe=0;
      nmax *= 2;
      intspc /= 2;
    }
    n++;
    if(n == NTREE-1) break;
  }
  printf("\n");





  /*
  
  // Init root
  int nodeNb=0,r = 0;
  root = tree[r];

  for(i=0;i<root->ndt;i++)
    root->data[i] = Y[i];

  root->ndt = 7;
  root->left = tree[1];
  root->right = tree[2];
  nodeNb++;
  
  // Populate childs
  // Left & Right

  rootL = root->left;
  rootR = root->right;
  rootL->ndt = 0;
  rootR->ndt = 0;
  firstL = rootL;
  
  for(i=0;i<root->ndt;i++){
    if(root->data[i]<4){
      rootL->data[rootL->ndt++] = root->data[i];
    }else{
      rootR->data[rootR->ndt++] = root->data[i];
    }
  }
  printf("ndt%d = %d ndt%d = %d\n",nodeNb++,rootL->ndt,nodeNb++,rootR->ndt);
  
  
  // Move root to the first Left node
  // and propagate to next right node
  
  root = firstL;
  for(i=0;i<root->ndt;i++){
    if(root->data[i]<4){
      rootL->data[rootL->ndt++] = root->data[i];
    }else{
      rootR->data[rootR->ndt++] = root->data[i];
    }
  }
  printf("ndt%d = %d ndt%d = %d\n",nodeNb++,rootL->ndt,nodeNb++,rootR->ndt);
  
  // Init left and right nodes of root tree
  tree[1]->ndt = 0;
  tree[2]->ndt = 0;
  
  for(i=0;i<tree[0]->ndt;i++){
    if(tree[0]->data[i]<4){
      tree[1]->data[tree[1]->ndt] = tree[0]->data[i];
      tree[1]->ndt++;
    }else{
      tree[2]->data[tree[2]->ndt] = tree[0]->data[i];
      tree[2]->ndt++;
    }
  }
  printf("ndt1 = %d ndt2 = %d\n",tree[1]->ndt,tree[2]->ndt);

  // Init node 3&4
  tree[3]->ndt = 0;
  tree[4]->ndt = 0;


  for(i=0;i<tree[1]->ndt;i++){
    if(tree[1]->data[i]<3){
      tree[3]->data[tree[3]->ndt] = tree[1]->data[i];
      tree[3]->ndt++;
    }else{
      tree[4]->data[tree[4]->ndt] = tree[1]->data[i];
      tree[4]->ndt++;
    }
  }
  printf("ndt3 = %d ndt4 = %d\n",tree[3]->ndt,tree[4]->ndt);


  //Init node 5&6
  tree[5]->ndt = 0;
  tree[6]->ndt = 0;


  for(i=0;i<tree[2]->ndt;i++){
    if(tree[2]->data[i]<5){
      tree[5]->data[tree[5]->ndt] = tree[2]->data[i];
      tree[5]->ndt++;
    }else{
      tree[6]->data[tree[6]->ndt] = tree[2]->data[i];
      tree[6]->ndt++;
    }
  }
  printf("ndt5 = %d ndt6 = %d\n",tree[5]->ndt,tree[6]->ndt);

 

  printree(tree,6,7);
  */

  // Clean up memory

  for(i=0;i<NTREE;i++){
    free(tree[i]);
  }
 

  return 0;
}
