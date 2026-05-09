OpenWorld_MapScripts::
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, OpenWorld_OnWrap
	.byte 0

OpenWorld_OnWrap:
	map_script_2 VAR_TEMP_1, 0, OpenWorld_MakePlayerTurn
	.2byte 0

OpenWorld_MakePlayerTurn:
	turnobject OBJ_EVENT_ID_PLAYER, DIR_NORTH
	setvar VAR_TEMP_1, 1
	end

EventScript_HeyDontLeaveYet::
	turnobject OBJ_EVENT_ID_PLAYER, DIR_NORTH
	msgbox Text_HeyDontLeaveYet, MSGBOX_AUTOCLOSE
	end

EventScript_Lotad::
	faceplayer
	playmoncry SPECIES_LOTAD, CRY_MODE_NORMAL
	msgbox Text_Cry, MSGBOX_AUTOCLOSE
	end

Text_HeyDontLeaveYet:
.string "PROF. BIRCH: Hey {PLAYER}, wait don't\n"
.string "leave yet.$"

Text_Cry:
.string "Squeee!$"
