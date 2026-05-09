.set LOCALID_COBALION, 1

CoblionsCave_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, CoblionsCave_OnResume
	map_script MAP_SCRIPT_ON_TRANSITION, CoblionsCave_OnTransition
	.byte 0

CoblionsCave_OnResume:
	call_if_set FLAG_SYS_CTRL_OBJ_DELETE, CoblionsCave_EventScript_TryRemoveCobalion
	end

CoblionsCave_EventScript_TryRemoveCobalion::
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, Common_EventScript_NopReturn
	removeobject LOCALID_COBALION
	return

CoblionsCave_OnTransition:
	callnative TryGenerateShinyOW
	setvar VAR_TEMP_6, SPECIES_COBALION	
	call_if_unset FLAG_DEFEATED_COBALION, CoblionsCave_EventScript_ShowCobalion
	end

CoblionsCave_EventScript_ShowCobalion::
	clearflag FLAG_HIDE_COBALION
	setvar VAR_TEMP_1, 1
	return

CoblionsCave_EventScript_Cobalion::
	lockall
	call Eventscript_TryGenerateShinyMon
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceUp
	waitmovement 0
	applymovement LOCALID_COBALION, CoblionsCave_Movement_CobalionApproachfront
	waitmovement 0
	waitse
	playmoncry SPECIES_COBALION, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setvar VAR_LAST_TALKED, LOCALID_COBALION
	setwildbattle SPECIES_COBALION, 60
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
	waitstate
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	setvar VAR_TEMP_1, 0
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, CoblionsCave_EventScript_RanFromCobalion
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, CoblionsCave_EventScript_DefeatedCobalion
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, CoblionsCave_EventScript_RanFromCobalion
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, CoblionsCave_EventScript_RanFromCobalion
	setflag FLAG_DEFEATED_COBALION
	setvar VAR_SHOULD_END_ABNORMAL_WEATHER, 1
	releaseall
	end

CoblionsCave_EventScript_DefeatedCobalion::
	setflag FLAG_DEFEATED_COBALION
	goto Common_EventScript_RemoveStaticPokemon
	end

CoblionsCave_EventScript_RanFromCobalion::
	setvar VAR_0x8004, SPECIES_COBALION
	goto Common_EventScript_LegendaryFlewAway
	end

CoblionsCave_Movement_CobalionApproachfront:
	delay_16
	walk_down
	walk_down
	delay_4
	step_end

