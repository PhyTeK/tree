#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define NTREE 64  // Number of nodes
#define N 10      // Number of ??
#define ND 9      // Number of data

typedef enum {false,true} bool;
struct node {
  int index;
  int data[100];
  int class[100];
  int c;
  int ndt;  // number of data
  int ndc;  // number of classes
  double entropy;
  double gini;
  double gain;
  struct node *left;
  struct node *right;
  struct node *prev;
  struct node *next;
  struct node *parent;
};

double entropy(int *,int *,int,int);
double gain(int *,int *,int *,int,int,int);
double gini(int *,int);
double prob(int *,int,int);
void printree(struct node **,int, int);
void initnodes(struct node **,int *,int *);
void printresults(struct node **);

int classes(int *X,int *C, int n){
  //Find all different classes in X
  //Return how many and put classes in C

  int i,j,nc=0;
  bool new;
  
  for(i=0;i<n;i++){
    new=true;
    for(j=0;j<nc;j++){
      if(C[j] == X[i]){
	new=false;
	break;
      }
    }
    if(new)
      C[nc++] = X[i];
  }
  
  return nc;
}


double entropy(int *p,int *c,int n,int nc){
  // Mesure the non-homogeneity/uncertainly of p probalities
  double h=0,pb;
  
  for(int i=0;i<nc;i++){
    
    pb = prob(p,c[i],n);
    printf("pb[%d]=%f\n",i,pb);
    if(pb != 0.0)    
      h += pb*log2(pb);
  }
  
  return -h;
}

double gain(int *X,int *class,int *A,int nx,int na,int nc){
  // Gain = Entropy(X) - Conditional entropy(X|A)
  // CondEntropy(Y|X) = - Sum_{x,y}(p(x,y)*log(p(x,y)/p(x)))
  
  double g=0;

  double hx = entropy(X,class,nx,nc);
  double ha = entropy(A,class,na,nc);  // Warning! is nc correct?
  
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

void initnodes(struct node **tree,int *X,int *class){
  size_t i,t;
  
  for(i=0;i<NTREE;i++){
    tree[i]->index = i;
    tree[i]->gini = 0;
    tree[i]->entropy = 0;
    tree[i]->ndt = 0;
    for(t=0;t<ND;t++)
      tree[i]->data[t]=0;  // Clear all nodes data

    if(i>1 && i<NTREE-1){
      tree[i]->ndt = 0;
      tree[i]->left = tree[2*i+1];
      tree[i]->right = tree[2*i+2];
      if(i%2 == 0)
	tree[i]->parent = tree[i-1]->parent;
      else
	tree[i]->parent = tree[tree[i-1]->parent->index+1];
      tree[i]->prev = tree[i-1];
      tree[i]->next = tree[i+1];
      tree[i]->ndt = 0;
      tree[i]->ndc = 0;
    
    }else if(i==0){
      for(t=0;t<ND;t++){
	tree[0]->data[t]=X[t]; // Populate root with data
	tree[0]->ndt = ND;
	tree[0]->ndc = classes(X,class,ND);
      }
      tree[0]->parent = NULL;
      tree[0]->prev = NULL;
      tree[0]->left = tree[1];
      tree[0]->right = tree[2];
      // Decision on node 0
      /*
      for(int t=0;t<=ND;t++)
	if(tree[0]->data[t+1]<2)
	  tree[0]->left->data[t+1]=tree[0]->data[t+1];
	else
	  tree[0]->right->data[t+1]=tree[0]->data[t+1];
      */
      tree[1]->parent = tree[0];
      tree[2]->parent = tree[0];
      tree[0]->next = tree[1];
    }else if(i== NTREE-1){
      if(i%2)
	tree[i]->parent = tree[i-1]->parent;
      else
	tree[i]->parent = tree[tree[i-1]->parent->index+1];
      tree[i]->ndt = 0;
      tree[i]->ndc = 0;
      tree[i]->next = NULL;
      tree[i]->prev = tree[i-1];
      tree[i]->left = NULL;
      tree[i]->right = NULL;
    }
  }
}

void printresults(struct node **tree){
  // Print results
  
  int i,j,n=0,nmax=1,nbe=0,nl=1;
  char s[200];
  int space = 54, intspc=48;
  struct node *root=NULL;
  
  strcpy(s," ");
  for(i=0;i<space;i++)
    strcat(s," ");
  printf("%2d%s",1,s);

  while(1){ // while because not all nodes are used
    root = tree[n];  // Pointer to pointer in struc
    //printf("%x %d %d\n",root,n,root->index);
    //printf("%d ",root->index);
    strcpy(s," ");
    for(j=0;j<intspc;j++)
      strcat(s," ");
    //    printf("%x %x %d%s",root,*root,*(*root)->next->index,s);
    //printf("%x %x %d%s",tree[n],root->parent,1,s);
    // Print parents
    /*
    if(n>0)
      printf("(%d,%d)%s",tree[n]->index,tree[n]->parent->index,s);
    else
      printf("(%d,%d)%s",tree[n]->index,0,s);
    */
    
    // Print data
    printf("(%d,",tree[n]->index);
    for(int t=0;t<ND;t++)
      if(tree[n]->data[t] != 0)
	printf("%d",tree[n]->data[t]);
    printf(",%d,%2.1f)%s",tree[n]->c,tree[n]->entropy,s);

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
}


int main(){
  int i,j;
  int minc=10000;  // Optimal class
  double mine=10000.0; // Minimum entropy

  struct node *l=NULL,*r=NULL;
  
  //  l=(struct node *)malloc(sizeof(struct node));
  // r=(struct node *)malloc(sizeof(struct node));

  //double T[4][4]={{1,1,1,1},{1,1,0,1},{0,0,1,2},{1,0,0,2}};
  // T = {{X},{Y},{Z},{C}}
  int T[4][4]={{1,1,0,1},{1,1,0,0},{1,0,1,0},{1,1,2,2}};


  int X[ND]={1,3,1,1,4,2,1,3,2};
  int class[4]={1,2,3,4};
  
  struct node *rootL, *rootR, *firstL,*lnd,*rnd;
  struct node *tree[NTREE];

  // From root populate left then right childs
  // Then move root to first left child
  // If more than one class populate left and right childs
  // move root to first rigth child from root
  // If more than one class populate left and right childs
  // move root to first left child ...
  
  int Y[7] = {2,4,1,5,3,2,6};
  
  // Allocate all trees in memory
  for(i=0;i<NTREE;i++){
    tree[i] = (struct node *)malloc(sizeof(struct node));
    if(tree[i] == NULL){
      printf("Error Memory: Cannot allocate tree[%d]\n",i);
      exit(1);
    }
  }
  printf("Size of tree: %d\n",sizeof(tree));
  

  // Apply decisions
  // for each possible classes

  //double X[2]={(double)3/8,(double)5/8};

  // Data
  int C[ND];
  
  // Print data
  for(i=0;i<ND;i++)
    printf("%d ",X[i]);
  printf("\n");
  
  //printf("%f=%f ",(double)5/8,prob(X,1,8));
  //printf("%f=%f ",(double)3/8,prob(X,0,8));
  
  int nc=0;

  // Find all classes & print them
  nc = classes(X,C,ND);
  printf("Number of classes %d\n",nc);
  for(i=0;i<nc;i++)
    printf("%d ",C[i]);

  printf("Total entropy: %f\n",entropy(X,C,ND,nc));
  printf("\n");

  // Get the probability for each class & print them

  double P[nc];
  for(i=0;i<nc;i++){
    P[i] = prob(X,C[i],ND);
    printf("Prob of class %d = %f\n",C[i],P[i]);
  }

  double lprob[ND],rprob[ND];
  int lclass[ND],rclass[ND];
    
  // For each possible decisions/branch get the information gain
  // Sorting

  for(int c=0;c<nc;c++){
    
    initnodes(tree,X,class);  // Populate root with data from X
    tree[0]->gini = gini(tree[0]->data,tree[0]->ndt);
    
    for(i=0;i<ND;i++){
      tree[i]->c = C[c];
      for(int t=0;t<ND;t++){
	// Left and Right separation for class C[c]
	if(tree[i]->left != NULL && tree[i]->data[t] <= C[c]){
	  tree[i]->left->data[tree[i]->left->ndt]=tree[i]->data[t];
	  tree[i]->left->ndt++;

	}
	if (tree[i]->right != NULL && tree[i]->data[t] > C[c]){
	  tree[i]->right->data[tree[i]->right->ndt]=tree[i]->data[t];
	  tree[i]->right->ndt++;
	}
      }
      
      // Caculate entropy for each node

      if(tree[i]->left != NULL){
      	l = tree[i]->left; // Copy tree left pointer
	l->entropy = entropy(l->data,l->class,l->ndt,l->ndc);
      }
      if(tree[i]->right != NULL){
      	r = tree[i]->right; // Copy tree right pointer
	r->entropy = entropy(r->data,r->class,r->ndt,r->ndc);
      }

      printf("index=%d c=%d -> le %f\n",l->index,c,l->entropy);
      // Caculate Gini impurity
      if(tree[i]->left != NULL)
	tree[i]->left->gini = gini(tree[i]->left->data,tree[i]->left->ndt);
      if(tree[i]->right != NULL)
	tree[i]->right->gini = gini(tree[i]->right->data,tree[i]->right->ndt);

      // Caculate information gain

      if(tree[i]->left != NULL){
      	l = tree[i]->left; // Copy tree left pointer
	l->gain = gain(l->data,l->class,l->parent->data,l->ndt, l->parent->ndt,l->ndc);
      }
      if(tree[i]->right != NULL){
      	r = tree[i]->right; // Copy tree right pointer
	r->gain = gain(r->data,r->class,r->parent->data,r->ndt, r->parent->ndt,r->ndc);
      }
      
    } // i-loop

    // Record class with minima entropy/gini/gain
      
    if(l->entropy < mine){
      minc = C[c];
      mine = l->entropy;
    }

    printf("Optimal class: %d -> %f\n",minc,mine);

    printresults(tree);

    
  } // class c-loop

  printf("\n");

  //printf("\nleft entropy=%f",eleft);
  //printf("\nright entropy=%f\n",eright);
  // Find the optimal set of decisiosn
  
  // Get the final results
  
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
