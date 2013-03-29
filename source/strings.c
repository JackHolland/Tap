/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   strings.c
    @brief  All of the string functions
    (C) 2011 Jack Holland. All rights reserved.
*/

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "strings.h"
#include "constants.h"
#include "constructors.h"

/*! Returns the index of the first occurrence of the given character in the given string (starting from start) or -1 if the character isn't found
    @param text     the text to search from
    @param chr      the character to search for
    @param start    the index of the text to start from
*/
int indexOf (char* text, char chr, uint start) {
    char* index = strchr(text + start, chr); // search the given text to find the address of the character
    if (index == NULL) { // if the character wasn't found
        return -1;
    } else { // if the character was found
        return index - text; // return the the character's index relative to the string
    }
}

/*! Returns the substring of the given string according to the given parameters
    @param text     the text to extract the substring from
    @param start    the start index of the desired substring
    @param end      the end index of the desired substring
    @return         the new substring
*/
char* substr (char* text, uint start, uint end) {
    char* newtext = (char*)allocate(end - start + 1); // allocate memory for the substring
    strncpy(newtext, text + start, end - start); // copy the portion of the text delimited by start and end into the new substring
    newtext[end - start] = '\0'; // escape the new string
    return newtext;
}

/*! Replaces the character at the given index with the given value if there is enough space, otherwise it reallocates more memory for the string
    @param text     the text to edit
    @param index    the index of the value to replace
    @param value    the value to replace the old one with
    @param size     the string's size
    @return         the new substring
*/
uint safeEdit (char* text, uint index, char value, uint size) {
    if (index < size) { // if the index is inside the bounds of the currently allocated memory
        text[index] = value; // replace the old value with the new one
    } else { // if the index is outside the bounds of the currently allocated memory
        char* old = text; // temporarily store the old string value
        size *= 2; // double the size
        text = allocate(size); // allocated more memory
        text = strcpy(text, old); // copy the string from the old location in memory to the new location in memory
        free(old); // free the old memory
    }
    return size; // return the potentially increased size
}

/*! Returns a copy of the given string according to the flag given (0 = upper, 1 = lower, 2 = sentence, 3 = title)
    @param origstr  the string to use
    @param flag     the integer flag used to determine how and when to change each character's case
    @return         the new string
*/
string* changeCase (string* origstr, int flag) {
    string* result = newString(strDup(origstr->content));
    int difference;
    int min1;
    int max1;
    int min2;
    int max2;
    if (flag == CASE_LOWER) {
        difference = 'A' - 'a';
        min1 = 'A';
        max1 = 'Z';
        min2 = 0;
        max2 = 0;
    } else {
        difference = 'a' - 'A';
        min1 = 'a';
        max1 = 'z';
        min2 = 'A';
        max2 = 'Z';
    }
    int first = 1;
    int i;
    for (i = 0; i < result->size; ++i) {
        int chr = origstr->content[i];
        if (!first && ((flag == CASE_SENTENCE && chr == '.') || (flag == CASE_TITLE && chr == ' '))) {
            first = 1;
        } else {
            if (chr >= min1 && chr <= max1 && first) {
                result->content[i] -= difference;
            }
            if ((flag == CASE_SENTENCE || flag == CASE_TITLE) && result->content[i] >= min2 && result->content[i] <= max2) {
                first = 0;
            }
        }
    }
    return result;
}

/*! Copies the given string, including its contents
    @param oldstr   the string to copy
    @return         a copy of the given string with a copy of its contents
*/
inline string* copyString (string* oldstr) {
    return newString(strDup(oldstr->content));
}

/*! Attempts to duplicate the string (using strDup) and errors out if the memory could not be allocated
    @param oldstr   the string to duplicate
    @return         the location in memory of the new string
*/
char* strDup (char* oldstr) {
    char* newstr = strdup(oldstr); // attempt to duplicate the string
    if (newstr == NULL) { // if the memory could not be allocated
        exit(EXIT_OUT_OF_MEMORY); // exit with the code representing an out of memory error
    } else { // if the memory was successfully allocated
        return newstr; // return the location of the new string
    }
}

/*! Returns whether or not the given character is a string
    @param c    the character to check
    @return     1 if the given character is a number, 0 if it isn't
*/
inline int isNumber (char c) {
    return c >= '0' && c <= '9';
}

/*! Returns the size (number of bytes) of the given integer if it were represented as a string
    @param c    the character to check
    @return     1 if the given character is a number, 0 if it isn't
*/
inline int stringSize (int i) {
    if (i == 0) {
        return 1;
    } else {
        return log10(i) + 1;
    }
}
