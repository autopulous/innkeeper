/*----------------------------------------------------------------------------
  Motel Questa
 
  application programmer's interface (API) header file
  ----------------------------------------------------------------------------
  Copyright 2010-2011 John L. Hart IV. All rights reserved.
 
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

#ifndef MOTEL_QUESTA_I_H
#define MOTEL_QUESTA_I_H

/*----------------------------------------------------------------------------
  An implementation of a double ended queue.
  Designed for use as a linked list, stack or queue.
  ----------------------------------------------------------------------------*/

#include "motel.questa.t.h"

/*****************************************************************************
                      Generic data structure operations
 *****************************************************************************/

/*----------------------------------------------------------------------------
  ValidateQuesta()
  ----------------------------------------------------------------------------
  Test every node within a questa to assure that the structure is correct.
  ----------------------------------------------------------------------------
  Parameters:

  pQuesta - (I) The questa handle
  ----------------------------------------------------------------------------
  Return Values:

  True  - Questa passed the validity check

  False - Questa did not pass the validity check due to:

          1. A node with no data object was detected
		  2. The head of the questa was incorrect
          3. The tail of the questa was incorrect
          4. The next node links were incorrect
          5. The previous node links were incorrect
          6. The node count mismatched the number of next links
          7. The node count mismatched the number of previous links
          8. A memory leak was detected
          9. The questa exceeded its memory size limit
  ----------------------------------------------------------------------------
  Note:

  This function is primarily used for debugging of the motelQuesta module. A
  validity check failure suggests a bug in the motelQuesta implementation.
  ----------------------------------------------------------------------------*/

EXPORT_STORAGE_CLASS success CALLING_CONVENTION ValidateQuesta
(
    motelQuestaHandle pQuesta
);

/*----------------------------------------------------------------------------
  ConstructQuesta()
  ----------------------------------------------------------------------------
  Construct an empty questa.
  ----------------------------------------------------------------------------
  Parameters:

  pQuesta      - (I/O) Pointer to recieve the questa handle
  pMaximumSize - (I)   The maximum number of bytes used by the questa
  pDataSize    - (I)   The number of bytes used by the data object
  ----------------------------------------------------------------------------
  Return Values:

  True  - Questa was succesfully constructed

  False - Questa was not successfully constructed due to:

          1. The SafeMallocBlock() failed
  ----------------------------------------------------------------------------
  Notes:

  The maximum memory paramter may be set to zero to allow the questa to grow
  to the operating system controlled process memory limit.
  ----------------------------------------------------------------------------*/

EXPORT_STORAGE_CLASS success CALLING_CONVENTION ConstructQuesta
(
    motelQuestaHandle * pQuesta,
	size_t pMaximumSize,
	size_t pDataSize
);

/*----------------------------------------------------------------------------
  ResetQuesta()
  ----------------------------------------------------------------------------
  Destruct all nodes.
  ----------------------------------------------------------------------------
  Parameters:

  pQuesta - (I/O) A questa handle
  ----------------------------------------------------------------------------
  Return Values:

  True  - Questa nodes were succesfully destructed

  False - Questa nodes were not successfully destructed due to:

          1. The pQuesta handle pointer was NULL
		  2. The destruction of a node data object failed
          3. The destruction of a node failed
  ----------------------------------------------------------------------------*/

EXPORT_STORAGE_CLASS success CALLING_CONVENTION ResetQuesta
(
    motelQuestaHandle pQuesta
);

/*----------------------------------------------------------------------------
  DestructQuesta()
  ----------------------------------------------------------------------------
  Destruct a questa by destructing all nodes.
  ----------------------------------------------------------------------------
  Parameters:

  pQuesta - (I/O) Pointer to the questa handle
  ----------------------------------------------------------------------------
  Return Values:

  True  - Questa was succesfully destructed

  False - Questa was not successfully destructed due to:

          1. The pQuesta handle pointer was NULL
		  2. The destruction of a node data object failed
          3. The destruction of a node failed
		  4. The destruction of the questa failed
  ----------------------------------------------------------------------------*/

EXPORT_STORAGE_CLASS success CALLING_CONVENTION DestructQuesta
(
    motelQuestaHandle * pQuesta
);

/*----------------------------------------------------------------------------
  SetQuestaMember()
  ----------------------------------------------------------------------------
  Set the value of a published Questa member variable.
  ----------------------------------------------------------------------------
  Parameters:

  pQuesta - (I) A questa handle
  pMember - (I) The member to retrieve
  pValue  - (I) A pointer to memory to receive the member's value
  ----------------------------------------------------------------------------
  Return Values:

  True  - The member variable was succesfully retrieved

  False - The member variable was not successfully retrieved due to:

          1. pValue was NULL
		  2. The pMember value was invalid
  ----------------------------------------------------------------------------
  Notes:

  Check the comments within motel.questa.t.h of the member variable to learn
  the memory and data type requirements of each.

  Sufficient memory must be allocated to retrieve string (char *) member
  variables. These values are string copied (strcpy) to the buffer provided
  by the calling application.

  This function will corrupt user memory when pValue is of insufficient size
  to hold the value of a given member variable.

  Special attention must be paid to assure that pValue points to a variable
  of the appropriate data type to recieive the member variable value.
  ----------------------------------------------------------------------------*/

EXPORT_STORAGE_CLASS success CALLING_CONVENTION SetQuestaMember
(
    motelQuestaHandle pQuesta,
	motelQuestaMember pMember,
	const void * pValue
);

/*----------------------------------------------------------------------------
  GetQuestaMember()
  ----------------------------------------------------------------------------
  Get the value of a published Questa member variable.
  ----------------------------------------------------------------------------
  Parameters:

  pQuesta - (I)   A questa handle
  pMember - (I)   The member variable to retrieve
  pValue  - (I/O) A pointer to memory to receive the member variable's value
  ----------------------------------------------------------------------------
  Return Values:

  True  - The member variable was succesfully retrieved

  False - The member variable was not successfully retrieved due to:

          1. pValue was NULL
		  2. The pMember value was invalid
  ----------------------------------------------------------------------------
  Notes:

  Check the comments within motel.questa.t.h of the member variable to learn
  the memory and data type requirements of each.

  Sufficient memory must be allocated to retrieve string (char *) member
  variables. These values are string copied (strcpy) to the buffer provided by
  the calling application.

  This function will corrupt user memory when pValue is of insufficient size
  to hold the value of a given member variable.

  Special attention must be paid to assure that pValue points to a variable
  of the appropriate data type to recieive the member variable value.
  ----------------------------------------------------------------------------*/

EXPORT_STORAGE_CLASS success CALLING_CONVENTION GetQuestaMember
(
    motelQuestaHandle pQuesta,
	motelQuestaMember pMember,
	void * pValue
);

/*----------------------------------------------------------------------------
  InsertQuestaNode()
  ----------------------------------------------------------------------------
  Insert a node into the questa at the current cursor location.
  ----------------------------------------------------------------------------
  Parameters:

  pQuesta - (I) Questa handle
  pData   - (I) Data object to add to the questa
  ----------------------------------------------------------------------------
  Return Values:

  True  - Node was succesfully inserted

  False - Node was not successfully insert due to:

          1. The pQuesta handle was NULL
          2. The pData handle was NULL
		  3. The questa cursor was NULL
  ----------------------------------------------------------------------------*/

EXPORT_STORAGE_CLASS success CALLING_CONVENTION InsertQuestaNode
(
    motelQuestaHandle pQuesta,
	void * pData
);

/*----------------------------------------------------------------------------
  SelectQuestaNode()
  ----------------------------------------------------------------------------
  Select (find) a node within the questa - changing the cursor location.
  ----------------------------------------------------------------------------
  Parameters:

  pQuesta - (I) Handle of the questa in which the node to delete exists
  pData   - (I) Data object to select from the questa
  ----------------------------------------------------------------------------
  Return Values:

  True  - Node was succesfully selected

  False - Node was not successfully selected due to:

          1. The pQuesta handle was NULL
          2. The pData handle was NULL
		  3. The questa is empty
  ----------------------------------------------------------------------------*/

EXPORT_STORAGE_CLASS success CALLING_CONVENTION SelectQuestaNode
(
    motelQuestaHandle pQuesta,
	void * pData
);

/*----------------------------------------------------------------------------
  FetchQuestaNode()
  ----------------------------------------------------------------------------
  Fetch a node from the questa at the current cursor location.
  ----------------------------------------------------------------------------
  Parameters:

  pQuesta - (I) Handle of the questa in which the node to delete exists
  pData   - (O) Data object to fetch to the questa
  ----------------------------------------------------------------------------
  Return Values:

  True  - Node was succesfully fetched

  False - Node was not successfully fetched due to:

          1. The pQuesta handle was NULL
		  2. The questa is empty
  ----------------------------------------------------------------------------*/

EXPORT_STORAGE_CLASS success CALLING_CONVENTION FetchQuestaNode
(
    motelQuestaHandle pQuesta,
	void * pData
);

/*----------------------------------------------------------------------------
  UpdateQuestaNode()
  ----------------------------------------------------------------------------
  Update a node in the questa at the current cursor location.
  ----------------------------------------------------------------------------
  Parameters:

  pQuesta - (I) Handle of the questa in which the node to delete exists
  pData   - (I) Data object to update to the questa
  ----------------------------------------------------------------------------
  Return Values:

  True  - Node was succesfully updated

  False - Node was not successfully updated due to:

          1. The pQuesta handle was NULL
          2. The pData handle was NULL
		  3. The questa cursor was NULL
  ----------------------------------------------------------------------------*/

EXPORT_STORAGE_CLASS success CALLING_CONVENTION UpdateQuestaNode
(
    motelQuestaHandle pQuesta,
	void * pData
);

/*----------------------------------------------------------------------------
  DeleteQuestaNode()
  ----------------------------------------------------------------------------
  Delete a node from the questa at the current cursor location.
  ----------------------------------------------------------------------------
  Parameters:

  pQuesta - (I) Handle of the questa in which the node to delete exists
  ----------------------------------------------------------------------------
  Return Values:

  True  - Node was succesfully deleted

  False - Node was not successfully deleted due to:

          1. The pQuesta handle was NULL
		  2. The questa cursor was NULL
  ----------------------------------------------------------------------------*/

EXPORT_STORAGE_CLASS success CALLING_CONVENTION DeleteQuestaNode
(
    motelQuestaHandle pQuesta
);

/*****************************************************************************
                          Stack and Queue operations
  *****************************************************************************/

/*----------------------------------------------------------------------------
  PushHeadQuestaNode()
  ----------------------------------------------------------------------------
  Add a data object to the questa at the head/top - aka push.
  ----------------------------------------------------------------------------
  Parameters:

  pQuesta - (I) Handle of the questa
  pData   - (O) Data object to add to the questa
  ----------------------------------------------------------------------------
  Return Values:

  True  - Node was succesfully stacked

  False - Node was not successfully stacked due to:

          1. The pQuesta handle was NULL
		  2. The pData handle was NULL
  ----------------------------------------------------------------------------
  Usage note:
  
  PushHeadQuestaNode() + PullHeadQuestaNode() = Stack
  ----------------------------------------------------------------------------
  This function is logically equivalent to the following macro:

#define StackQuestaNode(pQuesta, pData)                                       \
	    (SetQuestaMember(pQuesta, motelQuestaMember_Head, NULL) ?             \
		(InsertQuestaNode(pQuesta, pData)) :                                  \
		FALSE)
  ----------------------------------------------------------------------------*/

EXPORT_STORAGE_CLASS success CALLING_CONVENTION PushHeadQuestaNode
(
    motelQuestaHandle pQuesta,
	void * pData
);

#define PushTopQuestaNode(pQuesta, pData) PushHeadQuestaNode(pQuesta, pData)
#define StackQuestaNode(pQuesta, pData)   PushHeadQuestaNode(pQuesta, pData)

/*----------------------------------------------------------------------------
  PushTailQuestaNode()
  ----------------------------------------------------------------------------
  Add a data object to the questa at the tail/bottom.
  ----------------------------------------------------------------------------
  Parameters:

  pQuesta - (I) Handle of the questa
  pData   - (O) Data object to add to the questa
  ----------------------------------------------------------------------------
  Return Values:

  True  - Node was succesfully queued

  False - Node was not successfully queued due to:

          1. The pQuesta handle was NULL
		  2. The pData handle was NULL
  ----------------------------------------------------------------------------
  Usage note:
  
  PushTailQuestaNode() + PullHeadQuestaNode() = Queue
  ----------------------------------------------------------------------------
  This function is logically equivalent to the following macro:

#define QueueQuestaNode(pQuesta, pData)                                       \
	    (SetQuestaMember(pQuesta, motelQuestaMember_Tail, NULL) ?             \
		(InsertQuestaNode(pQuesta, pData))                                    \
		: FALSE)
  ----------------------------------------------------------------------------*/

EXPORT_STORAGE_CLASS success CALLING_CONVENTION PushTailQuestaNode
(
    motelQuestaHandle pQuesta,
	void * pData
);

#define PushBottomQuestaNode(pQuesta, pData) PushTailQuestaNode(pQuesta, pData)
#define QueueQuestaNode(pQuesta, pData)      PushTailQuestaNode(pQuesta, pData)

/*----------------------------------------------------------------------------
  PeekHeadQuestaNode()
  ----------------------------------------------------------------------------
  Non-destructively copy the data object at the head/top of the questa.
  ----------------------------------------------------------------------------
  Parameters:

  pQuesta - (I) Handle of the questa
  pData   - (O) Data object in which to copy the data value from the questa
  ----------------------------------------------------------------------------
  Return Values:

  True  - Node was succesfully peeked

  False - Node was not successfully peeked due to:

          1. The pQuesta handle was NULL
		  2. The pData handle pointer was NULL
		  3. The questa is empty
  ----------------------------------------------------------------------------
  This function is logically equivalent to the following macro:

#define PeekHeadQuestaNode(pQuesta, pData)                                    \
	    (SetQuestaMember(pQuesta, motelQuestaMember_Head, NULL) ?             \
		(FetchQuestaNode(pQuesta, pData) :                                    \
		FALSE)
  ----------------------------------------------------------------------------*/

EXPORT_STORAGE_CLASS success CALLING_CONVENTION PeekHeadQuestaNode
(
    motelQuestaHandle pQuesta,
	void * pData
);

#define PeekTopQuestaNode(pQuesta, pData) PeekHeadQuestaNode(pQuesta, pData)

/*----------------------------------------------------------------------------
  PeekTailQuestaNode()
  ----------------------------------------------------------------------------
  Non-destructively copy the data object at the tail/bottom of the questa.
  ----------------------------------------------------------------------------
  Parameters:

  pQuesta - (I) Handle of the questa
  pData   - (O) Data object in which to copy the data value from the questa
  ----------------------------------------------------------------------------
  Return Values:

  True  - Node was succesfully peeked

  False - Node was not successfully peeked due to:

          1. The pQuesta handle was NULL
		  2. The pData handle pointer was NULL
		  3. The questa is empty
  ----------------------------------------------------------------------------
  This function is logically equivalent to the following macro:

#define PeekTailQuestaNode(pQuesta, pData)                                    \
	    (SetQuestaMember(pQuesta, motelQuestaMember_Tail, NULL) ?             \
		(FetchQuestaNode(pQuesta, pData) :                                    \
		FALSE)
  ----------------------------------------------------------------------------*/

EXPORT_STORAGE_CLASS success CALLING_CONVENTION PeekTailQuestaNode
(
    motelQuestaHandle pQuesta,
	void * pData
);

#define PeekBottomQuestaNode(pQuesta, pData) PeekTailQuestaNode(pQuesta, pData)

/*----------------------------------------------------------------------------
  PeekPreviousQuestaNode()
  ----------------------------------------------------------------------------
  Non-destructively copy the data object at the previous node in the questa.

  Calling this function when the cursor is not set will result in the Tail
  node being returned; thus permitting wrap-around traversal of the values in
  the questa.
  ----------------------------------------------------------------------------
  Parameters:

  pQuesta - (I) Handle of the questa
  pData   - (O) Data object in which to copy the data value from the questa
  ----------------------------------------------------------------------------
  Return Values:

  True  - Node was succesfully peeked

  False - Node was not successfully peeked due to:

          1. The pQuesta handle was NULL
		  2. The pData handle pointer was NULL
		  3. The questa is empty
		  4. There is no previous node before the head node
  ----------------------------------------------------------------------------
  This function is logically equivalent to the following macro:

#define PeekPreviousQuestaNode(pQuesta, pData)                                \
	    (SetQuestaMember(pQuesta, motelQuestaMember_Previous, NULL) ?         \
		(FetchQuestaNode(pQuesta, pData) :                                    \
		FALSE)
  ----------------------------------------------------------------------------*/

EXPORT_STORAGE_CLASS success CALLING_CONVENTION PeekPreviousQuestaNode
(
    motelQuestaHandle pQuesta,
	void * pData
);

/*----------------------------------------------------------------------------
  PeekNextQuestaNode()
  ----------------------------------------------------------------------------
  Non-destructively copy the data object at the next node in the questa.

  Calling this function when the cursor is not set will result in the Head
  node being returned; thus permitting wrap-around traversal of the values in
  the questa.
  ----------------------------------------------------------------------------
  Parameters:

  pQuesta - (I) Handle of the questa
  pData   - (O) Data object in which to copy the data value from the questa
  ----------------------------------------------------------------------------
  Return Values:

  True  - Node was succesfully peeked

  False - Node was not successfully peeked due to:

          1. The pQuesta handle was NULL
		  2. The pData handle pointer was NULL
		  3. The questa is empty
		  4. There is no next node after the tail node
  ----------------------------------------------------------------------------
  This function is logically equivalent to the following macro:

#define PeekNextQuestaNode(pQuesta, pData)                                    \
	    (SetQuestaMember(pQuesta, motelQuestaMember_Next, NULL) ?             \
		(FetchQuestaNode(pQuesta, pData) :                                    \
		FALSE)
  ----------------------------------------------------------------------------*/

EXPORT_STORAGE_CLASS success CALLING_CONVENTION PeekNextQuestaNode
(
    motelQuestaHandle pQuesta,
	void * pData
);

/*----------------------------------------------------------------------------
  PullHeadQuestaNode()
  ----------------------------------------------------------------------------
  Destructively copy the data object at the head/top of the questa.
  ----------------------------------------------------------------------------
  Parameters:

  pQuesta - (I) Handle of the questa
  pData   - (O) Data object receive the data object value
  ----------------------------------------------------------------------------
  Return Values:

  True  - Node was succesfully unquestad

  False - Node was not successfully unquestad due to:

          1. The pQuesta handle was NULL
		  2. The pData handle pointer was NULL
		  3. The pData handle pointer was NULL
		  4. The node's data object could not destructed
		  5. The node could not destructed
  ----------------------------------------------------------------------------
  Usage note:
  
  PushHeadQuestaNode() + PullHeadQuestaNode() = Stack
  PushTailQuestaNode() + PullHeadQuestaNode() = Queue
  ----------------------------------------------------------------------------
  This function is logically equivalent to the following macro:

#define PullHeadQuestaNode(pQuesta, pData)                                    \
	    (SetQuestaMember(pQuesta, motelQuestaMember_Head, NULL) ?             \
		(FetchQuestaNode(pQuesta, pData) ?                                    \
		(DeleteQuestaNode(pQuesta))                                           \
		: FALSE) : FALSE)
  ----------------------------------------------------------------------------*/

EXPORT_STORAGE_CLASS success CALLING_CONVENTION PullHeadQuestaNode
(
    motelQuestaHandle pQuesta,
	void * pData
);

#define PullTopQuestaNode(pQuesta, pData) PullHeadQuestaNode(pQuesta, pData)
#define UnstackQuestaNode(pQuesta, pData) PullHeadQuestaNode(pQuesta, pData)
#define DequeueQuestaNode(pQuesta, pData) PullHeadQuestaNode(pQuesta, pData)

/*----------------------------------------------------------------------------
  PullTailQuestaNode()
  ----------------------------------------------------------------------------
  Destructively copy the data object at the tail/bottom of the questa.
  ----------------------------------------------------------------------------
  Parameters:

  pQuesta - (I) Handle of the questa
  pData   - (O) Data object receive the data object value
  ----------------------------------------------------------------------------
  Return Values:

  True  - Node was succesfully unquestad

  False - Node was not successfully unquestad due to:

          1. The pQuesta handle was NULL
          2. The questa is empty
		  3. The pData handle pointer was NULL
		  4. The node's data object could not destructed
		  5. The node could not destructed
  ----------------------------------------------------------------------------
  This function is logically equivalent to the following macro:

#define PullTailQuestaNode(pQuesta, pData)                                    \
	    (SetQuestaMember(pQuesta, motelQuestaMember_Tail, NULL) ?             \
		(FetchQuestaNode(pQuesta, pData) ?                                    \
		(DeleteQuestaNode(pQuesta))                                           \
		: FALSE) : FALSE)
  ----------------------------------------------------------------------------*/

EXPORT_STORAGE_CLASS success CALLING_CONVENTION PullTailQuestaNode
(
    motelQuestaHandle pQuesta,
	void * pData
);

#define PullBottomQuestaNode(pQuesta, pData) PullHeadQuestaNode(pQuesta, pData)

#endif
