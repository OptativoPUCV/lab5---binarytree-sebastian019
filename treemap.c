#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
  TreeMap *map = (TreeMap *)malloc(sizeof(TreeMap));

  map -> lower_than = lower_than;
  map ->  root = NULL;
  map -> current = NULL;
   //new->lower_than = lower_than;
  return map;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
  tree -> current = tree -> root;
  TreeNode *node = createTreeNode(key,value);
  TreeNode *padre = tree -> current;
  while(tree->current != NULL){
    if(is_equal(tree,key,tree->current->pair->key)){
      return;
    }
    padre = tree->current;
    if(tree->lower_than(key,tree->current->pair->key) == 1){
      tree -> current = tree -> current -> left;
    }
    else{
      tree -> current = tree -> current -> right;
    }
  }
  if(tree->lower_than(key,padre->pair->key) == 1){
    padre -> left = node;
    node -> parent = padre;
  }
  else{
    padre -> left = node;
    node -> parent = padre;
  }
  tree -> current = node;
}

TreeNode * minimum(TreeNode * x){
  while(x -> left != NULL){
     x = x -> left;
  }
  return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
  if(node -> left == NULL && node -> right == NULL){
    if(node == tree -> root){
      tree -> root = NULL;
    }
    else if(node -> parent -> left == node){
      node -> parent -> left = NULL;
    }
    else{
      node -> parent -> right = NULL;
    }
  }

  else if(node -> left == NULL || node -> right){
    TreeNode *hijo;
    if(node -> left != NULL){
      hijo = node -> left;
    }
    else{
      hijo = node -> right;
    }
    if(node == tree -> root){
      tree -> root = hijo;
      hijo -> parent = NULL;
    }
    else if(node -> parent -> left == node){
      node -> parent -> left = hijo;
      hijo -> parent = node -> parent;
    }
    else{
      node -> parent -> right = hijo;
      hijo -> parent = node -> parent;
    }
  }
  else{
    TreeNode *min = minimum(node -> right);
    node -> key = min -> key;
    //node -> value = min -> value;
    removeNode(tree,min);
  }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}

Pair * searchTreeMap(TreeMap * tree, void* key) {
  tree -> current = tree -> root;
  while(tree->current != NULL){
    int resultado = is_equal(tree,key,tree->current->pair->key);
    if(resultado == 1){
      
      return tree->current->pair;
    }
    else if(tree->lower_than(key,tree->current->pair->key) == 1){
      tree -> current = tree -> current -> left;
      //return tree->current->pair;
    }
    else{
      tree -> current = tree -> current -> right;
      //return tree->current->pair;
    }
  }
  return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
  TreeNode *a = tree -> root;
  TreeNode *ub_node = NULL; 
 while(a != NULL){
    int resultado = is_equal(tree,key,a->pair->key);
    if(resultado == 1){
      
      return a->pair;
    }
    else if(tree->lower_than(key,a->pair->key) == 1){
      ub_node = a;
      a = a -> left;
    }
    else{
      a = a -> right;
    }
  }
  if(ub_node != NULL){
    return ub_node -> pair;
  }
  return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
  TreeNode * node = minimum(tree -> root);
  return node -> pair;
}

Pair * nextTreeMap(TreeMap * tree) {
  if(tree -> current == NULL){
    tree -> current = minimum(tree->root);
    return tree -> current -> pair;
  }
  if(tree -> current -> right != NULL){
    tree -> current = minimum(tree->current->right);
    return tree -> current -> pair;
  }
  TreeNode * padre = tree -> current -> parent;
  while(padre !=NULL && tree -> current == padre -> right){
    tree -> current = padre;
    padre = tree -> current -> parent;
  }
  tree -> current = padre;
  if(tree -> current == NULL){
    return NULL;
  }
  return tree -> current -> pair;
}
