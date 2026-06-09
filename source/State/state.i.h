/*----------------------------------------------------------------------------
  Neoheurist state management header

  application programmer's interface (API) header file
  ---------------------------------------------------------------------------- 
  Copyright 2012-2015, 2026 John L. Hart IV. All rights reserved.
 
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
  Neoheurist state management header file
  ----------------------------------------------------------------------------*/

#ifndef STATE_I_H
#define STATE_I_H

/*----------------------------------------------------------------------------
  StateConstructor()
  ----------------------------------------------------------------------------
  Make the calling location's state live for the current visit. When the
  LocationStates tree already holds a serialized image for pUUId the image is
  rebuilt with pStateDeserializer; otherwise pStateConstructor builds a default.
  ----------------------------------------------------------------------------
  Parameters:

  pContext           - (I) execution context
  pUUId              - (I) the location's object identifier (state tree key)
  pState             - (O) receives the live state pointer
  pStateConstructor  - (I) callback that builds a default state
  pStateDeserializer - (I) callback that rebuilds a state from its JSON image
  ----------------------------------------------------------------------------*/

success CALLING_CONVENTION StateConstructor
(
    Context * pContext,
    const char * pUUId,
    void ** pState,
    success CALLING_CONVENTION pStateConstructor(void **),
    success CALLING_CONVENTION pStateDeserializer(void **, const char *)
);

success CALLING_CONVENTION StateRetriever
(
    Context * pContext,
    const char * pUUId,
    void ** pState
);

success CALLING_CONVENTION StateUpdater
(
    Context * pContext,
    const char * pUUId,
    void ** pState
);

/*----------------------------------------------------------------------------
  StatePersist()
  ----------------------------------------------------------------------------
  Serialize the active visit's live state to its JSON image and store it
  (engine owned) in the LocationStates tree keyed by pUUId, then release the
  live slot. The caller frees the live state itself, because only the location
  knows how to release any nested allocations the state holds.
  ----------------------------------------------------------------------------
  Parameters:

  pContext         - (I) execution context
  pUUId            - (I) the location's object identifier (state tree key)
  pStateSerializer - (I) callback that writes the state's JSON image to a buffer
  ----------------------------------------------------------------------------*/

success CALLING_CONVENTION StatePersist
(
    Context * pContext,
    const char * pUUId,
    success CALLING_CONVENTION pStateSerializer(void *, char *, size_t)
);

#endif
