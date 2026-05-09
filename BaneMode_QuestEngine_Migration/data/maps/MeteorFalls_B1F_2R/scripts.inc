.set LOCALID_AZELF, 2

MeteorFalls_B1F_2R_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, MeteorFalls_B1F_2R_OnResume
	map_script MAP_SCRIPT_ON_TRANSITION, MeteorFalls_B1F_2R_OnTransition
	.byte 0


MeteorFalls_B1F_2R_OnResume:
	call_if_set FLAG_SYS_CTRL_OBJ_DELETE, MeteorFalls_B1F_2R_EventScript_TryRemoveAzelf
	end

MeteorFalls_B1F_2R_EventScript_TryRemoveAzelf::
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, Common_EventScript_NopReturn
	removeobject LOCALID_AZELF
	return

MeteorFalls_B1F_2R_OnTransition:
	callnative TryGenerateShinyOW
	setvar VAR_TEMP_6, SPECIES_AZELF
	call_if_unset FLAG_DEFEATED_AZELF, MeteorFalls_B1F_2R_EventScript_ShowAzelf
	end

MeteorFalls_B1F_2R_EventScript_ShowAzelf::
	clearflag FLAG_HIDE_AZELF
	setvar VAR_TEMP_1, 1
	return

MeteorFalls_B1F_2R_EventScript_Azelf::
	lockall
	call Eventscript_TryGenerateShinyMon
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceUp
	waitmovement 0
	applymovement LOCALID_AZELF, MeteorFalls_B1F_2R_Movement_AzelfApproach
	waitmovement 0
	waitse
	playmoncry SPECIES_AZELF, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setvar VAR_LAST_TALKED, LOCALID_AZELF
	setwildbattle SPECIES_AZELF, 30
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
	waitstate
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	setvar VAR_TEMP_1, 0
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, MeteorFalls_B1F_2R_EventScript_RanFromAzelf
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, MeteorFalls_B1F_2R_EventScript_RanFromAzelf
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, MeteorFalls_B1F_2R_EventScript_RanFromAzelf
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, MeteorFalls_B1F_2R_EventScript_DefeatedAzelf
	goto_if_eq VAR_RESULT, B_OUTCOME_LOST, MeteorFalls_B1F_2R_EventScript_RanFromAzelf
	goto_if_eq VAR_RESULT, B_OUTCOME_DREW, MeteorFalls_B1F_2R_EventScript_RanFromAzelf
	setflag FLAG_DEFEATED_AZELF
	releaseall
	end

MeteorFalls_B1F_2R_EventScript_DefeatedAzelf::
	setflag FLAG_DEFEATED_AZELF
	goto Common_EventScript_RemoveStaticPokemon
	end

MeteorFalls_B1F_2R_EventScript_RanFromAzelf::
	setvar VAR_0x8004, SPECIES_AZELF
	goto Common_EventScript_LegendaryFlewAway
	end

MeteorFalls_B1F_2R_Movement_AzelfApproach:
	delay_16
	walk_down
	walk_down
	step_end
