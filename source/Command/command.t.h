/*----------------------------------------------------------------------------
  Neoheurist keyboard command processor type header

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
  Command application programmer's types (APT) header file
  ----------------------------------------------------------------------------*/

#ifndef COMMAND_T_H
#define COMMAND_T_H

#define BELL '\a'
#define BACKSPACE '\b'
#define NEWLINE '\n'
#define RETURN '\r'

#define COMMAND_HISTORY_SIZE 50
#define COMMAND_SIZE 512

/*
   Holds the command history to enable reuse of previously issued commands
*/

typedef struct CommandProcessor {
    char   Command[COMMAND_HISTORY_SIZE][COMMAND_SIZE];
    size_t OldestCommandCursor;
    size_t NextCommandCursor;
} CommandProcessor;

/*
   Holds pointers to the tokenized command string with up to eight token/substrings to be stored within the Command structure
*/

typedef struct Tokens {
	char Token[8][COMMAND_SIZE];
} Tokens;

/*
   [VERB [[[ARTICLE] [ADJECTIVE] NOUN] [PREPOSITION [ARTICLE] [ADJECTIVE] NOUN]]] || [[ARTICLE] [ADJECTIVE] NOUN]

   The SUBJECT is always assumed to be the character and is never referenced,
   for example the command "TAKE BLUE BALL" means "ATTEMPT TO take THE blue ball"

   In the case of direct object only commands there is an implied action determined
   by the command processor. For example the command "EAST" means "GO EAST" or more
   full "ATTEMPT TO GO east"

   Implied actions may be dependent upon the item of the object or the player for
   example the command "LAMP" could mean "TAKE LAMP" or "LIGHT LAMP" or "EXTINGUISH LAMP"
   depending upon whether the player posesses the lamp or whether the lamp is lit
*/

typedef struct Command {
    char Verb[COMMAND_SIZE];
    char DirectObjectArticle[COMMAND_SIZE];
    char DirectObjectAdjective[COMMAND_SIZE];
    char DirectObjectNoun[COMMAND_SIZE];
    char Preposition[COMMAND_SIZE];
    char IndirectObjectArticle[COMMAND_SIZE];
    char IndirectObjectAdjective[COMMAND_SIZE];
    char IndirectObjectNoun[COMMAND_SIZE];
} Command;

/*
    Lists of words that work together to define command actions
*/

typedef struct CommandTree {
	const char ** Verbs;
	const char ** DirectObjectArticles;
	const char ** DirectObjectAdjectives;
	const char ** DirectObjectNouns;
	const char ** Prepositions;
	const char ** IndirectObjectArticles;
	const char ** IndirectObjectAdjectives;
	const char ** IndirectObjectNouns;
} CommandTree;

#endif
