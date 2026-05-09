.set LOCALID_DARKRAI, 1

DarkraisCave_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, DarkraisCave_OnResume
	map_script MAP_SCRIPT_ON_TRANSITION, DarkraisCave_OnTransition	
	.byte 0

DarkraisCave_OnResume:
	call_if_set FLAG_SYS_CTRL_OBJ_DELETE, DarkraisCave_EventScript_TryRemoveDarkrai
	end

DarkraisCave_EventScript_TryRemoveDarkrai::
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, Common_EventScript_NopReturn
	removeobject LOCALID_DARKRAI
	return

DarkraisCave_OnTransition:
	callnative TryGenerateShinyOW
	setvar VAR_TEMP_6, SPECIES_DARKRAI	
	call_if_unset FLAG_DEFEATED_DARKRAI, DarkraisCave_EventScript_ShowDarkrai
	end

DarkraisCave_EventScript_ShowDarkrai::
	clearflag FLAG_HIDE_DARKRAI
	setvar VAR_TEMP_1, 1
	return

DarkraisCave_EventScript_Darkrai::
	lockall
	call Eventscript_TryGenerateShinyMon
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceUp
	waitmovement 0
	playmoncry SPECIES_DARKRAI, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setvar VAR_LAST_TALKED, LOCALID_DARKRAI
	setwildbattle SPECIES_DARKRAI, 50
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
	waitstate
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	setvar VAR_TEMP_1, 0
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, DarkraisCave_EventScript_RanFromDarkrai
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, DarkraisCave_EventScript_RanFromDarkrai
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, DarkraisCave_EventScript_RanFromDarkrai
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, DarkraisCave_EventScript_DefeatedDarkrai
	goto_if_eq VAR_RESULT, B_OUTCOME_LOST, DarkraisCave_EventScript_RanFromDarkrai
	goto_if_eq VAR_RESULT, B_OUTCOME_DREW, DarkraisCave_EventScript_RanFromDarkrai
	setflag FLAG_DEFEATED_DARKRAI
	releaseall
	end

DarkraisCave_EventScript_DefeatedDarkrai::
	setflag FLAG_DEFEATED_DARKRAI
	goto Common_EventScript_RemoveStaticPokemon
	end

DarkraisCave_EventScript_RanFromDarkrai::
	setvar VAR_0x8004, SPECIES_DARKRAI
	goto Common_EventScript_LegendaryFlewAway
	end
