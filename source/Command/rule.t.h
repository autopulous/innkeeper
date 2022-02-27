/*----------------------------------------------------------------------------
  Neoheurist type header
 
  Copyright 2012 John L. Hart IV. All rights reserved.
 
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
  Neoheurist rule type header file
  ----------------------------------------------------------------------------*/

#ifndef RULE_T_H
#define RULE_T_H

#include "engine.t.h"

#include "motel.compilation.t.h"

/*----------------------------------------------------------------------------
  _ListAdjacentRules()
  ----------------------------------------------------------------------------
  Functions of this type detect neighboring rules
  ----------------------------------------------------------------------------
  Parameters:

  pAssociations - (I) 3 x 3 x 3 cube of adjacent rules
  ----------------------------------------------------------------------------
  Notes:

  Implementation of this function is the responsibility of the rule engine
  ---------------------------------------------------------------------------*/

typedef void (CALLING_CONVENTION * _ListAdjacentRules)
(
    Associations pAssociations
);

/*----------------------------------------------------------------------------
  _RuleConstructor()
  ----------------------------------------------------------------------------
  Functions of this type initialize or reload the state of a rule
  ----------------------------------------------------------------------------
  Parameters:

  pContext - (I) execution context (travels from state to state as executed)
  ----------------------------------------------------------------------------
  Notes:

  Implementation of this function is the responsibility of each rule
  ---------------------------------------------------------------------------*/

#define RULE_CONSTRUCTOR_FUNCTION "RuleConstructor"

typedef void (CALLING_CONVENTION * _RuleConstructor)
(
    Context * pContext
);

/*----------------------------------------------------------------------------
  _RuleDestructor()
  ----------------------------------------------------------------------------
  Functions of this type persist and finalize the state of a rule
  ----------------------------------------------------------------------------
  Parameters:

  pContext - (I) execution context (travels from state to state as executed)
  ----------------------------------------------------------------------------
  Notes:

  Implementation of this function is the responsibility of each rule
  ---------------------------------------------------------------------------*/

#define RULE_DESTRUCTOR_FUNCTION "RuleDestructor"

typedef void (CALLING_CONVENTION * _RuleDestructor)
(
    Context * pContext
);

/*----------------------------------------------------------------------------
  _RuleValidator()
  ----------------------------------------------------------------------------
  Functions of this type validate the logical consistency of a rule in regard
  to a given context
  ----------------------------------------------------------------------------
  Parameters:

  pContext - (I) execution context (travels from state to state as executed)
  ----------------------------------------------------------------------------
  Notes:

  Implementation of this function is the responsibility of each rule
  ---------------------------------------------------------------------------*/

#define RULE_VALIDATOR_FUNCTION "RuleValidator"

typedef void (CALLING_CONVENTION * _RuleValidator)
(
    Context * pContext
);

/*----------------------------------------------------------------------------
  _RuleDescriber()
  ----------------------------------------------------------------------------
  Functions of this type output the state a rule in regard to a given context
  ----------------------------------------------------------------------------
  Parameters:

  pContext - (I) execution context (travels from state to state as executed)
  ----------------------------------------------------------------------------
  Notes:

  Implementation of this function is the responsibility of each rule
  ---------------------------------------------------------------------------*/

#define RULE_DESCRIBER_FUNCTION "RuleDescriber"

typedef void (CALLING_CONVENTION * _RuleDescriber)
(
    Context *,
    _ListAdjacentRules
);

/*----------------------------------------------------------------------------
  _RuleCommandProcessor()
  ----------------------------------------------------------------------------
  Functions of this type alter the state of the rule and context
  ----------------------------------------------------------------------------
  Parameters:

  pContext - (I) execution context (travels from state to state as executed)
  ----------------------------------------------------------------------------
  Notes:

  Implementation of this function is the responsibility of each rule
  ---------------------------------------------------------------------------*/

#define RULE_COMMAND_PROCESSOR_FUNCTION "RuleCommandProcessor"

typedef bool (CALLING_CONVENTION * _RuleCommandProcessor)
(
    Context * pContext,
    char *
);

#endif
