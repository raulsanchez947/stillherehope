// ============================================================
// gate_locke_scaling.c
// Pokemon Emerald Reloaded — Gate Locke Level Scaling System
//
// HOW TO USE:
// 1. Add this file to your pokeemerald src/ directory
// 2. Add the header declarations to include/gate_locke_scaling.h
// 3. Call GetGateLockeBossLevel() or GetGateLockeScaledLevel()
//    from your trainer battle scripts before initiating battle
// ============================================================

#include "global.h"
#include "pokemon.h"

// ============================================================
// STEP 1: Get the highest level in the player's current party
// ============================================================

u8 GetPlayerHighestPartyLevel(void)
{
    u8 highestLevel = 0;
    u8 i;

    for (i = 0; i < PARTY_SIZE; i++)
    {
        // Skip empty slots
        if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES) == SPECIES_NONE)
            continue;

        u8 level = GetMonData(&gPlayerParty[i], MON_DATA_LEVEL);
        if (level > highestLevel)
            highestLevel = level;
    }

    return highestLevel;
}

// ============================================================
// STEP 2: Scaled level for mini-bosses, grunts, Winstrates
// Sets to player's current highest party level, min 5
// ============================================================

u8 GetGateLockeScaledLevel(void)
{
    u8 level = GetPlayerHighestPartyLevel();
    if (level < 5)
        level = 5; // Safety floor so early-game isn't level 0
    return level;
}

// ============================================================
// STEP 3: Boss-tier level (gym leaders, Giovanni, story bosses)
// Sets to player's highest party level + 2, capped at 100
// ============================================================

u8 GetGateLockeBossLevel(void)
{
    u8 level = GetPlayerHighestPartyLevel() + 2;
    if (level > MAX_LEVEL) // MAX_LEVEL is 100 in pokeemerald
        level = MAX_LEVEL;
    return level;
}

// ============================================================
// STEP 4: Apply scaling to a trainer's full party before battle
//
// Call this function from your trainer's before_battle script
// or from the trainer encounter handler, passing:
//   - party:      pointer to the trainer's pokemon party array
//   - partySize:  number of pokemon on the trainer (e.g. 3 or 6)
//   - isBoss:     TRUE for gym leaders/Giovanni, FALSE for everything else
// ============================================================

void ApplyGateLockeScaling(struct Pokemon *party, u8 partySize, bool8 isBoss)
{
    u8 targetLevel = isBoss ? GetGateLockeBossLevel() : GetGateLockeScaledLevel();
    u8 i;

    for (i = 0; i < partySize; i++)
    {
        if (GetMonData(&party[i], MON_DATA_SPECIES) == SPECIES_NONE)
            continue;

        // Set the level
        SetMonData(&party[i], MON_DATA_LEVEL, &targetLevel);

        // Recalculate stats so HP/Atk/etc reflect the new level
        CalculateMonStats(&party[i]);
    }
}

// ============================================================
// STEP 5: Trainer type flags
// Use these when calling ApplyGateLockeScaling() so you don't
// have to remember TRUE/FALSE for isBoss everywhere
// ============================================================

// For route mini-bosses, Team Rocket grunts, Winstrate family:
//   ApplyGateLockeScaling(trainerParty, partySize, GATE_LOCKE_STANDARD);
//
// For gym leaders, Giovanni, major story battles:
//   ApplyGateLockeScaling(trainerParty, partySize, GATE_LOCKE_BOSS);

#define GATE_LOCKE_STANDARD FALSE
#define GATE_LOCKE_BOSS     TRUE

// ============================================================
// EXAMPLE — how to hook this into a trainer script
// In your trainer's .inc or .pory script, before StartTrainerBattle:
//
// (in C, e.g. in src/battle_setup.c or your custom trainer file)
//
//   extern struct Pokemon gEnemyParty[];
//   ApplyGateLockeScaling(gEnemyParty, 3, GATE_LOCKE_STANDARD); // mini-boss
//   ApplyGateLockeScaling(gEnemyParty, 6, GATE_LOCKE_BOSS);     // gym leader
//
// ============================================================
