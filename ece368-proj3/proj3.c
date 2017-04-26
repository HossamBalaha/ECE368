#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "proj3.h"

int main(int argc,char** argv){
  Graph*graph=load_File(argv[1]);
  

  int qsize,i;
  int dist[graph->V];
  int prev[graph->V];
  int**query= load_query(&qsize,argv[2]);
  calculate_weight(graph);
  for(i=0;i<qsize;i++){
      dijkstra(graph,query[i][0] ,query[i][1],dist,prev);
      print_nodes(prev,graph->V,query[i][0],query[i][1]);
  }  
 
  return 0;
}

AdjNode *create_AdjNode(int number){
  AdjNode* node = (AdjNode*)malloc(sizeof(AdjNode));
  node -> num =number;
  node -> weight =0;
  node -> next = NULL;
  return node; 
}

Vnode *create_Vnode(int num,int x,int y){
  Vnode* node = (Vnode*)malloc(sizeof(Vnode));
  node -> x=x;
  node -> y=y;
  node -> num =num;
  node -> next = NULL;
  return node; 
}
Mnode *create_Mnode(int index,int dist){
  Mnode* node=(Mnode*)malloc(sizeof(Mnode));
  node->idx=index;
  node->dist=dist;
  return node;
}
Graph *create_Graph(int vertex_num){
  Graph *graph =(Graph*)malloc(sizeof(Graph));
  graph->array=(AdjList*)malloc(vertex_num*sizeof(AdjList));
  graph->V=vertex_num;
  int i = vertex_num-1;
  while(i>=0){
    graph->array[i].head=NULL;
    i--;
  }
  return graph;
}

void insert_AdjNode(Vnode*head,AdjNode*node){
  AdjNode*curr=NULL;
  if(head->next==NULL){
    head->next=node;
  }else{
    curr=head->next;
    while(curr->next!=NULL){
      curr=curr->next;
    }
    curr->next=node;
  }
}

Graph *load_File(char *Filename){
  FILE*fptr = fopen(Filename,"r");
  int V,E,i,x,y,node_num,head_num,num;
  AdjNode* new;
  
  fscanf(fptr,"%d",&V);
  Graph* graph=create_Graph(V);
  
  fgetc(fptr);
  fscanf(fptr,"%d",&E);
  fgetc(fptr);
  for(i=0;i<V;i++){
    while(fgetc(fptr)==' '){}
    fseek(fptr,-1,SEEK_CUR);

    fscanf(fptr,"%d",&num);
   
    while(fgetc(fptr)==' '){}
    fseek(fptr,-1,SEEK_CUR);
   
    fscanf(fptr,"%d",&x);
   
    while(fgetc(fptr)==' '){}
    fseek(fptr,-1,SEEK_CUR);
    
    fscanf(fptr,"%d",&y);
    fgetc(fptr);
    
    graph->array[i].head=create_Vnode(num,x,y);
  }

  for(i=0;i<E;i++){
    while(fgetc(fptr)==' '){}
    fseek(fptr,-1,SEEK_CUR);
    fscanf(fptr,"%d",&head_num);
    
    while(fgetc(fptr)==' '){}
    fseek(fptr,-1,SEEK_CUR);
    fscanf(fptr,"%d",&node_num);
    fgetc(fptr);
    new=create_AdjNode(node_num);
    insert_AdjNode(graph->array[head_num].head,new);
     new=create_AdjNode(head_num);
    insert_AdjNode(graph->array[node_num].head,new);
  }   
  fclose(fptr);  
  
  return graph;
  
}

int** load_query(int*size,char*Filename){
  FILE*fptr = fopen(Filename,"r");
  int i;
  fscanf(fptr,"%d",size);
  fgetc(fptr);
  
  int**query=(int**)malloc(*size*sizeof(int*));
  for(i=0;i<*size;i++){
    query[i]=(int*)malloc(2*sizeof(int));
  }
  for(i=0;i<*size;i++){
    while(fgetc(fptr)==' '){}
    fseek(fptr,-1,SEEK_CUR);
    fscanf(fptr,"%d",&query[i][0]);
    while(fgetc(fptr)==' '){}
    fseek(fptr,-1,SEEK_CUR);
    fscanf(fptr,"%d",&query[i][1]);
    fgetc(fptr);
  }
  fclose(fptr);
  return query;
}
void calculate_weight(Graph*graph){
  int V,i,x1,y1,x2,y2; 
  AdjNode*curr=NULL;
  V = graph->V;
  for(i=0;i<V;i++){  
     if(graph->array[i].head->next!=NULL){
       curr=graph->array[i].head->next;
     }else{
       curr=NULL;
     }
     while(curr!=NULL){
       x1=graph->array[i].head->x;
       y1=graph->array[i].head->y;
       x2=graph->array[curr->num].head->x;
       y2=graph->array[curr->num].head->y;
       curr->weight=(int)sqrt(pow((double)abs(x1-x2),2.0)+pow((double)abs(y1-y2),2.0));
       curr=curr->next;
     }
  }
}

void print_Graph(Graph*graph){
  int i;
  
  AdjNode* curr;
  for(i=0;i<graph->V;i++){
    printf("%d:",graph->array[i].head->num);
    curr=graph->array[i].head->next;
    if(curr!=NULL){
      printf("% d",curr->num);
      while(curr->next!=NULL){
	curr=curr->next;
	printf("% d",curr->num);
      }
    }
    printf("\n");
  }
}


minheap *init_minheap(int Size){
  minheap* mhp=(minheap*)malloc(sizeof(minheap));
  mhp->size =Size;
  mhp->array=(Mnode**)malloc(Size*sizeof(Mnode*));
  mhp->pos=(int*)malloc(Size*sizeof(int));
  return mhp;
}


void swap_Mnode(Mnode** a,Mnode** b){
  Mnode* temp = *a;
  *a = *b;
  *b = temp;
}

void heapify(minheap* mhp,int parent){
  int min = parent;
  int left =2*parent+1;
  int right = 2*parent+2;
  if(left<mhp->size && (mhp->array[left]->dist) < (mhp->array[min]->dist)){
    min = left;
  }
  if(right<mhp->size && (mhp->array[right]->dist) < (mhp->array[min]->dist)){
    min = right;
  }
  if(parent != min){
    mhp->pos[mhp->array[min]->idx]=parent;
    mhp->pos[mhp->array[parent]->idx]=min;
    swap_Mnode(&mhp->array[min],&mhp->array[parent]);
    heapify(mhp,min);
  }
  
}

Mnode* delete_Mnode(minheap*mhp){
  Mnode* temp = mhp->array[0];
  mhp->array[0]=mhp->array[(mhp->size)-1];
  mhp->pos[mhp->array[0]->idx]=mhp->size-1;
  mhp->pos[mhp->array[mhp->size-1]->idx]=0;
  mhp->size=mhp->size-1;
  heapify(mhp,0);
  return temp;
}

minheap* construct_minheap(Graph*graph,int Size){
  minheap* mhp = init_minheap(Size);
  int i;
  for(i=0;i<Size;i++){
    mhp->array[i]=create_Mnode(i,INF);
    mhp->pos[i]=i;
  }
  return mhp;
}

int IsInMinheap(minheap*mhp,int num){
    if(mhp->pos[num]<mhp->size){
      return 1;
    }
  return 0;
}

void decreaseKey(minheap*mhp,int dist,int idx){
  int index=mhp->pos[idx];
  mhp->array[index]->dist=dist;
  while (index && mhp->array[index]->dist < mhp->array[(index-1)/2]->dist){
    mhp->pos[mhp->array[index]->idx] = (index-1)/2;
    mhp->pos[mhp->array[(index-1)/2]->idx] = index;
    swap_Mnode(&mhp->array[index],&mhp->array[(index-1)/2]);
    index =(index-1)/2;
  }
}


void print_nodes(int* prev,int size,int s,int d){
  int i,index,cnt;
  int c[size];
  cnt=1;
  for(i=0;i<size;i++){
    c[i]=0;
  }
 
  c[0]=d;
  index=d;
  while(index!=s){
    c[cnt]=prev[index];
    index=prev[index];
    cnt++;
  }
  c[cnt]=s;
    for(i=cnt-1;i>=0;i--){
      printf("%d ",c[i]);
    }
    printf("\n");
 
}
  



void dijkstra(Graph* graph, int s,int d,int*dist,int*prev){
  int w,i;
  int V = graph->V;
  minheap* mhp = construct_minheap(graph,V);
  Mnode* u =NULL;
  Vnode* head =NULL;
  AdjNode* curr=NULL;


  for(i=0;i<V;i++){
    dist[i]=INF;
    prev[i]=-1;
  }
  dist[s]=0;
  decreaseKey(mhp,0,s);

  while(mhp->size>0 ){
    u = delete_Mnode(mhp);
    head=graph->array[u->idx].head;
    if(head->next!=NULL){
      curr=head->next;
      while(curr!=NULL){
	w=curr->weight;
	if(IsInMinheap(mhp,curr->num) &&/* (dist[u->idx]!=INF) && */(dist[curr->num] > (dist[u->idx] + w))){
	  dist[curr->num] = dist[u->idx]+w;
	  prev[curr->num] = u->idx;
          decreaseKey(mhp,dist[curr->num],curr->num);
	}
      curr=curr->next;
      }
    }
      
    if (u->idx==d){
      break;
    }
  }
   printf("%d\n",dist[u->idx]);
   
}
