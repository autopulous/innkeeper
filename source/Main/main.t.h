/*----------------------------------------------------------------------------
  Neoheurist engine type header

  Copyright 2011-2015 John L. Hart IV. All rights reserved.

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
  Neoheurist engine types header file
  ----------------------------------------------------------------------------*/

#ifndef ENGINE_T_H
#define ENGINE_T_H

/*----------------------------------------------------------------------------
  Standard libraries
  ----------------------------------------------------------------------------*/

#include <windows.h>

/*----------------------------------------------------------------------------
  Public data types
  ----------------------------------------------------------------------------*/

#include "..\Motel\motel.compilation.t.h"
#include "..\Motel\motel.types.t.h"
#include "..\Motel\motel.results.t.h"

#include "..\Motel\motel.tree.t.h"
#include "..\Motel\motel.tree.i.h"

#include "types.t.h"
#include "vocabulary.t.h"

/*----------------------------------------------------------------------------
  Public functions
  ----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
  Public defines, data types and function prototypes
  ----------------------------------------------------------------------------*/

#define LIBRARY_NAME_SIZE 32

#define CONTEXT_NAME_SIZE 32

#define WEST       0
#define EAST       2
#define WESTEAST   3

#define SOUTH      0
#define NORTH      2
#define NORTHSOUTH 3

#define DOWN       0
#define UP         2
#define DOWNUP     3

#define NAUGHT     1 /* indicates a neutral axis orientation */

typedef struct Coordinates
{
    int X; /* West to east */
    int Y; /* North to south */
    int Z; /* Down to up */
} Coordinates;

typedef Coordinates Connections[NORTHSOUTH][WESTEAST][DOWNUP];

static char * VerbosityDescription[_Verbosity] = {"SMS", "Terse", "Normal", "Verbose"};

typedef struct GenderQualifier
{
	const char * Type;
	const char * Category;

	const char * Subjective;
	const char * Objective;
	const char * Determiner;
	const char * Posessive;
	const char * Reflexive;
} GenderQualifier;

typedef void * RoomStateHandle;

typedef struct Context
{
	/* Location */

    HMODULE Library;

    Coordinates Location;

    Connections * Connections;

	/* Player */

    Gender Gender;
	GenderQualifier * GenderQualifier;

	Level MaxStrength;
	Level Strength;

	Level MaxHealth;
	Level Health;

	Level MaxEnergy;
	Level Energy;

	Level MaxAlertness;
	Level Alertness;

	char Name[CONTEXT_NAME_SIZE];

	/* Perferences */

    Verbosity InteractionInitial;
    Verbosity InteractionSubsequent;
    Verbosity InteractionFrequent;

    boolean CompassTrustworthy;

    Verbosity CompassHeadingMode;

    /* System */

    boolean Trace;
    boolean Debug;
	boolean Valid;

    /* Inventory */

    motelTreeHandle ItemStates;

    /* Dungeon */

    motelTreeHandle LocationStates;
} Context;

#endif