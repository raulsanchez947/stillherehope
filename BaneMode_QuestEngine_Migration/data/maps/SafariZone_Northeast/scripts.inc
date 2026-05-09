SafariZone_Northeast_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SafariZone_Northeast_OnTransition
	.byte 0

SafariZone_Northeast_OnTransition:
	callnative TryGenerateShinyOW
	callnative PopupEncounters
	callnative GenerateOWEncounter
	end

@ Event scripts for SafariZone_Northeast are in SafariZone_South/scripts.inc
