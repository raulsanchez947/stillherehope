SafariZone_Southeast_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SafariZone_Southeast_OnTransition
	.byte 0

SafariZone_Southeast_OnTransition:
	callnative TryGenerateShinyOW
	callnative PopupEncounters
	callnative GenerateOWEncounter
	end

@ Event scripts for SafariZone_Southeast are in SafariZone_South/scripts.inc
