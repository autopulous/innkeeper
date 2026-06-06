---
name: innkeeper-vision-and-v1-foundation
title: Innkeeper / Neoheurist — v1 Foundation Requirements
type: brainstorm-requirements
status: ready
created: 2026-06-06
updated: 2026-06-06
goal: thoughts/shared/brainstorms/goals/2026-06-06-innkeeper-vision-and-v1-foundation.md
supersedes: []
related:
  - thoughts/shared/research/2026-06-06-innkeeper-design-evaluation.md
---

# Innkeeper / Neoheurist — v1 Foundation Requirements

> Goal: [Innkeeper / Neoheurist — Full Vision + v1 Foundation Release](../goals/2026-06-06-innkeeper-vision-and-v1-foundation.md) — status: ready

## Summary

For this single-release goal, **MVP** = the v1 foundation release; **later** = deferred vision/cleanup work.

|        | MVP | v1 | later |
| ------ | --- | -- | ----- |
| Must   | 11  | 0  | 0     |
| Should | 4   | 0  | 0     |
| Could  | 0   | 0  | 0     |
| Won't  | 0   | 0  | 7     |

Per-category: **FR** 11 (8 active + 3 Won't) · **NFR** 4 (all Won't/deferred) · **DR** 2 · **IR** 0 (N/A) · **UX** 3 · **OR** 1 · **CR** 1.

## Definitions

- **Author** — someone who builds worlds (location/item DLLs) on the platform. The *primary* audience.
- **Player** — someone who plays authored worlds. Downstream consumer.
- **Community contributor** — future class: people who port the platform to new platforms. Out of v1.
- **Move** — a single change of the player's **current location** to an **adjacent cell** in the `X.Y.Z` lattice, addressed by **boxing the compass** of the named `WEST/EAST/SOUTH/NORTH/DOWN/UP` axes — i.e. any of the **26 surrounding cells** (cardinals plus all intercardinal and vertical-diagonal combinations), not just the 6 axis-aligned directions. Mechanically: a navigation command → `ConnectAdjacentLocations` probes the 26 neighbors by filename (`%d.%d.%d.dll`) → if the target neighbor DLL exists, `LocationLoader` loads it, resolves the 5 contract functions, destructs the prior room, and makes the target current; a `0`/out-of-bounds/absent target is refused with "Cannot traverse in that direction." (`HOP` is a separate teleport, not an adjacency move.)
- **Hidden exit** — a populated adjacency (neighbor DLL exists) that is deliberately **not revealed** in the exits listing.
- **One-way (in only) exit** — an adjacency a player can **arrive from** but which is **not offered as a way back out** of the destination.
- **JSON in a motel wrapper** — the v1 save-file format: a JSON state payload enveloped in a `.motel` wrapper.

## Functional requirements

### FR-1: Reproducible build
**Priority:** Must
**Phase:** MVP
**Statement:** A fresh clone must build the engine and all five room DLLs to completion via one documented command/invocation on the installed toolchain, with no manual project-surgery.
**Acceptance criteria:**
- From a clean clone, the documented build path produces the engine EXE + 5 room DLLs with 0 errors.
- The dead/placeholder/missing-project references are removed (`FD1C5E16-…`, the two no-source template projects, the missing `Item` project).
- Stale `v141` / Windows SDK `10.0.17763.0` pins are retargeted to the installed toolset/SDK; the fixes are committed (not re-applied by hand each build).
**Traces to goal:** Definition of done #1, Scope/in.

### FR-2: Five-room navigation (26-direction compass)
**Priority:** Must
**Phase:** MVP
**Statement:** A player must be able to move among all five sample rooms by issuing direction commands across the full 26-direction compass.
**Acceptance criteria:**
- In the manual walkthrough, every populated adjacency among `555.553.553` (Low Powder), `555.554.554` (Powder), `555.555.553` (Changing), `555.555.554` (Tiny bedroom), `555.555.555` (Barracks/HOME) is reachable by its compound direction word.
- A zero / out-of-bounds / unpopulated direction is refused with "Cannot traverse in that direction."
- The 26 direction names are present in the command vocabulary (`HEADING_WORDS`).
**Traces to goal:** Definition of done #2, Outcome.

### FR-3: Working command pipeline
**Priority:** Must
**Phase:** MVP
**Statement:** Typed player commands must parse and dispatch correctly.
**Acceptance criteria:**
- The `GetCommand` refactor (self-annotated "severely broken - mid-refactoring") is completed.
- Tokenization is verified to already split tokens correctly: the prior research artifact's "trailing-space bug" was **refuted** on inspection — in `TokenizeCommand` (`source/Command/command.c:326-333`) `character++` runs before `CopyString`, so the delimiting space is excluded and `MatchToken` compares cleanly. No fix is required here; this item is a confirmation, not a change.
- The dangling-pointer return from `ParseTokenizedCommand` (returns `&pCommandTokens.Token[token]` into a by-value stack copy) is resolved. Note: the sole live caller (`source/Main/main.c:662`) currently discards the return value, so this is a latent defect rather than an active failure — fix it to harden the contract before any future caller dereferences it.
- In the walkthrough, multi-word commands (e.g. `OPEN TROPHY CASE`, compound directions) dispatch to the correct handler.
**Traces to goal:** Definition of done #2–#3, Risks (build-depth).

### FR-4: Stateful room interactions
**Priority:** Must
**Phase:** MVP
**Statement:** The sample rooms' stateful behaviors must work end-to-end.
**Acceptance criteria:**
- Trophy Case (Barracks): open/close/lock/unlock behave consistently in the walkthrough.
- Changing room: gender/name mutation persists on the `Context`.
- Tiny bedroom: sleep delegates to the Sleep module and returns.
**Traces to goal:** Definition of done #3.

### FR-5: State persistence (save / load)
**Priority:** Must
**Phase:** MVP
**Statement:** Game state must survive process exit and reload, via both an implicit continue slot and explicit named slots.
**Acceptance criteria:**
- **Save-on-quit / load-on-launch:** play → quit → relaunch restores the prior state (implicit continue slot).
- **Named slots:** `SAVE <name>` then mutate then `LOAD <name>` restores the named snapshot.
- Serializes the UUID-keyed `LocationStates` and `ItemStates` trees (`ItemStates` empty in v1 since Items are deferred).
- A loaded state is behaviorally indistinguishable from the pre-save state.
**Traces to goal:** Definition of done #4, Scope/in.

### FR-6: Plugin-contract documentation
**Priority:** Must
**Phase:** MVP
**Statement:** The 5-function location plugin contract must be documented well enough to author a new room from the doc alone, without reading engine source.
**Acceptance criteria:**
- Written reference covers all five functions: `LocationConstructor`, `LocationDestructor`, `LocationValidator`, `LocationDescriber`, `LocationCommandProcessor`.
- Documents the `.def` export list, the `.vcxproj` GUID, and the hand-typed `OBJECT_ID` UUID requirement.
- Includes a step-by-step room-authoring checklist.
- No proof-room build is required to satisfy this (per D — doc + checklist bar).
**Traces to goal:** Definition of done #5, Beneficiaries (authors primary).

### FR-7: Remove dead older engine
**Priority:** Must
**Phase:** MVP
**Statement:** The superseded older engine generation must be removed or quarantined so only the live `main.c` + `Location*`-contract generation remains.
**Acceptance criteria:**
- `engine.c`, `engine.t.h`, and `rule.*` are removed or excluded from the build.
- The build stays clean and the game still plays with the older generation gone.
**Traces to goal:** Risks (build-depth), Beneficiaries (authors read the source).

### FR-8: Per-direction exit visibility
**Priority:** Should
**Phase:** MVP
**Statement:** The engine must support hiding or one-way-in exits via a per-direction flag a location sets, honored by the exits listing.
**Acceptance criteria:**
- The `Context` carries a per-direction visibility/suppression flag (additive field, not a new contract function).
- The engine's default exits listing honors the flag.
- In the sample world: a room with a **hidden** exit omits it though the neighbor DLL exists; a **one-way-in** entry is not offered as a return exit.
**Traces to goal:** Outcome ("physics" author-owned), Scope.

### FR-9: Item subsystem (DEFERRED)
**Priority:** Won't
**Phase:** later
**Statement:** Instantiate the Item/Container subsystem (author `item.c`, wire item-state API symmetric to `LocationStates`).
**Acceptance criteria:** _N/A in v1 — deferred to next phase._
**Traces to goal:** Scope/out.

### FR-10: Authoring / scaffolding tooling (DEFERRED)
**Priority:** Won't
**Phase:** later
**Statement:** Tooling to scaffold a new location/item (eliminate the manual copy-rename-GUID dance).
**Acceptance criteria:** _N/A in v1 — deferred to next phase._
**Traces to goal:** Scope/out.

### FR-11: Vision features (DEFERRED)
**Priority:** Won't
**Phase:** later
**Statement:** Federation across hosts; recursively-composable "something"; physics-as-semantics; multiplayer (asynchronous → synchronous → ghost effects) and echo selves; runtime-loadable vocabulary; the n-dimensional vector space.
**Acceptance criteria:** _N/A in v1 — these are roadmap; the v1 coordinate grid is their seed._
**Traces to goal:** Scope/out, Outcome (vision).

## Non-functional requirements

### NFR-1: Non-critical Motel bugs (DEFERRED)
**Priority:** Won't
**Phase:** later
**Statement:** Fix the non-blocking Motel-library defects.
**Acceptance criteria:** _Tracked, deferred past v1:_ `SelectQuestaNode` unimplemented; OO `union string` empty method bodies; `STRANGE_WORDS` → `"DECFROBOZZ"` concat; `FindClassMemberCharacter` loop inversion; `PullBottomQuestaNode` alias mis-map.
**Traces to goal:** Risks (build-depth), Scope (lean v1).

### NFR-2: Per-move DLL-probe performance (DEFERRED)
**Priority:** Won't
**Phase:** later
**Statement:** Optimize the 26× `LoadLibrary`/`FreeLibrary` existence probe per move (e.g., cache loaded neighbor handles).
**Acceptance criteria:** _N/A in v1 — acceptable at 5-room local scale; deferred._
**Traces to goal:** Scope, Risks (scope-creep).

### NFR-3: Thread-safety / concurrency (DEFERRED)
**Priority:** Won't
**Phase:** later
**Statement:** Make Motel subsystems thread-safe (per-handle cursor/result state; global PRNG state).
**Acceptance criteria:** _N/A in v1 — single-player single-machine; deferred with the multiplayer vision._
**Traces to goal:** Scope/out (multiplayer deferred).

### NFR-4: Cross-platform portability (DEFERRED)
**Priority:** Won't
**Phase:** later
**Statement:** Abstract the Win32 binding layer (`LoadLibrary`/`GetProcAddress`/`FreeLibrary`/`HMODULE`/`TCHAR`/`__stdcall`/`wsprintf`) behind the `motel.compilation.t.h` seam (→ `dlopen`/`dlsym`, `.so`/`.dylib`).
**Acceptance criteria:** _N/A in v1 — Windows-only v1 by choice; broad support is community-driven later. v1 must not preclude it (the DLL-per-location model is portable in principle)._
**Traces to goal:** Constraints (forward-looking), Scope/out.

## Data requirements

### DR-1: Persisted state schema
**Priority:** Must
**Phase:** MVP
**Statement:** Persistence must round-trip the UUID-keyed state trees with full fidelity.
**Acceptance criteria:**
- Both `LocationStates` and `ItemStates` trees serialize and deserialize.
- A round-tripped state produces identical in-game behavior to the pre-save state.
- State remains UUID-keyed (survives DLL unload/reload, as today).
**Traces to goal:** Scope/in (save/load), Definition of done #4.

### DR-2: Save format — JSON in a motel wrapper
**Priority:** Must
**Phase:** MVP
**Statement:** Save files must be a JSON state payload enveloped in a `.motel` wrapper.
**Acceptance criteria:**
- A save file is a well-formed `.motel` document whose payload is valid JSON.
- The payload deserializes to satisfy DR-1.
- Implies implementing the `motel.io` serializer (currently a stub).
**Traces to goal:** Scope/in, Outcome (`.motel` is a design asset). See D-1.

## Integration requirements

_N/A — v1 is single-machine with no external/internal system contracts. The integration story (federation across physically separate hosts) is wholly deferred to a later phase (FR-11)._

## UX requirements

### UX-1: Save/load confirmation feedback
**Priority:** Should
**Phase:** MVP
**Statement:** Save/load operations must give the player clear textual feedback.
**Acceptance criteria:**
- `SAVE <name>` emits a "saved as ⟨name⟩" confirmation; `LOAD <name>` emits "loaded ⟨name⟩" or "no save found ⟨name⟩".
- Save-on-quit / load-on-launch surface a corresponding message.
**Traces to goal:** Beneficiaries (players), Definition of done #4 (supports manual acceptance of FR-5).

### UX-2: HELP command
**Priority:** Should
**Phase:** MVP
**Statement:** A `HELP` command must list the available verbs and the save/load commands.
**Acceptance criteria:**
- Typing `HELP` lists the global verbs and `SAVE`/`LOAD` usage.
- `HELP` is engine-level (global), independent of per-location vocabulary.
**Traces to goal:** Beneficiaries (players), UX (discoverability).

### UX-3: Available-exits listing
**Priority:** Should
**Phase:** MVP
**Statement:** The player must be able to see which directions lead somewhere, with locations able to hide or make exits one-way-in.
**Acceptance criteria:**
- The engine generates a default exits listing from the populated `Connections`.
- A location can suppress/override per-direction (per FR-8), so a **hidden** exit is omitted and a **one-way-in** entry is not offered as a return.
- Demonstrated in the sample world.
**Traces to goal:** Outcome, Beneficiaries (players). See D-2.

## Operational requirements

### OR-1: v1 release (tag + run docs + bundle)
**Priority:** Must
**Phase:** MVP
**Statement:** v1 must ship as a tagged release with run instructions and a downloadable bundle.
**Acceptance criteria:**
- A git tag `v1` (or equivalent) exists at the release commit.
- A README run section documents launch steps and the EXE / room-DLL co-location (fixing the x64 `OutDir`/`IntDir` co-location the research flagged).
- A zipped EXE + room-DLLs bundle runs on a clean machine matching the build target.
**Traces to goal:** Definition of done, Beneficiaries.
**Rationale:** Overkill for the functionality level, but deliberately establishes release discipline as a standard way of working from the first milestone.

## Compliance requirements

### CR-1: License declaration (MPL-2.0)
**Priority:** Must
**Phase:** MVP
**Statement:** The project must declare an explicit license and preserve existing attributions.
**Acceptance criteria:**
- A top-level `LICENSE` file contains the MPL-2.0 text.
- The `"John L. Hart IV"` per-file copyright notices are preserved.
- Vigna's xorshift PRNG attribution (CC0/public-domain) is preserved.
**Traces to goal:** Constraints (community-driven future), Beneficiaries (authors/community). See D-3.

## Cross-cutting acceptance criteria

- **Manual playthrough is the acceptance method for v1.** Gates FR-2/FR-3/FR-4/FR-5/FR-8/UX-* are validated by a single documented walkthrough run by hand and visually confirmed — no automated test harness in v1 (deferred to control scope-creep).
- **Build remains clean throughout.** Every committed change keeps the reproducible build (FR-1) at 0 errors.
- **Windows-only by choice, but not precluding portability.** No change may hard-block the future cross-platform port (NFR-4); the Win32 surface stays behind `motel.compilation.t.h` where practical.
- **Scope-creep guardrail.** v1 ships when the active requirements pass — Won't items stay out.

## Open decisions

### D-1: Save-file format
**Options considered:**
- Dogfood native `.motel` — human-readable, exercises a real design asset; but `.motel` *writing* isn't built.
- Unconstrained / plan's call (binary dump, ad-hoc text) — leanest; advances nothing.
- **JSON in a motel wrapper** — JSON payload in a `.motel` envelope; partially dogfoods `.motel`, leans on a standard serialization for the payload.
**Chosen:** JSON in a motel wrapper.
**Rationale:** Commits the `motel.io` serializer work while keeping the payload format standard/debuggable, and exercises the `.motel` wrapper without requiring a full `.motel`-native serializer.
**Affects:** DR-2, DR-1, FR-5.

### D-2: Who owns exit visibility — engine or location?
**Options considered:**
- Engine-generated with a visibility data model in the contract — consistent free UX, but grows engine + contract and leaks physics into the engine.
- Location-authored — zero new engine mechanism, fits the plugin philosophy; but no generic listing.
- **Hybrid** — engine default exits listing from `Connections`, locations suppress/override per-direction via a `Context` flag.
**Chosen:** Hybrid.
**Rationale:** Generic default exits UX for free, with author control over hidden / one-way-in, keeping "physics" author-owned without making every room reimplement exits.
**Affects:** UX-3, FR-8, FR-2.

### D-3: Project license
**Options considered:**
- MIT — simplest, max adoption; no copyleft (closed forks possible).
- Apache-2.0 — permissive + patent grant + NOTICE attribution; still no copyleft.
- **MPL-2.0** — file-level copyleft: engine/port modifications return, but author DLLs (new files) stay free/proprietary.
- GPL/LGPL-3.0 — strong copyleft; dynamic-load plugin ambiguity risks forcing author worlds open.
**Chosen:** MPL-2.0.
**Rationale:** Only option satisfying both load-bearing factors at once — community cross-platform ports flow back (file-level copyleft), while authors (primary audience) keep their worlds free to be proprietary. Clean attribution story for John's copyright + Vigna's CC0 PRNG.
**Affects:** CR-1.

## Restatement

For the Neoheurist v1 foundation, "done" means the salvaged stub stops being a stub: it builds reproducibly from a fresh clone on my machine with one documented command, the five sample rooms are navigable by boxing the compass across all 26 directions, the command pipeline actually parses and dispatches (GetCommand fixed, tokenizer trailing-space bug gone), the stateful rooms work, and state survives quit-and-relaunch as well as named saves — persisted as JSON in a motel wrapper. I'm proving it by hand with a documented walkthrough, not a test harness. The dead older engine comes out so the foundation reads honestly; the cosmetic Motel bugs can wait. Authors — my primary audience — get a documented 5-function plugin contract they can build a room from, a default exits listing they can hide or make one-way, and a HELP command; players get save/load confirmation messages. I'll cut a tagged v1 release with run instructions and a downloadable bundle to make releasing a standard habit from day one, and I'll put it under MPL-2.0 so community ports flow back while authored worlds stay free. Items, authoring tooling, cross-platform, and the whole grand vision are explicitly out — that boundary is the guardrail against my biggest risk, scope-creep.
