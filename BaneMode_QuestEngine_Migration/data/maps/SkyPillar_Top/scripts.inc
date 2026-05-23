.set LOCALID_MEW_RESTING, 1

SkyPillar_Top_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, SkyPillar_Top_OnResume
	map_script MAP_SCRIPT_ON_TRANSITION, SkyPillar_Top_OnTransition
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, SkyPillar_Top_OnWarp
	.byte 0

SkyPillar_Top_OnResume:
	call_if_set FLAG_SYS_CTRL_OBJ_DELETE, SkyPillar_Top_EventScript_TryRemoveMew
	end

SkyPillar_Top_EventScript_TryRemoveMew::
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, Common_EventScript_NopReturn
	removeobject VAR_LAST_TALKED
	return

SkyPillar_Top_OnTransition:
	callnative TryGenerateShinyOW
	setvar VAR_TEMP_6, SPECIES_MEW
	call_if_lt VAR_SKY_PILLAR_STATE, 2, SkyPillar_Top_EventScript_SetCleanLayout
	call_if_ge VAR_SKY_PILLAR_STATE, 2, SkyPillar_Top_EventScript_TryShowMew
	end

SkyPillar_Top_EventScript_SetCleanLayout::
	setmaplayoutindex LAYOUT_SKY_PILLAR_TOP_CLEAN
	setobjectmovementtype LOCALID_MEW_RESTING, MOVEMENT_TYPE_FACE_DOWN
	return

SkyPillar_Top_EventScript_TryShowMew::
	call_if_unset FLAG_DEFEATED_RAYQUAZA, SkyPillar_Top_EventScript_ShowMew
	return

SkyPillar_Top_EventScript_ShowMew::
	clearflag FLAG_HIDE_SKY_PILLAR_TOP_RAYQUAZA_STILL
	return

SkyPillar_Top_OnWarp:
	map_script_2 VAR_SKY_PILLAR_STATE, 0, SkyPillar_Top_EventScript_MewFaceDown
	.2byte 0

SkyPillar_Top_EventScript_MewFaceDown::
	turnobject LOCALID_MEW_RESTING, DIR_SOUTH
	end

@ Triggered when player interacts with MEW at the top
SkyPillar_Top_EventScript_Mew::
	lockall
	call Eventscript_TryGenerateShinyMon
	waitse
	playmoncry SPECIES_MEW, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	msgbox SkyPillar_Top_Text_MewSensesYou, MSGBOX_DEFAULT
	closemessage
	setwildbattle SPECIES_MEW, 70
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
	waitstate
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, SkyPillar_Top_EventScript_RanFromMew
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, SkyPillar_Top_EventScript_CaughtMew
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, SkyPillar_Top_EventScript_RanFromMew
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, SkyPillar_Top_EventScript_RanFromMew
	setflag FLAG_DEFEATED_RAYQUAZA
	releaseall
	end

SkyPillar_Top_EventScript_CaughtMew::
	setflag FLAG_DEFEATED_RAYQUAZA
	goto SkyPillar_Top_EventScript_MewGone
	end

SkyPillar_Top_EventScript_RanFromMew::
	setvar VAR_0x8004, SPECIES_MEW
	goto SkyPillar_Top_EventScript_MewFledMessage
	end

SkyPillar_Top_EventScript_MewGone::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject VAR_LAST_TALKED
	fadescreenswapbuffers FADE_FROM_BLACK
	releaseall
	end

SkyPillar_Top_EventScript_MewFledMessage::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject VAR_LAST_TALKED
	fadescreenswapbuffers FADE_FROM_BLACK
	bufferspeciesname STR_VAR_1, VAR_0x8004
	msgbox gText_LegendaryFlewAway, MSGBOX_DEFAULT
	releaseall
	end

@ Called from Sootopolis cutscene - awakens MEW, calms the raging birds
SkyPillar_Top_EventScript_AwakenRayquaza::
SkyPillar_Top_EventScript_AwakenMew::
	lockall
	fadeoutbgm 1
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceUp
	waitmovement 0
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, SkyPillar_Top_Movement_CameraPanUp
	waitmovement 0
	special RemoveCameraObject
	msgbox SkyPillar_Top_Text_MewAppears, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_MEW_RESTING, SkyPillar_Top_Movement_MewStir
	waitmovement 0
	waitse
	playmoncry SPECIES_MEW, CRY_MODE_ENCOUNTER
	setvar VAR_0x8004, 1  @ vertical pan
	setvar VAR_0x8005, 1  @ horizontal pan
	setvar VAR_0x8006, 8  @ num shakes
	setvar VAR_0x8007, 3  @ shake delay
	special ShakeCamera
	waitstate
	waitse
	playmoncry SPECIES_MEW, CRY_MODE_ENCOUNTER
	setvar VAR_0x8004, 1  @ vertical pan
	setvar VAR_0x8005, 2  @ horizontal pan
	setvar VAR_0x8006, 8  @ num shakes
	setvar VAR_0x8007, 5  @ shake delay
	special ShakeCamera
	waitstate
	waitmoncry
	msgbox SkyPillar_Top_Text_MewCalmsTheBirds, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_MEW_RESTING, SkyPillar_Top_Movement_MewFloat
	waitmovement 0
	removeobject LOCALID_MEW_RESTING
	msgbox SkyPillar_Top_Text_MewFlewOff, MSGBOX_DEFAULT
	closemessage
	delay 20
	fadeinbgm 1
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, SkyPillar_Top_Movement_CameraPanDown
	waitmovement 0
	special RemoveCameraObject
	setvar VAR_SOOTOPOLIS_CITY_STATE, 5
	setvar VAR_SKY_PILLAR_STATE, 1
	setvar VAR_SKY_PILLAR_RAYQUAZA_CRY_DONE, 1
	releaseall
	end

SkyPillar_Top_Movement_MewStir:
	delay_16
	walk_in_place_faster_left
	delay_16
	delay_16
	walk_in_place_faster_right
	delay_16
	delay_16
	walk_in_place_faster_up
	delay_16
	delay_16
	step_end

SkyPillar_Top_Movement_MewFloat:
	delay_16
	walk_faster_up
	slide_up
	slide_up
	slide_up
	slide_up
	slide_up
	step_end

SkyPillar_Top_Movement_CameraPanUp:
	walk_slow_up
	walk_slow_up
	walk_slow_up
	step_end

SkyPillar_Top_Movement_CameraPanDown:
	walk_slow_down
	walk_slow_down
	walk_slow_down
	step_end

SkyPillar_Top_Text_MewAppears:
	.string "…Something is here.\p"
	.string "At the top of the PILLAR,\n"
	.string "curled in perfect stillness.\p"
	.string "A tiny pink shape - impossibly calm\n"
	.string "in the middle of the storm.\p"
	.string "You sense it immediately.\p"
	.string "This is MEW.\p"
	.string "The first POKéMON.\n"
	.string "The one from which all others came.\p"
	.string "HOENN's oldest protector.$"

SkyPillar_Top_Text_MewSensesYou:
	.string "MEW stirs.\p"
	.string "Its eyes open.\p"
	.string "It looks at you - not afraid,\n"
	.string "not angry.\p"
	.string "Just present.$"

SkyPillar_Top_Text_MewCalmsTheBirds:
	.string "MEW rises into the air.\p"
	.string "A soft light pulses outward -\n"
	.string "warm, ancient, certain.\p"
	.string "The raging of MOLTRES and ARTICUNO\n"
	.string "begins to still.\p"
	.string "As if the world itself\n"
	.string "remembered how to breathe.$"

SkyPillar_Top_Text_MewFlewOff:
	.string "The awakened MEW flew off…$"
