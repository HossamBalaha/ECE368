#include <stdlib.h>
#include <stdio.h>

typedef struct _AdjNode{
  int num;
  int x;
  int y;
  struct _AdjNode*next;
}AdjNode;

typedef struct _AdjList{
  struct _AdjNode*head;
}AdjList;

typedef struct _Graph{
  int V;
  struct _AdjList*array;
}Graph;

AdjNode *create_AdjNode(int x_val, int y_val,int number);
Graph *create_Graph(int V);
Graph *load_File(char *Filename);
void insertNode(AdjNode*head,AdjNode*node);
void print_Graph(Graph*graph);

int main(int argc,char** argv){
  Graph*graph=load_File(argv[1]);
  print_Graph(graph);
  return 0;
}

AdjNode *create_AdjNode(int x_val, int y_val,int number){
  AdjNode* node = (AdjNode*)malloc(sizeof(AdjNode));
  node -> num =number;
  node -> x =x_val;
  node -> y =y_val;
  node -> next = NULL;
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

void insertNode(AdjNode*head,AdjNode*node){
  AdjNode*curr=head;
  
  while(curr->next!=NULL){
    curr=curr->next;
  }
  curr->next=node;
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
    fscanf(fptr,"%d",&num);
   
    while(fgetc(fptr)==' '){}
    fseek(fptr,-1,SEEK_CUR);
   
    fscanf(fptr,"%d",&x);
   
    while(fgetc(fptr)==' '){}
    fseek(fptr,-1,SEEK_CUR);
    
    fscanf(fptr,"%d",&y);
    fgetc(fptr);
    
    graph->array[i].head=create_AdjNode(x,y,num);
  }

  for(i=0;i<E;i++){
    fscanf(fptr,"%d",&head_num);
    fgetc(fptr);
    fscanf(fptr,"%d",&node_num);
    fgetc(fptr);
    new=create_AdjNode(0,0,node_num);
    insertNode(graph->array[head_num].head,new);
  }   
  fclose(fptr);  
  
  return graph;
  
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
  


