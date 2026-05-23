# Regional Forms Guide

This guide documents the regional-form and modern cross-gen evolution methods that are currently implemented in the Quest migration build. The methods below are taken from the live evolution table and the live wild encounter table, so they reflect what the ROM is actually checking.

## Key item sources

- Early evolution-item marts:
  - Petalburg Mart
  - Rustboro Mart
- Expanded modern-item stock:
  - Slateport specialty shops
- Legacy catch-up bundle:
  - Professor Birch's restore path for older saves

## How to get the Mega Stones into the bag

- The ROM uses a dedicated Mega Stone pocket.
- The practical player-facing source for the full Mega Stone set is **Professor Birch's legacy restore bundle** for migrated / older saves.
- That restore path grants:
  - every TM from `TM01` through `TM100`,
  - every Mega Stone from `Venusaurite` through `Diancite`,
  - modern evolution items like `Black Augurite`, `Linking Cord`, `Protector`, `Electirizer`, `Magmarizer`, `Dubious Disc`, `Reaper Cloth`, `Prism Scale`, `Sachet`, `Whipped Dream`, and `Peat Block`.
- If the bag is too full, the restore path pushes overflow to the PC item storage instead of dropping items.
- In practice: if you are on a migrated or older save and need the whole Mega Stone pocket filled, talk to **Professor Birch** and use the restore/catch-up bundle path.

## Direct wild regional-form catch locations

These are the regional-form species that are directly catchable in the current wild encounter table.

### Route 101
- Galarian Zigzagoon
  - Land encounter
  - Lv. 3

### Route 102
- Hisuian Qwilfish
  - Fishing
  - Lv. 12-18

### Route 103
- Paldean Wooper
  - Land encounter
  - Lv. 2-5
- Hisuian Zorua
  - Land encounter
  - Lv. 2

### Route 104
- Galarian Farfetch'd
  - Land encounter
  - Lv. 4

### Route 106 / Dewford waters
- Hisuian Qwilfish
  - Water encounter on Route 106
  - Lv. 15-25
- Hisuian Qwilfish
  - Water encounter in Dewford Town
  - Lv. 15-25

### Route 111 / Mirage Tower desert lane
- Alolan Diglett
  - Route 111 land encounter
  - Lv. 20
- Alolan Geodude
  - Route 111 Rock Smash encounter
  - Lv. 10-15
- Alolan Diglett
  - Mirage Tower 1F-4F land encounter
  - Lv. 20

### Granite Cave line
- Alolan Geodude
  - Granite Cave 1F land encounter
  - Lv. 6
- Alolan Rattata
  - Granite Cave B1F land encounter
  - Lv. 10
- Alolan Geodude
  - Granite Cave B2F Rock Smash encounter
  - Lv. 5-10

### Route 113 / Jagged Pass
- Hisuian Growlithe
  - Route 113 hidden encounter
  - Lv. 10-12
- Paldean Tauros (Blaze Breed)
  - Jagged Pass hidden encounter
  - Lv. 5

### Route 114 / 115
- Paldean Tauros (Aqua Breed)
  - Route 114 land encounter
  - Lv. 15
- Alolan Rattata
  - Route 115 land encounter
  - Lv. 23
- Alolan Sandshrew
  - Route 115 land encounter
  - Lv. 25
- Alolan Vulpix
  - Route 115 land encounter
  - Lv. 24
- Alolan Meowth
  - Route 115 land encounter
  - Lv. 25
- Alolan Grimer
  - Route 115 land encounter
  - Lv. 25-26
- Alolan Geodude
  - Route 115 land encounter
  - Lv. 25-26

### Route 118 / 119
- Galarian Linoone
  - Route 118 land encounter
  - Lv. 26
- Hisuian Electrode
  - Route 118 land encounter
  - Lv. 25
- Galarian Zigzagoon
  - Route 119 land encounter
  - Lv. 25
- Galarian Linoone
  - Route 119 land encounter
  - Lv. 25

### Route 123 / New Mauville
- Paldean Tauros (Combat Breed)
  - Route 123 land encounter
  - Lv. 26
- Hisuian Voltorb
  - New Mauville Inside land encounter
  - Lv. 24
- Alolan Geodude
  - New Mauville Entrance land encounter
  - Lv. 24

### Mt. Pyre
- Alolan Marowak
  - Mt. Pyre Exterior land encounter
  - Lv. 27
- Alolan Marowak
  - Mt. Pyre Summit land encounter
  - Lv. 27

### Shoal Cave
- Alolan Sandslash
  - Shoal Cave Low Tide Entrance Room land encounter
  - Lv. 32
- Galarian Mr. Mime
  - Shoal Cave Low Tide Ice Room land encounter
  - Lv. 32
- Hisuian Sneasel
  - Shoal Cave Low Tide Ice Room hidden encounter
  - Lv. 25-26

### Desert Underpass
- Galarian Rapidash
  - Land encounter
  - Lv. 43
- Galarian Weezing
  - Land encounter
  - Lv. 44
- Alolan Ninetales
  - Land encounter
  - Lv. 45

## Alolan Evolutions

### Pikachu -> Alolan Raichu
- Method:
  - Level in New Mauville, or
  - Seasonal summer evolution path

### Exeggcute -> Alolan Exeggutor
- Method:
  - Evolve in Petalburg Woods

### Cubone -> Alolan Marowak
- Method:
  - Use Dusk Stone

### Alolan Sandshrew -> Alolan Sandslash
- Method:
  - Ice Stone, or
  - Seasonal winter path

### Alolan Vulpix -> Alolan Ninetales
- Method:
  - Ice Stone, or
  - Seasonal winter path

### Alolan Diglett -> Alolan Dugtrio
- Method:
  - Level 26

### Alolan Meowth -> Alolan Persian
- Method:
  - Friendship

### Alolan Geodude -> Alolan Graveler -> Alolan Golem
- Method:
  - Geodude -> Graveler at level 25
  - Graveler -> Golem by trade or Linking Cord

### Alolan Grimer -> Alolan Muk
- Method:
  - Level 38

## Galarian Evolutions

### Galarian Slowpoke
- To Galarian Slowbro:
  - Use Galarica Cuff
- To Galarian Slowking:
  - Use Galarica Wreath

### Galarian Farfetch'd -> Sirfetch'd
- Method:
  - Land 3 critical hits in the evolution check path

### Galarian Mr. Mime -> Mr. Rime
- Method:
  - Level 42

### Galarian Corsola -> Cursola
- Method:
  - Level 38

### Galarian Zigzagoon -> Galarian Linoone -> Obstagoon
- Method:
  - Zigzagoon -> Linoone at level 20
  - Linoone -> Obstagoon at night at level 35

### Galarian Darumaka -> Galarian Darmanitan
- Method:
  - Ice Stone

### Galarian Yamask -> Runerigus
- Method:
  - Scripted damage-trigger evolution path

### Galarian Meowth -> Perrserker
- Method:
  - Level 28

## Hisuian Evolutions

### Quilava -> Hisuian Typhlosion
- Method:
  - Use Dusk Stone

### Dewott -> Hisuian Samurott
- Method:
  - Use Moon Stone

### Dartrix -> Hisuian Decidueye
- Method:
  - Use Leaf Stone

### Petilil -> Hisuian Lilligant
- Method:
  - Use Leaf Stone

### Rufflet -> Hisuian Braviary
- Method:
  - Use Dawn Stone

### Goomy -> Hisuian Sliggoo
- Method:
  - Use Shiny Stone

### Hisuian Sliggoo -> Hisuian Goodra
- Method:
  - Level 50 in rain or fog

### Bergmite -> Hisuian Avalugg
- Method:
  - Use Ice Stone

### Scyther -> Kleavor
- Method:
  - Use Black Augurite

### Ursaring -> Ursaluna
- Method:
  - Use Dusk Stone

### Stantler -> Wyrdeer
- Method:
  - Know Psyshield Bash

### White-Striped Basculin -> Basculegion
- Method:
  - Level 30

### Hisuian Qwilfish -> Overqwil
- Method:
  - Know Barb Barrage

### Hisuian Sneasel -> Sneasler
- Method:
  - Use Razor Claw during the day

### Hisuian Growlithe -> Hisuian Arcanine
- Method:
  - Use Fire Stone

### Hisuian Voltorb -> Hisuian Electrode
- Method:
  - Use Leaf Stone

### Hisuian Zorua -> Hisuian Zoroark
- Method:
  - Level 30

## Paldean and other modern evolutions

### Paldean Wooper -> Clodsire
- Method:
  - Level 20

### Girafarig -> Farigiraf
- Method:
  - Know Twin Beam

### Primeape -> Annihilape
- Method:
  - Level 35 in this build

### Dunsparce -> Dudunsparce
- Method:
  - Know Hyper Drill

### Bisharp -> Kingambit
- Method:
  - Use Leader's Crest

## Trade-style item evolutions that are item-compatible in this build

- Poliwhirl -> Politoed with King's Rock
- Slowpoke -> Slowking with King's Rock
- Seadra -> Kingdra with Dragon Scale
- Electabuzz -> Electivire with Electirizer
- Magmar -> Magmortar with Magmarizer
- Porygon2 -> Porygon-Z with Dubious Disc
- Feebas -> Milotic with Prism Scale
- Dusclops -> Dusknoir with Reaper Cloth
- Spritzee -> Aromatisse with Sachet
- Swirlix -> Slurpuff with Whipped Dream

## Mega Stone note

The ROM already supports a dedicated Mega Stone pocket. The live migration build also distributes Mega Stones through Birch catch-up logic and modern-item support paths rather than a separate TM-style inventory.
