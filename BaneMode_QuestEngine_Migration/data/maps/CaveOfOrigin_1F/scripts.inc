CaveOfOrigin_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, CaveOfOrigin_1F_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, CaveOfOrigin_1F_OnFrame
	.byte 0

CaveOfOrigin_1F_OnTransition:
	call EventScript_WB_17
	call_if_set FLAG_UNUSED_RS_LEGENDARY_BATTLE_DONE, CaveOfOrigin_EventScript_DisableTriggers
	call CaveOfOrigin_1F_EventScript_InitDeoxysScene
	end

CaveOfOrigin_1F_EventScript_InitDeoxysScene::
	setvar VAR_TEMP_1, 1
	goto_if_unset FLAG_DEFEATED_MAGMA_SPACE_CENTER, CaveOfOrigin_1F_EventScript_InitDeoxysSceneDone
	goto_if_set FLAG_OPEN_WORLD, CaveOfOrigin_1F_EventScript_InitDeoxysSceneDone
	setvar VAR_TEMP_1, 0
CaveOfOrigin_1F_EventScript_InitDeoxysSceneDone::
	return

CaveOfOrigin_1F_OnFrame:
	map_script_2 VAR_TEMP_1, 0, CaveOfOrigin_1F_EventScript_DeoxysCapture
	.2byte 0

CaveOfOrigin_1F_EventScript_DeoxysCapture::
	setvar VAR_TEMP_1, 1
	lockall
	msgbox CaveOfOrigin_1F_Text_GruntsCornerSomething, MSGBOX_DEFAULT
	closemessage
	msgbox CaveOfOrigin_1F_Text_BrendanVoiceOffScreen, MSGBOX_DEFAULT
	closemessage
	msgbox CaveOfOrigin_1F_Text_BrendanCapturedDeoxys, MSGBOX_DEFAULT
	closemessage
	msgbox CaveOfOrigin_1F_Text_BrendanToPlayer, MSGBOX_DEFAULT
	closemessage
	releaseall
	end

EventScript_WB_17::
	callnative TryGenerateShinyOW
	callnative GenerateWildBoss
	return

Roaming_Pokemon_17::
    lockall
	faceplayer
	setflag FLAG_NO_ESCAPE
	call Eventscript_TryGenerateShinyMon
	playmoncry VAR_TEMP_6, CRY_MODE_ENCOUNTER
    waitmoncry
	callnative setwildbossbattle
	dowildbattle
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, Eventscript_Won17
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, Eventscript_Escapedbossmon
	goto_if_eq VAR_RESULT, B_OUTCOME_MON_FLED, Eventscript_Escaped17
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, Eventscript_Won17
	goto_if_eq VAR_RESULT, B_OUTCOME_LOST, Eventscript_Escaped17
	goto_if_eq VAR_RESULT, B_OUTCOME_DREW, Eventscript_Escaped17
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, Eventscript_Escaped17
	clearflag FLAG_NO_ESCAPE
	releaseall
    end

Eventscript_Won17::
  	setflag FLAG_UNUSED_0x945
	removeobject VAR_LAST_TALKED
	clearflag FLAG_NO_ESCAPE
	addrankpoints 10
	releaseall
	end

Eventscript_Escaped17::
    clearflag FLAG_NO_ESCAPE
    releaseall
    end

CaveOfOrigin_1F_Text_GruntsCornerSomething:
	.string "…Two TEAM ROCKET grunts.\n"
	.string "Deep inside the CAVE OF ORIGIN.\p"
	.string "They're circling something in\n"
	.string "the shadows.\p"
	.string "Something that isn't moving.\p"
	.string "Something… that doesn't belong\n"
	.string "in this world.$"

CaveOfOrigin_1F_Text_BrendanVoiceOffScreen:
	.string "???: Yo.\p"
	.string "Back up.\n"
	.string "Both of you.$"

CaveOfOrigin_1F_Text_BrendanCapturedDeoxys:
	.string "The grunts scatter.\p"
	.string "A silhouette drops down from\n"
	.string "the cavern wall.\p"
	.string "BRENDAN - holding an empty\n"
	.string "POKé BALL.\p"
	.string "The strange POKéMON is gone.\n"
	.string "He caught it.$"

CaveOfOrigin_1F_Text_BrendanToPlayer:
	.string "BRENDAN: {PLAYER}.\p"
	.string "I got it. It's safe now.\n"
	.string "On everything.\p"
	.string "TEAM ROCKET wanted it as a weapon.\n"
	.string "That ain't happening on my watch.\p"
	.string "You focus on B1F.\n"
	.string "Wallace needs you.\p"
	.string "I'll handle this one.$"
