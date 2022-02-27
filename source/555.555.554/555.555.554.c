#include <stdio.h>

#include "../xxx.yyy.zzz/xxx.yyy.zzz.i.h"

#include "../Motel/motel.string.t.h"
#include "../Motel/motel.string.i.h"

#include "../Command/command.t.h"
#include "../Command/command.i.h"

#include "../Sleep/sleep.t.h"
#include "../Sleep/sleep.i.h"

#define LOCATION_NAME "Tiny bedroom"

#define OBJECT_ID "6BF590F2-E942-4B99-8888-A7E2F13FA51A"

#define LOCATION_ID "555.555.554"

const char * Verbs[] = { "Set", "Sleep", "" };
const char * DirectObjectArticles[] = { "The", "" };
const char * DirectObjectAdjectives[] = { "Player", "Character", "" };
const char * DirectObjectNouns[] = { "Alertness", "" };
const char * Prepositions[] = { "To", "For", "" };
const char * IndirectObjectArticles[] = { "" };
const char * IndirectObjectAdjectives[] = { "" };
const char * IndirectObjectNouns[] = { "Halucinating", "Exhausted", "Tired", "Awake", "Alert", "Sharp", "Hyper", "" };

CommandTree Commands = { Verbs, DirectObjectArticles, DirectObjectAdjectives, DirectObjectNouns, Prepositions, IndirectObjectArticles, IndirectObjectAdjectives, IndirectObjectNouns };

void CALLING_CONVENTION LocationConstructor
(
    Context * pContext
)
{
    if (pContext->Trace) printf(LOCATION_ID ".LocationConstructor()\n");

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

	if (SleepProcessor(pContext, pTokens))
	{
		return (TRUE);
	}

	Command lCommand;

	char * lCommandRemainder = ParseTokenizedCommand(pTokens, Commands, &lCommand);

	if (pContext->Debug)
	{
		if (EqualStringsi("Set", lCommand.Verb) && EqualStringsi("Alertness", lCommand.DirectObjectNoun) && EqualStringsi("To", lCommand.Preposition))
		{
			if (EqualStringsi("Halucinating", lCommand.IndirectObjectNoun))
			{
				pContext->Alertness = Zero;
			}
			else if (EqualStringsi("Exhausted", lCommand.IndirectObjectNoun))
			{
				pContext->Alertness = Two;
			}
			else if (EqualStringsi("Tired", lCommand.IndirectObjectNoun))
			{
				pContext->Alertness = Four;
			}
			else if (EqualStringsi("Awake", lCommand.IndirectObjectNoun))
			{
				pContext->Alertness = Six;
			}
			else if (EqualStringsi("Alert", lCommand.IndirectObjectNoun))
			{
				pContext->Alertness = Eight;
			}
			else if (EqualStringsi("Sharp", lCommand.IndirectObjectNoun))
			{
				pContext->Alertness = Ten;
			}
			else if (EqualStringsi("Hyper", lCommand.IndirectObjectNoun))
			{
				pContext->Alertness = Tweleve;
			}
			else if (NULL != lCommandRemainder)
			{
				int alertness = atoi(lCommandRemainder);

				pContext->Alertness = alertness;
			}
			else
			{
				return (FALSE);
			}

			printf("Alertness equals: %d\n", pContext->Alertness);
			return (TRUE);
		}
	}
	
	if (EqualStringsi("Sleep", lCommand.Verb))
	{
		if (Eight <= pContext->Alertness)
		{
			printf("You drift...\nyou float...\nbut no matter how much you try you fail to fall asleep...\n");
		}
		else
		{
			printf("Zzzzzzzzz...\n");

			int sleepDuration;

			if ((EqualStringsi("For", lCommand.Preposition) || *CC_STRING_TERMINATOR == *lCommand.Preposition) && NULL != lCommandRemainder)
			{
				int requestedSleepDuration = atoi(lCommandRemainder);

				if (Eight < pContext->Alertness + requestedSleepDuration)
				{
					sleepDuration = Eight - pContext->Alertness;
				}
				else
				{
					sleepDuration = requestedSleepDuration;
				}
			}
			else
			{
				sleepDuration = Eight - pContext->Alertness;
			}

			pContext->Alertness += sleepDuration;

			printf("you slept for %d units... ", sleepDuration);

			if (Tweleve <= pContext->Alertness)
			{
				printf("and are now hyperaware!\n");
			}
			else if (Ten <= pContext->Alertness)
			{
				printf("and are now sharp!\n");
			}
			else if (Eight <= pContext->Alertness)
			{
				printf("and are now alert.\n");
			}
			else if (Six <= pContext->Alertness)
			{
				printf("and are now awake and mellow.\n");
			}
			else if (Four <= pContext->Alertness)
			{
				printf("and are now awake but tired.\n");
			}
			else if (Two <= pContext->Alertness)
			{
				printf("and are now exhausted and dragging!\n");
			}
			else // if (Halucinating <= pContext->Alertness)
			{
				printf("and are now seeing things that are not there!\n");
			}

			if (pContext->Debug)
			{
				printf("Alertness equals: %d\n", pContext->Alertness);
			}
		}

		return (TRUE);
	}

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
            printf(LOCATION_NAME "\n");
        }

        if (Verbose <= pContext->InteractionInitial)
        {
            printf("You feel sleepy just entering this location\n");
            printf("There is bed here\n");
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

	return (FALSE);
}
