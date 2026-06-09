/*----------------------------------------------------------------------------
  Neoheurist state manager

Copyright 2015, 2026 John L. Hart IV. All rights reserved.

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

#include "state.h"

/* the largest JSON image a location may serialize for a single visit */

#define STATE_SERIALIZED_SIZE (size_t) 1024

success CALLING_CONVENTION StateConstructor
(
    Context * pContext,
    const char * pUUId,
    void ** pState,
    success CALLING_CONVENTION pStateConstructor(void **),
    success CALLING_CONVENTION pStateDeserializer(void **, const char *)
)
{
    /* a live state is already active for this visit */

    if (NULL != pContext->LocationState)
    {
        *pState = pContext->LocationState;

        return (TRUE);
    }

    void * lState = NULL;

    /* rebuild from the durable JSON image when one exists */

    if (SelectTreeNode(pContext->LocationStates, (void *) pUUId, 0))
    {
        char * lSerialized = NULL;

        if (FetchTreeNode(pContext->LocationStates, &lSerialized, NULL, NULL) && NULL != lSerialized)
        {
            if (!pStateDeserializer(&lState, lSerialized))
            {
                *pState = NULL;

                return (FALSE);
            }
        }
    }

    /* otherwise build a default state */

    if (NULL == lState)
    {
        if (!pStateConstructor(&lState))
        {
            *pState = NULL;

            return (FALSE);
        }
    }

    pContext->LocationState = lState;

    *pState = lState;

    return (TRUE);
}

success CALLING_CONVENTION StateRetriever
(
    Context * pContext,
    const char * pUUId,
    void ** pState
)
{
    /* return the active visit's live state (only one location is live at a time) */

    if (NULL != pContext->LocationState)
    {
        *pState = pContext->LocationState;

        return (TRUE);
    }

    *pState = NULL;

    return (FALSE);
}

success CALLING_CONVENTION StateUpdater
(
    Context * pContext,
    const char * pUUId,
    void ** pState
)
{
    /* the live state is mutated in place; the durable image is written on exit */

    if (NULL != pContext->LocationState)
    {
        *pState = pContext->LocationState;

        return (TRUE);
    }

    *pState = NULL;

    return (FALSE);
}

success CALLING_CONVENTION StatePersist
(
    Context * pContext,
    const char * pUUId,
    success CALLING_CONVENTION pStateSerializer(void *, char *, size_t)
)
{
    void * lState = pContext->LocationState;

    /* nothing active to persist */

    if (NULL == lState)
    {
        return (TRUE);
    }

    /* release engine ownership of the live slot up front (the caller frees lState) */

    pContext->LocationState = NULL;

    /* serialize the live state to its JSON image */

    char lSerialized[STATE_SERIALIZED_SIZE];

    if (!pStateSerializer(lState, lSerialized, sizeof(lSerialized)))
    {
        return (FALSE);
    }

    /* copy the image into an engine-owned heap block */

    char * lImage = NULL;

    if (!SafeMallocBlock((void **) &lImage, STATE_SERIALIZED_SIZE))
    {
        return (FALSE);
    }

    CopyString(lSerialized, lImage, STATE_SERIALIZED_SIZE);

    /* store the image in the durable tree, replacing any prior image */

    if (SelectTreeNode(pContext->LocationStates, (void *) pUUId, 0))
    {
        char * lPrevious = NULL;

        FetchTreeNode(pContext->LocationStates, &lPrevious, NULL, NULL);

        if (!UpdateTreeNode(pContext->LocationStates, &lImage))
        {
            SafeFreeBlock((void **) &lImage);

            return (FALSE);
        }

        if (NULL != lPrevious)
        {
            SafeFreeBlock((void **) &lPrevious);
        }

        return (TRUE);
    }

    if (!InsertTreeNode(pContext->LocationStates, &lImage, (void *) pUUId))
    {
        SafeFreeBlock((void **) &lImage);

        return (FALSE);
    }

    return (TRUE);
}