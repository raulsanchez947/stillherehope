.set LOCALID_MART_EMPLOYEE, 2
.set LOCALID_FOOTPRINTS_MAN, 3
.set LOCALID_RIVAL, 4

OldaleTown_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, OldaleTown_OnTransition
	.byte 0

OldaleTown_OnTransition:
	call Common_EventScript_SetupRivalGfxId
	setflag FLAG_VISITED_OLDALE_TOWN
	setflag FLAG_VISITED_ROUTE101
	call_if_unset FLAG_ADVENTURE_STARTED, OldaleTown_EventScript_BlockWestEntrance
	call_if_unset FLAG_RECEIVED_POTION_OLDALE, OldaleTown_EventScript_MoveMartEmployee
	call_if_set FLAG_ADVENTURE_STARTED, OldaleTown_EventScript_SetOldaleState
	end

@ This script seems pointless because nothing uses VAR_OLDALE_TOWN_STATE
OldaleTown_EventScript_SetOldaleState::
	setvar VAR_OLDALE_TOWN_STATE, 1
	return

OldaleTown_EventScript_BlockWestEntrance::
	setobjectxyperm LOCALID_FOOTPRINTS_MAN, 1, 11
	setobjectmovementtype LOCALID_FOOTPRINTS_MAN, MOVEMENT_TYPE_FACE_LEFT
	return

OldaleTown_EventScript_MoveMartEmployee::
	setobjectxyperm LOCALID_MART_EMPLOYEE, 13, 14
	setobjectmovementtype LOCALID_MART_EMPLOYEE, MOVEMENT_TYPE_FACE_DOWN
	return

OldaleTown_EventScript_TownSign::
	msgbox OldaleTown_Text_TownSign, MSGBOX_SIGN
	end

OldaleTown_EventScript_Girl::
	msgbox OldaleTown_Text_SavingMyProgress, MSGBOX_NPC
	end

OldaleTown_EventScript_MartEmployee::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_POTION_OLDALE, OldaleTown_EventScript_ExplainPotion
	goto_if_set FLAG_TEMP_1, OldaleTown_EventScript_ExplainPotion
	setflag FLAG_TEMP_1
	playbgm MUS_FOLLOW_ME, FALSE
	msgbox OldaleTown_Text_IWorkAtPokemonMart, MSGBOX_DEFAULT
	closemessage
	switch VAR_FACING
	case DIR_SOUTH, OldaleTown_EventScript_GoToMartSouth
	case DIR_NORTH, OldaleTown_EventScript_GoToMartNorth
	case DIR_EAST, OldaleTown_EventScript_GoToMartEast
	end

OldaleTown_EventScript_GoToMartSouth::
	applymovement LOCALID_MART_EMPLOYEE, OldaleTown_Movement_EmployeeSouth
	applymovement OBJ_EVENT_ID_PLAYER, OldaleTown_Movement_PlayerSouth
	waitmovement 0
	goto OldaleTown_EventScript_ExplainPokemonMart
	end

OldaleTown_EventScript_GoToMartNorth::
	applymovement LOCALID_MART_EMPLOYEE, OldaleTown_Movement_EmployeeNorth
	applymovement OBJ_EVENT_ID_PLAYER, OldaleTown_Movement_PlayerNorth
	waitmovement 0
	goto OldaleTown_EventScript_ExplainPokemonMart
	end

OldaleTown_EventScript_GoToMartEast::
	applymovement OBJ_EVENT_ID_PLAYER, OldaleTown_Movement_PlayerEast
	applymovement LOCALID_MART_EMPLOYEE, OldaleTown_Movement_EmployeeEast
	waitmovement 0
	goto OldaleTown_EventScript_ExplainPokemonMart
	end

OldaleTown_EventScript_ExplainPokemonMart::
	msgbox OldaleTown_Text_ThisIsAPokemonMart, MSGBOX_DEFAULT
	giveitem ITEM_POTION
	goto_if_eq VAR_RESULT, FALSE, OldaleTown_EventScript_BagIsFull
	msgbox OldaleTown_Text_PotionExplanation, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_POTION_OLDALE
	fadedefaultbgm
	release
	end

OldaleTown_EventScript_ExplainPotion::
	msgbox OldaleTown_Text_PotionExplanation, MSGBOX_DEFAULT
	release
	end

OldaleTown_EventScript_BagIsFull::
	msgbox gText_TooBadBagIsFull, MSGBOX_DEFAULT
	fadedefaultbgm
	release
	end

OldaleTown_Movement_EmployeeEast:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_in_place_faster_down
	step_end

OldaleTown_Movement_EmployeeSouth:
	walk_left
	walk_up
	walk_up
	walk_right
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_in_place_faster_down
	step_end

OldaleTown_Movement_EmployeeNorth:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_in_place_faster_down
	step_end

OldaleTown_Movement_Unknown1:
	walk_up
	walk_up
	walk_right
	walk_right
	walk_right
	walk_right
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	delay_8
	walk_in_place_faster_down
	step_end

OldaleTown_Movement_PlayerEast:
	walk_right
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

OldaleTown_Movement_PlayerSouth:
	delay_16
	delay_16
	delay_16
	delay_16
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

OldaleTown_Movement_PlayerNorth:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

OldaleTown_Movement_Unknown2:
	walk_left
	walk_up
	walk_up
	walk_right
	walk_right
	walk_right
	walk_right
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

OldaleTown_EventScript_FootprintsMan::
	lock
	faceplayer
	goto_if_quest_complete QUEST_1, EventScript_Thxq
	goto_if_quest_active QUEST_1, EventScript_ChecksQuest1
	goto_if_set FLAG_ADVENTURE_STARTED, OldaleTown_EventScript_NotBlockingPath
	msgbox OldaleTown_Text_DiscoveredFootprints, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_FOOTPRINTS_MAN, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

EventScript_ChecksQuest1::
    msgbox Text_DidYouGetTheRareMon, MSGBOX_DEFAULT
    special ChoosePartyMon
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, Eventscript_Incorrect
	specialvar VAR_RESULT, ScriptGetPartyMonSpecies
	goto_if_eq VAR_RESULT, SPECIES_EGG, Eventscript_Incorrect
	goto_if_eq VAR_RESULT, SPECIES_MUDKIP, EventScript_Thxq1
	goto Eventscript_Incorrect
	release
	end

Eventscript_Incorrect::
    msgbox Text_Thatisnotthemon, MSGBOX_DEFAULT
	release
	end

EventScript_Reward::
	msgbox Text_Rewardq1, MSGBOX_DEFAULT
    giveitem ITEM_SWAMPERTITE
	call_if_eq VAR_RESULT, FALSE, Common_EventScript_BagIsFullQuest
    completequest QUEST_1
	addrankpoints 10
	release
	end

Common_EventScript_BagIsFullQuest::
	msgbox gText_TooBadBagIsFull, MSGBOX_DEFAULT
	release
	end

EventScript_Thxq1::
    msgbox Text_Txq1, MSGBOX_DEFAULT
	goto EventScript_Reward
	release
	end

EventScript_Thxq::
    msgbox Text_Txq, MSGBOX_DEFAULT
	release
	end

OldaleTown_EventScript_BlockedPath::
	lockall
	applymovement OBJ_EVENT_ID_PLAYER, OldaleTown_Movement_PlayerStepBack
	applymovement LOCALID_FOOTPRINTS_MAN, OldaleTown_Movement_BackUp
	waitmovement 0
	msgbox OldaleTown_Text_WaitDontComeInHere, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_FOOTPRINTS_MAN, OldaleTown_Movement_ReturnToOriginalPosition
	waitmovement 0
	releaseall
	end

OldaleTown_EventScript_NotBlockingPath::
	msgbox OldaleTown_Text_FinishedSketchingFootprints, MSGBOX_DEFAULT
	goto_if_quest_not_active QUEST_1, EventScript_Quest1
	release
	end

EventScript_Quest1::
	msgbox Text_WillYouHelMeFindIt, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, EventScript_StartQuest1
	msgbox Text_ThatsOkay, MSGBOX_DEFAULT
	release
	end

EventScript_StartQuest1::
	questmenu QUEST_MENU_SET_ACTIVE QUEST_1
	startquest QUEST_1
	openquestmenu
	release
	end

OldaleTown_EventScript_Rival::
	lockall
	applymovement LOCALID_RIVAL, Common_Movement_FacePlayer
	waitmovement 0
	setvar VAR_0x8009, 0
	goto OldaleTown_EventScript_ShowRivalMessage
	end

OldaleTown_EventScript_RivalTrigger1::
	lockall
	applymovement LOCALID_RIVAL, OldaleTown_Movement_RivalApproachPlayer1
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	setvar VAR_0x8009, 1
	goto OldaleTown_EventScript_ShowRivalMessage
	end

OldaleTown_EventScript_RivalTrigger2::
	lockall
	applymovement LOCALID_RIVAL, OldaleTown_Movement_RivalApproachPlayer2
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	setvar VAR_0x8009, 1
	goto OldaleTown_EventScript_ShowRivalMessage
	end

OldaleTown_EventScript_RivalTrigger3::
	lockall
	applymovement LOCALID_RIVAL, OldaleTown_Movement_RivalApproachPlayer3
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	setvar VAR_0x8009, 1
	goto OldaleTown_EventScript_ShowRivalMessage
	end

OldaleTown_EventScript_ShowRivalMessage::
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, OldaleTown_EventScript_ShowMayMessage
	goto_if_eq VAR_RESULT, FEMALE, OldaleTown_EventScript_ShowBrendanMessage
	end

OldaleTown_EventScript_ShowMayMessage::
	msgbox OldaleTown_Text_MayLetsGoBack, MSGBOX_DEFAULT
	goto OldaleTown_EventScript_RivalFinish
	end

OldaleTown_EventScript_ShowBrendanMessage::
	msgbox OldaleTown_Text_BrendanLetsGoBack, MSGBOX_DEFAULT
	goto OldaleTown_EventScript_RivalFinish
	end

OldaleTown_EventScript_RivalFinish::
	closemessage
	call_if_eq VAR_0x8009, 0, OldaleTown_EventScript_DoExitMovement1
	call_if_eq VAR_0x8009, 1, OldaleTown_EventScript_DoExitMovement2
	applymovement LOCALID_RIVAL, OldaleTown_Movement_RivalExit
	waitmovement 0
	removeobject LOCALID_RIVAL
	setvar VAR_OLDALE_RIVAL_STATE, 2
	setflag FLAG_HIDE_OLDALE_TOWN_RIVAL
	releaseall
	end

OldaleTown_EventScript_DoExitMovement1::
	goto_if_ne VAR_FACING, DIR_SOUTH, OldaleTown_EventScript_DoExitMovement2
	applymovement LOCALID_RIVAL, OldaleTown_Movement_RivalExit
	waitmovement 0
	return

OldaleTown_EventScript_DoExitMovement2::
	applymovement OBJ_EVENT_ID_PLAYER, OldaleTown_Movement_WatchRivalExit
	applymovement LOCALID_RIVAL, OldaleTown_Movement_RivalExit
	waitmovement 0
	return

OldaleTown_Movement_RivalApproachPlayer1:
	walk_left
	walk_left
	step_end

OldaleTown_Movement_RivalApproachPlayer2:
	walk_left
	step_end

OldaleTown_Movement_RivalApproachPlayer3:
	face_left
	step_end

OldaleTown_Movement_RivalExit:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

OldaleTown_Movement_WatchRivalExit:
	delay_8
	delay_4
	walk_in_place_faster_down
	step_end

OldaleTown_Movement_PlayerStepBack:
	delay_8
	walk_right
	step_end

OldaleTown_Movement_BackUp:
	walk_fast_up
	walk_in_place_faster_left
	lock_facing_direction
	walk_right
	unlock_facing_direction
	step_end

OldaleTown_Movement_ReturnToOriginalPosition:
	walk_down
	walk_left
	step_end

OldaleTown_Text_SavingMyProgress:
	.string "I want to take a rest, so I'm saving my\n"
	.string "progress.$"

OldaleTown_Text_IWorkAtPokemonMart:
	.string "Hi!\n"
	.string "I work at a POKéMON MART.\p"
	.string "Can I get you to come with me?$"

OldaleTown_Text_ThisIsAPokemonMart:
	.string "This is a POKéMON MART.\n"
	.string "Just look for our blue roof.\p"
	.string "We sell a variety of goods including\n"
	.string "POKé BALLS for catching POKéMON.\p"
	.string "Here, I'd like you to have this as\n"
	.string "a promotional item.$"

OldaleTown_Text_PotionExplanation:
	.string "A POTION can be used anytime, so it's\n"
	.string "even more useful than a POKéMON CENTER\l"
	.string "in certain situations.$"

OldaleTown_Text_WaitDontComeInHere:
	.string "Aaaaah! Wait!\n"
	.string "Please don't come in here.\p"
	.string "I just discovered the footprints of\n"
	.string "a rare POKéMON!\p"
	.string "Wait until I finish sketching\n"
	.string "them, okay?$"

OldaleTown_Text_DiscoveredFootprints:
	.string "I just discovered the footprints of\n"
	.string "a rare POKéMON!\p"
	.string "Wait until I finish sketching\n"
	.string "them, okay?$"

OldaleTown_Text_FinishedSketchingFootprints:
	.string "I finished sketching the footprints of\n"
	.string "a rare POKéMON.\p"
	.string "But it turns out they were only my\n"
	.string "own footprints…$"

OldaleTown_Text_MayLetsGoBack:
	.string "MAY: {PLAYER}{KUN}!\n"
	.string "Over here!\l"
	.string "Let's hurry home!$"

OldaleTown_Text_BrendanLetsGoBack:
	.string "BRENDAN: I'm heading back to my dad's\n"
	.string "LAB now.\l"
	.string "{PLAYER}, you should hurry back too.$"

OldaleTown_Text_TownSign:
	.string "OLDALE TOWN\n"
	.string "“Where things start off scarce.”$"


Text_DidYouGetTheRareMon:
    .string "Did you find the rare POKéMON?\n"
	.string "Let me take a look at it.$"

Text_WillYouHelMeFindIt:
    .string "You are a Trainer right?\p"
	.string "Will you help me find this POKéMON called\n"
	.string "MUDKIP? It's very rare here$"

Text_ThatsOkay:
	.string "Oh, you won't?\n"
	.string "Well, that's fine, I'll keep looking.$"

Text_Thatisnotthemon:
    .string "That's definitely not a MUDKIP.$"

Text_Txq1:
    .string "Oh wow!, so this a MUDKIP, it's a\n"
	.string "very rare POKéMON in the HOENN region.\p"
	.string "Thank you for helping me find it.$"

Text_Txq:
    .string "Mudkip may be small, but it's definitely\n"
	.string "a force to be reckoned with in battles.$"

Text_Rewardq1:
	.string "Here have this stone, I found it while\n"
	.string "I was sketching the footprints earlier.$"
