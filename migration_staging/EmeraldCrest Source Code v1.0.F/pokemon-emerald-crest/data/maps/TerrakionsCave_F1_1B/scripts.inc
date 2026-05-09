.set LOCALID_TERRAKION, 1

TerrakionsCave_F1_1B_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, TerrakionsCave_F1_1B_OnResume
	map_script MAP_SCRIPT_ON_TRANSITION, TerrakionsCave_F1_1B_OnTransition
	.byte 0

TerrakionsCave_F1_1B_OnResume:
	call_if_set FLAG_SYS_CTRL_OBJ_DELETE, TerrakionsCave_F1_1B_EventScript_TryRemoveTerrakion
	end

TerrakionsCave_F1_1B_EventScript_TryRemoveTerrakion::
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, Common_EventScript_NopReturn
	removeobject LOCALID_TERRAKION
	return

TerrakionsCave_F1_1B_OnTransition:
	callnative TryGenerateShinyOW
	setvar VAR_TEMP_6, SPECIES_TERRAKION	
	call_if_unset FLAG_DEFEATED_TERRAKION, TerrakionsCave_F1_1B_EventScript_ShowTerrakion
	end

TerrakionsCave_F1_1B_EventScript_ShowTerrakion::
	clearflag FLAG_HIDE_TERRAKION
	setvar VAR_TEMP_1, 1
	return

TerrakionsCave_F1_1B_EventScript_Terrakion::
 lockall
 call Eventscript_TryGenerateShinyMon
 applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceUp
 waitmovement 0
 applymovement LOCALID_TERRAKION, TerrakionsCave_F1_1B_Movement_TerrakionApproachfront
 waitmovement 0
 waitse
 playmoncry SPECIES_TERRAKION, CRY_MODE_ENCOUNTER
 delay 40
 waitmoncry
 setvar VAR_LAST_TALKED, LOCALID_TERRAKION
 setwildbattle SPECIES_TERRAKION, 60
 setflag FLAG_SYS_CTRL_OBJ_DELETE
 special BattleSetup_StartLegendaryBattle
 waitstate
 clearflag FLAG_SYS_CTRL_OBJ_DELETE
 setvar VAR_TEMP_1, 0
 specialvar VAR_RESULT, GetBattleOutcome
 goto_if_eq VAR_RESULT, B_OUTCOME_WON, TerrakionsCave_F1_1B_EventScript_RanFromTerrakion
 goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, TerrakionsCave_F1_1B_EventScript_DefeatedTerrakion
 goto_if_eq VAR_RESULT, B_OUTCOME_RAN, TerrakionsCave_F1_1B_EventScript_RanFromTerrakion
 goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, TerrakionsCave_F1_1B_EventScript_RanFromTerrakion
 setflag FLAG_DEFEATED_TERRAKION
 setvar VAR_SHOULD_END_ABNORMAL_WEATHER, 1
 releaseall
 end

TerrakionsCave_F1_1B_EventScript_DefeatedTerrakion::
 setflag FLAG_DEFEATED_TERRAKION
 goto Common_EventScript_RemoveStaticPokemon
 end

TerrakionsCave_F1_1B_EventScript_RanFromTerrakion::
 setvar VAR_0x8004, SPECIES_TERRAKION
 goto Common_EventScript_LegendaryFlewAway
 end

TerrakionsCave_F1_1B_Movement_TerrakionApproachfront:
	delay_16
	walk_down
	walk_down
	delay_4
	step_end
