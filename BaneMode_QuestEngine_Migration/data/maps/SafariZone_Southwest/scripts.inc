SafariZone_Southwest_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SafariZone_Southwest_OnTransition
	.byte 0

SafariZone_Southwest_OnTransition:
	callnative TryGenerateShinyOW
	callnative PopupEncounters
	callnative GenerateOWEncounter
	end

SafariZone_Southwest_EventScript_Woman::
	msgbox SafariZone_Southwest_Text_Woman, MSGBOX_NPC
	end

SafariZone_Southwest_EventScript_RestHouseSign::
	msgbox SafariZone_Southwest_Text_RestHouseSign, MSGBOX_SIGN
	end

