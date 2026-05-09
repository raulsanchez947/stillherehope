MtPyre_Exterior_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, MtPyre_Exterior_OnTransition
	.byte 0

MtPyre_Exterior_OnTransition:
	call EventScript_WB_4
	call MtPyre_Exterior_EventScript_CheckEnterFromSummit
	call Event_TryGenerateLegend
	end

EventScript_WB_4::	
	callnative TryGenerateShinyOW
	callnative GenerateWildBoss
	return

MtPyre_Exterior_EventScript_CheckEnterFromSummit::
	getplayerxy VAR_TEMP_0, VAR_TEMP_1
	goto_if_lt VAR_TEMP_1, 12, MtPyre_Exterior_EventScript_EnterFromSummit
	return

MtPyre_Exterior_EventScript_EnterFromSummit::
	setweather WEATHER_FOG_HORIZONTAL
	return

MtPyre_Exterior_EventScript_FogTrigger::
	setweather WEATHER_FOG_HORIZONTAL
	doweather
	end

MtPyre_Exterior_EventScript_SunTrigger::
	setweather WEATHER_SUNNY
	doweather
	end

Roaming_Pokemon_4::
    lockall
	faceplayer
	setflag FLAG_NO_ESCAPE
	call Eventscript_TryGenerateShinyMon
	playmoncry VAR_TEMP_6, CRY_MODE_ENCOUNTER
    waitmoncry
	callnative setwildbossbattle
	dowildbattle
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, Eventscript_Won4
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, Eventscript_Escapedbossmon
	goto_if_eq VAR_RESULT, B_OUTCOME_MON_FLED, Eventscript_Escaped4
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, Eventscript_Won4
	goto_if_eq VAR_RESULT, B_OUTCOME_LOST, Eventscript_Escaped4
	goto_if_eq VAR_RESULT, B_OUTCOME_DREW, Eventscript_Escaped4
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, Eventscript_Escaped4
	clearflag FLAG_NO_ESCAPE
	releaseall
    end

Eventscript_Won4::
  	setflag FLAG_UNUSED_0x938
	removeobject VAR_LAST_TALKED
	clearflag FLAG_NO_ESCAPE
	addrankpoints 10
	releaseall
	end

Eventscript_Escaped4::
    clearflag FLAG_NO_ESCAPE
    releaseall
    end

EventScript_UB_Blacephalon::
    lockall
	playmoncry SPECIES_BLACEPHALON, CRY_MODE_ENCOUNTER
    waitmoncry
	setwildbattle SPECIES_BLACEPHALON, 55
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

Eventscript_UBEmpty::
	removeobject VAR_LAST_TALKED
	releaseall
	end

Eventscript_CheckUB::
	removeobject VAR_LAST_TALKED
	releaseall
	end

EventScript_WonUB::	
	setflag FLAG_HIDE_WORM_HOLE
	removeobject VAR_LAST_TALKED
	releaseall
	end
