ShoalCave_LowTideIceRoom_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, ShoalCave_LowTideIceRoom_OnTransition
	.byte 0

ShoalCave_LowTideIceRoom_OnTransition:
	call EventScript_WB_13
	end

EventScript_WB_13::
	callnative TryGenerateShinyOW 
	callnative GenerateWildBoss
	return

Roaming_Pokemon_13::
    lockall
	faceplayer
	setflag FLAG_NO_ESCAPE
	call Eventscript_TryGenerateShinyMon
	playmoncry VAR_TEMP_6, CRY_MODE_ENCOUNTER
    waitmoncry
	callnative setwildbossbattle
	dowildbattle
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, Eventscript_Won13
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, Eventscript_Escapedbossmon
	goto_if_eq VAR_RESULT, B_OUTCOME_MON_FLED, Eventscript_Escaped13
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, Eventscript_Won13
	goto_if_eq VAR_RESULT, B_OUTCOME_LOST, Eventscript_Escaped13
	goto_if_eq VAR_RESULT, B_OUTCOME_DREW, Eventscript_Escaped13
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, Eventscript_Escaped13
	clearflag FLAG_NO_ESCAPE
	releaseall
    end

Eventscript_Won13::
  	setflag FLAG_UNUSED_0x941
	removeobject VAR_LAST_TALKED
	clearflag FLAG_NO_ESCAPE
	addrankpoints 10
	releaseall
	end

Eventscript_Escaped13::
    clearflag FLAG_NO_ESCAPE
    releaseall
    end
