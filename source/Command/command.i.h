/*----------------------------------------------------------------------------
  Neoheurist keyboard command processor interface header

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
  Command application programmer's interface (API) header file
  ----------------------------------------------------------------------------*/

#ifndef COMMAND_I_H
#define COMMAND_I_H

  /*----------------------------------------------------------------------------
  AllocateCommandProcessor()
  ----------------------------------------------------------------------------
  Create a processor to handle keyboard input with a command history
  ----------------------------------------------------------------------------
  Parameters:
  ----------------------------------------------------------------------------
  Returns:

  A handle to a CommandProcessor structure for use in calls to GetCommand()
  ---------------------------------------------------------------------------*/

CommandProcessor * CALLING_CONVENTION AllocateCommandProcessor
(
);

/*----------------------------------------------------------------------------
  GetCommand()
  ----------------------------------------------------------------------------
  Accept a string from stdin, remove control characters, eliminate whitespace
  ----------------------------------------------------------------------------
  Parameters:

  pCommandProcessor - (I) Command history (up and down arrow scrolling)
  ----------------------------------------------------------------------------
  Returns:

  A string containing the user command
  ---------------------------------------------------------------------------*/

char * CALLING_CONVENTION GetCommand
(
    CommandProcessor * pCommandProcessor
);

/*----------------------------------------------------------------------------
  TokenizeCommand()
  ----------------------------------------------------------------------------
  Each word within the request string is turned into a token (string)
  ----------------------------------------------------------------------------
  Parameters:

  pX     - (IO)
  ----------------------------------------------------------------------------
  Returns:

  TRUE  - the command was syntactically well formed
  FALSE - the command was not syntactically well formed (but may be usable)
  ---------------------------------------------------------------------------*/

void CALLING_CONVENTION TokenizeCommand
(
	char * pCommandString,
	Tokens * pCommandTokens
);

/*----------------------------------------------------------------------------
  ParseTokenizedCommand()
  ----------------------------------------------------------------------------
  Break command into parts of speech according to scheme
  ----------------------------------------------------------------------------
  Parameters:

  pX     - (IO)
  ----------------------------------------------------------------------------
  Returns:

  TRUE  - the command was syntactically well formed
  FALSE - the command was not syntactically well formed (but may be usable)
  ---------------------------------------------------------------------------*/

char * CALLING_CONVENTION ParseTokenizedCommand
(
    Tokens pCommandTokens,
    CommandTree pCommands,
    Command * pParsedCommandTokens
);

#endif
