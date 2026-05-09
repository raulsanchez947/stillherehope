TerrakionsCave_F2_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, TerrakionsCave_F2_OnTransition	
	.byte 0

TerrakionsCave_F2_OnTransition:
	call EventScript_WB_15
	end

EventScript_WB_15::
	callnative TryGenerateShinyOW 
	callnative GenerateWildBoss
	return

Roaming_Pokemon_15::
    lockall
	faceplayer
	setflag FLAG_NO_ESCAPE
	call Eventscript_TryGenerateShinyMon
	playmoncry VAR_TEMP_6, CRY_MODE_ENCOUNTER
    waitmoncry
	callnative setwildbossbattle
	dowildbattle
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, Eventscript_Won15
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, Eventscript_Escapedbossmon
	goto_if_eq VAR_RESULT, B_OUTCOME_MON_FLED, Eventscript_Escaped15
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, Eventscript_Won15
	goto_if_eq VAR_RESULT, B_OUTCOME_LOST, Eventscript_Escaped15
	goto_if_eq VAR_RESULT, B_OUTCOME_DREW, Eventscript_Escaped15
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, Eventscript_Escaped15
	clearflag FLAG_NO_ESCAPE
	releaseall
    end

Eventscript_Won15::
  	setflag FLAG_UNUSED_0x943
	removeobject VAR_LAST_TALKED
	clearflag FLAG_NO_ESCAPE
	addrankpoints 10
	releaseall
	end

Eventscript_Escaped15::
    clearflag FLAG_NO_ESCAPE
    releaseall
    end
