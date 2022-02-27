/*----------------------------------------------------------------------------
  Neoheurist item header

  application programmer's interface (API) header file
  ---------------------------------------------------------------------------- 
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
  Neoheurist item header file
  ----------------------------------------------------------------------------*/

#ifndef XXXXXXXX_XXXX_XXXX_XXXX_XXXXXXXXXXXX_I_H
#define XXXXXXXX_XXXX_XXXX_XXXX_XXXXXXXXXXXX_I_H

#include "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx.t.h"

/*----------------------------------------------------------------------------
  DO NOT REFERENCE THESE FUNCTION PROTOTYPES FROM ANYWHERE OTHER THAN ITEMS
  THAT IMPLEMENT THESE. THE API FOR ENGINES TO INVOKE ITEMS IS DESCRIBED IN
  item.t.h - AS A SERIES OF FUNCTION POINTERS - THIS IS NECESSARY BECAUSE
  ENGINES ARE DIRECTLY RESPONSIBLE FOR LOADING ITEMS AS NEEDED.
  ----------------------------------------------------------------------------*/

void CALLING_CONVENTION ItemConstructor
(
    Context *
);

void CALLING_CONVENTION ItemDestructor
(
    Context *
);

void CALLING_CONVENTION ItemValidator
(
    Context *
);

boolean CALLING_CONVENTION ItemCommandProcessor
(
    Context *,
    Tokens
);

void CALLING_CONVENTION ItemDescriber
(
    Context *
);

#endif