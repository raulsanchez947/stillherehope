.set LOCALID_WALLACE, 1
.set LOCALID_RIVAL, 2
.set LOCALID_BIRCH, 3

EverGrandeCity_ChampionsRoom_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, EverGrandeCity_ChampionsRoom_OnTransition
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, EverGrandeCity_ChampionsRoom_OnWarp
	map_script MAP_SCRIPT_ON_FRAME_TABLE, EverGrandeCity_ChampionsRoom_OnFrame
	.byte 0

EverGrandeCity_ChampionsRoom_OnTransition:
	call Eventscript_ChampionOPW
	call Common_EventScript_SetupRivalGfxId
	end

Eventscript_ChampionOPW::
setvar VAR_OBJ_GFX_ID_1, OBJ_EVENT_GFX_BRENDAN_NORMAL
return

Eventscript_ChangeChampion::
setvar VAR_OBJ_GFX_ID_1, OBJ_EVENT_GFX_BRENDAN_NORMAL
return

EverGrandeCity_ChampionsRoom_OnWarp:
	map_script_2 VAR_TEMP_1, 0, EverGrandeCity_ChampionsRoom_EventScript_PlayerTurnNorth
	.2byte 0

EverGrandeCity_ChampionsRoom_EventScript_PlayerTurnNorth::
	turnobject OBJ_EVENT_ID_PLAYER, DIR_NORTH
	end

EverGrandeCity_ChampionsRoom_OnFrame:
	map_script_2 VAR_TEMP_1, 0, EverGrandeCity_ChampionsRoom_EventScript_EnterRoom
	.2byte 0

EverGrandeCity_ChampionsRoom_EventScript_EnterRoom::
	lockall
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkUp4
	waitmovement 0
	delay 40
	applymovement OBJ_EVENT_ID_PLAYER, EverGrandeCity_ChampionsRoom_Movement_PlayerApproachWallace
	waitmovement 0
	setvar VAR_TEMP_1, 1
	goto EverGrandeCity_ChampionsRoom_EventScript_Wallace
	releaseall
	end

EverGrandeCity_ChampionsRoom_Movement_PlayerApproachWallace:
	walk_up
	walk_up
	step_end

EverGrandeCity_ChampionsRoom_EventScript_Wallace::
	@ Already beaten the champion - rematch speech
	goto_if_set FLAG_OPEN_WORLD, EverGrandeCity_ChampionsRoom_EventScript_RematchSpeech
	@ Gate Battle 1: Brendan + Bane combined squad
	playbgm MUS_ENCOUNTER_CHAMPION, FALSE
	msgbox EverGrandeCity_ChampionsRoom_Text_IntroSpeech, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_WALLACE, EverGrandeCity_ChampionsRoom_Text_Defeat
	addrankpoints 15
	@ Won Gate 1 - full heal before Gate 2
	msgbox EverGrandeCity_ChampionsRoom_Text_GateHealMessage, MSGBOX_DEFAULT
	special HealPlayerParty
	@ Gate Battle 2: Brendan + Bane legendary squad
	playbgm MUS_ENCOUNTER_CHAMPION, FALSE
	msgbox EverGrandeCity_ChampionsRoom_Text_IntroSpeechOPW, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_STEVEN, EverGrandeCity_ChampionsRoom_Text_DefeatOPW
	addrankpoints 15
	@ Won both gates - mark champion cleared
	setflag FLAG_OPEN_WORLD
	goto EverGrandeCity_ChampionsRoom_EventScript_Defeated
	end

EverGrandeCity_ChampionsRoom_EventScript_Steven::
	end

EverGrandeCity_ChampionsRoom_EventScript_RematchSpeech::
	msgbox EverGrandeCity_ChampionsRoom_Text_PostBattleSpeech, MSGBOX_DEFAULT
	releaseall
	end

EverGrandeCity_ChampionsRoom_EventScript_Defeated::
	playse SE_DOOR
	setmetatile 6, 1, METATILE_EliteFour_OpenDoorChampion_Frame, FALSE
	setmetatile 6, 2, METATILE_EliteFour_OpenDoorChampion_Opening, FALSE
	special DrawWholeMapView
	msgbox EverGrandeCity_ChampionsRoom_Text_PostBattleSpeech, MSGBOX_DEFAULT
	closemessage
	playse SE_DOOR
	checkplayergender
	call_if_eq VAR_RESULT, MALE, EverGrandeCity_ChampionsRoom_EventScript_PlayMayMusic
	call_if_eq VAR_RESULT, FEMALE, EverGrandeCity_ChampionsRoom_EventScript_PlayBrendanMusic
	addobject LOCALID_RIVAL
	call EverGrandeCity_ChampionsRoom_EventScript_RivalApproachPlayer
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, EverGrandeCity_ChampionsRoom_EventScript_MayAdvice
	goto_if_eq VAR_RESULT, FEMALE, EverGrandeCity_ChampionsRoom_EventScript_BrendanAdvice
	end

EverGrandeCity_ChampionsRoom_EventScript_DefeatedOPW::
	playse SE_DOOR
	setmetatile 6, 1, METATILE_EliteFour_OpenDoorChampion_Frame, FALSE
	setmetatile 6, 2, METATILE_EliteFour_OpenDoorChampion_Opening, FALSE
	special DrawWholeMapView
	msgbox EverGrandeCity_ChampionsRoom_Text_PostBattleSpeechOPW, MSGBOX_DEFAULT
	closemessage
	playse SE_DOOR
	checkplayergender
	call_if_eq VAR_RESULT, MALE, EverGrandeCity_ChampionsRoom_EventScript_PlayMayMusic
	call_if_eq VAR_RESULT, FEMALE, EverGrandeCity_ChampionsRoom_EventScript_PlayBrendanMusic
	addobject LOCALID_RIVAL
	call EverGrandeCity_ChampionsRoom_EventScript_RivalApproachPlayer
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, EverGrandeCity_ChampionsRoom_EventScript_MayAdvice
	goto_if_eq VAR_RESULT, FEMALE, EverGrandeCity_ChampionsRoom_EventScript_BrendanAdvice
	end

EverGrandeCity_ChampionsRoom_EventScript_PlayMayMusic::
	playbgm MUS_ENCOUNTER_MAY, FALSE
	return

EverGrandeCity_ChampionsRoom_EventScript_PlayBrendanMusic::
	playbgm MUS_ENCOUNTER_BRENDAN, FALSE
	return

EverGrandeCity_ChampionsRoom_EventScript_MayAdvice::
	msgbox EverGrandeCity_ChampionsRoom_Text_MayAdvice, MSGBOX_DEFAULT
	delay 40
	playse SE_PIN
	applymovement LOCALID_RIVAL, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_RIVAL, Common_Movement_Delay48
	waitmovement 0
	call EverGrandeCity_ChampionsRoom_EventScript_RivalLookBackAndForth
	msgbox EverGrandeCity_ChampionsRoom_Text_MayItsAlreadyOver, MSGBOX_DEFAULT
	goto EverGrandeCity_ChampionsRoom_EventScript_BirchArrivesExitForHoF
	end

EverGrandeCity_ChampionsRoom_EventScript_BrendanAdvice::
	msgbox EverGrandeCity_ChampionsRoom_Text_BrendanAdvice, MSGBOX_DEFAULT
	delay 40
	playse SE_PIN
	applymovement LOCALID_RIVAL, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_RIVAL, Common_Movement_Delay48
	waitmovement 0
	call EverGrandeCity_ChampionsRoom_EventScript_RivalLookBackAndForth
	msgbox EverGrandeCity_ChampionsRoom_Text_BrendanYouveWon, MSGBOX_DEFAULT
	goto EverGrandeCity_ChampionsRoom_EventScript_BirchArrivesExitForHoF
	end

EverGrandeCity_ChampionsRoom_EventScript_BirchArrivesExitForHoF::
	closemessage
	addobject LOCALID_BIRCH
	applymovement LOCALID_BIRCH, EverGrandeCity_ChampionsRoom_Movement_BirchArrives
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox EverGrandeCity_ChampionsRoom_Text_BirchArriveRatePokedex, MSGBOX_DEFAULT
	call ProfBirch_EventScript_RatePokedex
	msgbox EverGrandeCity_ChampionsRoom_Text_BirchCongratulations, MSGBOX_DEFAULT
	applymovement LOCALID_WALLACE, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	delay 20
	applymovement LOCALID_WALLACE, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	msgbox EverGrandeCity_ChampionsRoom_Text_WallaceComeWithMe, MSGBOX_DEFAULT
	closemessage
	delay 30
	applymovement LOCALID_WALLACE, EverGrandeCity_ChampionsRoom_Movement_WallaceExitStart
	applymovement OBJ_EVENT_ID_PLAYER, EverGrandeCity_ChampionsRoom_Movement_PlayerExitStart
	applymovement LOCALID_BIRCH, Common_Movement_WalkInPlaceFasterUp
	applymovement LOCALID_RIVAL, EverGrandeCity_ChampionsRoom_Movement_RivalFollows
	waitmovement 0
	delay 20
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	msgbox EverGrandeCity_ChampionsRoom_Text_WallaceWaitOutside, MSGBOX_DEFAULT
	checkplayergender
	call_if_eq VAR_RESULT, MALE, EverGrandeCity_ChampionsRoom_EventScript_MayCongratulations
	call_if_eq VAR_RESULT, FEMALE, EverGrandeCity_ChampionsRoom_EventScript_BrendanCongratulations
	closemessage
	applymovement LOCALID_WALLACE, EverGrandeCity_ChampionsRoom_Movement_WallaceExit
	applymovement OBJ_EVENT_ID_PLAYER, EverGrandeCity_ChampionsRoom_Movement_PlayerExit
	waitmovement 0
	setflag FLAG_HIDE_PETALBURG_GYM_GREETER
	warp MAP_EVER_GRANDE_CITY_HALL_OF_FAME, 7, 16
	waitstate
	releaseall
	end

EverGrandeCity_ChampionsRoom_EventScript_MayCongratulations::
	msgbox EverGrandeCity_ChampionsRoom_Text_MayCongratulations, MSGBOX_DEFAULT
	return

EverGrandeCity_ChampionsRoom_EventScript_BrendanCongratulations::
	msgbox EverGrandeCity_ChampionsRoom_Text_BrendanCongratulations, MSGBOX_DEFAULT
	return

EverGrandeCity_ChampionsRoom_EventScript_RivalApproachPlayer::
	applymovement LOCALID_RIVAL, EverGrandeCity_ChampionsRoom_Movement_RivalApproachPlayer
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	return

EverGrandeCity_ChampionsRoom_EventScript_RivalLookBackAndForth::
	applymovement LOCALID_RIVAL, EverGrandeCity_ChampionsRoom_Movement_RivalLookBackAndForth
	waitmovement 0
	return

EverGrandeCity_ChampionsRoom_Movement_PlayerExitStart:
	walk_up
	walk_up
	step_end

EverGrandeCity_ChampionsRoom_Movement_PlayerExit:
	walk_up
	delay_16
	walk_up
	delay_8
	set_invisible
	step_end

EverGrandeCity_ChampionsRoom_Movement_RivalApproachPlayer:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	walk_up
	walk_in_place_faster_right
	step_end

EverGrandeCity_ChampionsRoom_Movement_RivalLookBackAndForth:
	walk_in_place_faster_up
	delay_16
	walk_in_place_faster_right
	delay_16
	walk_in_place_faster_up
	delay_8
	walk_in_place_faster_right
	delay_16
	step_end

EverGrandeCity_ChampionsRoom_Movement_RivalFollows:
	walk_right
	walk_up
	step_end

EverGrandeCity_ChampionsRoom_Movement_WallaceExitStart:
	walk_up
	walk_up
	walk_in_place_faster_down
	step_end

EverGrandeCity_ChampionsRoom_Movement_WallaceExit:
	walk_up
	delay_8
	set_invisible
	step_end

EverGrandeCity_ChampionsRoom_Movement_BirchArrives:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	walk_up
	walk_in_place_faster_left
	step_end

EverGrandeCity_ChampionsRoom_Text_IntroSpeech:
	.string "BRENDAN: Yo, {PLAYER}{KUN}.\p"
	.string "Every battle you fought.\n"
	.string "Every gym you cleared.\p"
	.string "I was watching.\p"
	.string "I seen you at MAUVILLE.\n"
	.string "At MOSSDEEP. On ROUTE 131.\p"
	.string "While you were moving forward,\n"
	.string "I was moving quiet.\p"
	.string "TEAM ROCKET wanted DEOXYS\n"
	.string "as a weapon.\p"
	.string "I caught it first.\n"
	.string "word to my dead homie and my\l"
	.string "buttas, I'll keep it safe.\p"
	.string "But now? You made it.\n"
	.string "Four ELITE FOUR. No losses.\p"
	.string "Two gates. Twelve POKéMON.\n"
	.string "Me and BANE built for this moment.\p"
	.string "Now I need to see it for myself.\n"
	.string "Let's go!$"

EverGrandeCity_ChampionsRoom_Text_GateHealMessage:
	.string "BRENDAN: Aight. You cleared\n"
	.string "round one. Word.\p"
	.string "We're healing your squad.\n"
	.string "No cap - you earned that.\p"
	.string "But the real test is round two.\n"
	.string "Legendary tier. Brace up.$"

EverGrandeCity_ChampionsRoom_Text_IntroSpeechOPW::
	.string "BRENDAN: This is round two.\p"
	.string "ARTICUNO. MOLTRES. ZAPDOS.\n"
	.string "MEW. DEOXYS. MEWTWO.\p"
	.string "All legendary. No filler.\n"
	.string "Deadass strongest squad alive.\p"
	.string "You want CHAMPION status?\n"
	.string "Beat this. Final round!$"

EverGrandeCity_ChampionsRoom_Text_Defeat:
	.string "BRENDAN: …Word.\n"
	.string "You beat round one. Facts.$"

EverGrandeCity_ChampionsRoom_Text_DefeatOPW::
	.string "BRENDAN: …Deadass.\p"
	.string "Both squads. Twelve POKéMON.\n"
	.string "You cleared the gate. Facts.\p"
	.string "I watched your whole journey.\n"
	.string "Kept DEOXYS safe for HOENN.\p"
	.string "Coordinated with STEVEN and\n"
	.string "the HOENN DEFENDERS.\p"
	.string "Now that title is yours.\n"
	.string "No cap - you're the CHAMPION.\p"
	.string "HOENN is yours.$"

EverGrandeCity_ChampionsRoom_Text_PostBattleSpeech:
	.string "BRENDAN: You already cleared\n"
	.string "the gate, {PLAYER}{KUN}.\p"
	.string "Both squads. No cap.\n"
	.string "You proved it.\p"
	.string "MOLTRES. ARTICUNO. ZAPDOS.\n"
	.string "Still out there.\p"
	.string "MEW went quiet.\n"
	.string "DEOXYS is safe - for now.\p"
	.string "TEAM ROCKET got pressed back,\n"
	.string "but they don't break easy.\p"
	.string "HOENN is yours.\n"
	.string "Stay sharp.\p"
	.string "I now proclaim you to be the new\n"
	.string "HOENN region…$"

EverGrandeCity_ChampionsRoom_Text_PostBattleSpeechOPW::
	.string "BRENDAN: Word.\n"
	.string "You cleared both gates. Respect.\p"
	.string "MOLTRES. ARTICUNO. ZAPDOS.\n"
	.string "Still out there.\p"
	.string "MEW went quiet.\n"
	.string "DEOXYS is safe - for now.\p"
	.string "TEAM ROCKET got pressed back,\n"
	.string "but they don't break easy.\p"
	.string "You now stand at the top\n"
	.string "of the POKéMON LEAGUE.\p"
	.string "Stay sharp, CHAMPION.\n"
	.string "I now proclaim you to be the new\l"
	.string "HOENN region…$"

EverGrandeCity_ChampionsRoom_Text_MayAdvice:
	.string "MAY: {PLAYER}{KUN}!\p"
	.string "Here's some advice before you challenge\n"
	.string "the CHAMPION…$"

EverGrandeCity_ChampionsRoom_Text_MayItsAlreadyOver:
	.string "MAY: Huh?\n"
	.string "What, what, what?\p"
	.string "{PLAYER}{KUN}…\n"
	.string "Could it be that…\l"
	.string "It's already over?$"

EverGrandeCity_ChampionsRoom_Text_BrendanAdvice:
	.string "BRENDAN: Yo, {PLAYER}!\p"
	.string "You want a little wisdom before\n"
	.string "you step to the CHAMPION?…$"

EverGrandeCity_ChampionsRoom_Text_BrendanYouveWon:
	.string "BRENDAN: {PLAYER}…\p"
	.string "Hold up, deadass?\n"
	.string "You already beat the CHAMPION?$"

EverGrandeCity_ChampionsRoom_Text_BirchArriveRatePokedex:
	.string "PROF. BIRCH: See?\n"
	.string "What did I tell you, {RIVAL}?\p"
	.string "Didn't I tell you that you don't need\n"
	.string "to worry about {PLAYER}{KUN}?\p"
	.string "… … … … … … … …\n"
	.string "{PLAYER}{KUN}, you've finally done it.\p"
	.string "When I heard that you defeated your\n"
	.string "own father at the PETALBURG GYM,\l"
	.string "I thought perhaps you had a chance…\p"
	.string "But to think you've actually become\n"
	.string "the CHAMPION!\p"
	.string "Ah, yes!\p"
	.string "What became of your POKéDEX?\n"
	.string "Here, let me see.$"

EverGrandeCity_ChampionsRoom_Text_BirchCongratulations:
	.string "PROF. BIRCH: Anyways,\n"
	.string "congratulations!\p"
	.string "Now, go proudly into the final room!$"

EverGrandeCity_ChampionsRoom_Text_WallaceComeWithMe:
	.string "BRENDAN: {PLAYER}{KUN}…\p"
	.string "Nah, let me say it right.\p"
	.string "The new CHAMPION!\p"
	.string "Come with me.$"

EverGrandeCity_ChampionsRoom_Text_WallaceWaitOutside:
	.string "BRENDAN: Aight, but…\p"
	.string "From here, only CHAMPIONS enter.\p"
	.string "You'll have to wait outside\n"
	.string "with PROF. BIRCH.$"

EverGrandeCity_ChampionsRoom_Text_MayCongratulations:
	.string "MAY: Groan…\p"
	.string "… … … … … … … …\n"
	.string "… … … … … … … …\p"
	.string "I'm just joking! That's okay!\n"
	.string "That's the rule!\p"
	.string "{PLAYER}{KUN}!\n"
	.string "Honestly, congratulations!$"

EverGrandeCity_ChampionsRoom_Text_BrendanCongratulations:
	.string "BRENDAN: Whaaaat?! … … … … …\n"
	.string "… … … … … … … …\p"
	.string "Aight, rules are rules.\p"
	.string "{PLAYER}, you really did that.\n"
	.string "Respect, CHAMPION!$"
