#include <stdio.h>

#include "../xxx.yyy.zzz/xxx.yyy.zzz.i.h"

#include "../Motel/motel.types.t.h"

#include "../Motel/motel.memory.i.h"

#include "../Motel/motel.string.t.h"
#include "../Motel/motel.string.i.h"

#include "../Motel/motel.questa.t.h"
#include "../Motel/motel.questa.i.h"

#include "../Command/command.t.h"
#include "../Command/command.i.h"

#include "../Main/types.t.h"
#include "../State/state.i.h"

#define LOCATION_NAME "Low Powder Room"

#define OBJECT_ID "4123D98A-6676-4F5E-8BC4-3CF2DBD5ECf4"

#define LOCATION_ID "555.553.553"

const Volume LocationCapacity = 77;

typedef struct State
{
    Volume itemsVolume;
    Items itemsPresent;
    motelQuestaHandle itemList;
} State;

const char * Verbs[] = { "" };
const char * DirectObjectArticles[] = { "" };
const char * DirectObjectAdjectives[] = { "" };
const char * DirectObjectNouns[] = { "" };
const char * Prepositions[] = { "" };
const char * IndirectObjectArticles[] = { "" };
const char * IndirectObjectAdjectives[] = { "" };
const char * IndirectObjectNouns[] = { "" };

CommandTree Commands = { Verbs, DirectObjectArticles, DirectObjectAdjectives, DirectObjectNouns, Prepositions, IndirectObjectArticles, IndirectObjectAdjectives, IndirectObjectNouns };
