#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

struct entry_s {
  char *key;
  char *value;
  struct entry_s *next;
};

typedef struct entry_s entry_t;

struct hashtable_s {
  int size;
  struct entry_s **table;
};

typedef struct hashtable_s hashtable_t;

hashtable_t *ht_create(int size) {
  hashtable_t* new;
  int i;

  if ((new = malloc(sizeof(hashtable_t))) == NULL)
    return NULL;

  if ((new->table = malloc(size * sizeof(entry_t *))) == NULL)
    return NULL;

  for (i=0; i<size; i++)
    new->table[i] = NULL;
  new->size = size;

  return new;
}

int ht_hash(hashtable_t *hashtable, char *key) {

  unsigned long int hashval = 0;
  int i = 0, length = strlen(key);

  while ( hashval < ULONG_MAX && i < length ) {
    hashval <<= 8;
    hashval += key[i++];
  }

  return hashval % hashtable->size;
}

entry_t *ht_newpair(char *key, char *value) {
  entry_t *new;

  if ( (new = malloc(sizeof(entry_t))) == NULL)
    return NULL;

  if ( (new->key = strdup(key)) == NULL)
    return NULL;

  if ( (new->value = strdup(value)) == NULL)
    return NULL;

  new->next = NULL;
  return new;
}

void ht_set(hashtable_t *hashtable, char *key, char *value) {
  int index = 0;
  entry_t *new = NULL;
  entry_t *current = NULL;
  entry_t *last = NULL;

  index = ht_hash(hashtable, key);
  current = hashtable->table[index];

  while (current != NULL && current->key != NULL && strcmp(key, current->key) > 0) {
    last = current;
    current = current->next;
  }

  if (current != NULL && current->key != NULL && strcmp(key, current->key) == 0) {
    /* key value is already existing and we are replacing it */
    free(current->value);
    current->value = strdup(value);
  } else {
    new = ht_newpair(key, value);

    if (current == hashtable->table[index]) {
      /* beginning of ordered list */
      hashtable->table[index] = new;
      new->next = current;
    } else {
      last->next = new;
      new->next = current;
    }
  }
}

char *ht_get(hashtable_t *hashtable, char *key) {
  int index = 0;
  entry_t* current;

  index = ht_hash(hashtable, key);
  current = hashtable->table[index];

  while (current != NULL && current->key != NULL && strcmp(key, current->key) != 0) {
    current = current->next;
  }

  if (current != NULL && current->key != NULL) {
    return current->value;
  }

  return NULL;
}

void ht_getp(hashtable_t *hashtable, char *key) {
  char *value;

  value = ht_get(hashtable, key);
  if (value) {
    printf("%s: %s\n", key, value);
  } else {
    printf("Key '%s' not found\n", key);
  }
}

int main(int argc, char *argv[]) {
  hashtable_t *hash;
  int value;

  hash = ht_create(65535);

  value = ht_hash(hash, "aZa");
  printf("value is %d\n", value);
  value = ht_hash(hash, "ZZh");
  printf("value is %d\n", value);

  ht_set(hash, "spam", "egg");
  ht_set(hash, "spammer", "sausage");
  ht_set(hash, "baked", "beans");
  ht_set(hash, "aZa", "unladen");
  ht_set(hash, "ZZh", "sparrow");

  ht_getp(hash, "spam");
  ht_getp(hash, "spammer");
  ht_getp(hash, "baked");
  ht_getp(hash, "fromage");
  ht_getp(hash, "aZa");
  ht_getp(hash, "ZZh");

  return EXIT_SUCCESS;
}
