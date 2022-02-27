#include <stdio.h>

#include "../xxx.yyy.zzz/xxx.yyy.zzz.i.h"

#include "../Motel/motel.memory.i.h"

#include "../Motel/motel.string.t.h"
#include "../Motel/motel.string.i.h"

#include "../Command/command.t.h"
#include "../Command/command.i.h"

#include "../Gender/gender.t.h"
#include "../Gender/gender.i.h"

#define LOCATION_NAME "Changing room"

#define OBJECT_ID "956C1734-DE27-4147-BEB5-67C01F0E36D3"

#define LOCATION_ID "555.555.553"

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

boolean CALLING_CONVENTION AssignGender
(
    Context * pContext,
    char * pGender
)
{
    Gender lGender = StringToEnum(pGender);

        char *lMessage = "";

        if (Male == lGender)
        {
            if (Male == pContext->Gender)
            {
                lMessage = "Macho macho man... I wanna be a macho man...";
            }
            else
            {
                lMessage = "You bulge and feel tingly all over...";
            }
        }
        else if (Female == lGender)
        {
            if (Female == pContext->Gender)
            {
                lMessage = "I'm just a girl in the world... Oh, I'm just a girl, lucky me...";
            }
            else
            {
                lMessage = "You blossom amd feel all tingly inside...";
            }
        }
        else if (Person == lGender)
        {
            if (Person == pContext->Gender)
            {
                lMessage = "If not us, then who? If not now, then when?";
            }
            else
            {
                lMessage = "You're not quite sure how you feel...";
            }
        }
        else if (Robot  == lGender)
        {
            if (Robot == pContext->Gender)
            {
                lMessage = "Why so personal about these things?";
            }
            else
            {
                lMessage = "It's only logical...";
            }
        }

        printf ("%s\n", lMessage);

        pContext->Gender = lGender;

        return (TRUE);
    }

boolean CALLING_CONVENTION LocationCommandProcessor
(
    Context * pContext,
    Tokens pTokens
)
{
    if (pContext->Trace) printf(LOCATION_ID ".LocationCommandProcessor()\n");
    
    Command lCommand;
    char * lCommandRemainder;

    const char * lVerbs[] = {""};
    const char * lDirectObjectArticles[] = {""};
    const char * lDirectObjectAdjectives[] = {""};
    const char * lDirectObjectNouns[] = {""};
    const char * lPrepositions[] = {""};
    const char * lIndirectObjectArticles[] = {""};
    const char * lIndirectObjectAdjectives[] = {""};
    const char * lIndirectObjectNouns[] = {""};

    {
        const char * lVerbs[] = {"Become", ""};
        const char * lIndirectObjectAdjectives[] = {GENDER_WORDS, ""};

        CommandTree Commands = {lVerbs, lDirectObjectArticles, lDirectObjectAdjectives, lDirectObjectNouns, lPrepositions, lIndirectObjectArticles, lIndirectObjectAdjectives, lIndirectObjectNouns};

        lCommandRemainder = ParseTokenizedCommand(pTokens, Commands, &lCommand);

        if (*CC_STRING_TERMINATOR != *lCommand.Verb && *CC_STRING_TERMINATOR != *lCommand.IndirectObjectAdjective && NULL == lCommandRemainder)
        {
            return (AssignGender(pContext, lCommand.IndirectObjectAdjective));
       }
    }

    {
        const char * lVerbs[] = {STATE_SET_WORDS, ""};
        const char * lDirectObjectAdjectives[] = {POSSESSIVE_WORDS, ""};
        const char * lDirectObjectNouns[] = {"Gender", ""};
        const char * lPrepositions[] = {"As", "To", ""};
        const char * lIndirectObjectAdjectives[] = {GENDER_WORDS, ""};

        CommandTree Commands = {lVerbs, lDirectObjectArticles, lDirectObjectAdjectives, lDirectObjectNouns, lPrepositions, lIndirectObjectArticles, lIndirectObjectAdjectives, lIndirectObjectNouns};

        lCommandRemainder = ParseTokenizedCommand(pTokens, Commands, &lCommand);

        if (*CC_STRING_TERMINATOR != *lCommand.Verb && *CC_STRING_TERMINATOR != *lCommand.DirectObjectNoun && *CC_STRING_TERMINATOR != *lCommand.IndirectObjectAdjective && NULL == lCommandRemainder)
        {
            return (AssignGender(pContext, lCommand.IndirectObjectAdjective));
       }
    }

    /* Change the user name */

    {
        const char * lVerbs[] = {STATE_SET_WORDS, ""};
        const char * lDirectObjectAdjectives[] = {POSSESSIVE_WORDS, ""};
        const char * lDirectObjectNouns[] = {"Name", ""};
        const char * lPrepositions[] = {"As", "To", ""};
        const char * lIndirectObjectAdjectives[] = {""};

        CommandTree Commands = {lVerbs, lDirectObjectArticles, lDirectObjectAdjectives, lDirectObjectNouns, lPrepositions, lIndirectObjectArticles, lIndirectObjectAdjectives, lIndirectObjectNouns};

        lCommandRemainder = ParseTokenizedCommand(pTokens, Commands, &lCommand);

        if (*CC_STRING_TERMINATOR != *lCommand.Verb && *CC_STRING_TERMINATOR != *lCommand.DirectObjectNoun && NULL != lCommandRemainder)
        {
            CopyString(lCommandRemainder, pContext->Name, offsetof(Context, InteractionInitial)-offsetof(Context, Name));

            printf("OK %s\n", pContext->Name);

            return (TRUE);
       }
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
            printf("White walls are illuminated with\na warm pleasing glow from the ceiling\n");
            printf("On the far wall there is a Full Length Mirror\n");
			printf("You pause to admire your Magnificence\n");
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
