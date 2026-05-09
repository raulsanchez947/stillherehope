AbandonedShip_Underwater2_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, AbandonedShip_Underwater2_OnResume
	map_script MAP_SCRIPT_ON_TRANSITION, AbandonedShip_Underwater2_OnTransition
	.byte 0

AbandonedShip_Underwater2_OnResume:
	setdivewarp MAP_ABANDONED_SHIP_ROOMS_B1F, 13, 7
	end

AbandonedShip_Underwater2_OnTransition::
call Event_TryGenerateLegend
end

