#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include<string.h>
#include "huffman.h"
tnode *construct_tnode(char Data,int Weight){
  tnode* new = (tnode *)malloc(sizeof(tnode));
  new -> data = Data;
  new -> weight = Weight;
  new -> left = NULL;
  new -> right = NULL;
  return new;
}
void InorderPrint(tnode*node){
  if(node == NULL){
    return;
  }
  InorderPrint(node->left);
  printf("%c\n",node->data);
  InorderPrint(node->right);
}

int main(int argc, char** argv){

  int size=0;
  int cnt=8,ccnt=0,c=0;
  tnode* huff_tree,*root;
  FILE *fptr =fopen(argv[1],"rb");
 if (fptr == NULL){
    fprintf
      (stderr,"Cannot load file %s\n", argv[1]);
    return EXIT_FAILURE;
  }
  int length=strlen(argv[1]);
  char *outfile_name=(char*)malloc((length+7)*sizeof(char));
  sprintf(outfile_name,"%s.unhuff",argv[1]);
  FILE *outfptr=fopen(outfile_name,"w");

  unsigned char bit_fld=0;
  huff_tree=construct_tnode('@',0);
  fread(&size,4,1,fptr);

  read_tree(fptr,huff_tree,size,&ccnt,&bit_fld,&cnt);
  root=huff_tree;
  // InorderPrint(huff_tree);
    while(1){

      c=read_bits(&bit_fld,1,&cnt,fptr);
      if(c==0){
	root=root->left;
      }
      else{
	root=root->right;
      }
      
      if(root->left==NULL&&root->right==NULL){
	if(root->data==EOF){
	  break;
	}
	else{
	  fprintf(outfptr,"%c",root->data);
	  root=huff_tree;
      }
    }
    }
  
      
  fclose(fptr);
  fclose(outfptr);
  free(huff_tree);
  free(outfile_name);
  return 0;
}

void read_tree(FILE *fptr,tnode*root,int size,int*ccnt,unsigned char*bit_fld,int*cnt){
  int ch;
  if(*ccnt==size){return;}
  else{
    ch=read_bits(bit_fld,1,cnt,fptr);
    //printf("%d",ch);
    if(ch==1){
      
      root->data=(char)read_bits(bit_fld,8,cnt,fptr);
      root->weight=1;
      *ccnt+=1;
      return;
    }
      else if(ch==0){
    root->left=construct_tnode('@',0);
    root->right=construct_tnode('@',0);
      
      }
  
  read_tree(fptr,root->left,size,ccnt,bit_fld,cnt);
  read_tree(fptr,root->right,size,ccnt,bit_fld,cnt);
  return;
  }		 
}
unsigned char read_bits(unsigned char*bit_fld,int num,int*cnt,FILE*fptr){
  unsigned char rvalue=0;
  unsigned char buff=0;
  if(*cnt==8){
    fread(bit_fld,1,1,fptr);
    *cnt=0;
  }
  
  if(num==1){
    buff=(*bit_fld>>7);
    if(buff==1){
      rvalue= 1;
    }
    else 
      {rvalue= 0;}
    *bit_fld<<=1;
    *cnt+=1;
    //printf("%d\n",*cnt);
  }
  else if(num == 8){
    rvalue=*bit_fld;
    fread(bit_fld,1,1,fptr);
    buff= *bit_fld;
    buff>>=(8-*cnt);
    rvalue+=buff;
    *bit_fld<<=(*cnt);
  }
  return rvalue;
}


  
  
    
    
