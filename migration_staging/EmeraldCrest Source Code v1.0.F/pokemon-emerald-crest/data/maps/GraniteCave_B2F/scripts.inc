GraniteCave_B2F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, GraniteCave_B2F_OnTransition	
	.byte 0

GraniteCave_B2F_OnTransition:
	call EventScript_WB_16
	call Event_TryGenerateLegend
	end

EventScript_WB_16::
	callnative TryGenerateShinyOW 
	callnative GenerateWildBoss
	return

Roaming_Pokemon_16::
    lockall
	faceplayer
	setflag FLAG_NO_ESCAPE
	call Eventscript_TryGenerateShinyMon
	playmoncry VAR_TEMP_6, CRY_MODE_ENCOUNTER
    waitmoncry
	callnative setwildbossbattle
	dowildbattle
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, Eventscript_Won16
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, Eventscript_Escapedbossmon
	goto_if_eq VAR_RESULT, B_OUTCOME_MON_FLED, Eventscript_Escaped16
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, Eventscript_Won16
	goto_if_eq VAR_RESULT, B_OUTCOME_LOST, Eventscript_Escaped16
	goto_if_eq VAR_RESULT, B_OUTCOME_DREW, Eventscript_Escaped16
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, Eventscript_Escaped16
	clearflag FLAG_NO_ESCAPE
	releaseall
    end

Eventscript_Won16::
  	setflag FLAG_UNUSED_0x944
	removeobject VAR_LAST_TALKED
	clearflag FLAG_NO_ESCAPE
	addrankpoints 10
	releaseall
	end

Eventscript_Escaped16::
    clearflag FLAG_NO_ESCAPE
    releaseall
    end

EventScript_UB_Guzzlord::
    lockall
	playmoncry SPECIES_GUZZLORD, CRY_MODE_ENCOUNTER
    waitmoncry
	setwildbattle SPECIES_GUZZLORD, 55
	dowildbattle
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, Eventscript_CheckUB
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, Eventscript_UBEmpty
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, Eventscript_EscapedUB
	goto_if_eq VAR_RESULT, B_OUTCOME_MON_FLED, Eventscript_EscapedUB
	goto_if_eq VAR_RESULT, B_OUTCOME_LOST, Eventscript_EscapedUB
	goto_if_eq VAR_RESULT, B_OUTCOME_DREW, Eventscript_EscapedUB
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, Eventscript_EscapedUB
	releaseall
    end

