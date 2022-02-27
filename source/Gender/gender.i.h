/*----------------------------------------------------------------------------
Neoheurist character gender object interface header

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

/*----------------------------------------------------------------------------
Gender application programmer's interface (API) header file
----------------------------------------------------------------------------*/

#ifndef GENDER_I_H
#define GENDER_I_H

/*----------------------------------------------------------------------------
StringToEnum()
----------------------------------------------------------------------------
Convert a string to a Gender enum value
----------------------------------------------------------------------------
Parameters:

pString - (I) The string to map to a Gender enum value
----------------------------------------------------------------------------
Returns:

A Gender enum value

_Gender - when the string was not recognized as a valid gender
---------------------------------------------------------------------------*/

Gender CALLING_CONVENTION StringToEnum
(
    char * pString
);

/*----------------------------------------------------------------------------
EnumToString()
----------------------------------------------------------------------------
Convert a Gender enum value to a string
----------------------------------------------------------------------------
Parameters:

pEnum - (I) The Gender enum value to map
----------------------------------------------------------------------------
Returns:

A string representing the passed Gender enum value

or

An empty string when an invalid Gender enum value is passed
---------------------------------------------------------------------------*/

char * CALLING_CONVENTION EnumToString
(
    Gender pEnum
);

#endif