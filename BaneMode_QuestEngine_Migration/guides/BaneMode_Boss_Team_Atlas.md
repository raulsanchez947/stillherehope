# BaneMode Boss Team Atlas

Date: 2026-04-12  
Primary source: `Bane_Mode_Trainer_Compendium.extracted.txt`  
Use: review document for all major boss teams from early mandatory fights through Elite Four final gate.

Current build note (2026-04-14):
- This atlas is still the boss-team baseline, but implementation should follow the live rebuild when behavior and trigger rules differ from older recovered docs.
- Route 101 AEG remains mandatory after Birch grants the Pokedex and no longer pre-battle loops.
- Portable PC / Bane Mode Services rules, Senzu Bean support, and current wild-routing rules are governed by the active rebuild docs, not older compendium wording.

## Important Canon Notes

- This atlas preserves recovered team data as the baseline.
- Story canon overrides are still in effect:
  - Team Rocket is the central villain faction.
  - Hoenn Defenders replace Magma role and are allies.
  - Mew + Galarian birds + Deoxys arc replaces older Giratina/Galactic end-thread in final rebuild canon.
- If a legacy team appears here (for example old Galactic/Giratina leftovers), treat it as "recovered baseline to be replaced/adapted."

---

## 1) Mandatory Route Mini-Boss Layer

- Mandatory rule: every route from Route 102 to Route 134 has 3 upgraded mini-boss trainers, each with 3-Pokemon teams.
- Route 101 is reserved for the dedicated mandatory AEG opening fight.
- Route 122 uses 3 custom-added mini-bosses.
- Level rule lock: route mini-boss battles, Team Rocket grunts, and Winstrate-family pressure fights are set to the player's current highest party level. Boss-tier battles are set to current highest party level +2, capped at 100.

### Route 101 Mandatory: AEG

Team concept: triple Shuckle pressure opener.

Trigger note:
- Live build behavior: AEG is mandatory only after the player receives Birch's Pokedex; the fight uses a non-looping mandatory trigger path and then falls through to post-battle dialogue after defeat. AEG is the Route 101 mini-boss exception: Route 101 has one mini-boss instead of the normal three-per-route structure, and AEG scales to the player's current highest party level.

1. Shuckle (Lv 6)  
Item: Berry Juice | Ability: Sturdy  
EVs: 252 HP / 252 Def / 4 SpD  
Moves: Infestation, Toxic, Stealth Rock, Protect

2. Shuckle (Lv 6)  
Item: Mental Herb | Ability: Sturdy  
EVs: 252 HP / 4 Def / 252 SpD  
Moves: Encore, Infestation, Rest, Sleep Talk

3. Shuckle (Lv 7)  
Item: Leftovers | Ability: Contrary  
EVs: 252 HP / 4 Def / 252 SpD  
Moves: Shell Smash, Rock Tomb, Struggle Bug, Knock Off

### Mandatory Route Mini-Boss Captains (Nacho / Douglas / Route Specialists)

- The full route-by-route mini-boss listing is now locked in:
  - `C:\Users\rauls\OneDrive\Documents\New project\notes\BaneMode_MiniBoss_Team_Sheet.md`
- This sheet includes:
  - all Route `102-134` assignments,
  - all tiered mini-boss teams for Nacho and Douglas,
  - Route 116 Shamus custom mini-boss override,
  - Route 122 custom override teams,
  - one-time Winstrate Bane battle (Bane is not part of route mini-boss lanes),
  - level rule lock (`current highest party level`) applied to every mandatory route mini-boss battle, including AEG. Boss-tier encounters use current highest party level +2.

Bane appearance lock:
- Bane is battled exactly 3 times total:
  1. one-time Winstrate family battle
  2. Elite Four final gate Battle 1
  3. Elite Four final gate Battle 2

Live Winstrate binding:
- Bane now appears in the Winstrate family house after the `MACHO BRACE` reward gate.
- Current live implementation uses the six-Scrafty pressure gimmick documented in the mini-boss guide.

---

## 2) Rival / Checkpoint Bosses

## Paul — Full Rival Arc (Wally Replacement)

Paul replaces Wally across all canonical Wally encounter points. His team evolves across three
stages mirroring his Sinnoh anime arc. Chimchar is present only in the first battle (released
before Mauville, per anime canon).

Wally's uncle has been fully removed from all maps and scripts
(MauvilleCity, VerdanturfTown_WandasHouse). No uncle NPC, no uncle dialogue.

---

### Stage 1 — First Battle (Route 111 area / Winstrate)
*Trainer ID: TRAINER_PAUL | Party: sParty_Paul*  
*3-Pokémon team. No Full Restores.*

1. **Elekid** (Lv 15) — Oran Berry | Static  
   IVs: 20 | Moves: Thunder Punch, Low Kick, Swift, Leer

2. **Chimchar** (Lv 15) — Oran Berry | Blaze  
   IVs: 20 | Moves: Ember, Mach Punch, Taunt, Scratch

3. **Turtwig** (Lv 15) — Oran Berry | Overgrow  
   IVs: 20 | Moves: Razor Leaf, Bite, Reflect, Tackle

*Chimchar does not appear in any later Paul battle (released per anime canon).*

---

### Stage 2 — Mauville Battle
*Trainer ID: TRAINER_WALLY_MAUVILLE | Party: sParty_WallyMauville*  
*Full 6v6. No Full Restores.*

**Pressure Mechanic:** Custom Mauville-only fighting spirit hook. The first time each
of Paul's Pokémon would be knocked out from full HP by a single attack, it survives
with 1 HP and prints the fighting-spirit message. This is not implemented with
Focus Sash items and does not affect any other trainer battle.

Post-battle overworld note: Paul's Mauville exit uses the standard NPC Fly-out
field effect and sets the Wally/Mauville defeated flag path so the battle does not
loop.

1. **Electabuzz** (Lv 26) — held item per live trainer table | Static  
   IVs: 30 | Moves: Thunder Punch, Low Kick, Ice Punch, Quick Attack

2. **Torterra** (Lv 27) — held item per live trainer table | Overgrow  
   IVs: 30 | Moves: Wood Hammer, Earthquake, Crunch, Stone Edge

3. **Honchkrow** (Lv 26) — held item per live trainer table | Insomnia  
   IVs: 30 | Moves: Night Slash, Drill Peck, Sucker Punch, Superpower

4. **Weavile** (Lv 25) — held item per live trainer table | Pressure  
   IVs: 30 | Moves: Ice Punch, Night Slash, Brick Break, Aerial Ace

5. **Ursaring** (Lv 25) — held item per live trainer table | Guts  
   IVs: 30 | Moves: Earthquake, Slash, Brick Break, Crunch

6. **Gliscor** (Lv 26) — held item per live trainer table | Hyper Cutter  
   IVs: 30 | Moves: Earthquake, X-Scissor, Stone Edge, Aerial Ace

---

## Brendan — Scott Replacement / Champion Reveal Arc

Current live role:
- Scott is removed as a visible overworld presence. Existing Scott story flags and vars
  remain under the hood for save compatibility.
- Scott's visible map objects now use Brendan's overworld sprite.
- Match Call header now reads Brendan / BKLYN WATCH.
- Mauville post-Paul observer scene is now Brendan: he watches the Paul battle, speaks
  in Brooklyn lines, then battles the player instead of giving Scott's pep talk.
- All former Scott dialogue should now read as Brendan dialogue in-game. The internal
  label names may still say Scott where old flags/vars are preserved for save safety.

Brendan observer locations:
- Petalburg City opening observer beat
- Rustboro Pokemon School
- Slateport City museum / Battle Tent beats
- Mauville City post-Paul beat
- Verdanturf Battle Tent lobby
- Fallarbor Battle Tent lobby
- Route 119 rival aftermath
- Lilycove motel
- Mossdeep City
- Ever Grande Pokemon Center
- S.S. Tidal corridor
- Battle Frontier Reception Gate
- Battle Frontier house slot

### Arc Identity

Brendan starts as a quiet Brooklyn scout who is "watching the league from the block."
Across the former Scott route, he keeps showing up after major battles, measuring the
player's growth. His final reveal is that he is the real Pokemon Champion waiting at
the League endgame.

### Petalburg Observer Seed

Story function:
- Brendan first clocks the player in Petalburg.
- No battle required here by default; this is the "I am watching you" seed.
- Dialogue tone: Brooklyn scout, calm pressure, still hiding Champion status.

Team concept if promoted to battle later:
1. Mudkip (Lv 12) - Oran Berry  
   Moves: Water Gun, Mud-Slap, Tackle, Growl
2. Shroomish (Lv 12) - Oran Berry  
   Moves: Absorb, Stun Spore, Leech Seed, Headbutt
3. Bagon (Lv 13) - Dragon Fang  
   Moves: Bite, Dragon Breath, Headbutt, Leer

### Rustboro / Slateport Scout Tier

Team concept:
1. Marshtomp (Lv 22) - Eviolite  
   Moves: Mud Shot, Water Pulse, Rock Tomb, Protect
2. Breloom (Lv 22) - Toxic Orb  
   Moves: Mach Punch, Bullet Seed, Spore, Rock Tomb
3. Manectric (Lv 22) - Magnet  
   Moves: Volt Switch, Thunderbolt, Flame Burst, Thunder Wave
4. Shelgon (Lv 23) - Lum Berry  
   Moves: Dragon Claw, Fire Fang, Bite, Protect

### Mauville Observer Battle
*Trainer ID: TRAINER_BRENDAN_PLACEHOLDER | Party: sParty_BrendanBoss*  
*Full 6v6. No Full Restores.*

1. **Swampert** (Lv 38) — Leftovers  
   Moves: Stealth Rock, Earthquake, Waterfall, Ice Punch

2. **Breloom** (Lv 38) — Toxic Orb  
   Moves: Spore, Mach Punch, Bullet Seed, Rock Tomb

3. **Gliscor** (Lv 39) — Toxic Orb  
   Moves: Earthquake, Knock Off, Roost, Swords Dance

4. **Manectric** (Lv 39) — Choice Specs  
   Moves: Thunderbolt, Volt Switch, Flamethrower, Hidden Power

5. **Metagross** (Lv 40) — Assault Vest  
   Moves: Meteor Mash, Bullet Punch, Zen Headbutt, Earthquake

6. **Salamence** (Lv 41) — Lum Berry  
   Moves: Dragon Dance, Dragon Claw, Earthquake, Fire Fang

### Hard-Mode Brendan Observer Battle
*Party: sHardParty_BrendanBoss*

1. **Swampert** (Lv 58) — Leftovers  
   Moves: Stealth Rock, Earthquake, Waterfall, Ice Punch

2. **Breloom** (Lv 58) — Toxic Orb  
   Moves: Spore, Mach Punch, Bullet Seed, Rock Tomb

3. **Gliscor** (Lv 59) — Toxic Orb  
   Moves: Earthquake, Knock Off, Roost, Swords Dance

4. **Rotom-Wash** (Lv 59) — Choice Scarf  
   Moves: Hydro Pump, Volt Switch, Thunderbolt, Trick

5. **Metagross** (Lv 60) — Assault Vest  
   Moves: Meteor Mash, Bullet Punch, Zen Headbutt, Earthquake

6. **Salamence** (Lv 61) — Lum Berry  
   Moves: Dragon Dance, Dragon Claw, Earthquake, Fire Fang

Implementation note:
- Poison Heal was hardened in the end-turn poison path so Toxic Orb Gliscor heals
  reliably when poisoned and missing HP.

### Lilycove / Mossdeep Late Scout Tier

Team concept:
1. Swampert (Lv 72) - Leftovers  
   Moves: Stealth Rock, Earthquake, Flip Turn, Ice Punch
2. Breloom (Lv 72) - Toxic Orb  
   Moves: Spore, Mach Punch, Bullet Seed, Swords Dance
3. Gliscor (Lv 73) - Toxic Orb  
   Moves: Earthquake, Knock Off, Roost, Swords Dance
4. Rotom-Wash (Lv 73) - Choice Scarf  
   Moves: Hydro Pump, Volt Switch, Thunderbolt, Trick
5. Metagross (Lv 74) - Assault Vest  
   Moves: Meteor Mash, Bullet Punch, Zen Headbutt, Earthquake
6. Salamence (Lv 75) - Lum Berry  
   Moves: Dragon Dance, Dragon Claw, Earthquake, Fire Fang

### Ever Grande Champion Reveal

Story function:
- Brendan stops pretending to be only an observer.
- He reveals he has been scouting the player as the hidden Champion-level gatekeeper.
- This is the end of the former Scott path and the start of Brendan's full Champion
  identity.

Champion reveal team:
1. Deoxys-Speed (Lv 100) - Focus Sash - Pressure  
   Moves: Psycho Boost, Stealth Rock, Ice Beam, Taunt
2. Swampert-Mega (Lv 100) - Swampertite - Swift Swim  
   Moves: Waterfall, Earthquake, Ice Punch, Flip Turn
3. Breloom (Lv 100) - Toxic Orb - Poison Heal  
   Moves: Spore, Mach Punch, Bullet Seed, Swords Dance
4. Gliscor (Lv 100) - Toxic Orb - Poison Heal  
   Moves: Earthquake, Knock Off, Roost, Swords Dance
5. Metagross-Mega (Lv 100) - Metagrossite - Tough Claws  
   Moves: Meteor Mash, Bullet Punch, Zen Headbutt, Earthquake
6. Salamence-Mega (Lv 100) - Salamencite - Aerilate  
   Moves: Dragon Dance, Double-Edge, Earthquake, Roost

Champion pressure identity:
- Brendan is a tempo-control Champion: hazards, pivoting, Toxic Orb sustain, and
  late-game Mega cleaners.
- His Brooklyn dialogue should feel relaxed but dangerous, like he has been watching
  every mistake the player made since Petalburg.

### Stage 3 — Evergrande / Victory Road Final Team
*Trainer IDs: TRAINER_WALLY_VR_1 through TRAINER_WALLY_VR_5 (rematch tiers)*  
*Party: sParty_WallyVR1–sParty_WallyVR5 (levels scale 55–60 across rematch tiers)*  
*Full 6v6. TRAINER_WALLY_VR_2+ carry 2× Full Restore.*

**Pressure Mechanic — Planned:** Permanent Magma Storm field effect targeting the
player's entire party at battle start. This requires a custom battle initialization hook
(not yet implemented). When added, it will be wired through the existing BaneMode
battle hooks in `src/battle_setup.c`. Froslass and Ninjask carry Focus Sash in the
interim to represent the oppressive pressure feel.

1. **Aggron** (Lv 55–60) — Choice Band | Rock Head  
   IVs: 255 | Moves: Iron Tail, Stone Edge, Earthquake, Ice Punch

2. **Gastrodon** (Lv 54–59) — Leftovers | Storm Drain  
   IVs: 255 | Moves: Surf, Ice Beam, Earthquake, Recover

3. **Drapion** (Lv 55–60) — Scope Lens | Sniper  
   IVs: 255 | Moves: Night Slash, Crunch, Earthquake, X-Scissor

4. **Froslass** (Lv 54–59) — Focus Sash | Cursed Body  
   IVs: 255 | Moves: Shadow Ball, Ice Beam, Spikes, Thunderbolt

5. **Ninjask** (Lv 55–60) — Focus Sash | Speed Boost  
   IVs: 255 | Moves: Swords Dance, Baton Pass, X-Scissor, Protect

6. **Electivire** (Lv 56–60) — Expert Belt | Motor Drive  
   IVs: 255 | Moves: Thunder Punch, Ice Punch, Fire Punch, Low Kick

*Level ranges reflect VR1 (first encounter) through VR5 (max rematch tier).*

## May (Opening + Checkpoints)

### Route 103 opening
- Uxie (Lv 5) opening signature pick.

### Rustboro checkpoint
1. Uxie (Lv 15) - Oran Berry - Levitate  
Moves: Confusion, Thunder Wave, Reflect, Yawn

2. Pelipper (Lv 15) - Oran Berry - Keen Eye  
Moves: Wing Attack, Water Pulse, Supersonic, Protect

3. Croagunk (Lv 14) - Oran Berry - Anticipation  
Moves: Venoshock, Fake Out, Mud Shot, Pursuit

4. Roselia (Lv 14) - Oran Berry - Natural Cure  
Moves: Magical Leaf, Stun Spore, Leech Seed, Mega Drain

5. Misdreavus (Lv 13) - Oran Berry - Levitate  
Moves: Psybeam, Astonish, Confuse Ray, Pain Split

6. Elekid (Lv 13) - Oran Berry - Static  
Moves: Thunder Shock, Swift, Low Kick, Quick Attack

### Route 110
1. Uxie (Lv 21) - Sitrus Berry - Levitate  
Moves: Confusion, Thunder Wave, Reflect, U-turn

2. Pelipper (Lv 20) - Oran Berry - Keen Eye  
Moves: Wing Attack, Water Pulse, Supersonic, Protect

3. Lairon (Lv 20) - Oran Berry - Rock Head  
Moves: Metal Claw, Rock Tomb, Mud-Slap, Roar

4. Kirlia (Lv 20) - Oran Berry - Synchronize  
Moves: Psybeam, Draining Kiss, Calm Mind, Charge Beam

5. Numel (Lv 19) - Charcoal - Simple  
Moves: Flame Charge, Mud Shot, Rock Tomb, Yawn

6. Roselia (Lv 19) - Oran Berry - Natural Cure  
Moves: Magical Leaf, Growth, Stun Spore, Leech Seed

### Route 119
1. Uxie (Lv 41) - Leftovers - Levitate  
Moves: Psychic, Thunder Wave, Stealth Rock, U-turn

2. Pelipper (Lv 39) - Damp Rock - Drizzle  
Moves: Scald, Hurricane, U-turn, Roost

3. Aggron (Lv 39) - Leftovers - Rock Head  
Moves: Heavy Slam, Rock Slide, Earthquake, Thunder Wave

4. Gardevoir (Lv 39) - Expert Belt - Synchronize  
Moves: Psychic, Moonblast, Thunderbolt, Mystical Fire

5. Camerupt (Lv 40) - Passho Berry - Solid Rock  
Moves: Earth Power, Lava Plume, Ancient Power, Yawn

6. Roserade (Lv 40) - Black Sludge - Natural Cure  
Moves: Leaf Storm, Sludge Bomb, Sleep Powder, Spikes

### Lilycove
1. Uxie (Lv 80) - Leftovers - Levitate  
Moves: Psychic, Thunder Wave, Stealth Rock, U-turn

2. Pelipper (Lv 79) - Damp Rock - Drizzle  
Moves: Scald, Hurricane, U-turn, Roost

3. Aggron (Lv 79) - Leftovers - Rock Head  
Moves: Head Smash, Heavy Slam, Earthquake, Ice Punch

4. Gardevoir (Lv 79) - Choice Scarf - Synchronize  
Moves: Psychic, Moonblast, Thunderbolt, Mystical Fire

5. Camerupt (Lv 80) - Passho Berry - Solid Rock  
Moves: Earth Power, Fire Blast, Ancient Power, Yawn

6. Roserade (Lv 80) - Black Sludge - Natural Cure  
Moves: Leaf Storm, Sludge Bomb, Sleep Powder, Spikes

---

## 3) Gym Leaders (Swapped Order)

_Atlas synced to the live Quest branch (`pokeemerald_modern.gba`) on 2026-05-11. This reflects current script bindings and active trainer-party slots, including the veteran-trial-only Blaine / Erika / Sabrina teams and Fortree's Blue & Green restoration. Core boss rule: every non-Kanto gym leader variant now carries at least one paradox or legendary slot, with Red, Blue, and Green as the only exceptions. Unless a section says otherwise, the listed gym teams match the live hard-path boss teams used by the Quest build._

Level rule lock for gyms: each gym leader team is `current player cap + 2`, max 100.

## Gym 1 - Juan (Rustboro)

_Live hard-path team: yes._

1. Pelipper (Lv 18) - Damp Rock - Drizzle  
Moves: Scald, Hurricane, U-turn, Roost

2. Qwilfish (Lv 18) - Oran Berry - Intimidate  
Moves: Spikes, Aqua Jet, Poison Sting, Taunt

3. Barboach (Lv 18) - Oran Berry - Oblivious  
Moves: Mud Shot, Water Pulse, Protect, Tickle

4. Poliwhirl (Lv 18) - Oran Berry - Water Absorb  
Moves: Water Pulse, Hypnosis, Icy Wind, Protect

5. Suicune (Lv 18) - Sitrus Berry - Pressure  
Moves: Scald, Icy Wind, Rain Dance, Protect

6. Gyarados (Lv 18) - Gyaradosite - Intimidate  
Moves: Waterfall, Crunch, Ice Fang, Taunt

Locked pressure overlay: permanent rain + player-side speed reduced to one-third.

## Gym 2 - Flannery (Dewford)

1. Magmar (Lv 34) - Eviolite - Flame Body  
Moves: Flamethrower, Thunder Punch, Focus Blast, Clear Smog

2. Torkoal (Lv 34) - Heat Rock - Drought  
Moves: Lava Plume, Rapid Spin, Yawn, Stealth Rock

3. Rotom-Heat (Lv 34) - Heavy-Duty Boots - Levitate  
Moves: Overheat, Volt Switch, Will-O-Wisp, Thunder Wave

4. Arcanine (Lv 34) - Sitrus Berry - Intimidate  
Moves: Flare Blitz, Extreme Speed, Will-O-Wisp, Morning Sun

5. Ninetales (Lv 34) - Charcoal - Flash Fire  
Moves: Flamethrower, Solar Beam, Hypnosis, Nasty Plot

6. Gouging Fire (Lv 34) - Booster Energy - Protosynthesis  
Moves: Dragon Dance, Flare Blitz, Dragon Claw, Protect

Locked pressure overlay: Desolate Land sun (permanent harsh sunlight; Water-type attacks fail).

## Gym 3 - Brawly (Mauville)

1. Slither Wing (Lv 50) - Booster Energy - Protosynthesis  
Moves: Leech Life, Close Combat, Flare Blitz, Morning Sun

2. Breloom (Lv 50) - Focus Sash - Technician  
Moves: Spore, Bullet Seed, Mach Punch, Rock Tomb

3. Mienshao (Lv 50) - Assault Vest - Regenerator  
Moves: Close Combat, Knock Off, Stone Edge, U-turn

4. Toxicroak (Lv 50) - Black Sludge - Dry Skin  
Moves: Swords Dance, Drain Punch, Gunk Shot, Sucker Punch

5. Hawlucha (Lv 50) - Sitrus Berry - Unburden  
Moves: Swords Dance, Acrobatics, Close Combat, Taunt

6. Lucario (Lv 50) - Life Orb - Inner Focus  
Moves: Swords Dance, Close Combat, Meteor Mash, Extreme Speed

Locked pressure overlay: Sticky Web + 1 Spikes.

## Gym 4 - Winona (Lavaridge)

1. Pelipper (Lv 66) - Damp Rock - Drizzle  
Moves: Hurricane, Scald, U-turn, Roost

2. Gliscor (Lv 66) - Toxic Orb - Poison Heal  
Moves: Earthquake, Acrobatics, Protect, Swords Dance

3. Corviknight (Lv 66) - Rocky Helmet - Mirror Armor  
Moves: Brave Bird, Body Press, Roost, U-turn

4. Kilowattrel (Lv 66) - Heavy-Duty Boots - Wind Power  
Moves: Thunderbolt, Hurricane, Volt Switch, Roost

5. Iron Jugulis (Lv 66) - Booster Energy - Quark Drive  
Moves: Hurricane, Dark Pulse, Earth Power, Taunt

6. Zapdos (Lv 66) - Leftovers - Static  
Moves: Thunderbolt, Hurricane, Heat Wave, Roost

Locked pressure overlay: permanent opponent Tailwind.

## Gym 5 - Red (Petalburg)

1. Pikachu (Lv 82) - Light Ball - Static  
Moves: Volt Tackle, Extreme Speed, Knock Off, Fake Out

2. Venusaur (Lv 82) - Black Sludge - Overgrow  
Moves: Sleep Powder, Giga Drain, Sludge Bomb, Earth Power

3. Charizard (Lv 82) - Charizardite X - Blaze  
Moves: Dragon Dance, Flare Blitz, Dragon Claw, Earthquake

4. Blastoise (Lv 82) - White Herb - Torrent  
Moves: Shell Smash, Hydro Pump, Ice Beam, Dark Pulse

5. Snorlax (Lv 82) - Leftovers - Thick Fat  
Moves: Curse, Body Slam, Heat Crash, Rest

6. Espeon (Lv 82) - Focus Sash - Magic Bounce  
Moves: Psychic, Dazzling Gleam, Morning Sun, Calm Mind

Pressure identity: permanent sun plus Stealth Rock chip to force aggressive trades around Red's mixed manga core.

## Gym 6 - Blue and Green (Fortree, Doubles)

1. Rillaboom (Lv 98) - Assault Vest - Grassy Surge  
Moves: Grassy Glide, Wood Hammer, Fake Out, U-turn

2. Charizard (Lv 98) - Charizardite Y - Blaze  
Moves: Heat Wave, Air Slash, Solar Beam, Protect

3. Venusaur (Lv 98) - Life Orb - Chlorophyll  
Moves: Sleep Powder, Giga Drain, Sludge Bomb, Earth Power

4. Jolteon (Lv 98) - Focus Sash - Volt Absorb  
Moves: Thunderbolt, Volt Switch, Helping Hand, Protect

5. Aerodactyl (Lv 98) - Lum Berry - Pressure  
Moves: Rock Slide, Tailwind, Taunt, Protect

6. Blastoise (Lv 98) - White Herb - Torrent  
Moves: Shell Smash, Hydro Pump, Ice Beam, Protect

Locked pressure overlay: permanent sun + opponent-side Tailwind.
Pressure identity: Kanto starter core with speed control, sleep pressure, and Shell Smash cleanup.

## Gym 7 - Roxanne (Mossdeep)

1. Tyranitar (Lv 100) - Smooth Rock - Sand Stream  
Moves: Stealth Rock, Rock Blast, Crunch, Ice Punch

2. Glimmora (Lv 100) - Focus Sash - Toxic Debris  
Moves: Mortal Spin, Earth Power, Power Gem, Sludge Wave

3. Excadrill (Lv 100) - Life Orb - Sand Rush  
Moves: Swords Dance, Earthquake, Iron Head, Rock Slide

4. Cradily (Lv 100) - Leftovers - Storm Drain  
Moves: Giga Drain, Recover, Rock Slide, Toxic

5. Garganacl (Lv 100) - Covert Cloak - Purifying Salt  
Moves: Salt Cure, Recover, Body Press, Iron Defense

6. Diancie (Lv 100) - Diancite - Clear Body  
Moves: Protect, Moonblast, Diamond Storm, Earth Power

Locked pressure overlay: permanent sand + 33% SE damage reduction on Roxanne side + Stealth Rock pressure.
Pressure identity: sand attrition core with hazard stack, anti-water pivots, and fast sand cleaner.

## Gym 8 - Wattson (Sootopolis)

1. Rotom-Wash (Lv 100) - Leftovers - Levitate  
Moves: Hydro Pump, Volt Switch, Thunder Wave, Pain Split

2. Zapdos (Lv 100) - Heavy-Duty Boots - Static  
Moves: Thunderbolt, Hurricane, Heat Wave, Roost

3. Thundurus-Therian (Lv 100) - Life Orb - Volt Absorb  
Moves: Nasty Plot, Thunderbolt, Focus Blast, Grass Knot

4. Miraidon (Lv 100) - Air Balloon - Hadron Engine  
Moves: Electro Drift, Draco Meteor, Overheat, Volt Switch

5. Regieleki (Lv 100) - Air Balloon - Transistor  
Moves: Thunder Cage, Volt Switch, Electro Ball, Rapid Spin

6. Iron Hands (Lv 100) - Air Balloon - Quark Drive  
Moves: Swords Dance, Drain Punch, Wild Charge, Ice Punch

Locked pressure overlay: permanent Electric Terrain + full-team Ground-immunity theme.
Pressure identity: terrain-enabled volt-turn kill-chain with legendary speed pressure and full Ground-immunity theme.

---

## 4) Required Rematch Bosses (Pre-Gym 6 Gate)

Trigger rule:
- Immediately after Gym 6 victory, player must clear rematch trio before Gym 7 unlocks.

Mandatory post-badge message sequence (script text):
1. `LEAGUE ALERT: Seventh-Badge access is locked.`
2. `Complete the Veteran Trial: Blaine, Erika, and Sabrina.`
3. `Defeat all three to restore Gym challenge clearance.`

## Blaine Rematch (Doubles, Lv 98)

1. Torkoal - Heat Rock - Drought  
Moves: Eruption, Earth Power, Yawn, Protect
2. Lilligant-Hisui - Focus Sash - Chlorophyll  
Moves: Sleep Powder, After You, Leaf Storm, Close Combat
3. Volcarona - Heavy-Duty Boots - Flame Body  
Moves: Quiver Dance, Heat Wave, Bug Buzz, Protect
4. Chi-Yu - Choice Scarf - Beads of Ruin  
Moves: Heat Wave, Dark Pulse, Overheat, Snarl
5. Arcanine-Hisui - Shuca Berry - Intimidate  
Moves: Flare Blitz, Rock Slide, Extreme Speed, Will-O-Wisp
6. Houndoom - Houndoominite - Solar Power  
Moves: Nasty Plot, Heat Wave, Dark Pulse, Solar Beam

Pressure identity:
- permanent sun + Water-move immunity rule,
- doubles speed control (`After You`, Scarf, priority),
- immediate spread damage pressure from turn 1.

Player prep:
- bring weather control or anti-sun tech,
- carry Wide Guard or spread-damage resistance,
- prepare sleep control for Lilligant-H.

## Erika Rematch (Doubles, Lv 98)

1. Venusaur - Life Orb - Chlorophyll  
Moves: Sleep Powder, Giga Drain, Sludge Bomb, Earth Power
2. Rillaboom - Assault Vest - Grassy Surge  
Moves: Grassy Glide, Wood Hammer, Fake Out, U-turn
3. Ferrothorn - Leftovers - Iron Barbs  
Moves: Power Whip, Gyro Ball, Leech Seed, Protect
4. Amoonguss - Black Sludge - Regenerator  
Moves: Spore, Giga Drain, Clear Smog, Protect
5. Roserade - Focus Sash - Technician  
Moves: Leaf Storm, Sludge Bomb, Sleep Powder, Protect
6. Leafeon - Lum Berry - Chlorophyll  
Moves: Leaf Blade, X-Scissor, Swords Dance, Protect

Pressure identity:
- permanent Grassy Terrain,
- layered sleep and healing pressure,
- bulky grass core with priority cleanup and setup finishers.

Player prep:
- bring strong Fire, Flying, or Poison coverage,
- have status control for repeated sleep attempts,
- expect Grassy Terrain sustain to drag trades out.

## Sabrina Trial Rematch (Singles, Lv 100)

1. Alakazam - Focus Sash - Magic Guard  
Moves: Psychic, Shadow Ball, Focus Blast, Nasty Plot
2. Espeon - Leftovers - Magic Bounce  
Moves: Calm Mind, Psychic, Dazzling Gleam, Morning Sun
3. Gardevoir - Life Orb - Trace  
Moves: Moonblast, Psychic, Mystical Fire, Focus Blast
4. Gallade - Lum Berry - Sharpness  
Moves: Swords Dance, Psycho Cut, Close Combat, Shadow Sneak
5. Metagross - Metagrossite - Clear Body  
Moves: Meteor Mash, Zen Headbutt, Earthquake, Bullet Punch
6. Hatterene - Assault Vest - Magic Bounce  
Moves: Psychic, Dazzling Gleam, Mystical Fire, Giga Drain

Pressure identity:
- permanent Psychic Terrain,
- hard special opening into physical cleanup,
- anti-priority field control with Mega Metagross as the closer.

Player prep:
- bring Steel/Dark answers that don't fold to mixed coverage,
- don't rely on priority to stabilize,
- be ready for setup from both special and physical angles.

## Brandon (Ever Grande Pre-League Regi Trial)

1. Regirock (Lv 100) - Leftovers - Clear Body  
Moves: Stone Edge, Earthquake, Body Press, Stealth Rock
2. Regice (Lv 100) - Assault Vest - Clear Body  
Moves: Freeze-Dry, Thunderbolt, Focus Blast, Ice Beam
3. Registeel (Lv 100) - Leftovers - Clear Body  
Moves: Iron Defense, Body Press, Heavy Slam, Thunder Wave
4. Regieleki (Lv 100) - Magnet - Transistor  
Moves: Thunder Cage, Volt Switch, Electro Ball, Rapid Spin
5. Regidrago (Lv 100) - Choice Specs - Dragon's Maw  
Moves: Dragon Energy, Draco Meteor, Flamethrower, Ancient Power
6. Regigigas (Lv 100) - Leftovers - Slow Start  
Moves: Protect, Body Slam, Drain Punch, Ice Punch

Pressure identity:
- permanent sand plus Stealth Rock,
- full-Regi stat wall into fast electric tempo,
- pre-League gate check before the Ever Grande Pokemon Center / Elite Four run.

---

## 5) Villain Boss Teams (Team Rocket Canon)
---

## 5) Villain Boss Teams (Team Rocket Canon)

Level rule lock for Team Rocket encounters: grunts are mini-boss-tier and scale to current highest player party level. Giovanni and major Rocket bosses scale to current highest player party level +2, capped at 100.

### Rocket Grunt Party Standard
- Rocket grunts now use curated full 6-mon singles parties instead of small vanilla parties.
- Their live roster pool currently rotates through competitive-style poison/dark/flying pressure pieces such as:
  - `Nidoking`
  - `Nidoqueen`
  - `Crobat`
  - `Weezing`
  - `Muk`
  - `Honchkrow`
  - `Houndoom`
  - `Drapion`
  - `Skuntank`
  - `Persian`
  - `Mismagius`
  - `Weavile`
- Live Rocket grunt pressure package: player side starts with `2 layers of Toxic Spikes`.

## Arianna (Rocket Admin)

1. Nidoqueen (Lv 100) - Black Sludge - Sheer Force  
Moves: Earth Power, Sludge Wave, Ice Beam, Stealth Rock
2. Roserade (Lv 100) - Focus Sash - Natural Cure  
Moves: Sleep Powder, Toxic Spikes, Sludge Bomb, Leaf Storm
3. Mismagius (Lv 100) - Colbur Berry - Levitate  
Moves: Will-O-Wisp, Hex, Mystical Fire, Taunt
4. Weavile (Lv 100) - Life Orb - Pressure  
Moves: Triple Axel, Knock Off, Low Kick, Ice Shard
5. Hatterene (Lv 100) - Leftovers - Magic Bounce  
Moves: Calm Mind, Psyshock, Draining Kiss, Mystical Fire
6. Mawile (Lv 100) - Mawilite - Intimidate  
Moves: Swords Dance, Play Rough, Sucker Punch, Fire Fang

Pressure identity: status + hazard compression into late-game mega cleanup.

## Giovanni Final Boss

1. Nidoking (Lv 100) - Life Orb - Sheer Force  
Moves: Earth Power, Sludge Wave, Ice Beam, Thunderbolt
2. Rhyperior (Lv 100) - Leftovers - Solid Rock  
Moves: Earthquake, Rock Blast, Megahorn, Stealth Rock
3. Krookodile (Lv 100) - Choice Scarf - Intimidate  
Moves: Earthquake, Knock Off, Stone Edge, Close Combat
4. Honchkrow (Lv 100) - Scope Lens - Super Luck  
Moves: Brave Bird, Sucker Punch, Superpower, Roost
5. Garchomp (Lv 100) - Yache Berry - Rough Skin  
Moves: Swords Dance, Earthquake, Scale Shot, Fire Fang
6. Mewtwo (Lv 100) - Mewtwonite Y - Pressure  
Moves: Psystrike, Aura Sphere, Ice Beam, Recover

Pressure identity: brute-force execution with layered Ground pressure into mega legendary finisher.

## Norman (Staged Late Rocket Reveal Team)

Status note:
- This is **not** the live Petalburg gym scene.
- Petalburg still uses the retired-Norman NY dialogue beat.
- This is the current staged 6v6 team slot intended for Norman's later Rocket-reveal boss binding.

1. Porygon2 (Lv 42) - Eviolite  
Moves: Tri Attack, Ice Beam, Recover, Thunder Wave
2. Slaking (Lv 42) - Sitrus Berry  
Moves: Double-Edge, Earthquake, Fire Punch, Yawn
3. Snorlax (Lv 43) - Leftovers  
Moves: Body Slam, Crunch, Rest, Sleep Talk
4. Kangaskhan (Lv 43) - Silk Scarf  
Moves: Fake Out, Return, Sucker Punch, Earthquake
5. Ursaring (Lv 44) - Flame Orb  
Moves: Facade, Crunch, Close Combat, Protect
6. Slaking (Lv 45) - Chesto Berry  
Moves: Giga Impact, Night Slash, Fire Punch, Rest

Pressure identity:
- heavy Normal-body pressure with para/yawn tempo,
- bulky sustain into double Slaking endgame,
- staged as a later-story boss package rather than a badge-fight team.

## Archer (Rocket Admin)

1. Crobat (Lv 100) - Black Sludge - Infiltrator  
Moves: Brave Bird, U-turn, Taunt, Roost
2. Porygon-Z (Lv 100) - Choice Scarf - Adaptability  
Moves: Tri Attack, Dark Pulse, Ice Beam, Trick
3. Magnezone (Lv 100) - Air Balloon - Magnet Pull  
Moves: Thunderbolt, Flash Cannon, Volt Switch, Body Press
4. Houndoom (Lv 100) - Heavy-Duty Boots - Flash Fire  
Moves: Flamethrower, Dark Pulse, Nasty Plot, Taunt
5. Honchkrow (Lv 100) - Choice Band - Moxie  
Moves: Brave Bird, Knock Off, Superpower, U-turn
6. Iron Jugulis (Lv 100) - Booster Energy - Quark Drive  
Moves: Hurricane, Dark Pulse, Flamethrower, Taunt

Pressure identity: high-velocity pivot assault with anti-setup disruption.

---

## 6) Elite Four + Final Gate

Roster lock:
- Yes, every standard Elite Four member (Ash, Alain, Silver, Lance) uses a full 6-Pokemon team.

League flow lock:
- Portable PC is available throughout the game.
- Portable PC is disabled for the entire Elite Four challenge run (from E4 entry until final gate completion).
- After each E4 member defeat, player receives a heal reminder message before proceeding.

## Elite Four 1 - Ash

1. Pikachu (Lv 100) - Light Ball - Static  
Moves: Volt Tackle, Thunderbolt, Iron Tail, Fake Out

2. Dragonite (Lv 100) - Heavy-Duty Boots - Multiscale  
Moves: Dragon Dance, Dual Wingbeat, Earthquake, Extreme Speed

3. Gengar (Lv 100) - Gengarite - Cursed Body  
Moves: Nasty Plot, Shadow Ball, Sludge Wave, Focus Blast

4. Lucario (Lv 100) - Life Orb - Inner Focus  
Moves: Swords Dance, Close Combat, Meteor Mash, Extreme Speed

5. Sirfetch'd (Lv 100) - Leek - Scrappy  
Moves: Close Combat, Leaf Blade, Knock Off, First Impression

6. Dracovish (Lv 100) - Choice Scarf - Strong Jaw  
Moves: Fishious Rend, Crunch, Psychic Fangs, Ice Fang

Pressure identity: immediate tempo with priority layers and high-velocity offense.
Player prep: bring fast revenge options and at least one sturdy Electric/Flying check.

## Elite Four 2 - Alain

1. Chesnaught (Lv 100) - Leftovers - Bulletproof  
Moves: Spikes, Wood Hammer, Drain Punch, Synthesis

2. Bisharp (Lv 100) - Black Glasses - Defiant  
Moves: Swords Dance, Kowtow Cleave, Iron Head, Sucker Punch

3. Weavile (Lv 100) - Heavy-Duty Boots - Pressure  
Moves: Triple Axel, Knock Off, Ice Shard, Low Kick

4. Tyranitar (Lv 100) - Smooth Rock - Sand Stream  
Moves: Stealth Rock, Stone Edge, Crunch, Thunder Wave

5. Metagross (Lv 100) - Assault Vest - Clear Body  
Moves: Meteor Mash, Earthquake, Zen Headbutt, Bullet Punch

6. Charizard (Lv 100) - Charizardite X - Blaze  
Moves: Dragon Dance, Flare Blitz, Dragon Claw, Earthquake

Pressure identity: hazard stack into physical setup sweepers.
Player prep: preserve hazard control and keep a hard answer to DD Charizard X.

## Elite Four 3 - Silver

1. Weavile (Lv 100) - Heavy-Duty Boots - Pressure  
Moves: Triple Axel, Knock Off, Ice Shard, Low Kick
2. Crobat (Lv 100) - Black Sludge - Infiltrator  
Moves: Brave Bird, U-turn, Taunt, Roost
3. Alakazam (Lv 100) - Focus Sash - Magic Guard  
Moves: Psyshock, Focus Blast, Shadow Ball, Encore
4. Gengar (Lv 100) - Life Orb - Cursed Body  
Moves: Shadow Ball, Sludge Wave, Nasty Plot, Focus Blast
5. Feraligatr (Lv 100) - Mystic Water - Sheer Force  
Moves: Dragon Dance, Liquidation, Crunch, Ice Punch
6. Gyarados (Lv 100) - Gyaradosite - Intimidate  
Moves: Dragon Dance, Waterfall, Crunch, Earthquake

Pressure identity: anti-control burst with disruptive pivots and double setup endgame.
Player prep: deny setup turns and keep both physical and special revenge lines available.

## Elite Four 4 - Lance

1. Dragonite (Lv 100) - Weakness Policy - Multiscale  
Moves: Dragon Dance, Dual Wingbeat, Earthquake, Extreme Speed
2. Gyarados (Lv 100) - Heavy-Duty Boots - Intimidate  
Moves: Dragon Dance, Waterfall, Crunch, Taunt
3. Aerodactyl (Lv 100) - Focus Sash - Pressure  
Moves: Stone Edge, Dual Wingbeat, Earthquake, Taunt
4. Kingdra (Lv 100) - Scope Lens - Sniper  
Moves: Draco Meteor, Hydro Pump, Hurricane, Focus Energy
5. Salamence (Lv 100) - Choice Scarf - Intimidate  
Moves: Draco Meteor, Flamethrower, Hydro Pump, U-turn
6. Hydreigon (Lv 100) - Life Orb - Levitate  
Moves: Draco Meteor, Dark Pulse, Flamethrower, Flash Cannon

Pressure identity: layered dragon tempo with mixed-speed threats and anti-stall taunt lines.
Player prep: bring Fairy/Ice backbone plus ways to handle both DD and Scarf speed control.

## Elite Four Final Gate - Brendan + Bane (Two Consecutive Battles)

Format:
- Gate Battle 1 triggers immediately after E4 Lance.
- Gate Battle 2 triggers immediately after Battle 1 victory.
- Player party is fully healed between Gate Battle 1 and Gate Battle 2.
- League clear flag is only awarded after Battle 2 victory.
- No Hall of Fame registration between Battle 1 and Battle 2.

### Brendan Squad (Most Powerful Core)

1. Arceus-Water (Lv 100) - Splash Plate - Multitype  
Moves: Calm Mind, Judgment, Ice Beam, Recover

2. Deoxys-Speed (Lv 100) - Focus Sash - Pressure  
Moves: Psycho Boost, Ice Beam, Superpower, Stealth Rock

3. Sceptile (Lv 100) - Sceptilite - Overgrow  
Moves: Leaf Storm, Dragon Pulse, Focus Blast, Giga Drain

### Bane Squad (Elite Four Partner Threat)

1. Charizard (Lv 100) - Heavy-Duty Boots - Blaze  
Moves: Fire Blast, Hurricane, Scorching Sands, Roost

2. Houndoom (Lv 100) - Choice Scarf - Flash Fire  
Moves: Flamethrower, Dark Pulse, Nasty Plot, Destiny Bond

3. Mewtwo (Lv 100) - Mewtwonite X - Pressure  
Moves: Bulk Up, Drain Punch, Zen Headbutt, Ice Punch

Battle 1 pressure identity:
- Brendan controls tempo and board state (Arceus sustain + Deoxys lead pressure).
- Bane forces immediate kill pressure and punishes passive lines (Mewtwo-X endgame threat).
- Together they function as a balanced control + execution boss package.

## Gate Battle 2 - Legendary Rematch (3 + 3)

### Brendan Legendary Squad

1. Kyogre (Lv 100) - Blue Orb - Drizzle  
Moves: Water Spout, Origin Pulse, Ice Beam, Thunder

2. Rayquaza (Lv 100) - Life Orb - Air Lock  
Moves: Dragon Ascent, Earthquake, Extreme Speed, Swords Dance

3. Deoxys-Attack (Lv 100) - Focus Sash - Pressure  
Moves: Psycho Boost, Superpower, Ice Beam, Extreme Speed

### Bane Legendary Squad

1. Reshiram (Lv 100) - Choice Scarf - Turboblaze  
Moves: Blue Flare, Draco Meteor, Earth Power, Shadow Ball

2. Darkrai (Lv 100) - Life Orb - Bad Dreams  
Moves: Dark Pulse, Sludge Bomb, Ice Beam, Nasty Plot

3. Mewtwo (Lv 100) - Mewtwonite X - Pressure  
Moves: Bulk Up, Drain Punch, Zen Headbutt, Ice Punch

Battle 2 pressure identity:
- all-legendary burst with minimal safe pivots,
- immediate speed and power checks from turn 1,
- endurance pressure after Battle 1 resource drain.

Player prep:
- bring one dedicated Arceus-Water answer and one dedicated Mewtwo-X answer,
- do not let Deoxys get free hazard tempo,
- keep at least one fast revenge line for legendary speed threats in Battle 2.

---

## 7) Teams To Finalize During Rebuild (Not Fully Locked Here)

- Norman remains retired from the Petalburg badge fight in the current Quest branch.
- Live Petalburg scene status: at 3 badges, Norman explicitly says he's retired and doesn't yet know who is taking the upstairs chair full-time.
- Norman's separate Rocket-reveal boss team still exists as a later-story canonical slot to finalize and bind to the final Rocket battle.
- Rocket admin final rebuilt teams for each storyline checkpoint.
- Paul later benchmark battles beyond early mandatory fight.
- Route-by-route mini-boss sheet now documented in `BaneMode_MiniBoss_Team_Sheet.md`.

---

## 8) Quick Review Checklist

When reviewing this atlas, confirm:
1. Team species list per battle is correct.
2. Level curve aligns with gym cap model.
3. Itemization supports intended pressure identity.
4. Any legacy story-team mismatch is marked for canonical replacement.
