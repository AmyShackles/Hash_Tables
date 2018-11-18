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
  // Why didn't the example calloc?
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

/*     Options for insertion:
       1) Situation: No element is using given index
          Solution:  ht->storage[index] = new_pair
       2) Situation: Element is using given index
                   : Key does not match insertion key
                   : Next equals null
           Solution: ht->storage[index]->next = new_pair
       3) Situation: Element is using given index
                   : Key does not match insertion key
                   : Next does not equal null
           Solution: Iterate through ht->storage[index]->next
                   : Until either: key matches
                         Solution: destroy_pair of current spot in storage
                               or: key doesn't match, next is null
                         Solution: current_index->next = new_pair
       4) Situation: Element is using given index
                   : Key equals insertion key
           Solution: Check to see if current_index->next is null
                   : If null
                         Solution: destroy_pair(current_index)
                                 : current_index = new_pair
                   : If not null
                         Solution: new_pointer = current_index->next
                                 : destroy_pair(current_index)
                                 : current_index = new_pair
                                 : current_index->next = new_pointer


*/
void hash_table_insert(HashTable *ht, char *key, char *value) {
  unsigned int index = hash(key, ht->capacity);
  LinkedPair *new_pair = create_pair(key, value);
  if (ht->storage[index] == NULL) {
    ht->storage[index] = new_pair;
    printf("Inserted for first time: {Key: %s, value: %s, index: %u}\n",
           ht->storage[index]->key, ht->storage[index]->value, index);
    return;
  }
  if (strcmp(ht->storage[index]->key, key) == 0) {
    if (ht->storage[index]->next == NULL) {
      destroy_pair(ht->storage[index]);
      ht->storage[index] = new_pair;
      printf("Inserted after delete: {Key: %s, value: %s, index: %u}\n",
             ht->storage[index]->key, ht->storage[index]->value, index);
    } else if (ht->storage[index]->next != NULL) {
      LinkedPair *next = ht->storage[index]->next;
      destroy_pair(ht->storage[index]);
      ht->storage[index] = new_pair;
      ht->storage[index]->next = next;
      printf("Inserted after delete, copying next: {Key: %s, value: %s, index: "
             "%u} Next: {Key: %s, value: %s, index: %u}\n",
             ht->storage[index]->key, ht->storage[index]->value, index,
             ht->storage[index]->next->key, ht->storage[index]->next->value,
             index);
    }
  } else if (strcmp(ht->storage[index]->key, key) != 0) {
    if (ht->storage[index]->next == NULL) {
      ht->storage[index]->next = new_pair;
      printf("Insert at next: {Key: %s, value: %s, index: %u}\n",
             ht->storage[index]->next->key, ht->storage[index]->next->value,
             index);
    } else if (ht->storage[index]->next != NULL) {
      LinkedPair *current = ht->storage[index];
      while (current->next != NULL) {
        if (strcmp(current->next->key, key) == 0 &&
            current->next->next != NULL) {
          LinkedPair *child = current->next;
          LinkedPair *grandchild = child->next;
          destroy_pair(child);
          child = new_pair;
          current->next = child;
          child->next = grandchild;
          printf("Insert after delete, connecting A "
                 "to B to C: {Key: %s, value: %s, "
                 "index: %u}\n",
                 child->key, child->value, index);
          return;
        } else if (strcmp(current->next->key, key) == 0 &&
                   current->next->next == NULL) {
          LinkedPair *child = current->next;
          destroy_pair(child);
          child = new_pair;
          current->next = child;
          printf("Insert after delete, connecting A to B: {Key: %s, value: %s, "
                 "index: %u}\n",
                 child->key, child->value, index);
          return;
        }
        current = current->next;
      }
      if (strcmp(current->key, key) == 0) {
        destroy_pair(current);
        current = new_pair;
        printf(
            "Last link deleted and inserted: {Key: %s, value: %s, index: %u}\n",
            current->key, current->value, index);
        return;
      } else if (strcmp(current->key, key) != 0) {
        current->next = new_pair;
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
  unsigned int index = hash(key, ht->capacity);
  if (ht->storage[index] != NULL) {
    if (strcmp(ht->storage[index]->key, key) == 0) {
      if (ht->storage[index]->next != NULL) {
        LinkedPair *next = ht->storage[index]->next;
        destroy_pair(ht->storage[index]);
        ht->storage[index] = next;
      } else if (ht->storage[index]->next == NULL) {
        destroy_pair(ht->storage[index]);
        ht->storage[index] = NULL;
      }
    } else if (strcmp(ht->storage[index]->key, key) != 0) {
      LinkedPair *current = ht->storage[index];
      while (current->next != NULL) {
        if (strcmp(current->next->key, key) == 0 &&
            current->next->next != NULL) {
          LinkedPair *child = current->next;
          LinkedPair *grandchild = child->next;
          destroy_pair(child);
          child = grandchild;
          current->next = child;
          printf("Remove child, connect current->next to child->next: {Key: "
                 "%s, value: %s, "
                 "index: %u}\n",
                 child->key, child->value, index);
        } else if (strcmp(current->next->key, key) == 0 &&
                   current->next->next == NULL) {
          LinkedPair *child = current->next;
          destroy_pair(child);
          current->next = NULL;
          printf("Remove child, turn current->next to null\n");
        }
      }
    }
  }
}

/****
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys.

  Return NULL if the key is not found.
 ****/
/*     Ways in which pairs may exist in hash table:
       1) At exact index
             - return ht->storage[index]
       2) At a next somewhere in index
             - iterate through linked list until key matches
       3) Not in hash table
             - Return null

*/
char *hash_table_retrieve(HashTable *ht, char *key) {
  unsigned int index = hash(key, ht->capacity);
  if (ht->storage[index] != NULL) {
    if (strcmp(ht->storage[index]->key, key) == 0) {
      printf("Retrieve if key matches at index: {Key: %s, value: %s, index: "
             "%u}\n",
             ht->storage[index]->key, ht->storage[index]->value, index);
      return ht->storage[index]->value;
    } else {
      LinkedPair *current = ht->storage[index];
      while (current->next != NULL) {
        if (strcmp(current->next->key, key) == 0) {
          printf("Retrieve if matches in linked list: {Key: %s, value: %s, "
                 "index: %u}\n",
                 current->next->key, current->next->value, index);
          return current->next->value;
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
    if (ht->storage[i] != NULL && ht->storage[i]->next != NULL) {
      LinkedPair *current = ht->storage[i];
      LinkedPair *child = ht->storage[i]->next;
      while (current->next != NULL) {
        destroy_pair(current);
        current = child;
        child = child->next;
      }
    }
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
  int old_capacity = ht->capacity;
  int new_capacity = ht->capacity * 2;
  HashTable *new = create_hash_table(new_capacity);

  for (int i = 0; i < old_capacity; i++) {
    if (ht->storage[i] != NULL) {
      hash_table_insert(new, ht->storage[i]->key, ht->storage[i]->value);
      if (ht->storage[i]->next) {
        LinkedPair *current = ht->storage[i];
        LinkedPair *next = ht->storage[i]->next;
        while (current->next != NULL) {
          current = current->next;
          next = next->next;
          hash_table_insert(new, current->key, current->value);
        }
      }
    }
    destroy_pair(ht->storage[i]);
  }
  free(ht->storage);
  return new;
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
