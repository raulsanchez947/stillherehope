.set LOCALID_ENTEI, 1

FieryPath_B1F_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, FieryPath_B1F_OnResume
	map_script MAP_SCRIPT_ON_TRANSITION, FieryPath_B1F_OnTransition	
	.byte 0

FieryPath_B1F_OnResume:
	call_if_set FLAG_SYS_CTRL_OBJ_DELETE, FieryPath_B1F_EventScript_TryRemoveEntei
	end

FieryPath_B1F_EventScript_TryRemoveEntei::
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, Common_EventScript_NopReturn
	removeobject LOCALID_ENTEI
	return

FieryPath_B1F_OnTransition:
	callnative TryGenerateShinyOW
	setvar VAR_TEMP_6, SPECIES_ENTEI
	call_if_unset FLAG_DEFEATED_ENTEI, FieryPath_B1F_EventScript_ShowEntei
	end

FieryPath_B1F_EventScript_ShowEntei::
	clearflag FLAG_HIDE_ENTEI
	setvar VAR_TEMP_1, 1
	return

FieryPath_B1F_EventScript_Entei::
	lockall
	call Eventscript_TryGenerateShinyMon
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceUp
	waitmovement 0
	playmoncry SPECIES_ENTEI, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setvar VAR_LAST_TALKED, LOCALID_ENTEI
	setwildbattle SPECIES_ENTEI, 40
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
	waitstate
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	setvar VAR_TEMP_1, 0
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, FieryPath_B1F_EventScript_RanFromEntei
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, FieryPath_B1F_EventScript_RanFromEntei
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, FieryPath_B1F_EventScript_RanFromEntei
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, FieryPath_B1F_EventScript_DefeatedEntei
	goto_if_eq VAR_RESULT, B_OUTCOME_LOST, FieryPath_B1F_EventScript_RanFromEntei
	goto_if_eq VAR_RESULT, B_OUTCOME_DREW, FieryPath_B1F_EventScript_RanFromEntei
	setflag FLAG_DEFEATED_ENTEI
	releaseall
	end

FieryPath_B1F_EventScript_DefeatedEntei::
	setflag FLAG_DEFEATED_ENTEI
	goto Common_EventScript_RemoveStaticPokemon
	end

FieryPath_B1F_EventScript_RanFromEntei::
	setvar VAR_0x8004, SPECIES_ENTEI
	goto Common_EventScript_LegendaryFlewAway
	end
