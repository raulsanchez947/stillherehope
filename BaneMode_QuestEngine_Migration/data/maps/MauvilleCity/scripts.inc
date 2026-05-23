.set LOCALID_PAUL, 6
.set LOCALID_WALLY, LOCALID_PAUL
.set LOCALID_SCOTT, 10

MauvilleCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, MauvilleCity_OnTransition
	.byte 0

MauvilleCity_OnTransition:
	setflag FLAG_VISITED_MAUVILLE_CITY
	setflag FLAG_VISITED_ROUTE103
	setflag FLAG_VISITED_ROUTE110
	clearflag FLAG_FORCE_MIRAGE_TOWER_VISIBLE
	clearflag FLAG_HIDE_SLATEPORT_MUSEUM_POPULATION
	setflag FLAG_SYS_TV_START
	clearflag FLAG_MAUVILLE_GYM_BARRIERS_STATE
	setvar VAR_MAUVILLE_GYM_STATE, 0
	call_if_set FLAG_GOT_TM24_FROM_WATTSON, MauvilleCity_EventScript_MoveWattsonBackToGym
	@ Silently restore any missing BaneMode items for saves that missed the Birch trigger
	call_if_set FLAG_RECEIVED_POKEDEX_FROM_BIRCH, MauvilleCity_EventScript_TryRestoreLegacyBundle
	end

MauvilleCity_EventScript_MoveWattsonBackToGym::
	clearflag FLAG_HIDE_MAUVILLE_GYM_WATTSON
	setflag FLAG_HIDE_MAUVILLE_CITY_WATTSON
	setflag FLAG_WATTSON_REMATCH_AVAILABLE
	return

MauvilleCity_EventScript_Boy::
	msgbox MauvilleCity_Text_NurseHurtMonBackToHealth, MSGBOX_NPC
	end

MauvilleCity_EventScript_Maniac::
	msgbox MauvilleCity_Text_AllSortsOfPeopleComeThrough, MSGBOX_NPC
	end

MauvilleCity_EventScript_Woman::
	msgbox MauvilleCity_Text_RydelVeryGenerous, MSGBOX_NPC
	end

MauvilleCity_EventScript_RichBoy::
	msgbox MauvilleCity_Text_PokemonCanJumpYouOnBike, MSGBOX_NPC
	end

MauvilleCity_EventScript_CitySign::
	msgbox MauvilleCity_Text_CitySign, MSGBOX_SIGN
	end

MauvilleCity_EventScript_GymSign::
	msgbox MauvilleCity_Text_GymSign, MSGBOX_SIGN
	end

MauvilleCity_EventScript_BikeShopSign::
	msgbox MauvilleCity_Text_BikeShopSign, MSGBOX_SIGN
	end

MauvilleCity_EventScript_GameCornerSign::
	msgbox MauvilleCity_Text_GameCornerSign, MSGBOX_SIGN
	end

MauvilleCity_EventScript_SchoolKidM::
	lock
	faceplayer
	goto_if_set FLAG_TV_EXPLAINED, MauvilleCity_EventScript_TVExplained
	msgbox MauvilleCity_Text_ExplainTV, MSGBOX_DEFAULT
	setflag FLAG_TV_EXPLAINED
	release
	end

MauvilleCity_EventScript_TVExplained::
	msgbox MauvilleCity_Text_BeenCheckingOutTV, MSGBOX_DEFAULT
	release
	end

MauvilleCity_EventScript_Wally::
	goto MauvilleCity_EventScript_Paul

MauvilleCity_EventScript_Paul::
	lockall
	msgbox MauvilleCity_Text_WallyWillYouBattleMe, MSGBOX_DEFAULT
	goto MauvilleCity_EventScript_BattlePaul
	end

MauvilleCity_EventScript_BattlePaulPrompt::
	goto_if_eq VAR_RESULT, YES, MauvilleCity_EventScript_BattlePaul
	goto_if_eq VAR_RESULT, NO, MauvilleCity_EventScript_DeclinePaulBattle
	releaseall
	end

MauvilleCity_EventScript_PaulFlyExitEast::
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, MauvilleCity_Movement_PlayerWatchPaulFlyEast
	applymovement LOCALID_PAUL, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	delay 50
	setfieldeffectargument 0, 1
	dofieldeffect FLDEFF_NPCFLY_OUT
	delay 15
	removeobject LOCALID_PAUL
	waitfieldeffect FLDEFF_NPCFLY_OUT
	goto MauvilleCity_EventScript_DefeatedPaul
	end

MauvilleCity_EventScript_DefeatedPaul::
	clearflag FLAG_HIDE_VERDANTURF_TOWN_WANDAS_HOUSE_WALLY
	setflag FLAG_DEFEATED_WALLY_MAUVILLE
	setvar VAR_WALLY_CALL_STEP_COUNTER, 0
	setflag FLAG_ENABLE_FIRST_WALLY_POKENAV_CALL
	call_if_eq VAR_FACING, DIR_NORTH, MauvilleCity_EventScript_ScottApproachPlayerNorth
	call_if_eq VAR_FACING, DIR_EAST, MauvilleCity_EventScript_ScottApproachPlayerEast
	call_if_eq VAR_FACING, DIR_WEST, MauvilleCity_EventScript_ScottApproachPlayerNorth
	call_if_eq VAR_FACING, DIR_SOUTH, MauvilleCity_EventScript_ScottApproachPlayerNorth
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	msgbox MauvilleCity_Text_BrendanYouDidntHoldBack, MSGBOX_DEFAULT
	special HealPlayerParty
	trainerbattle_no_intro TRAINER_BRENDAN_PLACEHOLDER, MauvilleCity_Text_BrendanDefeat
	msgbox MauvilleCity_Text_BrendanPostBattle, MSGBOX_DEFAULT
	closemessage
	msgbox MauvilleCity_Text_BrendanAnomaly, MSGBOX_DEFAULT
	closemessage
	addvar VAR_SCOTT_STATE, 1
	call_if_eq VAR_FACING, DIR_NORTH, MauvilleCity_EventScript_ScottExitNorth
	call_if_eq VAR_FACING, DIR_EAST, MauvilleCity_EventScript_ScottExitEast
	call_if_eq VAR_FACING, DIR_WEST, MauvilleCity_EventScript_ScottExitNorth
	call_if_eq VAR_FACING, DIR_SOUTH, MauvilleCity_EventScript_ScottExitNorth
	removeobject LOCALID_SCOTT
	releaseall
	end

MauvilleCity_EventScript_ScottApproachPlayerNorth::
	addobject LOCALID_SCOTT
	applymovement LOCALID_SCOTT, MauvilleCity_Movement_ScottApproachPlayerNorth
	waitmovement 0
	return

MauvilleCity_EventScript_ScottApproachPlayerEast::
	setobjectxyperm LOCALID_SCOTT, 12, 13
	addobject LOCALID_SCOTT
	applymovement LOCALID_SCOTT, MauvilleCity_Movement_ScottApproachPlayerEast
	waitmovement 0
	return

MauvilleCity_EventScript_ScottExitNorth::
	applymovement OBJ_EVENT_ID_PLAYER, MauvilleCity_Movement_PlayerWatchScottExitNorth
	applymovement LOCALID_SCOTT, MauvilleCity_Movement_ScottExitNorth
	waitmovement 0
	return

MauvilleCity_EventScript_ScottExitEast::
	applymovement OBJ_EVENT_ID_PLAYER, MauvilleCity_Movement_PlayerWatchScottExitEast
	applymovement LOCALID_SCOTT, MauvilleCity_Movement_ScottExitEast
	waitmovement 0
	return

MauvilleCity_EventScript_BattlePaul::
	msgbox MauvilleCity_Text_WallyHereICome, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_WALLY_MAUVILLE, MauvilleCity_Text_WallyDefeat
	applymovement LOCALID_PAUL, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox MauvilleCity_Text_WallyIllGoBackToVerdanturf, MSGBOX_DEFAULT
	applymovement LOCALID_PAUL, Common_Movement_FacePlayer
	waitmovement 0
	msgbox MauvilleCity_Text_PaulMauvillePostBattle1, MSGBOX_DEFAULT
	applymovement LOCALID_PAUL, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox MauvilleCity_Text_PaulMauvillePostBattle2, MSGBOX_DEFAULT
	goto MauvilleCity_EventScript_PaulFlyExitEast
	end

MauvilleCity_EventScript_DeclinePaulBattle::
	setflag FLAG_DECLINED_WALLY_BATTLE_MAUVILLE
	msgbox MauvilleCity_Text_WallyMyUncleWontKnowImStrong, MSGBOX_DEFAULT
	releaseall
	end

MauvilleCity_EventScript_PaulRequestBattleAgain::
	applymovement LOCALID_PAUL, Common_Movement_FacePlayer
	waitmovement 0
	msgbox MauvilleCity_Text_WallyPleaseBattleMe, MSGBOX_YESNO
	goto MauvilleCity_EventScript_BattlePaulPrompt
	end

MauvilleCity_Movement_WallyExitNorth1:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

MauvilleCity_Movement_WallyExitEast1:
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end


MauvilleCity_Movement_PlayerWatchScottExitNorth:
	delay_16
	walk_in_place_faster_left
	step_end

MauvilleCity_Movement_PlayerWatchScottExitEast:
	delay_16
	delay_16
	walk_in_place_faster_left
	step_end

MauvilleCity_Movement_PlayerWatchPaulFlyEast:
	delay_8
	walk_in_place_faster_right
	step_end

MauvilleCity_Movement_PlayerWatchWallyExitNorth1:
	delay_8
	walk_in_place_faster_up
	step_end

MauvilleCity_Movement_ScottApproachPlayerNorth:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_up
	step_end

MauvilleCity_Movement_ScottApproachPlayerEast:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_up
	step_end

MauvilleCity_Movement_ScottExitNorth:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

MauvilleCity_Movement_ScottExitEast:
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

MauvilleCity_EventScript_Wattson::
	lock
	faceplayer
	goto_if_set FLAG_GOT_TM24_FROM_WATTSON, MauvilleCity_EventScript_ReceivedThunderbolt
	goto_if_eq VAR_NEW_MAUVILLE_STATE, 2, MauvilleCity_EventScript_CompletedNewMauville
	goto_if_set FLAG_GOT_BASEMENT_KEY_FROM_WATTSON, MauvilleCity_EventScript_BegunNewMauville
	msgbox MauvilleCity_Text_WattsonNeedFavorTakeKey, MSGBOX_DEFAULT
	giveitem ITEM_BASEMENT_KEY
	setflag FLAG_GOT_BASEMENT_KEY_FROM_WATTSON
	msgbox MauvilleCity_Text_WattsonWontBeChallenge, MSGBOX_DEFAULT
	release
	end

MauvilleCity_EventScript_BegunNewMauville::
	msgbox MauvilleCity_Text_WattsonWontBeChallenge, MSGBOX_DEFAULT
	release
	end

MauvilleCity_EventScript_CompletedNewMauville::
	msgbox MauvilleCity_Text_WattsonThanksTakeTM, MSGBOX_DEFAULT
	giveitem ITEM_TM24
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_GOT_TM24_FROM_WATTSON
	msgbox MauvilleCity_Text_WattsonYoungTakeCharge, MSGBOX_DEFAULT
	release
	end

MauvilleCity_EventScript_ReceivedThunderbolt::
	msgbox MauvilleCity_Text_WattsonYoungTakeCharge, MSGBOX_DEFAULT
	release
	end

MauvilleCity_EventScript_RegisterWallyCall::
	lockall
	pokenavcall MauvilleCity_Text_WallyPokenavCall
	waitmessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox MauvilleCity_Text_RegisteredWally, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_WALLY_MATCH_CALL
	clearflag FLAG_ENABLE_FIRST_WALLY_POKENAV_CALL
	releaseall
	end

MauvilleCity_Text_WallyWantToChallengeGym:
	.string "PAUL: I've been putting reps in all day.\p"
	.string "Before I step to this GYM, I gotta see\n"
	.string "if my squad is really official.$"

MauvilleCity_Text_PaulMauvilleWarmup:
	.string "PAUL: Rustboro made me tighten every\n"
	.string "slot on the roster.\p"
	.string "Now I need a real check before I start\n"
	.string "talking spicy to this city's leader.$"

MauvilleCity_Text_WallyWeCanBeatAnyone:
	.string "PAUL: I ain't forcing nothing.\p"
	.string "Me and my team can clip anybody if\n"
	.string "they're really outside.$"

MauvilleCity_Text_WallyWillYouBattleMe:
	.string "PAUL: Oh, it's you, {PLAYER}.\p"
	.string "Bet. Run it.$"

MauvilleCity_Text_WallyMyUncleWontKnowImStrong:
	.string "PAUL: If you duck me now, people gonna\n"
	.string "think I'm all talk.$"

MauvilleCity_Text_WallyPleaseBattleMe:
	.string "PAUL: Yo, {PLAYER}!\n"
	.string "Run that battle, deadass.$"

MauvilleCity_Text_WallyHereICome:
	.string "PAUL: Bet, {PLAYER}.\p"
	.string "Let's see what your squad really do.$"

MauvilleCity_Text_WallyDefeat:
	.string "PAUL: ...Tch.\p"
	.string "You deadass beat me.$"

MauvilleCity_Text_WallyIllGoBackToVerdanturf:
	.string "PAUL: Aight.\n"
	.string "I'm sliding back to VERDANTURF to tune\n"
	.string "the squad up some more.$"

MauvilleCity_Text_PaulMauvillePostBattle1:
	.string "PAUL: You don't just throw six mons on\n"
	.string "a roster and call it a day.\p"
	.string "Every slot gotta earn its keep.$"

MauvilleCity_Text_PaulMauvillePostBattle2:
	.string "PAUL: But this ain't over.\p"
	.string "Next time we line it up, I'm pulling up\n"
	.string "way meaner than this.$"

MauvilleCity_Text_WallyPokenavCall:
	.string "… … … … … …\n"
	.string "… … … … … Beep!\p"
	.string "PAUL: Yo, what's good, {PLAYER}?\p"
	.string "I finally got a POKéNAV on me.\p"
	.string "Now I can tap in whenever I want,\n"
	.string "my guy.\p"
	.string "… … … … … …\n"
	.string "… … … … … Click!$"

MauvilleCity_Text_RegisteredWally:
	.string "This number is blocked!$"

MauvilleCity_Text_BrendanYouDidntHoldBack:
	.string "BRENDAN: Yo, I seen the whole thing\n"
	.string "from the cut.\p"
	.string "You ain't let PAUL breathe for a\n"
	.string "second. That's Brooklyn pressure.\p"
	.string "Now don't get comfy, fam.\n"
	.string "Run that with me too.$"

MauvilleCity_Text_BrendanDefeat:
	.string "Aight, you got it.\n"
	.string "That was clean, no frontin'.$"

MauvilleCity_Text_BrendanPostBattle:
	.string "BRENDAN: Keep moving like that and\n"
	.string "Hoenn's gonna hear your name.\p"
	.string "I'll be watching from the block.\n"
	.string "Don't start moving soft now.$"

MauvilleCity_Text_BrendanAnomaly:
	.string "BRENDAN: Yo, one more thing.\p"
	.string "Heard a TEAM ROCKET grunt running\n"
	.string "his mouth outside the GYM.\p"
	.string "Something about an anomaly.\n"
	.string "Said they were tracking it.\p"
	.string "Didn't sound like any POKéMON\n"
	.string "I ever heard of.\p"
	.string "You see anything weird out there -\n"
	.string "pay attention. That's all I'm saying.$"

MauvilleCity_Text_WattsonNeedFavorTakeKey:
	.string "WATTSON: Oh, {PLAYER}{KUN}!\n"
	.string "You look like you have a lot of zip!\l"
	.string "That's a good thing, wahahahaha!\p"
	.string "Fine! I've decided!\n"
	.string "I need a favor, {PLAYER}{KUN}!\p"
	.string "MAUVILLE CITY has an underground\n"
	.string "sector called NEW MAUVILLE.\p"
	.string "{PLAYER}{KUN}, I'd like you to go there and\n"
	.string "switch off the GENERATOR.\p"
	.string "The GENERATOR has been running a bit\n"
	.string "haywire. It's getting unsafe.\p"
	.string "Here, this is the KEY to get into\n"
	.string "NEW MAUVILLE.$"

MauvilleCity_Text_WattsonWontBeChallenge:
	.string "WATTSON: Don't you worry about it.\n"
	.string "It won't be a challenge to you.\p"
	.string "The entrance to NEW MAUVILLE is just\n"
	.string "a short SURF away from ROUTE 110.\p"
	.string "That's it, then, you have my trust!\n"
	.string "Wahahahaha!$"

MauvilleCity_Text_WattsonThanksTakeTM:
	.string "WATTSON: Wahahahaha!\p"
	.string "I knew it, {PLAYER}{KUN}! I knew I'd made\n"
	.string "the right choice asking you!\p"
	.string "This is my thanks--a TM containing\n"
	.string "THUNDERBOLT!\p"
	.string "Go on, you've earned it!$"

MauvilleCity_Text_WattsonYoungTakeCharge:
	.string "WATTSON: Wahahahaha!\p"
	.string "It pleases me to no end to see\n"
	.string "the young step up and take charge!$"

MauvilleCity_Text_NurseHurtMonBackToHealth:
	.string "You know, it's cool to have POKéMON\n"
	.string "battles and stuff…\p"
	.string "But if your POKéMON gets hurt,\n"
	.string "you have to nurse it back to health.$"

MauvilleCity_Text_AllSortsOfPeopleComeThrough:
	.string "The roads of this town stretch north\n"
	.string "and south, and east and west.\p"
	.string "Because of that, we get all sorts of\n"
	.string "people coming through.$"

MauvilleCity_Text_RydelVeryGenerous:
	.string "Have you been to RYDEL'S CYCLES yet?\p"
	.string "RYDEL, the owner, is a very generous\n"
	.string "man.$"

MauvilleCity_Text_PokemonCanJumpYouOnBike:
	.string "Even if you're riding a BIKE,\n"
	.string "wild POKéMON could jump you.$"

MauvilleCity_Text_CitySign:
	.string "MAUVILLE CITY\n"
	.string "“The bright and shiny city of fun!”$"

MauvilleCity_Text_GymSign:
	.string "MAUVILLE CITY POKéMON GYM\n"
	.string "LEADER: WATTSON\l"
	.string "“The cheerfully electrifying man!”$"

MauvilleCity_Text_BikeShopSign:
	.string "“Ride in gravel and shake up your\n"
	.string "soul!”\l"
	.string "RYDEL'S CYCLES$"

MauvilleCity_Text_GameCornerSign:
	.string "“The play spot for all!”\n"
	.string "MAUVILLE GAME CORNER$"

MauvilleCity_Text_ExplainTV:
	.string "Hi, do you check out TV at all?\p"
	.string "They've added a bunch of cool new\n"
	.string "shows recently.\p"
	.string "TV is so cool. I've seen my friends on\n"
	.string "TV just out of the blue.\p"
	.string "And, sometimes, they even put you on TV\n"
	.string "without telling you.\p"
	.string "I wish I would be on TV sometime.\p"
	.string "That's why I think you should check\n"
	.string "out TVs whenever you can.$"

MauvilleCity_Text_BeenCheckingOutTV:
	.string "Hi, have you been checking out TVs?$"

@ ============================================================
@ BaneMode legacy bundle catch-up (runs silently on map entry)
@ ============================================================
@ Checks for two sentinel items that are always in the fresh bundle.
@ If either is missing the full restore runs; once they're present it no-ops.
MauvilleCity_EventScript_TryRestoreLegacyBundle::
	checkitem ITEM_TM01, 1
	goto_if_eq VAR_RESULT, FALSE, MauvilleCity_EventScript_RestoreLegacyBundle
	checkitem ITEM_BLACK_AUGURITE, 1
	goto_if_eq VAR_RESULT, FALSE, MauvilleCity_EventScript_RestoreLegacyBundle
	return

MauvilleCity_EventScript_RestoreLegacyBundle::
	special BaneModeGiveLegacyBirchBundle
	call LittlerootTown_ProfessorBirchsLab_EventScript_GiveMissingBattleItems
	return
