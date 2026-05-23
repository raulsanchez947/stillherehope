// ============================================================
// gate_locke_scaling.h
// Pokemon Emerald Reloaded — Gate Locke Level Scaling System
//
// Add this to your include/ directory and #include it wherever
// you need to call the scaling functions.
// ============================================================

#ifndef GATE_LOCKE_SCALING_H
#define GATE_LOCKE_SCALING_H

#include "global.h"
#include "pokemon.h"

#define GATE_LOCKE_STANDARD FALSE
#define GATE_LOCKE_BOSS     TRUE

u8  GetPlayerHighestPartyLevel(void);
u8  GetGateLockeScaledLevel(void);
u8  GetGateLockeBossLevel(void);
void ApplyGateLockeScaling(struct Pokemon *party, u8 partySize, bool8 isBoss);

#endif // GATE_LOCKE_SCALING_H
