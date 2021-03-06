#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/****
  Basic hash table key/value pair
 ****/
typedef struct Pair {
  char *key;
  char *value;
} Pair;

/****
  Basic hash table
 ****/
typedef struct BasicHashTable {
  int capacity;
  struct Pair **storage;
} BasicHashTable;

/****
  Create a key/value pair to be stored in the hash table.
 ****/
Pair *create_pair(char *key, char *value)
{
  Pair *pair = malloc(sizeof(Pair));
  pair->key = strdup(key);
  pair->value = strdup(value);

  return pair;
}

/****
  Use this function to safely destroy a hashtable pair.
 ****/
void destroy_pair(Pair *pair)
{
  if (pair != NULL) {
    free(pair->key);
    pair->key = NULL;
    free(pair->value);
    pair->value = NULL;
    free(pair);
  }
}

/****
  djb2 hash function

  Do not modify this!
 ****/
unsigned int hash(char *str, int max)
{
  unsigned long hash = 5381;
  int c;
  unsigned char * u_str = (unsigned char *)str;

  while ((c = *u_str++)) {
    hash = ((hash << 5) + hash) + c;
  }

  return hash % max;
}


/****
  Fill this in.

  All values in storage should be initialized to NULL
  (hint: look up `calloc`)
 ****/
BasicHashTable *create_hash_table(int capacity)
{
  BasicHashTable *ht = malloc(sizeof(BasicHashTable));
  ht->capacity = capacity;  
  printf("Create hash table, capacity is %i\n", capacity);
  ht->storage = calloc(capacity, sizeof(Pair *));
  return ht;
}

/****
  Fill this in.

  If you are overwriting a value with a different key, print a warning.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_insert(BasicHashTable *ht, char *key, char *value)
{
  unsigned int index = hash(key, ht->capacity);

  Pair *stored_pair = ht->storage[index];

  if (stored_pair != NULL) {
    if (strcmp(key, stored_pair->key) != 0) {
      printf("WARNING: Hash Table Collision!!\n");
    }
    destroy_pair(stored_pair);
    ht->storage[index] = NULL;
  }
  ht->storage[index] = create_pair(key, value);

  // for (int i = 0; i < ht->capacity; i++) {
  //   if (ht->storage != NULL && ht->storage[i] != NULL && strcmp(ht->storage[i]->key, key) == 0) {
  //     printf("Keys must be unique");
  //   }
  // }
  // printf("Hash table insert, key is %s, value is %s\n", key, value);
  // Pair *item = create_pair(key, value);
  // int index = hash(key, ht->capacity);
  // printf("Index is %i\n", index);
  // ht->storage[index] = item;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(BasicHashTable *ht, char *key)
{
  unsigned int index = hash(key, ht->capacity);
  Pair *stored_pair = ht->storage[index];

  if (stored_pair != NULL) {
    if (strcmp(key, stored_pair->key) != 0) {
      printf("Key not found!!\n");
    } else if (strcmp(key, stored_pair->key) == 0) {
      destroy_pair(stored_pair);
      ht->storage[index] = NULL;
    }
  } else {
    printf("Key not found!\n");
  // ht->storage[index] = create_pair(key, value);
  // int index = hash(key, ht->capacity);
  // printf("index is %i\n", index);
  //   if (ht->storage != NULL && ht->storage[index] != NULL && strcmp(ht->storage[index]->key, key) == 0) {
  //     printf("Hash table remove, key at index is %s\n", ht->storage[index]->key);
  //     destroy_pair(ht->storage[index]);
  //     ht->storage[index] = NULL;
  // }
  // printf("Hash_table_remove, key is %s", key);
  }
}

/****
  Fill this in.

  Should return NULL if the key is not found.
 ****/
char *hash_table_retrieve(BasicHashTable *ht, char *key)
{
  unsigned int index = hash(key, ht->capacity);
  Pair *stored_pair = ht->storage[index];

  if (stored_pair != NULL) {
    if (strcmp(key, stored_pair->key) != 0) {
      printf("Key not found!!\n");
    } else if (strcmp(key, stored_pair->key) == 0) {
      return stored_pair->value;
    }
  } else {
    return NULL;
  }
  // int index = hash(key, ht->capacity);
  // printf("Index is %i\n", index);
  // if (ht->storage && ht->storage[index] && strcmp(ht->storage[index]->key, key) == 0) {
  //   printf("Hash table retrieve, value is %s\n", ht->storage[index]->value);
  //   return ht->storage[index]->value;
  // } 
  //   return NULL;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(BasicHashTable *ht)
{
  for (int i = 0; i < ht->capacity; i++) {
    destroy_pair(ht->storage[i]);
  }
      if (ht->storage) {
        free(ht->storage);
        ht->storage = NULL;
      }
  free(ht);
  printf("Destroy hash table\n");
}


#ifndef TESTING
int main(void)
{
  struct BasicHashTable *ht = create_hash_table(16);
  
  hash_table_insert(ht, "line", "Here today...\n");
  hash_table_insert(ht, "line", "Wonder what'll happen...\n");
  hash_table_insert(ht, "whaaaaaat", "Whatever\n");
  printf("%s", hash_table_retrieve(ht, "line"));

  hash_table_remove(ht, "line");


  if (hash_table_retrieve(ht, "line") == NULL) {
    printf("...gone tomorrow. (success)\n");
  } else {
    fprintf(stderr, "ERROR: STILL HERE\n");
  }

  destroy_hash_table(ht);

  return 0;
}
#endif
