/*----------------------------------------------------------------------------
  Neoheurist keyobard command processor

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
  Neoheurist command processor application programmer's interface (API) implementation file
  ----------------------------------------------------------------------------*/

#include "command.h"

/*----------------------------------------------------------------------------
  Embedded copyright
  ----------------------------------------------------------------------------*/

static const char *gCopyright UNUSED = "@(#)command.c - Copyright 2011-2015 John L. Hart IV - All rights reserved";

/*----------------------------------------------------------------------------
  Public functions
  ----------------------------------------------------------------------------*/

CommandProcessor * CALLING_CONVENTION AllocateCommandProcessor()
{
    CommandProcessor * lCommandProcessor = NULL;

    SafeCallocBlock(&lCommandProcessor, sizeof(CommandProcessor));

    return lCommandProcessor;
}

char * CALLING_CONVENTION GetCommand
(
    CommandProcessor * pCommandProcessor
)
{
    /************************************************************************************
         This function is severely broken - mid-refactoring to use pCommandProcessor
    *************************************************************************************/

    size_t lCommandCursor = pCommandProcessor->NextCommandCursor;

    char * lCommandString = pCommandProcessor->Command[lCommandCursor];

    char * lCharacterCursor = lCommandString;
    char * lLaggingCharacterCursor = lCharacterCursor;

    *lCharacterCursor = *CC_STRING_TERMINATOR;

    size_t lStringLength = COMMAND_SIZE - 1;

    char lCharacter;

    for (;;)
    {
        lCharacter = _getch();

        /* Handle multi-part non-text keystroke characters */

        if (*CC_STRING_TERMINATOR >= lCharacter)
        {
            char lControlCharacter = _getch();

#if defined _WIN32 || defined _WIN64
            if (-32 == lCharacter)
            {
                if ('H' == lControlCharacter) // Up Arrow key
                {
                    if (pCommandProcessor->OldestCommandCursor == lCommandCursor)
                    {
                        continue;
                    }

                    if (0 == lCommandCursor)
                    {
                        lCommandCursor = COMMAND_HISTORY_SIZE;
                    }

                    lCommandCursor--;
                }
                else if ('P' == lControlCharacter) // Down Arrow key
                {
                    if (pCommandProcessor->NextCommandCursor == lCommandCursor)
                    {
                        continue;
                    }

                    lCommandCursor++;

                    if (pCommandProcessor->NextCommandCursor == lCommandCursor)
                    {
                        Erase(lCommandString, lCharacterCursor);

                        *lCommandString = *CC_STRING_TERMINATOR;

                        lLaggingCharacterCursor = lCommandString;
                        lCharacterCursor = lCommandString;
                    }

                    if (COMMAND_HISTORY_SIZE == lCommandCursor)
                    {
                        lCommandCursor = 0;
                    }
                }
                else
                {
                    continue;
                }

                Erase(lCommandString, lCharacterCursor);

                /* select and display the command pointed to by the command cursor */

                CopyString(pCommandProcessor->Command[lCommandCursor], lCommandString, COMMAND_SIZE);

                printf("%s", lCommandString);

                /* move the lagging character cursor and the character cursor to the end of the command */

                lCharacterCursor = lCommandString + strlen(lCommandString);

                lLaggingCharacterCursor = lCharacterCursor + 1;

                if (lCommandString < lLaggingCharacterCursor)
                {
                    lLaggingCharacterCursor--;
                }
            }
#endif

            continue;
        }

        /* Handle the enter key */

        if (NEWLINE == lCharacter || RETURN == lCharacter)
        {
            if (*CC_STRING_TERMINATOR == *lCommandString)
            {
                if (0 == lCommandCursor)
                {
                    lCommandCursor = COMMAND_HISTORY_SIZE;
                }

                lCommandCursor--;

                /* select and display the command pointed to by the command cursor */

                CopyString(pCommandProcessor->Command[lCommandCursor], lCommandString, COMMAND_SIZE);

                printf("%s\n", lCommandString);

                /* move the lagging character cursor and the character cursor to the end of the command */

                lCharacterCursor = lCommandString + strlen(lCommandString);

                lLaggingCharacterCursor = lCharacterCursor + 1;

                if (lCommandString < lLaggingCharacterCursor)
                {
                    lLaggingCharacterCursor--;
                }
            }
            else
            {
                _putch(NEWLINE);

                if (*CC_STRING_TERMINATOR != *lLaggingCharacterCursor && ' ' != *lLaggingCharacterCursor)
                {
                    lLaggingCharacterCursor++;
                }
            }

            break;
        }

        /* Handle the backspace key */

        if (BACKSPACE == lCharacter)
        {
            if (lCommandString == lCharacterCursor)
            {
                _putch(BELL);
                continue;
            }

            Backspace(&lCharacterCursor);

            if (lCharacterCursor < lLaggingCharacterCursor)
            {
                lLaggingCharacterCursor = lCharacterCursor;
            }

            continue;
        }

        /* Handle punctuation */

        if ('.' != (int) lCharacter && ',' != (int) lCharacter && '-' != (int) lCharacter && '?' != (int) lCharacter && ispunct((int) lCharacter))
        {
            /* Treat any non (period, comma, or hyphen) punctuation character as a space */

            lCharacter = ' ';
        }

        /* Handle whitespace keystrokes */

        if (isspace((int) lCharacter))
        {
            /* Treat any whitespace as a space */

            lCharacter = ' ';

            /* Throw out leading spaces */

            if (lCommandString == lCharacterCursor)
            {
                continue;
            }

            /* Throw out redundant spaces */

            if (isspace((int) *lLaggingCharacterCursor))
            {
                continue;
            }
        }

        /* Handle a full input buffer */

        if (lStringLength == (lLaggingCharacterCursor - lCommandString))
        {
            _putch(BELL);
            continue;
        }

        /* Process the character */

        _putch(lCharacter);

        lLaggingCharacterCursor = lCharacterCursor;

        *lLaggingCharacterCursor = lCharacter;

        lCharacterCursor++;
    }

    /* Terminate input string - possibly trimming trailing spaces */

    *lLaggingCharacterCursor = *CC_STRING_TERMINATOR;

    /* update command history cursors */

    if (COMMAND_HISTORY_SIZE == ++pCommandProcessor->NextCommandCursor)
    {
        pCommandProcessor->NextCommandCursor = 0;
    }

    if (pCommandProcessor->OldestCommandCursor == pCommandProcessor->NextCommandCursor)
    {
        if (COMMAND_HISTORY_SIZE == ++pCommandProcessor->OldestCommandCursor)
        {
            pCommandProcessor->OldestCommandCursor = 0;
        }
    }

    /* return the command */

    return (lCommandString);
}

void CALLING_CONVENTION TokenizeCommand
(
	char * pCommandString,
	Tokens * pCommandTokens
)
{
	int token = 0;
	int character = 0;

	/* Initialize the token structure */

	memset(pCommandTokens, (int) *CC_STRING_TERMINATOR, sizeof(Tokens));

	/* each word within the command string is a token (space deliniated) */
			
   	loop
    {
        if (*CC_STRING_TERMINATOR == pCommandString[character])
		{
			character++;

			if (*CC_STRING_TERMINATOR != *pCommandString)
			{
				CopyString(pCommandString, pCommandTokens->Token[token++], min(character, COMMAND_SIZE));
			}

			break;
		}
		else if (' ' == pCommandString[character])
        {
			character++;

			CopyString(pCommandString, pCommandTokens->Token[token++], min(character, COMMAND_SIZE));
			pCommandString += character;
			character = 0;
        }
		else
		{
			character++;
		}
    }
}

char * CALLING_CONVENTION ParseTokenizedCommand
(
    Tokens pCommandTokens,
    CommandTree pCommands,
    Command * pParsedCommandTokens
)
{
    int token = 0;

    /* Initialize the parsed command structure */

    memset(pParsedCommandTokens, (int) *CC_STRING_TERMINATOR, sizeof(Command));

    /* Look for a verb */

    if (MatchToken(pCommandTokens.Token[token], pCommands.Verbs, pParsedCommandTokens->Verb))
    {
		CopyString(pCommandTokens.Token[token++], pParsedCommandTokens->Verb, sizeof(pParsedCommandTokens->Verb));

        if (*CC_STRING_TERMINATOR == *pCommandTokens.Token[token])
        {
            return ((char *) NULL);
        }
    }

    /* Look for a direct object article */

    if (MatchToken(pCommandTokens.Token[token], pCommands.DirectObjectArticles, pParsedCommandTokens->DirectObjectArticle))
    {
		CopyString(pCommandTokens.Token[token++], pParsedCommandTokens->DirectObjectArticle, sizeof(pParsedCommandTokens->DirectObjectArticle));

        if (*CC_STRING_TERMINATOR == *pCommandTokens.Token[token])
        {
            return ((char *) NULL);
        }
    }

    /* Look for a direct object adjective */

    if (MatchToken(pCommandTokens.Token[token], pCommands.DirectObjectAdjectives, pParsedCommandTokens->DirectObjectAdjective))
    {
		CopyString(pCommandTokens.Token[token++], pParsedCommandTokens->DirectObjectAdjective, sizeof(pParsedCommandTokens->DirectObjectAdjective));

        if (*CC_STRING_TERMINATOR == *pCommandTokens.Token[token])
        {
            return ((char *) NULL);
        }
    }

    /* Look for a direct object */

    if (MatchToken(pCommandTokens.Token[token], pCommands.DirectObjectNouns, pParsedCommandTokens->DirectObjectNoun))
    {
		CopyString(pCommandTokens.Token[token++], pParsedCommandTokens->DirectObjectNoun, sizeof(pParsedCommandTokens->DirectObjectNoun));

        if (*CC_STRING_TERMINATOR == *pCommandTokens.Token[token])
        {
            return ((char *) NULL);
        }
    }

    /* Look for a preposition */

    if (MatchToken(pCommandTokens.Token[token], pCommands.Prepositions, pParsedCommandTokens->Preposition))
    {
		CopyString(pCommandTokens.Token[token++], pParsedCommandTokens->Preposition, sizeof(pParsedCommandTokens->Preposition));

        if (*CC_STRING_TERMINATOR == *pCommandTokens.Token[token])
        {
            return ((char *) NULL);
        }
    }

    /* Look for a indirect object article */

    if (MatchToken(pCommandTokens.Token[token], pCommands.IndirectObjectArticles, pParsedCommandTokens->IndirectObjectArticle))
    {
		CopyString(pCommandTokens.Token[token++], pParsedCommandTokens->IndirectObjectArticle, sizeof(pParsedCommandTokens->IndirectObjectArticle));

        if (*CC_STRING_TERMINATOR == *pCommandTokens.Token[token])
        {
            return ((char *) NULL);
        }
    }

    /* Look for a indirect object adjective */

    if (MatchToken(pCommandTokens.Token[token], pCommands.IndirectObjectAdjectives, pParsedCommandTokens->IndirectObjectAdjective))
    {
		CopyString(pCommandTokens.Token[token++], pParsedCommandTokens->IndirectObjectAdjective, sizeof(pParsedCommandTokens->IndirectObjectAdjective));

        if (*CC_STRING_TERMINATOR == *pCommandTokens.Token[token])
        {
            return ((char *) NULL);
        }
    }

    /* Look for an indirect object */

	if (MatchToken(pCommandTokens.Token[token], pCommands.IndirectObjectNouns, pParsedCommandTokens->IndirectObjectNoun))
    {
		CopyString(pCommandTokens.Token[token++], pParsedCommandTokens->IndirectObjectNoun, sizeof(pParsedCommandTokens->IndirectObjectNoun));

        if (*CC_STRING_TERMINATOR == *pCommandTokens.Token[token])
        {
            return ((char *) NULL);
        }
    }

    return (pCommandTokens.Token[token]);
}

static void Erase
(
    char * pCommandString,
    char * pCharacterCursor
)
{
    while (pCommandString < pCharacterCursor)
    {
        Backspace(&pCharacterCursor);
    }
}

static void Backspace
(
    char ** pCharacterCursor
)
{
    _putch(BACKSPACE);
    _putch(' ');
    _putch(BACKSPACE);

    (*pCharacterCursor)--;
}

static boolean MatchToken
(
    char * pSearchToken,
    const char * pCommandTokens[],
    char * pFoundToken
)
{
    int lIndex;

    for (lIndex = 0; *CC_STRING_TERMINATOR != *pCommandTokens[lIndex]; lIndex++)
    {
        if (EqualStringsi(pCommandTokens[lIndex], pSearchToken))
        {
            CopyString(pCommandTokens[lIndex], pFoundToken, COMMAND_SIZE);
            return (TRUE);
        }
    }

    return (FALSE);
}
