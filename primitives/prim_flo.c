/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   prim_flo.c
    @brief  All of the primitive function for floats used by the language
    (C) 2011 Jack Holland. All rights reserved.
*/

#include <stdlib.h>
#include <math.h>

#include "prim_int.h"
#include "../source/constants.h"
#include "../source/typedefs.h"
#include "../source/constructors.h"
#include "../source/casting.h"

/*! Adds up the given list of floats (flo...)->flo
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_fAdd (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int i;
    double result = 0;
    for (i = 0; i < numargs; ++i) {
        result += castToFlo(args[i]);
    }
    *returntype = TYPE_FLO;
    returnval->floval = result;
}

/*! Subtracts each integer in the given list by the previous float (flo...)->flo
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_fSub (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int i;
    double result = args[0]->ev.floval;
    for (i = 1; i < numargs; ++i) {
        result -= castToFlo(args[i]);
    }
    *returntype = TYPE_FLO;
    returnval->floval = result;
}

/*! Multiplies together each float in the given list (flo...)->flo
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_fMul (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int i;
    double result = 1;
    for (i = 0; i < numargs; ++i) {
        result *= castToFlo(args[i]);
    }
    *returntype = TYPE_FLO;
    returnval->floval = result;
}

/*! Divides each float in the given list by the previous float (flo...)->flo
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_fDiv (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int i;
    double result = args[0]->ev.floval;
    for (i = 1; i < numargs; ++i) {
        result /= castToFlo(args[i]);
    }
    *returntype = TYPE_FLO;
    returnval->floval = result;
}

/*! Raises the first float to the power of the second integer or float (flo, int/flo)->flo
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_fPow (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int result;
    double exp = castToFlo(args[1]);
    if (exp < 0) {
        result = 0;
    } else {
        result = pow(args[0]->ev.floval, exp);
    }
    *returntype = TYPE_FLO;
    returnval->floval = result;
}

/*! Calculates the square root of the given float (flo)->flo
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_fSqrt (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    *returntype = TYPE_FLO;
    returnval->floval = sqrt(args[0]->ev.floval);
}

/*! Returns the logarithm of the given double (flo, [flo])->flo
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_fLog (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    double result = args[0]->ev.floval;
    if (numargs == 1) {
        result = log(result);
    } else {
        result = log(result) / log(castToFlo(args[1]));
    }
    *returntype = TYPE_FLO;
    returnval->floval = result;
}

/*! Returns the absolute value of the given integer (int)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_fAbs (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    double result = fabs(args[0]->ev.floval);
    *returntype = TYPE_FLO;
    returnval->floval = result;
}

/*! Returns the maximum of the given floats (flo...)->flo
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_fMax (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int i;
    double potential;
    double result = args[0]->ev.intval;
    for (i = 1; i < numargs; ++i) {
        potential = castToFlo(args[i]);
        if (potential > result) {
            result = potential;
        }
    }
    *returntype = TYPE_FLO;
    returnval->floval = result;
}

/*! Returns the minimum of the given floats (flo...)->flo
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_fMin (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int i;
    double potential;
    double result = args[0]->ev.intval;
    for (i = 1; i < numargs; ++i) {
        potential = castToFlo(args[i]);
        if (potential < result) {
            result = potential;
        }
    }
    *returntype = TYPE_FLO;
    returnval->floval = result;
}

/*! Returns the given float bankers'-rounded to the nearest digit of accuracy (returns an int if no accuracy specified) (flo, [int])->int/flo
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_fRound (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int digits;
    if (numargs == 1) {
        digits = -1;
    } else {
        digits = castToInt(args[1]);
    }
    expression* result = fRound(args[0]->ev.floval, digits);
    *returntype = result->type;
    if (result->type == TYPE_INT) {
        returnval->intval = result->ev.intval;
    } else {
        returnval->floval = result->ev.floval;
    }
}

/*! Returns the given float rounded up to the nearest digit of accuracy (returns an int if no accuracy specified) (flo, [int])->int/flo
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_fCeil (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int returnint;
    double result = args[0]->ev.floval;
    if (numargs == 1) {
        result = ceil(result);
        returnint = 1;
    } else {
        int scale = pow(BASE, castToInt(args[1]));
        result *= scale * BASE;
        returnint = 0;
        int toround = 1;
        int intval = result;
        while ((double)intval != result) {
            result *= BASE;
            intval = result;
            ++toround;
        }
        while (toround-- > 0) {
            result = ceil(result / BASE);
        }
        result /= scale;
    }
    if (returnint) {
        *returntype = TYPE_INT;
        returnval->intval = (int)result;
    } else {
        *returntype = TYPE_FLO;
        returnval->floval = result;
    }
}

/*! Returns the given float rounded down to the nearest digit of accuracy (returns an int if no accuracy specified) (flo, [int])->int/flo
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_fFloor (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int returnint;
    double result = args[0]->ev.floval;
    if (numargs == 1) {
        result = ceil(result);
        returnint = 1;
    } else {
        int scale = pow(BASE, castToInt(args[1]));
        result *= scale * BASE;
        returnint = 0;
        int toround = 1;
        int intval = result;
        while ((double)intval != result) {
            result *= BASE;
            intval = result;
            ++toround;
        }
        while (toround-- > 0) {
            result = floor(result / BASE);
        }
        result /= scale;
    }
    if (returnint) {
        *returntype = TYPE_INT;
        returnval->intval = (int)result;
    } else {
        *returntype = TYPE_FLO;
        returnval->floval = result;
    }
}

/*! Returns the sine of the given float (flo)->flo
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_fSin (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    double result = sin(args[0]->ev.floval);
    *returntype = TYPE_FLO;
    returnval->floval = result;
}

/*! Returns the cosine of the given float (flo)->flo
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_fCos (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    double result = cos(args[0]->ev.floval);
    *returntype = TYPE_FLO;
    returnval->floval = result;
}

/*! Returns the tangent of the given float (flo)->flo
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_fTan (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    double result = tan(args[0]->ev.floval);
    *returntype = TYPE_FLO;
    returnval->floval = result;
}

/*! Returns the arcsine of the given float (flo)->flo
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_fAsin (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    double result = asin(args[0]->ev.floval);
    *returntype = TYPE_FLO;
    returnval->floval = result;
}

/*! Returns the arccosine of the given float (flo)->flo
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_fAcos (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    double result = acos(args[0]->ev.floval);
    *returntype = TYPE_FLO;
    returnval->floval = result;
}

/*! Returns the arctangent of the given float (flo)->flo
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_fAtan (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    double result = atan(args[0]->ev.floval);
    *returntype = TYPE_FLO;
    returnval->floval = result;
}

/*! Returns the arctangent of the given float using two floats for sign purposes (flo, flo)->flo
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_fAtan2 (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    double result = atan2(args[0]->ev.floval, args[1]->ev.floval);
    *returntype = TYPE_FLO;
    returnval->floval = result;
}

/*! Returns the hyperbolic sine of the given float (flo)->flo
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_fSinh (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    double result = sinh(args[0]->ev.floval);
    *returntype = TYPE_FLO;
    returnval->floval = result;
}

/*! Returns the hyperbolic cosine of the given float (flo)->flo
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_fCosh (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    double result = cosh(args[0]->ev.floval);
    *returntype = TYPE_FLO;
    returnval->floval = result;
}

/*! Returns the hyperbolic tangent of the given float (flo)->flo
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_fTanh (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    double result = tanh(args[0]->ev.floval);
    *returntype = TYPE_FLO;
    returnval->floval = result;
}

///*! Returns the hyperbolic area sine of the given float (flo)->flo
//    @param args         the list of arguments
//    @param numargs      the number of arguments
//    @param returnval    the value the function returns after it ends
//    @param returntype   the type of value the function returns after it ends
//    @return             nothing
//*/
//void prim_fAsinh (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
//    double result = asin(args->value->ev.floval);
//    *returntype = TYPE_FLO;
//    returnval->floval = result;
//}
//
///*! Returns the hyperbolic area cosine of the given float (flo)->flo
//    @param args         the list of arguments
//    @param numargs      the number of arguments
//    @param returnval    the value the function returns after it ends
//    @param returntype   the type of value the function returns after it ends
//    @return             nothing
//*/
//void prim_fAcosh (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
//    double result = acos(args->value->ev.floval);
//    *returntype = TYPE_FLO;
//    returnval->floval = result;
//}
//
///*! Returns the hyperbolic area tangent of the given float (flo)->flo
//    @param args         the list of arguments
//    @param numargs      the number of arguments
//    @param returnval    the value the function returns after it ends
//    @param returntype   the type of value the function returns after it ends
//    @return             nothing
//*/
//void prim_fAtanh (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
//    double result = atan(args->value->ev.floval);
//    *returntype = TYPE_FLO;
//    returnval->floval = result;
//}

/*! Converts the given degrees to radians (flo)->flo
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_fRadians (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    double result = args[0]->ev.floval * PI / 180;
    *returntype = TYPE_FLO;
    returnval->floval = result;
}

/*! Converts the given radians to degrees (flo)->flo
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_fDegrees (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    double result = args[0]->ev.floval * 180 / PI;
    *returntype = TYPE_FLO;
    returnval->floval = result;
}

/*! Returns whether each given float is less than the next float (flo...)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_fLess (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int i;
    double last = args[0]->ev.floval;
    double current;
    int result = 1;
    for (i = 1; i < numargs; ++i) {
        current = castToFlo(args[i]);
        if (last >= current) {
            result = 0;
            break;
        }
        last = current;
    }
    *returntype = TYPE_INT;
    returnval->intval = result;
}

/*! Returns whether each given float is less than or equal to the next float (flo...)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_fLequal (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int i;
    double last = args[0]->ev.floval;
    double current;
    int result = 1;
    for (i = 1; i < numargs; ++i) {
        current = castToFlo(args[i]);
        if (last > current) {
            result = 0;
            break;
        }
        last = current;
    }
    *returntype = TYPE_INT;
    returnval->intval = result;
}

/*! Returns whether each given float is equal to the next float (flo...)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_fEqual (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int i;
    double last = args[0]->ev.floval;
    double current;
    int result = 1;
    for (i = 1; i < numargs; ++i) {
        current = castToFlo(args[i]);
        if (last != current) {
            result = 0;
            break;
        }
        last = current;
    }
    *returntype = TYPE_INT;
    returnval->intval = result;
}

/*! Returns whether each given float is not equal to the next float (flo...)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_fNequal (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int i;
    double last = args[0]->ev.floval;
    double current;
    int result = 1;
    for (i = 1; i < numargs; ++i) {
        current = castToFlo(args[i]);
        if (last == current) {
            result = 0;
            break;
        }
        last = current;
    }
    *returntype = TYPE_INT;
    returnval->intval = result;
}

/*! Returns whether each given float is more than or equal to the next float (flo...)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_fMequal (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int i;
    double last = args[0]->ev.floval;
    double current;
    int result = 1;
    for (i = 1; i < numargs; ++i) {
        current = castToFlo(args[i]);
        if (last < current) {
            result = 0;
            break;
        }
        last = current;
    }
    *returntype = TYPE_INT;
    returnval->intval = result;
}

/*! Returns whether each given float is more than the next float (flo...)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_fMore (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int i;
    double last = args[0]->ev.floval;
    double current;
    int result = 1;
    for (i = 1; i < numargs; ++i) {
        current = castToFlo(args[i]);
        if (last <= current) {
            result = 0;
            break;
        }
        last = current;
    }
    *returntype = TYPE_INT;
    returnval->intval = result;
}

/*! Converts the given float to an integer (flo)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_fInt (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int result = castToInt(args[0]);
    *returntype = TYPE_INT;
    returnval->intval = result;
}

/*! Returns the given float (this is so the function float can be called on any number) (flo)->flo
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_fFlo (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    *returntype = TYPE_FLO;
    returnval->floval = args[0]->ev.floval;
}

/*! Converts the given float to a string (flo)->str
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_fStr (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    string* result = castToStr(args[0]);
    *returntype = TYPE_STR;
    returnval->strval = result;
}

/*! Converts the given float to an array with the float as its only element (flo)->arr
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_fArr (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    array* result = newArray(1);
    expression* floval = newExpression_t(TYPE_FLO);
    floval->ev.floval = args[0]->ev.floval;
    result->content[0] = floval;
    *returntype = TYPE_STR;
    returnval->arrval = result;
}

/*! Returns type float (flo)->typ
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_fTyp (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    *returntype = TYPE_TYP;
    returnval->intval = TYPE_FLO;
}
