/*----------------------------------------------------------------------------
  Motel Questa

  application programmer's types (APT) header file 
  ----------------------------------------------------------------------------
  Copyright 2010-2011 John L. Hart IV. All rights reserved.
 
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

#ifndef MOTEL_QUESTA_T_H
#define MOTEL_QUESTA_T_H

/*----------------------------------------------------------------------------
  Establish pseudo-encapsulation
  ----------------------------------------------------------------------------*/

#ifndef MUTABILITY
#define MUTABILITY const
#endif

/*
** Public Questa members
*/

typedef enum motelQuestaMember motelQuestaMember;

enum motelQuestaMember
{
	motelQuestaMember_Result,            /*!< Data type:   (motelResult *)
                                              Description: The method result code */

	motelQuestaMember_Size,              /*!< Data type:   (size_t *)
                                              Description: Memory currently allocated by the questa object */

	motelQuestaMember_Nodes,             /*!< Data type:   (unsigned long *)
                                              Description: The number of nodes in the questa */

	motelQuestaMember_Head,              /*!< Data type:   NULL
                                              Description: Set the node cursor to the head of the questa */

	motelQuestaMember_Tail,              /*!< Data type:   NULL
                                              Description: Set the node cursor to the tail of the questa */

	motelQuestaMember_Previous,          /*!< Data type:   NULL
                                              Description: Move the node cursor toward the head of the questa */

	motelQuestaMember_Next,              /*!< Data type:   NULL
                                              Description: Move the node cursor toward the tail of the questa */

	motelQuestaMember_
};

typedef struct motelQuestaNode motelQuestaNode;
typedef motelQuestaNode * motelQuestaNodeHandle;

struct motelQuestaNode
{
	MUTABILITY motelQuestaNodeHandle previous;
	MUTABILITY motelQuestaNodeHandle subsequent;

	MUTABILITY void * MUTABILITY data;
};

typedef struct motelQuesta motelQuesta;
typedef motelQuesta * motelQuestaHandle;

struct motelQuesta
{
	MUTABILITY motelResult result;

	MUTABILITY unsigned long nodes;

	MUTABILITY size_t size;
	MUTABILITY size_t maximumSize;

	MUTABILITY size_t dataSize;

	MUTABILITY motelQuestaNodeHandle head;
	MUTABILITY motelQuestaNodeHandle tail;

	MUTABILITY motelQuestaNodeHandle cursor;
};

#endif
