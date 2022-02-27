/*----------------------------------------------------------------------------
Neoheurist character sleep object

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

#include "sleep.h"

const char * Verbs[] = { "Set", "Sleep", "" };
const char * DirectObjectArticles[] = { "The", "" };
const char * DirectObjectAdjectives[] = { "Player", "Character", "" };
const char * DirectObjectNouns[] = { "Alertness", "" };
const char * Prepositions[] = { "To", "For", "" };
const char * IndirectObjectArticles[] = { "" };
const char * IndirectObjectAdjectives[] = { "" };
const char * IndirectObjectNouns[] = { "Halucinating", "Exhausted", "Tired", "Awake", "Alert", "Sharp", "Hyper", "" };

CommandTree Commands = { Verbs, DirectObjectArticles, DirectObjectAdjectives, DirectObjectNouns, Prepositions, IndirectObjectArticles, IndirectObjectAdjectives, IndirectObjectNouns };

/*----------------------------------------------------------------------------
Embedded copyright
----------------------------------------------------------------------------*/

static const char *gCopyright UNUSED = "@(#)sleep.c - Copyright 2015 John L. Hart IV - All rights reserved";

/*----------------------------------------------------------------------------
Public functions
----------------------------------------------------------------------------*/

boolean CALLING_CONVENTION SleepProcessor
(
	Context * pContext,
	Tokens pTokens
)
{
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
