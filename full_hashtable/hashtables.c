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
LinkedPair *create_pair(char *key, char *value) {
  LinkedPair *pair = malloc(sizeof(LinkedPair));
  pair->key = strdup(key);
  pair->value = strdup(value);
  pair->next = NULL;

  return pair;
}

/****
  Use this function to safely destroy a hashtable pair.
 ****/
void destroy_pair(LinkedPair *pair) {
  if (pair != NULL) {
    free(pair->key);
    free(pair->value);
    free(pair);
  }
}

/****
  djb2 hash function

  Do not modify this!
 ****/
unsigned int hash(char *str, int max) {
  unsigned long hash = 5381;
  int c;
  unsigned char *u_str = (unsigned char *)str;

  while ((c = *u_str++)) {
    hash = ((hash << 5) + hash) + c;
  }

  return hash % max;
}

/****
  Fill this in.

  All values in storage should be initialized to NULL
 ****/
HashTable *create_hash_table(int capacity) {
  HashTable *ht = malloc(sizeof(HashTable));
  ht->capacity = capacity;
  ht->storage = calloc(capacity, sizeof(LinkedPair *));
  return ht;
}

/****
  Fill this in.

  Inserting values to the same index with different keys should be
  added to the corresponding LinkedPair list.

  Inserting values to the same index with existing keys can overwrite
  the value in th existing LinkedPair list.
 ****/
void hash_table_insert(HashTable *ht, char *key, char *value) {
  unsigned long int index = hash(key, ht->capacity);
  LinkedPair *new_pair = create_pair(key, value);
  if (ht->storage[index] == NULL) {
    ht->storage[index] = new_pair;
    printf("Inserting for first time: {key: %s, value: %s, index: %lu}\n",
           ht->storage[index]->key, ht->storage[index]->value, index);
    return;
  } else {
    LinkedPair *current = ht->storage[index];
    LinkedPair *child = ht->storage[index]->next;
    if (strcmp(ht->storage[index]->key, key) == 0) {

      destroy_pair(ht->storage[index]);
      ht->storage[index] = new_pair;
      ht->storage[index]->next = child;
      printf("Insrt in first equivalent: {Key: %s, value: %s, index: %lu}\n",
             ht->storage[index]->key, ht->storage[index]->value, index);
      return;
    }
    while (strcmp(current->key, key) != 0 && child != NULL) {
      current = child;
      child = current->next;
      if (current->next == NULL) {
        current->next = new_pair;
        printf(
            "Inserting into immediate next: {key: %s, value: %s, index: %lu}\n",
            current->next->key, current->next->value, index);
        return;
      }
      if (strcmp(current->key, key) == 0) {
        destroy_pair(current);
        current = new_pair;
        current->next = child;
        printf("Inserting after destroy: {key: %s, value: %s, index: %lu}\n",
               current->key, current->value, index);
        return;
      }
    }
  }
}
/****
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys and remove matching LinkedPairs safely.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(HashTable *ht, char *key) {
  unsigned long int index = hash(key, ht->capacity);

  // empty slot
  if (ht->storage[index] == NULL) {
    return;
  }

  // found at start of slot
  if (strcmp(ht->storage[index]->key, key) == 0) {
    LinkedPair *child = ht->storage[index]->next;
    destroy_pair(ht->storage[index]);
    ht->storage[index] = child;
    return;
  }

  // traverse linked list
  LinkedPair *current = ht->storage[index];
  LinkedPair *child = current->next;
  while (current->next != NULL) {
    current = child;
    child = current->next;
    if (strcmp(current->key, key) == 0) {
      destroy_pair(current);
      current = child;
      return;
    }
  }

  // if (strcmp(current->key, key) != 0 && ht->storage[index]->next) {
  //   while (current->next != NULL) {
  //     current = current->next;
  //     LinkedPair *child = current->next;
  //     if (strcmp(key, child->key) == 0) {
  //       current->next = child->next;
  //       destroy_pair(child);
  //     }
  //   }
  // }
}

/****
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys.

  Return NULL if the key is not found.
 ****/
char *hash_table_retrieve(HashTable *ht, char *key) {
  unsigned int index = hash(key, ht->capacity);
  if (ht->storage[index] == NULL) {
    return NULL;
  }
  if (ht->storage[index]) {
    printf("Actual value at index of hash: %s\n", ht->storage[index]->value);
    if (strcmp(ht->storage[index]->key, key) == 0) {
      printf("Retrieved value: %s\n", ht->storage[index]->value);
      return ht->storage[index]->value;
    } else {
      LinkedPair *current = ht->storage[index];
      while (current->next != NULL) {
        current = current->next;
        if (strcmp(current->key, key) == 0) {
          printf("Retrieved value in next: %s\n", current->value);
          return current->value;
        }
      }
    }
  }
  return NULL;
}

// /****
//   Fill this in.

//   Don't forget to free any malloc'ed memory!
//  ****/
void destroy_hash_table(HashTable *ht) {
  for (int i = 0; i < ht->capacity; i++) {
    LinkedPair *current = ht->storage[i];
    LinkedPair *next = current->next;
    while (current->next != NULL) {
      destroy_pair(current);
      current = next;
    }
    if (current != NULL) {
      destroy_pair(current);
      current = NULL;
    }
    free(ht->storage);
    free(ht);
    printf("Destroy hash table\n");
  }
}

/****
  Fill this in.

  Should create a new hash table with double the capacity
  of the original and copy all elements into the new hash table.

  Don't forget to free any malloc'ed memory!
 ****/
HashTable *hash_table_resize(HashTable *ht) {
  int oldCapacity = ht->capacity;
  int newCapacity = ht->capacity * 2;
  HashTable *new_ht = create_hash_table(newCapacity);
  for (int i = 0; i < oldCapacity; i++) {
    if (ht->storage && ht->storage[i]) {
      hash_table_insert(new_ht, ht->storage[i]->key, ht->storage[i]->value);
    }
  }
  destroy_hash_table(ht);
  return new_ht;
}

#ifndef TESTING
int main(void) {
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
