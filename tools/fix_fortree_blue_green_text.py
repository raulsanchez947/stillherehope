from pathlib import Path

ROOTS = [
    Path("/Users/ivan/Documents/BaneMode_Rocket_Porymap_Work/pokeemerald-rocket-porymap"),
    Path("/Users/ivan/Documents/BaneMode_Rocket_Porymap_Work"),
]

BLOCKS = {
    "FortreeCity_Gym_Text_GymGuideAdvice": """FortreeCity_Gym_Text_GymGuideAdvice:
\t.string "Yo, how's it going, CHAMPION-\\n"
\t.string "bound {PLAYER}?\\p"
\t.string "FORTREE's GYM LEADERS are BLUE\\n"
\t.string "and GREEN. They fight as one mind.\\p"
\t.string "They punish careless turns with\\n"
\t.string "Psychic Terrain and Trick Room.\\p"
\t.string "Read the board before it moves.\\n"
\t.string "Okay, go for it!$"
""",
    "FortreeCity_Gym_Text_FlintIntro": """FortreeCity_Gym_Text_FlintIntro:
\t.string "There's no need for BLUE and GREEN\\n"
\t.string "to deal with you yet!\\p"
\t.string "I'm plenty good enough for you!$"
""",
    "FortreeCity_Gym_Text_FlintDefeat": """FortreeCity_Gym_Text_FlintDefeat:
\t.string "BLUE, GREEN, I...\\n"
\t.string "I lost!$"
""",
    "FortreeCity_Gym_Text_FlintPostBattle": """FortreeCity_Gym_Text_FlintPostBattle:
\t.string "BLUE is sharp and GREEN is fearless.\\n"
\t.string "Together, they're terrifying.$"
""",
    "FortreeCity_Gym_Text_AshleyIntro": """FortreeCity_Gym_Text_AshleyIntro:
\t.string "BLUE and GREEN taught me personally!\\n"
\t.string "You can't beat me easily!$"
""",
    "FortreeCity_Gym_Text_AshleyPostBattle": """FortreeCity_Gym_Text_AshleyPostBattle:
\t.string "Thanks to BLUE and GREEN, FORTREE\\n"
\t.string "knows how to fight under pressure.$"
""",
    "FortreeCity_Gym_Text_HumbertoIntro": """FortreeCity_Gym_Text_HumbertoIntro:
\t.string "When BLUE calls the line, GREEN is\\n"
\t.string "already attacking the opening.\\p"
\t.string "I'm not letting you witness that\\n"
\t.string "kind of teamwork yet!$"
""",
    "FortreeCity_Gym_Text_HumbertoPostBattle": """FortreeCity_Gym_Text_HumbertoPostBattle:
\t.string "You'd better watch it!\\n"
\t.string "Don't lose track of either LEADER,\\l"
\t.string "or you'll crash before you move.$"
""",
    "FortreeCity_Gym_Text_DariusIntro": """FortreeCity_Gym_Text_DariusIntro:
\t.string "You'd better know how to fight\\n"
\t.string "through Psychic Terrain and Trick Room.\\p"
\t.string "You do know that, right?$"
""",
    "FortreeCity_Gym_Text_DariusPostBattle": """FortreeCity_Gym_Text_DariusPostBattle:
\t.string "Sure, you beat me all right.\\n"
\t.string "But you'd better watch it! Our LEADERS\\l"
\t.string "BLUE and GREEN are all business.$"
""",
    "FortreeCity_Gym_Text_WinonaIntro": """FortreeCity_Gym_Text_WinonaIntro:
\t.string "BLUE: You made it to the sixth badge.\\n"
\t.string "Good. That means you can be tested.\\p"
\t.string "GREEN: Tested? I'm going to break\\n"
\t.string "their rhythm before turn one.\\p"
\t.string "BLUE: Then they should think faster.\\n"
\t.string "A weak plan deserves to lose.\\p"
\t.string "GREEN: Six on six. No excuses.\\n"
\t.string "Show us the team you trust.$"
""",
    "FortreeCity_Gym_Text_WinonaDefeat": """FortreeCity_Gym_Text_WinonaDefeat:
\t.string "BLUE: You read the board cleanly.\\n"
\t.string "That is rare.\\p"
\t.string "GREEN: Rare doesn't mean safe.\\n"
\t.string "But it does mean you earned this.\\p"
\t.string "Take the badge.$"
""",
    "FortreeCity_Gym_Text_ReceivedFeatherBadge": """FortreeCity_Gym_Text_ReceivedFeatherBadge:
\t.string "{PLAYER} received the FEATHER BADGE\\n"
\t.string "from BLUE and GREEN.$"
""",
    "FortreeCity_Gym_Text_ExplainFeatherBadgeTakeThis": """FortreeCity_Gym_Text_ExplainFeatherBadgeTakeThis:
\t.string "BLUE: With the FEATHER BADGE, all\\n"
\t.string "POKéMON up to LV 70 obey you.\\p"
\t.string "GREEN: You can also use FLY outside\\n"
\t.string "of battle. Don't waste the opening.\\p"
\t.string "BLUE: And take this.$"
""",
    "FortreeCity_Gym_Text_ExplainAerialAce": """FortreeCity_Gym_Text_ExplainAerialAce:
\t.string "BLUE: TM40 contains AERIAL ACE.\\p"
\t.string "GREEN: A clean strike that doesn't\\n"
\t.string "miss. Simple. Useful. Mean.\\p"
\t.string "… … … … … …$"
""",
    "FortreeCity_Gym_Text_WinonaPostBattle": """FortreeCity_Gym_Text_WinonaPostBattle:
\t.string "BLUE: Keep building. A strong team is\\n"
\t.string "never finished.\\p"
\t.string "GREEN: And never boring. Remember that.$"
""",
    "FortreeCity_Gym_Text_GymStatueCertified": """FortreeCity_Gym_Text_GymStatueCertified:
\t.string "FORTREE CITY POKéMON GYM\\p"
\t.string "BLUE AND GREEN'S CERTIFIED TRAINERS:\\n"
\t.string "{PLAYER}$"
""",
    "FortreeCity_Gym_Text_WinonaPreRematch": """FortreeCity_Gym_Text_WinonaPreRematch:
\t.string "BLUE: You came back. Good.\\n"
\t.string "A rematch should feel different.\\p"
\t.string "GREEN: We changed the angles.\\n"
\t.string "You had better change your answers.\\p"
\t.string "BLUE: Champions do not get comfort.\\n"
\t.string "They get examined.\\p"
\t.string "GREEN: Move first in your head,\\n"
\t.string "or lose before the turn starts.$"
""",
    "FortreeCity_Gym_Text_WinonaRematchDefeat": """FortreeCity_Gym_Text_WinonaRematchDefeat:
\t.string "BLUE: Still reading correctly.\\p"
\t.string "GREEN: Still annoying to knock down.$"
""",
    "FortreeCity_Gym_Text_WinonaPostRematch": """FortreeCity_Gym_Text_WinonaPostRematch:
\t.string "BLUE: Repetition sharpened you.\\p"
\t.string "GREEN: Good. Stay sharp.$"
""",
    "FortreeCity_Gym_Text_WinonaRematchNeedTwoMons": """FortreeCity_Gym_Text_WinonaRematchNeedTwoMons:
\t.string "BLUE: This battle opens two-on-two.\\n"
\t.string "Bring at least two battle-ready\\l"
\t.string "POKéMON.\\p"
\t.string "GREEN: Then we can start properly.$"
""",
}


def replace_label_block(text, label, replacement):
    start = text.index(label + ":")
    next_pos = len(text)
    marker = "\nFortreeCity_Gym_"
    found = text.find(marker, start + 1)
    if found != -1:
        next_pos = found + 1
    return text[:start] + replacement + text[next_pos:]


for root in ROOTS:
    path = root / "data/maps/FortreeCity_Gym/scripts.inc"
    text = path.read_text()
    text = text.replace(
        "trainerbattle_single TRAINER_WINONA_1, FortreeCity_Gym_Text_WinonaIntro, FortreeCity_Gym_Text_WinonaDefeat, FortreeCity_Gym_EventScript_WinonaDefeated, NO_MUSIC",
        "trainerbattle_double TRAINER_WINONA_1, FortreeCity_Gym_Text_WinonaIntro, FortreeCity_Gym_Text_WinonaDefeat, FortreeCity_Gym_Text_WinonaRematchNeedTwoMons, FortreeCity_Gym_EventScript_WinonaDefeated, NO_MUSIC",
        1,
    )
    for label, replacement in BLOCKS.items():
        text = replace_label_block(text, label, replacement)
    path.write_text(text)

print("Rewrote Fortree Blue/Green dialogue and double-battle entry.")
