.set LOCALID_GRUNT_1, 1
.set LOCALID_GRUNT_2, 2
.set LOCALID_ROCKET_ADMIN, 3
.set LOCALID_SCIENTIST, 5
.set LOCALID_GRUNT_3, 7
.set LOCALID_GRUNT_4, 8

Route119_WeatherInstitute_2F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route119_WeatherInstitute_2F_OnTransition
	.byte 0

Route119_WeatherInstitute_2F_OnTransition:
	call_if_eq VAR_WEATHER_INSTITUTE_STATE, 0, Route119_WeatherInstitute_2F_EventScript_SetScientistPosAquaHere
	call_if_eq VAR_WEATHER_INSTITUTE_STATE, 1, Route119_WeatherInstitute_2F_EventScript_SetScientistPosAquaGone
	call_if_set FLAG_SYS_GAME_CLEAR, Route119_WeatherInstitute_2F_EventScript_SetScientistPosGameClear
	end

Route119_WeatherInstitute_2F_EventScript_SetScientistPosAquaHere::
	setobjectxyperm LOCALID_SCIENTIST, 1, 6
	setobjectmovementtype LOCALID_SCIENTIST, MOVEMENT_TYPE_FACE_RIGHT
	return

Route119_WeatherInstitute_2F_EventScript_SetScientistPosAquaGone::
	setobjectxyperm LOCALID_SCIENTIST, 4, 6
	setobjectmovementtype LOCALID_SCIENTIST, MOVEMENT_TYPE_FACE_RIGHT
	return

Route119_WeatherInstitute_2F_EventScript_SetScientistPosGameClear::
	setobjectxyperm LOCALID_SCIENTIST, 2, 2
	setobjectmovementtype LOCALID_SCIENTIST, MOVEMENT_TYPE_FACE_UP
	return

Route119_WeatherInstitute_2F_EventScript_Grunt5::
	trainerbattle_single TRAINER_GRUNT_WEATHER_INST_5, Route119_WeatherInstitute_2F_Text_Grunt5Intro, Route119_WeatherInstitute_2F_Text_Grunt5Defeat
	msgbox Route119_WeatherInstitute_2F_Text_Grunt5PostBattle, MSGBOX_AUTOCLOSE
	end

Route119_WeatherInstitute_2F_EventScript_Grunt2::
	trainerbattle_single TRAINER_GRUNT_WEATHER_INST_2, Route119_WeatherInstitute_2F_Text_Grunt2Intro, Route119_WeatherInstitute_2F_Text_Grunt2Defeat
	msgbox Route119_WeatherInstitute_2F_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	end

Route119_WeatherInstitute_2F_EventScript_Grunt3::
	trainerbattle_single TRAINER_GRUNT_WEATHER_INST_3, Route119_WeatherInstitute_2F_Text_Grunt3Intro, Route119_WeatherInstitute_2F_Text_Grunt3Defeat
	msgbox Route119_WeatherInstitute_2F_Text_Grunt3PostBattle, MSGBOX_AUTOCLOSE
	end

Route119_WeatherInstitute_2F_EventScript_Shelly::
	trainerbattle_single TRAINER_SHELLY_WEATHER_INSTITUTE, Route119_WeatherInstitute_2F_Text_ShellyIntro, Route119_WeatherInstitute_2F_Text_ShellyDefeat, Route119_WeatherInstitute_2F_EventScript_ShellyDefeated
	msgbox Route119_WeatherInstitute_2F_Text_ShellyPostBattle, MSGBOX_AUTOCLOSE
	end

Route119_WeatherInstitute_2F_EventScript_ShellyDefeated::
	msgbox Route119_WeatherInstitute_2F_Text_ShellyPostBattle, MSGBOX_DEFAULT
	closemessage
	addobject LOCALID_GRUNT_3
	applymovement LOCALID_GRUNT_3, Route119_WeatherInstitute_2F_Movement_GruntApproachShelly
	applymovement OBJ_EVENT_ID_PLAYER, Route119_WeatherInstitute_2F_Movement_ShovePlayerOutOfWay
	waitmovement 0
	msgbox Route119_WeatherInstitute_2F_Text_TeamMagmaJustPassedBy, MSGBOX_DEFAULT
	closemessage
	playse SE_PIN
	applymovement LOCALID_ROCKET_ADMIN, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_ROCKET_ADMIN, Common_Movement_Delay48
	waitmovement 0
	msgbox Route119_WeatherInstitute_2F_Text_WeHaveToHurryToMtPyre, MSGBOX_DEFAULT
	closemessage
	setvar VAR_WEATHER_INSTITUTE_STATE, 1
	clearflag FLAG_HIDE_WEATHER_INSTITUTE_2F_WORKERS
	fadedefaultbgm
	fadescreen FADE_TO_BLACK
	setflag FLAG_HIDE_ROUTE_119_TEAM_AQUA
	setflag FLAG_HIDE_WEATHER_INSTITUTE_2F_AQUA_GRUNT_M
	removeobject LOCALID_GRUNT_1
	removeobject LOCALID_GRUNT_2
	removeobject LOCALID_ROCKET_ADMIN
	removeobject LOCALID_GRUNT_4
	removeobject LOCALID_GRUNT_3
	applymovement OBJ_EVENT_ID_PLAYER, Route119_WeatherInstitute_2F_Movement_PlayerReturnToPosition
	waitmovement 0
	fadescreen FADE_FROM_BLACK
	applymovement LOCALID_SCIENTIST, Route119_WeatherInstitute_2F_Movement_ScientistApproachPlayer
	waitmovement 0
	copyobjectxytoperm LOCALID_SCIENTIST
	msgbox Route119_WeatherInstitute_2F_Text_ScientistFoundTempestOrb, MSGBOX_DEFAULT
	giveitem ITEM_TEMPEST_ORB
	goto Route119_WeatherInstitute_2F_EventScript_ReceiveCastform
	end

Route119_WeatherInstitute_2F_EventScript_ReceiveCastform::
	goto_if_set FLAG_OPEN_WORLD, Route119_WeatherInstitute_2F_CastformOP
	msgbox Route119_WeatherInstitute_2F_Text_ThanksPleaseTakePokemon, MSGBOX_DEFAULT
	setvar VAR_TEMP_1, SPECIES_CASTFORM
	givemon SPECIES_CASTFORM, 25, ITEM_MYSTIC_WATER
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PARTY, Route119_WeatherInstitute_2F_EventScript_ReceiveCastformParty
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PC, Route119_WeatherInstitute_2F_EventScript_ReceiveCastformPC
	goto Common_EventScript_NoMoreRoomForPokemon
	end

Route119_WeatherInstitute_2F_CastformOP::	
	msgbox Route119_WeatherInstitute_2F_Text_ThanksPleaseTakePokemonOP, MSGBOX_DEFAULT
	setvar VAR_TEMP_1, SPECIES_CASTFORM
	givemon SPECIES_CASTFORM, 10, ITEM_MYSTIC_WATER
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PARTY, Route119_WeatherInstitute_2F_EventScript_ReceiveCastformParty
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PC, Route119_WeatherInstitute_2F_EventScript_ReceiveCastformPC
	goto Common_EventScript_NoMoreRoomForPokemon
	end

Route119_WeatherInstitute_2F_EventScript_ReceiveCastformParty::
	call Route119_WeatherInstitute_2F_EventScript_ReceivedCastformFanfare
	specialvar VAR_RESULT, IsNuzlockeNicknamingActive
	goto_if_eq VAR_RESULT, TRUE, Route119_WeatherInstitute_2F_EventScript_ReceiveCastformParty_Nuzlocke
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Route119_WeatherInstitute_2F_EventScript_ExplainCastform
	call Common_EventScript_GetGiftMonPartySlot
	call Common_EventScript_NameReceivedPartyMon
	goto Route119_WeatherInstitute_2F_EventScript_ExplainCastform
	end

Route119_WeatherInstitute_2F_EventScript_ReceiveCastformParty_Nuzlocke::
	call Common_EventScript_GetGiftMonPartySlot
	call Common_EventScript_NameReceivedPartyMon
	goto Route119_WeatherInstitute_2F_EventScript_ExplainCastform
	end

Route119_WeatherInstitute_2F_EventScript_ReceiveCastformPC::
	call Route119_WeatherInstitute_2F_EventScript_ReceivedCastformFanfare
	specialvar VAR_RESULT, IsNuzlockeNicknamingActive
	goto_if_eq VAR_RESULT, TRUE, Route119_WeatherInstitute_2F_EventScript_ReceiveCastformPC_Nuzlocke
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Route119_WeatherInstitute_2F_EventScript_SendCastformToPC
	call Common_EventScript_NameReceivedBoxMon
	goto Route119_WeatherInstitute_2F_EventScript_SendCastformToPC
	end

Route119_WeatherInstitute_2F_EventScript_ReceiveCastformPC_Nuzlocke::
	call Common_EventScript_NameReceivedBoxMon
	goto Route119_WeatherInstitute_2F_EventScript_SendCastformToPC
	end

Route119_WeatherInstitute_2F_EventScript_SendCastformToPC::
	call Common_EventScript_TransferredToPC
	goto Route119_WeatherInstitute_2F_EventScript_ExplainCastform
	end

Route119_WeatherInstitute_2F_EventScript_ReceivedCastformFanfare::
	playfanfare MUS_OBTAIN_ITEM
	message Route119_WeatherInstitute_2F_Text_PlayerReceivedCastform
	waitmessage
	waitfanfare
	bufferspeciesname STR_VAR_1, SPECIES_CASTFORM
	return

Route119_WeatherInstitute_2F_EventScript_ExplainCastform::
	msgbox Route119_WeatherInstitute_2F_Text_PokemonChangesWithWeather, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_CASTFORM
	release
	end

Route119_WeatherInstitute_2F_EventScript_ScientistMentionWeather::
	goto_if_set FLAG_SYS_GAME_CLEAR, Route119_WeatherInstitute_2F_EventScript_TryStartAbnormalWeather
	msgbox Route119_WeatherInstitute_2F_Text_ChangingWeatherRidiculous, MSGBOX_DEFAULT
	release
	end

Route119_WeatherInstitute_2F_EventScript_TryStartAbnormalWeather::
	setvar VAR_0x8004, 0
	call_if_set FLAG_DEFEATED_KYOGRE, Route119_WeatherInstitute_2F_EventScript_LegendaryDefeated
	call_if_set FLAG_DEFEATED_GROUDON, Route119_WeatherInstitute_2F_EventScript_LegendaryDefeated
	goto_if_eq VAR_0x8004, 2, Route119_WeatherInstitute_2F_EventScript_NoAbnormalWeather  @ Both defeated
	call_if_unset FLAG_TEMP_2, Route119_WeatherInstitute_2F_EventScript_CreateAbnormalWeather
	specialvar VAR_RESULT, GetAbnormalWeatherMapNameAndType
	goto_if_eq VAR_RESULT, 1, Route119_WeatherInstitute_2F_EventScript_KyogreWeather
	msgbox Route119_WeatherInstitute_2F_Text_GroudonWeather, MSGBOX_DEFAULT
	release
	end

Route119_WeatherInstitute_2F_EventScript_KyogreWeather::
	msgbox Route119_WeatherInstitute_2F_Text_KyogreWeather, MSGBOX_DEFAULT
	release
	end

Route119_WeatherInstitute_2F_EventScript_CreateAbnormalWeather::
	special CreateAbnormalWeatherEvent
	setflag FLAG_TEMP_2
	return

Route119_WeatherInstitute_2F_EventScript_LegendaryDefeated::
	addvar VAR_0x8004, 1
	return

Route119_WeatherInstitute_2F_EventScript_NoAbnormalWeather::
	msgbox Route119_WeatherInstitute_2F_Text_NoAbnormalWeather, MSGBOX_DEFAULT
	release
	end

Route119_WeatherInstitute_2F_Movement_GruntApproachShelly:
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	step_end

Route119_WeatherInstitute_2F_Movement_ShovePlayerOutOfWay:
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	ride_water_current_up
	walk_in_place_faster_down
	step_end

Route119_WeatherInstitute_2F_Movement_PlayerReturnToPosition:
	slide_down
	walk_in_place_faster_left
	step_end

Route119_WeatherInstitute_2F_Movement_ScientistApproachPlayer:
	walk_right
	walk_right
	walk_right
	step_end

Route119_WeatherInstitute_2F_EventScript_WeatherScientist::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_CASTFORM, Route119_WeatherInstitute_2F_EventScript_ScientistMentionWeather
	goto Route119_WeatherInstitute_2F_EventScript_ReceiveCastform
	end

Route119_WeatherInstitute_2F_Text_Grunt2Intro:
	.string "The INSTITUTE created a POKéMON\n"
	.string "that has something to do with\l"
	.string "the weather. We're here to take it!$"

Route119_WeatherInstitute_2F_Text_Grunt2Defeat:
	.string "Our plan's being spoiled by a kid?$"

Route119_WeatherInstitute_2F_Text_Grunt2PostBattle:
	.string "If the POKéMON here can alter weather,\n"
	.string "TEAM ROCKET has the perfect use\l"
	.string "for something like that!$"

Route119_WeatherInstitute_2F_Text_Grunt3Intro:
	.string "TEAM ROCKET!\n"
	.string "We appear wherever power is\l"
	.string "waiting to be taken!$"

Route119_WeatherInstitute_2F_Text_Grunt3Defeat:
	.string "You got me!$"

Route119_WeatherInstitute_2F_Text_Grunt3PostBattle:
	.string "You don't have any idea what\n"
	.string "TEAM ROCKET is working towards!\l"
	.string "Stay out of our way!$"

Route119_WeatherInstitute_2F_Text_Grunt5Intro:
	.string "Don't tell me you're looking for\n"
	.string "that weather POKéMON, too?\p"
	.string "That's a no-no!\n"
	.string "TEAM ROCKET claimed it first!$"

Route119_WeatherInstitute_2F_Text_Grunt5Defeat:
	.string "Oh, will you look at my POKéMON?$"

Route119_WeatherInstitute_2F_Text_Grunt5PostBattle:
	.string "Humph. So what?\n"
	.string "What we want…\p"
	.string "What we really want isn't here…\n"
	.string "The BOSS will deal with this himself!$"

Route119_WeatherInstitute_2F_Text_ShellyIntro:
	.string "A TRAINER in our way?\p"
	.string "How boring.\p"
	.string "TEAM ROCKET doesn't answer to\n"
	.string "children. Stand aside - or battle!$"

Route119_WeatherInstitute_2F_Text_ShellyDefeat:
	.string "You- you're not nothing, are you?\p"
	.string "…The BOSS will hear of this.$"

Route119_WeatherInstitute_2F_Text_ShellyPostBattle:
	.string "Che.\p"
	.string "The HOENN DEFENDERS have been\n"
	.string "a thorn in our plans all along.\p"
	.string "And now some TRAINER shows up\n"
	.string "and beats me here, too?!\p"
	.string "This isn't over.$"

Route119_WeatherInstitute_2F_Text_TeamMagmaJustPassedBy:
	.string "Emergency report!\p"
	.string "A TEAM ROCKET squad just passed\n"
	.string "the WEATHER INSTITUTE heading west!\p"
	.string "They appear to be headed for\n"
	.string "MT. PYRE!$"

Route119_WeatherInstitute_2F_Text_WeHaveToHurryToMtPyre:
	.string "What?!\p"
	.string "MT. PYRE?\n"
	.string "The ORBS are kept there!\p"
	.string "TEAM ROCKET - retreat!\n"
	.string "We move now!$"

Route119_WeatherInstitute_2F_Text_ScientistFoundTempestOrb:
	.string "RESEARCHER: Thank goodness!\n"
	.string "TEAM ROCKET is gone!\p"
	.string "While they were ransacking the lab,\n"
	.string "one of them dropped this orb.\p"
	.string "It was crackling with static\n"
	.string "electricity when I found it.\p"
	.string "I don't know what it is, but it\n"
	.string "feels like it belongs with you.$"

Route119_WeatherInstitute_2F_Text_ThanksPleaseTakePokemon:
	.string "Thanks!\n"
	.string "Thanks to you, we're safe!\p"
	.string "It might be an odd way of thanking you,\n"
	.string "but take this POKéMON.$"

Route119_WeatherInstitute_2F_Text_ThanksPleaseTakePokemonOP:
	.string "Here take this POKéMON with you!$"

@ Unused
Route119_WeatherInstitute_2F_Text_NoRoomForPokemon:
	.string "Hm? You don't seem to have any room\n"
	.string "for this POKéMON.$"

Route119_WeatherInstitute_2F_Text_PlayerReceivedCastform:
	.string "{PLAYER} received CASTFORM!$"

Route119_WeatherInstitute_2F_Text_PokemonChangesWithWeather:
	.string "That POKéMON changes shape according\n"
	.string "to the weather conditions.\p"
	.string "There're plenty of them in the\n"
	.string "INSTITUTE--go ahead and take it.$"

Route119_WeatherInstitute_2F_Text_ChangingWeatherRidiculous:
	.string "I've been researching rain for many\n"
	.string "years, but it's ridiculous to think that\l"
	.string "humans can freely change the weather.$"

Route119_WeatherInstitute_2F_Text_GroudonWeather:
	.string "I track weather patterns over\n"
	.string "the HOENN region.\p"
	.string "Presently, extreme volcanic heat\n"
	.string "and ashfall in {STR_VAR_1}.\p"
	.string "Something powerful is near\n"
	.string "{STR_VAR_1}…$"

Route119_WeatherInstitute_2F_Text_KyogreWeather:
	.string "I track weather patterns over\n"
	.string "the HOENN region.\p"
	.string "Presently, a severe blizzard has\n"
	.string "been recorded over {STR_VAR_1}.\p"
	.string "Something powerful is near\n"
	.string "{STR_VAR_1}…$"

Route119_WeatherInstitute_2F_Text_NoAbnormalWeather:
	.string "Abnormal weather conditions are\n"
	.string "no longer being reported.\p"
	.string "The occasional rainfall is a blessing,\n"
	.string "really.$"

