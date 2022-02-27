/*----------------------------------------------------------------------------
Motel Aleatory

application programmer's interface (API) header file
----------------------------------------------------------------------------
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

#ifndef MOTEL_ALEATORY_I_H
#define MOTEL_ALEATORY_I_H

#include "motel.aleatory.t.h"

/*----------------------------------------------------------------------------
AleatorySeed1024Period()
----------------------------------------------------------------------------
Seed the 2^1024 period pseudo random number generator
----------------------------------------------------------------------------
Parameters:

pSeed - (I) Seed values (must not be all zero)
---------------------------------------------------------------------------*/

EXPORT_STORAGE_CLASS void CALLING_CONVENTION AleatorySeed1024Period
(
    uint64_t pSeed[64]
);

/*----------------------------------------------------------------------------
Aleatory1024Period()
----------------------------------------------------------------------------
Get the next pseudo randnom number using the 2^1024 period generator

http://xorshift.di.unimi.it/
----------------------------------------------------------------------------
Returns:

A 64 bit pseudo random number
---------------------------------------------------------------------------*/

EXPORT_STORAGE_CLASS uint64_t CALLING_CONVENTION Aleatory1024Period
(
    void
);

/*----------------------------------------------------------------------------
Probability1024Period()
----------------------------------------------------------------------------
Get the next pseudo randnom probability value using the 2^1024 period generator

http://xorshift.di.unimi.it/
----------------------------------------------------------------------------
Returns:

A pseudo random probability value between 0 and 1 inclusive
---------------------------------------------------------------------------*/

EXPORT_STORAGE_CLASS long double CALLING_CONVENTION Probability1024Period
(
    void
);

/*----------------------------------------------------------------------------
AleatorySeed4096Period()
----------------------------------------------------------------------------
Seed the 2^4096 period pseudo random number generator
----------------------------------------------------------------------------
Parameters:

pSeed - (I) Seed values (must not be all zero)
---------------------------------------------------------------------------*/

EXPORT_STORAGE_CLASS void CALLING_CONVENTION AleatorySeed4096Period
(
    uint64_t pSeed[64]
);

/*----------------------------------------------------------------------------
Aleatory4096Period()
----------------------------------------------------------------------------
Get the next pseudo randnom number using the 2^4096 period generator

http://xorshift.di.unimi.it/
----------------------------------------------------------------------------
Returns:

A 64 bit pseudo random number
---------------------------------------------------------------------------*/

EXPORT_STORAGE_CLASS uint64_t CALLING_CONVENTION Aleatory4096Period
(
    void
);

/*----------------------------------------------------------------------------
Probability4096Period()
----------------------------------------------------------------------------
Get the next pseudo randnom probability value using the 2^4096 period generator

http://xorshift.di.unimi.it/
----------------------------------------------------------------------------
Returns:

A pseudo random probability value between 0 and 1 inclusive
---------------------------------------------------------------------------*/

EXPORT_STORAGE_CLASS long double CALLING_CONVENTION Probability4096Period
(
    void
);

/*----------------------------------------------------------------------------
ProbabilityIndex()
----------------------------------------------------------------------------
Get an index into an array of cummulative probabilities using the 2^4096
period pseudo randnom probability generator

http://xorshift.di.unimi.it/
----------------------------------------------------------------------------
Parameters:

pProbabilities - (I) Probability array size (number of elements)
pProbability   - (I) Probabilities (must sum to 1.0)
----------------------------------------------------------------------------
Returns:

An index into the array of probability ranges.
---------------------------------------------------------------------------*/

EXPORT_STORAGE_CLASS size_t CALLING_CONVENTION ProbabilityIndex
(
    size_t pProbabilities,
    const long double pProbability[]
);

//todo add comments

EXPORT_STORAGE_CLASS char * CALLING_CONVENTION ProbabilityString
(
    size_t pStringCount,
    char ** pStrings,
    const long double * pStringProbabilities
);

#endif

