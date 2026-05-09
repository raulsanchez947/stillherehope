.set LOCALID_UXIE, 1

MeteorFalls_Uxie_Cave_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, MeteorFalls_Uxie_Cave_OnResume
	map_script MAP_SCRIPT_ON_TRANSITION, MeteorFalls_Uxie_Cave_OnTransition
	.byte 0


MeteorFalls_Uxie_Cave_OnResume:
	call_if_set FLAG_SYS_CTRL_OBJ_DELETE, MeteorFalls_Uxie_Cave_EventScript_TryRemoveUxie
	end

MeteorFalls_Uxie_Cave_EventScript_TryRemoveUxie::
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, Common_EventScript_NopReturn
	removeobject LOCALID_UXIE
	return

MeteorFalls_Uxie_Cave_OnTransition:
	callnative TryGenerateShinyOW
	setvar VAR_TEMP_6, SPECIES_UXIE
	call_if_unset FLAG_DEFEATED_UXIE, MeteorFalls_Uxie_Cave_EventScript_ShowUxie
	end

MeteorFalls_Uxie_Cave_EventScript_ShowUxie::
	clearflag FLAG_HIDE_UXIE
	setvar VAR_TEMP_1, 1
	return

MeteorFalls_Uxie_Cave_EventScript_Uxie::
	lockall
	call Eventscript_TryGenerateShinyMon
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceUp
	waitmovement 0
	applymovement LOCALID_UXIE, MeteorFalls_Uxie_Cave_Movement_UxieApproachfront
	waitmovement 0
	waitse
	playmoncry SPECIES_UXIE, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setvar VAR_LAST_TALKED, LOCALID_UXIE
	setwildbattle SPECIES_UXIE, 30
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
	waitstate
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	setvar VAR_TEMP_1, 0
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, MeteorFalls_Uxie_Cave_EventScript_RanFromUxie
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, MeteorFalls_Uxie_Cave_EventScript_RanFromUxie
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, MeteorFalls_Uxie_Cave_EventScript_RanFromUxie
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, MeteorFalls_Uxie_Cave_EventScript_DefeatedUxie
	goto_if_eq VAR_RESULT, B_OUTCOME_LOST, MeteorFalls_Uxie_Cave_EventScript_RanFromUxie
	goto_if_eq VAR_RESULT, B_OUTCOME_DREW, MeteorFalls_Uxie_Cave_EventScript_RanFromUxie
	setflag FLAG_DEFEATED_UXIE
	releaseall
	end

MeteorFalls_Uxie_Cave_EventScript_DefeatedUxie::
	setflag FLAG_DEFEATED_UXIE
	goto Common_EventScript_RemoveStaticPokemon
	end

MeteorFalls_Uxie_Cave_EventScript_RanFromUxie::
	setvar VAR_0x8004, SPECIES_UXIE
	goto Common_EventScript_LegendaryFlewAway
	end

MeteorFalls_Uxie_Cave_Movement_UxieApproachfront:
	init_affine_anim
	walk_down_start_affine
	delay_16
	walk_down_affine
	delay_16
	walk_down_affine
	step_end
