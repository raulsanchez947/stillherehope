.set LOCALID_MESPRIT, 2

ScorchedSlab_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, ScorchedSlab_OnResume
	map_script MAP_SCRIPT_ON_TRANSITION, ScorchedSlab_OnTransition
	.byte 0

ScorchedSlab_OnResume:
	call_if_set FLAG_SYS_CTRL_OBJ_DELETE, ScorchedSlab_EventScript_TryRemoveMesprit
	end

ScorchedSlab_EventScript_TryRemoveMesprit::
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, Common_EventScript_NopReturn
	removeobject LOCALID_MESPRIT
	return

ScorchedSlab_OnTransition:
	callnative TryGenerateShinyOW
	setvar VAR_TEMP_6, SPECIES_MESPRIT
	setflag FLAG_LANDMARK_SCORCHED_SLAB
	call_if_unset FLAG_DEFEATED_MESPRIT, ScorchedSlab_EventScript_ShowMesprit
	end

ScorchedSlab_EventScript_ShowMesprit::
	clearflag FLAG_HIDE_MESPRIT
	setvar VAR_TEMP_1, 1
	return

ScorchedSlab_EventScript_Mesprit::
	lockall
	call Eventscript_TryGenerateShinyMon
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceUp
	waitmovement 0
	applymovement LOCALID_MESPRIT, ScorchedSlab_Movement_MespritApproachfront
	waitmovement 0
	waitse
	playmoncry SPECIES_MESPRIT, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setvar VAR_LAST_TALKED, LOCALID_MESPRIT
	setwildbattle SPECIES_MESPRIT, 30
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
	waitstate
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	setvar VAR_TEMP_1, 0
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, ScorchedSlab_EventScript_RanFromMesprit
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, ScorchedSlab_EventScript_DefeatedMesprit
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, ScorchedSlab_EventScript_RanFromMesprit
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, ScorchedSlab_EventScript_RanFromMesprit
	setflag FLAG_DEFEATED_MESPRIT
	setvar VAR_SHOULD_END_ABNORMAL_WEATHER, 1
	releaseall
	end

ScorchedSlab_EventScript_DefeatedMesprit::
	setflag FLAG_DEFEATED_MESPRIT
	goto Common_EventScript_RemoveStaticPokemon
	end

ScorchedSlab_EventScript_RanFromMesprit::
	setvar VAR_0x8004, SPECIES_MESPRIT
	goto Common_EventScript_LegendaryFlewAway
	end

ScorchedSlab_Movement_MespritApproachfront:
	walk_down
	walk_down
	walk_down
	walk_down
	step_end
