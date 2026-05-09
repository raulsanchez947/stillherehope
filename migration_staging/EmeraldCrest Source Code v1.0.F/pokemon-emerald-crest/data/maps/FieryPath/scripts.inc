FieryPath_MapScripts::
    map_script MAP_SCRIPT_ON_RESUME, FieryPath_OnResume
	map_script MAP_SCRIPT_ON_LOAD, FieryPath_OnLoad
	map_script MAP_SCRIPT_ON_TRANSITION, FieryPath_OnTransition
	.byte 0

FieryPath_OnTransition:
	call EventScript_WB_5
	call_if_unset FLAG_LANDMARK_FIERY_PATH, FieryPath_EventScript_MoveScottToFallarbor
	setflag FLAG_LANDMARK_FIERY_PATH
	end

EventScript_WB_5::	
	callnative TryGenerateShinyOW	
	callnative GenerateWildBoss
	return

FieryPath_OnLoad:
	goto_if_le VAR_ROUTE105_STATE, 1, EventScript_Hide_Entie_Cave
    return

FieryPath_OnResume:
	call_if_eq VAR_ROUTE105_STATE, 0, EventScript_CheckHasRageOrbEntei
    end

EventScript_Hide_Entie_Cave::
    setmetatile 8, 0, METATILE_Lavaridge_RockWall, TRUE
	setmetatile 8, 1, METATILE_Lavaridge_RockWall, TRUE
	return

EventScript_CheckHasRageOrbEntei::
	checkitem ITEM_RAGE_ORB
	goto_if_eq VAR_RESULT, TRUE, EventScript_SetReadyToOpenEntiesCave
	return

EventScript_SetReadyToOpenEntiesCave::
	setvar VAR_ROUTE105_STATE, 1
	return

EventScript_Show_Entie_Cave::
    lockall
	setvar VAR_0x8004, 1  @ vertical pan
	setvar VAR_0x8005, 1  @ horizontal pan
	setvar VAR_0x8006, 8  @ num shakes
	setvar VAR_0x8007, 5  @ shake delay
	special ShakeCamera
	waitstate
	msgbox Text_RageOrbIsGlowing, MSGBOX_DEFAULT
	closemessage
	playmoncry SPECIES_ENTEI, CRY_MODE_ENCOUNTER
	waitmoncry
	setvar VAR_0x8004, 1   @ vertical pan
	setvar VAR_0x8005, 1   @ horizontal pan
	setvar VAR_0x8006, 16  @ num shakes
	setvar VAR_0x8007, 3   @ shake delay
	special ShakeCamera
	waitstate
	playse SE_EFFECTIVE
	setmetatile 8, 0, METATILE_Lavaridge_CaveEntrance_Top, TRUE
	setmetatile 8, 1, METATILE_Lavaridge_CaveEntrance_Bottom, FALSE
	special DrawWholeMapView
	delay 30
	setvar VAR_ROUTE105_STATE, 2
	waitse
	releaseall
	end

FieryPath_EventScript_MoveScottToFallarbor::
	setflag FLAG_HIDE_VERDANTURF_TOWN_SCOTT
	clearflag FLAG_HIDE_FALLARBOR_TOWN_BATTLE_TENT_SCOTT
	return

Roaming_Pokemon_5::
    lockall
	faceplayer
	setflag FLAG_NO_ESCAPE
	call Eventscript_TryGenerateShinyMon
	playmoncry VAR_TEMP_6, CRY_MODE_ENCOUNTER
    waitmoncry
	callnative setwildbossbattle
	dowildbattle
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, Eventscript_Won5
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, Eventscript_Escapedbossmon
	goto_if_eq VAR_RESULT, B_OUTCOME_MON_FLED, Eventscript_Escaped5
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, Eventscript_Won5
	goto_if_eq VAR_RESULT, B_OUTCOME_LOST, Eventscript_Escaped5
	goto_if_eq VAR_RESULT, B_OUTCOME_DREW, Eventscript_Escaped5
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, Eventscript_Escaped5
	clearflag FLAG_NO_ESCAPE
	releaseall
    end

Eventscript_Won5::
  	setflag FLAG_UNUSED_0x939
	removeobject VAR_LAST_TALKED
	clearflag FLAG_NO_ESCAPE
	addrankpoints 10
	releaseall
	end

Eventscript_Escaped5::
    clearflag FLAG_NO_ESCAPE
    releaseall
    end

