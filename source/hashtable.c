/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   hashtable.c
    @brief  Implementation of a hash table, i.e. a constant access time associative array
    (C) 2011 Jack Holland. All rights reserved.
*/

#include <stdlib.h>
#include <string.h>

#include "hashtable.h"
#include "constants.h"
#include "structs.h"
#include "strings.h"

static void deleteHash_(hashtable*, int);

static int insertFlaggedHash(hashtable*, char*, void*, int);

/*! Creates a new hash table with the given size
    @param size     the maximum number of elements allowed in the table
    @return         the new hash table
*/
hashtable* newHashtable (int size) {
    if (size < 1) { // if the given size is invalid
        return NULL; // don't make the table
    }
    hashtable* table = allocate(sizeof(hashtable));  // attempt to allocate the needed memory for the table
    table->table = allocate(sizeof(hashelement*) * size); // attempt to allocate the needed memory for the table elements
    int i;
    for (i = 0; i < size; ++i) { // initialize each element to null
        table->table[i] = NULL;
    }
    table->size = size; // set the table's size
    return table;
}

/*! Creates a hash element list structure, which contains a list of void pointers
    @param value    the unspecified (void pointer) value to be stored
    @return         the new general expression list structure
*/
hashlist* newHashlist (void* value, int flag) {
    hashlist* hl = allocate(sizeof(hashlist));
    hl->value = value;
    hl->flag = flag;
    hl->next = NULL;
    return hl;
}

/*! Generates a hashsum for the given table and key
    @param table    the pointer to the appropriate hash table
    @param key      the string key used to generate the hashsum
    @return         the generated hashsum
*/
inline hashsum hash (hashtable* table, char* key) {
    return hashWithSize(table->size, key);
}

/*! Generates a hashsum for the given size and key
    @param size    the pointer to the appropriate hash table
    @param key      the string key used to generate the hashsum
    @return         the generated hashsum
*/
hashsum hashWithSize (uint size, char* key) {
    hashsum hs = 0; // initialize the hashsum
    for (; *key != '\0'; ++key) { // for each character in the given string key
        hs = *key + (hs << 5) - hs; // generate the hashsum
    }
    return hs % size; // resize the hashsum according to the given size
}

/*! Returns the element in the given hash table corresponding to the given key
    @param table    the pointer to the appropriate hash table
    @param key      the string key used to generate the hashsum
    @return         the corresponding element as a void pointer
*/
void* lookupHash (hashtable* table, char* key) {
    hashsum hs = hash(table, key); // look up the hashsum for the given string key
    hashelement* list;
    for (list = table->table[hs]; list != NULL; list = list->next) { // for each element at this hashsum's index
        if (strcmp(key, list->key) == 0) { // if the right element is found, return it
            return list->value;
        }
    }
    return NULL; // if the element wasn't found, return null
}

/*! Returns all of the elements in the given hash table corresponding to the given key
    @param table    the pointer to the appropriate hash table
    @param key      the string key used to generate the hashsum
    @return         the corresponding elements as a hash element list
*/
hashlist* lookupHashes (hashtable* table, char* key) {
    hashsum hs = hash(table, key); // look up the hashsum for the given string key
    hashlist* newlist = NULL; // the new list which copies the values from the valid table elements
    hashlist* citem;
    hashelement* list;
    for (list = table->table[hs]; list != NULL; list = list->next) { // for each element at this hashsum's index
        if (strcmp(key, list->key) == 0) { // if the key matches the element's key
            if (newlist == NULL) { // if this is the first valid element
                newlist = newHashlist(list->value, list->flag); // create a new general expression list
                citem = newlist;
            } else { // if this isn't the first valid element
                citem->next = newHashlist(list->value, list->flag); // create a new general expression list
                citem = citem->next;
            }
        }
    }
    return newlist;
}

/*! Returns the element list in the given hash table corresponding to the given key
    @param table    the pointer to the appropriate hash table
    @param key      the string key used to generate the hashsum
    @return         the corresponding element list
*/
inline hashelement* lookupHashList (hashtable* table, char* key) {
    return table->table[hash(table, key)];
}

/*! Inserts an element into the given hash table using the given key and marking it as a primitive function
    @param table    the pointer to the appropriate hash table
    @param key      the string key used to generate the hashsum
    @param value    the value to be inserted into the table
    @return         if the key is the first item at its hashed index then return 1, otherwise return 0
*/
inline int insertPrimHash (hashtable* table, char* key, void* value) {
    return insertFlaggedHash(table, key, value, HFLAG_PRIM);
}

/*! Inserts an element into the given hash table using the given key and marking it as a user variable
    @param table    the pointer to the appropriate hash table
    @param key      the string key used to generate the hashsum
    @param value    the value to be inserted into the table
    @return         if the key is the first item at its hashed index then return 1, otherwise return 0
*/
inline int insertUserHash (hashtable* table, char* key, void* value) {
    return insertFlaggedHash(table, key, value, HFLAG_USER);
}

/*! Inserts an element into the given hash table using the given key and marking it as a user variable
    @param table    the pointer to the appropriate hash table
    @param key      the string key used to generate the hashsum
    @param value    the value to be inserted into the table
    @return         if the key is the first item at its hashed index then return 1, otherwise return 0
*/
inline int insertDirectHash (hashtable* table, char* key, void* value) {
    return insertFlaggedHash(table, key, value, HFLAG_DIRECT);
}

/*! Inserts an element into the given hash table using the given key and marking it as a user variable
    @param table    the pointer to the appropriate hash table
    @param key      the string key used to generate the hashsum
    @param value    the value to link with the given key
    @return         if the key is the first item at its hashed index then return 1, otherwise return 0
*/
inline int insertHash (hashtable* table, char* key, void* value) {
    return insertFlaggedHash(table, key, value, 0);
}

/*! Inserts an element into the given hash table using the given key
    @param table    the pointer to the appropriate hash table
    @param key      the string key used to generate the hashsum
    @param value    the value to link with the given key
    @param flag     a flag (0 or 1) indicating what type of hash element is being added (usually primitive vs. user)
    @return         if the key is the first item at its hashed index then return 1, otherwise return 0
*/
static int insertFlaggedHash (hashtable* table, char* key, void* value, int flag) {
    int first;
    hashelement* list;
    hashsum hs = hash(table, key); // look up the hashsum for the given string key
    list = lookupHashList(table, key); // get the list of elements with the calculated hashsum
    first = (list == NULL); // if the new element is the first with its hashsum return 1 at the end, otherwise return 0
    list = allocate(sizeof(hashelement)); // allocate the memory needed for a new list item
    list->key = strDup(key); // assign a copy of the key string to the list item
    list->value = value; // assign the element's vaule to the list item
    list->flag = flag; // mark the hash element with the given flag
    list->next = table->table[hs]; // attach the existing list (or null if no list yet exists) to this new list item
    table->table[hs] = list; // set this list item as the first list item for its hashsum
    return first; // return whether or not this is the only element with its hashsum
}

/*! Resets each of the given hash table's elements without removing the table itself from memory
    @param table    the pointer to the appropriate hash table
    @return         nothing
*/
inline void clearHash (hashtable* table) {
    deleteHash_(table, 0); // remove the table's elements from memory
}

/*! Removes the given hash table from memory
    @param table    the pointer to the appropriate hash table
    @return         nothing
*/
inline void deleteHash (hashtable* table) {
    deleteHash_(table, 1); // remove the table and its elements from memory
}

/*! Removes the given hash table and/or its elements from memory (should be called indirectly through either clearHash or deleteHash)
    @param table        the pointer to the appropriate hash table
    @param deletetable  if 1 then delete the table as well as its elements, otherwise just delete its elements
    @return             nothing
*/
static void deleteHash_ (hashtable* table, int deletetable) {
    if (table == NULL) { // if the table doesn't exist, nothing needs to be done
        return;
    }
    int i;
    hashelement* list;
    hashelement* temp;
    for (i = 0; i < table->size; ++i) { // for each element in the table
        list = table->table[i]; // get the list at this index
        while (list != NULL) { // while there are more list items
            temp = list; // store the item temporarily
            list = list->next; // advance to the next item
            free(temp->key); // free the memory of the string key
            if (temp->flag == HFLAG_PRIM) { // if the element is a primitive function then free it
                typelist* types = ((tap_prim_fun*)temp->value)->types;
                while (types != NULL) {
                    typelist* tempat = types->next;
                    free(types);
                    types = tempat;
                }
                free(temp->value);
            } else if (temp->flag == HFLAG_USER) { // if the element is a user expression then free it
                freeExpr(temp->value);
            }
            free(temp);
        }
        if (table->table[i] != NULL) {
            table->table[i] = NULL; // indicate there are no onger elements at this index
        }
    }
    if (deletetable) { // if the table should also be deleted
        free(table->table); // free the memory of the table itself
        free(table); // free the memory of the table structure
    }
}

