/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   main.c
    @brief  The main source file
    (C) 2011 Jack Holland. All rights reserved.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "engine.h"
#include "constants.h"
#include "constructors.h"

extern errorlist* errors;

/*! Main function run from the command line
    @param argc     argument count (the number of arguments given)
    @param argv     argument values (the array of arguments given)
    @return         the return code of the program (EXIT_SUCCESS for a successful output, another code for an error)
*/
int main (int argc, char* argv[]) {
    if (argc >= 2) {
        initializeGlobals();
        expression* parsed = parse(argv[1]);
        expression* evaluated;
        if (errors == NULL) {
            evaluated = evaluate(parsed);
        } else {
            evaluated = newExpression_t(TYPE_NIL);
        }
        char* printed = printExpression(evaluated);
        char* errortext = printErrors();
        printf("%s\n%s", printed, errortext);
        free(printed);
        freeExpr(evaluated);
        freeGlobals();
        return EXIT_SUCCESS;
    } else {
        return EXIT_NO_ARGS;
    }
}
