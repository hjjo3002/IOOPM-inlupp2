#include "datastruct.h"


#define No_buckets 17
typedef struct entry entry_t;

struct hash_table_iterator
{
  ioopm_hash_table_t *ht;
  size_t current_bucket;
  entry_t *current_entry;
};

struct entry
{
  value_t key;     // holds the key
  value_t value;    // holds the value
  entry_t *next; // points to the next entry (possibly NULL)
};

struct hash_table
{
  // DODGE: hard-coding number of buckets as No_buckets.
  // NOTE: addressing this dodge is optional.
  ioopm_eq_function *eq_fn;
  ioopm_hash_function *hash_fn;
  entry_t buckets[No_buckets];
};



typedef struct link link_t;
struct link
{
    value_t value;
    link_t *next;
    link_t *prev;
};

// Double linked list makes the struct more complex but makes most of the list operations easier.
struct list
{
    link_t *first;
    link_t *last;
};


struct list_iterator
{
    ioopm_list_t *list;
    link_t **prev_ptr;
};

//hash table

static void skip_sentinel_nodes(ioopm_hash_table_iterator_t *it);

static entry_t *entry_create(value_t key, value_t value, entry_t *next)
{
  entry_t *entry = calloc(1, sizeof(entry_t));
  entry->key = key;
  entry->value = value;
  entry->next = next;
  return entry;
}

static entry_t *entry_destroy(entry_t *entry)
{
  entry_t *next = entry->next;
  free(entry);
  return next;
}

static void bucket_destroy(entry_t *sentinel)
{
  entry_t *current = sentinel->next;
  while (current != NULL)
  {
    current = entry_destroy(current);
  }
}

ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function *hash_fn, ioopm_eq_function *eq_fn)
{
  ioopm_hash_table_t *ht = calloc(1, sizeof(ioopm_hash_table_t));
  ht->hash_fn = hash_fn;
  ht->eq_fn = eq_fn;
  return ht;
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht)
{
  // DOGE: update as to not only handle ht of length No_buckets
  for (int i = 0; i < No_buckets; i++)
  {
    bucket_destroy(&ht->buckets[i]);
  }
  free(ht);
}

size_t str_hash(const value_t key)
{
  size_t result = 0;
  char *str = (char *) key.p;
  while (*str != '\0')
  {
    result = result * 31 + ((unsigned char)*str);
    str++;
  }
  return result;
}

bool str_comp(const value_t a, const value_t b)
{
  return strcmp((char*)a.p, (char*)b.p) == 0;
}

static entry_t *find_previous_entry(ioopm_hash_table_t *ht, const value_t key)
{
  size_t bucket = ht->hash_fn(key) % No_buckets;

  entry_t *prev = &ht->buckets[bucket];
  entry_t *current = prev->next;

  while (current != NULL && !ht->eq_fn(current->key, key))
  {
    prev = current;
    current = prev->next;
  }
  return prev;
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, const value_t key, value_t value)
{
  // find previous entry, or the last entry if the key does not exist
  entry_t *previous = find_previous_entry(ht, key);

  // if the key exists, update the value, otherwise create a new entry
  if (previous->next != NULL)
  {
    previous->next->value = value;
  }
  else
  {
    previous->next = entry_create(key, value, NULL);
  }
}

bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, const value_t key, value_t *result)
{
  entry_t *prev = find_previous_entry(ht, key);

  if (prev->next != NULL)
  {
    *result = prev->next->value;
    return true;
  }
  else
  {
    return false;
  }
}

bool ioopm_hash_table_remove(ioopm_hash_table_t *ht, const value_t key, value_t *result)
{
  entry_t *prev = find_previous_entry(ht, key);
  if (prev->next == NULL)
  {
    return false;
  }
  else
  {
    *result = prev->next->value;
    prev->next = entry_destroy(prev->next);
    return true;
  }
}

bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, const value_t key)
{
  entry_t *prev = find_previous_entry(ht, key);
  if (prev->next == NULL)
  {
    return false;
  }
  else
  {
    return true;
  }
}

bool ioopm_hash_table_is_empty(const ioopm_hash_table_t *ht)
{

  for (int i = 0; i < No_buckets; i++)
  {
    if (ht->buckets[i].next != NULL)
    {
      return false;
    }
  }

  return true;
}

static size_t bucket_size(entry_t *entry)
{
  size_t size = 0;

  while (entry != NULL)
  {
    size++;
    entry = entry->next;
  }

  return size;
}

size_t ioopm_hash_table_size(const ioopm_hash_table_t *ht)
{
  size_t size = 0;
  for (int i = 0; i < No_buckets; i++)
  {
    size += bucket_size(ht->buckets[i].next);
  }
  return size;
}

// hash table Iter:

ioopm_hash_table_iterator_t *ioopm_hash_table_iterator_create(ioopm_hash_table_t *ht)
{
  ioopm_hash_table_iterator_t *it = malloc(sizeof(ioopm_hash_table_iterator_t));
  it->ht = ht;
  it->current_bucket = 0;
  it->current_entry = &ht->buckets[0];
  skip_sentinel_nodes(it);
  return it;
}

static void advance_iterator_state(ioopm_hash_table_iterator_t *it)
{
  // advance to the next entry in the bucket
  it->current_entry = it->current_entry->next;

  // if it was null advance to the next bucket
  if (it->current_entry == NULL)
  {
    it->current_bucket += 1;

    // if the next bucket existed, update the current entry
    if (it->current_bucket != No_buckets)
    {
      it->current_entry = &it->ht->buckets[it->current_bucket];
    }
  }
}

static void skip_sentinel_nodes(ioopm_hash_table_iterator_t *it)
{
  while (it->current_bucket != No_buckets &&
         it->current_entry == &it->ht->buckets[it->current_bucket])
  {
    advance_iterator_state(it); // Cheat!
  }
}

bool ioopm_hash_table_iterator_at_end(ioopm_hash_table_iterator_t *it)
{
  return it->current_bucket >= No_buckets;
}

void ioopm_hash_table_iterator_destroy(ioopm_hash_table_iterator_t *it)
{
  free(it);
}

void ioopm_hash_table_iterator_advance(ioopm_hash_table_iterator_t *it)
{
  advance_iterator_state(it);
  skip_sentinel_nodes(it);
}

value_t ioopm_hash_table_iterator_current_key(ioopm_hash_table_iterator_t *it)
{
  return it->current_entry->key;
}

value_t ioopm_hash_table_iterator_current_value(ioopm_hash_table_iterator_t *it)
{
  return it->current_entry->value;
}

//linked list

ioopm_list_t *ioopm_list_create(void)
{
    ioopm_list_t *list = calloc(1, sizeof(ioopm_list_t));
    list->first = NULL;
    list->last = NULL;
    return list;
}

static link_t *link_destroy(link_t *link)
{
    link_t *next = link->next;
    free(link);
    return next;
}
// Is longer due to the list being double linked
static value_t unlink(ioopm_list_t *list, link_t *link)
{
    if (link->prev)
    {
        link->prev->next = link->next;
    }
    else
    {
        list->first = link->next;
    }

    if (link->next)
    {
        link->next->prev = link->prev;
    }
    else
    {
        list->last = link->prev;
    }

    value_t res = link->value;
    free(link);
    return res;
}

void ioopm_list_destroy(ioopm_list_t *list)
{
    link_t *link = list->first;
    while (link)
    {
        link = link_destroy(link);
    }
    free(list);
}

static link_t *link_create(link_t *next, link_t *prev, value_t value)
{
    link_t *link = calloc(1, sizeof(link_t));
    link->next = next;
    link->prev = prev;
    link->value = value;
    return link;
}

// Is longer due to the list being double linked
static link_t *link_add(ioopm_list_t *list, link_t *next, link_t *prev, value_t value)
{
    link_t *new = link_create(next, prev, value);

    if (prev)
    {
        prev->next = new;
    }
    else
    {
        list->first = new;
    }
    if (next)
    {
        next->prev = new;
    }
    else
    {
        list->last = new;
    }
    return new;
}

void ioopm_list_append(ioopm_list_t *list, value_t value)
{
    link_add(list, NULL, list->last, value);
}

void ioopm_list_prepend(ioopm_list_t *list, value_t value)
{
    link_add(list, list->first, NULL, value);
}

value_t ioopm_list_head(ioopm_list_t *list)
{
    return list->first->value;
}

value_t ioopm_list_last(ioopm_list_t *list)
{
    return list->last->value;
}

//There is always a preivous pointer even if there are no links
static link_t **list_find_previous_ptr(ioopm_list_t *list, size_t index)
{
    link_t **ptr = &list->first;

    for (size_t i = 0; i < index; i++)
    {
        assert(*ptr && "Index out of bounds");
        ptr = &(*ptr)->next;
    }

    return ptr;
}

void ioopm_list_insert(ioopm_list_t *list, size_t index, value_t value)
{
    link_t **prev = list_find_previous_ptr(list, index);

    if (*prev)
    {
        link_add(list, *prev, (*prev)->prev, value);
    }
    else
    {
        link_add(list, NULL, list->last, value);
    }
}

value_t ioopm_list_remove(ioopm_list_t *list, size_t index)
{
    assert(!ioopm_list_is_empty(list) && "tried to remove from empty list");

    link_t **prev = list_find_previous_ptr(list, index);
    return unlink(list, *prev);
}

value_t ioopm_list_get(ioopm_list_t *list, size_t index)
{
    link_t **prev = list_find_previous_ptr(list, index);

    assert(*prev && "Index out of bounds");

    return (*prev)->value;
}

size_t ioopm_list_size(ioopm_list_t *list)
{
    size_t size = 0;
    link_t *current = list->first;
    while (current)
    {
        current = current->next;
        size++;
    }
    return size;
}

bool ioopm_list_is_empty(ioopm_list_t *list)
{
    return !list->first;
}

//linked list iter

ioopm_list_iterator_t *ioopm_list_iterator_create(ioopm_list_t *l)
{
    ioopm_list_iterator_t *it = calloc(1, sizeof(ioopm_list_iterator_t));
    it->prev_ptr = &l->first;
    it->list = l;
    return it;
}

void ioopm_list_iterator_destroy(ioopm_list_iterator_t *iter)
{
    free(iter);
}

bool ioopm_list_iterator_at_end(ioopm_list_iterator_t *iter)
{
    return !*iter->prev_ptr;
}

void ioopm_list_iterator_advance(ioopm_list_iterator_t *iter)
{
    if (!ioopm_list_iterator_at_end(iter))
    {
        iter->prev_ptr = &(*iter->prev_ptr)->next;
    }
}

value_t ioopm_list_iterator_current(ioopm_list_iterator_t *iter)
{
    return (*iter->prev_ptr)->value;
}

value_t ioopm_list_iterator_remove(ioopm_list_iterator_t *iter)
{
    link_t *to_remove = *iter->prev_ptr;
    return unlink(iter->list, to_remove);
}

void ioopm_list_iterator_insert(ioopm_list_iterator_t *iter, value_t value)
{
    if (ioopm_list_iterator_at_end(iter))
    {
        link_add(iter->list, NULL, iter->list->last, value);
    }
    else
    {
        link_t *current = *iter->prev_ptr;

        link_t *new = link_add(iter->list, current, current->prev, value);

        iter->prev_ptr = &new->next;
    }
}
