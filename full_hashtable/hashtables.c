#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/****
  Hash table key/value pair with linked list pointer
 ****/
typedef struct LinkedPair {
  char *key;
  char *value;
  struct LinkedPair *next;
} LinkedPair;

/****
  Hash table with linked pairs
 ****/
typedef struct HashTable {
  int capacity;
  LinkedPair **storage;
} HashTable;

/****
  Create a key/value linked pair to be stored in the hash table.
 ****/
LinkedPair *create_pair(char *key, char *value)
{
  LinkedPair *pair = malloc(sizeof(LinkedPair));
  pair->key = strdup(key);
  pair->value = strdup(value);
  pair->next = NULL;

  return pair;
}

/****
  Use this function to safely destroy a hashtable pair.
 ****/
void destroy_pair(LinkedPair *pair)
{
  if (pair) {    
    free(pair->key);
    pair->key = NULL;
    free(pair->value);
    pair->value = NULL;
    if (pair->next) {
      while (pair->next) {
        LinkedPair *next_node = pair->next;
        destroy_pair(pair);
        pair = next_node;
      }
    }
  }
  free(pair);
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
 ****/
HashTable *create_hash_table(int capacity)
{
  HashTable *ht = malloc(sizeof(HashTable));
  ht->capacity = capacity;
  ht->storage = calloc(capacity, sizeof(LinkedPair*));
  return ht;
}

/****
  Fill this in.

  Inserting values to the same index with different keys should be
  added to the corresponding LinkedPair list.

  Inserting values to the same index with existing keys can overwrite
  the value in th existing LinkedPair list.
 ****/
void hash_table_insert(HashTable *ht, char *key, char *value)
{
  unsigned int index = hash(key, ht->capacity);
  if (ht && ht->storage && ht->storage[index] && ht->storage[index]->key) {
    if (strcmp(ht->storage[index]->key, key) != 0 && ht->storage[index]->next) {
      LinkedPair *search = ht->storage[index];
        while (search->next) {
          if (strcmp(search->key, key) == 0) {
              LinkedPair *new_pair = create_pair(key, value);
              destroy_pair(search);
              search = new_pair;
            } else {
              search = search->next;
          }
        }
        search->next = create_pair(key, value);
    } else if (strcmp(ht->storage[index]->key, key) != 0 && ht->storage[index]->next == NULL) {
      LinkedPair *new_pair = create_pair(key, value);
      ht->storage[index]->next = new_pair;
    }
      if (strcmp(ht->storage[index]->key, key) == 0) {
        LinkedPair *new_pair = create_pair(key, value);
        destroy_pair(ht->storage[index]);
        ht->storage[index] = new_pair;
      } 
     
  }
  LinkedPair *new_pair = create_pair(key, value); 
  ht->storage[index] = new_pair;
}

/****
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys and remove matching LinkedPairs safely.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(HashTable *ht, char *key)
{
  unsigned int index = hash(key, ht->capacity);
  LinkedPair *stored_pair = ht->storage[index];

  if (ht && ht->storage && stored_pair && stored_pair->key) {
    if (strcmp(key, stored_pair->key) != 0 && stored_pair->next) {
      while (stored_pair->next) {
        stored_pair = stored_pair->next;
        if (strcmp(stored_pair->key, key) == 0) {
          destroy_pair(stored_pair);
          stored_pair = NULL;
        }
      } 
    }
    else if (strcmp(key, stored_pair->key) == 0) {
      destroy_pair(stored_pair);
      ht->storage[index] = NULL;
    }
  } else {
    printf("Key not found!\n");
  }
}

/****
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys.

  Return NULL if the key is not found.
 ****/
char *hash_table_retrieve(HashTable *ht, char *key)
{
  unsigned int index = hash(key, ht->capacity);
  LinkedPair *stored_pair = ht->storage[index];
  if (ht && ht->storage && ht->storage[index] &&  ht->storage[index]->key) {
    if (strcmp(ht->storage[index]->key, key) != 0 && ht->storage[index]->next) {
        LinkedPair *search = ht->storage[index];
        while (search->next) {
          search = search->next;
          if (strcmp(key, search->key) == 0) {
            return search->value;
          }
        }
      }
  
    if (strcmp(key, stored_pair->key) == 0) {
          return stored_pair->value;
      }
  }
  return NULL;
}
// /****
//   Fill this in.

//   Don't forget to free any malloc'ed memory!
//  ****/
void destroy_hash_table(HashTable *ht)
{
  for (int i = 0; i < ht->capacity; i++) {
      destroy_pair(ht->storage[i]);
      ht->storage[i] = NULL;
    }
  if (ht->storage) {
    free(ht->storage);
    ht->storage = NULL;
  }
    free(ht);
}

/****
  Fill this in.

  Should create a new hash table with double the capacity
  of the original and copy all elements into the new hash table.

  Don't forget to free any malloc'ed memory!
 ****/
HashTable *hash_table_resize(HashTable *ht) {
  HashTable *new_ht = create_hash_table(ht->capacity * 2);

  for (int i = 0; i < ht->capacity; i++) { 
    while (ht && ht->storage && ht->storage[i]) {
      hash_table_insert(new_ht, ht->storage[i]->key, ht->storage[i]->value);
      if (ht->storage[i]->next) {
        while (ht->storage[i]->next) {
          hash_table_insert(new_ht, ht->storage[i]->next->key, ht->storage[i]->next->value);
        }
      }
    }
  }
  free(ht->storage);
  ht->storage = NULL;
  free(ht);
  return new_ht;
}


#ifndef TESTING
int main(void)
{
  struct HashTable *ht = create_hash_table(2);

  hash_table_insert(ht, "line_1", "Tiny hash table\n");
  hash_table_insert(ht, "line_2", "Filled beyond capacity\n");
  hash_table_insert(ht, "line_3", "Linked list saves the day!\n");

  printf("%s", hash_table_retrieve(ht, "line_1"));
  printf("%s", hash_table_retrieve(ht, "line_2"));
  printf("%s", hash_table_retrieve(ht, "line_3"));

  int old_capacity = ht->capacity;
  ht = hash_table_resize(ht);
  int new_capacity = ht->capacity;

  printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

  destroy_hash_table(ht);

  return 0;
}
#endif
