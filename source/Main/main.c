/*----------------------------------------------------------------------------
  Neoheurist engine

  Copyright 2011-2015 John L. Hart IV. All rights reserved.

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

#include "../Motel/motel.compilation.t.h"
#include "../Motel/motel.types.t.h"

#include "../Motel/motel.memory.i.h"

#include "../Motel/motel.string.t.h"
#include "../Motel/motel.string.i.h"

#include "../Motel/motel.aleatory.t.h"
#include "../Motel/motel.aleatory.i.h"

#include "../Command/command.t.h"
#include "../Command/command.i.h"

#include "../xxx.yyy.zzz/xxx.yyy.zzz.t.h"

#include "main.t.h"

/*----------------------------------------------------------------------------
  Public functions
  ----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
  Private defines, data types and function prototypes
  ----------------------------------------------------------------------------*/

#define LIBRARY_EXTENSION ".dll"

#define ROOM_CONTEXT_MEMORY_LIMIT 1000000

#define CONTEXT_NAME "Acaj"
#define NOWHERE {000, 000, 000}
#define HOME {555, 555, 555}

/*----------------------------------------------------------------------------
  Embedded copyright
  ----------------------------------------------------------------------------*/

static const char *gCopyright UNUSED = "@(#)engine.c - Copyright 2011-2015 John L. Hart IV - All rights reserved";

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
           "SWD - Go Southwestdown\n"
           "SW  - Go Southwest\n"
           "SWU - Go Southwestup\n"
           "SD  - Go Southdown\n"
           "S   - Go South\n"
           "SU  - Go Southup\n"
           "SED - Go Southeastdown\n"
           "SE  - Go Southeast\n"
           "SEU - Go Southeastup\n"
           "WD  - Go Westdown\n"
           "W   - Go West\n"
           "WU  - Go Westup\n"
           "D   - Go Down\n"
           "U   - Go Up\n"
           "ED  - Go Eastdown\n"
           "E   - Go East\n"
           "EU  - Go Eastup\n"
	       "NWD - Go Northwestdown\n"
           "NW  - Go Northwest\n"
           "NWU - Go Northwestup\n"
           "ND  - Go Northdown\n"
           "N   - Go North\n"
           "NU  - Go Northup\n"
           "NED - Go Northeastdown\n"
           "NE  - Go Northeast\n"
           "NEU - Go Northeastup\n"
           "\n"
           "[L]ook      - Describe location\n"
           "Describe    - Describe location\n"
           "[V]alidate  - Validate location\n"
           "[VERBOSITY] - Toggle verbosity mode\n"
           "[DEBUG]     - Toggle debug mode\n"
           "\n"
           "[R]eset     - Reset current location\n"
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

GenderQualifier GenderQualifiers[_Gender] = {{MALE_TERMS}, {FEMALE_TERMS}, {PERSON_TERMS}, {ROBOT_TERMS}};

char * DirectionNames[3][3][3] = {{{"Southwestdown", "Southwest", "Southwestup"}, {"Southdown", "South", "Southup"}, {"Southeastdown", "Southeast", "Southeastup"}},
                                  {{"Westdown",      "West",      "Westup" },     {"Down",      " ",     "Up" },     {"Eastdown",      "East",      "Eastup"}},
                                  {{"Northeastdown", "Northeast", "Northeastup"}, {"Northdown", "North", "Northup"}, {"Northwestdown", "Northwest", "Northwestup"}}};

char * DirectionAbbreviations[3][3][3] = {{{"SWD", "SW", "SWU"}, {"SD", "S", "SU"}, {"SED", "SE", "SEU"}},
                                          {{"WD",  "W",  "WU"},  {"D",  " ", "U"},  {"ED", "E",  "EU"}},
                                          {{"NED", "NE", "NEU"}, {"ND", "N", "NU"}, {"NWD", "NW", "NWU"}}};

void CALLING_CONVENTION ListAdjacentLocations
(
    Context * pContext,
	_AssociationDescriber pAssociationDescriber
)
{
    if (Off == pContext->CompassHeadingMode)
    {
        return;
    }

    if (!pContext->CompassTrustworthy)
    {
        printf("No matter what you try the compass needle will not settle.\n");

        return;
    }

    int lX, lY, lZ;

    Coordinates * lAssociation;

    /*
    ** describe passages to the neighbors
    */

    for (lX = SOUTH; lX <= NORTH; lX++)
    {
        for (lY = WEST; lY <= EAST; lY++)
        {
            for (lZ = DOWN; lZ <= UP; lZ++)
            {
                if (NAUGHT != lX || NAUGHT != lY || NAUGHT != lZ)
                {
                    lAssociation = &(*pContext->Connections)[lX][lY][lZ];

                    if (0 != lAssociation->X && 0 != lAssociation->Y && 0 != lAssociation->Z)
                    {
                        pAssociationDescriber(pContext, lAssociation);
                    }
                }
            }
        }
    }

    /*
    ** create a passage headings list
    */

    char * lFormatSpecifier = "%s";

    for (lX = SOUTH; lX <= NORTH; lX++)
    {
        for (lY = WEST; lY <= EAST; lY++)
        {
            for (lZ = DOWN; lZ <= UP; lZ++)
            {
                if (NAUGHT != lX || NAUGHT != lY || NAUGHT != lZ)
                {
                    lAssociation = &(*pContext->Connections)[lX][lY][lZ];

                    if (0 != lAssociation->X && 0 != lAssociation->Y && 0 != lAssociation->Z)
                    {
                        if (Terse == pContext->CompassHeadingMode)
                        {
                            printf(lFormatSpecifier, DirectionAbbreviations[lX][lY][lZ]);
                        }
                        else
                        {
                            printf(lFormatSpecifier, DirectionNames[lX][lY][lZ]);
                        }

                        lFormatSpecifier = ", %s";
                    }
                }
            }
        }
    }

    printf("\n");
}

/*----------------------------------------------------------------------------
  Location interface functions (debug wrappers)
  ----------------------------------------------------------------------------*/

void ConstructLocation
(
    Context * pContext,
    _LocationConstructor pLocationConstructor
)
{
    if (pContext->Debug)
    {
        fprintf(stderr, "Location %d.%d.%d constructing\n", pContext->Location.X, pContext->Location.Y, pContext->Location.Z);
    }

    (* pLocationConstructor)(pContext);

    if (pContext->Debug)
    {
        fprintf(stderr, "Location %d.%d.%d constructed\n", pContext->Location.X, pContext->Location.Y, pContext->Location.Z);
    }
}

void DestructLocation
(
    Context * pContext,
    _LocationDestructor pLocationDestructor
)
{
    if (pContext->Debug)
    {
        fprintf(stderr, "Location %d.%d.%d destructing\n", pContext->Location.X, pContext->Location.Y, pContext->Location.Z);
    }

    (* pLocationDestructor)(pContext);

    if (pContext->Debug)
    {
        fprintf(stderr, "Location %d.%d.%d destructed\n", pContext->Location.X, pContext->Location.Y, pContext->Location.Z);
    }
}

void ValidateLocation
(
    Context * pContext,
    _LocationValidator pLocationValidator
)
{
    if (pContext->Debug)
    {
        fprintf(stderr, "Location %d.%d.%d validating\n", pContext->Location.X, pContext->Location.Y, pContext->Location.Z);
    }

    (* pLocationValidator)(pContext);

    if (pContext->Debug)
    {
        fprintf(stderr, "Location %d.%d.%d validated\n", pContext->Location.X, pContext->Location.Y, pContext->Location.Z);
    }
}

void DescribeLocation
(
    Context * pContext,
    _LocationDescriber pLocationDescriber
)
{
    if (pContext->Debug)
    {
        fprintf(stderr, "Location %d.%d.%d describing\n", pContext->Location.X, pContext->Location.Y, pContext->Location.Z);
    }

    (* pLocationDescriber)(pContext, ListAdjacentLocations);

    if (pContext->Debug)
    {
        fprintf(stderr, "Location %d.%d.%d described\n", pContext->Location.X, pContext->Location.Y, pContext->Location.Z);
    }
}

boolean ProcessCommand
(
    Context * pContext,
    _LocationCommandProcessor pLocationCommandProcessor,
    Tokens lCommandTokens
)
{
    boolean lResult;

    if (pContext->Debug)
    {
        fprintf(stderr, "Location %d.%d.%d processing command\n", pContext->Location.X, pContext->Location.Y, pContext->Location.Z);
    }

    lResult = (* pLocationCommandProcessor)(pContext, lCommandTokens);

    if (pContext->Debug)
    {
        fprintf(stderr, "Location %d.%d.%d processed command\n", pContext->Location.X, pContext->Location.Y, pContext->Location.Z);
    }

    return (lResult);
}

void ConnectAdjacentLocations
(
    Context * pContext,
    Connections pConnections
)
{
    Coordinates * lAssociation;

    TCHAR lLibraryName[LIBRARY_NAME_SIZE];

    HMODULE lLibrary = (HMODULE) NULL;

    int lX, lY, lZ;

    Coordinates lLocation;

    /*
    ** initialize all directions to connect to the non-existent location
    */

    for (lX = SOUTH; lX <= NORTH; lX++)
    {
        for (lY = WEST; lY <= EAST; lY++)
        {
            for (lZ = DOWN; lZ <= UP; lZ++)
            {
                lAssociation = &pConnections[lX][lY][lZ];

                lAssociation->X = 000, lAssociation->Y = 000, lAssociation->Z = 000;
            }
        }
    }

    /*
    ** reference from the (potenitally imaginary) northwestdown corner
    */

    lLocation = pContext->Location;

    lLocation.X--, lLocation.Y--, lLocation.Z--;

    /*
    ** create associations to adjacent locations
    */

    for (lX = SOUTH; lX <= NORTH; lX++)
    {
        /*
        ** X edge detection
        */

        if (lLocation.X + lX < 000 || lLocation.X + lX > 888)
        {
            continue;
        }

        for (lY = WEST; lY <= EAST; lY++)
        {
            /*
            ** Y edge detection
            */

            if (lLocation.Y + lY < 000 || lLocation.Y + lY > 888)
            {
                continue;
            }

            for (lZ = DOWN; lZ <= UP; lZ++)
            {
                /*
                ** Z edge detection
                */

                if (lLocation.Z + lZ < 000 || lLocation.Z + lZ > 888)
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

                    lAssociation = &pConnections[NAUGHT][NAUGHT][NAUGHT];

                    lAssociation->X = lLocation.X + lX, lAssociation->Y = lLocation.Y + lY, lAssociation->Z = lLocation.Z + lZ;

                    continue;
                }

                /*
                ** detect an adjacent location by trying to load its library
                */

                wsprintf(lLibraryName, TEXT("%d.%d.%d%s"), lLocation.X + lX, lLocation.Y + lY, lLocation.Z + lZ, TEXT(LIBRARY_EXTENSION));

                lLibrary = LoadLibrary(lLibraryName);

                if (NULL != lLibrary)
                {
                    FreeLibrary(lLibrary);

                    /*
                    ** create association to the adjacent location
                    */

                    lAssociation = &pConnections[lX][lY][lZ];

                    lAssociation->X = lLocation.X + lX, lAssociation->Y = lLocation.Y + lY, lAssociation->Z = lLocation.Z + lZ;
                }
            }
        }
    }
}

void LocationLoader
(
    Context * pContext,
    const Coordinates * pLocation,
    _LocationConstructor * pLocationConstructor,
    _LocationDestructor * pLocationDestructor,
    _LocationValidator * pLocationValidator,
    _LocationDescriber * pLocationDescriber,
    _LocationCommandProcessor * pLocationCommandProcessor,
    Connections pConnections
)
{
    HMODULE lLibrary = (HMODULE) NULL;

    _LocationConstructor lLocationConstructor = (_LocationConstructor) NULL;
    _LocationDestructor lLocationDestructor = (_LocationDestructor) NULL;
    _LocationValidator lLocationValidator = (_LocationValidator) NULL;
    _LocationDescriber lLocationDescriber = (_LocationDescriber) NULL;
    _LocationCommandProcessor lLocationCommandProcessor = (_LocationCommandProcessor) NULL;

    TCHAR lLibraryName[LIBRARY_NAME_SIZE];

    if (000 == pLocation->X || 000 == pLocation->Y || 000 == pLocation->Z)
    {
        printf("Cannot traverse in that direction\n");

        return;
    }

    wsprintf(lLibraryName, TEXT("%d.%d.%d%s"), pLocation->X, pLocation->Y, pLocation->Z, TEXT(LIBRARY_EXTENSION));

    lLibrary = LoadLibrary(lLibraryName);

    if (NULL != lLibrary)
    {
        lLocationConstructor = (_LocationConstructor) GetProcAddress(lLibrary, LOCATION_CONSTRUCTOR_FUNCTION);
        lLocationDestructor = (_LocationDestructor) GetProcAddress(lLibrary, LOCATION_DESTRUCTOR_FUNCTION);
        lLocationValidator = (_LocationValidator) GetProcAddress(lLibrary, LOCATION_VALIDATOR_FUNCTION);
        lLocationDescriber = (_LocationDescriber) GetProcAddress(lLibrary, LOCATION_DESCRIBER_FUNCTION);
        lLocationCommandProcessor = (_LocationCommandProcessor) GetProcAddress(lLibrary, LOCATION_COMMAND_PROCESSOR_FUNCTION);
    }

    if (NULL == lLibrary || NULL == lLocationConstructor || NULL == lLocationDestructor || NULL == lLocationValidator || NULL == lLocationDescriber || NULL == lLocationCommandProcessor)
    {
        Emptiness();

        return;
    }

    if (NULL != (* pLocationDestructor))
    {
        (* pLocationDestructor)(pContext);
    }

    if (NULL != pContext->Library)
    {
        FreeLibrary(pContext->Library);
    }

    * pLocationConstructor = lLocationConstructor;
    * pLocationDestructor = lLocationDestructor;
    * pLocationValidator = lLocationValidator;
    * pLocationDescriber = lLocationDescriber;
    * pLocationCommandProcessor = lLocationCommandProcessor;

    pContext->Location.X = pLocation->X, pContext->Location.Y = pLocation->Y, pContext->Location.Z = pLocation->Z;

    pContext->Library = lLibrary;

    ConnectAdjacentLocations(pContext, pConnections);

    ConstructLocation(pContext, * pLocationConstructor);
}

long _compareKeys
(
    const void * pGUId1,
    const void * pGUId2
)
{
    return (CompareiStrings(pGUId1, pGUId2));
}

char * Verbs[] = { QUIT_WORDS, HELP_WORDS, REDO_WORDS, DESCRIBE_WORDS, GO_WORDS, TEST_WORDS, STATE_MANIPULATION_WORDS, "TAKEOFF", ""};
char * DirectObjectArticles[] = {""};
char * DirectObjectAdjectives[] = {FREQUENCY_WORDS, VERBOSITY_WORDS, LENGTH_WORDS, ""};
char * DirectObjectNouns[] = {BEARING_WORDS, "TRACE", "DEBUG", "VERBOSITY", "MYSELF", "SELF", ""};
char * Prepositions[] = {"AS", TRANSLOCUATIVE_WORDS, ""};
char * IndirectObjectArticles[] = {"THE", ""};
char * IndirectObjectAdjectives[] = {HEADING_WORDS, STATE_WORDS, VERBOSITY_WORDS, LENGTH_WORDS, ""};
char * IndirectObjectNouns[] = {SPACE_WORDS, SELF_WORDS, "INVENTORY", "BACKPACK", "SATCHEL", "PURSE", "BAG", "BRIEFCASE", "CABINET", "VAULT", ""};

CommandTree Commands = {Verbs, DirectObjectArticles, DirectObjectAdjectives, DirectObjectNouns, Prepositions, IndirectObjectArticles, IndirectObjectAdjectives, IndirectObjectNouns};

/*----------------------------------------------------------------------------
  Main
  ----------------------------------------------------------------------------*/

int main
(
    void
)
{
    _LocationConstructor lLocationConstructor = (_LocationConstructor) NULL;
    _LocationDestructor lLocationDestructor = (_LocationDestructor) NULL;
    _LocationValidator lLocationValidator = (_LocationValidator) NULL;
    _LocationDescriber lLocationDescriber = (_LocationDescriber) NULL;
    _LocationCommandProcessor lLocationCommandProcessor = (_LocationCommandProcessor) NULL;

	Connections lConnections = {{{{0,0,0},{0,0,0},{0,0,0}}, {{0,0,0},{0,0,0},{0,0,0}}, {{0,0,0},{0,0,0},{0,0,0}}},
	                            {{{0,0,0},{0,0,0},{0,0,0}}, {{0,0,0},{0,0,0},{0,0,0}}, {{0,0,0},{0,0,0},{0,0,0}}},
	                            {{{0,0,0},{0,0,0},{0,0,0}}, {{0,0,0},{0,0,0},{0,0,0}}, {{0,0,0},{0,0,0},{0,0,0}}}};

    motelTreeHandle ItemStates = NULL;

    ConstructTree(&ItemStates, ROOM_CONTEXT_MEMORY_LIMIT, POINTER_SIZE, UUID_SIZE, _compareKeys);

    motelTreeHandle lLocationStates = NULL;

    ConstructTree(&lLocationStates, ROOM_CONTEXT_MEMORY_LIMIT, POINTER_SIZE, UUID_SIZE, _compareKeys);

    if (NULL == lLocationStates)
    {
        UltimateEmptiness();

        fgetc(stdin);

        exit(-1);
    }

    Context lContext = {(HMODULE) NULL, NOWHERE, &lConnections, Female, GenderQualifiers, Tweleve, Eight, Tweleve, Eight, Tweleve, Eight, Tweleve, Eight, CONTEXT_NAME, Verbose, Normal, Terse, TRUE, Terse, FALSE, FALSE, FALSE, ItemStates, lLocationStates};

	Tokens lCommandTokens;

	char lLastCommand[COMMAND_SIZE] = "";

    char * lCommand = "LOOK";

    CommandProcessor * lCommandProcessor = AllocateCommandProcessor();

    Command lParsedCommand;

    uint64_t lSeed[64] = { 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x60, 0x61, 0x62, 0x63, 0x64 };

    AleatorySeed4096Period(lSeed);

    LocationLoader(&lContext, &gHome, &lLocationConstructor, &lLocationValidator, &lLocationDestructor, &lLocationDescriber, &lLocationCommandProcessor, lConnections);

    if (NULL == lLocationConstructor)
    {
        UltimateEmptiness();

        fgetc(stdin);

        exit(-1);
    }

    for (;;printf("Command: "), lCommand = GetCommand(lCommandProcessor))
    {
    reprocess:

        TokenizeCommand(lCommand, &lCommandTokens);

		/* The location gets the first opportunity to process the request */
		
        if (ProcessCommand(&lContext, lLocationCommandProcessor, lCommandTokens))
        {
			next;
        }

		/* Slot the tokens found in the command as parts of speech */

		ParseTokenizedCommand(lCommandTokens, Commands, &lParsedCommand);

		/* Interpret the tokenized sentence */

		if (ClassMatchString(lParsedCommand.Verb, REDO_WORDS, ""))
        {
            CopyString(lLastCommand, lCommand, sizeof(lCommand));
			same;
        }

		/* Process commands */

        if (ClassMatchString(lParsedCommand.Verb, QUIT_WORDS, ""))
        {
            DestructLocation(&lContext, lLocationDestructor);
            done;
        }
		
		if (ClassMatchString(lParsedCommand.Verb, HELP_WORDS, ""))
        {
            DisplayOptions();
			next;
        }
		
		if (EqualStringsi("Reset", lParsedCommand.Verb) && *CC_STRING_TERMINATOR == *lParsedCommand.DirectObjectNoun)
        {
            DestructLocation(&lContext, lLocationDestructor), ConstructLocation(&lContext, lLocationConstructor);
			next;
        }
		
        if (EqualStringsi("Trace", lParsedCommand.DirectObjectNoun))
        {
            if (EqualStringsi("Set", lParsedCommand.Verb))
            {
                lContext.Trace = TRUE;
            }
            else if (EqualStringsi("Reset", lParsedCommand.Verb))
            {
                lContext.Trace = FALSE;
            }
            else if (EqualStringsi("Toggle", lParsedCommand.Verb))
            {
                lContext.Trace = !lContext.Debug;
            }
            else if (*CC_STRING_TERMINATOR == *lParsedCommand.Verb)
            {
                if (EqualStringsi("On", lParsedCommand.IndirectObjectAdjective))
                {
                    lContext.Trace = TRUE;
                }
                else if (EqualStringsi("Off", lParsedCommand.IndirectObjectAdjective))
                {
                    lContext.Trace = FALSE;
                }
            }
            else
            {
                what;
            }

            printf("Trace: %s\n", lContext.Trace ? "ON" : "OFF");

            next;
        }

        if (EqualStringsi("Debug", lParsedCommand.DirectObjectNoun))
        {
            if (EqualStringsi("Set", lParsedCommand.Verb))
            {
                lContext.Debug = TRUE;
            }
            else if (EqualStringsi("Reset", lParsedCommand.Verb))
            {
                lContext.Debug = FALSE;
            }
            else if (EqualStringsi("Toggle", lParsedCommand.Verb))
            {
                lContext.Debug = !lContext.Debug;
            }
            else if (*CC_STRING_TERMINATOR == *lParsedCommand.Verb)
            {
                if (EqualStringsi("On", lParsedCommand.IndirectObjectAdjective))
                {
                    lContext.Debug = TRUE;
                }
                else if (EqualStringsi("Off", lParsedCommand.IndirectObjectAdjective))
                {
                    lContext.Debug = FALSE;
                }
            }
            else
            {
                what;
            }

            printf("Debug: %s\n", lContext.Debug ? "ON" : "OFF");

            next;
        }

        if (EqualStringsi("Validate", lParsedCommand.Verb))
        {
            if (ValidateLocation(&lContext, lLocationValidator), lContext.Valid)
            {
                next;
            }
            else
            {
                fail; 
            }
        }

        if (ClassMatchString(lParsedCommand.DirectObjectNoun, BEARING_WORDS, ""))
        {
            /* Grammar checks */

            /* Prohibited sentence part check */

            if (*CC_STRING_TERMINATOR != *lParsedCommand.DirectObjectArticle || *CC_STRING_TERMINATOR != *lParsedCommand.IndirectObjectArticle || *CC_STRING_TERMINATOR != *lParsedCommand.IndirectObjectNoun)
            {
                what;
            }

            /* Required sentence part check */

            if (*CC_STRING_TERMINATOR != *lParsedCommand.DirectObjectAdjective && *CC_STRING_TERMINATOR != *lParsedCommand.IndirectObjectAdjective)
            {
                what;
            }

            /* Permitted verb check */

            if (*CC_STRING_TERMINATOR != *lParsedCommand.Verb && !ClassMatchString(lParsedCommand.Verb, "Set", "Change", ""))
            {
                what;
            }

            /* Permitted preposition check */

            if (*CC_STRING_TERMINATOR != *lParsedCommand.Preposition && !ClassMatchString(lParsedCommand.Preposition, "As", "To", ""))
            {
                what;
            }

            /* Verb/preposition agreement */

            if (EqualStringsi("Change", lParsedCommand.Verb) && !EqualStringsi("To", lParsedCommand.Preposition))
            {
                what;
            }

            /* Required adjective check */

            if (!ClassMatchString(lParsedCommand.DirectObjectAdjective, "Off", "On", "Terse", "Short", "Abbreviated", "Long", "Normal", "Full", "") && !ClassMatchString(lParsedCommand.IndirectObjectAdjective, "Off", "On", "Terse", "Short", "Abbreviated", "Long", "Normal", "Full", ""))
            {
                what;
            }

            /* Process request */

            if (EqualStringsi("Off", lParsedCommand.IndirectObjectAdjective))
            {
                lContext.CompassHeadingMode = Off;
            }
            else if (ClassMatchString(lParsedCommand.DirectObjectAdjective, "On", "Short", "Terse", "Abbreviated", "") || ClassMatchString(lParsedCommand.IndirectObjectAdjective, "On", "Short", "Terse", "Abbreviated", ""))
            {
                lContext.CompassHeadingMode = Terse;
            }
            else if (ClassMatchString(lParsedCommand.DirectObjectAdjective, "Long", "Normal", "Full", "") || ClassMatchString(lParsedCommand.IndirectObjectAdjective, "Long", "Normal", "Full", ""))
            {
                lContext.CompassHeadingMode = Normal;
            }

            /* Return response */

            printf("Direction descriptions are: %s\n", Off == lContext.CompassHeadingMode ? "off" : Terse == lContext.CompassHeadingMode ? "abbreviated (short)" : "full (long)");

            next;
        }

        /* Handle irregular forms of the VERBOSITY command (e.g. make these commands regular) */
		
		if (*CC_STRING_TERMINATOR == *lParsedCommand.Verb && *CC_STRING_TERMINATOR == *lParsedCommand.DirectObjectNoun && ClassMatchString(lParsedCommand.IndirectObjectAdjective, VERBOSITY_WORDS, ""))
		{
			CopyString("Set", lParsedCommand.Verb, sizeof(lParsedCommand.Verb));
			CopyString("Verbosity", lParsedCommand.DirectObjectNoun, sizeof(lParsedCommand.DirectObjectNoun));
		}

		if (EqualStringsi("Verbosity", lParsedCommand.DirectObjectNoun))
		{
			if (*CC_STRING_TERMINATOR != *lParsedCommand.Verb)
			{
				if (EqualStringsi("Reset", lParsedCommand.Verb))
				{
					if (*CC_STRING_TERMINATOR == *lParsedCommand.Preposition || *CC_STRING_TERMINATOR == *lParsedCommand.IndirectObjectAdjective)
					{
						printf("RESET VERBOSITY doesn't accept prepositional clauses (try \"SET VERBOSITY\" instead)\n");
						continue;
					}

					if (*CC_STRING_TERMINATOR == *lParsedCommand.DirectObjectAdjective)
					{
						lContext.InteractionInitial = Normal;
						lContext.InteractionSubsequent = Normal;
						lContext.InteractionFrequent = Normal;
					}
					else if (EqualStringsi("Initial", lParsedCommand.DirectObjectAdjective))
					{
						lContext.InteractionInitial = Normal;
					}
					else if (EqualStringsi("Subsequent", lParsedCommand.DirectObjectAdjective))
					{
						lContext.InteractionSubsequent = Normal;
					}
					else if (EqualStringsi("Frequent", lParsedCommand.DirectObjectAdjective))
					{
						lContext.InteractionFrequent = Normal;
					}
					else
					{
						printf("VERBOSITY trigger incorrectly specified (try \"Initial\", \"Subsequent\", or \"Frequent\")\n");
						continue;
					}
				}
				else if (EqualStringsi("Increment", lParsedCommand.Verb))
				{
					if (*CC_STRING_TERMINATOR == *lParsedCommand.Preposition || *CC_STRING_TERMINATOR == *lParsedCommand.IndirectObjectAdjective)
					{
						printf("INCREMENT VERBOSITY doesn't accept prepositional clauses (try \"SET VERBOSITY\" instead)\n");
						continue;
					}

					if (*CC_STRING_TERMINATOR == *lParsedCommand.DirectObjectAdjective)
					{
						lContext.InteractionInitial = (Verbosity) (((int) lContext.InteractionInitial + 1) % 4);
						lContext.InteractionSubsequent = (Verbosity) (((int) lContext.InteractionSubsequent + 1) % 4);
						lContext.InteractionFrequent = (Verbosity) (((int) lContext.InteractionFrequent + 1) % 4);
					}
					else if (EqualStringsi("Initial", lParsedCommand.DirectObjectAdjective))
					{
						lContext.InteractionInitial = (Verbosity) (((int) lContext.InteractionInitial + 1) % 4);
					}
					else if (EqualStringsi("Subsequent", lParsedCommand.DirectObjectAdjective))
					{
						lContext.InteractionSubsequent = (Verbosity) (((int) lContext.InteractionSubsequent + 1) % 4);
					}
					else if (EqualStringsi("Frequent", lParsedCommand.DirectObjectAdjective))
					{
						lContext.InteractionFrequent = (Verbosity) (((int) lContext.InteractionFrequent + 1) % 4);
					}
					else
					{
						printf("VERBOSITY trigger incorrectly specified (try \"Initial\", \"Subsequent\", or \"Frequent\")\n");
						continue;
					}
				}
				else if (EqualStringsi("Set", lParsedCommand.Verb))
				{
					if (*CC_STRING_TERMINATOR != *lParsedCommand.Preposition && !EqualStringsi("To", lParsedCommand.Preposition) && !EqualStringsi("As", lParsedCommand.Preposition))
					{
						printf("Unacceptble preposition (try \"TO\", \"AS\", or omitting the preoposition)\n");
						continue;
					}

					if (*CC_STRING_TERMINATOR == *lParsedCommand.DirectObjectAdjective)
					{
						if (EqualStringsi("SMS", lParsedCommand.IndirectObjectAdjective))
						{
							lContext.InteractionInitial = SMS;
							lContext.InteractionSubsequent = SMS;
							lContext.InteractionFrequent = SMS;
						}
						else if (EqualStringsi("Terse", lParsedCommand.IndirectObjectAdjective))
						{
							lContext.InteractionInitial = Terse;
							lContext.InteractionSubsequent = Terse;
							lContext.InteractionFrequent = Terse;
						}
						else if (EqualStringsi("Normal", lParsedCommand.IndirectObjectAdjective))
						{
							lContext.InteractionInitial = Normal;
							lContext.InteractionSubsequent = Normal;
							lContext.InteractionFrequent = Normal;
						}
						else if (EqualStringsi("Verbose", lParsedCommand.IndirectObjectAdjective))
						{
							lContext.InteractionInitial = Verbose;
							lContext.InteractionSubsequent = Verbose;
							lContext.InteractionFrequent = Verbose;
						}
						else
						{
							printf("\"%s\" is not a VERBOSITY mode (try \"SMS\", \"Terse\", \"Normal\", or \"Verbose\")\n", lParsedCommand.IndirectObjectAdjective);
							continue;
						}
					}
					else if (EqualStringsi("Initial", lParsedCommand.DirectObjectAdjective))
					{
						if (EqualStringsi("SMS", lParsedCommand.IndirectObjectAdjective))
						{
							lContext.InteractionInitial = SMS;
						}
						else if (EqualStringsi("Terse", lParsedCommand.IndirectObjectAdjective))
						{
							lContext.InteractionInitial = Terse;
						}
						else if (EqualStringsi("Normal", lParsedCommand.IndirectObjectAdjective))
						{
							lContext.InteractionInitial = Normal;
						}
						else if (EqualStringsi("Verbose", lParsedCommand.IndirectObjectAdjective))
						{
							lContext.InteractionInitial = Verbose;
						}
						else
						{
							printf("\"%s\" is not a VERBOSITY mode (try \"SMS\", \"Terse\", \"Normal\", or \"Verbose\")\n", lParsedCommand.IndirectObjectAdjective);
							continue;
						}
					}
					else if (EqualStringsi("Subsequent", lParsedCommand.DirectObjectAdjective))
					{
						if (EqualStringsi("SMS", lParsedCommand.IndirectObjectAdjective))
						{
							lContext.InteractionSubsequent = SMS;
						}
						else if (EqualStringsi("Terse", lParsedCommand.IndirectObjectAdjective))
						{
							lContext.InteractionSubsequent = Terse;
						}
						else if (EqualStringsi("Normal", lParsedCommand.IndirectObjectAdjective))
						{
							lContext.InteractionSubsequent = Normal;
						}
						else if (EqualStringsi("Verbose", lParsedCommand.IndirectObjectAdjective))
						{
							lContext.InteractionSubsequent = Verbose;
						}
						else
						{
							printf("\"%s\" is not a VERBOSITY mode (try \"SMS\", \"Terse\", \"Normal\", or \"Verbose\")\n", lParsedCommand.IndirectObjectAdjective);
							continue;
						}
					}
					else if (EqualStringsi("Frequent", lParsedCommand.DirectObjectAdjective))
					{
						if (EqualStringsi("SMS", lParsedCommand.IndirectObjectAdjective))
						{
							lContext.InteractionFrequent = SMS;
						}
						else if (EqualStringsi("Terse", lParsedCommand.IndirectObjectAdjective))
						{
							lContext.InteractionFrequent = Terse;
						}
						else if (EqualStringsi("Normal", lParsedCommand.IndirectObjectAdjective))
						{
							lContext.InteractionFrequent = Normal;
						}
						else if (EqualStringsi("Verbose", lParsedCommand.IndirectObjectAdjective))
						{
							lContext.InteractionFrequent = Verbose;
						}
						else
						{
							printf("\"%s\" is not a VERBOSITY mode (try \"SMS\", \"Terse\", \"Normal\", or \"Verbose\")\n", lParsedCommand.IndirectObjectAdjective);
							continue;
						}
					}
					else
					{
						printf("\"%s\" is not a VERBOSITY trigger (try \"Initial\", \"Subsequent\", or \"Frequent\")\n", lParsedCommand.IndirectObjectAdjective);
						continue;
					}
				}
			}

			printf("Verbostiy: %s:%s:%s\n", VerbosityDescription[lContext.InteractionInitial], VerbosityDescription[lContext.InteractionSubsequent], VerbosityDescription[lContext.InteractionFrequent]);
			next;
		}
		
		if (ClassMatchString(lParsedCommand.Verb, DESCRIBE_WORDS, "") && (*CC_STRING_TERMINATOR == *lParsedCommand.IndirectObjectNoun || ClassMatchString(lParsedCommand.IndirectObjectNoun, SPACE_WORDS, "") ))
        {
            DescribeLocation(&lContext, lLocationDescriber);
			next;
        }

		if (ClassMatchString(lParsedCommand.Verb, DESCRIBE_WORDS, "") && ClassMatchString(lParsedCommand.IndirectObjectNoun, SELF_WORDS, ""))
        {
			printf("You are a magnificent %s named %s\n", lContext.GenderQualifier[lContext.Gender].Type, lContext.Name);
			next;
        }

		if (EqualStringsi("Takeoff", lParsedCommand.Verb) || (EqualStringsi("Take", lParsedCommand.Verb) && EqualStringsi("Off", lParsedCommand.IndirectObjectAdjective)))
        {
            printf("I'm not sure how you're doing this, but\nyou're now hovering 10 inches above the ground.\n");
			next;
        }
		
		if (EqualStringsi("Hop", lParsedCommand.Verb) && *CC_STRING_TERMINATOR == *lParsedCommand.IndirectObjectAdjective)
        {
            LocationLoader(&lContext, &lConnections[NAUGHT][NAUGHT][NAUGHT], &lLocationConstructor, &lLocationValidator, &lLocationDestructor, &lLocationDescriber, &lLocationCommandProcessor, lConnections);

            printf("Are you having fun hopping around the dungeon?\n");
			next;
        }

		if (*CC_STRING_TERMINATOR == *lParsedCommand.Verb && ClassMatchString(lParsedCommand.IndirectObjectAdjective, HEADING_WORDS, ""))
        {
            CopyString("Bolt", lParsedCommand.Verb, sizeof(lParsedCommand.Verb));
        }
		
		if (ClassMatchString(lParsedCommand.Verb, GO_WORDS, ""))
        {
			int x, y, z;

			if (ClassMatchString(lParsedCommand.IndirectObjectAdjective, NWD_HEADING, ""))
			{
				x = NORTH, y = WEST, z = DOWN;
			}
			else if (ClassMatchString(lParsedCommand.IndirectObjectAdjective, NW_HEADING, ""))
			{
				x = NORTH, y = WEST, z = NAUGHT;
			}
			else if (ClassMatchString(lParsedCommand.IndirectObjectAdjective, NWU_HEADING, ""))
			{
				x = NORTH, y = WEST, z = UP;
			}
			else if (ClassMatchString(lParsedCommand.IndirectObjectAdjective, WD_HEADING, ""))
			{
				x = NAUGHT, y = WEST, z = DOWN;
			}
			else if (ClassMatchString(lParsedCommand.IndirectObjectAdjective, W_HEADING, ""))
			{
				x = NAUGHT, y = WEST, z = NAUGHT;
			}
			else if (ClassMatchString(lParsedCommand.IndirectObjectAdjective, WU_HEADING, ""))
			{
				x = NAUGHT, y = WEST, z = UP;
			}
			else if (ClassMatchString(lParsedCommand.IndirectObjectAdjective, SWD_HEADING, ""))
			{
				x = SOUTH, y = WEST, z = DOWN;
			}
			else if (ClassMatchString(lParsedCommand.IndirectObjectAdjective, SW_HEADING, ""))
			{
				x = SOUTH, y = WEST, z = NAUGHT;
			}
			else if (ClassMatchString(lParsedCommand.IndirectObjectAdjective, SWU_HEADING, ""))
			{
				x = SOUTH, y = WEST, z = UP;
			}
			else if (ClassMatchString(lParsedCommand.IndirectObjectAdjective, ND_HEADING, ""))
			{
				x = NORTH, y = NAUGHT, z = DOWN;
			}
			else if (ClassMatchString(lParsedCommand.IndirectObjectAdjective, N_HEADING, ""))
			{
				x = NORTH, y = NAUGHT, z = NAUGHT;
			}
			else if (ClassMatchString(lParsedCommand.IndirectObjectAdjective, NU_HEADING, ""))
			{
				x = NORTH, y = NAUGHT, z = UP;
			}
			else if (ClassMatchString(lParsedCommand.IndirectObjectAdjective, D_HEADING, ""))
			{
				x = NAUGHT, y = NAUGHT, z = DOWN;
			}
			else if (ClassMatchString(lParsedCommand.IndirectObjectAdjective, U_HEADING, ""))
			{
				x = NAUGHT, y = NAUGHT, z = UP;
			}
			else if (ClassMatchString(lParsedCommand.IndirectObjectAdjective, SD_HEADING, ""))
			{
				x = SOUTH, y = NAUGHT, z = DOWN;
			}
			else if (ClassMatchString(lParsedCommand.IndirectObjectAdjective, S_HEADING, ""))
			{
				x = SOUTH, y = NAUGHT, z = NAUGHT;
			}
			else if (ClassMatchString(lParsedCommand.IndirectObjectAdjective, SU_HEADING, ""))
			{
				x = SOUTH, y = NAUGHT, z = UP;
			}
			else if (ClassMatchString(lParsedCommand.IndirectObjectAdjective, NED_HEADING, ""))
			{
				x = NORTH, y = EAST, z = DOWN;
			}
			else if (ClassMatchString(lParsedCommand.IndirectObjectAdjective, NE_HEADING, ""))
			{
				x = NORTH, y = EAST, z = NAUGHT;
			}
			else if (ClassMatchString(lParsedCommand.IndirectObjectAdjective, NEU_HEADING, ""))
			{
				x = NORTH, y = EAST, z = UP;
			}
			else if (ClassMatchString(lParsedCommand.IndirectObjectAdjective, ED_HEADING, ""))
			{
				x = NAUGHT, y = EAST, z = DOWN;
			}
			else if (ClassMatchString(lParsedCommand.IndirectObjectAdjective, E_HEADING, ""))
			{
				x = NAUGHT, y = EAST, z = NAUGHT;
			}
			else if (ClassMatchString(lParsedCommand.IndirectObjectAdjective, EU_HEADING, ""))
			{
				x = NAUGHT, y = EAST, z = UP;
			}
			else if (ClassMatchString(lParsedCommand.IndirectObjectAdjective, SED_HEADING, ""))
			{
				x = SOUTH, y = EAST, z = DOWN;
			}
			else if (ClassMatchString(lParsedCommand.IndirectObjectAdjective, SE_HEADING, ""))
			{
				x = SOUTH, y = EAST, z = NAUGHT;
			}
			else if (ClassMatchString(lParsedCommand.IndirectObjectAdjective, SEU_HEADING, ""))
			{
				x = SOUTH, y = EAST, z = UP;
			}
			else /* NOW HERE */
			{
				x = NAUGHT, y = NAUGHT, z = NAUGHT;
			}

			LocationLoader(&lContext, &lConnections[x][y][z], &lLocationConstructor, &lLocationValidator, &lLocationDestructor, &lLocationDescriber, &lLocationCommandProcessor, lConnections);
			DescribeLocation(&lContext, lLocationDescriber);
			next;
		}

sorry:
		printf("I don't understand (try \"Help\" to list the basic commands)\n");
		continue;

iterator:
		CopyString(lCommand, lLastCommand, sizeof(lLastCommand));
    }

failure:
	printf("Glee! Argh... they got me...\n");
	exit(1);

complete:
	printf("Have a nice danish!\n");
    exit(0);
}
