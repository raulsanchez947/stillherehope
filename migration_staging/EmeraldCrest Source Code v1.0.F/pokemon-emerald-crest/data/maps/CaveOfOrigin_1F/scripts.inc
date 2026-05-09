CaveOfOrigin_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, CaveOfOrigin_1F_OnTransition
	.byte 0

CaveOfOrigin_1F_OnTransition:
	call EventScript_WB_17
	call_if_set FLAG_UNUSED_RS_LEGENDARY_BATTLE_DONE, CaveOfOrigin_EventScript_DisableTriggers
	end

EventScript_WB_17::
	callnative TryGenerateShinyOW 
	callnative GenerateWildBoss
	return

Roaming_Pokemon_17::
    lockall
	faceplayer
	setflag FLAG_NO_ESCAPE
	call Eventscript_TryGenerateShinyMon
	playmoncry VAR_TEMP_6, CRY_MODE_ENCOUNTER
    waitmoncry
	callnative setwildbossbattle
	dowildbattle
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, Eventscript_Won17
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, Eventscript_Escapedbossmon
	goto_if_eq VAR_RESULT, B_OUTCOME_MON_FLED, Eventscript_Escaped17
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, Eventscript_Won17
	goto_if_eq VAR_RESULT, B_OUTCOME_LOST, Eventscript_Escaped17
	goto_if_eq VAR_RESULT, B_OUTCOME_DREW, Eventscript_Escaped17
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, Eventscript_Escaped17
	clearflag FLAG_NO_ESCAPE
	releaseall
    end

Eventscript_Won17::
  	setflag FLAG_UNUSED_0x945
	removeobject VAR_LAST_TALKED
	clearflag FLAG_NO_ESCAPE
	addrankpoints 10
	releaseall
	end

Eventscript_Escaped17::
    clearflag FLAG_NO_ESCAPE
    releaseall
    end
