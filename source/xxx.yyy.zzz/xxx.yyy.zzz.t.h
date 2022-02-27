/*----------------------------------------------------------------------------
Neoheurist xxx.yyy.zzz type header

Copyright 2012-2015 John L. Hart IV. All rights reserved.

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
Neoheurist xxx.yyy.zzz type header file
----------------------------------------------------------------------------*/

#ifndef XXX_YYY_ZZZ_T_H
#define XXX_YYY_ZZZ_T_H

#include "../Motel/motel.compilation.t.h"
#include "../Motel/motel.types.t.h"

#include "../Main/main.t.h"

#include "../Command/command.t.h"

/*----------------------------------------------------------------------------
_AssociationDescriber()
----------------------------------------------------------------------------
Functions of this type output the state an association in regard to a given context
----------------------------------------------------------------------------
Parameters:

pContext - (I) execution context (carries state as executed)
pConnections - (I) three dimensional arrary of navigable locations
----------------------------------------------------------------------------
Notes:

Implementation of this function is the responsibility of each location
---------------------------------------------------------------------------*/

#define ASSOCIATION_DESCRIBER_FUNCTION "AssociationDescriber"

typedef boolean(CALLING_CONVENTION * _AssociationDescriber)
(
    Context *,
    Coordinates *
);

/*----------------------------------------------------------------------------
_ListAdjacentLocations()
----------------------------------------------------------------------------
Functions of this type detect neighboring locations
----------------------------------------------------------------------------
Parameters:

Context *             - (I) holds a pointer to a 3 x 3 x 3 cube of associated locations
_AssociationDescriber - (I) function to determine and display the state of an association
----------------------------------------------------------------------------
Notes:

Implementation of this function is the responsibility of the location engine
---------------------------------------------------------------------------*/

typedef void (CALLING_CONVENTION * _ListAdjacentLocations)
(
    Context *,
    _AssociationDescriber
);

/*----------------------------------------------------------------------------
_LocationDescriber()
----------------------------------------------------------------------------
Functions of this type output the state a location in regard to a given context
----------------------------------------------------------------------------
Parameters:

pContext - (I) execution context (travels from state to state as executed)
----------------------------------------------------------------------------
Notes:

Implementation of this function is the responsibility of each location
---------------------------------------------------------------------------*/

#define LOCATION_DESCRIBER_FUNCTION "LocationDescriber"

typedef void (CALLING_CONVENTION * _LocationDescriber)
(
    Context *,
    _ListAdjacentLocations
);

/*----------------------------------------------------------------------------
_LocationCommandProcessor()
----------------------------------------------------------------------------
Functions of this type alter the state of the location and context
----------------------------------------------------------------------------
Parameters:

pContext - (I) execution context (travels from state to state as executed)
----------------------------------------------------------------------------
Notes:

Implementation of this function is the responsibility of each location
---------------------------------------------------------------------------*/

#define LOCATION_COMMAND_PROCESSOR_FUNCTION "LocationCommandProcessor"

typedef boolean(CALLING_CONVENTION * _LocationCommandProcessor)
(
    Context *,
    Tokens
);

/*----------------------------------------------------------------------------
_LocationValidator()
----------------------------------------------------------------------------
Functions of this type validate the logical consistency of a location in regard
to a given context
----------------------------------------------------------------------------
Parameters:

pContext - (I) execution context (travels from state to state as executed)
----------------------------------------------------------------------------
Notes:

Implementation of this function is the responsibility of each location
---------------------------------------------------------------------------*/

#define LOCATION_VALIDATOR_FUNCTION "LocationValidator"

typedef void (CALLING_CONVENTION * _LocationValidator)
(
    Context *
);

/*----------------------------------------------------------------------------
_LocationDestructor()
----------------------------------------------------------------------------
Functions of this type persist and finalize the state of a location
----------------------------------------------------------------------------
Parameters:

pContext - (I) execution context (travels from state to state as executed)
----------------------------------------------------------------------------
Notes:

Implementation of this function is the responsibility of each location
---------------------------------------------------------------------------*/

#define LOCATION_DESTRUCTOR_FUNCTION "LocationDestructor"

typedef void (CALLING_CONVENTION * _LocationDestructor)
(
    Context *
);

/*----------------------------------------------------------------------------
_LocationConstructor()
----------------------------------------------------------------------------
Functions of this type initialize or reload the state of a location
----------------------------------------------------------------------------
Parameters:

pContext - (I) execution context (travels from state to state as executed)
----------------------------------------------------------------------------
Notes:

Implementation of this function is the responsibility of each location
---------------------------------------------------------------------------*/

#define LOCATION_CONSTRUCTOR_FUNCTION "LocationConstructor"

typedef void (CALLING_CONVENTION * _LocationConstructor)
(
    Context *
);

#endif

