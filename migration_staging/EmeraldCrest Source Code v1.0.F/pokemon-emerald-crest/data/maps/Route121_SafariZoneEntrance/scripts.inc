Route121_SafariZoneEntrance_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Route121_SafariZoneEntrance_OnFrame
	.byte 0

Route121_SafariZoneEntrance_OnFrame:
	map_script_2 VAR_SAFARI_ZONE_STATE, 1, Route121_SafariZoneEntrance_EventScript_ExitSafariZone
	map_script_2 VAR_SAFARI_ZONE_STATE, 3, Route121_SafariZoneEntrance_OnLoad
	.2byte 0

Route121_SafariZoneEntrance_OnLoad:
	turnobject OBJ_EVENT_ID_PLAYER, DIR_NORTH
	callnative ReturnSpotlightSpecies	
	goto_if_set FLAG_IS_SPOTLIGHT_EVENT, EventScript_end
	end

EventScript_end:
	setvar VAR_TEMP_2, 2
	setvar VAR_SAFARI_ZONE_STATE, 0
	releaseall
	end

Route121_SafariZoneEntrance_EventScript_ExitSafariZone::
	lockall
	applymovement OBJ_EVENT_ID_PLAYER, Route121_SafariZoneEntrance_Movement_ExitSafariZone
	waitmovement 0
	setvar VAR_SAFARI_ZONE_STATE, 0
	clearflag FLAG_GENERATED_SPOTLIGHT_EVENT
	goto_if_eq VAR_UNUSED_0x40A8, 4, EventScript_SpotlightEventEnded
	callnative ReturnSpotlightSpecies	
	goto_if_set FLAG_IS_SPOTLIGHT_EVENT, EventScript_end
	releaseall
	end

Route121_SafariZoneEntrance_Movement_ExitSafariZone:
	walk_up
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

Route121_SafariZoneEntrance_EventScript_WelcomeAttendant::
	goto_if_quest_complete QUEST_4, EventScript_RewardQ4
    goto_if_quest_active QUEST_4, EventScript_ChecksQ4
	goto_if_set FLAG_BADGE07_GET, EventScript_Quest4
	msgbox Route121_SafariZoneEntrance_Text_WelcomeToSafariZone, MSGBOX_NPC
	end

EventScript_welcomeZone::
	msgbox Route121_SafariZoneEntrance_Text_WelcomeToSafariZone, MSGBOX_NPC
	end

EventScript_Quest4::
	lock
	msgbox HelpSafariZoneAtNight MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, EventScript_StartQuest4
	release
	end

EventScript_EndQ4::
	msgbox Route121_SafariZoneEntrance_Text_WelcomeToSafariZone, MSGBOX_NPC
	end

EventScript_StartQuest4::
	msgbox Text_AcceptThanksQ4, MSGBOX_DEFAULT
	startquest QUEST_4
	openquestmenu
	release
	end

EventScript_ChecksQ4::
	msgbox Text_AcceptThanksQ4, MSGBOX_DEFAULT
	release
	end

EventScript_RewardQ4::
	goto_if_set FLAG_RECEIVED_OLD_SEA_MAP, EventScript_welcomeZone
	msgbox Text_Thxq4, MSGBOX_DEFAULT
	giveitem ITEM_OLD_SEA_MAP
	setflag FLAG_RECEIVED_OLD_SEA_MAP
	setflag FLAG_ENABLE_SHIP_FARAWAY_ISLAND
	release
	end

Roaming_Pokemon_Quest::
    lockall
	call Eventscript_TryGenerateShinyMon
	faceplayer
	setflag FLAG_NO_ESCAPE
	playmoncry SPECIES_LYCANROC_MIDNIGHT, CRY_MODE_ENCOUNTER
    waitmoncry
	setwildbattle SPECIES_LYCANROC_MIDNIGHT, 46
	dowildbattle
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, Eventscript_WonQuest
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, Eventscript_EscapedQuest
	goto_if_eq VAR_RESULT, B_OUTCOME_MON_FLED, Eventscript_EscapedQuest
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, Eventscript_WonQuest
	goto_if_eq VAR_RESULT, B_OUTCOME_LOST, Eventscript_EscapedQuest
	goto_if_eq VAR_RESULT, B_OUTCOME_DREW, Eventscript_EscapedQuest
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, Eventscript_EscapedQuest
	clearflag FLAG_NO_ESCAPE
	releaseall
    end


Eventscript_WonQuest::
  	setflag FLAG_ROAMING_QUEST
	removeobject VAR_LAST_TALKED
	clearflag FLAG_NO_ESCAPE
	completequest QUEST_4
	clearflag OW_FLAG_NO_ENCOUNTER
	clearflag FLAG_SYS_SAFARI_MODE
	warp MAP_ROUTE121_SAFARI_ZONE_ENTRANCE, 9, 4
	addrankpoints 10
	releaseall
	end

Eventscript_EscapedQuest::
    clearflag FLAG_NO_ESCAPE
    releaseall
    end

Route121_SafariZoneEntrance_EventScript_InfoAttendant::
	lock
	faceplayer
	msgbox Route121_SafariZoneEntrance_Text_WelcomeFirstTime, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Route121_SafariZoneEntrance_EventScript_FirstTimeInfo
	msgbox Route121_SafariZoneEntrance_Text_ComeInAndEnjoy, MSGBOX_DEFAULT
	release
	end

Route121_SafariZoneEntrance_EventScript_FirstTimeInfo::
	msgbox Route121_SafariZoneEntrance_Text_FirstTimeInfo, MSGBOX_DEFAULT
	release
	end

Route121_SafariZoneEntrance_EventScript_EntranceCounterTrigger::
	lockall
	goto_if_eq VAR_UNUSED_0x40A8, 4, EventScript_SpotlightEventEnded
	setflag FLAG_ROAMING_QUEST
	goto_if_quest_complete QUEST_4, EventScript_TriggerZone
	goto_if_quest_active QUEST_4, EventScript_ProceedQuest4
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	showmoneybox 0, 0
	goto_if_set FLAG_IS_SPOTLIGHT_EVENT Route121_SafariZoneEntrance_EventScript_EntranceCounterTrigger_SpotlightEvent
	msgbox Text_CreateSpotlightEvent, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, EventScript_Returns
	fadescreen FADE_TO_BLACK
	callnative DoSpotlightNamingScreen
	fadescreen FADE_FROM_BLACK
	goto_if_unset FLAG_GENERATED_SPOTLIGHT_EVENT, EventScript_CodeNotFound
	goto Route121_SafariZoneEntrance_EventScript_TryEnterSafariZone
	end

EventScript_SpotlightEventEnded::
	turnobject OBJ_EVENT_ID_PLAYER, DIR_NORTH
	msgbox Text_SpotlightEventIsOver, MSGBOX_DEFAULT
	setvar VAR_TEMP_2, 2
	releaseall
	end

EventScript_TriggerZone::
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	showmoneybox 0, 0
	goto_if_set FLAG_IS_SPOTLIGHT_EVENT Route121_SafariZoneEntrance_EventScript_EntranceCounterTrigger_SpotlightEvent
	msgbox Text_CreateSpotlightEvent, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, EventScript_Returns
	fadescreen FADE_TO_BLACK
	callnative DoSpotlightNamingScreen
	fadescreen FADE_FROM_BLACK
	goto_if_unset FLAG_GENERATED_SPOTLIGHT_EVENT, EventScript_CodeNotFound
	goto Route121_SafariZoneEntrance_EventScript_TryEnterSafariZone
	end

Route121_SafariZoneEntrance_EventScript_EntranceCounterTrigger_SpotlightEvent::
	msgbox Route121_SafariZoneEntrance_Text_WouldYouLikeToPlay_SpotlightEvent, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Route121_SafariZoneEntrance_EventScript_TryEnterSafariZone
	msgbox Text_CreateSpotlightEvent, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, EventScript_Returns
	fadescreen FADE_TO_BLACK
	callnative DoSpotlightNamingScreen
	fadescreen FADE_FROM_BLACK
	goto_if_unset FLAG_GENERATED_SPOTLIGHT_EVENT, EventScript_CodeNotFound
	goto Route121_SafariZoneEntrance_EventScript_TryEnterSafariZone
	end

EventScript_CodeNotFound:
	msgbox Text_PleaseEnterCodes, MSGBOX_DEFAULT
	goto Route121_SafariZoneEntrance_EventScript_MovePlayerBackFromCounter
	end

EventScript_Returns:
	msgbox Route121_SafariZoneEntrance_Text_WouldYouLikeToPlay, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Route121_SafariZoneEntrance_EventScript_TryEnterSafariZone
	msgbox Route121_SafariZoneEntrance_Text_PlayAnotherTime, MSGBOX_DEFAULT
	goto Route121_SafariZoneEntrance_EventScript_MovePlayerBackFromCounter
	end

Text_PleaseEnterCodes:
	.string "CODE NOT VALID...\n"
	.string "visit Rom Hack Studios for vaild codes\p"
	.string "Link: romhackstudios.github.io$"

Text_CreateSpotlightEvent:
	.string "Would you like to use a ticket id\n"
	.string "to host a spotlight event?$"

EventScript_ProceedQuest4::
	clearflag FLAG_ROAMING_QUEST
	setflag OW_FLAG_NO_ENCOUNTER
	setflag FLAG_SYS_SAFARI_MODE
	applymovement OBJ_EVENT_ID_PLAYER, Route121_SafariZoneEntrance_Movement_EnterSafariZone
	waitmovement 0
	setvar VAR_SAFARI_ZONE_STATE, 2
	warp MAP_SAFARI_ZONE_SOUTH, 32, 33
	waitstate
	end

Route121_SafariZoneEntrance_EventScript_TryEnterSafariZone::
	clearflag OW_FLAG_NO_ENCOUNTER
	call Route121_SafariZoneEntrance_EventScript_CheckHasRoomForPokemon
	goto_if_set FLAG_GENERATED_SPOTLIGHT_EVENT, Route121_SafariZoneEntrance_EventScript_SpotlightEvent
	callnative ReturnSpotlightSpecies
	goto_if_set FLAG_IS_SPOTLIGHT_EVENT, Route121_SafariZoneEntrance_EventScript_SpotlightEvent 
	checkmoney 500
	goto_if_eq VAR_RESULT, 0, Route121_SafariZoneEntrance_EventScript_NotEnoughMoney
	playse SE_SHOP
	msgbox Route121_SafariZoneEntrance_Text_ThatWillBe500Please, MSGBOX_DEFAULT
	removemoney 500
	updatemoneybox
	msgbox Route121_SafariZoneEntrance_Text_HereAreYourSafariBalls, MSGBOX_DEFAULT
	playfanfare MUS_OBTAIN_ITEM
	message Route121_SafariZoneEntrance_Text_Received30SafariBalls
	waitfanfare
	msgbox Route121_SafariZoneEntrance_Text_PleaseEnjoyYourself, MSGBOX_DEFAULT
	closemessage
	hidemoneybox
	applymovement OBJ_EVENT_ID_PLAYER, Route121_SafariZoneEntrance_Movement_EnterSafariZone
	waitmovement 0
	special EnterSafariMode
	setvar VAR_SAFARI_ZONE_STATE, 2
	clearflag FLAG_GOOD_LUCK_SAFARI_ZONE
	warp MAP_SAFARI_ZONE_SOUTH, 32, 33
	waitstate
	end

Route121_SafariZoneEntrance_EventScript_SpotlightEvent::
	checkmoney 3000
	goto_if_eq VAR_RESULT, 0, Route121_SafariZoneEntrance_EventScript_NotEnoughMoney
	playse SE_SHOP
	msgbox Route121_SafariZoneEntrance_Text_ThatWillBe3000Please, MSGBOX_DEFAULT
	removemoney 3000
	updatemoneybox
	msgbox Route121_SafariZoneEntrance_Text_HereAreYourSafariBalls, MSGBOX_DEFAULT
	playfanfare MUS_OBTAIN_ITEM
	message Route121_SafariZoneEntrance_Text_Received99SafariBalls
	waitfanfare
	msgbox Route121_SafariZoneEntrance_Text_PleaseEnjoyYourself, MSGBOX_DEFAULT
	closemessage
	hidemoneybox
	applymovement OBJ_EVENT_ID_PLAYER, Route121_SafariZoneEntrance_Movement_EnterSafariZone
	waitmovement 0
	special EnterSafariMode
	setvar VAR_SAFARI_ZONE_STATE, 2
	clearflag FLAG_GOOD_LUCK_SAFARI_ZONE
	warp MAP_SAFARI_ZONE_SOUTH, 32, 33
	waitstate
	end

Route121_SafariZoneEntrance_EventScript_WantToExit::
	lockall
	turnobject OBJ_EVENT_ID_PLAYER, DIR_NORTH
	msgbox Text_WantToExit, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, EventScript_ExitSpotlightEvent
	applymovement OBJ_EVENT_ID_PLAYER, Movement_PlayerStepBack
	waitmovement 0
	releaseall
	end

Movement_PlayerStepBack:
	walk_left
	step_end

EventScript_ExitSpotlightEvent::
	setvar VAR_SAFARI_ZONE_STATE, 0
	callnative SetCB2WhiteOut2 
	releaseall
	end

Route121_SafariZoneEntrance_EventScript_CheckHasRoomForPokemon::
	getpartysize
	goto_if_ne VAR_RESULT, PARTY_SIZE, Route121_SafariZoneEntrance_EventScript_HasRoomForPokemon
	specialvar VAR_RESULT, ScriptCheckFreePokemonStorageSpace
	goto_if_eq VAR_RESULT, 1, Route121_SafariZoneEntrance_EventScript_HasRoomForPokemon
	msgbox Route121_SafariZoneEntrance_Text_PCIsFull, MSGBOX_DEFAULT
	goto Route121_SafariZoneEntrance_EventScript_MovePlayerBackFromCounter
	end

Route121_SafariZoneEntrance_EventScript_HasRoomForPokemon::
	return

Route121_SafariZoneEntrance_EventScript_NoPokeblockCase::
	msgbox Route121_SafariZoneEntrance_Text_YouNeedPokeblockCase, MSGBOX_DEFAULT
	goto Route121_SafariZoneEntrance_EventScript_MovePlayerBackFromCounter
	end

Route121_SafariZoneEntrance_EventScript_NotEnoughMoney::
	msgbox Route121_SafariZoneEntrance_Text_NotEnoughMoney, MSGBOX_DEFAULT
	goto Route121_SafariZoneEntrance_EventScript_MovePlayerBackFromCounter
	end

Route121_SafariZoneEntrance_EventScript_MovePlayerBackFromCounter::
	closemessage
	hidemoneybox
	applymovement OBJ_EVENT_ID_PLAYER, Route121_SafariZoneEntrance_Movement_BackAwayFromCounter
	waitmovement 0
	releaseall
	end

Route121_SafariZoneEntrance_Movement_BackAwayFromCounter:
	walk_right
	step_end

Route121_SafariZoneEntrance_Movement_EnterSafariZone:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	delay_16
	step_end

Route121_SafariZoneEntrance_EventScript_TrainerTipSign::
	msgbox Route121_SafariZoneEntrance_Text_TrainerTip, MSGBOX_SIGN
	end

HelpSafariZoneAtNight:
	.string "Oh my goodness, you must be pretty \n"
	.string "strong with all those badges.\p"
	.string "Can you fend off the wild Pokémon that\n"
	.string "is causing chaos in the safari zone?$"

Text_AcceptThanksQ4:
	.string "Thank you so much.\p"
	.string "Go ahead towards the zone entrance, you\n"
	.string "won't be charged this time.\p"
	.string "We've moved all the wild Pokémon to\n"
	.string "a much safer location, so that you won't\l"
	.string "be bothered during your battle.\p"
	.string "You can find the strong Pokémon at\n"
	.string "the north west side of the safari zone.$"

Text_Thxq4:
	.string "You did it, you are quite powerful.\p"
	.string "Thanks to you, we're safe!\p"
	.string "It's not worth much but you could frame\n"
	.string "it somewhere, here take this.$"

Text_WantToExit:
	.string "Would you like to go back?$"

Text_SpotlightEventIsOver:
	.string "Thank you for playing!\p"
	.string "The current spotlight event ended\n"
	.string "come back later for more.$"
