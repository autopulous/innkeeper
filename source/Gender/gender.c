/*----------------------------------------------------------------------------
Neoheurist character gender object

Copyright 2015 John L. Hart IV. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain all copyright notices,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce all copyright
notices, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY John L. Hart IV "AS IS" AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
NO EVENT SHALL John L. Hart IV OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
DAMAGE.

The views and conclusions contained in the software and documentation are
those of the authors and should not be interpreted as representing official
policies, either expressed or implied, of John L Hart IV.
----------------------------------------------------------------------------*/

#include "gender.h"

Gender CALLING_CONVENTION StringToEnum
(
    char * pString
)
{
    if (ClassMatchString(pString, MALE_WORDS, ""))
    {
        return (Male);
    }
    else if (ClassMatchString(pString, FEMALE_WORDS, ""))
    {
        return (Female);
    }
    else if (ClassMatchString(pString, PERSON_WORDS, ""))
    {
        return (Person);
    }
    else if (ClassMatchString(pString, ROBOT_WORDS, ""))
    {
        return (Robot);
    }
    else
    {
    return (_Gender);
    }
}

char * CALLING_CONVENTION EnumToString
(
    Gender pEnum
)
{
    if (Male == pEnum)
    {
        char * lStrings[] = {MALE_WORDS};

        const long double lStringProbabilities[] = {0.50, 0.25, 0.25}; // must sum to 1.0

        return (ProbabilityString(sizeof(lStrings) / POINTER_SIZE, lStrings, lStringProbabilities));
    }
    else if (Female == pEnum)
    {
        char * lStrings[] = {FEMALE_WORDS};

        const long double lStringProbabilities[] = {0.50, 0.25, 0.25}; // must sum to 1.0

        return (ProbabilityString(sizeof(lStrings) / POINTER_SIZE, lStrings, lStringProbabilities));
    }
    else if (Person == pEnum)
    {
        char * lStrings[] = {PERSON_WORDS};

        const long double lStringProbabilities[] = {0.34, 0.33, 0.33}; // must sum to 1.0

        return (ProbabilityString(sizeof(lStrings) / POINTER_SIZE, lStrings, lStringProbabilities));
    }
    else if (Robot == pEnum)
    {
        char * lStrings[] = {ROBOT_WORDS};

        const long double lStringProbabilities[] = {0.15, 0.35, 0.25, 0.25}; // must sum to 1.0

        return (ProbabilityString(sizeof(lStrings) / POINTER_SIZE, lStrings, lStringProbabilities));
    }
    else
    {
        return ("");
    }
}
