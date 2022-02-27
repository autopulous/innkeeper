#include "555.553.553.h"

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

void CALLING_CONVENTION LocationConstructor
(
    Context * pContext
)
{
    if (pContext->Trace) printf(LOCATION_ID ".LocationConstructor()\n");

    State * lState;

    StateConstructor(pContext, OBJECT_ID, (void **) &lState, _StateConstructor);

    LocationValidator(pContext);
}

void CALLING_CONVENTION LocationDestructor
(
    Context * pContext
)
{
    if (pContext->Trace) printf(LOCATION_ID ".LocationDestructor()\n");
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
            printf("High Powder Room\n");
       }

        if (Verbose <= pContext->InteractionInitial)
        {
            printf("Off-white walls are illuminated with\n a warm pleasing glow from the ceiling\n");
			printf("A slightly dusty smell hangs fragrently in the air\n");
            printf("There are ten small barrels, a wicker chair, and sink in the near corner\n");
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

	if (555 == pDestination->X && 554 == pDestination->Y && 554 == pDestination->Z)
	{
		printf("An ascending ladder ");

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

        printf(" goes Eastup\n");

        return (TRUE);
    }

	return (FALSE);
}
