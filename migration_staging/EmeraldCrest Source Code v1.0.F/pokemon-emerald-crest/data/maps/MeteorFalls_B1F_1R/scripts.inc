MeteorFalls_B1F_1R_MapScripts::
map_script MAP_SCRIPT_ON_TRANSITION, MeteorFalls_B1F_1R_OnTransition
map_script MAP_SCRIPT_ON_LOAD, MeteorFalls_B1F_1R_OnLoad
	.byte 0


MeteorFalls_B1F_1R_OnLoad:
	call_if_eq VAR_ROUTE102_STATE, 2, EventScript_Show_Azelf_Cave_Forever
	end

MeteorFalls_B1F_1R_OnTransition:
	call_if_eq VAR_ROUTE102_STATE, 0, EventScript_CheckHasRageOrbAzelfCave
	end

EventScript_Show_Azelf_Cave_Forever::
	setmetatile 17, 2, METATILE_MeteorFalls_CaveEntrance_Top, TRUE
	setmetatile 16, 3, METATILE_MeteorFalls_CaveEntrance_Left, TRUE
	setmetatile 17, 3, METATILE_MeteorFalls_CaveEntrance_Bottom, FALSE
	setmetatile 18, 3, METATILE_MeteorFalls_CaveEntrance_Right, TRUE
    end
 
EventScript_CheckHasRageOrbAzelfCave::
	checkitem ITEM_RAGE_ORB
	goto_if_eq VAR_RESULT, TRUE, EventScript_SetReadyToOpenAzelfsCave
	return

EventScript_SetReadyToOpenAzelfsCave::
	setvar VAR_ROUTE102_STATE, 1
	return

EventScript_Show_Azelf_Cave::
    lockall
	setvar VAR_0x8004, 1  @ vertical pan
	setvar VAR_0x8005, 1  @ horizontal pan
	setvar VAR_0x8006, 8  @ num shakes
	setvar VAR_0x8007, 5  @ shake delay
	special ShakeCamera
	waitstate
	msgbox Text_RageOrbIsGlowing, MSGBOX_DEFAULT
	closemessage
	playmoncry SPECIES_AZELF, CRY_MODE_ENCOUNTER
	waitmoncry
	setvar VAR_0x8004, 1   @ vertical pan
	setvar VAR_0x8005, 1   @ horizontal pan
	setvar VAR_0x8006, 16  @ num shakes
	setvar VAR_0x8007, 3   @ shake delay
	special ShakeCamera
	waitstate
	playse SE_EFFECTIVE
	setmetatile 17, 2, METATILE_MeteorFalls_CaveEntrance_Top, TRUE
	setmetatile 16, 3, METATILE_MeteorFalls_CaveEntrance_Left, TRUE
	setmetatile 17, 3, METATILE_MeteorFalls_CaveEntrance_Bottom, FALSE
	setmetatile 18, 3, METATILE_MeteorFalls_CaveEntrance_Right, TRUE
	special DrawWholeMapView
	delay 30
	setvar VAR_ROUTE102_STATE, 2
	waitse
	releaseall
	end
