/*----------------------------------------------------------------------------
  Neoheurist state manager

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

#include "state.h"

success CALLING_CONVENTION StateConstructor
(
    Context * pContext,
    const char * pUUId,
    void ** pState,
    success CALLING_CONVENTION pStateConstructor(void **)
)
{
    /* when state exists there's no need to construct it */

    if (StateRetriever(pContext, pUUId, pState))
    {
        return (TRUE);
    }

    /* use the callback function to create a default state */

    void * lState = NULL;

    if (pStateConstructor(&lState))
        {
        /* save the state pointer in the state tree (cache) */

        if (InsertTreeNode(pContext->LocationStates, &lState, (void *)pUUId))
            {
            *pState = lState;

                return (TRUE);
            }
        }

    *pState = NULL;

    return (FALSE);
}

success CALLING_CONVENTION StateRetriever
(
    Context * pContext,
    const char * pUUId,
    void ** pState
)
{
    /* find the state in the cache */

    if (SelectTreeNode(pContext->LocationStates, (void *) pUUId, 0))
    {
        /* retrieve the state from the cache */

        if (FetchTreeNode(pContext->LocationStates, pState, NULL, NULL))
        {
            return (TRUE);
        }
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
    /* find the state in the cache */

    if (SelectTreeNode(pContext->LocationStates, (void *) pUUId, 0))
    {
        /* alter the state in the cache */

        if (UpdateTreeNode(pContext->LocationStates, pState))
    {
            return (TRUE);
        }
    }

    *pState = NULL;

    return (FALSE);
}