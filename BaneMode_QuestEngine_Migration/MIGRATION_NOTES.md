# BaneMode -> Emerald Quest Engine Migration Notes

## Staged Base
- Source staged from: `/Users/ivan/Downloads/EmeraldCrest Source Code v1.0.F.zip`
- Migration workspace: `/Users/ivan/Documents/New project/BaneMode_QuestEngine_Migration`

## Quick Read
The staged engine is a full decomp-style Emerald codebase with quest/engine additions. It is structurally close to the live BaneMode repo, but it is not the same branch family. That means a clean migration is possible, but it should be treated as a **new base with ports**, not as an in-place overwrite.

## Major Similarities
- `src/`, `data/`, `graphics/`, `include/`, `tools/`, `asm/` are all present
- Both projects use decomp-style map JSON / generated event data
- Both projects build from a top-level `Makefile`

## Major Differences
- Engine base names differ (`emeraldcrest.gba` vs `pokeemerald.gba`)
- The live BaneMode repo has extra migration/dev tooling (`migration_scripts`, `dev_scripts`, `trainer_rules.mk`, `porymap.project.cfg`)
- EmeraldCrest includes quest-engine specific assets and systems (`graphics/quest_menu`, `graphics/sliding_puzzle`, `graphics/soar`)
- Linker/build script naming differs (`ld_script.txt` vs `ld_script.ld`)

## Recommendation
Use EmeraldCrest as a **fresh integration base** and port BaneMode features over in layers:
1. Build/tooling sanity
2. Core gameplay systems (battle toggles, pressure systems, items, evolution rules)
3. Trainer data and identities
4. Maps / gym swaps / museum scene / custom route logic
5. Graphics and overworld actors
6. Documentation refresh

## Important Caution
Save compatibility should be assumed broken during migration. We should not promise old `.sav` support while changing engine base.

## Current Status
- macOS build/tooling has been repaired enough to produce a clean `modern` build locally.
- First BaneMode gameplay layer already ported in:
  - competitive Rocket party generation
  - kaizo trainer IV/EV buff pass
  - dynamic trainer-party integration
- Additional Quest-side BaneMode systems now ported:
  - starter trio swapped to `Poliwag / Lileep / Houndour`
  - shiny starter logic preserved for the new trio
  - native-to-Hisuian branch evolutions moved to stone evolutions
  - `Pawmo -> Pawmot` at level 32
  - `Stantler -> Wyrdeer` by knowing `Psyshield Bash`
  - Pokemon Summary memo page now shows evolution hints
  - fresh new games receive a BaneMode convenience bundle
  - Petalburg Pokemon Center starter helper text now matches the new trio
  - Petalburg Mart expanded inventory now mirrors the broader BaneMode progression shop
  - May's route / Rustboro / Lilycove teams now follow the Uxie-centered BaneMode progression instead of the older dragon side-build
  - first-pass gym identity scripts are swapped
  - first-pass battle pressure hook is live for Rocket battles and swapped gym leaders
- External save migration helper added:
  - `/Users/ivan/Documents/New project/tools/banemode_migrate_save_to_quest.py`
  - intended flow: old BaneMode save + fresh Quest Engine save shell -> migrated Quest Engine save

## What Was Fixed To Get Builds Working
- Rebuilt native macOS copies of helper tools that shipped as Linux binaries:
  - `tools/mid2agb`
  - `tools/preproc`
  - `tools/aif2pcm`
- Patched several old-style callback/function-pointer sites for the modern toolchain:
  - `field_player_avatar.c`
  - `main_menu.c`
  - `m4a.c`
  - `m4a_internal.h`
  - `librfu.h`
  - `librfu_intr.c`
  - `scrcmd.c`

## Next Port Targets
1. Battle/progression rules from old `battle_main.c`
   - Rocket / boss level rules
   - expand pressure coverage beyond the first-pass hook
   - Rocket admin / boss rule mapping
2. Evolution-rule changes
   - remaining custom evolution edge cases
3. UI / convenience systems
   - shiny rate selector
   - broader Pokemon Center utility rollout
   - move tutor / relearner migration
4. Map and story layer after the battle side is stable
   - Rocket / Giovanni / rival / miniboss scene scripting
   - full gym text polish and identity cleanup
   - complete rival script cleanup so every scene beats the old ROM instead of just matching team logic

## Suggested First Port Targets
- Team / trainer data from `src/data/trainers.h`
- Battle customizations from `src/battle_main.c` and nearby helpers
- Object event graphics definitions for custom actors
- `data/maps/` gym identity swaps and key story scene scripts

## Suggested Workspace Roles
- Live current hack (do not delete): `/Users/ivan/Documents/BaneMode_Rocket_Porymap_Work/pokeemerald-rocket-porymap`
- New migration base: `/Users/ivan/Documents/New project/BaneMode_QuestEngine_Migration`
