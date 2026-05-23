SootopolisCity_Gym_1F_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, SootopolisCity_Gym_1F_OnLoad
	.byte 0

SootopolisCity_Gym_1F_OnLoad:
	goto_if_set FLAG_DEFEATED_SOOTOPOLIS_GYM, SootopolisCity_Gym_1F_EventScript_DeactivatePuzzle
	end

SootopolisCity_Gym_1F_EventScript_DeactivatePuzzle::
	special MauvilleGymDeactivatePuzzle
	end

SootopolisCity_Gym_1F_EventScript_Switch1::
	setvar VAR_0x8004, 0
	special MauvilleGymPressSwitch
	end

SootopolisCity_Gym_1F_EventScript_Switch2::
	setvar VAR_0x8004, 1
	special MauvilleGymPressSwitch
	end

SootopolisCity_Gym_1F_EventScript_Switch3::
	setvar VAR_0x8004, 2
	special MauvilleGymPressSwitch
	end

SootopolisCity_Gym_1F_EventScript_Switch4::
	setvar VAR_0x8004, 3
	special MauvilleGymPressSwitch
	end

SootopolisCity_Gym_1F_EventScript_Wattson::
	trainerbattle_single TRAINER_WATTSON_1, SootopolisCity_Gym_1F_Text_WattsonIntro, SootopolisCity_Gym_1F_Text_WattsonDefeat, SootopolisCity_Gym_1F_EventScript_WattsonDefeated, NO_MUSIC
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, SootopolisCity_Gym_1F_EventScript_WattsonRematch
	goto_if_unset FLAG_RECEIVED_TM34, SootopolisCity_Gym_1F_EventScript_GiveWaterPulse2
	goto_if_unset FLAG_BADGE06_GET, SootopolisCity_Gym_1F_EventScript_GoGetFortreeBadge
	msgbox SootopolisCity_Gym_1F_Text_WattsonPostBattle, MSGBOX_DEFAULT
	release
	end

SootopolisCity_Gym_1F_EventScript_WattsonDefeated::
	message SootopolisCity_Gym_1F_Text_ReceivedRainBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox SootopolisCity_Gym_1F_Text_ExplainRainBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_SOOTOPOLIS_GYM
	setflag FLAG_BADGE08_GET
	setflag FLAG_HIDE_SOOTOPOLIS_CITY_RESIDENTS
	setflag FLAG_HIDE_SOOTOPOLIS_CITY_STEVEN
	setflag FLAG_HIDE_SOOTOPOLIS_CITY_WALLACE
	setvar VAR_SOOTOPOLIS_CITY_STATE, 6
	clearflag FLAG_HIDE_SOOTOPOLIS_CITY_MAN_1
	setvar VAR_0x8008, 8
	call Common_EventScript_SetGymTrainers
	call SootopolisCity_Gym_1F_EventScript_GiveWaterPulse
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox SootopolisCity_Gym_1F_Text_RegisteredWattson, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_WATTSON_MATCH_CALL
	special BadgeCount
	addrankpoints 5
	release
	end

SootopolisCity_Gym_1F_EventScript_GiveWaterPulse::
	giveitem ITEM_TERRAIN_EXTENDER
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_BagIsFull
	msgbox SootopolisCity_Gym_1F_Text_ExplainWaterPulse, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM34
	return

SootopolisCity_Gym_1F_EventScript_GiveWaterPulse2::
	giveitem ITEM_TERRAIN_EXTENDER
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	msgbox SootopolisCity_Gym_1F_Text_ExplainWaterPulse, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM34
	release
	end

SootopolisCity_Gym_1F_EventScript_GoGetFortreeBadge::
	msgbox SootopolisCity_Gym_1F_Text_GoGetFortreeBadge, MSGBOX_DEFAULT
	release
	end

SootopolisCity_Gym_1F_EventScript_WattsonRematch::
	trainerbattle_rematch_double TRAINER_WATTSON_1, SootopolisCity_Gym_1F_Text_WattsonPreRematch, SootopolisCity_Gym_1F_Text_WattsonRematchDefeat, SootopolisCity_Gym_1F_Text_WattsonRematchNeedTwoMons
	msgbox SootopolisCity_Gym_1F_Text_WattsonPostRematch, MSGBOX_AUTOCLOSE
	end

SootopolisCity_Gym_1F_EventScript_GymGuide::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_SOOTOPOLIS_GYM, SootopolisCity_Gym_1F_EventScript_GymGuidePostVictory
	msgbox SootopolisCity_Gym_1F_Text_GymGuideAdvice, MSGBOX_DEFAULT
	release
	end

SootopolisCity_Gym_1F_EventScript_GymGuidePostVictory::
	msgbox SootopolisCity_Gym_1F_Text_GymGuidePostVictory, MSGBOX_DEFAULT
	release
	end

SootopolisCity_Gym_1F_EventScript_LeftGymStatue::
	lockall
	goto_if_set FLAG_BADGE08_GET, SootopolisCity_Gym_1F_EventScript_GymStatueCertified
	goto SootopolisCity_Gym_1F_EventScript_GymStatue
	end

SootopolisCity_Gym_1F_EventScript_RightGymStatue::
	lockall
	goto_if_set FLAG_BADGE08_GET, SootopolisCity_Gym_1F_EventScript_GymStatueCertified
	goto SootopolisCity_Gym_1F_EventScript_GymStatue
	end

SootopolisCity_Gym_1F_EventScript_GymStatueCertified::
	msgbox SootopolisCity_Gym_1F_Text_GymStatueCertified, MSGBOX_DEFAULT
	releaseall
	end

SootopolisCity_Gym_1F_EventScript_GymStatue::
	msgbox SootopolisCity_Gym_1F_Text_GymStatue, MSGBOX_DEFAULT
	releaseall
	end

SootopolisCity_Gym_1F_Text_GymGuideAdvice:
	.string "Yo! How's it going, CHAMPION-\n"
	.string "bound {PLAYER}?\p"
	.string "SOOTOPOLIS's GYM LEADER WATTSON is\n"
	.string "a master of ELECTRIC-type POKéMON.\p"
	.string "And to get to WATTSON, an icy floor\n"
	.string "will throw your footing off while his\l"
	.string "field crackles the whole time.\p"
	.string "Listen, I'm sorry, but that's all the\n"
	.string "advice that I have for you.\p"
	.string "The rest of the way, you have to\n"
	.string "go for it yourself!$"

SootopolisCity_Gym_1F_Text_GymGuidePostVictory:
	.string "Yow! You've beaten even WATTSON, who\n"
	.string "was supposedly the best in all HOENN!\p"
	.string "Okay! Check out your TRAINER CARD.\p"
	.string "If you've gotten all the BADGES, you're\n"
	.string "set for the POKéMON LEAGUE challenge!$"

SootopolisCity_Gym_1F_Text_WattsonIntro:
	.string "WATTSON: Ah-ha! You made it, my guy!\p"
	.string "The floor's slick, the current's live,\n"
	.string "and one bad step gets you cooked.\p"
	.string "You crossed the whole grid.\n"
	.string "Now survive the charge.$"

@ NOTE: This defeat text actually causes a buffer overflow. It's about 50 bytes too long for
@ the gDisplayedStringBattle buffer that it's put into, and it stomps all over the gBattleTextBuffs
@ after, as well as the otherwise unused array after that. One wonders if that's the reason for
@ the existence of that unused array of ints.
SootopolisCity_Gym_1F_Text_WattsonDefeat:
	.string "Wahahaha!\n"
	.string "Now that was a proper surge!\p"
	.string "You kept your line through the whole\n"
	.string "grid. Take this BADGE!$"

SootopolisCity_Gym_1F_Text_ReceivedRainBadge:
	.string "{PLAYER} received the DYNAMO BADGE\n"
	.string "from WATTSON.$"

SootopolisCity_Gym_1F_Text_ExplainRainBadgeTakeThis:
	.string "Having the DYNAMO BADGE shall assure you\n"
	.string "the full obedience of all your POKéMON\l"
	.string "to your every command.\p"
	.string "The DYNAMO BADGE also allows the use\n"
	.string "of the HM move WATERFALL to scale\l"
	.string "walls of cascading water.\p"
	.string "And, so that you never forget the\n"
	.string "battle we shared, take this…$"

SootopolisCity_Gym_1F_Text_ExplainWaterPulse:
	.string "That TERRAIN EXTENDER keeps the field\n"
	.string "charged longer.\p"
	.string "Electric pressure is the whole room\n"
	.string "buzzing before the first hit lands.$"

SootopolisCity_Gym_1F_Text_RegisteredWattson:
	.string "This number is blocked!$"

SootopolisCity_Gym_1F_Text_WattsonPostBattle:
	.string "WATTSON: That's all eight, then.\p"
	.string "Take those Buttas east to EVER GRANDE\n"
	.string "and see if the League can really hold you.$"

SootopolisCity_Gym_1F_Text_GoGetFortreeBadge:
	.string "There remains but one BADGE to\n"
	.string "obtain in HOENN.\p"
	.string "If you wish to challenge the POKéMON\n"
	.string "LEAGUE, you must obtain the last\l"
	.string "BADGE from the GYM in FORTREE.$"

SootopolisCity_Gym_1F_Text_GymStatue:
	.string "SOOTOPOLIS CITY POKéMON GYM$"

SootopolisCity_Gym_1F_Text_GymStatueCertified:
	.string "SOOTOPOLIS CITY POKéMON GYM\p"
	.string "WATTSON'S CERTIFIED TRAINERS:\n"
	.string "{PLAYER}$"

SootopolisCity_Gym_1F_Text_WattsonPreRematch:
	.string "WATTSON: Hah! Back for another surge?\n"
	.string "Good, good!\p"
	.string "Let's light the floor up again and see\n"
	.string "if your decisions are still faster than mine!$"

SootopolisCity_Gym_1F_Text_WattsonRematchDefeat:
	.string "Wahahaha!\n"
	.string "You outpaced the whole grid again!$"

SootopolisCity_Gym_1F_Text_WattsonPostRematch:
	.string "WATTSON: Wahahaha! Still sharp!\p"
	.string "Come back anytime you want another\n"
	.string "high-voltage lesson.\p"
	.string "A trainer who can think under pressure\n"
	.string "is a trainer worth watching!$"

SootopolisCity_Gym_1F_Text_WattsonRematchNeedTwoMons:
	.string "WATTSON: Hold it right there!\p"
	.string "If you want another proper match,\n"
	.string "bring enough battle-ready POKéMON.\p"
	.string "You've only got one with you.\n"
	.string "Come back with two or more, and then\l"
	.string "we'll start the current flowing!$"
