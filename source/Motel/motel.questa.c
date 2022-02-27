/*----------------------------------------------------------------------------
  Motel Questa
 
  library implementation file
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

#include "motel.questa.h"

/*----------------------------------------------------------------------------
  Embedded copyright
  ----------------------------------------------------------------------------*/

static const char *gCopyright = "@(#)motel.questa.c - Copyright 2010-2012 John L. Hart IV - All rights reserved";

/*----------------------------------------------------------------------------
  Public functions
  ----------------------------------------------------------------------------*/

EXPORT_STORAGE_CLASS success CALLING_CONVENTION ValidateQuesta
(
    motelQuestaHandle pQuesta
)
{
    motelQuestaNodeHandle lNode;
    motelQuestaNodeHandle lLastVisitedNode;

	unsigned long lSubsequentNodes;
	unsigned long lPreviousNodes;

    /*
    ** there is no questa
    */

	if (NULL == pQuesta)
	{
		return (FALSE);
	}

	/*
	** traverse the questa from head to tail
	*/

	lSubsequentNodes = 0L;

	lLastVisitedNode = (motelQuestaNodeHandle) NULL;

	lNode = pQuesta->head;

	while (NULL != lNode)
	{
		lSubsequentNodes++;

		lLastVisitedNode = lNode;

		if (NULL == lNode->data)
		{
			pQuesta->result = motelResult_NoData;

			return (FALSE);
		}

		lNode = lNode->subsequent;
	}

	/*
	** the traversal did not end at the tail
	*/

	if (lLastVisitedNode != pQuesta->tail)
	{
		pQuesta->result = motelResult_Structure;

		return (FALSE);
	}

	/*
	** traverse the questa from tail to head
	*/

	lPreviousNodes = 0L;

	lLastVisitedNode = (motelQuestaNodeHandle) NULL;

	lNode = pQuesta->tail;

	while (NULL != lNode)
	{
		lPreviousNodes++;

		lLastVisitedNode = lNode;

		if (NULL == lNode->data)
		{
			pQuesta->result = motelResult_NoData;

			return (FALSE);
		}

		lNode = lNode->previous;
	}

	/*
	** the traversal did not end at the head
	*/

	if (lLastVisitedNode != pQuesta->head)
	{
		pQuesta->result = motelResult_Structure;

		return (FALSE);
	}

	/*
	** make sure the nodes counts match
	*/

	if (pQuesta->nodes != lSubsequentNodes || pQuesta->nodes != lPreviousNodes)
	{
		pQuesta->result = motelResult_NodeCount;

		return (FALSE);
	}

	/*
	** make sure the size is correct
	*/

	if (pQuesta->size != sizeof(motelQuesta) + (size_t) (pQuesta->nodes * (long) (sizeof(motelQuestaNode) + pQuesta->dataSize)))
	{
		pQuesta->result = motelResult_MemoryLeak;

		return (FALSE);
	}

	/*
	** make sure the size is within the maximum limit
	*/

	if (UNLIMITED_MEMORY != pQuesta->maximumSize && pQuesta->size > pQuesta->maximumSize)
	{
		pQuesta->result = motelResult_MaximumSize;

		return (FALSE);
	}

	/*
	** the questa is valid
	*/

	pQuesta->result = motelResult_OK;

	return (TRUE);
}

EXPORT_STORAGE_CLASS success CALLING_CONVENTION ConstructQuesta
(
    motelQuestaHandle * pQuesta,
	size_t pMaximumSize,
	size_t pDataSize
)
{
	/*
	** the maximum size is too small to even create the questa control structure
	*/

	if (UNLIMITED_MEMORY != pMaximumSize && sizeof(motelQuesta) > pMaximumSize)
	{
		return (FALSE);
	}
    
	/*
	** allocate the questa control structure
	*/

    if (!SafeMallocBlock((void **) pQuesta, sizeof(motelQuesta)))
	{
		return (FALSE);
	}

	/*
	** initialize the questa control structure
	*/

	(* pQuesta)->result = motelResult_OK;

    (* pQuesta)->nodes = 0;

	(* pQuesta)->maximumSize = pMaximumSize;
	(* pQuesta)->size = sizeof(motelQuesta);

	(* pQuesta)->dataSize = pDataSize;

    (* pQuesta)->head = (motelQuestaNodeHandle) NULL;
    (* pQuesta)->tail = (motelQuestaNodeHandle) NULL;

	(* pQuesta)->cursor = (motelQuestaNodeHandle) NULL;

	return (TRUE);
}

EXPORT_STORAGE_CLASS success CALLING_CONVENTION ResetQuesta
(
    motelQuestaHandle pQuesta
)
{
    motelQuestaNodeHandle lNode;
    motelQuestaNodeHandle lSubsequentNode;

    /*
    ** there is no questa
    */

    if (NULL == pQuesta)
	{
		return (FALSE);
	}

	/*
	** destruct all nodes in the questa
	*/

	for (lNode = pQuesta->head; NULL != lNode; lNode = lSubsequentNode)
    {
        lSubsequentNode = lNode->subsequent;

		/*
		** destruct the node
		*/

		if (!DestructNode(pQuesta, &lNode))
		{
			return (FALSE); // pass through result code
		}

		pQuesta->nodes--;
    }

	/*
	** make sure the node count is now zero
	*/

	if (0 != pQuesta->nodes)
	{
		pQuesta->result = motelResult_NodeCount;

		return (FALSE);
	}

	/*
	** make sure the size is correct
	*/

	if (pQuesta->size > sizeof(motelQuesta))
	{
		pQuesta->result = motelResult_MemoryLeak;

		return (FALSE);
	}

	/*
	** reset state
	*/

    pQuesta->head = (motelQuestaNodeHandle) NULL;
    pQuesta->tail = (motelQuestaNodeHandle) NULL;

	pQuesta->cursor = (motelQuestaNodeHandle) NULL;

	return (TRUE);
}

EXPORT_STORAGE_CLASS success CALLING_CONVENTION DestructQuesta
(
    motelQuestaHandle * pQuesta
)
{
    /*
    ** there is no questa
    */

    if (NULL == pQuesta || NULL == * pQuesta)
	{
		return (FALSE);
	}

	ResetQuesta(* pQuesta);

	/*
	** release the questa control structure
	*/

    SafeFreeBlock((void **) pQuesta);

	return (TRUE);
}

EXPORT_STORAGE_CLASS success CALLING_CONVENTION SetQuestaMember
(
    motelQuestaHandle pQuesta,
	motelQuestaMember pMember,
	const void * pValue
)
{
    /*
    ** there is no questa object
    */

	if (NULL == pQuesta)
	{
		return (FALSE);
	}

	pQuesta->result = motelResult_OK;

	/*
	** node cursor navigation
	*/

	switch (pMember)
	{
		motelQuestaNodeHandle lCurrentNode;

		case motelQuestaMember_Head:

            lCurrentNode = pQuesta->head;

			if (NULL == lCurrentNode)
			{
				pQuesta->result = motelResult_NoNode;
			}

			pQuesta->cursor = lCurrentNode;

			return (TRUE);

		case motelQuestaMember_Tail:

            lCurrentNode = pQuesta->tail;

			if (NULL == lCurrentNode)
			{
				pQuesta->result = motelResult_NoNode;
			}

			pQuesta->cursor = lCurrentNode;

			return (TRUE);

		case motelQuestaMember_Previous:

            lCurrentNode = pQuesta->cursor;

		    if (NULL == lCurrentNode)
			{
				pQuesta->cursor = pQuesta->tail;

				return (TRUE);
			}

			lCurrentNode = lCurrentNode->previous;

			if (NULL == lCurrentNode)
			{
				pQuesta->result = motelResult_NoNode;
			}

			pQuesta->cursor = lCurrentNode;

			return (TRUE);

		case motelQuestaMember_Next:

            lCurrentNode = pQuesta->cursor;

		    if (NULL == lCurrentNode)
			{
				pQuesta->cursor = pQuesta->head;

				return (TRUE);
			}

			lCurrentNode = lCurrentNode->subsequent;

			if (NULL == lCurrentNode)
			{
				pQuesta->result = motelResult_NoNode;
			}

			pQuesta->cursor = lCurrentNode;

			return (TRUE);
	}

	/*
	** set member values
	*/
/*
	if (NULL == pValue)
	{
		pQuesta->result = motelResult_NullPointer;

		return (FALSE);
	}

	switch (pMember)
	{
		default:

            break;
	}
*/
	pQuesta->result = motelResult_InvalidMember;

	return (FALSE);
}

EXPORT_STORAGE_CLASS success CALLING_CONVENTION GetQuestaMember
(
    motelQuestaHandle pQuesta,
	motelQuestaMember pMember,
	void * pValue
)
{
    /*
    ** there is no questa object
    */

	if (NULL == pQuesta)
	{
		return (FALSE);
	}

	/*
	** no return parameter
	*/

	if (NULL == pValue)
	{
		return (FALSE);
	}

	/*
	** result code is a special case because we want to set a result code for GetQuestaMember()
	*/

	if (pMember == motelQuestaMember_Result)
	{
		* (motelResult *) pValue = pQuesta->result;

		pQuesta->result = motelResult_OK;

		return (TRUE);
	}

    /*
    ** get member
    */

	pQuesta->result = motelResult_OK;

	switch (pMember)
	{
		case motelQuestaMember_Nodes:

            * (unsigned long *) pValue = pQuesta->nodes;

			return (TRUE);

		case motelQuestaMember_Size:

            * (size_t *) pValue = pQuesta->size;

			return (TRUE);
	}

	pQuesta->result = motelResult_InvalidMember;

	return (FALSE);
}

EXPORT_STORAGE_CLASS success CALLING_CONVENTION InsertQuestaNode
(
    motelQuestaHandle pQuesta,
	void * pData
)
{
	motelQuestaNodeHandle lNode = (motelQuestaNodeHandle) NULL;

	/*
    ** there is no questa
    */

    if (NULL == pQuesta)
    {
        return (FALSE);
    }

	/*
	** construct the node
	*/

	if (!ConstructNode(pQuesta, &lNode))
	{
		return (FALSE); // pass through result code
	}

	pQuesta->result = motelResult_OK;

	if (NULL == pQuesta->head) // && NULL == pQuesta->tail
	{
		/*
		** insert first node
		*/

		pQuesta->head = lNode;
		pQuesta->tail = lNode;
	}
	else if (NULL == pQuesta->cursor)
	{
		/*
		** insert before the top node
		*/

		lNode->subsequent = pQuesta->head;
		lNode->subsequent->previous = lNode;
		pQuesta->head = lNode;
	}
	else if (pQuesta->tail == pQuesta->cursor)
	{
		/*
		** insert after the bottom node
		*/

		lNode->previous = pQuesta->tail;
		lNode->previous->subsequent = lNode;
		pQuesta->tail = lNode;
	}
	else
	{
		/*
		** insert after the current node
		*/

		lNode->subsequent = pQuesta->cursor->subsequent;
		lNode->subsequent->previous = lNode;
		lNode->previous = pQuesta->cursor;
		pQuesta->cursor->subsequent = lNode;
	}

	pQuesta->nodes++;

	/*
	** copy in data object
	*/

    if (NULL != pData) {
        memcpy((void *)lNode->data, (const void *)pData, pQuesta->dataSize);
    }

	/*
	** set the cursor to the inserted node
	*/

	pQuesta->cursor = lNode;

	return (TRUE);
}

EXPORT_STORAGE_CLASS success CALLING_CONVENTION SelectQuestaNode
(
    motelQuestaHandle pQuesta,
	void * pData
)
{
	/*
    ** there is no questa
    */

    if (NULL == pQuesta)
    {
        return (FALSE);
    }

	pQuesta->result = motelResult_Unimplemented;

	return (FALSE);
}

EXPORT_STORAGE_CLASS success CALLING_CONVENTION FetchQuestaNode
(
    motelQuestaHandle pQuesta,
	void * pData
)
{
	/*
    ** there is no questa
    */

    if (NULL == pQuesta)
    {
        return (FALSE);
    }

	/*
    ** the questa cursor is not validly positioned
    */

    if (NULL == pQuesta->cursor)
    {
		pQuesta->result = motelResult_NoNode;

        return (FALSE);
    }

	pQuesta->result = motelResult_OK;

	/*
	** copy out data object
	*/

	memcpy((void *) pData, (const void *) pQuesta->cursor->data, pQuesta->dataSize);

	return (TRUE);
}

EXPORT_STORAGE_CLASS success CALLING_CONVENTION UpdateQuestaNode
(
    motelQuestaHandle pQuesta,
	void * pData
)
{
	/*
    ** there is no questa
    */

    if (NULL == pQuesta)
    {
        return (FALSE);
    }

	/*
    ** the questa cursor is not validly positioned
    */

    if (NULL == pQuesta->cursor)
    {
		pQuesta->result = motelResult_NoNode;

        return (FALSE);
    }

	pQuesta->result = motelResult_OK;

	/*
	** copy in data object
	*/

	memcpy((void *) pQuesta->cursor->data, (const void *) pData, pQuesta->dataSize);

	return (TRUE);
}

EXPORT_STORAGE_CLASS success CALLING_CONVENTION DeleteQuestaNode
(
    motelQuestaHandle pQuesta
)
{
	/*
    ** there is no questa
    */

    if (NULL == pQuesta)
    {
        return (FALSE);
    }

	/*
    ** the questa is empty
    */

    if (NULL == pQuesta->head || NULL == pQuesta->tail)
    {
		pQuesta->result = motelResult_NoNode;

        return (FALSE);
    }

	/*
    ** the questa cursor is not validly positioned
    */

    if (NULL == pQuesta->cursor)
    {
		pQuesta->result = motelResult_NoNode;

        return (FALSE);
    }

	pQuesta->result = motelResult_OK;

	/*
	** cut node from the linked list
	*/

	if (pQuesta->head == pQuesta->tail) // last node
	{
		pQuesta->head = (motelQuestaNodeHandle) NULL;
		pQuesta->tail = (motelQuestaNodeHandle) NULL;
	}
	else if (pQuesta->head == pQuesta->cursor) // head node
	{
		pQuesta->head = pQuesta->cursor->subsequent;
		pQuesta->cursor->subsequent->previous = (motelQuestaNodeHandle) NULL;
	}
	else if (pQuesta->tail == pQuesta->cursor) // tail node
	{
		pQuesta->tail = pQuesta->cursor->previous;
		pQuesta->cursor->previous->subsequent = (motelQuestaNodeHandle) NULL;
	}
	else // middle node
	{
		pQuesta->cursor->previous->subsequent = pQuesta->cursor->subsequent;
		pQuesta->cursor->subsequent->previous = pQuesta->cursor->previous;
	}

	/*
	** destruct the node
	*/

	if (!DestructNode(pQuesta, &pQuesta->cursor))
	{
		return (FALSE); // pass through result code
	}

	pQuesta->nodes--;

	/*
	** reset the cursor
	*/

	pQuesta->cursor = (motelQuestaNodeHandle) NULL;

	return (TRUE);
}

EXPORT_STORAGE_CLASS success CALLING_CONVENTION PushHeadQuestaNode
(
    motelQuestaHandle pQuesta,
	void * pData
)
{
	motelQuestaNodeHandle lNode = (motelQuestaNodeHandle) NULL;

	/*
    ** there is no questa
    */

    if (NULL == pQuesta)
    {
        return (FALSE);
    }

	/*
    ** there is no data object
    */

	if (NULL == pData)
	{
		pQuesta->result = motelResult_NoData;

		return (FALSE);
	}

	pQuesta->result = motelResult_OK;

	/*
	** construct the node
	*/

	if (!ConstructNode(pQuesta, &lNode))
	{
		return (FALSE); // pass through result code
	}

	/*
	** copy the data object to the node
	*/

	memcpy((void *) lNode->data, (const void *) pData, pQuesta->dataSize);

	/*
	** hook up the node
	*/

	if (NULL == pQuesta->head || NULL == pQuesta->tail) // no nodes
	{
		pQuesta->head = lNode;
		pQuesta->tail = lNode;
	}
	else // insert before the top node
	{
		lNode->subsequent = pQuesta->head;
		lNode->subsequent->previous = lNode;
		pQuesta->head = lNode;
	}

	/*
	** update the questa length
	*/

    pQuesta->nodes++;

	/*
	** set the cursor to the inserted node
	*/

	pQuesta->cursor = lNode;

	return (TRUE);
}

EXPORT_STORAGE_CLASS success CALLING_CONVENTION PushTailQuestaNode
(
    motelQuestaHandle pQuesta,
	void * pData
)
{
	motelQuestaNodeHandle lNode = (motelQuestaNodeHandle) NULL;

	/*
    ** there is no questa
    */

    if (NULL == pQuesta)
    {
        return (FALSE);
    }

	/*
    ** there is no data object
    */

	if (NULL == pData)
	{
		pQuesta->result = motelResult_NoData;

		return (FALSE);
	}

	pQuesta->result = motelResult_OK;

	/*
	** construct the node
	*/

	if (!ConstructNode(pQuesta, &lNode))
	{
		return (FALSE); // pass through result code
	}

	/*
	** copy the data object to the node
	*/

	memcpy((void *) lNode->data, (const void *) pData, pQuesta->dataSize);

	/*
	** hook up the node
	*/

	if (NULL == pQuesta->tail || NULL == pQuesta->head) // no nodes
	{
		pQuesta->head = lNode;
		pQuesta->tail = lNode;
	}
	else
	{
		lNode->previous = pQuesta->tail;
		lNode->previous->subsequent = lNode;
		pQuesta->tail = lNode;
	}

	/*
	** update the questa length
	*/

    pQuesta->nodes++;

	/*
	** set the cursor to the inserted node
	*/

	pQuesta->cursor = lNode;

	return (TRUE);
}

EXPORT_STORAGE_CLASS success CALLING_CONVENTION PeekHeadQuestaNode
(
    motelQuestaHandle pQuesta,
	void * pData
)
{
    /*
    ** there is no questa
    */

    if (NULL == pQuesta)
    {
        return (FALSE);
    }

	pQuesta->result = motelResult_OK;

	/*
	** set the cursor to the head node
	*/

	pQuesta->cursor = pQuesta->head;

	/*
	** the questa is empty
	*/

    if (NULL == pQuesta->cursor)
    {
		pQuesta->result = motelResult_NoNode;

        return (FALSE);
    }

	/*
	** copy out data object
	*/

	memcpy((void *) pData, (const void *) pQuesta->cursor->data, pQuesta->dataSize);

	return (TRUE);
}

EXPORT_STORAGE_CLASS success CALLING_CONVENTION PeekTailQuestaNode
(
    motelQuestaHandle pQuesta,
	void * pData
)
{
    /*
    ** there is no questa
    */

    if (NULL == pQuesta)
    {
        return (FALSE);
    }

	pQuesta->result = motelResult_OK;

	/*
	** set the cursor to the head node
	*/

	pQuesta->cursor = pQuesta->tail;

	/*
	** the questa is empty
	*/

    if (NULL == pQuesta->cursor)
    {
		pQuesta->result = motelResult_NoNode;

        return (FALSE);
    }

	/*
	** copy out data object
	*/

	memcpy((void *) pData, (const void *) pQuesta->cursor->data, pQuesta->dataSize);

	return (TRUE);
}

EXPORT_STORAGE_CLASS success CALLING_CONVENTION PeekPreviousQuestaNode
(
    motelQuestaHandle pQuesta,
	void * pData
)
{
    /*
    ** there is no questa
    */

    if (NULL == pQuesta)
    {
        return (FALSE);
    }

	/*
	** the questa is empty
	*/

    if (NULL == pQuesta->tail)
    {
		pQuesta->result = motelResult_NoNode;

        return (FALSE);
    }

	pQuesta->result = motelResult_OK;

	/*
	** when the cursor is not set, move backward from the tail node
	*/

    if (NULL == pQuesta->cursor)
    {
		pQuesta->cursor = pQuesta->tail;
    }
	else
	{
		pQuesta->cursor = pQuesta->cursor->previous;

		if (NULL == pQuesta->cursor)
		{
			pQuesta->result = motelResult_NoNode;

			return (FALSE);
		}
	}

	/*
	** copy out data object
	*/

	memcpy((void *) pData, (const void *) pQuesta->cursor->data, pQuesta->dataSize);

	return (TRUE);
}

EXPORT_STORAGE_CLASS success CALLING_CONVENTION PeekNextQuestaNode
(
    motelQuestaHandle pQuesta,
	void * pData
)
{
    /*
    ** there is no questa
    */

    if (NULL == pQuesta)
    {
        return (FALSE);
    }

	/*
	** the questa is empty
	*/

    if (NULL == pQuesta->head)
    {
		pQuesta->result = motelResult_NoNode;

        return (FALSE);
    }

	pQuesta->result = motelResult_OK;

	/*
	** when the cursor is not set, move forward from the head node
	*/

    if (NULL == pQuesta->cursor)
    {
		pQuesta->cursor = pQuesta->head;
    }
	else
	{
		pQuesta->cursor = pQuesta->cursor->subsequent;

		if (NULL == pQuesta->cursor)
		{
			pQuesta->result = motelResult_NoNode;

			return (FALSE);
		}
	}

	/*
	** copy out data object
	*/

	memcpy((void *) pData, (const void *) pQuesta->cursor->data, pQuesta->dataSize);

	return (TRUE);
}

EXPORT_STORAGE_CLASS success CALLING_CONVENTION PullHeadQuestaNode
(
    motelQuestaHandle pQuesta,
	void * pData
)
{
    motelQuestaNodeHandle lNode;

    /*
    ** there is no questa
    */

    if (NULL == pQuesta)
    {
        return (FALSE);
    }

	/*
	** there is no place to put the data object
	*/

	if (NULL == pData)
	{
		pQuesta->result = motelResult_NullPointer;

		return (FALSE);
	}

	pQuesta->result = motelResult_OK;

	/*
	** retrieve the head node
	*/

	lNode = pQuesta->head;

	/*
	** the questa is empty
	*/

    if (NULL == lNode)
    {
		pQuesta->result = motelResult_NoNode;

        return (FALSE);
    }

	/*
	** copy out data object
	*/

	memcpy((void *) pData, (const void *) lNode->data, pQuesta->dataSize);

	/*
	** update the node
	*/

	if (pQuesta->head == pQuesta->tail) // last node
	{
		pQuesta->head = (motelQuestaNodeHandle) NULL;
		pQuesta->tail = (motelQuestaNodeHandle) NULL;
	}
	else // head node
	{
		pQuesta->head = lNode->subsequent;
		lNode->subsequent->previous = (motelQuestaNodeHandle) NULL;
	}

	/*
	** destruct the node
	*/

	if (!DestructNode(pQuesta, &lNode))
	{
		return (FALSE); // pass through result code
	}

	pQuesta->nodes--;

	/*
	** reset the node cursor
	*/

	pQuesta->cursor = pQuesta->head;

    return (TRUE);
}

EXPORT_STORAGE_CLASS success CALLING_CONVENTION PullTailQuestaNode
(
    motelQuestaHandle pQuesta,
	void * pData
)
{
    motelQuestaNodeHandle lNode;

    /*
    ** there is no questa
    */

    if (NULL == pQuesta)
    {
        return (FALSE);
    }

	/*
	** there is no place to put the data object
	*/

	if (NULL == pData)
	{
		pQuesta->result = motelResult_NullPointer;

		return (FALSE);
	}

	pQuesta->result = motelResult_OK;

	/*
	** retrieve the head node
	*/

	lNode = pQuesta->tail;

	/*
	** the questa is empty
	*/

    if (NULL == lNode)
    {
		pQuesta->result = motelResult_NoNode;

        return (FALSE);
    }

	/*
	** copy out data object
	*/

	memcpy((void *) pData, (const void *) lNode->data, pQuesta->dataSize);

	/*
	** update questa
	*/

	if (pQuesta->head == pQuesta->tail) // last node
	{
		pQuesta->head = (motelQuestaNodeHandle) NULL;
		pQuesta->tail = (motelQuestaNodeHandle) NULL;
	}
	else // tail node
	{
		pQuesta->tail = lNode->previous;
		lNode->previous->subsequent = (motelQuestaNodeHandle) NULL;
	}

	/*
	** destruct the node
	*/

	if (!DestructNode(pQuesta, &lNode))
	{
		return (FALSE); // pass through result code
	}

	pQuesta->nodes--;

	/*
	** reset the node cursor
	*/

	pQuesta->cursor = pQuesta->tail;

    return (TRUE);
}

/*----------------------------------------------------------------------------
  Private functions
  ----------------------------------------------------------------------------*/

static boolean ConstructNode
(
    motelQuestaHandle pQuesta,
    motelQuestaNodeHandle * pNode
)
{
	pQuesta->result = motelResult_OK;

	/*
	** there isn't room for a new node
	*/

	if (pQuesta->maximumSize > UNLIMITED_MEMORY && pQuesta->maximumSize < pQuesta->size + sizeof(motelQuestaNode) + pQuesta->dataSize)
	{
		pQuesta->result = motelResult_MaximumSize;

		return (FALSE);
	}

	/*
	** allocate memory for the node
	*/

    if (!ManagedMallocBlock((void **) pNode, sizeof(motelQuestaNode), &pQuesta->size))
	{
		pQuesta->result = motelResult_MemoryAllocation;

		return (FALSE);
	}

	/*
	** initialize the node
	*/

    (* pNode)->previous = (motelQuestaNodeHandle) NULL;
    (* pNode)->subsequent = (motelQuestaNodeHandle) NULL;

    (* pNode)->data = (motelQuestaNodeHandle) NULL;

	/*
	** allocate memory for the data object
	*/

	if (!ManagedMallocBlock((void **) &(* pNode)->data, pQuesta->dataSize, &pQuesta->size))
	{
		pQuesta->result = motelResult_MemoryAllocation;

		return (FALSE);
	}

    return (TRUE);
}

static boolean DestructNode
(
    motelQuestaHandle pQuesta,
    motelQuestaNodeHandle * pNode
)
{
	pQuesta->result = motelResult_OK;

	/*
	** release the data object
	*/

	if (!ManagedFreeBlock((void **) &(* pNode)->data, pQuesta->dataSize, &pQuesta->size))
	{
		pQuesta->result = motelResult_MemoryDeallocation;

		return (FALSE);
	}

	/*
	** release the node
	*/

	if (!ManagedFreeBlock((void **) pNode, sizeof(motelQuestaNode), &pQuesta->size))
	{
		pQuesta->result = motelResult_MemoryDeallocation;

		return (FALSE);
	}

	return (TRUE);
}
