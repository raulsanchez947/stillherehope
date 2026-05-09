.set LOCALID_RAIKOU, 1

RaikousCave_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, RaikousCave_OnResume
	map_script MAP_SCRIPT_ON_TRANSITION, RaikousCave_OnTransition	
	.byte 0

RaikousCave_OnResume:
	call_if_set FLAG_SYS_CTRL_OBJ_DELETE, RaikousCave_EventScript_TryRemoveRaikou
	end

RaikousCave_EventScript_TryRemoveRaikou::
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, Common_EventScript_NopReturn
	removeobject LOCALID_RAIKOU
	return

RaikousCave_OnTransition:
	callnative TryGenerateShinyOW
	setvar VAR_TEMP_6, SPECIES_RAIKOU
	call_if_unset FLAG_DEFEATED_RAIKOU, RaikousCave_EventScript_ShowRaikou
	end

RaikousCave_EventScript_ShowRaikou::
	clearflag FLAG_HIDE_RAIKOU
	setvar VAR_TEMP_1, 1
	return

RaikousCave_EventScript_Raikou::
	lockall
	call Eventscript_TryGenerateShinyMon
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceUp
	waitmovement 0
	playmoncry SPECIES_RAIKOU, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setvar VAR_LAST_TALKED, LOCALID_RAIKOU
	setwildbattle SPECIES_RAIKOU, 40
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
	waitstate
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	setvar VAR_TEMP_1, 0
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, RaikousCave_EventScript_RanFromRaikou
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, RaikousCave_EventScript_RanFromRaikou
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, RaikousCave_EventScript_RanFromRaikou
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, RaikousCave_EventScript_DefeatedRaikou
	goto_if_eq VAR_RESULT, B_OUTCOME_LOST, RaikousCave_EventScript_RanFromRaikou
	goto_if_eq VAR_RESULT, B_OUTCOME_DREW, RaikousCave_EventScript_RanFromRaikou
	setflag FLAG_DEFEATED_RAIKOU
	releaseall
 end

RaikousCave_EventScript_DefeatedRaikou::
	setflag FLAG_DEFEATED_RAIKOU
	goto Common_EventScript_RemoveStaticPokemon
 end

RaikousCave_EventScript_RanFromRaikou::
	setvar VAR_0x8004, SPECIES_RAIKOU
	goto Common_EventScript_LegendaryFlewAway
 end
