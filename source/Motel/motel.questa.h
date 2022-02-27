/*----------------------------------------------------------------------------
  Motel Questa
 
  private header file
  ----------------------------------------------------------------------------
  Copyright 2010-2012 John L. Hart IV. All rights reserved.
 
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

#ifndef MOTEL_QUESTA_H
#define MOTEL_QUESTA_H

#define MUTABILITY

#include <memory.h>

#include "../Motel/motel.compilation.t.h"
#include "../Motel/motel.types.t.h"
#include "../Motel/motel.results.t.h"

#include "motel.memory.t.h"
#include "motel.memory.i.h"

/*----------------------------------------------------------------------------
  Public macros and data types
  ----------------------------------------------------------------------------*/

#include "motel.questa.t.h"

/*----------------------------------------------------------------------------
  Private macros 
  ----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
  Private data types
  ----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
  Public function prototypes
  ----------------------------------------------------------------------------*/

#include "motel.questa.i.h"

/*----------------------------------------------------------------------------
  Private function prototypes
  ----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
  ConstructNode()
  ----------------------------------------------------------------------------
  Construct a questa node.
  ----------------------------------------------------------------------------
  Parameters:

  pQuesta    - (I)   Questa handle
  pNode     - (I/O) Pointer to recieve the node handle
  ----------------------------------------------------------------------------
  Return Values:

  True  - Node was succesfully constructed

  False - Node was not successfully constructed due to:

          1. The questa handle was invalid
		  2. ManagedMallocBlock() failed
		  3. Would make the questa exceed its maximum number of bytes
  ----------------------------------------------------------------------------
  Notes:

  The memory allocated for a node, its key object and its data object are
  managed by the questa associated to the node. Pushing and pullping of nodes
  from the associated questa does nothing to the memory allocation. As a result
  nodes may be reused (without reconstruction) after being pullped.

  Destructing a questa will destruct all associated nodes and data objects.
  ----------------------------------------------------------------------------*/

static boolean ConstructNode
(
    motelQuestaHandle pQuesta,
    motelQuestaNodeHandle * pNode
);

/*----------------------------------------------------------------------------
  DestructNode()
  ----------------------------------------------------------------------------
  Destruct a node.
  ----------------------------------------------------------------------------
  Parameters:

  pQuesta - (I)   Handle of the questa to which the node is associated
  pNode  - (I/O) Handle of the node remove
  ----------------------------------------------------------------------------
  Return Values:

  True  - Node was succesfully destructed

  False - Node was not successfully destructed due to:

          1. The pQuesta handle was NULL
          2. The pNode handle pointer was NULL
          3. The pData handle pointer was NULL
          4. The data destruction failed
          5. The node destruction failed
  ----------------------------------------------------------------------------*/

static boolean DestructNode
(
    motelQuestaHandle pQuesta,
    motelQuestaNodeHandle * pNode
);

#endif