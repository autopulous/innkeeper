---
name: innkeeper-vision-and-v1-foundation
title: Innkeeper / Neoheurist — Full Vision + v1 Foundation Release
type: brainstorm-goal
status: ready
created: 2026-06-06
updated: 2026-06-06
supersedes: []
related:
  - thoughts/shared/research/2026-06-06-innkeeper-design-evaluation.md
---

# Innkeeper / Neoheurist — Full Vision + v1 Foundation Release

> **Roles defined for this artifact**
> - **Author** — someone who builds worlds on the platform. The *primary* audience: they are the people the platform exists to empower.
> - **Player** — someone who plays the worlds authors build. A downstream consumer of authored worlds.
> - **Community contributor** — a future class: people who carry the platform to new operating systems / hardware. Broad cross-platform support is intended to be community-driven, not solo, and not in v1.

This is a **two-phase goal**, captured one discrete answer at a time.

- **Phase 1 — the vision documents themselves** (this brainstorm chain and its successors). Outcome: the full vision is written down well enough to drive everything after it.
- **Phase 2 — a working initial release (v1)** of the foundation. Outcome below. Scope choice for v1 is deliberately **"prove the foundation," not "prove the vision."**

## Outcome

**Phase 1:** A captured, written vision for Innkeeper (the application/world) on the **Neoheurist** engine — a *federated, infinitely-extensible, vector-database reality* for authors — clear enough to be consumed by requirements and planning.

**Phase 2 (v1):** The salvaged Neoheurist stub becomes a thing you can point at and say "it works." From a fresh checkout on John's own machine, the engine **builds, launches the 5-room sample world, is playable, and persists state across save/reload** — and the location plugin contract is documented well enough that a new room can be authored from the docs alone.

The vision that v1 is the *foundation* for (captured, but **not** v1 scope):

- **Federated reality.** Every world can **link to and/or contain any other world**, including worlds on **physically separate host computers** — a distributed "greater vector-database reality." Building on your own system is just building a node.
- **Uniform composable unit = a "something".** No room-vs-world type distinction — a "something" is a **shared library the engine loads and populates with player(s)**; it can *be* a room or a world depending on use. Recursively composable.
- **Multiplayer along a spectrum set by the author's "physics":** baseline **asynchronous** shared occupancy (each player in their own time) → optional **fully synchronous** interactivity → **ghost effects** (players as ghosts in each other's world). Plus **"echo selves":** a player can be helped or interfered with by **their own past self** when revisiting a previously-visited location (today's UUID-keyed persistent location state is the seed).
- **"Physics" = semantics.** Each domain is a **vector space of meaning**; within a world the **meaning of words can be completely redefined**, even **restructuring the ideograms underlying words** — producing realities that are **"unreal" but logically consistent**.
- **The game *is* a vector database** — literal, not metaphor. The **3-D `X.Y.Z` grid is the seed of an *n*-dimensional vector space**, which grows out of the v1 coordinate grid later.

## Definition of done

**v1 (Phase 2) is done when, from a fresh checkout on John's machine:**

1. the solution builds with no missing-project / toolset errors;
2. the engine launches the 5-room sample world and a player can move among all five rooms;
3. the stateful interactions work — Trophy Case open/close/lock/unlock, the Changing room's gender/name mutation, the Tiny bedroom's sleep;
4. **save then reload restores prior state** (the newly in-scope persistence);
5. the 5-function location plugin contract is **documented well enough that a brand-new room can be authored from the doc alone**, without reading engine source.

**Phase 1 is done when** the vision is captured across the goal → requirements artifacts to `status: ready`.

## Motivation

**Why now = LLMs.** The full vision is a heavy lift for one person. An LLM is a **force-multiplier and thinking-partner** that can energize a solo author against a concept of this size — and part of the point is to explore **both the potential and the limits** of that partnership against a genuinely hard, idiosyncratic systems-and-language project. The code was salvaged from SourceForge (copyrights "John L. Hart IV," 2010–2015) and has sat dormant; the moment to resurrect it is now that a capable thinking-partner exists.

## Beneficiaries and stakeholders

- **Authors (primary).** The platform exists to empower people who build worlds on it. v1's documented plugin contract is *for them*.
- **Players (downstream).** They consume the worlds authors build.
- **Community contributors (future).** Broad cross-platform support is intended to be **community-driven** — not solo, not v1.
- **John** — original author, sole developer for v1, owner of the vision.

## Scope

**In scope (v1 / Phase 2)**
- Get the existing single-machine **5-room sample world** building, running, and playable.
- A **clean, documented** location plugin contract (the 5-function location ABI).
- **Save/load persistence** — state must survive process exit and reload.
- The vision lives in **documents as roadmap** (Phase 1 output).

**Out of scope / non-goals (deferred to the next phase)**
- **The Item subsystem** — items have never been instantiated (no `item.c`); v1 ships rooms-only.
- **Cross-platform portability** — v1 consciously targets John's machine only. (The DLL-per-location *model* is portable in principle — `dlopen`/`dlsym`/`dlclose` mirror `LoadLibrary`/`GetProcAddress`/`FreeLibrary` — so this is deferral, not architectural exclusion.)
- **Authoring / scaffolding tooling** — the manual copy-rename-GUID dance stays manual for v1.
- **All vision features** — federation across hosts, the recursively-composable "something," physics-as-semantics, multiplayer / ghost effects / echo selves, runtime-loadable vocabulary, the *n*-dimensional vector space. These are roadmap, not v1.

## Constraints

- **(soft) v1 toolchain = "whatever builds on John's machine for now."** Exact versions (Visual Studio / MSVC, Windows SDK, C standard) are free to float; the stale `v141` / SDK `10.0.17763.0` pins get retargeted to whatever is installed.
- **(forward-looking, not a v1 constraint) Broad platform support will eventually be required, and will be community-driven.** v1 should not *preclude* this, but is not obligated to deliver it.
- **(hard) Solo developer** for v1 (LLM-assisted).

## Alternatives considered

- **Rewrite from scratch (new language / new architecture) — rejected.** The distinctive mechanism (dynamic per-location plugin loading) is **portable in principle**; only the thin Win32 binding layer (`LoadLibrary`/`HMODULE`/`TCHAR`/`__stdcall`/`wsprintf`) is Windows-specific, and the codebase already has a home for an abstraction seam (`motel.compilation.t.h`). Refactoring preserves real accumulated value, not sunk cost. "Momentum" here is **legitimate fitness-for-purpose**, because the mechanism survives a future port.
- **Replacing the coordinate-as-filename topology in v1 — rejected.** The `X.Y.Z` 3-D Cartesian grid is the **deliberate seed of the eventual *n*-dimensional vector space** (vision through-line). The refactor should harden it, not treat it as throwaway scaffold.
- **v1 = "prove the vision" (Option B) instead of "prove the foundation" (Option A) — rejected for v1.** The stub cannot currently build; a foundation you can't compile can't host a vision demo. Foundation-first makes the vision demo a credible *next* release rather than a gamble on sand.

## Risks

- **Scope-creep (top risk).** Getting pulled into the vision and leaving the foundation half-built. *Mitigation:* the two-phase split, the explicit deferrals above, and the strict 5-point definition of done are the guardrails — v1 ships when those five gates pass, no more.
- **The build/refactor is a deeper hole than it looks.** `GetCommand` is self-described in-source as "severely broken — mid-refactoring"; a tokenizer trailing-space bug blocks parsing; two parallel engine generations (`main.c` vs. the older `engine.c`/`rule.*`) must be untangled. *Mitigation:* sequence the mechanical build-blockers (Phase 0 in the research artifact) before correctness work.
- **Solo bandwidth + LLM-as-force-multiplier dependency.** The whole "why now" rides on the LLM actually being a multiplier for deep, idiosyncratic C systems work. *Mitigation:* treat exploring that partnership's limits as an explicit secondary objective, not a hidden assumption.

## Horizon

**Indeterminate by design.** Per John's general working rule, time horizons are meaningless absent a hard external deadline — *"it takes what it takes given the time and attention I have available."* Sequence and dependency ordering matter; duration estimates do not.

## Restatement

I'm resurrecting Innkeeper — my dormant text-adventure platform on the Neoheurist engine — and I'm doing it in two phases. First I'm writing down the full vision: a federated, infinitely-extensible *vector-database reality* where every "something" is a shared library the engine loads and fills with players, where worlds can link to or contain other worlds across separate machines, where the author's "physics" redefines the very meaning of words to make realities that are unreal but logically consistent, and where players meet asynchronously, synchronously, as ghosts, or even as echoes of their own past selves. The 3-D coordinate grid I already have is the seed of the n-dimensional vector space all of that grows into. But the initial release isn't trying to prove any of that — v1's job is to **prove the foundation**: take the salvaged stub that can't even build today and get it building, running, playable across the five sample rooms, persisting state across save and reload, with the location plugin contract documented well enough that an author can write a new room from the docs alone. Items, cross-platform support, and authoring tooling all wait for the next phase; broad platform support, when it comes, will be community-driven. I'm doing this now because an LLM finally gives a solo author a real thinking-partner against a concept this big — and I want to find out both what that partnership can do and where it breaks. It'll take as long as it takes.
