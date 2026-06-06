---
date: 2026-06-06T10:57:36Z
researcher: John Hart
git_commit: 4b5babf3bd154c1f2d3db82e1f5949fd7006ba61
branch: main
repository: autopulous/innkeeper
topic: "Design evaluation of the Innkeeper / Neoheurist text-adventure platform with an eye toward refactoring to an initial release"
tags: [research, architecture, c, windows, dll-plugin, text-adventure, motel, neoheurist, build-system, portability]
status: complete
last_updated: 2026-06-06
last_updated_by: John Hart
---

# Research: Design evaluation of the Innkeeper / Neoheurist "infinitely extensible" text-adventure platform

**Permalink base for all `file:line` references below:**
`https://github.com/autopulous/innkeeper/blob/4b5babf3bd154c1f2d3db82e1f5949fd7006ba61/`

## Research Question

Evaluate the design of this stub implementation of what is intended to be an *infinitely extensible text-based adventure game platform*, with the intention of potentially refactoring the design with the goal of bringing the application to an initial release.

## Summary

Innkeeper (engine name **Neoheurist**) is a ~16,700-LOC Windows-only C project, salvaged from SourceForge (last real commit 2022-02-27, code copyrights 2010–2015). Its defining architectural idea is genuinely novel: **every game location is a separate Windows DLL whose filename is its 3D grid coordinate** (`555.555.555.dll` = "Barracks"/HOME). The engine walks a 3D integer grid by probing for neighbor DLLs with `LoadLibrary`, then resolves a fixed five-function contract (`LocationConstructor`/`Destructor`/`Validator`/`Describer`/`CommandProcessor`) via `GetProcAddress`. Items use the same contract but are named by UUID. This is the "infinitely extensible" mechanism: drop a new DLL into the directory and a new room exists. It sits on a hand-rolled foundation library, **Motel** (memory, string, balanced tree, deque "questa", xorshift PRNG "aleatory"), and a natural-language command parser that slots tokens into parts of speech (Verb / Direct-Object Article-Adjective-Noun / Preposition / Indirect-Object …).

**The design is conceptually strong and unusually clean in its layering, but the implementation is a mid-refactor stub that cannot currently build.** The honest assessment for an "initial release" goal:

- **Keep:** the DLL-per-location plugin contract (it's the soul of the project), the four-layer header convention (`.t.h`/`.i.h`/`.h`/`.c`), the Motel data-structure library, the parts-of-speech parser concept.
- **Fix before anything else (build blockers):** a solution that references a non-existent project directory, two placeholder "template" projects with no source compiled into the build, a missing `Item` project, stale `v141`/SDK `10.0.17763.0` pinning, and `GetCommand` self-described in-source as "severely broken — mid-refactoring."
- **Reckon with structurally:** total Windows lock-in (`LoadLibrary`/`HMODULE`/`TCHAR`/`__stdcall`), `LoadLibrary`/`FreeLibrary` of up to 26 neighbor DLLs *on every move*, no save/load persistence (all state dies with the process), hardcoded compile-time vocabulary, a tokenizer/parser data-flow bug (trailing-space tokens won't match), no test suite, and several known-stub functions (`SelectQuestaNode`, the OO `union string`, `motel.io`, `motel.signal`).

The gap between the **vision** (documented heavily in `documentation/reference/`: a Word spec, a 3.3 MB Sparx EA model, a "Smart Interface Pattern" spreadsheet, a Visio "smart pool") and the **implemented stub** is the central risk. The vision is ambitious; the code is an early, partially-wired prototype.

## Detailed Findings

### 1. The extensibility model — locations and items as coordinate/UUID-named DLLs

This is the architecturally distinctive core and the literal meaning of "infinitely extensible."

- **The contract.** Every location DLL must export five functions, resolved by name in `LocationLoader` at [`source/Main/main.c#L525-L529`](https://github.com/autopulous/innkeeper/blob/4b5babf3bd154c1f2d3db82e1f5949fd7006ba61/source/Main/main.c#L525). The names come from string macros in [`source/xxx.yyy.zzz/xxx.yyy.zzz.t.h`](https://github.com/autopulous/innkeeper/blob/4b5babf3bd154c1f2d3db82e1f5949fd7006ba61/source/xxx.yyy.zzz/xxx.yyy.zzz.t.h): `LocationConstructor`, `LocationDestructor`, `LocationValidator`, `LocationDescriber`, `LocationCommandProcessor`. If any of the five is NULL, the engine prints `Emptiness()` and refuses the move (`main.c#L532-L537`). The exports are declared in the shared [`source/xxx.yyy.zzz/xxx.yyy.zzz.def`](https://github.com/autopulous/innkeeper/blob/4b5babf3bd154c1f2d3db82e1f5949fd7006ba61/source/xxx.yyy.zzz/xxx.yyy.zzz.def), which every concrete location `.vcxproj` points its linker at via `$(SolutionDir)xxx.yyy.zzz\xxx.yyy.zzz.def`.

- **The grid.** Coordinates are `{int X, Y, Z}` ([`main.t.h#L86-L91`](https://github.com/autopulous/innkeeper/blob/4b5babf3bd154c1f2d3db82e1f5949fd7006ba61/source/Main/main.t.h#L86)) with axis constants `WEST/EAST/SOUTH/NORTH/DOWN/UP` and `NAUGHT=1` center. `HOME = {555,555,555}`, `NOWHERE = {000,000,000}` (`main.c#L82-L83`). Hard bounds are `000..888` per axis (`main.c#L421-L443`). A coordinate component of `0` means "no location there" — `LocationLoader` rejects it with "Cannot traverse in that direction" (`main.c#L512-L517`).

- **Neighbor discovery.** On every move, `ConnectAdjacentLocations` ([`main.c#L370-L488`](https://github.com/autopulous/innkeeper/blob/4b5babf3bd154c1f2d3db82e1f5949fd7006ba61/source/Main/main.c#L370)) zeroes the 3×3×3 `Connections` array, then loops the 26 surrounding cells, builds each candidate filename with `wsprintf("%d.%d.%d.dll", …)` (`main.c#L469`), and calls `LoadLibrary`/`FreeLibrary` on each just to test existence (`main.c#L471-L475`). Existing neighbors get their coordinates recorded.

- **Sample world.** Five rooms exist: `555.553.553` Low Powder Room, `555.554.554` Powder Room, `555.555.553` Changing room, `555.555.554` Tiny bedroom, `555.555.555` Barracks (HOME). The Barracks ([`source/555.555.555/555.555.555.c`](https://github.com/autopulous/innkeeper/blob/4b5babf3bd154c1f2d3db82e1f5949fd7006ba61/source/555.555.555/555.555.555.c)) is the richest — a stateful Trophy Case with open/close/lock/unlock/damage/repair and probabilistic damage messages. The Changing room mutates `pContext->Gender`/`Name`; the Tiny bedroom delegates to the Sleep module.

- **Items.** A parallel template, the GUID-named [`source/xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx/`](https://github.com/autopulous/innkeeper/blob/4b5babf3bd154c1f2d3db82e1f5949fd7006ba61/source/xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx), defines `Item`/`Container` structs and the five `Item*` exports. An item DLL would be named by its UUID (`37073B4A-….dll`). The older [`source/Item/`](https://github.com/autopulous/innkeeper/blob/4b5babf3bd154c1f2d3db82e1f5949fd7006ba61/source/Item) directory has the headers/.def but **no `.c` and no `.vcxproj`** — it predates the GUID template.

**Author ergonomics (a release concern):** creating one room requires copying two template directories, renaming 6+ files plus the `.vcxproj` GUID, supplying a hand-typed unique `OBJECT_ID` UUID, defining a `State` struct, implementing six functions, and registering the project in the `.sln`. There is no scaffolding tool.

### 2. State persistence — in-memory only, UUID-keyed

Location/item state is decoupled from the DLL lifecycle, which is elegant: state lives in two `motelTree` handles on the `Context` (`ItemStates`, `LocationStates`), keyed by UUID string, constructed in `main` at [`main.c#L603-L609`](https://github.com/autopulous/innkeeper/blob/4b5babf3bd154c1f2d3db82e1f5949fd7006ba61/source/Main/main.c#L603). The [`source/State/state.c`](https://github.com/autopulous/innkeeper/blob/4b5babf3bd154c1f2d3db82e1f5949fd7006ba61/source/State/state.c) module provides `StateConstructor` (cache-miss factory + insert), `StateRetriever`, `StateUpdater`. Because state is keyed by UUID rather than DLL handle, a room's state **survives unload/reload** — revisiting a room finds its prior state. **But there is no serialization**: process exit loses everything. A shippable game needs save/load — and the `motel.io.*` files that would presumably back it are explicit stubs (`motel.io.i.h:38` — "todo: find a reason for this file or eliminate it").

Note: `StateConstructor`/`Retriever`/`Updater` only ever touch `LocationStates`; there is no item-state API symmetric to it, even though `ItemStates` exists on the `Context`.

### 3. The Motel foundation library

A consistent, hand-rolled C library under [`source/Motel/`](https://github.com/autopulous/innkeeper/blob/4b5babf3bd154c1f2d3db82e1f5949fd7006ba61/source/Motel), five subsystems, 78 exports in [`motel.def`](https://github.com/autopulous/innkeeper/blob/4b5babf3bd154c1f2d3db82e1f5949fd7006ba61/source/Motel/motel.def):

- **memory** — null-guarded malloc/calloc/realloc/free wrappers, "managed" variants that maintain an external byte counter, bounds-checked `CopyBlock`/`CompareBlocks`.
- **string** — a full NUL-safe replacement string library (`CopyString`, `AppendString`, trims, character-class predicates, a `tr`-style translator, variadic `ClassMatchString`/`ClassConformString`, `SearchString`, `ParseString`, case-insensitive compare). Optional `Motel_`-prefixed namespace. **Contains an OO `union string` type whose `Allocate`/`Deallocate`/`Fill` method bodies are empty stubs** (`motel.string.c:1001-1023`) and a likely loop-logic inversion in `FindClassMemberCharacter` (`motel.string.c:351-354`).
- **tree** — a *weight-balanced* BST (counts virtual null-leaf nodes per branch; rotates when ratio ≥ 3), cursor-based traversal, duplicate keys via instance counters, configurable memory cap. Uses arboricultural vocabulary (sprout/sapling/trunk/fork/twig/leaf).
- **questa** — doubly-linked list usable as list/stack/queue via head/tail push/pull aliases. **`SelectQuestaNode` returns `Unimplemented`** (`motel.questa.c:597-615`); `PullBottomQuestaNode` alias appears mis-mapped to `PullHead…`.
- **aleatory** — Vigna xorshift1024\*/4096\* PRNGs plus probability-distribution selectors; used by the Gender module for randomized synonym output. **Module-global PRNG state → not thread-safe.**

**Cross-cutting:** every handle carries a `motelResult result`; a `Set/GetMember` enum-dispatch pattern exposes internal fields as pseudo-properties; every `.c` has a `@(#)` what-string copyright. **None of the five subsystems is thread-safe** (mutable cursor/result state per handle; global PRNG state).

### 4. The header-layering convention (a genuine strength)

A disciplined four-file pattern, applied consistently across the whole codebase:

- `.t.h` — **A**pplication **P**rogrammer's **T**ypes: structs/enums/constants only, no prototypes.
- `.i.h` — **A**pplication **P**rogrammer's **I**nterface: public prototypes (+ convenience macros) only.
- `.h` — private implementation header, included only by its `.c`.
- `.c` — implementation; includes its own `.h` first.
- `.def` — Windows module-definition export list.

A clever `MUTABILITY` trick ties `.t.h`↔`.h`: in `.t.h` it defaults to `const` (handles look immutable to callers); the `.h` redefines it empty before including the `.t.h`, so the `.c` gets write access. This is real, intentional encapsulation discipline in C — worth preserving in any refactor.

### 5. The command parser & control-flow style

- **Pipeline:** `GetCommand` (raw keystrokes via Windows `_getch`, with a 50-entry command-history ring) → `TokenizeCommand` (split into ≤8 tokens) → `ParseTokenizedCommand` (slot tokens into 8 parts-of-speech against a per-context `CommandTree` of null-sentinel word-lists). Structures in [`source/Command/command.t.h`](https://github.com/autopulous/innkeeper/blob/4b5babf3bd154c1f2d3db82e1f5949fd7006ba61/source/Command/command.t.h).
- **Grammar:** `[VERB [[[ARTICLE][ADJECTIVE]NOUN][PREPOSITION[ARTICLE][ADJECTIVE]NOUN]]]`; subject is always the implicit player. Bare noun phrases imply an action (`EAST` ⇒ `GO EAST`).
- **Per-location vocabulary:** the engine hands the raw `Tokens` to the location's `LocationCommandProcessor` *first* (`main.c#L655`); each room parses against its *own* `CommandTree`, so vocabulary is location-scoped. The engine only parses if the room returns `FALSE`.
- **Control flow:** custom `goto`-macros defined in [`source/Motel/motel.types.t.h#L109-L119`](https://github.com/autopulous/innkeeper/blob/4b5babf3bd154c1f2d3db82e1f5949fd7006ba61/source/Motel/motel.types.t.h#L109): `same`→`goto reprocess`, `next`→`goto iterator`, `what`→`goto sorry`, `fail`→`goto failure`, `done`→`goto complete`, plus `loop`=`for(;;)` and `escape(x)`=`if(x)break`. The entire ~550-line main loop is one `for(;;)` with five labels. Readable once learned, but idiosyncratic and a maintenance/onboarding cost.

### 6. The `.motel` data format and the "MOTEL/Neoheurist/Smart Interface" vision

`.motel` is a hierarchical colon-and-dot config language: lines like `:DirectObject.Adjective:verbose`, `::` comments, optional type tags (`b`/`d`/`c`/`x`) and colon-count modifiers. `motel.questa` is the parser substrate; `documentation/reference/MOTEL Parser.script.motel` specifies a 13-state parser machine *in `.motel` itself*. **Naming:** *Neoheurist* = the engine/app layer (per source copyrights); *MOTEL* = the data/config meta-language and the foundation library; *Smart Interface Pattern* = the linguistic-parsing-plus-context-inference philosophy (documented in `Smart Interface Pattern.xlsx` and the `smart.pool.*` diagrams, not readable here). Heavy design documentation exists but is in binary Office/Sparx-EA/Visio formats: `MOTEL.document.docx` (38 KB), `.xlsx` (1.2 MB), `.eap` (3.3 MB Sparx model), plus `smart.pool.pdf`/`.vsd`. **The code implements only a fraction of what these documents appear to describe.**

## Code References

- `source/Main/main.c#L490-L562` — `LocationLoader`: DLL load + 5× `GetProcAddress` + destructor/free of prior room.
- `source/Main/main.c#L370-L488` — `ConnectAdjacentLocations`: 26× `LoadLibrary`/`FreeLibrary` per move.
- `source/Main/main.c#L647-L1206` — the single `for(;;)` command-dispatch loop with `goto`-label macros.
- `source/Main/main.t.h#L111-L163` — the central `Context` struct passed to every location.
- `source/xxx.yyy.zzz/xxx.yyy.zzz.t.h` — the five `_Location*` function-pointer typedefs + `LOCATION_*_FUNCTION` name macros (the plugin contract).
- `source/xxx.yyy.zzz/xxx.yyy.zzz.def` — shared location export list.
- `source/555.555.555/555.555.555.c#L174-L414` — richest sample `LocationCommandProcessor` (Trophy Case).
- `source/State/state.c#L35-L117` — UUID-keyed state cache (`StateConstructor`/`Retriever`/`Updater`).
- `source/Command/command.c#L58-L296` — `GetCommand`, self-annotated "severely broken - mid-refactoring" at L64-L65.
- `source/Command/command.c#L298-L451` — `TokenizeCommand` / `ParseTokenizedCommand`.
- `source/Motel/motel.types.t.h#L109-L119` — `same`/`next`/`what`/`fail`/`done`/`loop`/`escape` macros.
- `source/Motel/motel.def` — 78 Motel exports.
- `source/Motel/motel.compilation.t.h#L42-L88` — the only cross-platform conditional (Windows vs. else; `CALLING_CONVENTION`).
- `source/Innkeeper.sln` — 13 project entries; `FD1C5E16-…` (L79) references a missing directory.

## Architecture Insights

1. **The plugin boundary is the product.** "Infinitely extensible" is real: the engine knows nothing about any room; rooms are discovered by filename and bound by a 5-function ABI. This is a strong, clean seam — preserve it. A refactor should formalize it as a versioned, documented ABI (e.g. an explicit `LocationInterface` struct returned by one entry point, rather than five separate `GetProcAddress` lookups), which also makes it portable.

2. **Coordinate-as-filename is clever but couples world topology to the filesystem.** Pros: zero central registry, trivially "drop in a room." Cons: 26 filesystem probes per move; spatial topology is forced to a literal 3D Cartesian lattice (no non-Euclidean links, no teleporters except the existing `HOP`); coordinate exhaustion/aliasing; and the entire scheme is Win32-only via `LoadLibrary`/`wsprintf`/`TCHAR`.

3. **Layering is better than the implementation.** The `.t.h`/`.i.h`/`.h`/`.c` discipline, the `Context`-passing convention, and the Motel/engine/location/item strata are coherent. The problems are *completeness and correctness* (stubs, a broken tokenizer data path, a half-finished `GetCommand` refactor, dead older `engine.c`/`engine.t.h`/`rule.*` parallel to the live `main.c`), not *structure*.

4. **Two parallel generations of the engine coexist.** `source/Command/engine.c` + `engine.t.h` + `rule.*` (older: `Rule*` exports, `(Context*, char*)` processor, reversed N/S axis) are superseded by `source/Main/main.c` + `main.t.h` + the `Location*` contract. Carrying both is confusing and inflates the "what builds" question.

5. **Vocabulary is compile-time concrete.** All words are macros expanding to comma-separated literals consumed by variadic `ClassMatchString(subject, WORDS…, "")`. Flexible to author, but every grammar change is a recompile, `HEADING_WORDS` expands to ~156 literals at each call site, and there's a literal bug (`STRANGE_WORDS` concatenates `"DEC" "FROBOZZ"` → `"DECFROBOZZ"`). Runtime-loadable vocabulary (the `.motel` files already model this!) would better serve "extensible."

## Path to an Initial Release (recommended sequencing)

**Phase 0 — Make it build (mechanical, unblocks everything):**
- Remove/replace the missing `FD1C5E16-…` project from `Innkeeper.sln`.
- Give the two template projects (`xxx.yyy.zzz`, GUID item) a stub `.c` or exclude them from the default build.
- Decide `Item`'s fate (author `item.c` + `.vcxproj`, or drop).
- Retarget `v141`→installed toolset and SDK `10.0.17763.0`→installed SDK; fix x64 `OutDir`/`IntDir` so EXE+DLLs co-locate; normalize the Gender/Sleep x64 `.def` paths.

**Phase 1 — Make it run correctly:**
- Finish the `GetCommand` refactor and fix the tokenizer trailing-space bug (tokens carry a trailing space, so `ParseTokenizedCommand` can't match any non-final word) — this is likely why the prototype isn't demonstrably playable.
- Remove the dead older engine (`engine.c`/`rule.*`) or clearly quarantine it.
- Resolve the dangling-pointer return from `ParseTokenizedCommand` (returns into a by-value stack copy).

**Phase 2 — Make it shippable:**
- Add save/load (serialize the `LocationStates`/`ItemStates` trees) — implement or replace the stub `motel.io`.
- Add a test suite (none exists today) — the Motel library is the easiest, highest-value place to start.
- Author tooling to scaffold a new location/item (eliminate the manual copy-rename-GUID dance).

**Phase 3 — Strategic (decide explicitly):**
- **Portability:** abstract `LoadLibrary`/`GetProcAddress`/`FreeLibrary`/`HMODULE`/`TCHAR`/`wsprintf` behind the existing `motel.compilation.t.h` seam (→ `dlopen`/`dlsym`, `.so`/`.dylib`) — or consciously stay Windows-only for v1.
- **Build system:** replace 13 `.vcxproj` with CMake to drop the VS-version lock.
- **Vocabulary:** move word classes from compile-time macros to runtime-loaded `.motel` data.
- **Per-move DLL probing:** cache loaded neighbor handles instead of `LoadLibrary`/`FreeLibrary`×26 each step.

## Open Questions

1. **Release scope:** is "initial release" a *playable demo* of the existing 5-room sample world, or a *content-authoring SDK* others extend? The refactor priorities differ sharply.
2. **Windows-only or cross-platform for v1?** This is the single biggest architectural fork.
3. **Is the DLL-per-room model a feature to market or an implementation detail to hide?** It's the project's identity, but also its main portability/performance liability.
4. **How much of the binary design docs** (`MOTEL.document.eap` Sparx model, `Smart Interface Pattern.xlsx`, `smart.pool.vsd`) is authoritative and still intended? A discrepancy review of those against the implemented code would clarify how much vision remains unbuilt. (These are binary/Office/Sparx formats not readable in this session — the `.eap` could be exported and rendered via the `xmi-to-mermaid` workflow if a current XMI export exists.)
5. **Items have never been instantiated** (no `item.c`). Is the item subsystem in-scope for v1?

## Related Research

None found — `thoughts/` did not previously exist in this repository; this is the first research artifact.
