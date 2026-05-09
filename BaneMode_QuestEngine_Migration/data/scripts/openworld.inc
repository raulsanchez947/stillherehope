openworld_empty::
end

openworld_end::
lockall
releaseall
end

Get_OpenWorld_Ready::
	lockall
	faceplayer
	msgbox Text_ChooseAStarter, MSGBOX_DEFAULT
	call EventScript_Flags
	call EventScript_Vars
	call EventScript_misc
	special ChooseStarter
	waitstate
	bufferleadmonspeciesname STR_VAR_1
	playfanfare MUS_OBTAIN_ITEM
	waitfanfare
	specialvar VAR_RESULT, IsNuzlockeNicknamingActive
	goto_if_eq VAR_RESULT, TRUE, EventScript_NicknameNuzlocke
	msgbox LittlerootTown_ProfessorBirchsLab_Text_WhyNotGiveNicknameToMon, MSGBOX_DEFAULT
	call Common_EventScript_NameReceivedPartyMon
	fadescreen FADE_TO_BLACK
	special StartWallClock
	waitstate
	callnative IsMapRandmoized
	goto_if_eq VAR_RESULT, TRUE, EventScript_RandomMapSeed
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, EventScript_IntroFlagsMale
	goto_if_eq VAR_RESULT, FEMALE, EventScript_IntroFlagsFemale
	msgbox Text_GoThatWay, MSGBOX_DEFAULT
	goto_if_set FLAG_SYS_POKEMON_GET, EventScript_TP_STARTLOC
	releaseall
	end

EventScript_RandomMapSeed::
	setvar VAR_CABLE_CLUB_TUTORIAL_STATE, 2
	setvar VAR_LILYCOVE_MUSEUM_2F_STATE, 1
	setflag FLAG_ENTERED_ELITE_FOUR
	msgbox Text_EnableSeed, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, EventScript_Return
	fadescreen FADE_TO_BLACK
	callnative DoSeedNamingScreen
	fadescreen FADE_FROM_BLACK
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, EventScript_IntroFlagsMale
	goto_if_eq VAR_RESULT, FEMALE, EventScript_IntroFlagsFemale
	msgbox Text_GoThatWay, MSGBOX_DEFAULT
	goto_if_set FLAG_SYS_POKEMON_GET, EventScript_TP_STARTLOC
	releaseall
	end

EventScript_Return:
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, EventScript_IntroFlagsMale
	goto_if_eq VAR_RESULT, FEMALE, EventScript_IntroFlagsFemale
	msgbox Text_GoThatWay, MSGBOX_DEFAULT
	goto_if_set FLAG_SYS_POKEMON_GET, EventScript_TP_STARTLOC
	releaseall
	end

EventScript_TP_STARTLOC::
	applymovement OBJ_EVENT_ID_PLAYER, Movement_PlayerTPStartLoc
	waitmovement 0
	goto EventScript_TP
	releaseall
	end

EventScript_TP::
	goto_if_set FLAG_START_LOC_LITTLEROOT, EventScript_Littleroot1
	goto_if_set FLAG_START_LOC_OLDALE, EventScript_Oldale1
	goto_if_set FLAG_START_LOC_PETALBURG, EventScript_Petalburg1
	goto_if_set FLAG_START_LOC_RUSTBORO, TP_Rustboro1
	goto_if_set FLAG_START_LOC_DEWFORD, TP_Dewford1
	goto_if_set FLAG_START_LOC_SLATEPORT, TP_Slateport1
	goto_if_set FLAG_START_LOC_MAUVILLE, TP_Mauville1
	goto_if_set FLAG_START_LOC_VERDANTURF, TP_Verdanturf
	goto_if_set FLAG_START_LOC_LAVARIDGE, TP_Lavaridge1
	goto_if_set FLAG_START_LOC_FALLARBOR, TP_Fallarbor
	goto_if_set FLAG_START_LOC_FORTREE, TP_Fortree1
	goto_if_set FLAG_START_LOC_LILYCOVE, TP_Lilycove1
	goto_if_set FLAG_START_LOC_MOSSDEEP, TP_Mossdeep1
	goto_if_set FLAG_START_LOC_SOOTOPOLIS, TP_Sootopolis1
	goto_if_set FLAG_START_LOC_PACIFIDLOG, TP_Pacifidlog1
	goto_if_set FLAG_START_LOC_RANDOM, EventScript_RandomStartLoc
	releaseall
	end

EventScript_RandomStartLoc::
    random 14
	switch VAR_RESULT
	case 0, EventScript_Littleroot1
	case 1, EventScript_Oldale1
	case 2, EventScript_Petalburg1
	case 3, TP_Rustboro1
	case 4, TP_Dewford1
	case 5, TP_Slateport1
	case 6, TP_Mauville1
	case 7, TP_Verdanturf
	case 8, TP_Lavaridge1
	case 9, TP_Fallarbor
	case 10, TP_Fortree1
	case 11, TP_Lilycove1
	case 12, TP_Mossdeep1
	case 13, TP_Sootopolis1
	case 14, TP_Pacifidlog1
	releaseall
	end

EventScript_IntroFlagsMale::
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_MOM
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_TRUCK
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_TRUCK
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_RIVAL_MOM
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_RIVAL_SIBLING
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_2F_POKE_BALL
	msgbox Text_GoThatWay, MSGBOX_DEFAULT
	goto_if_set FLAG_SYS_POKEMON_GET, EventScript_TP_STARTLOC
	releaseall
	end

EventScript_IntroFlagsFemale::
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_MOM
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_TRUCK
    setflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_TRUCK
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_RIVAL_MOM
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_RIVAL_SIBLING
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_2F_POKE_BALL
	msgbox Text_GoThatWay, MSGBOX_DEFAULT
	goto_if_set FLAG_SYS_POKEMON_GET, EventScript_TP_STARTLOC
	releaseall
	end

EventScript_NicknameNuzlocke::
	message LittlerootTown_ProfessorBirchsLab_Text_WhyNotGiveNicknameToMon_Nuzlocke
	waitmessage
	setvar VAR_0x8004, 0
	call Common_EventScript_NameReceivedPartyMon
	fadescreen FADE_TO_BLACK
	special StartWallClock
	waitstate
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, EventScript_IntroFlagsMale
	goto_if_eq VAR_RESULT, FEMALE, EventScript_IntroFlagsFemale
	msgbox Text_GoThatWay, MSGBOX_DEFAULT
	goto_if_set FLAG_SYS_POKEMON_GET, EventScript_TP_STARTLOC
	releaseall
	end


Movement_PlayerTPStartLoc::
	face_down
	walk_down
	walk_down
	step_end

EventScript_ChoosePlaceAbraOpenWorld::
   switch VAR_RESULT
   case 0, EventScript_Littleroot2
   case 1, EventScript_Oldale2
   case 2, EventScript_Petalburg2
   case 3, EventScript_Rustboro1
   case 4, EventScript_Dewford1
   case 5, EventScript_Slateport1
   case 6, EventScript_Mauville1
   case 7, EventScript_Lavaridge1
   case 8, EventScript_Fortree1
   case 9, EventScript_Lilycove1
   case 10, EventScript_Mossdeep1
   case 11, EventScript_Sootopolis1
   case 12, EventScript_Pacifidlog1
   case 13, EventScript_Evergrande1
   case 14, EventScript_Verdanturf
   case 15, EventScript_Fallarbor
   case 16, EventScript_Exit1
   closemessage
   end

EventScript_ChoosePlaceOpenWorld::
   switch VAR_RESULT
   case 0, EventScript_Littleroot2
   case 1, EventScript_Oldale2
   case 2, EventScript_Petalburg2
   case 3, EventScript_Rustboro
   case 4, EventScript_Dewford
   case 5, EventScript_Slateport
   case 6, EventScript_Mauville
   case 7, EventScript_Lavaridge
   case 8, EventScript_Fortree
   case 9, EventScript_Lilycove
   case 10, EventScript_Mossdeep
   case 11, EventScript_Sootopolis
   case 12, EventScript_Pacifidlog
   case 13, EventScript_Evergrande
   case 14, EventScript_Verdanturf1
   case 15, EventScript_Fallarbor1
   case 16, EventScript_Exit
   hidemoneybox
   closemessage
   end

EventScript_Littleroot2::
	lockall
	goto_if_set FLAG_VISITED_LITTLEROOT_TOWN, TP_Littleroot
	closemessage 
	hidemoneybox
	msgbox Visit_RustboroFirst, MSGBOX_NPC
	end

TP_Littleroot::
	warpteleport MAP_LITTLEROOT_TOWN, 5, 9
	end

EventScript_Oldale2::
	lockall
	goto_if_set FLAG_VISITED_OLDALE_TOWN, TP_Oldale
	closemessage
	hidemoneybox 
	msgbox Visit_RustboroFirst, MSGBOX_NPC
	end

TP_Oldale::
	warpteleport MAP_OLDALE_TOWN, 6, 17
	end

EventScript_Petalburg2::
	lockall
	goto_if_set FLAG_VISITED_PETALBURG_CITY, TP_Petalburg
	closemessage
	hidemoneybox
	msgbox Visit_RustboroFirst, MSGBOX_NPC
	end

TP_Petalburg::
	warpteleport MAP_PETALBURG_CITY, 20, 17
	end

EventScript_Flags:
	call EventScript_SetFlags
	call EventScript_ClearFlags
	return

EventScript_SetFlags:
	setflag FLAG_SYS_POKEMON_GET
	setflag FLAG_RESCUED_BIRCH
	setflag FLAG_HIDE_ROUTE_101_BIRCH_ZIGZAGOON_BATTLE
	setflag FLAG_ADVENTURE_STARTED
	setflag FLAG_HIDE_ROUTE_101_ZIGZAGOON
	setflag FLAG_HIDE_ROUTE_103_RIVAL
	setflag FLAG_HIDE_LITTLEROOT_TOWN_PLAYERS_HOUSE_VIGOROTH_1
	setflag FLAG_HIDE_LITTLEROOT_TOWN_PLAYERS_HOUSE_VIGOROTH_2
	setflag FLAG_SYS_DEXNAV_GET
	setflag FLAG_SYS_QUEST_MENU_GET
	setflag FLAG_HIDE_KIRLIA_MAN
	setflag FLAG_SHOW_IV_AND_EV
	setflag FLAG_HAS_MATCH_CALL
	setflag FLAG_ADDED_MATCH_CALL_TO_POKENAV
	setflag FLAG_MR_BRINEY_SAILING_INTRO
	setflag FLAG_DELIVERED_STEVEN_LETTER
	setflag FLAG_DELIVERED_DEVON_GOODS
	setflag FLAG_HIDE_PETALBURG_WOODS_DEVON_EMPLOYEE
	setflag FLAG_HIDE_PETALBURG_WOODS_AQUA_GRUNT
	setflag FLAG_RECOVERED_DEVON_GOODS
	setflag FLAG_DEVON_GOODS_STOLEN
	setflag FLAG_RETURNED_DEVON_GOODS
	setflag FLAG_SYS_POKENAV_GET
	setflag FLAG_RECEIVED_POKENAV
	setflag FLAG_HIDE_RUSTBORO_CITY_POKEMON_SCHOOL_SCOTT
	setflag FLAG_RECEIVED_EXP_SHARE
	setflag FLAG_HIDE_GRANITE_CAVE_STEVEN
	setflag FLAG_DELIVERED_DEVON_GOODS
	setflag FLAG_HIDE_ROUTE_101_BIRCH_STARTERS_BAG
	setflag FLAG_HIDE_SLATEPORT_CITY_TEAM_AQUA
	setflag FLAG_HIDE_ROUTE_110_TEAM_AQUA
	setflag FLAG_HIDE_SLATEPORT_CITY_OCEANIC_MUSEUM_AQUA_GRUNTS
	setflag FLAG_HIDE_SLATEPORT_CITY_OCEANIC_MUSEUM_2F_CAPTAIN_STERN
	setflag FLAG_HIDE_MAUVILLE_CITY_WALLYS_UNCLE
	setflag FLAG_HIDE_MAUVILLE_CITY_WALLY
	setflag FLAG_RUSTURF_TUNNEL_OPENED
	setflag FLAG_HIDE_RUSTURF_TUNNEL_WANDAS_BOYFRIEND
	setflag FLAG_HIDE_RUSTURF_TUNNEL_WANDA
	setflag FLAG_HIDE_VERDANTURF_TOWN_WANDAS_HOUSE_WALLY
	setflag FLAG_HIDE_ROUTE_112_TEAM_MAGMA
	setflag FLAG_HIDE_MT_CHIMNEY_TEAM_AQUA
	setflag FLAG_HIDE_MT_CHIMNEY_TEAM_MAGMA
	setflag FLAG_RECEIVED_METEORITE
	setflag FLAG_HIDE_JAGGED_PASS_MAGMA_GUARD
	setflag FLAG_HIDE_MAGMA_HIDEOUT_4F_GROUDON_ASLEEP
	setflag FLAG_MET_ARCHIE_METEOR_FALLS
	setflag FLAG_HIDE_METEOR_FALLS_TEAM_MAGMA
	setflag FLAG_HIDE_METEOR_FALLS_TEAM_AQUA
	setflag FLAG_HIDE_METEOR_FALLS_1F_1R_COZMO
	setflag FLAG_RECEIVED_TM27
	setflag FLAG_HIDE_ROUTE_118_STEVEN
	setflag FLAG_HIDE_ROUTE_119_TEAM_AQUA
	setflag FLAG_HIDE_WEATHER_INSTITUTE_2F_AQUA_GRUNT_M
	setflag FLAG_HIDE_FORTREE_CITY_KECLEON
	setflag FLAG_HIDE_ROUTE_120_STEVEN
	setflag FLAG_HIDE_ROUTE_121_TEAM_AQUA_GRUNTS
	setflag FLAG_HIDE_MT_PYRE_SUMMIT_TEAM_AQUA
	setflag FLAG_HIDE_MT_PYRE_SUMMIT_MAXIE
	setflag FLAG_HIDE_LILYCOVE_CITY_AQUA_GRUNTS
	setflag FLAG_HIDE_AQUA_HIDEOUT_1F_GRUNT_2_BLOCKING_ENTRANCE
	setflag FLAG_HIDE_AQUA_HIDEOUT_B2F_SUBMARINE_SHADOW
	setflag FLAG_HIDE_AQUA_HIDEOUT_GRUNTS_OPW
	setflag FLAG_TEAM_AQUA_ESCAPED_IN_SUBMARINE
	setflag FLAG_HIDE_MOSSDEEP_CITY_TEAM_MAGMA
	setflag FLAG_HIDE_MOSSDEEP_CITY_SPACE_CENTER_1F_TEAM_MAGMA
	setflag FLAG_HIDE_MOSSDEEP_CITY_SPACE_CENTER_2F_TEAM_MAGMA
	setflag FLAG_HIDE_MOSSDEEP_CITY_SPACE_CENTER_2F_STEVEN
	setflag FLAG_HIDE_MOSSDEEP_CITY_SPACE_CENTER_1F_STEVEN
	setflag FLAG_HIDE_SOOTOPOLIS_CITY_KYOGRE
	setflag FLAG_HIDE_SOOTOPOLIS_CITY_GROUDON
	setflag FLAG_HIDE_SOOTOPOLIS_CITY_RAYQUAZA
	setflag FLAG_HIDE_SOOTOPOLIS_CITY_STEVEN
	setflag FLAG_HIDE_SOOTOPOLIS_CITY_ARCHIE
	setflag FLAG_HIDE_SOOTOPOLIS_CITY_MAXIE
	setflag FLAG_HIDE_SOOTOPOLIS_CITY_WALLACE
	setflag FLAG_SOOTOPOLIS_ARCHIE_MAXIE_LEAVE
	setflag FLAG_HIDE_CAVE_OF_ORIGIN_B1F_WALLACE
	setflag FLAG_HIDE_VICTORY_ROAD_ENTRANCE_WALLY
	setflag FLAG_HIDE_VICTORY_ROAD_EXIT_WALLY
	setflag FLAG_SET_WALL_CLOCK
	setflag FLAG_HIDE_SEAFLOOR_CAVERN_ENTRANCE_AQUA_GRUNT
	setflag FLAG_HIDE_SEAFLOOR_CAVERN_AQUA_GRUNTS
	setflag FLAG_HIDE_SEAFLOOR_CAVERN_ROOM_9_KYOGRE
	setflag FLAG_HIDE_SEAFLOOR_CAVERN_ROOM_9_MAXIE
	setflag FLAG_HIDE_SEAFLOOR_CAVERN_ROOM_9_MAGMA_GRUNTS
	setflag FLAG_HIDE_MT_PYRE_SUMMIT_ARCHIE
	setflag FLAG_WALLACE_GOES_TO_SKY_PILLAR
	setflag FLAG_HIDE_SKY_PILLAR_TOP_RAYQUAZA
	setflag FLAG_SYS_POKEDEX_GET
	setflag FLAG_RECEIVED_POKEDEX_FROM_BIRCH
	setflag FLAG_SYS_NATIONAL_DEX
	setflag FLAG_RECEIVED_RUNNING_SHOES
	setflag FLAG_SYS_B_DASH
	setflag FLAG_RECEIVED_BIKE
	setflag FLAG_HIDE_ROUTE_116_MR_BRINEY
	setflag FLAG_HIDE_ROUTE_110_RIVAL
	setflag FLAG_HIDE_ROUTE_110_RIVAL_ON_BIKE
	setflag FLAG_HIDE_SAFARI_ZONE_SOUTH_CONSTRUCTION_WORKERS
	return

EventScript_ClearFlags:
	clearflag FLAG_HIDE_SLATEPORT_CITY_HARBOR_CAPTAIN_STERN
	clearflag FLAG_HIDE_LITTLEROOT_TOWN_BIRCHS_LAB_BIRCH
	clearflag FLAG_HIDE_VERDANTURF_TOWN_WANDAS_HOUSE_WANDAS_BOYFRIEND
	clearflag FLAG_HIDE_VERDANTURF_TOWN_WANDAS_HOUSE_WANDA
	clearflag FLAG_HIDE_FALLARBOR_HOUSE_PROF_COZMO
	clearflag FLAG_HIDE_MT_CHIMNEY_LAVA_COOKIE_LADY
	clearflag OW_FLAG_NO_ENCOUNTER
	clearflag FLAG_HIDE_BRINEYS_HOUSE_MR_BRINEY
	clearflag FLAG_HIDE_BRINEYS_HOUSE_PEEKO
	clearflag FLAG_NO_CATCHING
	clearflag FLAG_NO_ESCAPE
	clearflag FLAG_HIDE_WORM_HOLE
	return

EventScript_Vars:
	setvar VAR_BIRCH_LAB_STATE, 5
	setvar VAR_ROUTE101_STATE, 3
	setvar VAR_LITTLEROOT_TOWN_STATE, 4 @ 4: Received Running Shoes
	setvar VAR_LITTLEROOT_INTRO_STATE, 7 @ 7: Told to go meet rival
	setvar VAR_LITTLEROOT_HOUSES_STATE_BRENDAN, 2 @ 2: Met Rivals Mom (and is corresponding gender)
	setvar VAR_LITTLEROOT_HOUSES_STATE_MAY, 2 @ 2: Met Rivals Mom (and is corresponding gender)
	setvar VAR_LITTLEROOT_RIVAL_STATE, 4 @ 4: Received Pokedex
	setvar VAR_PETALBURG_CITY_STATE, 9
	setvar VAR_PETALBURG_GYM_STATE, 6
	setvar VAR_ROUTE104_STATE, 2 
	setvar VAR_PETALBURG_WOODS_STATE, 1
	setvar VAR_RUSTBORO_CITY_STATE, 8
	setvar VAR_DEVON_CORP_3F_STATE, 1
	setvar VAR_SLATEPORT_CITY_STATE, 2
	setvar VAR_SLATEPORT_OUTSIDE_MUSEUM_STATE, 3
	setvar VAR_NEW_MAUVILLE_STATE, 2
	setvar VAR_RUSTURF_TUNNEL_STATE, 6
	setvar VAR_JAGGED_PASS_STATE, 2
	setvar VAR_METEOR_FALLS_STATE, 1
	setvar VAR_ROUTE118_STATE, 1
	setvar VAR_ROUTE119_STATE, 1
	setvar VAR_WEATHER_INSTITUTE_STATE, 1
	setvar VAR_ROUTE121_STATE, 1
	setvar VAR_MT_PYRE_STATE, 3
	setvar VAR_MOSSDEEP_CITY_STATE, 3
	setvar VAR_MOSSDEEP_SPACE_CENTER_STATE, 3
	setvar VAR_SOOTOPOLIS_CITY_STATE, 6
	setvar VAR_SEAFLOOR_CAVERN_STATE, 1
	setvar VAR_VICTORY_ROAD_1F_STATE, 4
	setvar VAR_SKY_PILLAR_RAYQUAZA_CRY_DONE, 1
	setvar VAR_SKY_PILLAR_STATE, 3
	setvar VAR_BRINEY_HOUSE_STATE, 1
	setvar VAR_ROUTE116_STATE, 2
	setvar VAR_ROUTE110_STATE, 1
	setvar VAR_CABLE_CLUB_TUTORIAL_STATE, 1
	setvar VAR_CABLE_CLUB_TUTORIAL_STATE, 1
	return

EventScript_misc:
	special SetUnlockedPokedexFlags
	special EnableNationalPokedex
	additem ITEM_ACRO_BIKE
	additem ITEM_POKE_BALL, 10
	additem ITEM_Z_POWER_RING
	additem ITEM_NORMALIUM_Z
	additem ITEM_EXP_SHARE
	additem ITEM_DEVON_SCOPE
	call Common_EventScript_ReadyPetalburgGymForBattle
	return

Text_ChooseAStarter:
.string "Welcome, {PLAYER}! Before embarking on your\n"
.string "adventure, you must choose a partner\l"
.string "POKéMON to accompany you on your\l"
.string "journey.$"

Text_GoThatWay:
.string "Alright, good luck on your journey!\n"
.string "May your path be filled with adventure\l"
.string "and success.$"

Text_EnableSeed:
	.string "Would you like to change your\n"
	.string "random map seed?$"
