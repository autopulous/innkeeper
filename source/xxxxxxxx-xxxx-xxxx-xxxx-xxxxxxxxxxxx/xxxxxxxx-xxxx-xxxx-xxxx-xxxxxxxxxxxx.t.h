/*----------------------------------------------------------------------------
  Neoheurist item type header
 
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
  Neoheurist item type header file
  ----------------------------------------------------------------------------*/

#ifndef XXXXXXXX_XXXX_XXXX_XXXX_XXXXXXXXXXXX_T_H
#define XXXXXXXX_XXXX_XXXX_XXXX_XXXXXXXXXXXX_T_H

#include "../Motel/motel.compilation.t.h"
#include "../Motel/motel.results.t.h"

#include "../Motel/motel.questa.t.h"

#include "../Main/main.t.h"
#include "../Command/command.t.h"

typedef struct Item
{
    char * UUId;

    Volume volume;
    Weight weight;
} Item;

typedef struct Container
{
    Item item;

    Volume currentVolume; // content volume
    Weight currentWeight; // content weight

    Volume maximumVolume; // container volume capacity
    Weight maximumWeight; // container weight capacity

    motelQuestaHandle items;
} Container;

  /*----------------------------------------------------------------------------
  _ItemDescriber()
  ----------------------------------------------------------------------------
  Functions of this type output the item description from a given context
  ----------------------------------------------------------------------------
  Parameters:

  pContext - (I) execution context
  ----------------------------------------------------------------------------
  Notes:

  Implementation of this function is the responsibility of each item
  ---------------------------------------------------------------------------*/

#define ITEM_DESCRIBER_FUNCTION "ItemDescriber"

typedef void (CALLING_CONVENTION * _ItemDescriber)
(
    Context *
);

/*----------------------------------------------------------------------------
  _ItemCommandProcessor()
  ----------------------------------------------------------------------------
  Functions of this type alter the item of the item and context
  ----------------------------------------------------------------------------
  Parameters:

  pContext - (I) execution context
  ----------------------------------------------------------------------------
  Notes:

  Implementation of this function is the responsibility of each item
  ---------------------------------------------------------------------------*/

#define ITEM_COMMAND_PROCESSOR_FUNCTION "ItemCommandProcessor"

typedef boolean (CALLING_CONVENTION * _ItemCommandProcessor)
(
    Context *,
    Tokens
);

/*----------------------------------------------------------------------------
  _ItemValidator()
  ----------------------------------------------------------------------------
  Functions of this type validate the logical consistency of a item in regard
  to a given context
  ----------------------------------------------------------------------------
  Parameters:

  pContext - (I) execution context
  ----------------------------------------------------------------------------
  Notes:

  Implementation of this function is the responsibility of each item
  ---------------------------------------------------------------------------*/

#define ITEM_VALIDATOR_FUNCTION "ItemValidator"

typedef void (CALLING_CONVENTION * _ItemValidator)
(
    Context *
);

/*----------------------------------------------------------------------------
  _ItemDestructor()
  ----------------------------------------------------------------------------
  Functions of this type persist and finalize the state of a item
  ----------------------------------------------------------------------------
  Parameters:

  pContext - (I) execution context
  ----------------------------------------------------------------------------
  Notes:

  Implementation of this function is the responsibility of each item
  ---------------------------------------------------------------------------*/

#define ITEM_DESTRUCTOR_FUNCTION "ItemDestructor"

typedef void (CALLING_CONVENTION * _ItemDestructor)
(
    Context *
);

/*----------------------------------------------------------------------------
  _ItemConstructor()
  ----------------------------------------------------------------------------
  Functions of this type initialize or reload the state of an item
  ----------------------------------------------------------------------------
  Parameters:

  pContext - (I) execution context
  ----------------------------------------------------------------------------
  Notes:

  Implementation of this function is the responsibility of each item
  ---------------------------------------------------------------------------*/

#define ITEM_CONSTRUCTOR_FUNCTION "ItemConstructor"

typedef void (CALLING_CONVENTION * _ItemConstructor)
(
    Context *
);

#endif