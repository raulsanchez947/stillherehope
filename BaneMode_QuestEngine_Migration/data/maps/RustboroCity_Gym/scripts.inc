RustboroCity_Gym_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, RustboroCity_Gym_OnFrame
	map_script MAP_SCRIPT_ON_RESUME, RustboroCity_Gym_OnResume
	map_script MAP_SCRIPT_ON_LOAD, RustboroCity_Gym_OnLoad
	map_script MAP_SCRIPT_ON_TRANSITION, RustboroCity_Gym_OnTransition
	.byte 0

RustboroCity_Gym_OnTransition:
	end

RustboroCity_Gym_OnResume:
	end

RustboroCity_Gym_OnLoad:
	setvar VAR_ICE_STEP_COUNT, 67
	setmetatile 8, 4, METATILE_SootopolisGym_Stairs, FALSE
	setmetatile 8, 5, METATILE_SootopolisGym_Stairs, FALSE
	setmetatile 8, 10, METATILE_SootopolisGym_Stairs, FALSE
	setmetatile 8, 11, METATILE_SootopolisGym_Stairs, FALSE
	setmetatile 8, 15, METATILE_SootopolisGym_Stairs, FALSE
	setmetatile 8, 16, METATILE_SootopolisGym_Stairs, FALSE
	special DrawWholeMapView
	end

RustboroCity_Gym_EventScript_CheckSetStairMetatiles::
	goto_if_lt VAR_ICE_STEP_COUNT, 8, RustboroCity_Gym_EventScript_StopCheckingStairs
	goto_if_lt VAR_ICE_STEP_COUNT, 28, RustboroCity_Gym_EventScript_OpenFirstStairs
	goto_if_lt VAR_ICE_STEP_COUNT, 67, RustboroCity_Gym_EventScript_OpenFirstAndSecondStairs
	setmetatile 8, 4, METATILE_SootopolisGym_Stairs, FALSE
	setmetatile 8, 5, METATILE_SootopolisGym_Stairs, FALSE
RustboroCity_Gym_EventScript_OpenFirstAndSecondStairs::
	setmetatile 8, 10, METATILE_SootopolisGym_Stairs, FALSE
	setmetatile 8, 11, METATILE_SootopolisGym_Stairs, FALSE
RustboroCity_Gym_EventScript_OpenFirstStairs::
	setmetatile 8, 15, METATILE_SootopolisGym_Stairs, FALSE
	setmetatile 8, 16, METATILE_SootopolisGym_Stairs, FALSE
RustboroCity_Gym_EventScript_StopCheckingStairs::
	return

RustboroCity_Gym_OnFrame:
	.2byte 0

RustboroCity_Gym_EventScript_UnlockFirstStairs::
	addvar VAR_ICE_STEP_COUNT, 1
	delay 40
	playse SE_ICE_STAIRS
	call RustboroCity_Gym_EventScript_CheckSetStairMetatiles
	special DrawWholeMapView
	end

RustboroCity_Gym_EventScript_UnlockSecondStairs::
	addvar VAR_ICE_STEP_COUNT, 1
	delay 40
	playse SE_ICE_STAIRS
	call RustboroCity_Gym_EventScript_CheckSetStairMetatiles
	special DrawWholeMapView
	end

RustboroCity_Gym_EventScript_UnlockThirdStairs::
	addvar VAR_ICE_STEP_COUNT, 1
	delay 40
	playse SE_ICE_STAIRS
	call RustboroCity_Gym_EventScript_CheckSetStairMetatiles
	special DrawWholeMapView
	end

RustboroCity_Gym_EventScript_FallThroughIce::
	lockall
	delay 20
	applymovement LOCALID_PLAYER, RustboroCity_Gym_Movement_FallThroughIce
	waitmovement 0
	playse SE_FALL
	delay 60
	warp MAP_RUSTBORO_CITY_GYM, 8, 24
	waitstate
	end

RustboroCity_Gym_Movement_FallThroughIce:
	set_invisible
	step_end

RustboroCity_Gym_EventScript_Roxanne::
	trainerbattle_single TRAINER_JUAN_1, RustboroCity_Gym_Text_RoxanneIntro, RustboroCity_Gym_Text_RoxanneDefeat, RustboroCity_Gym_EventScript_RoxanneDefeated, NO_MUSIC
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, RustboroCity_Gym_EventScript_RoxanneRematch
	goto_if_unset FLAG_RECEIVED_TM03, RustboroCity_Gym_EventScript_GiveWaterPulse
	msgbox RustboroCity_Gym_Text_RoxannePostBattle, MSGBOX_DEFAULT
	release
	end

RustboroCity_Gym_EventScript_RoxanneDefeated::
	message RustboroCity_Gym_Text_ReceivedStoneBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox RustboroCity_Gym_Text_StoneBadgeInfoTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_RUSTBORO_GYM
	setflag FLAG_BADGE01_GET
	setvar VAR_RUSTBORO_CITY_STATE, 1
	addvar VAR_PETALBURG_GYM_STATE, 1
	setvar VAR_0x8008, 1
	call Common_EventScript_SetGymTrainers
	call_if_eq VAR_PETALBURG_GYM_STATE, 6, Common_EventScript_ReadyPetalburgGymForBattle
	call_if_set FLAG_OPEN_WORLD, EventScript_OPRustboroGym
	special BadgeCount
	addrankpoints 5
	goto RustboroCity_Gym_EventScript_GiveWaterPulse
	end

EventScript_OPRustboroGym::
	call_if_unset FLAG_BADGE05_GET, Common_EventScript_ReadyPetalburgGymForBattleOP
 	setvar VAR_RUSTBORO_CITY_STATE, 8
	return

RustboroCity_Gym_EventScript_GiveWaterPulse::
	giveitem ITEM_CHOICE_SCARF
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_TM03
	msgbox RustboroCity_Gym_Text_ExplainWaterPulse, MSGBOX_DEFAULT
	release
	end

RustboroCity_Gym_EventScript_RoxanneRematch::
	trainerbattle_rematch_double TRAINER_JUAN_1, RustboroCity_Gym_Text_RoxannePreRematch, RustboroCity_Gym_Text_RoxanneRematchDefeat, RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons
	msgbox RustboroCity_Gym_Text_RoxannePostRematch, MSGBOX_AUTOCLOSE
	end

RustboroCity_Gym_EventScript_Josh::
	trainerbattle_single TRAINER_JOSH, RustboroCity_Gym_Text_JoshIntro, RustboroCity_Gym_Text_JoshDefeat
	msgbox RustboroCity_Gym_Text_JoshPostBattle, MSGBOX_AUTOCLOSE
	end

RustboroCity_Gym_EventScript_Tommy::
	trainerbattle_single TRAINER_TOMMY, RustboroCity_Gym_Text_TommyIntro, RustboroCity_Gym_Text_TommyDefeat
	msgbox RustboroCity_Gym_Text_TommyPostBattle, MSGBOX_AUTOCLOSE
	end

RustboroCity_Gym_EventScript_Marc::
	trainerbattle_single TRAINER_MARC, RustboroCity_Gym_Text_MarcIntro, RustboroCity_Gym_Text_MarcDefeat
	msgbox RustboroCity_Gym_Text_MarcPostBattle, MSGBOX_AUTOCLOSE
	end

RustboroCity_Gym_EventScript_GymGuide::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_RUSTBORO_GYM, RustboroCity_Gym_EventScript_GymGuidePostVictory
	msgbox RustboroCity_Gym_Text_GymGuideAdvice, MSGBOX_DEFAULT
	release
	end

RustboroCity_Gym_EventScript_GymGuidePostVictory::
	msgbox RustboroCity_Gym_Text_GymGuidePostVictory, MSGBOX_DEFAULT
	release
	end

RustboroCity_Gym_EventScript_LeftGymStatue::
	lockall
	goto_if_set FLAG_BADGE01_GET, RustboroCity_Gym_EventScript_GymStatueCertified
	goto RustboroCity_Gym_EventScript_GymStatue
	end

RustboroCity_Gym_EventScript_RightGymStatue::
	lockall
	goto_if_set FLAG_BADGE01_GET, RustboroCity_Gym_EventScript_GymStatueCertified
	goto RustboroCity_Gym_EventScript_GymStatue
	end

RustboroCity_Gym_EventScript_GymStatueCertified::
	msgbox RustboroCity_Gym_Text_GymStatueCertified, MSGBOX_DEFAULT
	releaseall
	end

RustboroCity_Gym_EventScript_GymStatue::
	msgbox RustboroCity_Gym_Text_GymStatue, MSGBOX_DEFAULT
	releaseall
	end

RustboroCity_Gym_EventScript_RegisterRoxanne::
	lockall
	pokenavcall RustboroCity_Gym_Text_RoxanneRegisterCall
	waitmessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox RustboroCity_Gym_Text_RegisteredRoxanne, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_JUAN_MATCH_CALL
	clearflag FLAG_ENABLE_ROXANNE_FIRST_CALL
	releaseall
	end

RustboroCity_Gym_Text_GymGuideAdvice:
	.string "Yo, how's it going?\p"
	.string "Listen, my friend!\n"
	.string "Would you like to become the CHAMPION?\p"
	.string "I'm no TRAINER, not me, but I can\n"
	.string "sure give you winning advice.\p"
	.string "That's settled, then! We'll aim for the\n"
	.string "POKéMON CHAMPIONSHIP together!\p"
	.string "It's your job as a TRAINER to collect\n"
	.string "GYM BADGES, am I right?\p"
	.string "But GYM LEADERS aren't pushovers!\p"
	.string "And that's where I come in!\n"
	.string "I'm here to provide expert advice!\p"
	.string "JUAN, the GYM LEADER, is a user\n"
	.string "of WATER-type POKéMON.\p"
	.string "Water teams can drown you in tempo,\n"
	.string "so bring strong ELECTRIC- or\l"
	.string "GRASS-type answers.\p"
	.string "Come see me afterwards, if you beat\n"
	.string "the GYM LEADER.\p"
	.string "Well, go for it!$"

RustboroCity_Gym_Text_GymGuidePostVictory:
	.string "Whoa! What a breathtaking victory!\n"
	.string "My cheering must've worked!\p"
	.string "Great!\n"
	.string "Check your TRAINER CARD.\p"
	.string "The RAIN BADGE you got should be\n"
	.string "properly recorded on it.\p"
	.string "In other words…\p"
	.string "You've climbed the first step on\n"
	.string "the stairs to the CHAMPIONSHIP!\p"
	.string "That's got to feel awesome!$"

RustboroCity_Gym_Text_JoshIntro:
	.string "Don't take us GYM TRAINERS lightly!\n"
	.string "I'll show you why we're better!$"

RustboroCity_Gym_Text_JoshDefeat:
	.string "You were too good for me…$"

RustboroCity_Gym_Text_JoshPostBattle:
	.string "You haven't seen how hard JUAN can\n"
	.string "push a WATER team yet!$"

RustboroCity_Gym_Text_TommyIntro:
	.string "If you can't beat me, you won't stand\n"
	.string "a chance against JUAN!$"

RustboroCity_Gym_Text_TommyDefeat:
	.string "Wow! You've got some potential!$"

RustboroCity_Gym_Text_TommyPostBattle:
	.string "JUAN is stronger than me by\n"
	.string "several times.\p"
	.string "You'd better be on your guard!$"

RustboroCity_Gym_Text_MarcIntro:
	.string "We might be in the middle of town,\n"
	.string "but so what?\p"
	.string "My WATER POKéMON need room to\n"
	.string "make waves!$"

RustboroCity_Gym_Text_MarcDefeat:
	.string "Oh, man oh man!\n"
	.string "Our challenger is one feisty customer!$"

RustboroCity_Gym_Text_MarcPostBattle:
	.string "I have to hand it to our LEADER.\p"
	.string "JUAN can turn a calm board into a\n"
	.string "flood in a heartbeat.$"

RustboroCity_Gym_Text_RoxanneIntro:
	.string "JUAN: Aight, what's good, kid?\p"
	.string "You stepped in my house with those\n"
	.string "Buttas laced, so don't freeze now.\p"
	.string "I run this room with WATER pressure,\n"
	.string "clean lines, and no panic.\p"
	.string "Show me if your team can stay solid\n"
	.string "once the whole block gets flooded.$"

RustboroCity_Gym_Text_RoxanneDefeat:
	.string "JUAN: Sheesh…\n"
	.string "You deadass washed me.\p"
	.string "No fake respect either, that was\n"
	.string "clean pressure from jump.\p"
	.string "League rules are League rules.\n"
	.string "Take the official RAIN BADGE.$"

RustboroCity_Gym_Text_ReceivedStoneBadge:
	.string "{PLAYER} received the RAIN BADGE\n"
	.string "from JUAN.$"

RustboroCity_Gym_Text_StoneBadgeInfoTakeThis:
	.string "The RAIN BADGE heightens the ATTACK\n"
	.string "power of your POKéMON.\p"
	.string "It also enables them to use the HM move\n"
	.string "CUT outside of battle.\p"
	.string "Please take this with you, too.$"

RustboroCity_Gym_Text_ExplainWaterPulse:
	.string "JUAN: Take this CHOICE SCARF.\p"
	.string "Water battles are about tempo, mijo.\n"
	.string "Move first, control the whole block.$"

RustboroCity_Gym_Text_RoxannePostBattle:
	.string "JUAN: Keep climbing.\p"
	.string "The next gyms get way grimier than\n"
	.string "this one, so stay ready.$"

RustboroCity_Gym_Text_GymStatue:
	.string "RUSTBORO CITY POKéMON GYM$"

RustboroCity_Gym_Text_GymStatueCertified:
	.string "RUSTBORO CITY POKéMON GYM\p"
	.string "JUAN'S CERTIFIED TRAINERS:\n"
	.string "{PLAYER}$"

RustboroCity_Gym_Text_RoxanneRegisterCall:
	.string "… … … … … …\n"
	.string "… … … … … Beep!\p"
	.string "JUAN: Oh, hello, {PLAYER}?\n"
	.string "This is JUAN of RUSTBORO CITY.\p"
	.string "I heard from BRAWLY that you're\n"
	.string "moving up through the ranks of\l"
	.string "TRAINERS at an incredible clip.\p"
	.string "Well, I'd rather not think that\n"
	.string "I can't beat you.\p"
	.string "I promise, I will shape up my GYM\n"
	.string "so that you may challenge us again.\p"
	.string "I hope you'll be looking forward to\n"
	.string "that occasion as much as me!\p"
	.string "… … … … … …\n"
	.string "… … … … … Click!$"

RustboroCity_Gym_Text_RegisteredRoxanne:
	.string "This number is blocked!$"

RustboroCity_Gym_Text_RoxannePreRematch:
	.string "JUAN: I'm so glad to see you again.\n"
	.string "I'm JUAN, the GYM LEADER here.\p"
	.string "I'm sure we've both experienced many\n"
	.string "battles since we last met.\p"
	.string "I would like to see how much better\n"
	.string "we've become. Let us battle!$"

RustboroCity_Gym_Text_RoxanneRematchDefeat:
	.string "Grr…\n"
	.string "Again, I have lost…$"

RustboroCity_Gym_Text_RoxannePostRematch:
	.string "JUAN: I still have much to learn\n"
	.string "when it comes to battling POKéMON.\p"
	.string "That awareness makes me love battling\n"
	.string "all that much more!$"

RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons:
	.string "JUAN: I'm so glad to see you again.\n"
	.string "I'm JUAN, the GYM LEADER here.\p"
	.string "I'm sure we've both experienced many\n"
	.string "battles since we last met.\p"
	.string "I would like to see how much better\n"
	.string "we've become…\p"
	.string "Oh, no…\n"
	.string "You only have one POKéMON with you?\p"
	.string "Please return with at least one more\n"
	.string "POKéMON.$"
