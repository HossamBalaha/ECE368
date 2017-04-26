#ifndef __proj3_h_
#define __proj3_h_

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define INF 2000000000


typedef struct _AdjNode{
  int num;
  int weight;
  struct _AdjNode*next;
}AdjNode;

typedef struct _Vnode{
  int num;
  int x;
  int y;
  struct _AdjNode*next;
  
}Vnode;

typedef struct _AdjList{
  struct _Vnode*head;
}AdjList;

typedef struct _Graph{
  int V;
  struct _AdjList*array;
}Graph;

typedef struct _minheap{
  int size;
  int *pos;
  struct _Mnode **array;
}minheap;

typedef struct _Mnode{
  int idx;
  int dist;
}Mnode;



AdjNode *create_AdjNode(int number);
Vnode *create_Vnode(int num,int x,int y);
Mnode *create_Mnode(int index,int dist);
Graph *create_Graph(int V);
Graph *load_File(char *Filename);
int** load_query(int*size,char*Filename);
void insert_AdjNode(Vnode*head,AdjNode*node);
void print_Graph(Graph*graph);
void calculate_weight(Graph*graph);
void swap_Mnode(Mnode** a,Mnode** b);
void heapify(minheap* mhp,int parent);
Mnode* delete_Mnode(minheap*mhp);
minheap* construct_minheap(Graph*graph,int Size);
void dijkstra(Graph* graph, int s,int d,int*dist,int*prev);
int IsInMinheap(minheap*mhp,int num);
void decreaseKey(minheap*mhp,int dist,int idx);
void print_nodes(int* prev,int size,int s,int d);


#endif
