SafariZone_North_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SafariZone_North_OnTransition
	.byte 0

SafariZone_North_OnTransition:
	callnative TryGenerateShinyOW
	callnative PopupEncounters
	callnative GenerateOWEncounter
	end 

SafariZone_North_EventScript_Fisherman::
	msgbox SafariZone_North_Text_Fisherman, MSGBOX_NPC
	end

SafariZone_North_EventScript_Man::
	msgbox SafariZone_North_Text_Man, MSGBOX_NPC
	end

