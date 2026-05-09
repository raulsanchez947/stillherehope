Underwater_Route127_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, Underwater_Route127_OnResume
	map_script MAP_SCRIPT_ON_TRANSITION, Underwater_Route127_OnTransition
	.byte 0

Underwater_Route127_OnResume:
	call AbnormalWeather_Underwater_SetupEscapeWarp
	end

Underwater_Route127_OnTransition:
	call EventScript_WB_11
	end

EventScript_WB_11::
	callnative TryGenerateShinyOW 
	callnative GenerateWildBoss
	return

Roaming_Pokemon_11::
    lockall
	faceplayer
	setflag FLAG_NO_ESCAPE
	call Eventscript_TryGenerateShinyMon
	playmoncry VAR_TEMP_6, CRY_MODE_ENCOUNTER
    waitmoncry
	callnative setwildbossbattle
	dowildbattle
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, Eventscript_Won11
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, Eventscript_Escapedbossmon
	goto_if_eq VAR_RESULT, B_OUTCOME_MON_FLED, Eventscript_Escaped11
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, Eventscript_Won11
	goto_if_eq VAR_RESULT, B_OUTCOME_LOST, Eventscript_Escaped11
	goto_if_eq VAR_RESULT, B_OUTCOME_DREW, Eventscript_Escaped11
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, Eventscript_Escaped11
	clearflag FLAG_NO_ESCAPE
	releaseall
    end

Eventscript_Won11::
  	setflag FLAG_UNUSED_0x93F
	removeobject VAR_LAST_TALKED
	clearflag FLAG_NO_ESCAPE
	addrankpoints 10
	releaseall
	end

Eventscript_Escaped11::
    clearflag FLAG_NO_ESCAPE
    releaseall
    end
