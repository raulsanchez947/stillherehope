	.set LOCALID_BRANDON, 4

EverGrandeCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, EverGrandeCity_OnTransition
	.byte 0

EverGrandeCity_OnTransition:
	call EventScript_WB_18
	call_if_set FLAG_SYS_WEATHER_CTRL, Common_EventScript_SetAbnormalWeather
	end

EventScript_WB_18::
	callnative TryGenerateShinyOW 
	setvar VAR_TEMP_6, SPECIES_GARCHOMP
	return

EverGrandeCity_EventScript_VictoryRoadSign::
	msgbox EverGrandeCity_Text_EnteringVictoryRoad, MSGBOX_SIGN
	end

EverGrandeCity_EventScript_CitySign::
	msgbox EverGrandeCity_Text_CitySign, MSGBOX_SIGN
	end

EverGrandeCity_EventScript_PokemonLeagueSign::
	msgbox EverGrandeCity_Text_EnteringPokemonLeague, MSGBOX_SIGN
	end

EverGrandeCity_EventScript_BrandonLeagueGate::
	trainerbattle_single TRAINER_BRANDON, EverGrandeCity_Text_BrandonLeagueGateIntro, EverGrandeCity_Text_BrandonLeagueGateDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, EverGrandeCity_EventScript_BrandonLeagueGateRematch
	msgbox EverGrandeCity_Text_BrandonLeagueGatePostBattle, MSGBOX_AUTOCLOSE
	end

EverGrandeCity_EventScript_BrandonLeagueGateRematch::
	trainerbattle_rematch TRAINER_BRANDON, EverGrandeCity_Text_BrandonLeagueGateIntro, EverGrandeCity_Text_BrandonLeagueGateDefeat
	msgbox EverGrandeCity_Text_BrandonLeagueGatePostBattle, MSGBOX_AUTOCLOSE
	end

EverGrandeCity_EventScript_SetVisitedEverGrande::
	setflag FLAG_VISITED_EVER_GRANDE_CITY
	setvar VAR_TEMP_1, 1
	end

Roaming_Pokemon_18::
    lockall
	faceplayer
	setflag FLAG_NO_ESCAPE
	call Eventscript_TryGenerateShinyMon
	playmoncry SPECIES_GARCHOMP, CRY_MODE_ENCOUNTER
    waitmoncry
	setwildbattle SPECIES_GARCHOMP, 55, ITEM_GARCHOMPITE
	dowildbattle
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, Eventscript_Won18
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, Eventscript_Escapedbossmon
	goto_if_eq VAR_RESULT, B_OUTCOME_MON_FLED, Eventscript_Escaped18
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, Eventscript_Won18
	goto_if_eq VAR_RESULT, B_OUTCOME_LOST, Eventscript_Escaped18
	goto_if_eq VAR_RESULT, B_OUTCOME_DREW, Eventscript_Escaped18
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, Eventscript_Escaped18
	clearflag FLAG_NO_ESCAPE
	releaseall
    end

Eventscript_Won18::
  	setflag FLAG_UNUSED_0x946
	removeobject VAR_LAST_TALKED
	clearflag FLAG_NO_ESCAPE
	addrankpoints 10
	releaseall
	end

Eventscript_Escaped18::
    clearflag FLAG_NO_ESCAPE
    releaseall
    end

EverGrandeCity_Text_EnteringVictoryRoad:
	.string "ENTERING VICTORY ROAD$"

EverGrandeCity_Text_EnteringPokemonLeague:
	.string "ENTERING POKéMON LEAGUE\n"
	.string "CENTER GATE$"

EverGrandeCity_Text_CitySign:
	.string "EVER GRANDE CITY\p"
	.string "“The paradise of flowers, the sea,\n"
	.string "and POKéMON.”$"

EverGrandeCity_Text_BrandonLeagueGateIntro:
	.string "BRANDON: Hold up.\p"
	.string "Before you post up in that LEAGUE\n"
	.string "CENTER, show me you can tank a full\l"
	.string "Regi squad under pressure.$"

EverGrandeCity_Text_BrandonLeagueGateDefeat:
	.string "BRANDON: Solid.\p"
	.string "You cracked the relic line clean.$"

EverGrandeCity_Text_BrandonLeagueGatePostBattle:
	.string "BRANDON: Good work.\p"
	.string "Now go heal, lock in, and don't let\n"
	.string "the ELITE FOUR catch you sleeping.$"
