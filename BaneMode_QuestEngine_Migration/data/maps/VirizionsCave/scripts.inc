.set LOCALID_VIRIZION, 1

VirizionsCave_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, VirizionsCave_OnResume
	map_script MAP_SCRIPT_ON_TRANSITION, VirizionsCave_OnTransition
	.byte 0

VirizionsCave_OnResume:
	call_if_set FLAG_SYS_CTRL_OBJ_DELETE, VirizionsCave_EventScript_TryRemoveVirizion
	end

VirizionsCave_EventScript_TryRemoveVirizion::
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, Common_EventScript_NopReturn
	removeobject LOCALID_VIRIZION
	return

VirizionsCave_OnTransition:
	callnative TryGenerateShinyOW
	setvar VAR_TEMP_6, SPECIES_VIRIZION	
	call_if_unset FLAG_DEFEATED_VIRIZION, VirizionsCave_EventScript_ShowVirizion
	end

VirizionsCave_EventScript_ShowVirizion::
	clearflag FLAG_HIDE_VIRIZION
	setvar VAR_TEMP_1, 1
	return

VirizionsCave_EventScript_Virizion::
	lockall
	call Eventscript_TryGenerateShinyMon
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceUp
	waitmovement 0
	applymovement LOCALID_VIRIZION, VirizionsCave_Movement_VirizionApproachfront
	waitmovement 0
	waitse
	playmoncry SPECIES_VIRIZION, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setvar VAR_LAST_TALKED, LOCALID_VIRIZION
	setwildbattle SPECIES_VIRIZION, 60
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
	waitstate
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	setvar VAR_TEMP_1, 0
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, VirizionsCave_EventScript_RanFromVirizion
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, VirizionsCave_EventScript_DefeatedVirizion
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, VirizionsCave_EventScript_RanFromVirizion
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, VirizionsCave_EventScript_RanFromVirizion
	setflag FLAG_DEFEATED_VIRIZION
	setvar VAR_SHOULD_END_ABNORMAL_WEATHER, 1
	releaseall
	end

VirizionsCave_EventScript_DefeatedVirizion::
	setflag FLAG_DEFEATED_VIRIZION
	goto Common_EventScript_RemoveStaticPokemon
	end

VirizionsCave_EventScript_RanFromVirizion::
	setvar VAR_0x8004, SPECIES_VIRIZION
	goto Common_EventScript_LegendaryFlewAway
	end

VirizionsCave_Movement_VirizionApproachfront:
	delay_16
	walk_down
	walk_down
	delay_4
	step_end

