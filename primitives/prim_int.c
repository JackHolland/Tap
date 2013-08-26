/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   prim_int.c
    @brief  All of the primitive functions for integers used by the language
    (C) 2011 Jack Holland. All rights reserved.
*/

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "prim_int.h"
#include "prim_flo.h"
#include "../source/constants.h"
#include "../source/typedefs.h"
#include "../source/engine.h"
#include "../source/constructors.h"
#include "../source/casting.h"

/*! Throws an error with the given code and message (int, str)->nil
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iError (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    errorlist* error = newErrorlist(args[0]->ev.intval, castToStr(args[1]), 1, 0);
    addError(error);
    *returntype = TYPE_NIL;
}

/*! Adds up the given list of integers (int...)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iAdd (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int i;
    long result = 0;
    for (i = 0; i < numargs; ++i) {
        result += castToInt(args[i]);
    }
    *returntype = TYPE_INT;
    returnval->intval = result;
}

/*! Subtracts each integer in the given list by the previous integer (int...)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iSub (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int i;
    long result = args[0]->ev.intval;
    for (i = 1; i < numargs; ++i) {
        result -= castToInt(args[i]);
    }
    *returntype = TYPE_INT;
    returnval->intval = result;
}

/*! Multiplies together each integer in the given list (int...)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iMul (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int i;
    long result = 1;
    for (i = 0; i < numargs; ++i) {
        result *= castToInt(args[i]);
    }
    *returntype = TYPE_INT;
    returnval->intval = result;
}

/*! Divides each integer in the given list by the previous integer (int...)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iDiv (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int i;
    long result = args[0]->ev.intval;
    for (i = 1; i < numargs; ++i) {
        result /= castToInt(args[i]);
    }
    *returntype = TYPE_INT;
    returnval->intval = result;
}

/*! Calculates the modulus of each division, which is calculated by each integer in the list divided by the next integer (int...)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iMod (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int i;
    long result = args[0]->ev.intval;
    for (i = 1; i < numargs; ++i) {
        result %= castToInt(args[i]);
    }
    *returntype = TYPE_INT;
    returnval->intval = result;
}

/*! Raises the first integer to the power of the second integer or float (int, int/flo)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iPow (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    long result;
    double exp = castToFlo(args[1]);
    if (exp < 0) {
        result = 0;
    } else {
        result = pow(args[0]->ev.intval, exp);
    }
    *returntype = TYPE_INT;
    returnval->intval = result;
}

/*! Calculates the square root of the given integer (int)->flo
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iSqrt (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int intval = args[0]->ev.intval;
    args[0]->ev.floval = (double)intval;
    prim_fSqrt(args, numargs, returnval, returntype);
    args[0]->ev.intval = intval;
}

/*! Returns the logarithm of the given integer (int, [flo])->flo
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iLog (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int intval = args[0]->ev.intval;
    args[0]->ev.floval = (double)intval;
    prim_fLog(args, numargs, returnval, returntype);
    args[0]->ev.intval = intval;
}

/*! Returns the absolute value of the given integer (int)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iAbs (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    long result = abs(args[0]->ev.intval);
    *returntype = TYPE_INT;
    returnval->intval = result;
}

/*! Returns the maximum of the given integers (int...)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iMax (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int i;
    int potential;
    long result = args[0]->ev.intval;
    for (i = 1; i < numargs; ++i) {
        potential = castToInt(args[i]);
        if (potential > result) {
            result = potential;
        }
    }
    *returntype = TYPE_INT;
    returnval->intval = result;
}

/*! Returns the minimum of the given integers (int...)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iMin (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int i;
    int potential;
    long result = args[0]->ev.intval;
    for (i = 1; i < numargs; ++i) {
        potential = castToInt(args[0]);
        if (potential < result) {
            result = potential;
        }
    }
    *returntype = TYPE_INT;
    returnval->intval = result;
}

/*! Returns the given integer bankers'-rounded to the nearest digit of accuracy (int, [int])->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iRound (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    long result = args[0]->ev.intval;
    int nearest;
    if (numargs == 1) {
        nearest = BASE;
    } else {
        nearest = pow(BASE, castToInt(args[1]));
    }
    if (nearest > 1) {
        int roundoff = result % nearest;
        if (roundoff < nearest / 2 || (roundoff == nearest / 2 && (nearest / BASE) % 2 == 0)) {
            result -= roundoff;
        } else {
            result += nearest - roundoff;
        }
    }
    *returntype = TYPE_INT;
    returnval->intval = result;
}

/*! Returns the given integer rounded up to the nearest digit of accuracy (int, [int])->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iCeil (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    long result = args[0]->ev.intval;
    int nearest;
    if (numargs == 1) {
        nearest = BASE;
    } else {
        nearest = pow(BASE, castToInt(args[1]));
    }
    if (nearest > 1) {
        result += nearest - result % nearest;
    }
    *returntype = TYPE_INT;
    returnval->intval = result;
}

/*! Returns the given integer rounded down to the nearest digit of accuracy (int, [int])->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iFloor (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    long result = args[0]->ev.intval;
    int nearest;
    if (numargs == 1) {
        nearest = BASE;
    } else {
        nearest = pow(BASE, castToInt(args[1]));
    }
    if (nearest > 1) {
        result -= result % nearest;
    }
    *returntype = TYPE_INT;
    returnval->intval = result;
}

/*! Calculates the sine of the given integer (int)->flo
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iSin (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int intval = args[0]->ev.intval;
    args[0]->ev.floval = (double)intval;
    prim_fSin(args, numargs, returnval, returntype);
    args[0]->ev.intval = intval;
}

/*! Calculates the cosine of the given integer (int)->flo
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iCos (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int intval = args[0]->ev.intval;
    args[0]->ev.floval = (double)intval;
    prim_fCos(args, numargs, returnval, returntype);
    args[0]->ev.intval = intval;
}

/*! Calculates the tangent of the given integer (int)->flo
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iTan (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int intval = args[0]->ev.intval;
    args[0]->ev.floval = (double)intval;
    prim_fTan(args, numargs, returnval, returntype);
    args[0]->ev.intval = intval;
}

/*! Calculates the arcsine of the given integer (int)->flo
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iAsin (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int intval = args[0]->ev.intval;
    args[0]->ev.floval = (double)intval;
    prim_fAsin(args, numargs, returnval, returntype);
    args[0]->ev.intval = intval;
}

/*! Calculates the arccosine of the given integer (int)->flo
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iAcos (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int intval = args[0]->ev.intval;
    args[0]->ev.floval = (double)intval;
    prim_fAcos(args, numargs, returnval, returntype);
    args[0]->ev.intval = intval;
}

/*! Calculates the arctangent of the given integer (int)->flo
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iAtan (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int intval = args[0]->ev.intval;
    args[0]->ev.floval = (double)intval;
    prim_fAtan(args, numargs, returnval, returntype);
    args[0]->ev.intval = intval;
}

/*! Calculates the arctangent of the given integer using two integers for sign purposes (int, int)->flo
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iAtan2 (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int intval1 = args[0]->ev.intval;
    args[0]->ev.floval = (double)intval1;
    int intval2 = args[1]->ev.intval;
    args[1]->ev.floval = (double)intval2;
    prim_fAtan2(args, numargs, returnval, returntype);
    args[0]->ev.intval = intval1;
    args[1]->ev.intval = intval2;
}

/*! Calculates the hyperbolic sine of the given integer (int)->flo
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iSinh (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int intval = args[0]->ev.intval;
    args[0]->ev.floval = (double)intval;
    prim_fSinh(args, numargs, returnval, returntype);
    args[0]->ev.intval = intval;
}

/*! Calculates the hyperbolic cosine of the given integer (int)->flo
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iCosh (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int intval = args[0]->ev.intval;
    args[0]->ev.floval = (double)intval;
    prim_fCosh(args, numargs, returnval, returntype);
    args[0]->ev.intval = intval;
}

/*! Calculates the hyperbolic tangent of the given integer (int)->flo
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iTanh (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int intval = args[0]->ev.intval;
    args[0]->ev.floval = (double)intval;
    prim_fTanh(args, numargs, returnval, returntype);
    args[0]->ev.intval = intval;
}

///*! Calculates the hyperbolic arcsine of the given integer (int)->flo
//    @param args         the list of arguments
//    @param numargs      the number of arguments
//    @param returnval    the value the function returns after it ends
//    @param returntype   the type of value the function returns after it ends
//    @return             nothing
//*/
//void prim_iAsinh (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
//    int intval = args->value->ev.intval;
//    args->value->ev.floval = (double)intval;
//    prim_fAsinh(args, numargs, returnval, returntype);
//    args->value->ev.intval = intval;
//}
//
///*! Calculates the hyperbolic arccosine of the given integer (int)->flo
//    @param args         the list of arguments
//    @param numargs      the number of arguments
//    @param returnval    the value the function returns after it ends
//    @param returntype   the type of value the function returns after it ends
//    @return             nothing
//*/
//void prim_iAcosh (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
//    int intval = args->value->ev.intval;
//    args->value->ev.floval = (double)intval;
//    prim_fAcosh(args, numargs, returnval, returntype);
//    args->value->ev.intval = intval;
//}
//
///*! Calculates the hyperbolic tangent of the given integer (int)->flo
//    @param args         the list of arguments
//    @param numargs      the number of arguments
//    @param returnval    the value the function returns after it ends
//    @param returntype   the type of value the function returns after it ends
//    @return             nothing
//*/
//void prim_iAtanh (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
//    int intval = args->value->ev.intval;
//    args->value->ev.floval = (double)intval;
//    prim_fAtanh(args, numargs, returnval, returntype);
//    args->value->ev.intval = intval;
//}

/*! Converts the given degrees to radians (int)->flo
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iRadians (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int intval = args[0]->ev.intval;
    args[0]->ev.floval = (double)intval;
    prim_fRadians(args, numargs, returnval, returntype);
    args[0]->ev.intval = intval;
}

/*! Converts the given radians to degrees (int)->flo
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iDegrees (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int intval = args[0]->ev.intval;
    args[0]->ev.floval = (double)intval;
    prim_fDegrees(args, numargs, returnval, returntype);
    args[0]->ev.intval = intval;
}

/*! Inverts every bit in the given integer (int)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iBnot (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    long result = ~args[0]->ev.intval;
    *returntype = TYPE_INT;
    returnval->intval = result;
}

/*! Ands each bit of each given integer with each bit of the next integer (int...)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iBand (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int i;
    long result = args[0]->ev.intval;
    for (i = 1; i < numargs; ++i) {
        result = result & castToInt(args[i]);
    }
    *returntype = TYPE_INT;
    returnval->intval = result;
}

/*! Ors each bit of each given integer with each bit of the next integer (int...)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iBor (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int i;
    long result = args[0]->ev.intval;
    for (i = 1; i < numargs; ++i) {
        result = result | castToInt(args[i]);
    }
    *returntype = TYPE_INT;
    returnval->intval = result;
}

/*! Exclusive-ors each bit of each given integer with each bit of the next integer (int...)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iBxor (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int i;
    long result = args[0]->ev.intval;
    for (i = 1; i < numargs; ++i) {
        result = result ^ castToInt(args[i]);
    }
    *returntype = TYPE_INT;
    returnval->intval = result;
}

/*! Shifts each bit of the given integer left by 1 (if no argument is given) or by the given integer (int, [int])->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iLshift (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int shiftby;
    if (numargs == 1) {
        shiftby = 1;
    } else {
        shiftby = castToInt(args[1]);
    }
    long result = args[0]->ev.intval << shiftby;
    *returntype = TYPE_INT;
    returnval->intval = result;
}

/*! Arithmetically shifts each bit of the given integer right by 1 (if no argument is given) or by the given integer (int, [int])->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iRashift (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int shiftby;
    if (numargs == 1) {
        shiftby = 1;
    } else {
        shiftby = castToInt(args[1]);
    }
    long result = (uint)(args[0]->ev.intval) >> shiftby;
    *returntype = TYPE_INT;
    returnval->intval = result;
}

/*! Logically shifts each bit of the given integer right by 1 (if no argument is given) or by the given integer (int, [int])->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iRlshift (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int shiftby;
    if (numargs == 1) {
        shiftby = 1;
    } else {
        shiftby = castToInt(args[1]);
    }
    long result = args[0]->ev.intval >> shiftby;
    *returntype = TYPE_INT;
    returnval->intval = result;
}

/*! Inverts the given integer (0->1, anything else->0) (int)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iLnot (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int result = !args[0]->ev.intval;
    *returntype = TYPE_INT;
    returnval->intval = result;
}

/*! Ands each given integer as a boolean with the next integer as a boolean (int...)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iLand (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int i;
    int result = 1;
    for (i = 0; i < numargs; ++i) {
        if (!castToBoo(args[i])) {
            result = 0;
            break;
        }
    }
    *returntype = TYPE_INT;
    returnval->intval = result;
}

/*! Ors each bit of each given integer with each bit of the next integer (int...)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iLor (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int i;
    int result = 0;
    for (i = 0; i < numargs; ++i) {
        if (castToBoo(args[i])) {
            result = 1;
            break;
        }
    }
    *returntype = TYPE_INT;
    returnval->intval = result;
}

/*! Exclusive-ors each bit of each given integer with each bit of the next integer (int...)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iLxor (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int i;
    int result = 0;
    for (i = 0; i < numargs; ++i) {
        if (castToBoo(args[i])) {
            ++result;
        }
    }
    result = (result > 0 && result != numargs);
    *returntype = TYPE_INT;
    returnval->intval = result;
}

/*! Returns whether each given integer is less than the next integer (int...)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iLess (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int i;
    int last = args[0]->ev.intval;
    int current;
    int result = 1;
    for (i = 1; i < numargs; ++i) {
        current = castToInt(args[i]);
        if (last >= current) {
            result = 0;
            break;
        }
        last = current;
    }
    *returntype = TYPE_INT;
    returnval->intval = result;
}

/*! Returns whether each given integer is less than or equal to the next integer (int...)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iLequal (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int i;
    int last = args[0]->ev.intval;
    int current;
    int result = 1;
    for (i = 1; i < numargs; ++i) {
        current = castToInt(args[i]);
        if (last > current) {
            result = 0;
            break;
        }
        last = current;
    }
    *returntype = TYPE_INT;
    returnval->intval = result;
}

/*! Returns whether each given integer is equal to the next integer (int...)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iEqual (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int i;
    int last = args[0]->ev.intval;
    int current;
    int result = 1;
    for (i = 1; i < numargs; ++i) {
        current = castToInt(args[i]);
        if (last != current) {
            result = 0;
            break;
        }
        last = current;
    }
    *returntype = TYPE_INT;
    returnval->intval = result;
}

/*! Returns whether each given integer is not equal to the next integer (int...)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iNequal (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int i;
    int last = args[0]->ev.intval;
    int current;
    int result = 1;
    for (i = 1; i < numargs; ++i) {
        current = castToInt(args[i]);
        if (last == current) {
            result = 0;
            break;
        }
        last = current;
    }
    *returntype = TYPE_INT;
    returnval->intval = result;
}

/*! Returns whether each given integer is more than or equal to the next integer (int...)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iMequal (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int i;
    int last = args[0]->ev.intval;
    int current;
    int result = 1;
    for (i = 1; i < numargs; ++i) {
        current = castToInt(args[i]);
        if (last < current) {
            result = 0;
            break;
        }
        last = current;
    }
    *returntype = TYPE_INT;
    returnval->intval = result;
}

/*! Returns whether each given integer is more than the next integer (int...)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iMore (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int i;
    int last = args[0]->ev.intval;
    int current;
    int result = 1;
    for (i = 1; i < numargs; ++i) {
        current = castToInt(args[i]);
        if (last <= current) {
            result = 0;
            break;
        }
        last = current;
    }
    *returntype = TYPE_INT;
    returnval->intval = result;
}

/*! Returns the second argument if the first is true, the fourth argument if the second is true, etc. (int...)->*
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iIf (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int i;
    expression* result;
    for (i = 0; i < numargs - 1; i += 2) {
        if (i == numargs - 1) {
            result = evaluateLaz(copyExpression(args[i]));
            *returntype = result->type;
            returnval->intval = result->ev.intval;
            freeExpr(result);
        } else {
            result = castToBoo(args[i]);
            if (result->type != TYPE_INT) {
                ///error
            } else if (result->ev.intval) {
                result = evaluateLaz(copyExpression(args[i + 1]));
                *returntype = result->type;
                returnval->intval = result->ev.intval;
                freeExpr(result);
                break;
            }
        }
    }
}

/*! Generates a random number from 0 (inclusive) to the given integer (exclusive) (int)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iRand (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    long result = rand() % args[0]->ev.intval;
    *returntype = TYPE_INT;
    returnval->intval = result;
}

/*! Changes the seed of the random number generator to the given integer seed (int)->nil
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iSrand (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    srand(args[0]->ev.intval);
    *returntype = TYPE_INT;
    returnval->intval = NIL;
}

/*! Converts the given integer from its ASCII value to the equivalent character string (int)->str
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iFascii (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    char* result = allocate(2);
    result[0] = args[0]->ev.intval;
    result[1] = '\0';
    *returntype = TYPE_STR;
    returnval->strval = newString(result);
}

/*! Converts the given integer to a boolean (0 or 1) (int)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iBoo (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    *returntype = TYPE_INT;
    returnval->intval = castToBoo(args[0])->ev.intval;
}

/*! Converts the given integer (0-9) to its ASCII equivalent (48-57) or returns the same integer if it's not 0-9 (int)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iAscii (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    long result = args[0]->ev.intval;
    if (result >= 0 && result <= 9) {
        result += '0';
    }
    *returntype = TYPE_INT;
    returnval->intval = result;
}

/*! Returns the given integer (this is so the function int can be called on any number) (int)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iInt (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    *returntype = TYPE_INT;
    returnval->intval = args[0]->ev.intval;
}

/*! Converts the given integer to a float (int)->flo
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iFlo (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    double result = castToFlo(args[0]);
    *returntype = TYPE_FLO;
    returnval->floval = result;
}

/*! Converts the given integer to a string using the given base (10 if unspecified) (int, [int])->str
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iStr (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    string* result;
    if (numargs == 1) {
        result = castToStr(args[0]);
    } else {
        result = castToStrWithBase(args[0]->ev.intval, castToInt(args[1]));
    }
    *returntype = TYPE_STR;
    returnval->strval = result;
}

/*! Converts the given integer to an array with the integer as its only element (int)->arr
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iArr (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    array* result = newArray(1);
    expression* intval = newExpressionOfType(TYPE_INT);
    intval->ev.intval = args[0]->ev.intval;
    result->content[0] = intval;
    *returntype = TYPE_ARR;
    returnval->arrval = result;
}

/*! Returns type integer (int)->typ
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_iTyp (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    *returntype = TYPE_TYP;
    returnval->intval = TYPE_INT;
}
