/*----------------------------------------------------------------------------
Neoheurist enumaration type header

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

/*--------------------------------------------------------------------------
  Neoheurist enumeration types header file
  --------------------------------------------------------------------------*/

#ifndef TYPES_T_H
#define TYPES_T_H

#include "../Motel/motel.memory.t.h"

/*--------------------------------------------------------------------------
  Constants
  --------------------------------------------------------------------------*/

#define POINTER_SIZE sizeof(void *)

#define UUID_SIZE sizeof("XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX")

#define MEMORY_MAX UNLIMITED_MEMORY

#define ITEM_LIST_MAX (size_t) 100

/*--------------------------------------------------------------------------
  Simple types
  --------------------------------------------------------------------------*/

typedef long Length;
typedef long Volume;
typedef long Weight;
typedef size_t Items;

/*--------------------------------------------------------------------------
  Complex types
  --------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
  Enumeration types
  --------------------------------------------------------------------------*/

typedef enum Verbosity
{
    Off,     /* silent                    */
	SMS,     /* 160 characters or less    */
	Terse,   /* names only                */
	Normal,  /* names and facts           */
	Verbose, /* names and facts and prose */

	_Verbosity
} Verbosity;

typedef enum Gender
{
	Male,
	Female,
	Person,
	Robot,

	_Gender
} Gender;

typedef enum Closure
{
	Opened,
	Closed,
	Locked,
	Unclosable,

	_Closure
} Closure;

typedef enum Level
{
	Zero = 0,
	One = 10,
	Two = 20,
	Three = 30,
	Four = 40,
	Five = 50,
	Six = 60,
	Seven = 70,
	Eight = 80,
	Nine = 90,
	Ten = 100,
	Eleven = 110,
	Tweleve = 120,

	_Level
} Level;

typedef enum Color
{
    Black = 0,
    Grey = 127,
    Gray = 127,
    White = 255,

    Red = 1,
    Orange = 2,
    Yellow = 3,
    Green = 4,
    Blue = 5,
    Indigo = 6,
    Violet = 7,

    _Color = 256
} Color;

typedef enum Sharpness
{
    Blunt = 0,
    Blunted = 1,
    Pointed = 2,
    Pointy = 3,

    _Sharpness
} Sharpness;

#endif