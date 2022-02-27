/*----------------------------------------------------------------------------
  Neoheurist engine

  Copyright 2011-2012 John L. Hart IV. All rights reserved.

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

/*----------------------------------------------------------------------------
  Neoheurist engine implementation file
  ----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
  Standard libraries
  ----------------------------------------------------------------------------*/

#include <stdio.h>
#include <time.h>

#include <windows.h>

/*----------------------------------------------------------------------------
  Public data types
  ----------------------------------------------------------------------------*/

#include "motel.compilation.t.h"
#include "motel.types.t.h"

#include "motel.string.t.h"
#include "motel.string.i.h"

#include "command.t.h"
#include "command.i.h"

#include "engine.t.h"
#include "rule.t.h"

/*----------------------------------------------------------------------------
  Public functions
  ----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
  Private defines, data types and function prototypes
  ----------------------------------------------------------------------------*/

#define LIBRARY_EXTENSION ".dll"

#define CONTEXT_NAME "Acaj"
#define NOWHERE {000, 000, 000}
#define HOME {555, 555, 555}

/*----------------------------------------------------------------------------
  Embedded copyright
  ----------------------------------------------------------------------------*/

static const char *gCopyright UNUSED = "@(#)engine.c - Copyright 2011-2012 John L. Hart IV - All rights reserved";

/*----------------------------------------------------------------------------
  :Eeek:Globals
  ----------------------------------------------------------------------------*/

const Coordinates gNowhere = NOWHERE;
const Coordinates gHome = HOME;

/*----------------------------------------------------------------------------
  User message functions
  ----------------------------------------------------------------------------*/

void DisplayOptions
(
    void
)
{
    printf("\n"
           "Options:\n"
           "\n"
           "NWD - Go Northwestdown\n"
           "NW  - Go Northwest\n"
           "NWU - Go Northwestup\n"
           "WD  - Go Westdown\n"
           "W   - Go West\n"
           "WU  - Go Westup\n"
           "SWD - Go Southwestdown\n"
           "SW  - Go Southwest\n"
           "SWU - Go Southwestup\n"
           "ND  - Go Northdown\n"
           "N   - Go North\n"
           "NU  - Go Northup\n"
           "D   - Go Down\n"
           "U   - Go Up\n"
           "SD  - Go Southdown\n"
           "S   - Go South\n"
           "SU  - Go Southup\n"
           "NED - Go Northeastdown\n"
           "NE  - Go Northeast\n"
           "NEU - Go Northeastup\n"
           "ED  - Go Eastdown\n"
           "E   - Go East\n"
           "EU  - Go Eastup\n"
           "SED - Go Southeastdown\n"
           "SE  - Go Southeast\n"
           "SEU - Go Southeastup\n"
           "\n"
           "[L]ook      - Describe rule\n"
           "Describe    - Describe rule\n"
           "[V]alidate  - Validate rule\n"
           "[VERBOSITY] - Toggle verbosity mode\n"
           "[DEBUG]     - Toggle debug mode\n"
           "\n"
           "[R]eset     - Reset current rule\n"
           "\n"
           "[Q]uit - Quit\n"
           "E[x]it - Exit\n"
           "[H]elp - Display this option list\n"
           "\n");
}

void Emptiness
(
    void
)
{
    printf("With great sorrow the destination you seek fails to appear...\n\n"
           "Apparently there is something wrong with the Universe...\n\n"
           "Perhaps it's your fault or perhaps it's mine...\n\n"
           "You are confronted with a void beyond imagining, you react by\n"
           "pointing mindlessly into the distance, aghast at the incomprehensible\n"
           "emptiness beyond.\n\n"
           "After a moment you gather your wits and finally recognize your situation,\n"
           "you are who you were and you are where you were...\n\n");
}

void UltimateEmptiness
(
    void
)
{
    printf("and that's nowhere...\n\n"
           "The universe does not welcome your presence, be gone!\n\n");
}

EXPORT_STORAGE_CLASS void CALLING_CONVENTION ListAdjacentRules
(
    Associations pAssociations
)
{
    int lX, lY, lZ;

    Coordinates * lAssociation;

    char * directionNames[3][3][3] = {{{"Northwestdown", "Northwest", "Northwestup"}, {"Westdown", "West", "Westup"}, {"Southwestdown", "Southwest", "Southwestup"}},
                                      {{"Northdown",     "North",     "Northup"},     {"Down",     " ",    "Up"},     {"Southdown",     "South",     "Southup"}},
                                      {{"Northeastdown", "Northeast", "Northeastup"}, {"Eastdown", "East", "Eastup"}, {"Southeastdown", "Southeast", "Southeastup"}}};

    /*
    ** scan for associations
    */

    for (lX = WEST; lX <= EAST; lX++)
    {
        for (lY = NORTH; lY <= SOUTH; lY++)
        {
            for (lZ = DOWN; lZ <= UP; lZ++)
            {
                if (NAUGHT != lX || NAUGHT != lY || NAUGHT != lZ)
                {
                    lAssociation = &pAssociations[lX][lY][lZ];

                    if (0 != lAssociation->X && 0 != lAssociation->Y && 0 != lAssociation->Z)
                    {
                        printf("An association leads %s\n", directionNames[lX][lY][lZ]);
                    }
                }
            }
        }
    }
}

/*----------------------------------------------------------------------------
  Rule interface functions (debug wrappers)
  ----------------------------------------------------------------------------*/

void ConstructRule
(
    Context * pContext,
    _RuleConstructor pRuleConstructor
)
{
    if (pContext->Debug)
    {
        fprintf(stderr, "Rule %d.%d.%d constructing\n", pContext->Rule.X, pContext->Rule.Y, pContext->Rule.Z);
    }

    (* pRuleConstructor)(pContext);

    if (pContext->Debug)
    {
        fprintf(stderr, "Rule %d.%d.%d constructed\n", pContext->Rule.X, pContext->Rule.Y, pContext->Rule.Z);
    }
}

void DestructRule
(
    Context * pContext,
    _RuleDestructor pRuleDestructor
)
{
    if (pContext->Debug)
    {
        fprintf(stderr, "Rule %d.%d.%d destructing\n", pContext->Rule.X, pContext->Rule.Y, pContext->Rule.Z);
    }

    (* pRuleDestructor)(pContext);

    if (pContext->Debug)
    {
        fprintf(stderr, "Rule %d.%d.%d destructed\n", pContext->Rule.X, pContext->Rule.Y, pContext->Rule.Z);
    }
}

void ValidateRule
(
    Context * pContext,
    _RuleValidator pRuleValidator
)
{
    if (pContext->Debug)
    {
        fprintf(stderr, "Rule %d.%d.%d validating\n", pContext->Rule.X, pContext->Rule.Y, pContext->Rule.Z);
    }

    (* pRuleValidator)(pContext);

    if (pContext->Debug)
    {
        fprintf(stderr, "Rule %d.%d.%d validated\n", pContext->Rule.X, pContext->Rule.Y, pContext->Rule.Z);
    }
}

void DescribeRule
(
    Context * pContext,
    _RuleDescriber pRuleDescriber
)
{
    if (pContext->Debug)
    {
        fprintf(stderr, "Rule %d.%d.%d describing\n", pContext->Rule.X, pContext->Rule.Y, pContext->Rule.Z);
    }

    (* pRuleDescriber)(pContext, ListAdjacentRules);

    if (pContext->Debug)
    {
        fprintf(stderr, "Rule %d.%d.%d described\n", pContext->Rule.X, pContext->Rule.Y, pContext->Rule.Z);
    }
}

bool ProcessCommand
(
    Context * pContext,
    _RuleCommandProcessor pRuleCommandProcessor,
    char * pCommand
)
{
    bool lResult;

    if (pContext->Debug)
    {
        fprintf(stderr, "Rule %d.%d.%d processing command\n", pContext->Rule.X, pContext->Rule.Y, pContext->Rule.Z);
    }

    lResult = (* pRuleCommandProcessor)(pContext, pCommand);

    if (pContext->Debug)
    {
        fprintf(stderr, "Rule %d.%d.%d processed command\n", pContext->Rule.X, pContext->Rule.Y, pContext->Rule.Z);
    }

    return(lResult);
}

void ConnectAdjacentRules
(
    Context * pContext,
    Associations pAssociations
)
{
    Coordinates * lAssociation;

    TCHAR lLibraryName[LIBRARY_NAME_SIZE];

    HMODULE lLibrary = (HMODULE) NULL;

    int lX, lY, lZ;

    Coordinates lRule;

    /*
    ** initialize all directions to connect to the non-existent rule
    */

    for (lX = WEST; lX <= EAST; lX++)
    {
        for (lY = NORTH; lY <= SOUTH; lY++)
        {
            for (lZ = DOWN; lZ <= UP; lZ++)
            {
                lAssociation = &pAssociations[lX][lY][lZ];

                lAssociation->X = 000;
                lAssociation->Y = 000;
                lAssociation->Z = 000;
            }
        }
    }

    /*
    ** reference from the (potenitally imaginary) northwestdown corner
    */

    lRule = pContext->Rule;

    lRule.X--;
    lRule.Y--;
    lRule.Z--;

    /*
    ** create associations to adjacent rules
    */

    for (lX = WEST; lX <= EAST; lX++)
    {
        /*
        ** X edge detection
        */

        if (lRule.X + lX < 0 || lRule.X + lX > 888)
        {
            continue;
        }

        for (lY = NORTH; lY <= SOUTH; lY++)
        {
            /*
            ** Y edge detection
            */

            if (lRule.Y + lY < 0 || lRule.Y + lY > 888)
            {
                continue;
            }

            for (lZ = DOWN; lZ <= UP; lZ++)
            {
                /*
                ** Z edge detection
                */

                if (lRule.Z + lZ < 0 || lRule.Z + lZ > 888)
                {
                    continue;
                }

                /*
                ** center special case
                */

                if (NAUGHT == lX && NAUGHT == lY && NAUGHT == lZ)
                {
                    /*
                    ** create recursive association
                    */

                    lAssociation = &pAssociations[NAUGHT][NAUGHT][NAUGHT];

                    lAssociation->X = lRule.X + lX;
                    lAssociation->Y = lRule.Y + lY;
                    lAssociation->Z = lRule.Z + lZ;

                    continue;
                }

                /*
                ** detect an adjacent rule by trying to load its library
                */

                wsprintf(lLibraryName, TEXT("%d.%d.%d%s"), lRule.X + lX, lRule.Y + lY, lRule.Z + lZ, TEXT(LIBRARY_EXTENSION));

                lLibrary = LoadLibrary(lLibraryName);

                if (NULL != lLibrary)
                {
                    FreeLibrary(lLibrary);

                    /*
                    ** create association to the adjacent rule
                    */

                    lAssociation = &pAssociations[lX][lY][lZ];

                    lAssociation->X = lRule.X + lX;
                    lAssociation->Y = lRule.Y + lY;
                    lAssociation->Z = lRule.Z + lZ;
                }
            }
        }
    }
}

void RuleLoader
(
    Context * pContext,
    const Coordinates * pRule,
    _RuleConstructor * pRuleConstructor,
    _RuleDestructor * pRuleDestructor,
    _RuleValidator * pRuleValidator,
    _RuleDescriber * pRuleDescriber,
    _RuleCommandProcessor * pRuleCommandProcessor,
    Associations pAssociations
)
{
    HMODULE lLibrary = (HMODULE) NULL;

    _RuleConstructor lRuleConstructor = (_RuleConstructor) NULL;
    _RuleDestructor lRuleDestructor = (_RuleDestructor) NULL;
    _RuleValidator lRuleValidator = (_RuleValidator) NULL;
    _RuleDescriber lRuleDescriber = (_RuleDescriber) NULL;
    _RuleCommandProcessor lRuleCommandProcessor = (_RuleCommandProcessor) NULL;

    TCHAR lLibraryName[LIBRARY_NAME_SIZE];

    if (000 == pRule->X || 000 == pRule->Y || 000 == pRule->Z)
    {
        printf("Cannot traverse in that direction\n");

        return;
    }

    wsprintf(lLibraryName, TEXT("%d.%d.%d%s"), pRule->X, pRule->Y, pRule->Z, TEXT(LIBRARY_EXTENSION));

    lLibrary = LoadLibrary(lLibraryName);

    if (NULL != lLibrary)
    {
        lRuleConstructor = (_RuleConstructor) GetProcAddress(lLibrary, RULE_CONSTRUCTOR_FUNCTION);
        lRuleDestructor = (_RuleDestructor) GetProcAddress(lLibrary, RULE_DESTRUCTOR_FUNCTION);
        lRuleValidator = (_RuleValidator) GetProcAddress(lLibrary, RULE_VALIDATOR_FUNCTION);
        lRuleDescriber = (_RuleDescriber) GetProcAddress(lLibrary, RULE_DESCRIBER_FUNCTION);
        lRuleCommandProcessor = (_RuleCommandProcessor) GetProcAddress(lLibrary, RULE_COMMAND_PROCESSOR_FUNCTION);
    }

    if (NULL == lLibrary || NULL == lRuleConstructor || NULL == lRuleDestructor || NULL == lRuleValidator || NULL == lRuleDescriber || NULL == lRuleCommandProcessor)
    {
        Emptiness();

        return;
    }

    if (NULL != (* pRuleDestructor))
    {
        (* pRuleDestructor)(pContext);
    }

    if (NULL != pContext->Library)
    {
        FreeLibrary(pContext->Library);
    }

    * pRuleConstructor = lRuleConstructor;
    * pRuleDestructor = lRuleDestructor;
    * pRuleValidator = lRuleValidator;
    * pRuleDescriber = lRuleDescriber;
    * pRuleCommandProcessor = lRuleCommandProcessor;

    pContext->Rule.X = pRule->X;
    pContext->Rule.Y = pRule->Y;
    pContext->Rule.Z = pRule->Z;

    pContext->Library = lLibrary;

    ConnectAdjacentRules(pContext, pAssociations);

    ConstructRule(pContext, * pRuleConstructor);
}

/*----------------------------------------------------------------------------
  Main
  ----------------------------------------------------------------------------*/

int main
(
    void
)
{
    _RuleConstructor lRuleConstructor = (_RuleConstructor) NULL;
    _RuleDestructor lRuleDestructor = (_RuleDestructor) NULL;
    _RuleValidator lRuleValidator = (_RuleValidator) NULL;
    _RuleDescriber lRuleDescriber = (_RuleDescriber) NULL;
    _RuleCommandProcessor lRuleCommandProcessor = (_RuleCommandProcessor) NULL;

    Associations lAssociations;

    Context lContext = {(HMODULE) NULL, NOWHERE, &lAssociations, CONTEXT_NAME, Normal, Normal, Normal, FALSE};

    srand((unsigned int)time((time_t *) NULL));

    RuleLoader(&lContext, &gHome, &lRuleConstructor, &lRuleValidator, &lRuleDestructor, &lRuleDescriber, &lRuleCommandProcessor, lAssociations);

    if (NULL == lRuleConstructor)
    {
        UltimateEmptiness();

        fgetc(stdin);

        exit(-1);
    }

    ConstructRule(&lContext, lRuleConstructor);

    DescribeRule(&lContext, lRuleDescriber);

    for (;;)
    {
        char * lVerbs[] = {"HOP", "GO", "TAKE", "R", "RESET", "VALIDATE", "L", "LOOK", "DESCRIBE", "H", "HELP", "Q", "QUIT", "X", "EXIT", ""};
        char * lDirectObjectAdjectives[] = {""};
        char * lDirectObjectNouns[] = {"DEBUG", "V", "VERBOSITY", ""};
        char * lPrepositions[] = {"TO", "ON", "OFF", ""};
        char * lIndirectObjectAdjectives[] = {""};
        char * lIndirectObjectNouns[] = {"NWD", "Northwestdown", "NW", "Northwest", "NWU", "Northwestup", "WD", "Westdown", "W", "West", "WU", "Westup", "SWD", "Southwestdown", "SW", "Southwest", "SWU", "Southwestup", "ND", "Northdown", "N", "North", "NU", "Northup", "D", "Down", "U", "Up", "SD", "Southdown", "S", "South", "SU", "Southup", "NED", "Northeastdown", "NE", "Northeast", "NEU", "Northeastup", "ED", "Eastdown", "E", "East", "EU", "Eastup", "SED", "Southeastdown", "SE", "Southeast", "SEU", "Southeastup", ""};

        char lCommand[COMMAND_SIZE];

        Command lParsedCommand;

        printf("Command: ");

        GetCommand(lCommand, sizeof(lCommand));

        if (ParseCommand(lCommand, lVerbs, lDirectObjectAdjectives, lDirectObjectNouns, lPrepositions, lIndirectObjectAdjectives, lIndirectObjectNouns, &lParsedCommand))
        {
            if (StringEquali("Q", lParsedCommand.Verb) || StringEquali("Quit", lParsedCommand.Verb) || StringEquali("X", lParsedCommand.Verb) || StringEquali("Exit", lParsedCommand.Verb))
            {
                DestructRule(&lContext, lRuleDestructor);

                break;
            }

            if (StringEquali("H", lParsedCommand.Verb) || StringEquali("Help", lParsedCommand.Verb))
            {
                DisplayOptions();

                continue;
            }

            if (StringEquali("R", lParsedCommand.Verb) || StringEquali("Reset", lParsedCommand.Verb))
            {
                DestructRule(&lContext, lRuleDestructor);
                ConstructRule(&lContext, lRuleConstructor);

                continue;
            }

            if (StringEquali("Validate", lParsedCommand.Verb))
            {
                ValidateRule(&lContext, lRuleValidator);

                continue;
            }

			if (StringEquali("Debug", lParsedCommand.DirectObjectNoun))
            {
                if (StringEquali("On", lParsedCommand.Preposition))
                {
                    lContext.Debug = TRUE;
                    printf("Debug: ON\n");

                    continue;
                }

                if (StringEquali("Off", lParsedCommand.Preposition))
                {
                    lContext.Debug = FALSE;
                    printf("Debug: OFF\n");

                    continue;
                }

                if (STRING_TERMINATOR == *lParsedCommand.Preposition)
                {
                    if (lContext.Debug)
                    {
                        lContext.Debug = FALSE;
                        printf("Debug: OFF\n");
                    }
                    else
                    {
                        lContext.Debug = TRUE;
                        printf("Debug: ON\n");
                    }

                    continue;
                }
            }

            if (StringEquali("V", lParsedCommand.DirectObjectNoun) || StringEquali("Verbosity", lParsedCommand.DirectObjectNoun))
            {
                lContext.InteractionInitial++;
                lContext.InteractionInitial %= 4;

                lContext.InteractionSubsequent++;
                lContext.InteractionSubsequent %= 4;

                lContext.InteractionFrequent++;
                lContext.InteractionFrequent %= 4;

                printf("Verbostiy: %s\n", VerbosityDescription[lContext.InteractionInitial]);

                continue;
            }

            if (StringEquali("L", lParsedCommand.Verb) || StringEquali("Look", lParsedCommand.Verb) || StringEquali("Describe", lParsedCommand.Verb))
            {
                DescribeRule(&lContext, lRuleDescriber);

                continue;
            }

            if (StringEquali("Take", lParsedCommand.Verb) && StringEquali("Off", lParsedCommand.Preposition) )
            {
                printf("I'm not sure how you're doing this, but\nyou're now hovering 10 inches above the ground.\n");

                continue;
            }

            if (StringEquali("Hop", lParsedCommand.Verb))
            {
                RuleLoader(&lContext, &lAssociations[NAUGHT][NAUGHT][NAUGHT], &lRuleConstructor, &lRuleValidator, &lRuleDestructor, &lRuleDescriber, &lRuleCommandProcessor, lAssociations);

                printf("Are you having fun hopping around the dungeon?\n");

                continue;
            }

            if (STRING_TERMINATOR == *lParsedCommand.Verb || StringEquali("GO", lParsedCommand.Verb) || StringEquali("TAKE", lParsedCommand.Verb))
            {
                if (StringEquali("NWD", lParsedCommand.IndirectObjectNoun) || StringEquali("Northwestdown", lParsedCommand.IndirectObjectNoun))
                {
                    RuleLoader(&lContext, &lAssociations[WEST][NORTH][DOWN], &lRuleConstructor, &lRuleValidator, &lRuleDestructor, &lRuleDescriber, &lRuleCommandProcessor, lAssociations);

                    continue;
                }

                if (StringEquali("NW", lParsedCommand.IndirectObjectNoun) || StringEquali("Northwest", lParsedCommand.IndirectObjectNoun))
                {
                    RuleLoader(&lContext, &lAssociations[WEST][NORTH][NAUGHT], &lRuleConstructor, &lRuleValidator, &lRuleDestructor, &lRuleDescriber, &lRuleCommandProcessor, lAssociations);

                    continue;
                }

                if (StringEquali("NWU", lParsedCommand.IndirectObjectNoun) || StringEquali("Northwestup", lParsedCommand.IndirectObjectNoun))
                {
                    RuleLoader(&lContext, &lAssociations[WEST][NORTH][UP], &lRuleConstructor, &lRuleValidator, &lRuleDestructor, &lRuleDescriber, &lRuleCommandProcessor, lAssociations);

                    continue;
                }

                if (StringEquali("WD", lParsedCommand.IndirectObjectNoun) || StringEquali("Westdown", lParsedCommand.IndirectObjectNoun))
                {
                    RuleLoader(&lContext, &lAssociations[WEST][NAUGHT][DOWN], &lRuleConstructor, &lRuleValidator, &lRuleDestructor, &lRuleDescriber, &lRuleCommandProcessor, lAssociations);

                    continue;
                }

                if (StringEquali("W", lParsedCommand.IndirectObjectNoun) || StringEquali("West", lParsedCommand.IndirectObjectNoun))
                {
                    RuleLoader(&lContext, &lAssociations[WEST][NAUGHT][NAUGHT], &lRuleConstructor, &lRuleValidator, &lRuleDestructor, &lRuleDescriber, &lRuleCommandProcessor, lAssociations);

                    continue;
                }

                if (StringEquali("WU", lParsedCommand.IndirectObjectNoun) || StringEquali("Westup", lParsedCommand.IndirectObjectNoun))
                {
                    RuleLoader(&lContext, &lAssociations[WEST][NAUGHT][UP], &lRuleConstructor, &lRuleValidator, &lRuleDestructor, &lRuleDescriber, &lRuleCommandProcessor, lAssociations);

                    continue;
                }

                if (StringEquali("SWD", lParsedCommand.IndirectObjectNoun) || StringEquali("Southwestdown", lParsedCommand.IndirectObjectNoun))
                {
                    RuleLoader(&lContext, &lAssociations[WEST][SOUTH][DOWN], &lRuleConstructor, &lRuleValidator, &lRuleDestructor, &lRuleDescriber, &lRuleCommandProcessor, lAssociations);

                    continue;
                }

                if (StringEquali("SW", lParsedCommand.IndirectObjectNoun) || StringEquali("Southwest", lParsedCommand.IndirectObjectNoun))
                {
                    RuleLoader(&lContext, &lAssociations[WEST][SOUTH][NAUGHT], &lRuleConstructor, &lRuleValidator, &lRuleDestructor, &lRuleDescriber, &lRuleCommandProcessor, lAssociations);

                    continue;
                }

                if (StringEquali("SWU", lParsedCommand.IndirectObjectNoun) || StringEquali("Southwestup", lParsedCommand.IndirectObjectNoun))
                {
                    RuleLoader(&lContext, &lAssociations[WEST][SOUTH][UP], &lRuleConstructor, &lRuleValidator, &lRuleDestructor, &lRuleDescriber, &lRuleCommandProcessor, lAssociations);

                    continue;
                }

                if (StringEquali("ND", lParsedCommand.IndirectObjectNoun) || StringEquali("Northdown", lParsedCommand.IndirectObjectNoun))
                {
                    RuleLoader(&lContext, &lAssociations[NAUGHT][NORTH][DOWN], &lRuleConstructor, &lRuleValidator, &lRuleDestructor, &lRuleDescriber, &lRuleCommandProcessor, lAssociations);

                    continue;
                }

                if (StringEquali("N", lParsedCommand.IndirectObjectNoun) || StringEquali("North", lParsedCommand.IndirectObjectNoun))
                {
                    RuleLoader(&lContext, &lAssociations[NAUGHT][NORTH][NAUGHT], &lRuleConstructor, &lRuleValidator, &lRuleDestructor, &lRuleDescriber, &lRuleCommandProcessor, lAssociations);

                    continue;
                }

                if (StringEquali("NU", lParsedCommand.IndirectObjectNoun) || StringEquali("Northup", lParsedCommand.IndirectObjectNoun))
                {
                    RuleLoader(&lContext, &lAssociations[NAUGHT][NORTH][UP], &lRuleConstructor, &lRuleValidator, &lRuleDestructor, &lRuleDescriber, &lRuleCommandProcessor, lAssociations);

                    continue;
                }

                if (StringEquali("D", lParsedCommand.IndirectObjectNoun) || StringEquali("Down", lParsedCommand.IndirectObjectNoun))
                {
                    RuleLoader(&lContext, &lAssociations[NAUGHT][NAUGHT][DOWN], &lRuleConstructor, &lRuleValidator, &lRuleDestructor, &lRuleDescriber, &lRuleCommandProcessor, lAssociations);

                    continue;
                }

                if (StringEquali("U", lParsedCommand.IndirectObjectNoun) || StringEquali("Up", lParsedCommand.IndirectObjectNoun))
                {
                    RuleLoader(&lContext, &lAssociations[NAUGHT][NAUGHT][UP], &lRuleConstructor, &lRuleValidator, &lRuleDestructor, &lRuleDescriber, &lRuleCommandProcessor, lAssociations);

                    continue;
                }

                if (StringEquali("SD", lParsedCommand.IndirectObjectNoun) || StringEquali("Southdown", lParsedCommand.IndirectObjectNoun))
                {
                    RuleLoader(&lContext, &lAssociations[NAUGHT][SOUTH][DOWN], &lRuleConstructor, &lRuleValidator, &lRuleDestructor, &lRuleDescriber, &lRuleCommandProcessor, lAssociations);

                    continue;
                }

                if (StringEquali("S", lParsedCommand.IndirectObjectNoun) || StringEquali("South", lParsedCommand.IndirectObjectNoun))
                {
                    RuleLoader(&lContext, &lAssociations[NAUGHT][SOUTH][NAUGHT], &lRuleConstructor, &lRuleValidator, &lRuleDestructor, &lRuleDescriber, &lRuleCommandProcessor, lAssociations);

                    continue;
                }

                if (StringEquali("SU", lParsedCommand.IndirectObjectNoun) || StringEquali("Southup", lParsedCommand.IndirectObjectNoun))
                {
                    RuleLoader(&lContext, &lAssociations[NAUGHT][SOUTH][UP], &lRuleConstructor, &lRuleValidator, &lRuleDestructor, &lRuleDescriber, &lRuleCommandProcessor, lAssociations);

                    continue;
                }

                if (StringEquali("NED", lParsedCommand.IndirectObjectNoun) || StringEquali("Northeastdown", lParsedCommand.IndirectObjectNoun))
                {
                    RuleLoader(&lContext, &lAssociations[EAST][NORTH][DOWN], &lRuleConstructor, &lRuleValidator, &lRuleDestructor, &lRuleDescriber, &lRuleCommandProcessor, lAssociations);

                    continue;
                }

                if (StringEquali("NE", lParsedCommand.IndirectObjectNoun) || StringEquali("Northeast", lParsedCommand.IndirectObjectNoun))
                {
                    RuleLoader(&lContext, &lAssociations[EAST][NORTH][NAUGHT], &lRuleConstructor, &lRuleValidator, &lRuleDestructor, &lRuleDescriber, &lRuleCommandProcessor, lAssociations);

                    continue;
                }

                if (StringEquali("NEU", lParsedCommand.IndirectObjectNoun) || StringEquali("Northeastup", lParsedCommand.IndirectObjectNoun))
                {
                    RuleLoader(&lContext, &lAssociations[EAST][NORTH][UP], &lRuleConstructor, &lRuleValidator, &lRuleDestructor, &lRuleDescriber, &lRuleCommandProcessor, lAssociations);

                    continue;
                }

                if (StringEquali("ED", lParsedCommand.IndirectObjectNoun) || StringEquali("Eastdown", lParsedCommand.IndirectObjectNoun))
                {
                    RuleLoader(&lContext, &lAssociations[EAST][NAUGHT][DOWN], &lRuleConstructor, &lRuleValidator, &lRuleDestructor, &lRuleDescriber, &lRuleCommandProcessor, lAssociations);

                    continue;
                }

                if (StringEquali("E", lParsedCommand.IndirectObjectNoun) || StringEquali("East", lParsedCommand.IndirectObjectNoun))
                {
                    RuleLoader(&lContext, &lAssociations[EAST][NAUGHT][NAUGHT], &lRuleConstructor, &lRuleValidator, &lRuleDestructor, &lRuleDescriber, &lRuleCommandProcessor, lAssociations);

                    continue;
                }

                if (StringEquali("EU", lParsedCommand.IndirectObjectNoun) || StringEquali("Eastup", lParsedCommand.IndirectObjectNoun))
                {
                    RuleLoader(&lContext, &lAssociations[EAST][NAUGHT][UP], &lRuleConstructor, &lRuleValidator, &lRuleDestructor, &lRuleDescriber, &lRuleCommandProcessor, lAssociations);

                    continue;
                }

                if (StringEquali("SED", lParsedCommand.IndirectObjectNoun) || StringEquali("Southeastdown", lParsedCommand.IndirectObjectNoun))
                {
                    RuleLoader(&lContext, &lAssociations[EAST][SOUTH][DOWN], &lRuleConstructor, &lRuleValidator, &lRuleDestructor, &lRuleDescriber, &lRuleCommandProcessor, lAssociations);

                    continue;
                }

                if (StringEquali("SE", lParsedCommand.IndirectObjectNoun) || StringEquali("Southeast", lParsedCommand.IndirectObjectNoun))
                {
                    RuleLoader(&lContext, &lAssociations[EAST][SOUTH][NAUGHT], &lRuleConstructor, &lRuleValidator, &lRuleDestructor, &lRuleDescriber, &lRuleCommandProcessor, lAssociations);

                    continue;
                }

                if (StringEquali("SEU", lParsedCommand.IndirectObjectNoun) || StringEquali("Southeastup", lParsedCommand.IndirectObjectNoun))
                {
                    RuleLoader(&lContext, &lAssociations[EAST][SOUTH][UP], &lRuleConstructor, &lRuleValidator, &lRuleDestructor, &lRuleDescriber, &lRuleCommandProcessor, lAssociations);

                    continue;
                }

                if ('\0' == *lParsedCommand.IndirectObjectNoun)
                {
                    RuleLoader(&lContext, &lAssociations[NAUGHT][NAUGHT][NAUGHT], &lRuleConstructor, &lRuleValidator, &lRuleDestructor, &lRuleDescriber, &lRuleCommandProcessor, lAssociations);

                    printf("Are you having fun running around in circles?\n");

                    continue;
                }
            }
        }

        if (!ProcessCommand(&lContext, lRuleCommandProcessor, lCommand))
        {
            printf("Huh?\n");
        }
    }

    exit(0);
}
