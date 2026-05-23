# BaneMode Mini-Boss Team Guide

Date: 2026-05-10  
Scope: live Quest migration branch miniboss rules and route coverage.

## Core Rule

- **Route 101** is the exception route:
  - it has the dedicated mandatory **AEG** fight instead of the normal three-route-miniboss structure.
- **Route 102 through Route 134** use the global BaneMode route miniboss rule:
  - the first three promoted miniboss trainer slots on each numbered route are treated as miniboss-tier fights.

## Global Mini-Boss Mechanics

- Level rule:
  - route minibosses scale to the player's **current highest party level**
- Pressure overlay:
  - **Stealth Rock** is set on the player's side
  - **Tailwind** is set on the miniboss side
- Arena lock:
  - the route miniboss pressure package is locked like gym pressure
  - hazard / terrain removal tools are blocked in these fights

## Route-by-Route Coverage

The live branch treats the following routes as miniboss lanes under the shared global system:

- Route 102
- Route 103
- Route 104
- Route 105
- Route 106
- Route 107
- Route 108
- Route 109
- Route 110
- Route 111
- Route 112
- Route 113
- Route 114
- Route 115
- Route 116
- Route 117
- Route 118
- Route 119
- Route 120
- Route 121
- Route 122
- Route 123
- Route 124
- Route 125
- Route 126
- Route 127
- Route 128
- Route 129
- Route 130
- Route 131
- Route 132
- Route 133
- Route 134

## Custom Exceptions and Named Mini-Bosses

### Route 101

#### AEG (mandatory opening exception)

1. Shuckle  
Item: Berry Juice | Ability: Sturdy  
Moves: Infestation, Toxic, Stealth Rock, Protect

2. Shuckle  
Item: Mental Herb | Ability: Sturdy  
Moves: Encore, Infestation, Rest, Sleep Talk

3. Shuckle  
Item: Leftovers | Ability: Contrary  
Moves: Shell Smash, Rock Tomb, Struggle Bug, Knock Off

Notes:
- Mandatory only after Birch grants the Pokedex
- Uses the route-miniboss scaling rule instead of a fixed vanilla early-game team

### Route 116

#### Douglas

1. Clodsire (Lv 47) - Leftovers  
Moves: Toxic Spikes, Earthquake, Recover, Haze
2. Amoonguss (Lv 47) - Black Sludge  
Moves: Spore, Giga Drain, Sludge Bomb, Protect
3. Sableye (Lv 47) - Covert Cloak  
Moves: Will-O-Wisp, Recover, Knock Off, Taunt

Notes:
- This is a custom override miniboss team, not just a shared route shell
- Douglas is configured as a double-battle expert slot in the live trainer data

### Route 122

#### Nacho

The live trainer slot is custom-bound as `TRAINER_BANEMODE_ROUTE122_NACHO` and uses its own curated custom-moves party.

#### Douglas

The live trainer slot is custom-bound as `TRAINER_BANEMODE_ROUTE122_DOUGLAS` and uses its own curated custom-moves party.

#### Shamus

The live trainer slot is custom-bound as `TRAINER_BANEMODE_ROUTE122_SPECIAL` and is labeled `SHAMUS` in trainer data.

Current Route 122 special-party spine:

1. Dragonite (Lv 63) - Heavy-Duty Boots  
Moves: Dragon Dance, Dual Wingbeat, Earthquake, Extreme Speed
2. Gengar (Lv 63) - Focus Sash  
Moves: Shadow Ball, Sludge Wave, Nasty Plot, Destiny Bond
3. Kingambit (Lv 63) - Black Glasses  
Moves: Kowtow Cleave, Iron Head, Sucker Punch, Swords Dance

## Winstrate Note

- The one-time Winstrate-family Bane battle is miniboss-tier in pressure/difficulty,
  but it is **not** one of the standard route miniboss lanes.
- Live location:
  - [Route111_WinstrateFamilysHouse map.json](/Users/ivan/Documents/New%20project/BaneMode_QuestEngine_Migration/data/maps/Route111_WinstrateFamilysHouse/map.json)
- Unlock condition:
  - beat the Winstrate family and receive the `MACHO BRACE`

### Winstrate Bane (Live One-Time Battle)

1. Scrafty (Lead) - Rocky Helmet - Intimidate  
Moves: Fake Out, Taunt, Knock Off, Drain Punch
2. Scrafty (Setup) - Lum Berry - Moxie  
Moves: Dragon Dance, Crunch, Drain Punch, Ice Punch
3. Scrafty (Tank) - Assault Vest - Intimidate  
Moves: Drain Punch, Knock Off, Ice Punch, Poison Jab
4. Scrafty (Sustain) - Leftovers - Shed Skin  
Moves: Bulk Up, Rest, Drain Punch, Crunch
5. Scrafty (Speed Control) - Choice Scarf - Moxie  
Moves: Knock Off, Close Combat, Stone Edge, Ice Punch
6. Scrafty (Utility) - Sitrus Berry - Intimidate  
Moves: Fake Out, Snarl, Drain Punch, Thunder Punch

## Practical Reading Guide

- If a route is **not** one of the custom exception routes above, assume:
  - three promoted miniboss fights,
  - level = current highest party level,
  - Stealth Rock on player side,
  - Tailwind on enemy side,
  - locked arena rules.

- If a route **is** a custom exception route, use the named team notes above first.
