from pathlib import Path

ROOTS = [
    Path("/Users/ivan/Documents/BaneMode_Rocket_Porymap_Work/pokeemerald-rocket-porymap"),
    Path("/Users/ivan/Documents/BaneMode_Rocket_Porymap_Work"),
]

OLD_ROXANNE_BLOCK = """=== TRAINER_TATE_AND_LIZA_1 ===
Name: BLUE&GREEN
Class: Leader
Pic: Champion Rival Frlg
Gender: Male
Music: Female
Items: Full Restore / Full Restore
Double Battle: Yes
AI: Check Bad Move / Try To Faint / Check Viability / Smart Switching / Smart Mon Choices / Powerful Status / Ace Pokemon

SPECIES_RILLABOOM @ ITEM_TERRAIN_EXTENDER
Ability: ABILITY_GRASSY_SURGE
Level: 96
EVs: 252 HP / 252 Atk / 4 SpD
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
Nature: Adamant
- MOVE_FAKE_OUT
- MOVE_GRASSY_GLIDE
- MOVE_WOOD_HAMMER
- MOVE_U_TURN

SPECIES_ARCHALUDON @ ITEM_ASSAULT_VEST
Ability: ABILITY_STAMINA
Level: 96
EVs: 252 HP / 4 Def / 252 SpA
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
Nature: Modest
- MOVE_ELECTRO_SHOT
- MOVE_DRACO_METEOR
- MOVE_FLASH_CANNON
- MOVE_BODY_PRESS

SPECIES_KINGAMBIT @ ITEM_BLACK_GLASSES
Ability: ABILITY_DEFIANT
Level: 96
EVs: 252 Atk / 4 SpD / 252 Spe
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
Nature: Adamant
- MOVE_KOWTOW_CLEAVE
- MOVE_SUCKER_PUNCH
- MOVE_IRON_HEAD
- MOVE_PROTECT

SPECIES_DRAGONITE @ ITEM_LUM_BERRY
Ability: ABILITY_MULTISCALE
Level: 96
EVs: 252 HP / 252 Atk / 4 SpD
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
Nature: Adamant
- MOVE_EXTREME_SPEED
- MOVE_EARTHQUAKE
- MOVE_DRACO_METEOR
- MOVE_PROTECT

SPECIES_URSALUNA_BLOODMOON @ ITEM_LEFTOVERS
Ability: ABILITY_MINDS_EYE
Level: 96
EVs: 252 HP / 4 Def / 252 SpA
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
Nature: Modest
- MOVE_BLOOD_MOON
- MOVE_EARTH_POWER
- MOVE_HYPER_VOICE
- MOVE_PROTECT

SPECIES_SAMUROTT_HISUI @ ITEM_FOCUS_SASH
Ability: ABILITY_SHARPNESS
Level: 96
EVs: 252 Atk / 4 SpD / 252 Spe
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
Nature: Jolly
- MOVE_CEASELESS_EDGE
- MOVE_RAZOR_SHELL
- MOVE_AQUA_JET
- MOVE_PROTECT
"""

NEW_ROXANNE_BLOCK = """=== TRAINER_TATE_AND_LIZA_1 ===
Name: ROXANNE
Class: Leader
Pic: Leader Roxanne
Gender: Female
Music: Female
Items: Full Restore / Full Restore
Double Battle: No
AI: Check Bad Move / Try To Faint / Check Viability / Smart Switching / Smart Mon Choices / Powerful Status / Ace Pokemon / Omniscient

SPECIES_TYRANITAR @ ITEM_SMOOTH_ROCK
Ability: ABILITY_SAND_STREAM
Level: 100
EVs: 252 HP / 252 Atk / 4 SpD
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
Nature: Adamant
- MOVE_STEALTH_ROCK
- MOVE_ROCK_BLAST
- MOVE_CRUNCH
- MOVE_ICE_PUNCH

SPECIES_GLIMMORA @ ITEM_FOCUS_SASH
Ability: ABILITY_TOXIC_DEBRIS
Level: 100
EVs: 4 Def / 252 SpA / 252 Spe
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
Nature: Timid
- MOVE_MORTAL_SPIN
- MOVE_EARTH_POWER
- MOVE_POWER_GEM
- MOVE_SLUDGE_WAVE

SPECIES_EXCADRILL @ ITEM_LIFE_ORB
Ability: ABILITY_SAND_RUSH
Level: 100
EVs: 252 Atk / 4 SpD / 252 Spe
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
Nature: Adamant
- MOVE_SWORDS_DANCE
- MOVE_EARTHQUAKE
- MOVE_IRON_HEAD
- MOVE_ROCK_SLIDE

SPECIES_CRADILY @ ITEM_LEFTOVERS
Ability: ABILITY_STORM_DRAIN
Level: 100
EVs: 252 HP / 4 Def / 252 SpD
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
Nature: Calm
- MOVE_GIGA_DRAIN
- MOVE_RECOVER
- MOVE_ROCK_SLIDE
- MOVE_TOXIC

SPECIES_GARGANACL @ ITEM_COVERT_CLOAK
Ability: ABILITY_PURIFYING_SALT
Level: 100
EVs: 252 HP / 4 Atk / 252 SpD
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
Nature: Careful
- MOVE_SALT_CURE
- MOVE_RECOVER
- MOVE_BODY_PRESS
- MOVE_IRON_DEFENSE

SPECIES_DIANCIE @ ITEM_DIANCITE
Ability: ABILITY_CLEAR_BODY
Level: 100
EVs: 252 HP / 4 SpA / 252 Spe
IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
Nature: Timid
- MOVE_PROTECT
- MOVE_MOONBLAST
- MOVE_DIAMOND_STORM
- MOVE_EARTH_POWER
"""


def replace_once(text, old, new, path):
    if old not in text:
        raise SystemExit(f"Expected text not found in {path}")
    return text.replace(old, new, 1)


def update_trainers(root):
    path = root / "src/data/trainers.party"
    text = path.read_text()
    text = replace_once(
        text,
        """=== TRAINER_WINONA_1 ===
Name: TATE&LIZA
Class: Leader
Pic: Leader Tate And Liza
Gender: Male""",
        """=== TRAINER_WINONA_1 ===
Name: BLUE&GREEN
Class: Leader
Pic: Champion Rival Frlg
Gender: Male""",
        path,
    )
    text = replace_once(text, OLD_ROXANNE_BLOCK, NEW_ROXANNE_BLOCK, path)
    path.write_text(text)


def update_mossdeep(root):
    scripts = root / "data/maps/MossdeepCity_Gym/scripts.inc"
    text = scripts.read_text()
    text = text.replace("trainerbattle_double TRAINER_TATE_AND_LIZA_1", "trainerbattle_single TRAINER_TATE_AND_LIZA_1", 1)
    text = text.replace("setflag FLAG_BADGE06_GET", "setflag FLAG_BADGE07_GET", 1)
    text = text.replace("setvar VAR_0x8008, 6", "setvar VAR_0x8008, 7", 1)
    text = text.replace("goto_if_set FLAG_BADGE06_GET, MossdeepCity_Gym_EventScript_GymStatueCertified", "goto_if_set FLAG_BADGE07_GET, MossdeepCity_Gym_EventScript_GymStatueCertified")
    text = replace_once(
        text,
        """MossdeepCity_Gym_Text_TateAndLizaIntro:
\t.string "BLUE: Five badges. Good.\\n"
\t.string "Then you get us at full speed.\\p"
\t.string "GREEN: The pressure stays exactly\\n"
\t.string "where you left it.\\p"
\t.string "BLUE: No hiding behind gimmicks.\\n"
\t.string "No soft turns.\\p"
\t.string "GREEN: Six against six.\\n"
\t.string "Bring a real team.$"

MossdeepCity_Gym_Text_TateAndLizaDefeat:
\t.string "BLUE: Hah. You held the line.\\p"
\t.string "GREEN: That was a real answer.\\n"
\t.string "Take this.$"

MossdeepCity_Gym_Text_ReceivedMindBadge:
\t.string "{PLAYER} received the MIND BADGE\\n"
\t.string "from BLUE and GREEN.$"

MossdeepCity_Gym_Text_ExplainMindBadgeTakeThis:
\t.string "BLUE: The MIND BADGE enhances the\\n"
\t.string "SP. ATK and SP. DEF of POKéMON.\\p"
\t.string "GREEN: It also lets you use the HM\\n"
\t.string "move DIVE outside of battle.\\p"
\t.string "BLUE: Take this too.$"

MossdeepCity_Gym_Text_ExplainCalmMind:
\t.string "GREEN: That TM04 contains\\n"
\t.string "CALM MIND.\\p"
\t.string "BLUE: It raises SP. ATK and\\n"
\t.string "SP. DEF in one turn.\\p"
\t.string "GREEN: Use it when you can force\\n"
\t.string "your opponent to respect the setup.\\p"
\t.string "… … … … … …$"

MossdeepCity_Gym_Text_RegisteredTateAndLiza:
\t.string "This number has been blocked.$"

MossdeepCity_Gym_Text_TateAndLizaPostBattle:
\t.string "BLUE: Keep that edge.\\n"
\t.string "The next challenge will punish autopilot.\\p"
\t.string "GREEN: Your team earned the badge.\\n"
\t.string "Now prove it travels.$"

MossdeepCity_Gym_Text_TateAndLizaNeedTwoMons:
\t.string "BLUE: You only have one POKéMON\\n"
\t.string "that can battle.\\p"
\t.string "GREEN: This is a two-on-two opener.\\n"
\t.string "Come back with more.$"

MossdeepCity_Gym_Text_GymStatue:
\t.string "MOSSDEEP CITY POKéMON GYM$"

MossdeepCity_Gym_Text_GymStatueCertified:
\t.string "MOSSDEEP CITY POKéMON GYM\\p"
\t.string "BLUE AND GREEN'S CERTIFIED TRAINERS:\\n"
\t.string "{PLAYER}$"

MossdeepCity_Gym_Text_TateAndLizaPreRematch:
\t.string "BLUE: You came back.\\n"
\t.string "Good. We tightened the rotation.\\p"
\t.string "GREEN: Same pressure, sharper answers.\\n"
\t.string "Let's see what changed.$"

MossdeepCity_Gym_Text_TateAndLizaRematchDefeat:
\t.string "BLUE: Still standing.\\p"
\t.string "GREEN: Still dangerous.$"

MossdeepCity_Gym_Text_TateAndLizaPostRematch:
\t.string "BLUE: A team that adapts wins again.\\p"
\t.string "GREEN: Keep adapting.$"

MossdeepCity_Gym_Text_TateAndLizaRematchNeedTwoMons:
\t.string "BLUE: A rematch still needs two\\n"
\t.string "battle-ready POKéMON.\\p"
\t.string "GREEN: Fix the roster and come back.$"
""",
        """MossdeepCity_Gym_Text_TateAndLizaIntro:
\t.string "ROXANNE: I study every layer of a\\n"
\t.string "battle before I make my first move.\\p"
\t.string "This sand will test whether your team\\n"
\t.string "can stand after the easy answers fail.\\p"
\t.string "Show me structure. Show me patience.\\n"
\t.string "Then show me you can break stone.$"

MossdeepCity_Gym_Text_TateAndLizaDefeat:
\t.string "ROXANNE: Excellent. You found the\\n"
\t.string "fault line and struck it cleanly.\\p"
\t.string "In recognition of your victory,\\n"
\t.string "take this.$"

MossdeepCity_Gym_Text_ReceivedMindBadge:
\t.string "{PLAYER} received the MIND BADGE\\n"
\t.string "from ROXANNE.$"

MossdeepCity_Gym_Text_ExplainMindBadgeTakeThis:
\t.string "ROXANNE: The MIND BADGE enhances\\n"
\t.string "SP. ATK and SP. DEF of POKéMON.\\p"
\t.string "It also lets you use the HM move\\n"
\t.string "DIVE outside of battle.\\p"
\t.string "Please take this too.$"

MossdeepCity_Gym_Text_ExplainCalmMind:
\t.string "ROXANNE: TM04 contains CALM MIND.\\p"
\t.string "It raises SP. ATK and SP. DEF.\\p"
\t.string "A patient setup can turn pressure\\n"
\t.string "into control.\\p"
\t.string "… … … … … …$"

MossdeepCity_Gym_Text_RegisteredTateAndLiza:
\t.string "This number has been blocked.$"

MossdeepCity_Gym_Text_TateAndLizaPostBattle:
\t.string "ROXANNE: Your team adapted under\\n"
\t.string "sand, hazards, and resistance.\\p"
\t.string "That is what a serious challenger\\n"
\t.string "must do.$"

MossdeepCity_Gym_Text_TateAndLizaNeedTwoMons:
\t.string "ROXANNE: You need more battle-ready\\n"
\t.string "POKéMON before this challenge.$"

MossdeepCity_Gym_Text_GymStatue:
\t.string "MOSSDEEP CITY POKéMON GYM$"

MossdeepCity_Gym_Text_GymStatueCertified:
\t.string "MOSSDEEP CITY POKéMON GYM\\p"
\t.string "ROXANNE'S CERTIFIED TRAINERS:\\n"
\t.string "{PLAYER}$"

MossdeepCity_Gym_Text_TateAndLizaPreRematch:
\t.string "ROXANNE: You returned. Good.\\n"
\t.string "The lesson deepens with repetition.$"

MossdeepCity_Gym_Text_TateAndLizaRematchDefeat:
\t.string "ROXANNE: Your answer remains sound.$"

MossdeepCity_Gym_Text_TateAndLizaPostRematch:
\t.string "ROXANNE: Refinement is its own\\n"
\t.string "kind of strength.$"

MossdeepCity_Gym_Text_TateAndLizaRematchNeedTwoMons:
\t.string "ROXANNE: Bring enough battle-ready\\n"
\t.string "POKéMON for a proper rematch.$"
""",
        scripts,
    )
    scripts.write_text(text)

    map_path = root / "data/maps/MossdeepCity_Gym/map.json"
    text = map_path.read_text()
    text = replace_once(
        text,
        """    {
      "graphics_id": "OBJ_EVENT_GFX_BLUE",
      "x": 5,
      "y": 2,
      "elevation": 0,
      "movement_type": "MOVEMENT_TYPE_FACE_DOWN",
      "movement_range_x": 0,
      "movement_range_y": 0,
      "trainer_type": "TRAINER_TYPE_NONE",
      "trainer_sight_or_berry_tree_id": "0",
      "script": "MossdeepCity_Gym_EventScript_TateAndLiza",
      "flag": "0"
    },
    {
      "graphics_id": "OBJ_EVENT_GFX_GREEN_NORMAL",
      "x": 6,
      "y": 2,
      "elevation": 0,
      "movement_type": "MOVEMENT_TYPE_FACE_DOWN",
      "movement_range_x": 0,
      "movement_range_y": 0,
      "trainer_type": "TRAINER_TYPE_NONE",
      "trainer_sight_or_berry_tree_id": "0",
      "script": "MossdeepCity_Gym_EventScript_TateAndLiza",
      "flag": "0"
    },
""",
        """    {
      "graphics_id": "OBJ_EVENT_GFX_ROXANNE",
      "x": 5,
      "y": 2,
      "elevation": 0,
      "movement_type": "MOVEMENT_TYPE_FACE_DOWN",
      "movement_range_x": 0,
      "movement_range_y": 0,
      "trainer_type": "TRAINER_TYPE_NONE",
      "trainer_sight_or_berry_tree_id": "0",
      "script": "MossdeepCity_Gym_EventScript_TateAndLiza",
      "flag": "0"
    },
""",
        map_path,
    )
    map_path.write_text(text)


def update_fortree(root):
    scripts = root / "data/maps/FortreeCity_Gym/scripts.inc"
    text = scripts.read_text()
    replacements = {
        "FORTREE GYM LEADER WINONA is\n": "FORTREE GYM LEADERS BLUE and\n",
        "a master of FLYING-type POKéMON.\\p": "GREEN fight as one sharp mind.\\p",
        "She's waiting at the back of this GYM,\n": "They are waiting at the back of this\n",
        "behind the rotating doors.\\p": "GYM, behind the rotating doors.\\p",
        "She's waiting for new challengers\n": "They punish careless turns with\n",
        "sorts of FLYING-type POKéMON.\\p": "Psychic Terrain and Trick Room.\\p",
        "You do know that, right?$": "You do know that, right?$",
        "I am WINONA. I am the LEADER of\n": "BLUE: I do not need a title to know\n",
        "the FORTREE POKéMON GYM.\\p": "when a challenger is hesitating.\\p",
        "I have become one with BIRD POKéMON\n": "GREEN: And I do not need permission\n",
        "and have soared the skies…\\p": "to tear that hesitation apart.\\p",
        "However grueling the battle, we have\n": "BLUE: You made it to the sixth badge.\n",
        "triumphed with grace…\\p": "Good. Now think faster than fear.\\p",
        "Witness the elegant choreography\n": "GREEN: Six on six. No excuses.\n",
        "of BIRD POKéMON and I!$": "Show us the team you trust.$",
        "Never before have I seen a TRAINER\n": "BLUE: You read the board cleanly.\n",
        "command POKéMON with more grace\\l": "That is rare.\\p",
        "than I…\\p": "GREEN: Rare does not mean safe.\n",
        "In recognition of your prowess,\n": "But it does mean you earned this.\n",
        "I present to you this GYM BADGE.$": "Take the badge.$",
        "{PLAYER} received the FEATHER BADGE\nfrom WINONA.$": "{PLAYER} received the FEATHER BADGE\nfrom BLUE and GREEN.$",
        "Though I fell to you, I will remain\ndevoted to BIRD POKéMON.$": "BLUE: Keep building. A strong team is\nnever finished.$",
        "WINONA'S CERTIFIED TRAINERS:\n": "BLUE AND GREEN'S CERTIFIED TRAINERS:\n",
        "WINONA: We humans can never escape\n": "BLUE: You came back. Good.\n",
        "gravity's pull on the ground.\\p": "A rematch should feel different.\\p",
        "But by striving for excellence,\n": "GREEN: We changed the angles.\n",
        "we have the power to soar!\\p": "You had better change your answers.\\p",
        "You have scaled the pinnacle as\n": "BLUE: Champions do not get comfort.\n",
        "the reigning CHAMPION.\\p": "They get examined.\\p",
        "Please, allow me to see your power\n": "GREEN: So move first in your head,\n",
        "at full flight!$": "or lose before the turn starts.$",
    }
    for old, new in replacements.items():
        text = text.replace(old, new)
    scripts.write_text(text)

    map_path = root / "data/maps/FortreeCity_Gym/map.json"
    text = map_path.read_text()
    text = replace_once(text, '"graphics_id": "OBJ_EVENT_GFX_WINONA"', '"graphics_id": "OBJ_EVENT_GFX_BLUE"', map_path)
    if '"graphics_id": "OBJ_EVENT_GFX_GREEN_NORMAL"' not in text:
        text = replace_once(
            text,
            """    {
      "graphics_id": "OBJ_EVENT_GFX_BLUE",
      "x": 23,
      "y": 7,
      "elevation": 3,
      "movement_type": "MOVEMENT_TYPE_FACE_DOWN",
      "movement_range_x": 0,
      "movement_range_y": 0,
      "trainer_type": "TRAINER_TYPE_NONE",
      "trainer_sight_or_berry_tree_id": "0",
      "script": "FortreeCity_Gym_EventScript_Winona",
      "flag": "0"
    },
""",
            """    {
      "graphics_id": "OBJ_EVENT_GFX_BLUE",
      "x": 23,
      "y": 7,
      "elevation": 3,
      "movement_type": "MOVEMENT_TYPE_FACE_DOWN",
      "movement_range_x": 0,
      "movement_range_y": 0,
      "trainer_type": "TRAINER_TYPE_NONE",
      "trainer_sight_or_berry_tree_id": "0",
      "script": "FortreeCity_Gym_EventScript_Winona",
      "flag": "0"
    },
    {
      "graphics_id": "OBJ_EVENT_GFX_GREEN_NORMAL",
      "x": 24,
      "y": 7,
      "elevation": 3,
      "movement_type": "MOVEMENT_TYPE_FACE_DOWN",
      "movement_range_x": 0,
      "movement_range_y": 0,
      "trainer_type": "TRAINER_TYPE_NONE",
      "trainer_sight_or_berry_tree_id": "0",
      "script": "FortreeCity_Gym_EventScript_Winona",
      "flag": "0"
    },
""",
            map_path,
        )
    map_path.write_text(text)


def update_swapped_gym_identity_shells(root):
    map_swaps = {
        "RustboroCity_Gym": ("OBJ_EVENT_GFX_ROXANNE", "OBJ_EVENT_GFX_JUAN"),
        "DewfordTown_Gym": ("OBJ_EVENT_GFX_BRAWLY", "OBJ_EVENT_GFX_FLANNERY"),
        "MauvilleCity_Gym": ("OBJ_EVENT_GFX_WATTSON", "OBJ_EVENT_GFX_BRAWLY"),
        "LavaridgeTown_Gym_1F": ("OBJ_EVENT_GFX_FLANNERY", "OBJ_EVENT_GFX_WINONA"),
        "SootopolisCity_Gym_1F": ("OBJ_EVENT_GFX_JUAN", "OBJ_EVENT_GFX_WATTSON"),
    }
    for map_name, (old, new) in map_swaps.items():
        path = root / f"data/maps/{map_name}/map.json"
        text = path.read_text()
        text = text.replace(f'"graphics_id": "{old}"', f'"graphics_id": "{new}"', 1)
        path.write_text(text)

    petalburg = root / "data/maps/PetalburgCity_Gym/map.json"
    text = petalburg.read_text()
    text = text.replace('"graphics_id": "OBJ_EVENT_GFX_NORMAN"', '"graphics_id": "OBJ_EVENT_GFX_RED"', 1)
    petalburg.write_text(text)

    text_swaps = {
        "RustboroCity_Gym/scripts.inc": [
            ("ROXANNE", "JUAN"),
            ("STONE BADGE", "STONE BADGE"),
        ],
        "DewfordTown_Gym/scripts.inc": [
            ("BRAWLY", "FLANNERY"),
            ("KNUCKLE BADGE", "KNUCKLE BADGE"),
        ],
        "MauvilleCity_Gym/scripts.inc": [
            ("WATTSON", "BRAWLY"),
            ("DYNAMO BADGE", "DYNAMO BADGE"),
        ],
        "LavaridgeTown_Gym_1F/scripts.inc": [
            ("FLANNERY", "WINONA"),
            ("HEAT BADGE", "HEAT BADGE"),
        ],
        "PetalburgCity_Gym/scripts.inc": [
            ("NORMAN", "RED"),
            ("BALANCE BADGE", "BALANCE BADGE"),
        ],
        "SootopolisCity_Gym_1F/scripts.inc": [
            ("JUAN", "WATTSON"),
            ("RAIN BADGE", "RAIN BADGE"),
        ],
    }
    for rel, swaps in text_swaps.items():
        path = root / f"data/maps/{rel}"
        text = path.read_text()
        for old, new in swaps:
            text = text.replace(old, new)
        path.write_text(text)


for root in ROOTS:
    update_trainers(root)
    update_mossdeep(root)
    update_fortree(root)
    update_swapped_gym_identity_shells(root)

print("Updated gym identities using BaneMode_Boss_Team_Atlas.md as source of truth.")
