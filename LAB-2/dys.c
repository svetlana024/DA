#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <inttypes.h>
#include <stdbool.h>
#include <math.h>

#define S_LIMIT 257
#define BLACK 1
#define RED 0

const size_t BUFFER_SIZE = 4096;

typedef struct{
  int * value;
  int size;
  int capacity;
} Vector;

Vector * CreateVector(){
  Vector * v= (Vector *)malloc(sizeof(Vector));
  v-> value= (int *)malloc(sizeof(int));
  v-> capacity= 1;
  v-> size= 0;
  return v;
}

int Push(Vector * v, int value){
  if (v-> size == v-> capacity) {
    v-> value = (int *)realloc(v-> value, 2 * v-> capacity * sizeof(int));
	v-> capacity= v->capacity * 2;
  }
  v-> value[v->size]= value;
  v-> size++;
  return 0;
}

void Clear(Vector * v){
  v->size = 0;
}

void DestroyVector(Vector * v){
  free(v-> value);
  free(v);
}


typedef struct RBTree{
  struct RBTree * left;
  struct RBTree * right;
  struct RBTree * parent;
  int color;
  unsigned long long value;
  char * key;
} RBNode;

#define NIL &EmptyNode

RBNode EmptyNode = { NIL, NIL, NIL, BLACK, 0, 0};

RBNode * root = NIL;

void RBDestroy(RBNode * tree){
  if (tree == NIL || tree == NULL)
    return;
  RBNode * left = tree-> left;
  RBNode * right = tree-> right;
  if(left != NIL)
    RBDestroy(left);
  if(right != NIL)
    RBDestroy(right);
  free(tree-> key);
  free(tree);
}

void LeftRotate(RBNode * x) {
  RBNode * y = x-> right;
  x-> right = y-> left;
  if (y-> left != NIL)
    y-> left-> parent = x;
  if(y != NIL)
    y-> parent = x-> parent;
  if(x-> parent != NIL){
    if (x == x-> parent-> left)
      x-> parent-> left = y;
    else
      x-> parent-> right = y;
  }
  else
    root = y;
  y-> left = x;
  if (x != NIL)
    x-> parent = y;
}

void RightRotate(RBNode * x) {
  RBNode * y = x-> left;
  x-> left = y-> right;
  if(y-> right != NIL)
    y-> right-> parent = x;
  if(y != NIL)
    y-> parent = x-> parent;
  if(x-> parent != NIL){
    if(x == x-> parent-> right)
      x-> parent-> right = y;
    else
      x-> parent-> left = y;
  }
  else
    root = y;
  y-> right = x;
  if(x != NIL)
    x-> parent = y;
}

RBNode * RBInFind(RBNode * tree, char * key){
  int res;
  RBNode * node;
  if (tree == NIL){
    return tree;
  }
  while(tree != NIL){
    node = tree;
    res = strcmp(key, tree-> key);
    if(res > 0)
      tree = tree-> right;
    else if(res < 0)
      tree = tree-> left;
    else
      return NIL;
  }
  return node;
}

RBNode * RBFind(RBNode * tree, char * key){
  int res;
  if(tree == NIL)
    return NIL;
  while(tree != NIL){
    res = strcmp(key, tree-> key);
    if(res > 0)
      tree = tree-> right;
    else if(res < 0)
      tree = tree-> left;
    else
      return tree;
  }
  return tree;
}

void RBInsertFixUp(RBNode * x){
  while(x != root && x-> parent-> color == RED){
    if (x-> parent == x-> parent-> parent-> left){
      RBNode * y = x-> parent-> parent-> right;
      if(y-> color == RED){
        x-> parent-> color = BLACK;
        y-> color = BLACK;
        x-> parent-> parent-> color = RED;
        x = x-> parent-> parent;
      }
      else{
        if(x == x-> parent-> right){
          x = x-> parent;
          LeftRotate(x);
        }
        x-> parent-> color = BLACK;
        x-> parent-> parent-> color = RED;
        RightRotate(x-> parent-> parent);
      }
    }
    else {
      RBNode * y = x-> parent-> parent-> left;
      if(y-> color == RED){
        // uncle is RED
        x-> parent-> color = BLACK;
        y-> color = BLACK;
        x-> parent-> parent-> color = RED;
        x = x-> parent-> parent;
      }
      else{
        // uncle is BLACK
        if(x == x-> parent-> left){
          x = x->parent;
          RightRotate(x);
        }
        x-> parent-> color = BLACK;
        x-> parent-> parent-> color = RED;
        LeftRotate(x-> parent-> parent);
      }
    }
  }
  root-> color = BLACK;
}

bool RBInsert(unsigned long long int value, char * key, RBNode * parent){
  RBNode * node; 
  int res = 0;
  int size = strlen(key);
  if(parent != NIL)
    res = strcmp(key, parent-> key);
  node = (RBNode*)malloc(sizeof(RBNode));
  if(node == NULL)
    return false;
  node-> value = value;
  node-> key = (char*)malloc(size + 1);
  strcpy(node-> key, key);
  node->key[size] = '\0';
  node-> parent = parent;
  node-> left = NIL;
  node-> right = NIL;
  node-> color = RED;
  if(parent != NIL){
    if(res < 0 )
      parent-> left = node;
    else
      parent-> right = node;
  }
  else
    root = node;
  RBInsertFixUp(node);
  return true;
}

void RBDeleteFixUp(RBNode * x) {
  while(x != root && x-> color == BLACK){
    if(x == x-> parent-> left){
      RBNode * w = x-> parent-> right;
      if(w-> color == RED){
        w-> color = BLACK;
        x-> parent-> color = RED;
        LeftRotate(x->parent);
        w = x-> parent-> right;
      }
      if(w-> left-> color == BLACK && w-> right-> color == BLACK){
        w-> color = RED;
        x = x-> parent;
      }
      else{
        if(w-> right-> color == BLACK){
          w-> left-> color = BLACK;
          w-> color = RED;
          RightRotate(w);
          w = x->parent->right;
        }
        w-> color = x-> parent-> color;
        x-> parent-> color = BLACK;
        w-> right-> color = BLACK;
        LeftRotate(x->parent);
        x = root;
      }
    }
    else{
      RBNode * w = x-> parent-> left;
      if(w-> color == RED){
        w-> color = BLACK;
        x-> parent-> color = RED;
        RightRotate(x->parent);
        w = x-> parent-> left;
      }
      if(w-> right-> color == BLACK && w-> left-> color == BLACK){
        w->color = RED;
        x = x->parent;
      }
      else{
        if(w-> left-> color == BLACK){
          w-> right-> color = BLACK;
          w-> color = RED;
          LeftRotate(w);
          w = x-> parent->left;
        }
        w-> color = x-> parent-> color;
        x-> parent-> color = BLACK;
        w-> left-> color = BLACK;
        RightRotate(x->parent);
        x = root;
      }
    }
  }
  x-> color = BLACK;
}


RBNode * LoadFile(FILE * in){
  unsigned long long int value = 0;
  int count;
  int n;
  int k = fread(&n, sizeof(int), 1, in);
  if(k < 0)
    perror("fread");
  int * array = (int*)malloc(sizeof(int) * n);
  root = NIL;
  while((count = (int) fread(array, sizeof(int), (size_t) n, in))){
    for (int i = 0; i < (count == n
                        ? count - 1
                        : count); ++i){
      int k = array[i];
      char *string = (char*)malloc(sizeof(char) * (k + 1));
      ++i;
      for (int j = 0; j < k; ++j, ++i){
        string[j] = (char) array[i];
      }
      string[k] = '\0';
      value = (((unsigned long long)array[i] << 32) + array[i + 1]);
      RBNode * found = RBInFind(root, string);
      RBInsert(value, string, found);
      ++i;
    }
    if (count == n){
      n = array[count - 1];
      free(array);
      array = (int*)malloc(sizeof(int) * n);
    }
    else
      break;
  }
  free(array);
  return root;
}

void UploadFile(RBNode * tree, FILE * file, Vector * v){
  int size;
  int i = 0;
  if (tree == NIL)
  	return;
  if(tree-> left != NIL)
    UploadFile(tree->left, file, v);
  if(tree-> right!= NIL)
    UploadFile(tree-> right, file, v);
  if(tree != NIL){
    size = (int) strlen(tree->key);
    Push(v, size);
    while(i < size){
      Push(v, tree->key[i]);
      i++;
    }
    int first = (int) tree-> value;
    int second = (int) ( tree-> value >> 32);
    Push(v, second);
    Push(v, first);
  }
  if(tree-> parent == NIL || (v)->size > BUFFER_SIZE){
    (v)-> value[0] = (v)-> size;
    fwrite((v)-> value, sizeof(int), (size_t) (v)->size, file);
    Clear(v);
    Push(v, 0);
  }
}


void RBDelete(RBNode * z){
  RBNode * x;
  RBNode * y;
  int size = 0;
  if(!z || z == NIL)
    return;
  if(z-> left == NIL || z-> right == NIL){
    y = z;
  }
  else{
    y = z-> right;
    while(y-> left != NIL)
        y = y-> left;
  }
  if(y-> left != NIL)
    x = y-> left;
  else
    x = y-> right;
  x-> parent = y-> parent;
  if(y-> parent != NIL)
    if(y == y-> parent-> left)
      y-> parent-> left = x;
    else
      y-> parent-> right = x;
  else
    root = x;
  if (y != z){
    z-> value = y-> value;
    size = (int) strlen(y->key);
    z-> key = (char*)realloc(z-> key, sizeof(char)*(size+1));
    strcpy(z->key, y->key);
    z->key[size] = '\0';
  }

  if(y->color == BLACK)
    RBDeleteFixUp(x);
  printf("OK\n");
  free(y->key);
  free(y);
}

int main(int argc, char **argv) {
  int c;
  char key[S_LIMIT];
  unsigned long long int value;
  int i = 0;
  char command[5];
  Vector * vector;

  while((c = getchar()) != EOF){
    switch(c){
    case '+':{
      getchar();
      c = getchar();
      while(c != ' ' && i < S_LIMIT){
        key[i] = (char) (c >= 'A' && c <= 'Z'
                                    ? c - 'A' + 'a'
                                    : c);
        i++;
        c = getchar();
      }
      key[i] = '\0';
      i = 0;
      if(scanf("%llu", &value) != 1)
        perror("scanf");

      RBNode * node = RBInFind(root, key);
      if(root != NIL && node == NIL){
        printf("Exist\n");
        break;
      }
      if(RBInsert(value, key, node))
        printf("OK\n");
      break;
    }
    case '-':{
      getchar();
      c = getchar();
      while(c != '\n' && i < S_LIMIT){
        key[i] = (char) (c >= 'A' && c <= 'Z'
                                    ? c - 'A' + 'a'
                                    : c);
        i++;
        c = getchar();
      }
      key[i] = '\0';
      i = 0;

      RBNode * deleted = RBFind(root, key);
      if(deleted == NIL){
        printf("NoSuchWord\n");
        break;
      }
      RBDelete(deleted);
      break;
    }
    case '!':{
      getchar();
      c = getchar();
      i = 0;
      while(!isspace(c) && i < 5){
        command[i] = (char) c;
        i++;
        c = getchar();
      }
      command[i] = '\0';
      c = getchar();
      i = 0;
      while(c != '\n'){
        key[i] = (char) c;
        i++;
        c = getchar();
      }
      key[i] = '\0';
      i = 0;

      if(strcmp(command, "Save") == 0){
        FILE * file = fopen(key, "w+");
        if(file == NULL){
          perror("ERROR");
          break;
        }
        vector = CreateVector();
        Push(vector, 0);
        UploadFile(root, file, vector);
        fclose(file);
        DestroyVector(vector);
        printf("OK\n");
        break;
      }
      if(strcmp(command, "Load") == 0){
        FILE * in = fopen(key, "r");
        if(in == NULL){
          perror("ERROR");
        }
        RBDestroy(root);
        root = LoadFile(in);
        fclose(in);
        printf("OK\n");
      }
    }
    break;
    case '\n':
      break;
    default:{
      while(c!= '\n' && i < S_LIMIT){
        key[i] = (char) (c >= 'A' && c <= 'Z'
                                    ? c - 'A' + 'a'
                                    : c);
        i++;
        c = getchar();
      }
      key[i] = '\0';
      i = 0;

      RBNode * found = RBFind(root, key);
      if (found == NIL)
        printf("NoSuchWord\n");
      else
        printf("OK: %llu\n", found->value);
    }
    }
  }
  if(root!= NIL)
    RBDestroy(root);
  return 0;
}
