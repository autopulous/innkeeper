#include <stdio.h>
#include <stdlib.h>

#include "../xxx.yyy.zzz/xxx.yyy.zzz.i.h"

#include "../Motel/motel.types.t.h"

#include "../Motel/motel.memory.i.h"

#include "../Motel/motel.string.t.h"
#include "../Motel/motel.string.i.h"

#include "../Motel/motel.questa.t.h"
#include "../Motel/motel.questa.i.h"

#include "../Command/command.t.h"
#include "../Command/command.i.h"

#include "../Main/types.t.h"
#include "../State/state.i.h"

#define LOCATION_NAME "Powder Room"

#define OBJECT_ID "C054BC23-6EE3-420B-8CED-1F872C66EB73"

#define LOCATION_ID "555.554.554"

const Volume LocationCapacity = 100;

typedef struct State
{
    Volume itemsVolume;
    Items itemsPresent;
    motelQuestaHandle itemList;
} State;

const char * Verbs[] = {""};
const char * DirectObjectArticles[] = {""};
const char * DirectObjectAdjectives[] = {""};
const char * DirectObjectNouns[] = {""};
const char * Prepositions[] = {""};
const char * IndirectObjectArticles[] = {""};
const char * IndirectObjectAdjectives[] = {""};
const char * IndirectObjectNouns[] = {""};

CommandTree Commands = {Verbs, DirectObjectArticles, DirectObjectAdjectives, DirectObjectNouns, Prepositions, IndirectObjectArticles, IndirectObjectAdjectives, IndirectObjectNouns};

success CALLING_CONVENTION _StateConstructor
(
    void ** pState
)
{
    /* allocate the state structure from the heap */

    if (!SafeMallocBlock(pState, sizeof(State)))
    {
        return (FALSE);
    }

    /* populate the state structure */

    ((State *) *pState)->itemsVolume = 0;
    ((State *) *pState)->itemsPresent = 0;

    ((State *) *pState)->itemList = NULL;

    if (!ConstructQuesta(&((State *) *pState)->itemList, MEMORY_MAX, sizeof(motelQuestaHandle /* todo: change to item handle type */)))
    {
        SafeFreeBlock(pState);

        return (FALSE);
    }

    if (!InsertQuestaNode(((State *) *pState)->itemList, NULL))
    {
        DestructQuesta(&((State *) *pState)->itemList);
        SafeFreeBlock(pState);

        return (FALSE);
    }

    return (TRUE);
}

static success CALLING_CONVENTION _StateSerializer
(
    void * pState,
    char * pSerialized,
    size_t pSerializedSize
)
{
    State * lState = (State *) pState;

    /* the item list is empty in this release; only the scalars are persisted */

    snprintf(pSerialized, pSerializedSize, "{\"itemsVolume\":%lld,\"itemsPresent\":%llu}", (long long) lState->itemsVolume, (unsigned long long) lState->itemsPresent);

    return (TRUE);
}

static const char * _ReadStateValue
(
    const char * pCursor,
    long long * pValue
)
{
    /* advance to the next decimal digit */

    while (*CC_STRING_TERMINATOR != * pCursor && !IsDecimal(* pCursor))
    {
        pCursor++;
    }

    * pValue = atoll(pCursor);

    /* advance past the digits just consumed */

    while (IsDecimal(* pCursor))
    {
        pCursor++;
    }

    return (pCursor);
}

static success CALLING_CONVENTION _StateDeserializer
(
    void ** pState,
    const char * pSerialized
)
{
    /* build a default state (allocates the empty item list), then restore the scalars */

    if (!_StateConstructor(pState))
    {
        return (FALSE);
    }

    State * lState = (State *) * pState;

    long long lVolume = 0;
    long long lPresent = 0;

    const char * lCursor = pSerialized;

    lCursor = _ReadStateValue(lCursor, &lVolume);
    lCursor = _ReadStateValue(lCursor, &lPresent);

    lState->itemsVolume = (Volume) lVolume;
    lState->itemsPresent = (Items) lPresent;

    return (TRUE);
}

void CALLING_CONVENTION LocationConstructor
(
    Context * pContext
)
{
    if (pContext->Trace) printf(LOCATION_ID ".LocationConstructor()\n");

    State * lState;

    StateConstructor(pContext, OBJECT_ID, (void **) &lState, _StateConstructor, _StateDeserializer);

	LocationValidator(pContext);
}

void CALLING_CONVENTION LocationDestructor
(
    Context * pContext
)
{
    if (pContext->Trace) printf(LOCATION_ID ".LocationDestructor()\n");

    State * lState;

    if (!StateRetriever(pContext, OBJECT_ID, (void **) &lState))
    {
        return;
    }

    StatePersist(pContext, OBJECT_ID, _StateSerializer);

    DestructQuesta(&lState->itemList);

    SafeFreeBlock((void **) &lState);
}

void CALLING_CONVENTION LocationValidator
(
    Context * pContext
)
{
    if (pContext->Trace) printf(LOCATION_ID ".LocationValidator()\n");
    
    pContext->Valid = TRUE;
}

boolean CALLING_CONVENTION LocationCommandProcessor
(
    Context * pContext,
	Tokens pTokens
)
{
    if (pContext->Trace) printf(LOCATION_ID ".LocationCommandProcessor()\n");

    return (FALSE);
}

void CALLING_CONVENTION LocationDescriber
(
    Context * pContext,
    _ListAdjacentLocations pListAdjacentLocations
)
{
    if (pContext->Trace) printf(LOCATION_ID ".LocationDescriber()\n");

    if (SMS == pContext->InteractionInitial)
    {
        printf(LOCATION_NAME "\n");
    }
    else
    {
        if (Verbose <= pContext->InteractionInitial)
        {
			printf("%s finds %s in the ", pContext->Name, pContext->GenderQualifier[pContext->Gender].Reflexive);
        }

        if (Normal <= pContext->InteractionInitial)
        {
            printf("Powder Room\n");
        }

        if (Verbose <= pContext->InteractionInitial)
        {
            printf("Off-white walls are illuminated with\n a warm pleasing glow from the ceiling\n");
			printf("A slightly dusty smell hangs fragrently in the air\n");
            printf("There is a large barrel in the far corner\n");
        }
    }

    (pListAdjacentLocations)(pContext, AssociationDescriber);
}

boolean CALLING_CONVENTION AssociationDescriber
(
    Context * pContext,
	Coordinates * pDestination
)
{
    if (pContext->Trace) printf(LOCATION_ID ".AssociationDescriber()\n");

    if (555 == pDestination->X && 553 == pDestination->Y && 553 == pDestination->Z)
    {
        printf("A descending ladder ");

        if (SMS == pContext->InteractionInitial)
        {
            printf("that is covered with a loose cloth");
        }
        else if (Normal == pContext->InteractionInitial)
        {
            printf("that is covered with a loose and flowing polka dot blanket");
        }
        else if (Verbose == pContext->InteractionInitial)
        {
            printf("that is covered with a mat of thick spider silk that's captured several large red insects");
        }

        printf(" goes Westdown\n");

        return (TRUE);
    }

	if (555 == pDestination->X && 555 == pDestination->Y && 555 == pDestination->Z)
	{
		printf("A passage ");

		if (SMS == pContext->InteractionInitial)
		{
			printf("that is mud choked leads");
		}
		else if (Normal == pContext->InteractionInitial)
		{
			printf("that is blocked by mud and debris apparently washed in by a recent flood seems to lead");
		}
        else if (Verbose == pContext->InteractionInitial)
		{
			printf("that is clogged with logs, branches, twigs, and leaves cemented together by dark red mud that must have been washed in during the recent regional flooding apparently leads");
		}

        printf(" Eastup\n");

        return (TRUE);
    }

	return (FALSE);
}
