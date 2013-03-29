/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   types.c
    @brief  All of the type functions
    (C) 2011 Jack Holland. All rights reserved.
*/

#include <string.h>

#include "types.h"

/*! Returns whether or not the given list of required properties are satisfied by the given list of extant properties
    @param required     the list of strings indicating the required properties
    @param properties   the list of properties currently assigned to the type
    @return             1 if the list is satisfied, 0 otherwise
*/
int propReqsValid (stringlist* required, property* properties) {
    stringlist* sl = required;
    while (sl != NULL) {
        char* reqstr = sl->str->content;
        int found = 0;
        property* prop = properties;
        while (prop != NULL) {
            if (strcmp(reqstr, prop->name) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            return 0;
        }
        sl = sl->next;
    }
    return 1;
}

/*! Returns whether or not the given property value meets the type requirements of the given property definition
    @param propdef  the property definition to get the type requirements from
    @param propval  the property value whose type is being checked
    @return         1 if the requirements are met, 0 otherwise
*/
int propTypeValid (property* propdef, expression* propval) {
    typelist* validtypes = propdef->types;
    int validtype = validtypes == NULL;
    while (validtypes != NULL) {
        if (propval->type == validtypes->type) {
            validtype = 1;
            break;
        }
        validtypes = validtypes->next;
    }
    return validtype;
}
