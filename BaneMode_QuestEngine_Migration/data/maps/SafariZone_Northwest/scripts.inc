SafariZone_Northwest_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SafariZone_Northwest_OnTransition
	.byte 0

SafariZone_Northwest_OnTransition:
	callnative TryGenerateShinyOW
	setvar VAR_TEMP_6, SPECIES_LYCANROC_MIDNIGHT
	callnative PopupEncounters
	callnative GenerateOWEncounter
	end

SafariZone_Northwest_EventScript_Man::
	msgbox SafariZone_Northwest_Text_Man, MSGBOX_NPC
	end

