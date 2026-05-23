.set LOCALID_AIDE, 1
.set LOCALID_BIRCH, 2
.set LOCALID_RIVAL, 3
.set LOCALID_BALL_CYNDAQUIL, 4
.set LOCALID_BALL_TOTODILE, 5
.set LOCALID_BALL_CHIKORITA, 6

LittlerootTown_ProfessorBirchsLab_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, LittlerootTown_ProfessorBirchsLab_OnTransition
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, LittlerootTown_ProfessorBirchsLab_OnWarp
	map_script MAP_SCRIPT_ON_FRAME_TABLE, LittlerootTown_ProfessorBirchsLab_OnFrame
	.byte 0

	@ State descriptions for VAR_DEX_UPGRADE_JOHTO_STARTER_STATE
	@ 1: Beat Elite Four, Dex upgrade ready
	@ 2: Received National Dex
	@ 3: Left lab after receiving National Dex
	@ 4: Entered lab after catching all Hoenn mons
	@ 5: Birch told player to choose Johto starter
	@ 6: Chose Johto starter

	@ State descriptions for VAR_BIRCH_LAB_STATE
	@ 1: Never occurs
	@ 2: Chose starter
	@ 3: Received starter in lab
	@ 4: Defeated rival on Route 103
	@ 5: Received pokedex

LittlerootTown_ProfessorBirchsLab_OnTransition:
	call Common_EventScript_SetupRivalGfxId
	call ProfBirch_EventScript_UpdateLocation
	goto_if_ge VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 6, LittlerootTown_ProfessorBirchsLab_EventScript_SetAfterJohtoStarterLayout
	goto_if_ge VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 4, LittlerootTown_ProfessorBirchsLab_EventScript_SetJohtoStarterLayout
	goto_if_eq VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 3, LittlerootTown_ProfessorBirchsLab_EventScript_CheckReadyForJohtoStarter
	end

LittlerootTown_ProfessorBirchsLab_EventScript_CheckReadyForJohtoStarter::
	specialvar VAR_RESULT, HasAllHoennMons
	goto_if_eq VAR_RESULT, TRUE, LittlerootTown_ProfessorBirchsLab_EventScript_SetReadyForJohtoStarter
	setobjectmovementtype LOCALID_RIVAL, MOVEMENT_TYPE_WANDER_UP_AND_DOWN
	setobjectxyperm LOCALID_RIVAL, 5, 10
	end

LittlerootTown_ProfessorBirchsLab_EventScript_SetReadyForJohtoStarter::
	setvar VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 4
	goto LittlerootTown_ProfessorBirchsLab_EventScript_SetJohtoStarterLayout

LittlerootTown_ProfessorBirchsLab_EventScript_SetJohtoStarterLayout::
	setmaplayoutindex LAYOUT_LITTLEROOT_TOWN_PROFESSOR_BIRCHS_LAB_WITH_TABLE
	end

LittlerootTown_ProfessorBirchsLab_EventScript_SetAfterJohtoStarterLayout::
	setmaplayoutindex LAYOUT_LITTLEROOT_TOWN_PROFESSOR_BIRCHS_LAB_WITH_TABLE
	setobjectmovementtype LOCALID_RIVAL, MOVEMENT_TYPE_WANDER_UP_AND_DOWN
	setobjectxyperm LOCALID_RIVAL, 5, 10
	end

LittlerootTown_ProfessorBirchsLab_OnWarp:
	map_script_2 VAR_BIRCH_LAB_STATE, 2, LittlerootTown_ProfessorBirchsLab_EventScript_SetPlayerPosForReceiveStarter
	map_script_2 VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 1, LittlerootTown_ProfessorBirchsLab_EventScript_SetObjectPosForDexUpgrade
	map_script_2 VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 2, LittlerootTown_ProfessorBirchsLab_EventScript_SetObjectPosForDexUpgrade
	map_script_2 VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 3, LittlerootTown_ProfessorBirchsLab_EventScript_AddRivalObject
	map_script_2 VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 6, LittlerootTown_ProfessorBirchsLab_EventScript_AddRivalObject
	map_script_2 VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 4, LittlerootTown_ProfessorBirchsLab_EventScript_SetObjectPosForJohtoStarters
	map_script_2 VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 5, LittlerootTown_ProfessorBirchsLab_EventScript_SetObjectPosForJohtoStarters
	.2byte 0

LittlerootTown_ProfessorBirchsLab_EventScript_SetPlayerPosForReceiveStarter::
	turnobject OBJ_EVENT_ID_PLAYER, DIR_NORTH
	end

LittlerootTown_ProfessorBirchsLab_EventScript_SetObjectPosForDexUpgrade::
	turnobject OBJ_EVENT_ID_PLAYER, DIR_NORTH
	addobject LOCALID_BIRCH
	addobject LOCALID_RIVAL
	turnobject LOCALID_RIVAL, DIR_NORTH
	setobjectxy LOCALID_RIVAL, 5, 5
	turnobject LOCALID_BIRCH, DIR_SOUTH
	setobjectxy LOCALID_BIRCH, 6, 4
	turnobject LOCALID_AIDE, DIR_EAST
	setobjectxy LOCALID_AIDE, 10, 10
	end

LittlerootTown_ProfessorBirchsLab_EventScript_AddRivalObject::
	addobject LOCALID_RIVAL
	end

LittlerootTown_ProfessorBirchsLab_EventScript_SetObjectPosForJohtoStarters::
	addobject LOCALID_BALL_CYNDAQUIL
	addobject LOCALID_BALL_TOTODILE
	addobject LOCALID_BALL_CHIKORITA
	addobject LOCALID_RIVAL
	turnobject LOCALID_BALL_CYNDAQUIL, DIR_SOUTH
	setobjectxy LOCALID_BALL_CYNDAQUIL, 8, 4
	turnobject LOCALID_BALL_TOTODILE, DIR_SOUTH
	setobjectxy LOCALID_BALL_TOTODILE, 9, 4
	turnobject LOCALID_BALL_CHIKORITA, DIR_SOUTH
	setobjectxy LOCALID_BALL_CHIKORITA, 10, 4
	turnobject LOCALID_BIRCH, DIR_SOUTH
	setobjectxy LOCALID_BIRCH, 6, 4
	turnobject LOCALID_RIVAL, DIR_EAST
	setobjectxy LOCALID_RIVAL, 5, 5
	end

LittlerootTown_ProfessorBirchsLab_OnFrame:
	map_script_2 VAR_BIRCH_LAB_STATE, 2, LittlerootTown_ProfessorBirchsLab_EventScript_GiveStarterEvent
	map_script_2 VAR_BIRCH_LAB_STATE, 4, LittlerootTown_ProfessorBirchsLab_EventScript_GivePokedexEvent
	map_script_2 VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 1, LittlerootTown_ProfessorBirchsLab_EventScript_UpgradeToNationalDex
	map_script_2 VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 4, LittlerootTown_ProfessorBirchsLab_EventScript_ChooseJohtoStarter
	.2byte 0

@ The starter is technically given prior to this on Route 101 by special ChooseStarter
@ This is just where the game tells you its yours and lets you nickname it
LittlerootTown_ProfessorBirchsLab_EventScript_GiveStarterEvent::
	lockall
	bufferleadmonspeciesname STR_VAR_1
	message LittlerootTown_ProfessorBirchsLab_Text_LikeYouToHavePokemon
	waitmessage
	playfanfare MUS_OBTAIN_ITEM
	waitfanfare
	specialvar VAR_RESULT, IsNuzlockeNicknamingActive
	goto_if_eq VAR_RESULT, TRUE, LittlerootTown_ProfessorBirchsLab_EventScript_NicknameStarter_Nuzlocke
	msgbox LittlerootTown_ProfessorBirchsLab_Text_WhyNotGiveNicknameToMon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, LittlerootTown_ProfessorBirchsLab_EventScript_NicknameStarter
	goto_if_eq VAR_RESULT, NO, LittlerootTown_ProfessorBirchsLab_EventScript_GoSeeRival
	end

LittlerootTown_ProfessorBirchsLab_EventScript_NicknameStarter::
	setvar VAR_0x8004, 0
	call Common_EventScript_NameReceivedPartyMon
	goto LittlerootTown_ProfessorBirchsLab_EventScript_GoSeeRival
	end

LittlerootTown_ProfessorBirchsLab_EventScript_NicknameStarter_Nuzlocke::
	message LittlerootTown_ProfessorBirchsLab_Text_WhyNotGiveNicknameToMon_Nuzlocke
	waitmessage
	setvar VAR_0x8004, 0
	call Common_EventScript_NameReceivedPartyMon
	goto LittlerootTown_ProfessorBirchsLab_EventScript_GoSeeRival
	end

LittlerootTown_ProfessorBirchsLab_EventScript_GoSeeRival::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_MightBeGoodIdeaToGoSeeRival, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, LittlerootTown_ProfessorBirchsLab_EventScript_AgreeToSeeRival
	goto_if_eq VAR_RESULT, NO, LittlerootTown_ProfessorBirchsLab_EventScript_DeclineSeeingRival
	end

LittlerootTown_ProfessorBirchsLab_EventScript_AgreeToSeeRival::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_GetRivalToTeachYou, MSGBOX_DEFAULT
	clearflag FLAG_HIDE_ROUTE_101_BOY
	setvar VAR_BIRCH_LAB_STATE, 3
	releaseall
	end

LittlerootTown_ProfessorBirchsLab_EventScript_DeclineSeeingRival::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_DontBeThatWay, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, LittlerootTown_ProfessorBirchsLab_EventScript_AgreeToSeeRival
	goto_if_eq VAR_RESULT, NO, LittlerootTown_ProfessorBirchsLab_EventScript_DeclineSeeingRival
	end

LittlerootTown_ProfessorBirchsLab_EventScript_GivePokedexEvent::
	lockall
	applymovement OBJ_EVENT_ID_PLAYER, LittlerootTown_ProfessorBirchsLab_Movement_PlayerEnterLabForPokedex
	waitmovement 0
	goto LittlerootTown_ProfessorBirchsLab_EventScript_GivePokedex
	end

LittlerootTown_ProfessorBirchsLab_Movement_PlayerEnterLabForPokedex:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

LittlerootTown_ProfessorBirchsLab_EventScript_UpgradeToNationalDex::
	lockall
	delay 30
	msgbox LittlerootTown_ProfessorBirchsLab_Text_OtherRegionsUpgradeToNational, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_BIRCH, LittlerootTown_ProfessorBirchsLab_Movement_BirchRetrievePokedexes
	waitmovement 0
	delay 20
	playse SE_CLICK
	delay 10
	playse SE_CLICK
	delay 10
	playse SE_CLICK
	delay 10
	playse SE_CLICK
	delay 20
	applymovement LOCALID_RIVAL, Common_Movement_WalkInPlaceFasterRight
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	checkplayergender
	call_if_eq VAR_RESULT, MALE, LittlerootTown_ProfessorBirchsLab_EventScript_MayUpgradeComment
	call_if_eq VAR_RESULT, FEMALE, LittlerootTown_ProfessorBirchsLab_EventScript_BrendanUpgradeComment
	playse SE_PC_ON
	waitse
	delay 20
	applymovement LOCALID_BIRCH, LittlerootTown_ProfessorBirchsLab_Movement_BirchReturnPokedex
	waitmovement 0
	applymovement LOCALID_RIVAL, Common_Movement_WalkInPlaceFasterUp
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox LittlerootTown_ProfessorBirchsLab_Text_OkayAllDone, MSGBOX_DEFAULT
	playfanfare MUS_OBTAIN_ITEM
	message LittlerootTown_ProfessorBirchsLab_Text_PokedexUpgradedToNational
	waitmessage
	waitfanfare
	setflag FLAG_SYS_NATIONAL_DEX
	special EnableNationalPokedex
	msgbox LittlerootTown_ProfessorBirchsLab_Text_GrassyPatchWaiting2, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BIRCH
	setflag FLAG_HIDE_LITTLEROOT_TOWN_RIVAL
	setvar VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 2
	setvar VAR_SCOTT_BF_CALL_STEP_COUNTER, 0
	setflag FLAG_SCOTT_CALL_BATTLE_FRONTIER
	releaseall
	end

LittlerootTown_ProfessorBirchsLab_EventScript_MayUpgradeComment::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_MayUpgradeSoCool, MSGBOX_DEFAULT
	closemessage
	return

LittlerootTown_ProfessorBirchsLab_EventScript_BrendanUpgradeComment::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_BrendanYouCanThankMe, MSGBOX_DEFAULT
	closemessage
	return

LittlerootTown_ProfessorBirchsLab_Movement_BirchRetrievePokedexes:
	walk_left
	walk_in_place_faster_down
	delay_16
	delay_16
	walk_right
	walk_in_place_faster_down
	delay_16
	delay_16
	walk_right
	walk_down
	walk_down
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	walk_in_place_faster_up
	step_end

LittlerootTown_ProfessorBirchsLab_Movement_BirchReturnPokedex:
	walk_left
	walk_left
	walk_left
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	walk_in_place_faster_down
	step_end

LittlerootTown_ProfessorBirchsLab_EventScript_ChooseJohtoStarter::
	lockall
	applymovement OBJ_EVENT_ID_PLAYER, LittlerootTown_ProfessorBirchsLab_Movement_PlayerEnterLabForJohtoStarter
	waitmovement 0
	msgbox LittlerootTown_ProfessorBirchsLab_Text_CompletedDexChoosePokemon, MSGBOX_DEFAULT
	setvar VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 5
	releaseall
	end

LittlerootTown_ProfessorBirchsLab_Movement_PlayerEnterLabForJohtoStarter:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

LittlerootTown_ProfessorBirchsLab_EventScript_Aide::
	lock
	faceplayer
	goto_if_ge VAR_BIRCH_LAB_STATE, 3, LittlerootTown_ProfessorBirchsLab_EventScript_AideReceivedStarter
	goto_if_set FLAG_BIRCH_AIDE_MET, LittlerootTown_ProfessorBirchsLab_EventScript_AideAlreadyMet
	msgbox LittlerootTown_ProfessorBirchsLab_Text_BirchAwayOnFieldwork, MSGBOX_DEFAULT
	setflag FLAG_BIRCH_AIDE_MET
	release
	end

LittlerootTown_ProfessorBirchsLab_EventScript_AideAlreadyMet::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_BirchIsntOneForDeskWork, MSGBOX_DEFAULT
	release
	end

LittlerootTown_ProfessorBirchsLab_EventScript_AideReceivedStarter::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_BirchEnjoysRivalsHelpToo, MSGBOX_DEFAULT
	release
	end

LittlerootTown_ProfessorBirchsLab_EventScript_Cyndaquil::
	release
	goto_if_ge VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 6, LittlerootTown_ProfessorBirchsLab_EventScript_AlreadyChoseJohtoStarter
	applymovement LOCALID_BIRCH, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	showmonpic SPECIES_CYNDAQUIL, 10, 3
	msgbox LittlerootTown_ProfessorBirchsLab_Text_YoullTakeCyndaquil, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, LittlerootTown_ProfessorBirchsLab_EventScript_TakeYourTime
	goto LittlerootTown_ProfessorBirchsLab_EventScript_GiveCyndaquil
	end

LittlerootTown_ProfessorBirchsLab_EventScript_Totodile::
	release
	goto_if_ge VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 6, LittlerootTown_ProfessorBirchsLab_EventScript_AlreadyChoseJohtoStarter
	applymovement LOCALID_BIRCH, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	showmonpic SPECIES_TOTODILE, 10, 3
	msgbox LittlerootTown_ProfessorBirchsLab_Text_YoullTakeTotodile, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, LittlerootTown_ProfessorBirchsLab_EventScript_TakeYourTime
	goto LittlerootTown_ProfessorBirchsLab_EventScript_GiveTotodile
	end

LittlerootTown_ProfessorBirchsLab_EventScript_Chikorita::
	release
	goto_if_ge VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 6, LittlerootTown_ProfessorBirchsLab_EventScript_AlreadyChoseJohtoStarter
	applymovement LOCALID_BIRCH, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	showmonpic SPECIES_CHIKORITA, 10, 3
	msgbox LittlerootTown_ProfessorBirchsLab_Text_YoullTakeChikorita, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, LittlerootTown_ProfessorBirchsLab_EventScript_TakeYourTime
	goto LittlerootTown_ProfessorBirchsLab_EventScript_GiveChikorita
	end

LittlerootTown_ProfessorBirchsLab_EventScript_AlreadyChoseJohtoStarter::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_BetterLeaveOthersAlone, MSGBOX_DEFAULT
	releaseall
	end

LittlerootTown_ProfessorBirchsLab_EventScript_TakeYourTime::
	hidemonpic
	msgbox LittlerootTown_ProfessorBirchsLab_Text_TakeYourTimeAllInvaluable, MSGBOX_DEFAULT
	releaseall
	end

LittlerootTown_ProfessorBirchsLab_EventScript_GiveCyndaquil::
	bufferspeciesname STR_VAR_1, SPECIES_CYNDAQUIL
	setvar VAR_TEMP_1, SPECIES_CYNDAQUIL
	givemon SPECIES_CYNDAQUIL, 5
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PARTY, LittlerootTown_ProfessorBirchsLab_EventScript_SendCyndaquilToParty
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PC, LittlerootTown_ProfessorBirchsLab_EventScript_SendCyndaquilToPC
	hidemonpic
	goto Common_EventScript_NoMoreRoomForPokemon
	end

LittlerootTown_ProfessorBirchsLab_EventScript_SendCyndaquilToParty::
	call LittlerootTown_ProfessorBirchsLab_EventScript_ReceivedJohtoStarter
	removeobject LOCALID_BALL_CYNDAQUIL
	specialvar VAR_RESULT, IsNuzlockeNicknamingActive
	goto_if_eq VAR_RESULT, TRUE, LittlerootTown_ProfessorBirchsLab_EventScript_SendCyndaquilToParty_Nuzlocke
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, LittlerootTown_ProfessorBirchsLab_EventScript_ReceivedCyndaquil
	call Common_EventScript_GetGiftMonPartySlot
	call Common_EventScript_NameReceivedPartyMon
	goto LittlerootTown_ProfessorBirchsLab_EventScript_ReceivedCyndaquil
	end

LittlerootTown_ProfessorBirchsLab_EventScript_SendCyndaquilToParty_Nuzlocke::
	call Common_EventScript_GetGiftMonPartySlot
	call Common_EventScript_NameReceivedPartyMon
	goto LittlerootTown_ProfessorBirchsLab_EventScript_ReceivedCyndaquil
	end

LittlerootTown_ProfessorBirchsLab_EventScript_SendCyndaquilToPC::
	call LittlerootTown_ProfessorBirchsLab_EventScript_ReceivedJohtoStarter
	removeobject LOCALID_BALL_CYNDAQUIL
	specialvar VAR_RESULT, IsNuzlockeNicknamingActive
	goto_if_eq VAR_RESULT, TRUE, LittlerootTown_ProfessorBirchsLab_EventScript_SendCyndaquilToPC_Nuzlocke
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, LittlerootTown_ProfessorBirchsLab_EventScript_CyndaquilTransferredToPC
	call Common_EventScript_NameReceivedBoxMon
	goto LittlerootTown_ProfessorBirchsLab_EventScript_CyndaquilTransferredToPC
	end

LittlerootTown_ProfessorBirchsLab_EventScript_SendCyndaquilToPC_Nuzlocke::
	call Common_EventScript_NameReceivedBoxMon
	goto LittlerootTown_ProfessorBirchsLab_EventScript_CyndaquilTransferredToPC
	end

LittlerootTown_ProfessorBirchsLab_EventScript_CyndaquilTransferredToPC::
	call Common_EventScript_TransferredToPC
	goto LittlerootTown_ProfessorBirchsLab_EventScript_ReceivedCyndaquil
	end

LittlerootTown_ProfessorBirchsLab_EventScript_ReceivedCyndaquil::
	hidemonpic
	msgbox LittlerootTown_ProfessorBirchsLab_Text_GrassyPatchWaiting, MSGBOX_DEFAULT
	setvar VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 6
	releaseall
	end

LittlerootTown_ProfessorBirchsLab_EventScript_GiveTotodile::
	bufferspeciesname STR_VAR_1, SPECIES_TOTODILE
	setvar VAR_TEMP_1, SPECIES_TOTODILE
	givemon SPECIES_TOTODILE, 5
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PARTY, LittlerootTown_ProfessorBirchsLab_EventScript_SendTotodileToParty
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PC, LittlerootTown_ProfessorBirchsLab_EventScript_SendTotodileToPC
	hidemonpic
	goto Common_EventScript_NoMoreRoomForPokemon
	end

LittlerootTown_ProfessorBirchsLab_EventScript_SendTotodileToParty::
	call LittlerootTown_ProfessorBirchsLab_EventScript_ReceivedJohtoStarter
	removeobject LOCALID_BALL_TOTODILE
	specialvar VAR_RESULT, IsNuzlockeNicknamingActive
	goto_if_eq VAR_RESULT, TRUE, LittlerootTown_ProfessorBirchsLab_EventScript_SendTotodileToParty_Nuzlocke
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, LittlerootTown_ProfessorBirchsLab_EventScript_ReceivedTotodile
	call Common_EventScript_GetGiftMonPartySlot
	call Common_EventScript_NameReceivedPartyMon
	goto LittlerootTown_ProfessorBirchsLab_EventScript_ReceivedTotodile
	end

LittlerootTown_ProfessorBirchsLab_EventScript_SendTotodileToParty_Nuzlocke::
	call Common_EventScript_GetGiftMonPartySlot
	call Common_EventScript_NameReceivedPartyMon
	goto LittlerootTown_ProfessorBirchsLab_EventScript_ReceivedTotodile
	end

LittlerootTown_ProfessorBirchsLab_EventScript_SendTotodileToPC::
	call LittlerootTown_ProfessorBirchsLab_EventScript_ReceivedJohtoStarter
	removeobject LOCALID_BALL_TOTODILE
	specialvar VAR_RESULT, IsNuzlockeNicknamingActive
	goto_if_eq VAR_RESULT, TRUE, LittlerootTown_ProfessorBirchsLab_EventScript_SendTotodileToPC_Nuzlocke
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, LittlerootTown_ProfessorBirchsLab_EventScript_TotodileTransferredToPC
	call Common_EventScript_NameReceivedBoxMon
	goto LittlerootTown_ProfessorBirchsLab_EventScript_TotodileTransferredToPC
	end

LittlerootTown_ProfessorBirchsLab_EventScript_SendTotodileToPC_Nuzlocke::
	call Common_EventScript_NameReceivedBoxMon
	goto LittlerootTown_ProfessorBirchsLab_EventScript_TotodileTransferredToPC
	end

LittlerootTown_ProfessorBirchsLab_EventScript_TotodileTransferredToPC::
	call Common_EventScript_TransferredToPC
	goto LittlerootTown_ProfessorBirchsLab_EventScript_ReceivedTotodile
	end

LittlerootTown_ProfessorBirchsLab_EventScript_ReceivedTotodile::
	hidemonpic
	msgbox LittlerootTown_ProfessorBirchsLab_Text_GrassyPatchWaiting, MSGBOX_DEFAULT
	setvar VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 6
	releaseall
	end

LittlerootTown_ProfessorBirchsLab_EventScript_GiveChikorita::
	bufferspeciesname STR_VAR_1, SPECIES_CHIKORITA
	setvar VAR_TEMP_1, SPECIES_CHIKORITA
	givemon SPECIES_CHIKORITA, 5
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PARTY, LittlerootTown_ProfessorBirchsLab_EventScript_SendChikoritaToParty
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PC, LittlerootTown_ProfessorBirchsLab_EventScript_SendChikoritaToPC
	hidemonpic
	goto Common_EventScript_NoMoreRoomForPokemon
	end

LittlerootTown_ProfessorBirchsLab_EventScript_SendChikoritaToParty::
	call LittlerootTown_ProfessorBirchsLab_EventScript_ReceivedJohtoStarter
	removeobject LOCALID_BALL_CHIKORITA
	specialvar VAR_RESULT, IsNuzlockeNicknamingActive
	goto_if_eq VAR_RESULT, TRUE, LittlerootTown_ProfessorBirchsLab_EventScript_SendChikoritaToParty_Nuzlocke
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, LittlerootTown_ProfessorBirchsLab_EventScript_ReceivedChikorita
	call Common_EventScript_GetGiftMonPartySlot
	call Common_EventScript_NameReceivedPartyMon
	goto LittlerootTown_ProfessorBirchsLab_EventScript_ReceivedChikorita
	end

LittlerootTown_ProfessorBirchsLab_EventScript_SendChikoritaToParty_Nuzlocke::
	call Common_EventScript_GetGiftMonPartySlot
	call Common_EventScript_NameReceivedPartyMon
	goto LittlerootTown_ProfessorBirchsLab_EventScript_ReceivedChikorita
	end

LittlerootTown_ProfessorBirchsLab_EventScript_SendChikoritaToPC::
	call LittlerootTown_ProfessorBirchsLab_EventScript_ReceivedJohtoStarter
	removeobject LOCALID_BALL_CHIKORITA
	specialvar VAR_RESULT, IsNuzlockeNicknamingActive
	goto_if_eq VAR_RESULT, TRUE, LittlerootTown_ProfessorBirchsLab_EventScript_SendChikoritaToPC_Nuzlocke
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, LittlerootTown_ProfessorBirchsLab_EventScript_ChikoritaTransferredToPC
	call Common_EventScript_NameReceivedBoxMon
	goto LittlerootTown_ProfessorBirchsLab_EventScript_ChikoritaTransferredToPC
	end

LittlerootTown_ProfessorBirchsLab_EventScript_SendChikoritaToPC_Nuzlocke::
	call Common_EventScript_NameReceivedBoxMon
	goto LittlerootTown_ProfessorBirchsLab_EventScript_ChikoritaTransferredToPC
	end

LittlerootTown_ProfessorBirchsLab_EventScript_ChikoritaTransferredToPC::
	call Common_EventScript_TransferredToPC
	goto LittlerootTown_ProfessorBirchsLab_EventScript_ReceivedChikorita
	end

LittlerootTown_ProfessorBirchsLab_EventScript_ReceivedChikorita::
	hidemonpic
	msgbox LittlerootTown_ProfessorBirchsLab_Text_GrassyPatchWaiting, MSGBOX_DEFAULT
	setvar VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 6
	releaseall
	end

LittlerootTown_ProfessorBirchsLab_EventScript_ReceivedJohtoStarter::
	playfanfare MUS_OBTAIN_ITEM
	message LittlerootTown_ProfessorBirchsLab_Text_ReceivedJohtoStarter
	waitmessage
	waitfanfare
	return

LittlerootTown_ProfessorBirchsLab_EventScript_Birch::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_POKEDEX_FROM_BIRCH, LittlerootTown_ProfessorBirchsLab_EventScript_TryRestoreLegacyBundle
	goto_if_eq VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 5, LittlerootTown_ProfessorBirchsLab_EventScript_CanHaveAnyOneOfRarePokemon
	goto_if_eq VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 2, LittlerootTown_ProfessorBirchsLab_EventScript_GrassyPatchWaiting
	goto_if_unset FLAG_HAS_MATCH_CALL, LittlerootTown_ProfessorBirchsLab_EventScript_TryRatePokedexOrRegister
	goto_if_unset FLAG_ENABLE_PROF_BIRCH_MATCH_CALL, EventScript_RegisterProfBirch
	goto LittlerootTown_ProfessorBirchsLab_EventScript_TryRatePokedexOrRegister
	end

LittlerootTown_ProfessorBirchsLab_EventScript_TryRestoreLegacyBundle::
	checkitem ITEM_TM01, 1
	goto_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_RestoreLegacyBundle
	checkitem ITEM_BLACK_AUGURITE, 1
	goto_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_RestoreLegacyBundle
	call LittlerootTown_ProfessorBirchsLab_EventScript_GiveMissingBattleItems
	goto LittlerootTown_ProfessorBirchsLab_EventScript_TryRatePokedexOrRegister
	end

LittlerootTown_ProfessorBirchsLab_EventScript_RestoreLegacyBundle::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_BirchRestoringBundle, MSGBOX_DEFAULT
	special BaneModeGiveLegacyBirchBundle
	call LittlerootTown_ProfessorBirchsLab_EventScript_GiveMissingBattleItems
	msgbox LittlerootTown_ProfessorBirchsLab_Text_BirchRestoredBundleDone, MSGBOX_DEFAULT
	release
	end

LittlerootTown_ProfessorBirchsLab_EventScript_CanHaveAnyOneOfRarePokemon::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_CanHaveAnyOneOfRarePokemon, MSGBOX_DEFAULT
	release
	end

LittlerootTown_ProfessorBirchsLab_EventScript_GrassyPatchWaiting::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_GrassyPatchWaiting2, MSGBOX_DEFAULT
	release
	end

LittlerootTown_ProfessorBirchsLab_EventScript_TryRatePokedexOrRegister::
	goto_if_unset FLAG_HIDE_LITTLEROOT_TOWN_BIRCHS_LAB_UNKNOWN_0x380, ProfBirch_EventScript_RatePokedexOrRegister
	goto_if_eq VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 3, ProfBirch_EventScript_RatePokedexOrRegister
	goto_if_ge VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 6, ProfBirch_EventScript_RatePokedexOrRegister
	goto_if_eq VAR_BIRCH_LAB_STATE, 5, LittlerootTown_ProfessorBirchsLab_EventScript_PokemonAwait
	msgbox LittlerootTown_ProfessorBirchsLab_Text_BirchRivalGoneHome, MSGBOX_DEFAULT
	release
	end

EventScript_RegisterProfBirch::
	msgbox MatchCall_Text_BirchRegisterCall, MSGBOX_DEFAULT
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox MatchCall_Text_RegisteredBirch, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_PROF_BIRCH_MATCH_CALL
	setvar VAR_REGISTER_BIRCH_STATE, 2
	release
	end

LittlerootTown_ProfessorBirchsLab_EventScript_GivePokedex::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_HeardYouBeatRivalTakePokedex, MSGBOX_DEFAULT
	call LittlerootTown_ProfessorBirchsLab_EventScript_ReceivePokedex
	msgbox LittlerootTown_ProfessorBirchsLab_Text_ExplainPokedex, MSGBOX_DEFAULT
	closemessage
	delay 30
	msgbox LittlerootTown_ProfessorBirchsLab_Text_OtherRegionsUpgradeToNational, MSGBOX_DEFAULT
	closemessage
	delay 20
	playse SE_CLICK
	delay 10
	playse SE_CLICK
	delay 10
	playse SE_CLICK
	delay 10
	playse SE_CLICK
	delay 20
	msgbox LittlerootTown_ProfessorBirchsLab_Text_OkayAllDone, MSGBOX_DEFAULT
	playfanfare MUS_OBTAIN_ITEM
	message LittlerootTown_ProfessorBirchsLab_Text_PokedexUpgradedToNational
	closemessage
	applymovement LOCALID_RIVAL, LittlerootTown_ProfessorBirchsLab_Movement_RivalApproachPlayer
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	checkplayergender
	call_if_eq VAR_RESULT, MALE, LittlerootTown_ProfessorBirchsLab_EventScript_MayGivePokeBalls
	call_if_eq VAR_RESULT, FEMALE, LittlerootTown_ProfessorBirchsLab_EventScript_BrendanGivePokeBalls
	setvar VAR_BIRCH_LAB_STATE, 5
	setflag FLAG_SYS_DEXNAV_GET
	clearflag FLAG_NO_CATCHING
	clearflag FLAG_NO_ESCAPE
	setflag FLAG_SYS_QUEST_MENU_GET
	setflag FLAG_SYS_NATIONAL_DEX
	special EnableNationalPokedex
	setvar VAR_ROUTE101_STATE, 4
	setvar VAR_OLDALE_TOWN_STATE, 1
	setvar VAR_LITTLEROOT_RIVAL_STATE, 4
	setvar VAR_LITTLEROOT_TOWN_STATE, 3
	releaseall
	end

LittlerootTown_ProfessorBirchsLab_EventScript_MayGivePokeBalls::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_MayGotPokedexTooTakeThese, MSGBOX_DEFAULT
	giveitem ITEM_POKE_BALL, 5
	call LittlerootTown_ProfessorBirchsLab_EventScript_GiveBirchTmBundle
	setflag FLAG_ADVENTURE_STARTED
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_MayNoRoomForPokeBalls
	msgbox LittlerootTown_ProfessorBirchsLab_Text_CatchCutePokemonWithPokeBalls, MSGBOX_DEFAULT
	setvar VAR_RESULT, 0
	return

LittlerootTown_ProfessorBirchsLab_EventScript_BrendanGivePokeBalls::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_BrendanGotPokedexTooTakeThese, MSGBOX_DEFAULT
	giveitem ITEM_POKE_BALL, 5
	call LittlerootTown_ProfessorBirchsLab_EventScript_GiveBirchTmBundle
	setflag FLAG_ADVENTURE_STARTED
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_BrendanNoRoomForPokeBalls
	msgbox LittlerootTown_ProfessorBirchsLab_Text_CatchCoolPokemonWithPokeBalls, MSGBOX_DEFAULT
	setvar VAR_RESULT, 1
	return

LittlerootTown_ProfessorBirchsLab_EventScript_GiveBirchTmBundle::
	special BaneModeGiveAllTMs
	return

LittlerootTown_ProfessorBirchsLab_EventScript_ReceivePokedex::
	playfanfare MUS_OBTAIN_ITEM
	message LittlerootTown_ProfessorBirchsLab_Text_ReceivedPokedex
	waitfanfare
	message LittlerootTown_ProfessorBirchsLab_Text_ReceivedDexnav
	setflag FLAG_SYS_POKEDEX_GET
	special SetUnlockedPokedexFlags
	setflag FLAG_RECEIVED_POKEDEX_FROM_BIRCH
	setvar VAR_CABLE_CLUB_TUTORIAL_STATE, 1
	return

LittlerootTown_ProfessorBirchsLab_EventScript_PokemonAwait::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_CountlessPokemonAwait, MSGBOX_DEFAULT
	release
	end

LittlerootTown_ProfessorBirchsLab_EventScript_MayNoRoomForPokeBalls::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_OhYourBagsFull, MSGBOX_DEFAULT
	return

LittlerootTown_ProfessorBirchsLab_EventScript_BrendanNoRoomForPokeBalls::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_HeyYourBagsFull, MSGBOX_DEFAULT
	return

LittlerootTown_ProfessorBirchsLab_Movement_RivalApproachPlayer:
	walk_down
	walk_in_place_faster_left
	step_end

LittlerootTown_ProfessorBirchsLab_EventScript_Machine::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_SeriousLookingMachine, MSGBOX_SIGN
	end

LittlerootTown_ProfessorBirchsLab_EventScript_Rival::
	lock
	faceplayer
	goto_if_eq VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 5, LittlerootTown_ProfessorBirchsLab_EventScript_RivalFuturePlans
	goto_if_ge VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 6, LittlerootTown_ProfessorBirchsLab_EventScript_RivalHaveYouGoneToBattleFrontier
	goto_if_ge VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 2, LittlerootTown_ProfessorBirchsLab_EventScript_RivalTakeBreakFromFieldwork
	checkplayergender
	call_if_eq VAR_RESULT, MALE, LittlerootTown_ProfessorBirchsLab_EventScript_MayWhereShouldIGoNext
	call_if_eq VAR_RESULT, FEMALE, LittlerootTown_ProfessorBirchsLab_EventScript_BrendanWhereShouldIGoNext
	release
	end

LittlerootTown_ProfessorBirchsLab_EventScript_MayWhereShouldIGoNext::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_MayWhereShouldIGoNext, MSGBOX_DEFAULT
	return

LittlerootTown_ProfessorBirchsLab_EventScript_BrendanWhereShouldIGoNext::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_BrendanWhereShouldIGoNext, MSGBOX_DEFAULT
	return

LittlerootTown_ProfessorBirchsLab_EventScript_RivalFuturePlans::
	checkplayergender
	call_if_eq VAR_RESULT, MALE, LittlerootTown_ProfessorBirchsLab_EventScript_MayWhatNextImStayingHere
	call_if_eq VAR_RESULT, FEMALE, LittlerootTown_ProfessorBirchsLab_EventScript_BrendanPreferCollectingSlowly
	release
	end

LittlerootTown_ProfessorBirchsLab_EventScript_MayWhatNextImStayingHere::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_MayWhatNextImStayingHere, MSGBOX_DEFAULT
	return

LittlerootTown_ProfessorBirchsLab_EventScript_BrendanPreferCollectingSlowly:
	msgbox LittlerootTown_ProfessorBirchsLab_Text_BrendanPreferCollectingSlowly, MSGBOX_DEFAULT
	return

LittlerootTown_ProfessorBirchsLab_EventScript_RivalHaveYouGoneToBattleFrontier::
	checkplayergender
	call_if_eq VAR_RESULT, MALE, LittlerootTown_ProfessorBirchsLab_EventScript_MayHaveYouGoneToBattleFrontier
	call_if_eq VAR_RESULT, FEMALE, LittlerootTown_ProfessorBirchsLab_EventScript_BrendanHaveYouGoneToBattleFrontier
	release
	end

LittlerootTown_ProfessorBirchsLab_EventScript_MayHaveYouGoneToBattleFrontier::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_MayHaveYouGoneToBattleFrontier, MSGBOX_DEFAULT
	return

LittlerootTown_ProfessorBirchsLab_EventScript_BrendanHaveYouGoneToBattleFrontier::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_BrendanHaveYouGoneToBattleFrontier, MSGBOX_DEFAULT
	return

LittlerootTown_ProfessorBirchsLab_EventScript_RivalTakeBreakFromFieldwork::
	checkplayergender
	call_if_eq VAR_RESULT, MALE, LittlerootTown_ProfessorBirchsLab_EventScript_MayTakeBreakFromFieldwork
	call_if_eq VAR_RESULT, FEMALE, LittlerootTown_ProfessorBirchsLab_EventScript_BrendanTakeBreakFromFieldwork
	release
	end

LittlerootTown_ProfessorBirchsLab_EventScript_MayTakeBreakFromFieldwork::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_MayTakeBreakFromFieldwork, MSGBOX_DEFAULT
	return

LittlerootTown_ProfessorBirchsLab_EventScript_BrendanTakeBreakFromFieldwork::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_BrendanTakeBreakFromFieldwork, MSGBOX_DEFAULT
	return

LittlerootTown_ProfessorBirchsLab_EventScript_PC::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_PCUsedForResearch, MSGBOX_SIGN
	end

LittlerootTown_ProfessorBirchsLab_EventScript_Bookshelf::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_CrammedWithBooksOnPokemon, MSGBOX_SIGN
	end

LittlerootTown_ProfessorBirchsLab_EventScript_Book::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_BookTooHardToRead, MSGBOX_SIGN
	end

LittlerootTown_ProfessorBirchsLab_EventScript_ScottAboardSSTidalCall::
	lockall
	pokenavcall LittlerootTown_ProfessorBirchsLab_Text_ScottAboardSSTidalCall
	waitmessage
	clearflag FLAG_SCOTT_CALL_BATTLE_FRONTIER
	releaseall
	end

LittlerootTown_ProfessorBirchsLab_Text_BirchAwayOnFieldwork:
	.string "Hunh? PROF. BIRCH?\p"
	.string "The PROF's away on fieldwork.\n"
	.string "Ergo, he isn't here.\p"
	.string "Oh, let me explain what fieldwork is.\p"
	.string "It is to study things in the natural\n"
	.string "environment, like fields and mountains,\l"
	.string "instead of a laboratory.\p"
	.string "The PROF isn't one for doing desk work.\n"
	.string "He's the type of person who would\l"
	.string "rather go outside and experience\l"
	.string "things than read about them here.$"

LittlerootTown_ProfessorBirchsLab_Text_BirchIsntOneForDeskWork:
	.string "The PROF isn't one for doing desk work.\n"
	.string "He's the type of person who would\l"
	.string "rather go outside and experience\l"
	.string "things than read about them here.$"

LittlerootTown_ProfessorBirchsLab_Text_BirchEnjoysRivalsHelpToo:
	.string "PROF. BIRCH is studying the habitats\n"
	.string "and distribution of POKéMON.\p"
	.string "The PROF enjoys {RIVAL}'s help, too.\n"
	.string "There's a lot of love there.$"

LittlerootTown_ProfessorBirchsLab_Text_LikeYouToHavePokemon:
	.string "PROF. BIRCH: So, {PLAYER}{KUN}.\p"
	.string "I've heard so much about you from\n"
	.string "your father.\p"
	.string "I've heard that you don't have your\n"
	.string "own POKéMON yet.\p"
	.string "But the way you battled earlier,\n"
	.string "you pulled it off with aplomb!\p"
	.string "I guess you have your father's blood\n"
	.string "in your veins after all!\p"
	.string "Oh, yes. As thanks for rescuing me,\n"
	.string "I'd like you to have the POKéMON you\l"
	.string "used earlier.\p"
	.string "{PLAYER} received the {STR_VAR_1}!$"

LittlerootTown_ProfessorBirchsLab_Text_WhyNotGiveNicknameToMon:
	.string "PROF. BIRCH: While you're at it, why not\n"
	.string "give a nickname to that {STR_VAR_1}?$"

LittlerootTown_ProfessorBirchsLab_Text_WhyNotGiveNicknameToMon_Nuzlocke:
	.string "PROF. BIRCH: While you're at it, why not\n"
	.string "give a nickname to that {STR_VAR_1}?\p$"

LittlerootTown_ProfessorBirchsLab_Text_MightBeGoodIdeaToGoSeeRival:
	.string "PROF. BIRCH: Yo, what's good, fam?\p"
	.string "You got the look of somebody who's\n"
	.string "about to learn fast out here.\p"
	.string "My kid, {RIVAL}, is out studying\n"
	.string "POKéMON and putting in work, too.\p"
	.string "{PLAYER}{KUN}, go see {RIVAL} and let\n"
	.string "the streets grade your first step.$"

LittlerootTown_ProfessorBirchsLab_Text_GetRivalToTeachYou:
	.string "PROF. BIRCH: That's the energy.\p"
	.string "{RIVAL} can show you what's poppin'\n"
	.string "once a real battle starts.$"

LittlerootTown_ProfessorBirchsLab_Text_DontBeThatWay:
	.string "PROF. BIRCH: Oh, don't be that way.\n"
	.string "You should go meet my kid.$"

LittlerootTown_ProfessorBirchsLab_Text_BirchRivalGoneHome:
	.string "PROF. BIRCH: {RIVAL}?\n"
	.string "Gone home, I think.\p"
	.string "Or maybe that kid's scrabbling around\n"
	.string "in tall grass again somewhere…\p"
	.string "If you or your POKéMON get tired,\n"
	.string "you should get some rest at home.$"

LittlerootTown_ProfessorBirchsLab_Text_HeardYouBeatRivalTakePokedex:
	.string "PROF. BIRCH: Oh, there you are,\n"
	.string "{PLAYER}{KUN}!\p"
	.string "Word is you deadass beat {RIVAL}\n"
	.string "first try. I respect that.\p"
	.string "{RIVAL}'s been helping with my research\n"
	.string "for ages, so that's no light work.\p"
	.string "Take this DEXNAV and POKéDEX.\p"
	.string "And hear me good: this BANE MODE run is\n"
	.string "nasty, my guy. It only gets meaner\l"
	.string "from here.$"

LittlerootTown_ProfessorBirchsLab_Text_ReceivedPokedex:
	.string "{PLAYER} received the POKéDEX!$"

LittlerootTown_ProfessorBirchsLab_Text_ReceivedDexnav:
    .string "{PLAYER} received the DEXNAV!$"

LittlerootTown_ProfessorBirchsLab_Text_ExplainPokedex:
	.string "PROF. BIRCH: The POKéDEX is a high-tech\n"
	.string "tool that automatically makes a record\l"
	.string "of any POKéMON you meet or catch.\p"
	.string "My kid, {RIVAL}, goes everywhere\n"
	.string "with it.\p"
	.string "Whenever my kid catches a rare POKéMON\n"
	.string "and records its data in the POKéDEX,\l"
	.string "why, {RIVAL} looks for me while I'm out\l"
	.string "doing fieldwork, and shows me.\p"
	.string "I also stuffed your BAG with the nasty\n"
	.string "BANE MODE essentials. Rare Candies,\l"
	.string "battle items, berries, and your Holy\l"
	.string "Blunt are all packed already, word.$"

LittlerootTown_ProfessorBirchsLab_Text_CountlessPokemonAwait:
	.string "PROF. BIRCH: Countless POKéMON\n"
	.string "await you!\p"
	.string "Argh, I'm getting the itch to get out\n"
	.string "and do fieldwork again!\p"
	.string "Stay sharp out there. HOENN under\n"
	.string "BANE MODE does not forgive lazy prep.$"

LittlerootTown_ProfessorBirchsLab_Text_MayGotPokedexTooTakeThese:
	.string "MAY: Oh, wow, {PLAYER}{KUN}!\n"
	.string "You got a POKéDEX, too!\p"
	.string "That's great! Just like me!\n"
	.string "I've got something for you, too!$"

LittlerootTown_ProfessorBirchsLab_Text_CatchCutePokemonWithPokeBalls:
	.string "MAY: It's fun if you can get a lot of\n"
	.string "POKéMON!\p"
	.string "I'm going to look all over the place\n"
	.string "because I want different POKéMON.\p"
	.string "If I find any cute POKéMON, I'll catch\n"
	.string "them with POKé BALLS!$"

LittlerootTown_ProfessorBirchsLab_Text_OhYourBagsFull:
	.string "Oh? Your BAG's full.$"

LittlerootTown_ProfessorBirchsLab_Text_MayWhereShouldIGoNext:
	.string "MAY: I wonder where I should go look\n"
	.string "for POKéMON next?$"

LittlerootTown_ProfessorBirchsLab_Text_BrendanGotPokedexTooTakeThese:
	.string "BRENDAN: Huh…\n"
	.string "So you got a POKéDEX too.\p"
	.string "Aight, here.\n"
	.string "I'll give you these.$"

LittlerootTown_ProfessorBirchsLab_Text_CatchCoolPokemonWithPokeBalls:
	.string "BRENDAN: You know it's more fun\n"
	.string "with a whole squad of POKéMON.\p"
	.string "I'm gonna move all over the place\n"
	.string "looking for different POKéMON.\p"
	.string "If I find anything cool, best believe\n"
	.string "I'm catching it with POKé BALLS.$"

LittlerootTown_ProfessorBirchsLab_Text_HeyYourBagsFull:
	.string "Hey, your BAG's full.$"

LittlerootTown_ProfessorBirchsLab_Text_BrendanWhereShouldIGoNext:
	.string "BRENDAN: Where should I go looking\n"
	.string "for POKéMON next…$"

LittlerootTown_ProfessorBirchsLab_Text_SeriousLookingMachine:
	.string "It's a serious-looking machine.\n"
	.string "The PROF must use this for research.$"

LittlerootTown_ProfessorBirchsLab_Text_PCUsedForResearch:
	.string "It's a PC used for research.\n"
	.string "Better not mess around with it.$"

LittlerootTown_ProfessorBirchsLab_Text_CrammedWithBooksOnPokemon:
	.string "It's crammed with books on POKéMON.$"

LittlerootTown_ProfessorBirchsLab_Text_BookTooHardToRead:
	.string "It's a book that's too hard to read.$"

LittlerootTown_ProfessorBirchsLab_Text_OtherRegionsUpgradeToNational:
	.string "PROF. BIRCH: Oh I almost forgot, now…\p"
	.string "It appears that in the HOENN region,\n"
	.string "there are also POKéMON from other\l"
	.string "regions.\p"
	.string "It goes to show how rich and varied\n"
	.string "the natural environments of HOENN\l"
	.string "happen to be.\p"
	.string "That's why I think it's necessary for\n"
	.string "me to upgrade your POKéDEX to\l"
	.string "the NATIONAL Mode.\p"
	.string "Here, let me see your POKéDEX unit.$"

LittlerootTown_ProfessorBirchsLab_Text_MayUpgradeSoCool:
	.string "MAY: Eheheh!\p"
	.string "It's so cool that even my POKéDEX\n"
	.string "is getting updated!\p"
	.string "It's because you went out and caught\n"
	.string "so many POKéMON, {PLAYER}{KUN}!$"

LittlerootTown_ProfessorBirchsLab_Text_BrendanYouCanThankMe:
	.string "BRENDAN: I went all over HOENN\n"
	.string "checking out POKéMON.\p"
	.string "You can thank me for getting\n"
	.string "the NATIONAL Mode POKéDEX.\p"
	.string "You're lucky, {PLAYER}, no cap!$"

LittlerootTown_ProfessorBirchsLab_Text_OkayAllDone:
	.string "PROF. BIRCH: Okay, all done!$"

LittlerootTown_ProfessorBirchsLab_Text_PokedexUpgradedToNational:
	.string "{PLAYER}'s POKéDEX was upgraded\n"
	.string "to the NATIONAL Mode!$"

LittlerootTown_ProfessorBirchsLab_Text_GrassyPatchWaiting2:
	.string "PROF. BIRCH: But listen.\n"
	.string "You've become the CHAMPION,\l"
	.string "but your POKéMON journey isn't over.\p"
	.string "There is no end to the road\n"
	.string "that is POKéMON.\p"
	.string "Somewhere, there is a grassy patch\n"
	.string "that's waiting for you!$"

LittlerootTown_ProfessorBirchsLab_Text_MayTakeBreakFromFieldwork:
	.string "MAY: I think I'll take a short break\n"
	.string "from fieldwork.\p"
	.string "I think I'll help the PROF here for\n"
	.string "a while.$"

LittlerootTown_ProfessorBirchsLab_Text_BrendanTakeBreakFromFieldwork:
	.string "BRENDAN: For now, I'm taking\n"
	.string "a break from fieldwork.\p"
	.string "I'll help the PROF here\n"
	.string "for a while.$"

LittlerootTown_ProfessorBirchsLab_Text_CompletedDexChoosePokemon:
	.string "PROF. BIRCH: Oh, {PLAYER}{KUN}!\n"
	.string "Let's have a look at your POKéDEX.\p"
	.string "… … … … … …\n"
	.string "… … … … … …\p"
	.string "Yes, there's no doubt about it.\p"
	.string "You really have completed the HOENN\n"
	.string "region's POKéDEX.\p"
	.string "That's more than just impressive.\p"
	.string "I have a gift for you.\p"
	.string "Consider it my show of appreciation\n"
	.string "for the fantastic work you've done.\p"
	.string "My gift is a rare POKéMON only found\n"
	.string "in another region!\p"
	.string "You can have any one of these\n"
	.string "three POKéMON!$"

LittlerootTown_ProfessorBirchsLab_Text_CanHaveAnyOneOfRarePokemon:
	.string "PROF. BIRCH: These are rare POKéMON\n"
	.string "only found in another region!\p"
	.string "You can have any one of these\n"
	.string "three POKéMON!$"

LittlerootTown_ProfessorBirchsLab_Text_YoullTakeCyndaquil:
	.string "PROF. BIRCH: The FIRE POKéMON\n"
	.string "CYNDAQUIL caught your eye!\p"
	.string "You're as sharp as ever!\p"
	.string "So the CYNDAQUIL is your choice?$"

LittlerootTown_ProfessorBirchsLab_Text_YoullTakeTotodile:
	.string "PROF. BIRCH: The WATER POKéMON\n"
	.string "TOTODILE is your choice!\p"
	.string "You know how to pick a good one.\p"
	.string "So, you'll take the TOTODILE?$"

LittlerootTown_ProfessorBirchsLab_Text_YoullTakeChikorita:
	.string "PROF. BIRCH: The GRASS POKéMON\n"
	.string "CHIKORITA is your choice!\p"
	.string "You sure know what you're doing.\p"
	.string "So, you'll take the CHIKORITA?$"

LittlerootTown_ProfessorBirchsLab_Text_TakeYourTimeAllInvaluable:
	.string "PROF. BIRCH: Take your time before\n"
	.string "you decide.\p"
	.string "They're all invaluable POKéMON.$"

@ Unused
LittlerootTown_ProfessorBirchsLab_Text_PickedFinePokemon:
	.string "PROF. BIRCH: I see!\n"
	.string "You picked a fine POKéMON!$"

LittlerootTown_ProfessorBirchsLab_Text_ReceivedJohtoStarter:
	.string "{PLAYER} received the {STR_VAR_1}\n"
	.string "from PROF. BIRCH!$"

@ Unused
LittlerootTown_ProfessorBirchsLab_Text_NicknameJohtoStarter:
	.string "Want to give a nickname to\n"
	.string "the {STR_VAR_1} you received?$"

LittlerootTown_ProfessorBirchsLab_Text_GrassyPatchWaiting:
	.string "PROF. BIRCH: Listen, {PLAYER}{KUN}.\n"
	.string "You've completed the HOENN POKéDEX,\l"
	.string "but your POKéMON journey isn't over.\p"
	.string "There is no end to the road\n"
	.string "that is POKéMON.\p"
	.string "Somewhere, there is a grassy patch\n"
	.string "that's waiting for you!$"

LittlerootTown_ProfessorBirchsLab_Text_BetterLeaveOthersAlone:
	.string "You received the promised POKéMON.\n"
	.string "Better leave the others alone.$"

@ Unused
LittlerootTown_ProfessorBirchsLab_Text_DontHaveAnyRoomForPokemon:
	.string "Oh, you don't have any room for\n"
	.string "this POKéMON.$"

LittlerootTown_ProfessorBirchsLab_Text_MayWhatNextImStayingHere:
	.string "MAY: {PLAYER}{KUN}, after this…\n"
	.string "What are you going to do?\p"
	.string "Are you going to keep battling\n"
	.string "and sharpening your skills?\p"
	.string "Or are you going to try filling\n"
	.string "the NATIONAL POKéDEX?\p"
	.string "I'm staying here to help the PROF.$"

LittlerootTown_ProfessorBirchsLab_Text_BrendanPreferCollectingSlowly:
	.string "BRENDAN: Instead of catching everything,\n"
	.string "I like raising my chosen one slow\l"
	.string "and steady.$"

LittlerootTown_ProfessorBirchsLab_Text_MayHaveYouGoneToBattleFrontier:
	.string "MAY: Oh, hi, {PLAYER}{KUN}!\n"
	.string "Have you gone to that place,\l"
	.string "the BATTLE FRONTIER?$"

LittlerootTown_ProfessorBirchsLab_Text_BrendanHaveYouGoneToBattleFrontier:
	.string "BRENDAN: Yo, {PLAYER}!\n"
	.string "You been out to that place yet,\l"
	.string "the BATTLE FRONTIER?$"

LittlerootTown_ProfessorBirchsLab_Text_ScottAboardSSTidalCall:
	.string "… … … … … …\n"
	.string "… … … … … Beep!\p"
	.string "BRENDAN: Yo, yo, {PLAYER}!\n"
	.string "It's BRENDAN.\p"
	.string "I'm on the S.S. TIDAL now.\n"
	.string "Feels good being out at sea!\p"
	.string "By the way…\p"
	.string "There's a place I want you to\n"
	.string "come see.\p"
	.string "If you're interested, catch a ferry\n"
	.string "from SLATEPORT or LILYCOVE.\p"
	.string "I'll explain when we meet.\n"
	.string "I'll be waiting!\p"
	.string "… … … … … …\n"
	.string "… … … … … Click!$"

LittlerootTown_ProfessorBirchsLab_Text_BirchRestoringBundle:
	.string "PROF. BIRCH: Yo, {PLAYER}, my bad.\n"
	.string "Looks like your BaneMode bundle ain't\l"
	.string "fully loaded on this file yet.\p"
	.string "Lemme stock you back up proper.$"

LittlerootTown_ProfessorBirchsLab_Text_BirchRestoredBundleDone:
	.string "PROF. BIRCH: You good now, fam.\n"
	.string "Your TM CASE, mega stones, and evo\l"
	.string "gear should all be packed in.\p"
	.string "If your bag was tight, check your PC\n"
	.string "for the overflow stash.$"

@ ============================================================
@ Battle Items Distribution - checks and fills missing items
@ ============================================================

LittlerootTown_ProfessorBirchsLab_EventScript_GiveMissingBattleItems::
	@ Choice Items
	checkitem ITEM_CHOICE_BAND, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_ChoiceBand
	checkitem ITEM_CHOICE_SCARF, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_ChoiceScarf
	checkitem ITEM_CHOICE_SPECS, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_ChoiceSpecs
	@ Stat-Boosting Held Items
	checkitem ITEM_LIFE_ORB, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_LifeOrb
	checkitem ITEM_ASSAULT_VEST, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_AssaultVest
	checkitem ITEM_EXPERT_BELT, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_ExpertBelt
	checkitem ITEM_MUSCLE_BAND, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_MuscleBand
	checkitem ITEM_WISE_GLASSES, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_WiseGlasses
	checkitem ITEM_EVIOLITE, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_Eviolite
	checkitem ITEM_WEAKNESS_POLICY, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_WeaknessPolicy
	@ Survival / Defense
	checkitem ITEM_FOCUS_SASH, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_FocusSash
	checkitem ITEM_FOCUS_BAND, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_FocusBand
	checkitem ITEM_ROCKY_HELMET, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_RockyHelmet
	checkitem ITEM_WHITE_HERB, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_WhiteHerb
	checkitem ITEM_MENTAL_HERB, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_MentalHerb
	checkitem ITEM_POWER_HERB, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_PowerHerb
	@ HP Recovery
	checkitem ITEM_LEFTOVERS, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_Leftovers
	checkitem ITEM_SHELL_BELL, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_ShellBell
	checkitem ITEM_BIG_ROOT, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_BigRoot
	@ Status Orbs
	checkitem ITEM_FLAME_ORB, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_FlameOrb
	checkitem ITEM_TOXIC_ORB, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_ToxicOrb
	checkitem ITEM_BLACK_SLUDGE, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_BlackSludge
	@ Speed Items
	checkitem ITEM_QUICK_CLAW, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_QuickClaw
	checkitem ITEM_LAGGING_TAIL, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_LaggingTail
	checkitem ITEM_ZOOM_LENS, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_ZoomLens
	checkitem ITEM_ROOM_SERVICE, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_RoomService
	@ Accuracy Items
	checkitem ITEM_WIDE_LENS, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_WideLens
	checkitem ITEM_SCOPE_LENS, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_ScopeLens
	checkitem ITEM_BRIGHT_POWDER, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_BrightPowder
	checkitem ITEM_KINGS_ROCK, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_KingsRock
	@ Weather Rocks
	checkitem ITEM_HEAT_ROCK, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_HeatRock
	checkitem ITEM_DAMP_ROCK, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_DampRock
	checkitem ITEM_SMOOTH_ROCK, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_SmoothRock
	checkitem ITEM_ICY_ROCK, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_IcyRock
	checkitem ITEM_LIGHT_CLAY, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_LightClay
	checkitem ITEM_TERRAIN_EXTENDER, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_TerrainExtender
	@ Seeds
	checkitem ITEM_ELECTRIC_SEED, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_ElectricSeed
	checkitem ITEM_PSYCHIC_SEED, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_PsychicSeed
	checkitem ITEM_MISTY_SEED, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_MistySeed
	checkitem ITEM_GRASSY_SEED, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_GrassySeed
	@ Reactive Held Items
	checkitem ITEM_ABSORB_BULB, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_AbsorbBulb
	checkitem ITEM_CELL_BATTERY, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_CellBattery
	checkitem ITEM_LUMINOUS_MOSS, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_LuminousMoss
	checkitem ITEM_SNOWBALL, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_Snowball
	checkitem ITEM_ADRENALINE_ORB, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_AdrenalineOrb
	checkitem ITEM_THROAT_SPRAY, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_ThroatSpray
	checkitem ITEM_BLUNDER_POLICY, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_BlunderPolicy
	checkitem ITEM_EJECT_BUTTON, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_EjectButton
	checkitem ITEM_EJECT_PACK, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_EjectPack
	checkitem ITEM_RED_CARD, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_RedCard
	@ Utility / Competitive
	checkitem ITEM_AIR_BALLOON, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_AirBalloon
	checkitem ITEM_FLOAT_STONE, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_FloatStone
	checkitem ITEM_IRON_BALL, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_IronBall
	checkitem ITEM_GRIP_CLAW, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_GripClaw
	checkitem ITEM_BINDING_BAND, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_BindingBand
	checkitem ITEM_SHED_SHELL, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_ShedShell
	checkitem ITEM_STICKY_BARB, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_StickyBarb
	checkitem ITEM_RAZOR_CLAW, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_RazorClaw
	checkitem ITEM_RING_TARGET, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_RingTarget
	checkitem ITEM_SAFETY_GOGGLES, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_SafetyGoggles
	checkitem ITEM_HEAVY_DUTY_BOOTS, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_HeavyDutyBoots
	checkitem ITEM_PROTECTIVE_PADS, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_ProtectivePads
	checkitem ITEM_UTILITY_UMBRELLA, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_UtilityUmbrella
	checkitem ITEM_NORMAL_GEM, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_NormalGem
	@ Type-Boosting Items
	checkitem ITEM_SILK_SCARF, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_SilkScarf
	checkitem ITEM_CHARCOAL, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_Charcoal
	checkitem ITEM_MYSTIC_WATER, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_MysticWater
	checkitem ITEM_MAGNET, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_Magnet
	checkitem ITEM_MIRACLE_SEED, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_MiracleSeed
	checkitem ITEM_NEVER_MELT_ICE, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_NeverMeltIce
	checkitem ITEM_BLACK_BELT, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_BlackBelt
	checkitem ITEM_POISON_BARB, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_PoisonBarb
	checkitem ITEM_SOFT_SAND, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_SoftSand
	checkitem ITEM_SHARP_BEAK, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_SharpBeak
	checkitem ITEM_TWISTED_SPOON, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_TwistedSpoon
	checkitem ITEM_SILVER_POWDER, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_SilverPowder
	checkitem ITEM_HARD_STONE, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_HardStone
	checkitem ITEM_SPELL_TAG, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_SpellTag
	checkitem ITEM_DRAGON_FANG, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_DragonFang
	checkitem ITEM_BLACK_GLASSES, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_BlackGlasses
	checkitem ITEM_METAL_COAT, 1
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_Give_MetalCoat
	return

@ ---- Individual give subroutines ----
LittlerootTown_ProfessorBirchsLab_EventScript_Give_ChoiceBand::
	additem ITEM_CHOICE_BAND
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_ChoiceScarf::
	additem ITEM_CHOICE_SCARF
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_ChoiceSpecs::
	additem ITEM_CHOICE_SPECS
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_LifeOrb::
	additem ITEM_LIFE_ORB
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_AssaultVest::
	additem ITEM_ASSAULT_VEST
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_ExpertBelt::
	additem ITEM_EXPERT_BELT
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_MuscleBand::
	additem ITEM_MUSCLE_BAND
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_WiseGlasses::
	additem ITEM_WISE_GLASSES
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_Eviolite::
	additem ITEM_EVIOLITE
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_WeaknessPolicy::
	additem ITEM_WEAKNESS_POLICY
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_FocusSash::
	additem ITEM_FOCUS_SASH
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_FocusBand::
	additem ITEM_FOCUS_BAND
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_RockyHelmet::
	additem ITEM_ROCKY_HELMET
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_WhiteHerb::
	additem ITEM_WHITE_HERB
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_MentalHerb::
	additem ITEM_MENTAL_HERB
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_PowerHerb::
	additem ITEM_POWER_HERB
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_Leftovers::
	additem ITEM_LEFTOVERS
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_ShellBell::
	additem ITEM_SHELL_BELL
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_BigRoot::
	additem ITEM_BIG_ROOT
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_FlameOrb::
	additem ITEM_FLAME_ORB
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_ToxicOrb::
	additem ITEM_TOXIC_ORB
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_BlackSludge::
	additem ITEM_BLACK_SLUDGE
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_QuickClaw::
	additem ITEM_QUICK_CLAW
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_LaggingTail::
	additem ITEM_LAGGING_TAIL
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_ZoomLens::
	additem ITEM_ZOOM_LENS
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_RoomService::
	additem ITEM_ROOM_SERVICE
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_WideLens::
	additem ITEM_WIDE_LENS
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_ScopeLens::
	additem ITEM_SCOPE_LENS
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_BrightPowder::
	additem ITEM_BRIGHT_POWDER
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_KingsRock::
	additem ITEM_KINGS_ROCK
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_HeatRock::
	additem ITEM_HEAT_ROCK
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_DampRock::
	additem ITEM_DAMP_ROCK
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_SmoothRock::
	additem ITEM_SMOOTH_ROCK
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_IcyRock::
	additem ITEM_ICY_ROCK
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_LightClay::
	additem ITEM_LIGHT_CLAY
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_TerrainExtender::
	additem ITEM_TERRAIN_EXTENDER
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_ElectricSeed::
	additem ITEM_ELECTRIC_SEED
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_PsychicSeed::
	additem ITEM_PSYCHIC_SEED
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_MistySeed::
	additem ITEM_MISTY_SEED
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_GrassySeed::
	additem ITEM_GRASSY_SEED
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_AbsorbBulb::
	additem ITEM_ABSORB_BULB
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_CellBattery::
	additem ITEM_CELL_BATTERY
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_LuminousMoss::
	additem ITEM_LUMINOUS_MOSS
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_Snowball::
	additem ITEM_SNOWBALL
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_AdrenalineOrb::
	additem ITEM_ADRENALINE_ORB
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_ThroatSpray::
	additem ITEM_THROAT_SPRAY
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_BlunderPolicy::
	additem ITEM_BLUNDER_POLICY
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_EjectButton::
	additem ITEM_EJECT_BUTTON
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_EjectPack::
	additem ITEM_EJECT_PACK
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_RedCard::
	additem ITEM_RED_CARD
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_AirBalloon::
	additem ITEM_AIR_BALLOON
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_FloatStone::
	additem ITEM_FLOAT_STONE
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_IronBall::
	additem ITEM_IRON_BALL
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_GripClaw::
	additem ITEM_GRIP_CLAW
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_BindingBand::
	additem ITEM_BINDING_BAND
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_ShedShell::
	additem ITEM_SHED_SHELL
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_StickyBarb::
	additem ITEM_STICKY_BARB
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_RazorClaw::
	additem ITEM_RAZOR_CLAW
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_RingTarget::
	additem ITEM_RING_TARGET
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_SafetyGoggles::
	additem ITEM_SAFETY_GOGGLES
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_HeavyDutyBoots::
	additem ITEM_HEAVY_DUTY_BOOTS
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_ProtectivePads::
	additem ITEM_PROTECTIVE_PADS
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_UtilityUmbrella::
	additem ITEM_UTILITY_UMBRELLA
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_NormalGem::
	additem ITEM_NORMAL_GEM
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_SilkScarf::
	additem ITEM_SILK_SCARF
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_Charcoal::
	additem ITEM_CHARCOAL
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_MysticWater::
	additem ITEM_MYSTIC_WATER
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_Magnet::
	additem ITEM_MAGNET
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_MiracleSeed::
	additem ITEM_MIRACLE_SEED
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_NeverMeltIce::
	additem ITEM_NEVER_MELT_ICE
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_BlackBelt::
	additem ITEM_BLACK_BELT
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_PoisonBarb::
	additem ITEM_POISON_BARB
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_SoftSand::
	additem ITEM_SOFT_SAND
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_SharpBeak::
	additem ITEM_SHARP_BEAK
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_TwistedSpoon::
	additem ITEM_TWISTED_SPOON
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_SilverPowder::
	additem ITEM_SILVER_POWDER
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_HardStone::
	additem ITEM_HARD_STONE
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_SpellTag::
	additem ITEM_SPELL_TAG
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_DragonFang::
	additem ITEM_DRAGON_FANG
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_BlackGlasses::
	additem ITEM_BLACK_GLASSES
	return
LittlerootTown_ProfessorBirchsLab_EventScript_Give_MetalCoat::
	additem ITEM_METAL_COAT
	return
