/*----------------------------------------------------------------------------
  Motel Input Output
  
  application programmer's interface (API) header file
  ---------------------------------------------------------------------------- 
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

#ifndef MOTEL_IO_I_H
#define MOTEL_IO_I_H

// todo: find a reason for this file or eliminate it

#include "motel.compilation.t.h"

/*----------------------------------------------------------------------------
  MotelOutputPush()
  ----------------------------------------------------------------------------
  Sends a byte through an output stream.
  ----------------------------------------------------------------------------
  Parameters:

  pInput - (I) Pointer to the output stream
  pByte  - (I) The byte to send
  ----------------------------------------------------------------------------
  Returns:

  ---------------------------------------------------------------------------*/

typedef boolean (CALLING_CONVENTION MotelOutputPush)
(
   motelOutputHandle,
   byte
);

/*----------------------------------------------------------------------------
  MotelInputPeek()
  ----------------------------------------------------------------------------
  Return the byte pointed to by the cursor.
  ----------------------------------------------------------------------------
  Parameters:

  pInput - (I) Pointer to the input stream
  pByte  - (O) Pointer to receive the byte at the cursor
  ----------------------------------------------------------------------------
  Returns:

  The byte at the cursor
  ---------------------------------------------------------------------------*/

typedef boolean (CALLING_CONVENTION MotelInputPeek)
(
   motelInputHandle,
   byte *
);

/*----------------------------------------------------------------------------
  MotelInputPull()
  ----------------------------------------------------------------------------
  Receives a byte through an input stream.
  ----------------------------------------------------------------------------
  Parameters:

  pInput - (I) Pointer to the input stream
  pByte  - (O) Pointer to the byte transfer buffer
  ----------------------------------------------------------------------------
  Returns:

  The next byte from the input stream
  ---------------------------------------------------------------------------*/

typedef boolean (CALLING_CONVENTION MotelInputPull)
(
   motelInputHandle,
   byte *
);

#endif
