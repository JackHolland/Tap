/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   prim_str.c
    @brief  All of the primitive functions for strings used by the language
    (C) 2011 Jack Holland. All rights reserved.
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <limits.h>

#include "prim_flo.h"
#include "prim_str.h"
#include "../source/constants.h"
#include "../source/dep_structs.h"
#include "../source/externs.h"
#include "../source/engine.h"
#include "../source/constructors.h"
#include "../source/casting.h"
#include "../source/strings.h"
#include "../source/arrays.h"
#include "../source/dates.h"
#include "../source/hashtable.h"

extern environment* environments[];
extern uint cenvironment;
extern datatype ctypeid;

/*! Maps the given string to the given variable value for the duration of the current scope, returning 1 on success and 0 on failure (str, *)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_sSet (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    addToEnvironment(args[0]->ev.strval->content, args[1]);
    *returntype = TYPE_INT;
    returnval->intval = 1;
}

/*! Defines a new type with the given name and properties (str, laz)->nil
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_sNewtype (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    environment* env = environments[cenvironment];
    char* name = args[0]->ev.strval->content;
    datatype tid = TYPE_UNK;
    if (lookupHash(env->variables, name) == NULL) {
        expression* expr = newExpressionOfType(TYPE_TYP);
        expr->ev.intval = ctypeid;
        insertUserHash(env->variables, name, (void*)expr);
        stringlist* required = NULL;
        typelist* inherits = NULL;
        property* properties = NULL;
        expression* definitions = args[1]->ev.lazval->expval;
        while (definitions != NULL) {
            if (definitions->type == TYPE_EXP) {
                expression* def = copyExpression(definitions->ev.expval);
                if (def->type == TYPE_STR) {
                    char* str = def->ev.strval->content;
                    if (strcmp(str, "required") == 0) {
                        expression* req = def->next;
                        while (req != NULL) {
                            if (req->type == TYPE_STR) {
                                stringlist* sl = newStringlist(req->ev.strval, required);
                                required = sl;
                            } else {
                                ///error
                            }
                            req = req->next;
                        }
                    } else if (strcmp(str, "inherits") == 0) {
                        expression* inh = def->next;
                        while (inh != NULL) {
                            if (inh->type == TYPE_STR) {
                                datatype inhtype = typeFromString(inh->ev.strval->content);
                                if (inhtype == TYPE_UNK) {
                                    ///error
                                } else {
                                    typelist* tempinh = inherits;
                                    inherits->type = inhtype;
                                    inherits->next = tempinh;
                                }
                            } else {
                                ///error
                            }
                            inh = inh->next;
                        }
                    } else if (strcmp(str, "property") == 0) {
                        int privacy = PROP_ATTR_UNK;
                        int range = PROP_ATTR_UNK;
                        expression* next = def->next;
                        if (next == NULL) {
                            ///error
                        } else {
                            expression* propattr = evaluateArgument(next);
                            int i;
                            for (i = 0; i < 2; ++i) {
                                if (propattr->type == TYPE_STR) {
                                    char* attr = propattr->ev.strval->content;
                                    if (strcmp(attr, "public") == 0) {
                                        privacy = PROP_PRIVACY_PUBLIC;
                                    } else if (strcmp(attr, "private") == 0) {
                                        privacy = PROP_PRIVACY_PRIVATE;
                                    } else if (strcmp(attr, "global") == 0) {
                                        range = PROP_RANGE_GLOBAL;
                                    } else if (strcmp(attr, "local") == 0) {
                                        range = PROP_RANGE_LOCAL;
                                    } else {
                                        ///error
                                    }
                                } else {
                                    ///error
                                }
                                if (i == 0) {
                                    next = next->next;
                                    if (next == NULL) {
                                        ///error
                                    } else {
                                        propattr = evaluateArgument(next);
                                    }
                                }
                            }
                            if (privacy == PROP_ATTR_UNK || range == PROP_ATTR_UNK) {
                                ///error
                            } else {
                                next = next->next;
                                if (next == NULL) {
                                    ///error
                                } else {
                                    expression* proptypearr = evaluateArgument(next);
                                    if (proptypearr->type == TYPE_ARR) {
                                        typelist* proptypes = NULL;
                                        array* arr = proptypearr->ev.arrval;
                                        for (i = arr->start; i < arr->end; ++i) {
                                            expression* proptyp = evaluateArgument(arr->content[i]);
                                            if (proptyp->type == TYPE_TYP) {
                                                typelist* tl = newTypelist_n(proptyp->ev.intval, proptypes);
                                                proptypes = tl;
                                                freeExpr(proptyp);
                                            } else {
                                                ///error
                                            }
                                        }
                                        next = next->next;
                                        if (next == NULL) {
                                            ///error
                                        } else {
                                            expression* propname = evaluateArgument(proptypearr->next);
                                            if (propname->type == TYPE_STR) {
                                                next = next->next;
                                                expression* propvalue = evaluateArgument(next);
                                                property* prop = newProperty(propname->ev.strval->content, proptypes, privacy, range, propvalue);
                                                prop->next = properties;
                                                properties = prop;
                                            } else {
                                                ///error
                                            }
                                        }
                                    } else {
                                        ///error
                                    }
                                }
                            }
                        }
                    } else {
                        ///error
                    }
                } else {
                    ///error
                }
                freeExpr(def);
            } else {
                ///error
            }
            definitions = definitions->next;
        }
        tid = ctypeid;
        char* name = args[0]->ev.strval->content;
        type* typ = newType(ctypeid++, strDup(name), required, inherits, properties);
        typedefs* td = newTypedefs(typ);
        td->next = env->types;
        env->types = td;
        expression* typexpr = newExpressionOfType(TYPE_TYP);
        typexpr->ev.intval = tid;
        addToEnvironment(name, typexpr);
    } else {
        ///error
    }
    *returntype = TYPE_TYP;
    returnval->intval = tid;
}

/*! Prints the given string to the output buffer (str)->nil
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_sPrint (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    printf("%s", args[0]->ev.strval->content);
    *returntype = TYPE_NIL;
}

/*! Returns a copy of the given string (str)->str
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_sCopy (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    *returntype = TYPE_STR;
    returnval->strval = newString(strDup(args[0]->ev.strval->content));
}

/*! Returns the size of the given string (str)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_sSize (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    *returntype = TYPE_INT;
    returnval->intval = args[0]->ev.strval->size;
}

/*! Returns the integer value of the character at the given index of the given string (str, int)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_sChar (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    *returntype = TYPE_INT;
    returnval->intval = args[0]->ev.strval->content[castToInt(args[1])];
}

/*! Returns a substring of the given string between the two given integers (or the integer and the end if only one is given) (str, int, [int])->str
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_sSubstr (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int end;
    if (numargs == 2) {
        end = args[0]->ev.strval->size;
    } else {
        end = castToInt(args[2]) + 1;
    }
    *returntype = TYPE_STR;
    returnval->strval = newString(substr(args[0]->ev.strval->content, castToInt(args[1]), end));
}

/*! Returns the first index of the given substring/character in the given string or -1 if nothing is found (str, str/int)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_sFind (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    string* str = args[0]->ev.strval;
    char* result;
    if (args[1]->type == TYPE_INT) {
        result = strchr(str->content, castToInt(args[1]));
    } else {
        string* tempstr = castToStr(args[1]);
        result = strstr(str->content, tempstr->content);
        if (args[1]->type != TYPE_STR) {
            free(tempstr);
        }
    }
    *returntype = TYPE_INT;
    if (result == NULL) {
        returnval->intval = -1;
    } else {
        returnval->intval = result - str->content;
    }
}

/*! Returns the last index of the given substring/character in the given string or -1 if nothing is found (str, str/int)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_sFindlast (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    string* haystack = args[0]->ev.strval;
    char* result = NULL;
    if (args[1]->type == TYPE_INT) {
        result = strrchr(haystack->content, castToInt(args[1]));
    } else {
        string* needle = castToStr(args[1]);
        int i;
        int j;
        for (i = haystack->size - needle->size; i >= 0; --i) {
            j = 0;
            while (haystack->content[i + j] == needle->content[j]) {
                ++j;
            }
            if (j == needle->size) {
                result = haystack->content + i;
                break;
            }
        }
        if (args[1]->type != TYPE_STR) {
            free(needle);
        }
    }
    *returntype = TYPE_INT;
    if (result == NULL) {
        returnval->intval = -1;
    } else {
        returnval->intval = result - haystack->content;
    }
}

/*! Returns the every index of the given substring/character in the given string or an empty list if nothing is found (str, str/int)->arr
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_sFindall (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    static int proportion = 10;
    static int increaseby = 2;
    string* haystack = args[0]->ev.strval;
    int size;
    if (haystack->size < proportion) {
        size = 2;
    } else {
        size = haystack->size / proportion;
    }
    array* result = newArray(size);
    int csize = 0;
    int realsize = 0;
    int i;
    if (args[1]->type == TYPE_INT) {
        int needle = castToInt(args[1]);
        for (i = 0; i < haystack->size; ++i) {
            if (haystack->content[i] == needle) {
                if (csize >= size) {
                    result = resizeArray(result, 0, result->size * increaseby);
                }
                expression* expr = newExpressionOfType(TYPE_INT);
                expr->ev.intval = i;
                result->content[csize++] = expr;
                ++realsize;
            }
        }
    } else {
        string* needle = castToStr(args[1]);
        int j;
        for (i = 0; i < haystack->size; ++i) {
            j = 0;
            while (haystack->content[i + j] != 0 && haystack->content[i + j] == needle->content[j]) {
                ++j;
            }
            if (j == needle->size) {
                if (csize >= size) {
                    result = resizeArray(result, 0, result->size * increaseby);
                }
                expression* expr = newExpressionOfType(TYPE_INT);
                expr->ev.intval = i;
                result->content[csize++] = expr;
                ++realsize;
                i += j - 1;
            }
        }
        if (args[1]->type != TYPE_STR) {
            free(needle);
        }
    }
    resizeArray(result, 0, realsize);
    *returntype = TYPE_ARR;
    returnval->arrval = result;
}

/*! Returns whether or not the given substring/character is in the given string (str, [int])->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_sContains (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    prim_sFind(args, numargs, returnval, returntype);
    *returntype = TYPE_INT;
    returnval->intval = returnval->intval > 0;
}

/*! Concats each given string together (str...)->str
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_sConcat (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int size = 0;
    int i;
    for (i = 0; i < numargs; ++i) {
        size += args[i]->ev.strval->size;
    }
    char* filler = allocate(size + 1);
    memset(filler, '0', size);
    filler[size] = '\0';
    string* result = newString(filler);
    int start = 0;
    int j;
    for (i = 0; i < numargs; ++i) {
        for (j = 0; j < args[i]->ev.strval->size; ++j) {
            result->content[start + j] = args[i]->ev.strval->content[j];
        }
        start += args[i]->ev.strval->size;
    }
    *returntype = TYPE_STR;
    returnval->strval = result;
}

/*! Returns the given string after replacing each instance of one given substring for another (str, str, str)->str
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_sReplace (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    prim_sFindall(args, numargs, returnval, returntype);
    array* indices = returnval->arrval;
    string* origstr = args[0]->ev.strval;
    string* oldstr = castToStr(args[1]);
    string* newstr = castToStr(args[2]);
    int size = origstr->size + (newstr->size - oldstr->size) * indices->end;
    char* filler = allocate(size + 1);
    filler[size] = '\0';
    string* result = newString(filler);
    result->size = size;
    int arrindex = indices->start;
    int origindex = 0;
    int i;
    int j;
    for (i = 0; i < size; ++i) {
        if (arrindex < indices->end && i == indices->content[arrindex]->ev.intval) {
            for (j = 0; j < newstr->size; ++j) {
                result->content[i + j] = newstr->content[j];
            }
            i += j - 1;
            origindex += oldstr->size;
            ++arrindex;
        } else {
            result->content[i] = origstr->content[origindex++];
        }
    }
    if (args[1]->type != TYPE_STR) {
        free(oldstr);
    }
    if (args[2]->type != TYPE_STR) {
        free(newstr);
    }
    *returntype = TYPE_STR;
    returnval->strval = result;
}

/*! Returns the given string after inserting the second given substring after each index of the first (str, str, str)->str
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_sInsert (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    prim_sFindall(args, numargs, returnval, returntype);
    array* indices = returnval->arrval;
    string* origstr = args[0]->ev.strval;
    string* oldstr = castToStr(args[1]);
    string* newstr = castToStr(args[2]);
    int size = origstr->size + newstr->size * indices->end;
    char* filler = allocate(size + 1);
    filler[size] = '\0';
    string* result = newString(filler);
    result->size = size;
    int arrindex = indices->start;
    int origindex = 0;
    int i;
    int j;
    for (i = 0; i < size; ++i) {
        if (arrindex < indices->end && origindex == indices->content[arrindex]->ev.intval + oldstr->size) {
            for (j = 0; j < newstr->size; ++j) {
                result->content[i + j] = newstr->content[j];
            }
            i += j - 1;
            ++arrindex;
        } else {
            result->content[i] = origstr->content[origindex++];
        }
    }
    if (args[1]->type != TYPE_STR) {
        free(oldstr);
    }
    if (args[2]->type != TYPE_STR) {
        free(newstr);
    }
    *returntype = TYPE_STR;
    returnval->strval = result;
}

/*! Returns the given string after replacing each instance of one given substring/character for another (str, str/int, str/int)->str
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_sRemove (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    expression* result = newExpressionOfType(TYPE_STR);
    result->ev.strval = newString(strDup(args[0]->ev.strval->content));
    expression* empty = newExpressionOfType(TYPE_STR);
    empty->ev.strval = newString("");
    expression* newargs[3];
    newargs[0] = result;
    newargs[2] = empty;
    int i;
    for (i = 1; i < numargs; ++i) {
        newargs[1] = args[i];
        prim_sReplace(newargs, 3, returnval, returntype);
        free(result->ev.strval->content);
        free(result->ev.strval);
        result->ev.strval = returnval->strval;
    }
    free(empty->ev.strval);
    free(empty);
    *returntype = TYPE_STR;
    returnval->strval = result->ev.strval;
}

/*! Returns a reversed copy of the given string (str)->str
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_sReverse (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    string* origstr = args[0]->ev.strval;
    string* result = newString(strDup(origstr->content));
    int i;
    for (i = 0; i < result->size; ++i) {
        result->content[i] = origstr->content[result->size - i - 1];
    }
    *returntype = TYPE_STR;
    returnval->strval = result;
}

/*! Returns an all upper-case copy of the given string (str)->str
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_sUpper (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    *returntype = TYPE_STR;
    returnval->strval = changeCase(args[0]->ev.strval, CASE_UPPER);
}

/*! Returns an all lower-case copy of the given string (str)->str
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_sLower (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    *returntype = TYPE_STR;
    returnval->strval = changeCase(args[0]->ev.strval, CASE_LOWER);
}

/*! Returns an all sentence-case copy of the given string (str)->str
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_sSentence (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    *returntype = TYPE_STR;
    returnval->strval = changeCase(args[0]->ev.strval, CASE_SENTENCE);
}

/*! Returns an all title-case copy of the given string (str)->str
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_sTitle (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    *returntype = TYPE_STR;
    returnval->strval = changeCase(args[0]->ev.strval, CASE_TITLE);
}

/*! Tries to convert the given string to an integer using the given base or 10 if none is given, returning nil on failure (str, [int])->int/nil
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_sInt (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    prim_sFlo(args, numargs, returnval, returntype);
    if (*returntype != TYPE_NIL){
        expression* newargs[1];
        newargs[0] = newExpressionOfType(TYPE_FLO);
        newargs[0]->ev.floval = returnval->floval;
        prim_fRound(newargs, 1, returnval, returntype);
        free(newargs[0]);
    }
}

/*! Tries to convert the given string to a float, returning nil on failure (str)->flt/nil
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_sFlo (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int base;
    if (numargs == 1) {
        base = BASE;
    } else {
        base = castToInt(args[1]);
        if (base == 0) {
            *returntype = TYPE_NIL;
            return;
        }
    }
    expression* result = castToNum(args[0]->ev.strval, base);
    *returntype = result->type;
    returnval->floval = result->ev.floval;
}

/*! Returns a copy of the given string (str)->str
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_sStr (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    *returntype = TYPE_STR;
    returnval->strval = newString(strDup(args[0]->ev.strval->content));
}

/*! Converts the given string to an array, splitting the string by the given delimiter or each character if none is given (str, [str])->arr
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_sArr (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    string* str = args[0]->ev.strval;
    string* delimeter = NULL;
    array* arrval;
    int dsize;
    int i;
    if (numargs == 2) {
        delimeter = castToStr(args[1]);
        dsize = delimeter->size;
        arrval = newArray(str->size + dsize);
        for (i = 1; i < arrval->size; i += dsize + 1) {
            memcpy(arrval->content + i, delimeter->content, dsize);
        }
    } else {
        dsize = 0;
        arrval = newArray(str->size);
    }
    //int j = 0;
    for (i = 0; i < arrval->size; i += dsize + 1) {
        //arrval->content[i] = str->content[j++]; !!!
    }
    if (numargs == 2 && args[1]->type != TYPE_STR) {
        free(delimeter->content);
        free(delimeter);
    }
    *returntype = TYPE_STR;
    returnval->arrval = arrval;
}

/*! Converts the given string to a date if in the correct format or returns nil (str)->dat/nil
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_sDat (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
	date dat = newDate(args[0]->ev.strval);
	if (dat == NIL) {
		*returntype = TYPE_NIL;
	} else {
    	*returntype = TYPE_DAT;
    	returnval->datval = dat;
    }
}

/*! Returns type string (str)->typ
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_sTyp (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    *returntype = TYPE_TYP;
    returnval->intval = TYPE_STR;
}

