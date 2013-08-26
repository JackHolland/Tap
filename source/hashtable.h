/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   hashtable.h
    @brief  The header file for hashtable.c
    (C) 2011 Jack Holland. All rights reserved.
*/

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "memory.h"
#include "structs.h"
#include "typedefs.h"

typedef uint hashsum;

typedef struct hashelement_ hashelement;
typedef struct hashtable_ hashtable;
typedef struct hashlist_ hashlist;

struct hashelement_ {
    char* key;
    void* value;
    uint flag:2;
    struct hashelement_* next;
};

struct hashtable_ {
    int size;
    hashelement** table;
};

struct hashlist_ {
    void* value;
    uint flag:1;
    hashlist* next;
};

hashtable* newHashtable(int);
hashlist* newHashlist(void*, int);
hashsum hash(hashtable*, char*);
hashsum hashWithSize(uint, char*);
void* lookupHash(hashtable*, char*);
hashlist* lookupHashes(hashtable*, char*);
hashelement* lookupHashList(hashtable*, char*);
int insertPrimHash(hashtable*, char*, void*);
int insertUserHash(hashtable*, char*, void*);
int insertDirectHash(hashtable*, char*, void*);
int insertHash(hashtable*, char*, void*);
void clearHash(hashtable*);
void deleteHash(hashtable*);

#endif

