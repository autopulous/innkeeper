#include <stdio.h>

#include "../xxx.yyy.zzz/xxx.yyy.zzz.i.h"

#include "../Motel/motel.memory.i.h"

#include "../Motel/motel.aleatory.i.h"

#include "../Motel/motel.string.t.h"
#include "../Motel/motel.string.i.h"

#include "../Command/command.t.h"
#include "../Command/command.i.h"

#include "../Main/types.t.h"
#include "../State/state.i.h"

#define LOCATION_NAME "Barracks"

#define OBJECT_ID "37073B4A-A032-499F-8438-CD99AB70D384"

#define LOCATION_ID "555.555.555"

typedef struct State
{
    Closure caseClosure;
    Level caseCondition;

    motelTreeHandle ItemStates; // location inventory (contents)
} State;

#define INDIRECT_OBJECT_ADJECTIVE "Trophy"
#define INDIRECT_OBJECT_NOUN "Case"

#define CASE_NAME INDIRECT_OBJECT_ADJECTIVE " " INDIRECT_OBJECT_NOUN

#define CASE_SEVERE_DAMAGE_VERBS "ATTACK", "BASH", "SMASH", "KICK"
#define CASE_MINOR_DAMAGE_VERBS "BREAK", "DAMAGE"

#define CASE_DAMAGE_VERBS CASE_SEVERE_DAMAGE_VERBS, CASE_MINOR_DAMAGE_VERBS
#define CASE_REPAIR_VERBS "PATCH", "MEND", "FIX", "REPAIR", "REFINISH", "REBUILD"
#define CASE_OPEN_VERBS "OPEN", "UNSEAL"
#define CASE_CLOSE_VERBS "CLOSE", "SHUT", "SEAL"

#define CASE_VERBS "UNLOCK", "LOCK", CASE_OPEN_VERBS, CASE_CLOSE_VERBS, CASE_DAMAGE_VERBS, CASE_REPAIR_VERBS
#define CASE_ARTICLES "THE"
#define CASE_ADJECTIVES "DISPLAY", "TROPHY"

const char * Verbs[] = { CASE_VERBS, "PUT", "TAKE" };
const char * DirectObjectArticles[] = { CASE_ARTICLES, "" };
const char * DirectObjectAdjectives[] = { CASE_ADJECTIVES, "*", "" };
const char * DirectObjectNouns[] = { "*", "CASE", "" };
const char * Prepositions[] = { "IN", "INTO", "FROM", "WITH", "" };
const char * IndirectObjectArticles[] = { "*", "" };
const char * IndirectObjectAdjectives[] = { "*", "" };
const char * IndirectObjectNouns[] = { "*", "" };

CommandTree Commands = {Verbs, DirectObjectArticles, DirectObjectAdjectives, DirectObjectNouns, Prepositions, IndirectObjectArticles, IndirectObjectAdjectives, IndirectObjectNouns};

/*---------------------------------------------------------------------------
  Private functions
  ---------------------------------------------------------------------------*/

static void StateDefault
(
    State * pState
)
{
    pState->caseClosure = Opened;
    pState->caseCondition = Ten;
}

/*---------------------------------------------------------------------------
  Callback functions
  ---------------------------------------------------------------------------*/

static success CALLING_CONVENTION _StateConstructor
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

    StateDefault((State *) *pState);

    return (TRUE);
}

/*---------------------------------------------------------------------------
  Interface functions
  ---------------------------------------------------------------------------*/

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

void CALLING_CONVENTION LocationReset
(
    Context * pContext
)
{
    if (pContext->Trace) printf(LOCATION_ID ".LocationReset()\n");

    State * lState;

    if (!StateRetriever(pContext, OBJECT_ID, (void **) &lState))
    {
        return;
    }

    StateDefault(lState);
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

    pContext->Valid = FALSE; // assume to be invalid

    /* get the location state */

    State * lState;

    if (!StateRetriever(pContext, OBJECT_ID, (void **) &lState))
    {
        return;
    }

    /* validate case closure state */

    if (Opened != lState->caseClosure && Closed != lState->caseClosure && Locked != lState->caseClosure && Unclosable != lState->caseClosure)
    {
        return;
    }

    /* validate case condition */

    if (Zero > lState->caseCondition || Tweleve < lState->caseCondition)
    {
        return;
    }

    pContext->Valid = TRUE; // proved to be valid
}

boolean CALLING_CONVENTION LocationCommandProcessor
(
    Context * pContext,
	Tokens pTokens
)
{
    if (pContext->Trace) printf(LOCATION_ID ".LocationCommandProcessor()\n");

    State * lState = NULL;

    if (!StateRetriever(pContext, OBJECT_ID, (void **) &lState))
    {
        return (FALSE);
    }

    Command lCommand;

    char * lCommandRemainder = ParseTokenizedCommand(pTokens, Commands, &lCommand);

	if (ClassMatchString(lCommand.Verb, CASE_VERBS, ""))
	{
        if (*CC_STRING_TERMINATOR == *lCommand.DirectObjectArticle)
        {
            CopyString("the", lCommand.DirectObjectArticle, sizeof("the"));
        }
        else if (!ClassMatchString(lCommand.DirectObjectArticle, CASE_ARTICLES, ""))
        {
            return (FALSE);
        }

        if (*CC_STRING_TERMINATOR == *lCommand.DirectObjectAdjective)
		{
			CopyString(INDIRECT_OBJECT_ADJECTIVE, lCommand.DirectObjectAdjective, sizeof(INDIRECT_OBJECT_ADJECTIVE));
		}
		else if (!ClassMatchString(lCommand.DirectObjectAdjective, CASE_ADJECTIVES, ""))
		{
			return (FALSE);
		}

        if (*CC_STRING_TERMINATOR == *lCommand.DirectObjectNoun)
        {
            CopyString(INDIRECT_OBJECT_NOUN, lCommand.DirectObjectNoun, sizeof(INDIRECT_OBJECT_NOUN));
        }
        else if (!EqualStringsi(INDIRECT_OBJECT_NOUN, lCommand.DirectObjectNoun))
        {
            return (FALSE);
        }

		if (EqualStringsi("open", lCommand.Verb))
		{
			if (Unclosable == lState->caseClosure)
			{
				printf("The " CASE_NAME " cannot be opened because it has been destroyed.\n");
			}
			else
			{
				if (Opened == lState->caseClosure)
				{
					printf("The " CASE_NAME " was already open.\n");
				}
				else
				{
                    if (Locked == lState->caseClosure)
                    {
                        printf("You fumble for the key to unlock the " CASE_NAME ".\n");
                        printf("The " CASE_NAME " is now unlocked and open.\n");
                    }
                    else
                    {
                        printf("The " CASE_NAME " is now open.\n");
                    }

                    lState->caseClosure = Opened;

                    done;
                }
			}

			return (TRUE);
		}
		else if (EqualStringsi("close", lCommand.Verb))
		{
			if (Unclosable == lState->caseClosure)
			{
				printf("The " CASE_NAME " cannot be closed because it has been destroyed.\n");
			}
			else
			{
				if (Closed == lState->caseClosure || Locked == lState->caseClosure)
				{
					printf("The " CASE_NAME " was already closed.\n");
				}
				else
				{
                    printf("The " CASE_NAME " is now closed.\n");

                    lState->caseClosure = Closed;

                    done;
                }
			}

			return (TRUE);
		}
		else if (EqualStringsi("unlock", lCommand.Verb))
		{
			if (Unclosable == lState->caseClosure)
			{
				printf("The " CASE_NAME " cannot be unlocked because it has been destroyed.\n");
			}
			else
			{
				if (Closed == lState->caseClosure || Opened == lState->caseClosure)
				{
					printf("The " CASE_NAME " was already unlocked.\n");
				}
				else
				{
                    printf("The " CASE_NAME " is now unlocked.\n");

                    lState->caseClosure = Closed;

                    done;
                }
			}

			return (TRUE);
		}
		else if (EqualStringsi("lock", lCommand.Verb))
		{
			if (Unclosable == lState->caseClosure)
			{
				printf("The " CASE_NAME " cannot be locked because it has been destroyed.\n");
			}
			else
			{
				if (Locked == lState->caseClosure)
				{
					printf("The " CASE_NAME " was already locked.\n");
				}
				else
				{
                    if (Opened == lState->caseClosure)
                    {
                        printf("You fumble for the key to lock the " CASE_NAME ".\n");
                        printf("The " CASE_NAME " is now closed and locked.\n");
                    }
                    else
                    {
                        printf("The " CASE_NAME " is now locked.\n");
                    }

                    lState->caseClosure = Locked;

                    done;
                }
			}

			return (TRUE);
		}
		else if (ClassMatchString(lCommand.Verb, CASE_DAMAGE_VERBS, ""))
		{
            const long double lMessageProbabilities[5] = { 0.10, 0.34, 0.17, 0.22, 0.17 };

            const char * lMinorDamageMessages[5] = { "Ouch", "Thud", "Bump", "Whoa!", "Dink" };
            const char * lMajorDamageMessages[5] = { "CRUNCH!", "SMASH!", "POW!", "BAM!", "BASH!" };

            const char * lMessage;

            size_t lIndex = ProbabilityIndex(sizeof(lMessageProbabilities) / sizeof(long double), lMessageProbabilities);

            if (ClassMatchString(lCommand.Verb, CASE_MINOR_DAMAGE_VERBS, ""))
            {
                lState->caseCondition -= 5;

                lMessage = lMinorDamageMessages[lIndex];
            }
            else
            {
                lState->caseCondition -= 10;

                lMessage = lMajorDamageMessages[lIndex];
            }

            printf("%s\n", lMessage);

            if (Zero > lState->caseCondition)
			{
                lState->caseCondition = Zero;
			}

            switch (lState->caseCondition)
            {
                case Zero:
                    if (Unclosable != lState->caseClosure)
                    {
                        printf("The " CASE_NAME " is destroyed.\n");
                        lState->caseClosure = Unclosable;
                    }
                    else
                    {
                        printf("I hope you're happy...\n");
                        printf("Where once was a magnificent " CASE_NAME " is now a pile of debris.\n");
                    }
                    break;
                case One:
                    printf("The " CASE_NAME " is wobbling badly.\n");
                case Two:
                    printf("The " CASE_NAME " is not very stylish.\n");
                case Three:
                    printf("The " CASE_NAME " is cracked down the center.\n");
                case Four:
                    printf("The " CASE_NAME " is looking very distressed.\n");
                case Five:
                    printf("The " CASE_NAME " has a deep gouge.\n");
                case Six:
                    printf("The " CASE_NAME " has a dent in the side.\n");
                case Seven:
                    printf("The " CASE_NAME " is missing a door.\n");
                case Eight:
                    printf("The " CASE_NAME " is missing a shelf.\n");
                case Nine:
                    printf("The " CASE_NAME " has a broken window.\n");
                    break;
            }

            done;
		}
	}

	return (FALSE);

complete:

    if (!StateUpdater(pContext, OBJECT_ID, &lState))
    {
        return (FALSE);
    }

    return (TRUE);
}

void CALLING_CONVENTION LocationDescriber
(
    Context * pContext,
    _ListAdjacentLocations pListAdjacentLocations
)
{
    if (pContext->Trace) printf(LOCATION_ID ".LocationDescriber()\n");

    State * lState = NULL;

    if (StateRetriever(pContext, OBJECT_ID, (void **) &lState))
    {
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
                printf("White walls are illuminated\nwith a warm pleasing glow from the ceiling\n");

                if (Unclosable == lState->caseClosure)
                {
                    printf("There is a distroyed " CASE_NAME " here\n");
                }
                else if (Opened == lState->caseClosure)
                {
                    printf("There is an open " CASE_NAME " here\n");
                }
                else
                {
                    printf("There is a closed " CASE_NAME " here\n");
                }
            }
        }

        (pListAdjacentLocations)(pContext, AssociationDescriber);
    }
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
