.set LOCALID_GIOVANNI, 2
.set LOCALID_OLD_LADY, 3
.set LOCALID_GRUNT_1, 4
.set LOCALID_GRUNT_2, 5
.set LOCALID_GRUNT_3, 6
.set LOCALID_GRUNT_4, 7
.set LOCALID_APOLLO, 8
.set LOCALID_OLD_MAN, 1
.set LOCALID_LEGEND, 9

MtPyre_Summit_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, MtPyre_Summit_OnTransition
	.byte 0

MtPyre_Summit_OnTransition:
	call_if_eq VAR_MT_PYRE_STATE, 1, MtPyre_Summit_EventScript_MoltresOnSummit
	call_if_eq VAR_MT_PYRE_STATE, 2, MtPyre_Summit_EventScript_SetArchieMaxiePositions
	call_if_eq VAR_MT_PYRE_STATE, 9, MtPyre_Summit_EventScript_CaughtAllLegends
	call_if_ge VAR_MT_PYRE_STATE, 3, MtPyre_Summit_EventScript_GetLegendsReady
	end

MtPyre_Summit_EventScript_CaughtAllLegends::
	setobjectxyperm LOCALID_OLD_LADY, 23, 5
	setobjectxyperm LOCALID_OLD_MAN, 22, 5
	setobjectxyperm LOCALID_LEGEND, 22, 47
	end

MtPyre_Summit_EventScript_GetLegendsReady::
	setobjectxyperm LOCALID_OLD_LADY, 23, 47
	setobjectxyperm LOCALID_OLD_MAN, 24, 47
	goto_if_ge VAR_MT_PYRE_STATE, 6, EventScript_DragonTrio
	goto_if_eq VAR_MT_PYRE_STATE, 5, EventScript_Landorus
	random 2
	switch VAR_RESULT
	case 0, EventScript_Tornadus
	case 1, EventScript_Thundurus
	end

EventScript_DragonTrio::
	goto_if_unset FLAG_IS_CHAMPION, MtPyre_Summit_EventScript_CaughtAllLegends
	goto_if_eq VAR_MT_PYRE_STATE, 8, EventScript_Kyurem	
	random 2
	switch VAR_RESULT
	case 0, EventScript_Reshiram
	case 1, EventScript_Zeckrom
	end

EventScript_Kyurem::
	setvar VAR_0x8001, SPECIES_RESHIRAM
	callnative checkpartyhasmon
	goto_if_eq VAR_0x8000, FALSE, MtPyre_Summit_EventScript_CaughtAllLegends
	setvar VAR_0x8001, SPECIES_ZEKROM
	callnative checkpartyhasmon
	goto_if_eq VAR_0x8000, FALSE, MtPyre_Summit_EventScript_CaughtAllLegends
	callnative TryGenerateShinyOW
	setvar VAR_TEMP_6, SPECIES_KYUREM
	setvar VAR_0x8004, SPECIES_KYUREM
	setobjectxyperm LOCALID_LEGEND, 23, 5
	end

EventScript_Reshiram::
	goto_if_set FLAG_DEFEATED_RESHIRAM, EventScript_Zeckrom
	setvar VAR_0x8001, SPECIES_LANDORUS
	callnative checkpartyhasmon
	goto_if_eq VAR_0x8000, FALSE, MtPyre_Summit_EventScript_CaughtAllLegends
	callnative TryGenerateShinyOW
	setvar VAR_TEMP_6, SPECIES_RESHIRAM
	setvar VAR_0x8004, SPECIES_RESHIRAM
	setobjectxyperm LOCALID_LEGEND, 23, 5
	end

EventScript_Zeckrom::
	special DayOrNight
	goto_if_eq VAR_0x8005, 0, MtPyre_Summit_EventScript_CaughtAllLegends
	goto_if_set FLAG_DEFEATED_ZEKROM, EventScript_Reshiram
	setvar VAR_0x8001, SPECIES_LANDORUS
	callnative checkpartyhasmon
	goto_if_eq VAR_0x8000, FALSE, MtPyre_Summit_EventScript_CaughtAllLegends
	callnative TryGenerateShinyOW
	setvar VAR_TEMP_6, SPECIES_ZEKROM
	setvar VAR_0x8004, SPECIES_ZEKROM
	setobjectxyperm LOCALID_LEGEND, 23, 5
	end

EventScript_Landorus::
	goto_if_set FLAG_DEFEATED_LANDORUS, MtPyre_Summit_EventScript_CaughtAllLegends	
	setvar VAR_0x8001, SPECIES_TORNADUS
	callnative checkpartyhasmon
	goto_if_eq VAR_0x8000, FALSE, MtPyre_Summit_EventScript_CaughtAllLegends
	setvar VAR_0x8001, SPECIES_THUNDURUS
	callnative checkpartyhasmon
	goto_if_eq VAR_0x8000, FALSE, MtPyre_Summit_EventScript_CaughtAllLegends
	callnative TryGenerateShinyOW
	setvar VAR_TEMP_6, SPECIES_LANDORUS
	setvar VAR_0x8004, SPECIES_LANDORUS
	setobjectxyperm LOCALID_LEGEND, 23, 5
	end

EventScript_Tornadus::
	goto_if_set FLAG_DEFEATED_TORNADUS, EventScript_Thundurus
	setvar VAR_0x8001, SPECIES_CASTFORM
	callnative checkpartyhasmon
	goto_if_eq VAR_0x8000, FALSE, MtPyre_Summit_EventScript_CaughtAllLegends
	callnative TryGenerateShinyOW
	setvar VAR_TEMP_6, SPECIES_TORNADUS
	setvar VAR_0x8004, SPECIES_TORNADUS
	setobjectxyperm LOCALID_LEGEND, 23, 5
	end

EventScript_Thundurus::
	goto_if_set FLAG_DEFEATED_THUNDURUS, EventScript_Tornadus
	setvar VAR_0x8001, SPECIES_CASTFORM
	callnative checkpartyhasmon
	goto_if_eq VAR_0x8000, FALSE, MtPyre_Summit_EventScript_CaughtAllLegends
	callnative TryGenerateShinyOW
	setvar VAR_TEMP_6, SPECIES_THUNDURUS
	setvar VAR_0x8004, SPECIES_THUNDURUS
	setobjectxyperm LOCALID_LEGEND, 23, 5
	end

MtPyre_Summit_EventScript_MoltresOnSummit::
	goto_if_set FLAG_DEFEATED_G_MOLTRES, MtPyre_Summit_EventScript_MoltresGone
	callnative TryGenerateShinyOW
	setvar VAR_TEMP_6, SPECIES_MOLTRES_GALARIAN
	setvar VAR_0x8004, SPECIES_MOLTRES_GALARIAN
	setobjectxyperm LOCALID_LEGEND, 23, 5
	return

MtPyre_Summit_EventScript_MoltresGone::
	setobjectxyperm LOCALID_LEGEND, 20, 47
	return

MtPyre_Summit_EventScript_SetArchieMaxiePositions::
	setobjectxyperm LOCALID_APOLLO, 23, 6
	setobjectxyperm LOCALID_GIOVANNI, 22, 6
	end

MtPyre_Summit_EventScript_TeamAquaTrigger0::
	lockall
	setvar VAR_0x8008, 0
	goto MtPyre_Summit_EventScript_TeamAquaExits
	end

MtPyre_Summit_EventScript_TeamAquaTrigger1::
	lockall
	setvar VAR_0x8008, 1
	goto MtPyre_Summit_EventScript_TeamAquaExits
	end

MtPyre_Summit_EventScript_TeamAquaTrigger2::
	lockall
	setvar VAR_0x8008, 2
	goto MtPyre_Summit_EventScript_TeamAquaExits
	end

MtPyre_Summit_EventScript_TeamAquaExits::
	playbgm MUS_ENCOUNTER_AQUA, FALSE
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceUp
	waitmovement 0
	applymovement LOCALID_GIOVANNI, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	delay 50
	call_if_eq VAR_0x8008, 0, MtPyre_Summit_EventScript_ArchieFacePlayer0
	call_if_eq VAR_0x8008, 1, MtPyre_Summit_EventScript_ArchieFacePlayer1
	call_if_eq VAR_0x8008, 2, MtPyre_Summit_EventScript_ArchieFacePlayer2
	msgbox MtPyre_Summit_Text_ArchieWeGotTheOrbLetsGo, MSGBOX_DEFAULT
	closemessage
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_GIOVANNI
	removeobject LOCALID_GRUNT_1
	removeobject LOCALID_GRUNT_2
	removeobject LOCALID_GRUNT_3
	removeobject LOCALID_GRUNT_4
	setflag FLAG_HIDE_MT_PYRE_SUMMIT_ARCHIE
	setflag FLAG_HIDE_MT_PYRE_SUMMIT_TEAM_AQUA
	fadedefaultbgm
	fadescreenswapbuffers FADE_FROM_BLACK
	delay 20
	setvar VAR_MT_PYRE_STATE, 1
	call_if_eq VAR_0x8008, 0, MtPyre_Summit_EventScript_OldLadyApproachPlayer0
	call_if_eq VAR_0x8008, 1, MtPyre_Summit_EventScript_OldLadyApproachPlayer1
	call_if_eq VAR_0x8008, 2, MtPyre_Summit_EventScript_OldLadyApproachPlayer2
	msgbox MtPyre_Summit_Text_BothOrbsTakenMagmaLeftThis, MSGBOX_DEFAULT
	giveitem ITEM_MAGMA_EMBLEM
	setflag FLAG_RECEIVED_RED_OR_BLUE_ORB
	setflag FLAG_HIDE_JAGGED_PASS_MAGMA_GUARD
	releaseall
	end

MtPyre_Summit_EventScript_ArchieFacePlayer0::
	applymovement LOCALID_GIOVANNI, MtPyre_Summit_Movement_ArchieFacePlayer0
	waitmovement 0
	return

@ Archie is already facing player
MtPyre_Summit_EventScript_ArchieFacePlayer1::
	return

MtPyre_Summit_EventScript_ArchieFacePlayer2::
	applymovement LOCALID_GIOVANNI, MtPyre_Summit_Movement_ArchieFacePlayer2
	waitmovement 0
	return

MtPyre_Summit_EventScript_OldLadyApproachPlayer0::
	applymovement LOCALID_OLD_LADY, MtPyre_Summit_Movement_OldLadyApproachPlayer0
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	return

MtPyre_Summit_EventScript_OldLadyApproachPlayer1::
	applymovement LOCALID_OLD_LADY, MtPyre_Summit_Movement_OldLadyApproachPlayer1
	waitmovement 0
	return

MtPyre_Summit_EventScript_OldLadyApproachPlayer2::
	applymovement LOCALID_OLD_LADY, MtPyre_Summit_Movement_OldLadyApproachPlayer2
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	return

MtPyre_Summit_Movement_OldLadyApproachPlayer0:
	walk_down
	walk_down
	walk_in_place_faster_left
	step_end

MtPyre_Summit_Movement_OldLadyApproachPlayer1:
	walk_down
	step_end

MtPyre_Summit_Movement_OldLadyApproachPlayer2:
	walk_down
	walk_down
	walk_in_place_faster_right
	step_end

MtPyre_Summit_Movement_ArchieFacePlayer0:
	walk_left
	walk_in_place_faster_down
	step_end

MtPyre_Summit_Movement_ArchieFacePlayer2:
	walk_right
	walk_in_place_faster_down
	step_end

MtPyre_Summit_EventScript_OldMan::
	lock
	faceplayer
	goto_if_set FLAG_SOOTOPOLIS_ARCHIE_MAXIE_LEAVE, MtPyre_Summit_EventScript_OldManAfterRayquaza
	msgbox MtPyre_Summit_Text_WillYouHearOutMyTale, MSGBOX_YESNO
	call_if_eq VAR_RESULT, YES, MtPyre_Summit_EventScript_OldManTale
	call_if_eq VAR_RESULT, NO, MtPyre_Summit_EventScript_DeclineOldManTale
	release
	end

MtPyre_Summit_EventScript_OldManAfterRayquaza::
	msgbox MtPyre_Summit_Text_HearTheNewLegendOfHoenn, MSGBOX_YESNO
	call_if_eq VAR_RESULT, YES, MtPyre_Summit_EventScript_OldManNewTale
	call_if_eq VAR_RESULT, NO, MtPyre_Summit_EventScript_DeclineOldManTale
	release
	end

MtPyre_Summit_EventScript_OldManTale::
	msgbox MtPyre_Summit_Text_GroudonKyogreTale, MSGBOX_DEFAULT
	return

MtPyre_Summit_EventScript_DeclineOldManTale::
	msgbox MtPyre_Summit_Text_WellThatTooIsFine, MSGBOX_DEFAULT
	return

MtPyre_Summit_EventScript_OldManNewTale::
	msgbox MtPyre_Summit_Text_HoennTrioTale, MSGBOX_DEFAULT
	return

MtPyre_Summit_EventScript_OldLady::
	lock
	faceplayer
	goto_if_set FLAG_RETURNED_RED_OR_BLUE_ORB, MtPyre_Summit_EventScript_OldLadyAfterOrbsReturned
	call_if_ge VAR_MT_PYRE_STATE, 3, MtPyre_Summit_EventScript_OldLadyOrbsReturned
	goto_if_set FLAG_KYOGRE_ESCAPED_SEAFLOOR_CAVERN, MtPyre_Summit_EventScript_OldLadyLegendariesAwake
	msgbox MtPyre_Summit_Text_OrbsHaveBeenTaken, MSGBOX_DEFAULT
	release
	end

MtPyre_Summit_EventScript_OldLadyOrbsReturned::
	msgbox MtPyre_Summit_Text_ThoseTwoMenReturnedOrbs, MSGBOX_DEFAULT
	setflag FLAG_RETURNED_RED_OR_BLUE_ORB
	release
	end

MtPyre_Summit_EventScript_OldLadyLegendariesAwake::
	msgbox MtPyre_Summit_Text_GroudonKyogreAwakened, MSGBOX_DEFAULT
	release
	end

MtPyre_Summit_EventScript_OldLadyAfterOrbsReturned::
	msgbox MtPyre_Summit_Text_SuperAncientPokemonTaughtUs, MSGBOX_DEFAULT
	release
	end

MtPyre_Summit_EventScript_ArchieMaxieTrigger0::
	lockall
	setvar VAR_0x8008, 0
	goto MtPyre_Summit_EventScript_ArchieMaxieReturnOrbs
	end

MtPyre_Summit_EventScript_ArchieMaxieTrigger1::
	lockall
	setvar VAR_0x8008, 1
	goto MtPyre_Summit_EventScript_ArchieMaxieReturnOrbs
	end

MtPyre_Summit_EventScript_ArchieMaxieTrigger2::
	lockall
	setvar VAR_0x8008, 2
	goto MtPyre_Summit_EventScript_ArchieMaxieReturnOrbs
	end

MtPyre_Summit_EventScript_ArchieMaxieReturnOrbs::
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	delay 60
	call_if_eq VAR_0x8008, 0, MtPyre_Summit_EventScript_ArchieMaxieBeginExit0
	call_if_eq VAR_0x8008, 1, MtPyre_Summit_EventScript_ArchieMaxieBeginExit1
	call_if_eq VAR_0x8008, 2, MtPyre_Summit_EventScript_ArchieMaxieBeginExit2
	playse SE_PIN
	applymovement LOCALID_APOLLO, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_APOLLO, Common_Movement_Delay48
	waitmovement 0
	delay 30
	call_if_eq VAR_0x8008, 0, MtPyre_Summit_EventScript_MaxieApproachPlayer0
	call_if_eq VAR_0x8008, 1, MtPyre_Summit_EventScript_MaxieApproachPlayer1
	call_if_eq VAR_0x8008, 2, MtPyre_Summit_EventScript_MaxieApproachPlayer2
	msgbox MtPyre_Summit_Text_MaxieSilence, MSGBOX_DEFAULT
	closemessage
	call_if_eq VAR_0x8008, 0, MtPyre_Summit_EventScript_MaxieApproachArchie0
	call_if_eq VAR_0x8008, 1, MtPyre_Summit_EventScript_MaxieApproachArchie1
	call_if_eq VAR_0x8008, 2, MtPyre_Summit_EventScript_MaxieApproachArchie2
	delay 30
	applymovement LOCALID_GIOVANNI, MtPyre_Summit_Movement_ArchieExit
	applymovement LOCALID_APOLLO, MtPyre_Summit_Movement_MaxieExit
	waitmovement 0
	removeobject LOCALID_GIOVANNI
	removeobject LOCALID_APOLLO
	setvar VAR_MT_PYRE_STATE, 3
	releaseall
	end

MtPyre_Summit_EventScript_ArchieMaxieBeginExit0::
	applymovement OBJ_EVENT_ID_PLAYER, MtPyre_Summit_Movement_PlayerWatchArchieMaxieExit0
	applymovement LOCALID_GIOVANNI, MtPyre_Summit_Movement_ArchieExit
	applymovement LOCALID_APOLLO, MtPyre_Summit_Movement_MaxieExit
	waitmovement 0
	return

MtPyre_Summit_EventScript_ArchieMaxieBeginExit1::
	applymovement OBJ_EVENT_ID_PLAYER, MtPyre_Summit_Movement_PlayerWatchArchieMaxieExit1
	applymovement LOCALID_GIOVANNI, MtPyre_Summit_Movement_ArchieExit
	applymovement LOCALID_APOLLO, MtPyre_Summit_Movement_MaxieExit
	waitmovement 0
	return

MtPyre_Summit_EventScript_ArchieMaxieBeginExit2::
	applymovement OBJ_EVENT_ID_PLAYER, MtPyre_Summit_Movement_PlayerWatchArchieMaxieExit2
	applymovement LOCALID_GIOVANNI, MtPyre_Summit_Movement_ArchieExit
	applymovement LOCALID_APOLLO, MtPyre_Summit_Movement_MaxieExit
	waitmovement 0
	return

MtPyre_Summit_EventScript_MaxieApproachPlayer0::
	applymovement OBJ_EVENT_ID_PLAYER, MtPyre_Summit_Movement_PlayerFaceMaxie0
	applymovement LOCALID_GIOVANNI, MtPyre_Summit_Movement_ArchieWatchMaxie
	applymovement LOCALID_APOLLO, MtPyre_Summit_Movement_MaxieApproachPlayer0
	waitmovement 0
	return

MtPyre_Summit_EventScript_MaxieApproachPlayer1::
	applymovement OBJ_EVENT_ID_PLAYER, MtPyre_Summit_Movement_PlayerFaceMaxie
	applymovement LOCALID_GIOVANNI, MtPyre_Summit_Movement_ArchieWatchMaxie
	applymovement LOCALID_APOLLO, MtPyre_Summit_Movement_MaxieApproachPlayer1
	waitmovement 0
	return

MtPyre_Summit_EventScript_MaxieApproachPlayer2::
	applymovement OBJ_EVENT_ID_PLAYER, MtPyre_Summit_Movement_PlayerFaceMaxie
	applymovement LOCALID_GIOVANNI, MtPyre_Summit_Movement_ArchieWatchMaxie
	applymovement LOCALID_APOLLO, MtPyre_Summit_Movement_MaxieApproachPlayer2
	waitmovement 0
	return

MtPyre_Summit_EventScript_MaxieApproachArchie0::
	applymovement OBJ_EVENT_ID_PLAYER, MtPyre_Summit_Movement_PlayerWatchMaxie
	applymovement LOCALID_APOLLO, MtPyre_Summit_Movement_MaxieApproachArchie0
	waitmovement 0
	applymovement LOCALID_GIOVANNI, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	return

MtPyre_Summit_EventScript_MaxieApproachArchie1::
	applymovement OBJ_EVENT_ID_PLAYER, MtPyre_Summit_Movement_PlayerWatchMaxie
	applymovement LOCALID_APOLLO, MtPyre_Summit_Movement_MaxieApproachArchie1
	waitmovement 0
	applymovement LOCALID_GIOVANNI, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	return

MtPyre_Summit_EventScript_MaxieApproachArchie2::
	applymovement OBJ_EVENT_ID_PLAYER, MtPyre_Summit_Movement_PlayerWatchMaxie
	applymovement LOCALID_APOLLO, MtPyre_Summit_Movement_MaxieApproachArchie2
	waitmovement 0
	applymovement LOCALID_GIOVANNI, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	return

MtPyre_Summit_EventScript_Legend::
	call_if_eq VAR_0x8004, SPECIES_MOLTRES_GALARIAN, MtPyre_Summit_EventScript_GMoltres
	call_if_eq VAR_0x8004, SPECIES_TORNADUS, MtPyre_Summit_EventScript_Tornadus
	call_if_eq VAR_0x8004, SPECIES_THUNDURUS, MtPyre_Summit_EventScript_Thundurs
	call_if_eq VAR_0x8004, SPECIES_LANDORUS, MtPyre_Summit_EventScript_Landorus
	call_if_eq VAR_0x8004, SPECIES_RESHIRAM, MtPyre_Summit_EventScript_Reshiram
	call_if_eq VAR_0x8004, SPECIES_ZEKROM, MtPyre_Summit_EventScript_Zekrom
	call_if_eq VAR_0x8004, SPECIES_KYUREM, MtPyre_Summit_EventScript_kyurem
	end

MtPyre_Summit_EventScript_GMoltres::
	lockall
	call Eventscript_TryGenerateShinyMon
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceUp
	waitmovement 0
	msgbox MtPyre_Summit_Text_GMoltresAppears, MSGBOX_DEFAULT
	closemessage
	waitse
	playmoncry SPECIES_MOLTRES_GALARIAN, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	msgbox MtPyre_Summit_Text_GMoltresTurns, MSGBOX_DEFAULT
	closemessage
	setvar VAR_LAST_TALKED, LOCALID_LEGEND
	setwildbattle SPECIES_MOLTRES_GALARIAN, 50
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
	waitstate
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	setvar VAR_TEMP_1, 0
	setvar VAR_0x8004, SPECIES_MOLTRES_GALARIAN
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, MtPyre_Summit_EventScript_DefeatedLegend
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, MtPyre_Summit_EventScript_RanFromLegend
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, MtPyre_Summit_EventScript_RanFromLegend
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, MtPyre_Summit_EventScript_DefeatedLegend2
	goto_if_eq VAR_RESULT, B_OUTCOME_LOST, MtPyre_Summit_EventScript_RanFromLegend
	goto_if_eq VAR_RESULT, B_OUTCOME_DREW, MtPyre_Summit_EventScript_RanFromLegend
	releaseall
	end
	
MtPyre_Summit_EventScript_Tornadus::
	lockall
	call Eventscript_TryGenerateShinyMon
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceUp
	waitmovement 0
	playmoncry SPECIES_TORNADUS, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setvar VAR_LAST_TALKED, LOCALID_LEGEND
	setwildbattle SPECIES_TORNADUS, 50
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
	waitstate
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	setvar VAR_TEMP_1, 0
	setvar VAR_0x8004, SPECIES_TORNADUS
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, MtPyre_Summit_EventScript_DefeatedLegend
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, MtPyre_Summit_EventScript_RanFromLegend
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, MtPyre_Summit_EventScript_RanFromLegend
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, MtPyre_Summit_EventScript_DefeatedLegend2
	goto_if_eq VAR_RESULT, B_OUTCOME_LOST, MtPyre_Summit_EventScript_RanFromLegend
	goto_if_eq VAR_RESULT, B_OUTCOME_DREW, MtPyre_Summit_EventScript_RanFromLegend
	releaseall
	end

MtPyre_Summit_EventScript_Thundurs::
	lockall
	call Eventscript_TryGenerateShinyMon
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceUp
	waitmovement 0
	playmoncry SPECIES_THUNDURUS, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setvar VAR_LAST_TALKED, LOCALID_LEGEND
	setwildbattle SPECIES_THUNDURUS, 50
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
	waitstate
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	setvar VAR_TEMP_1, 0
	setvar VAR_0x8004, SPECIES_THUNDURUS
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, MtPyre_Summit_EventScript_DefeatedLegend
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, MtPyre_Summit_EventScript_RanFromLegend
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, MtPyre_Summit_EventScript_RanFromLegend
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, MtPyre_Summit_EventScript_DefeatedLegend2
	goto_if_eq VAR_RESULT, B_OUTCOME_LOST, MtPyre_Summit_EventScript_RanFromLegend
	goto_if_eq VAR_RESULT, B_OUTCOME_DREW, MtPyre_Summit_EventScript_RanFromLegend
	releaseall
	end

MtPyre_Summit_EventScript_Landorus::
	lockall
	call Eventscript_TryGenerateShinyMon
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceUp
	waitmovement 0
	playmoncry SPECIES_LANDORUS, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setvar VAR_LAST_TALKED, LOCALID_LEGEND
	setwildbattle SPECIES_LANDORUS, 50
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
	waitstate
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	setvar VAR_TEMP_1, 0
	setvar VAR_0x8004, SPECIES_LANDORUS
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, MtPyre_Summit_EventScript_DefeatedLegend
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, MtPyre_Summit_EventScript_RanFromLegend
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, MtPyre_Summit_EventScript_RanFromLegend
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, MtPyre_Summit_EventScript_DefeatedLegend2
	goto_if_eq VAR_RESULT, B_OUTCOME_LOST, MtPyre_Summit_EventScript_RanFromLegend
	goto_if_eq VAR_RESULT, B_OUTCOME_DREW, MtPyre_Summit_EventScript_RanFromLegend
	releaseall
	end

MtPyre_Summit_EventScript_Reshiram::
	lockall
	call Eventscript_TryGenerateShinyMon
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceUp
	waitmovement 0
	playmoncry SPECIES_RESHIRAM, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setvar VAR_LAST_TALKED, LOCALID_LEGEND
	setwildbattle SPECIES_RESHIRAM, 50
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
	waitstate
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	setvar VAR_TEMP_1, 0
	setvar VAR_0x8004, SPECIES_RESHIRAM
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, MtPyre_Summit_EventScript_DefeatedLegend
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, MtPyre_Summit_EventScript_RanFromLegend
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, MtPyre_Summit_EventScript_RanFromLegend
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, MtPyre_Summit_EventScript_DefeatedLegend2
	goto_if_eq VAR_RESULT, B_OUTCOME_LOST, MtPyre_Summit_EventScript_RanFromLegend
	goto_if_eq VAR_RESULT, B_OUTCOME_DREW, MtPyre_Summit_EventScript_RanFromLegend
	releaseall
	end

MtPyre_Summit_EventScript_Zekrom::
	lockall
	call Eventscript_TryGenerateShinyMon
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceUp
	waitmovement 0
	playmoncry SPECIES_ZEKROM, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setvar VAR_LAST_TALKED, LOCALID_LEGEND
	setwildbattle SPECIES_ZEKROM, 50
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
	waitstate
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	setvar VAR_TEMP_1, 0
	setvar VAR_0x8004, SPECIES_ZEKROM
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, MtPyre_Summit_EventScript_DefeatedLegend
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, MtPyre_Summit_EventScript_RanFromLegend
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, MtPyre_Summit_EventScript_RanFromLegend
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, MtPyre_Summit_EventScript_DefeatedLegend2
	goto_if_eq VAR_RESULT, B_OUTCOME_LOST, MtPyre_Summit_EventScript_RanFromLegend
	goto_if_eq VAR_RESULT, B_OUTCOME_DREW, MtPyre_Summit_EventScript_RanFromLegend
	releaseall
	end

MtPyre_Summit_EventScript_kyurem::
	lockall
	call Eventscript_TryGenerateShinyMon
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceUp
	waitmovement 0
	playmoncry SPECIES_KYUREM, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setvar VAR_LAST_TALKED, LOCALID_LEGEND
	setwildbattle SPECIES_KYUREM, 50, ITEM_DNA_SPLICERS
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
	waitstate
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	setvar VAR_TEMP_1, 0
	setvar VAR_0x8004, SPECIES_KYUREM
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, MtPyre_Summit_EventScript_DefeatedLegend
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, MtPyre_Summit_EventScript_RanFromLegend
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, MtPyre_Summit_EventScript_RanFromLegend
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, MtPyre_Summit_EventScript_DefeatedLegend2
	goto_if_eq VAR_RESULT, B_OUTCOME_LOST, MtPyre_Summit_EventScript_RanFromLegend
	goto_if_eq VAR_RESULT, B_OUTCOME_DREW, MtPyre_Summit_EventScript_RanFromLegend
	releaseall
	end

MtPyre_Summit_EventScript_DefeatedLegend::
	setobjectxyperm LOCALID_LEGEND, 20, 47
	goto Common_EventScript_RemoveStaticPokemon
	releaseall
	end

MtPyre_Summit_EventScript_DefeatedLegend2::
	setobjectxyperm LOCALID_LEGEND, 20, 47
	call_if_eq VAR_0x8004, SPECIES_MOLTRES_GALARIAN, EventScript_DefeatedGMoltres
	call_if_eq VAR_0x8004, SPECIES_TORNADUS, EventScript_DefeatedTornadus
	call_if_eq VAR_0x8004, SPECIES_THUNDURUS, EventScript_DefeatedThundurus
	call_if_eq VAR_0x8004, SPECIES_LANDORUS, EventScript_DefeatedLandorus
	call_if_eq VAR_0x8004, SPECIES_RESHIRAM, EventScript_DefeatedReshiram
	call_if_eq VAR_0x8004, SPECIES_ZEKROM, EventScript_DefeatedZekrom
	call_if_eq VAR_0x8004, SPECIES_KYUREM, EventScript_DefeatedKyurem
	goto Common_EventScript_RemoveStaticPokemon
	releaseall
	end

EventScript_DefeatedGMoltres::
	setflag FLAG_DEFEATED_G_MOLTRES
	return

EventScript_DefeatedTornadus::
	setflag FLAG_DEFEATED_TORNADUS
	addvar VAR_MT_PYRE_STATE, 1
	return

EventScript_DefeatedThundurus::
	setflag FLAG_DEFEATED_THUNDURUS
	addvar VAR_MT_PYRE_STATE, 1
	return

EventScript_DefeatedLandorus::
	setflag FLAG_DEFEATED_LANDORUS
	addvar VAR_MT_PYRE_STATE, 1
	return

EventScript_DefeatedReshiram::
	setflag FLAG_DEFEATED_RESHIRAM
	addvar VAR_MT_PYRE_STATE, 1
	return

EventScript_DefeatedZekrom::
	setflag FLAG_DEFEATED_ZEKROM
	addvar VAR_MT_PYRE_STATE, 1
	return

EventScript_DefeatedKyurem::
	addvar VAR_MT_PYRE_STATE, 1
	return

MtPyre_Summit_EventScript_RanFromLegend::
	setobjectxyperm LOCALID_LEGEND, 20, 47
	goto Common_EventScript_LegendaryFlewAway
	releaseall
	end

MtPyre_Summit_Movement_PlayerFaceMaxie0:
	delay_16
	delay_16
	walk_in_place_faster_right
	step_end

MtPyre_Summit_Movement_PlayerFaceMaxie:
	delay_16
	delay_16
	walk_in_place_faster_left
	step_end

MtPyre_Summit_Movement_ArchieExit:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

MtPyre_Summit_Movement_MaxieExit:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

MtPyre_Summit_Movement_PlayerWatchArchieMaxieExit0:
	delay_16
	delay_8
	walk_left
	walk_in_place_faster_right
	delay_16
	walk_in_place_faster_down
	step_end

MtPyre_Summit_Movement_PlayerWatchArchieMaxieExit1:
	delay_16
	delay_8
	walk_right
	walk_in_place_faster_left
	delay_16
	walk_in_place_faster_down
	step_end

MtPyre_Summit_Movement_PlayerWatchArchieMaxieExit2:
	delay_16
	delay_8
	walk_in_place_faster_left
	delay_16
	walk_in_place_faster_down
	step_end

MtPyre_Summit_Movement_MaxieApproachPlayer0:
	walk_up
	walk_up
	walk_up
	walk_left
	step_end

MtPyre_Summit_Movement_MaxieApproachPlayer1:
	walk_up
	walk_up
	walk_up
	walk_in_place_faster_right
	step_end

MtPyre_Summit_Movement_MaxieApproachPlayer2:
	walk_up
	walk_up
	walk_up
	walk_in_place_faster_right
	step_end

MtPyre_Summit_Movement_ArchieWatchMaxie:
	delay_16
	walk_in_place_faster_up
	step_end

MtPyre_Summit_Movement_PlayerWatchMaxie:
	delay_16
	walk_in_place_faster_down
	step_end

MtPyre_Summit_Movement_MaxieApproachArchie0:
	walk_down
	walk_right
	walk_down
	walk_down
	walk_in_place_faster_left
	step_end

MtPyre_Summit_Movement_MaxieApproachArchie1:
	walk_down
	walk_down
	walk_down
	walk_in_place_faster_left
	step_end

MtPyre_Summit_Movement_MaxieApproachArchie2:
	walk_down
	walk_down
	walk_down
	walk_in_place_faster_left
	step_end

MtPyre_Summit_EventScript_Grunt1::
	trainerbattle_single TRAINER_GRUNT_MT_PYRE_1, MtPyre_Summit_Text_Grunt1Intro, MtPyre_Summit_Text_Grunt1Defeat
	msgbox MtPyre_Summit_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

MtPyre_Summit_EventScript_Grunt2::
	trainerbattle_single TRAINER_GRUNT_MT_PYRE_2, MtPyre_Summit_Text_Grunt2Intro, MtPyre_Summit_Text_Grunt2Defeat
	msgbox MtPyre_Summit_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	end

MtPyre_Summit_EventScript_Grunt3::
	trainerbattle_single TRAINER_GRUNT_MT_PYRE_3, MtPyre_Summit_Text_Grunt3Intro, MtPyre_Summit_Text_Grunt3Defeat
	msgbox MtPyre_Summit_Text_Grunt3PostBattle, MSGBOX_AUTOCLOSE
	end

MtPyre_Summit_EventScript_Grunt4::
	trainerbattle_single TRAINER_GRUNT_MT_PYRE_4, MtPyre_Summit_Text_Grunt4Intro, MtPyre_Summit_Text_Grunt4Defeat
	msgbox MtPyre_Summit_Text_Grunt4PostBattle, MSGBOX_AUTOCLOSE
	end

MtPyre_Summit_Text_Grunt1Intro:
	.string "Out of the way!\n"
	.string "TEAM ROCKET has business here!\p"
	.string "Nobody interferes with the BOSS!$"

MtPyre_Summit_Text_Grunt1Defeat:
	.string "H-how?!\n"
	.string "Nobody's ever beaten me that fast…$"

MtPyre_Summit_Text_Grunt1PostBattle:
	.string "Tch… You're strong.\n"
	.string "But the BOSS already has what he wants!$"

MtPyre_Summit_Text_Grunt2Intro:
	.string "You think stopping one of us matters?\p"
	.string "TEAM ROCKET doesn't lose because\n"
	.string "one grunt goes down!\p"
	.string "The BOSS will be long gone\n"
	.string "before you get through all of us!$"

MtPyre_Summit_Text_Grunt2Defeat:
	.string "Unbelievable…\n"
	.string "Did the BOSS really underestimate you?!$"

MtPyre_Summit_Text_Grunt2PostBattle:
	.string "You don't know what you've\n"
	.string "gotten yourself into…$"

MtPyre_Summit_Text_Grunt3Intro:
	.string "You again?!\n"
	.string "We saw you at MT. CHIMNEY!\p"
	.string "Why do you keep following TEAM ROCKET?\n"
	.string "Give it up!$"

MtPyre_Summit_Text_Grunt3Defeat:
	.string "Fine! Go ahead!\n"
	.string "You're already too late!$"

MtPyre_Summit_Text_Grunt3PostBattle:
	.string "The BOSS has the ORB.\n"
	.string "Doesn't matter that you beat me!$"

MtPyre_Summit_Text_Grunt4Intro:
	.string "TEAM ROCKET is everywhere!\p"
	.string "There is nowhere in HOENN you can\n"
	.string "run from us!\p"
	.string "And there is nowhere you can hide\n"
	.string "from GIOVANNI's vision!$"

MtPyre_Summit_Text_Grunt4Defeat:
	.string "You're… something else…\n"
	.string "The BOSS will want to know about you.$"

MtPyre_Summit_Text_Grunt4PostBattle:
	.string "The BOSS and APOLLO make an\n"
	.string "unstoppable team. You'll see.$"

MtPyre_Summit_Text_ArchieWeGotTheOrbLetsGo:
	.string "GIOVANNI: The HOENN DEFENDERS thought\n"
	.string "they could protect this place.\p"
	.string "They were wrong.\p"
	.string "The CINDER ORB preserved at MT. PYRE…\n"
	.string "Now belongs to TEAM ROCKET.\p"
	.string "With this, our plan advances.\n"
	.string "TEAM ROCKET - we move out!$"

MtPyre_Summit_Text_BothOrbsTakenMagmaLeftThis:
	.string "Oh, no…\n"
	.string "This cannot happen…\p"
	.string "TEAM ROCKET took both ORBS.\n"
	.string "The THUNDER ORB first, now the CINDER…\p"
	.string "The THUNDER ORB and CINDER ORB must\n"
	.string "never be separated. They belong here.\p"
	.string "In the wrong hands, they could bring\n"
	.string "catastrophe upon HOENN.\p"
	.string "… … … … … …\n"
	.string "… … … … … …\p"
	.string "When those men stormed through,\n"
	.string "one of them dropped this emblem.\p"
	.string "I do not know what it means.\p"
	.string "But I would like you to have it.\n"
	.string "Perhaps it will be useful in some way.$"

MtPyre_Summit_Text_OrbsHaveBeenTaken:
	.string "The THUNDER ORB and CINDER ORB taken\n"
	.string "by those sinister men…\p"
	.string "They must never be apart…\n"
	.string "I fear something terrible will happen…$"

MtPyre_Summit_Text_GroudonKyogreAwakened:
	.string "Oh, my goodness…\p"
	.string "You say that ARTICUNO and ZAPDOS\n"
	.string "have both been disturbed?\p"
	.string "It is said that the THUNDER ORB and\n"
	.string "the CINDER ORB could quell the rage\l"
	.string "of the ancient birds…\p"
	.string "And MOLTRES…\n"
	.string "It stirred here when the CINDER ORB\l"
	.string "was taken from this very summit.\p"
	.string "Perhaps you encountered it.\p"
	.string "Three birds in turmoil…\n"
	.string "I pray something ancient hears us.$"

MtPyre_Summit_Text_ThoseTwoMenReturnedOrbs:
	.string "The two men who took the ORBS came\n"
	.string "back to return them on their own.\p"
	.string "Those men…\n"
	.string "Perhaps they are not so evil after all…$"

MtPyre_Summit_Text_SuperAncientPokemonTaughtUs:
	.string "The embodiments of fire, ice,\n"
	.string "and storm…\p"
	.string "That is said to be the identities of\n"
	.string "the ancient guardian birds…\p"
	.string "In the same way that we humans and\n"
	.string "POKéMON have life, our world, too,\l"
	.string "is alive…\p"
	.string "The ancient birds have\n"
	.string "taught us that…$"

MtPyre_Summit_Text_WillYouHearOutMyTale:
	.string "MT. PYRE is where we calm the spirits\n"
	.string "of POKéMON that have passed on…\p"
	.string "This is a high place where one can\n"
	.string "see all of HOENN…\p"
	.string "The disembodied spirits of POKéMON\n"
	.string "must find solace here…\p"
	.string "Is it coincidence that you are here…\p"
	.string "Will you hear out my tale? A tale long\n"
	.string "told in the HOENN region?$"

MtPyre_Summit_Text_GroudonKyogreTale:
	.string "It happened long, long ago…\p"
	.string "The world was wracked by a ferocious\n"
	.string "clash between the bird of flame\l"
	.string "and the bird of eternal ice…\p"
	.string "The bird of flame scorched the skies\n"
	.string "and set the mountains ablaze…\p"
	.string "The bird of ice froze the seas\n"
	.string "and buried the coasts in frost…\p"
	.string "Their fierce duel raged without end.\p"
	.string "The world was engulfed in fire\n"
	.string "and blizzard in equal measure.\p"
	.string "The GLACIAL ORB and the CINDER ORB\n"
	.string "brought an end to the calamity.\p"
	.string "A shimmering light from both ORBS\n"
	.string "becalmed the enraged birds.\p"
	.string "Stilled at last, they vanished into\n"
	.string "the deep places of the world…$"

MtPyre_Summit_Text_WellThatTooIsFine:
	.string "I see…\n"
	.string "Well, that, too, is fine…$"

MtPyre_Summit_Text_MaxieSilence:
	.string "GIOVANNI: {PLAYER}…\p"
	.string "… … … … … …\n"
	.string "… … … … … …$"

MtPyre_Summit_Text_HearTheNewLegendOfHoenn:
	.string "It is my role to pass on the legends\n"
	.string "of HOENN to future generations.\p"
	.string "And the crisis that just ended in\n"
	.string "SOOTOPOLIS rewrote a legend…\p"
	.string "Have you the time to hear the new\n"
	.string "legend of HOENN?$"

MtPyre_Summit_Text_HoennTrioTale:
	.string "It happened long, long ago…\p"
	.string "The world was wracked by a ferocious\n"
	.string "clash between the POKéMON of the land\l"
	.string "and the POKéMON of the sea…\p"
	.string "The POKéMON of the land raised\n"
	.string "mountains and created continents…\p"
	.string "The POKéMON of the sea raised huge\n"
	.string "waves and carved out oceans…\p"
	.string "Their fierce duel raged without end.\p"
	.string "The world was engulfed in massive\n"
	.string "fires and vicious storms.\p"
	.string "It was then that the POKéMON of\n"
	.string "the sky descended from a storm.\p"
	.string "The POKéMON, shining a vivid green,\n"
	.string "becalmed the two enraged POKéMON.\p"
	.string "Then, its duty done, the green POKéMON\n"
	.string "flew off into the wild blue yonder…$"

MtPyre_Summit_Text_GMoltresAppears:
	.string "The air above MT. PYRE has changed.\p"
	.string "Something vast circles overhead -\n"
	.string "dark against a darker sky.\p"
	.string "The CINDER ORB was held here\n"
	.string "for generations.\p"
	.string "It kept this one still.\n"
	.string "Dormant. Bound.\p"
	.string "Now the ORB is gone.\p"
	.string "And MOLTRES is awake.$"

MtPyre_Summit_Text_GMoltresTurns:
	.string "MOLTRES descends.\p"
	.string "Its eyes carry something ancient -\n"
	.string "not hatred, but a darkness\l"
	.string "that predates language.\p"
	.string "It studies you.\p"
	.string "Then it decides.$"
