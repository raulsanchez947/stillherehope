.set LOCALID_HOENN_DEFENDER, 1
.set LOCALID_GIOVANNI, 2
.set LOCALID_ROCKET_ADMIN, 3
.set LOCALID_ROCKET_GRUNT_2, 22
.set LOCALID_ROCKET_GRUNT_1, 29


MtChimney_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, MtChimney_OnResume
	map_script MAP_SCRIPT_ON_TRANSITION, MtChimney_OnTransition
	.byte 0

MtChimney_OnTransition:
	setvar VAR_JAGGED_PASS_ASH_WEATHER, 1
	setflag FLAG_VISITED_MT_CHIMNEY
	call Event_TryGenerateLegend
	end

MtChimney_OnResume:
	setstepcallback STEP_CB_ASH
	end

MtChimney_EventScript_Archie::
	lock
	faceplayer
	call_if_unset FLAG_EVIL_LEADER_PLEASE_STOP, MtChimney_EventScript_ArchieGoStopTeamMagma
	call_if_set FLAG_EVIL_LEADER_PLEASE_STOP, MtChimney_EventScript_ArchieBusyFighting
	closemessage
	applymovement LOCALID_HOENN_DEFENDER, Common_Movement_FaceOriginalDirection
	waitmovement 0
	setflag FLAG_EVIL_LEADER_PLEASE_STOP
	release
	end

MtChimney_EventScript_DefenderAnabel::
	lock
	faceplayer
	call_if_unset FLAG_EVIL_LEADER_PLEASE_STOP, MtChimney_EventScript_ArchieGoStopTeamMagma
	call_if_set FLAG_EVIL_LEADER_PLEASE_STOP, MtChimney_EventScript_ArchieBusyFighting
	closemessage
	applymovement LOCALID_HOENN_DEFENDER, Common_Movement_FaceOriginalDirection
	waitmovement 0
	setflag FLAG_EVIL_LEADER_PLEASE_STOP
	release
	end

MtChimney_EventScript_ArchieGoStopTeamMagma::
	msgbox MtChimney_Text_ArchieGoStopTeamMagma, MSGBOX_DEFAULT
	return

MtChimney_EventScript_ArchieBusyFighting::
	msgbox MtChimney_Text_ArchieIHaveMyHandsFull, MSGBOX_DEFAULT
	return

MtChimney_EventScript_Maxie::
	lockall
	playbgm MUS_RG_ENCOUNTER_ROCKET, FALSE
	msgbox MtChimney_Text_MeteoriteWillActivateVolcano, MSGBOX_DEFAULT
	applymovement LOCALID_GIOVANNI, Common_Movement_FacePlayer
	waitmovement 0
	playse SE_PIN
	applymovement LOCALID_GIOVANNI, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_GIOVANNI, Common_Movement_Delay48
	waitmovement 0
	msgbox MtChimney_Text_MaxieIntro, MSGBOX_DEFAULT
	msgbox MtChimney_Text_StevenJoinsGiovanniBattle, MSGBOX_DEFAULT
	special SavePlayerParty
	fadescreen FADE_TO_BLACK
	special ChooseHalfPartyForBattle
	waitstate
	goto_if_ne VAR_RESULT, 0, MtChimney_EventScript_DoStevenGiovanniBattle
	special LoadPlayerParty
	releaseall
	end

MtChimney_EventScript_DoStevenGiovanniBattle::
	special ReducePlayerPartyToSelectedMons
	frontier_set FRONTIER_DATA_SELECTED_MON_ORDER
	setvar VAR_0x8004, SPECIAL_BATTLE_MT_CHIMNEY_STEVEN
	setvar VAR_0x8005, 0
	special DoSpecialTrainerBattle
	waitstate
	frontier_saveparty
	special LoadPlayerParty
	switch VAR_RESULT
	case 1, MtChimney_EventScript_DefeatedGiovanniWithSteven
	fadescreen FADE_TO_BLACK
	special SetCB2WhiteOut
	waitstate

MtChimney_EventScript_DefeatedGiovanniWithSteven::
	msgbox MtChimney_Text_MaxieYouHaventSeenLastOfMagma, MSGBOX_DEFAULT
	closemessage
	delay 30
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_GIOVANNI
	removeobject LOCALID_ROCKET_GRUNT_1
	removeobject LOCALID_ROCKET_ADMIN
	removeobject LOCALID_ROCKET_GRUNT_2
	setflag FLAG_HIDE_MT_CHIMNEY_TEAM_MAGMA
	fadescreenswapbuffers FADE_FROM_BLACK
	setobjectxyperm LOCALID_HOENN_DEFENDER, 10, 12
	addobject LOCALID_HOENN_DEFENDER
	call_if_eq VAR_FACING, DIR_EAST, MtChimney_EventScript_ArchieApproachPlayerEast
	call_if_eq VAR_FACING, DIR_NORTH, MtChimney_EventScript_ArchieApproachPlayerNorth
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	msgbox MtChimney_Text_ArchieThankYou, MSGBOX_DEFAULT
	msgbox MtChimney_Text_LanceFindsOrbAtMachine, MSGBOX_DEFAULT
	giveitem ITEM_RED_ORB
	closemessage
	call_if_eq VAR_FACING, DIR_EAST, MtChimney_EventScript_ArchieExitEast
	call_if_eq VAR_FACING, DIR_NORTH, MtChimney_EventScript_ArchieExitNorth
	removeobject LOCALID_HOENN_DEFENDER
	setflag FLAG_HIDE_MT_CHIMNEY_TEAM_AQUA
	setflag FLAG_DEFEATED_EVIL_TEAM_MT_CHIMNEY
	clearflag FLAG_HIDE_FALLARBOR_HOUSE_PROF_COZMO
	setflag FLAG_HIDE_METEOR_FALLS_1F_1R_COZMO
	clearflag FLAG_HIDE_MT_CHIMNEY_LAVA_COOKIE_LADY
	releaseall
	end

MtChimney_EventScript_GiovanniTrainer::
	trainerbattle TRAINER_BATTLE_SET_TRAINER_A, TRAINER_MAXIE_MT_CHIMNEY, 0, MtChimney_Text_MaxieDefeat, MtChimney_Text_MaxieDefeat
	end

MtChimney_EventScript_RocketAdminTrainer::
	trainerbattle TRAINER_BATTLE_SET_TRAINER_B, TRAINER_TABITHA_MT_CHIMNEY, 0, MtChimney_Text_TabithaDefeat, MtChimney_Text_TabithaDefeat
	end

MtChimney_EventScript_ArchieApproachPlayerEast::
	applymovement LOCALID_HOENN_DEFENDER, MtChimney_Movement_ArchieApproachPlayerEast
	waitmovement 0
	return

MtChimney_EventScript_ArchieApproachPlayerNorth::
	applymovement LOCALID_HOENN_DEFENDER, MtChimney_Movement_ArchieApproachPlayerNorth
	waitmovement 0
	return

MtChimney_EventScript_ArchieExitEast::
	applymovement LOCALID_HOENN_DEFENDER, MtChimney_Movement_ArchieExitEast
	waitmovement 0
	return

MtChimney_EventScript_ArchieExitNorth::
	applymovement LOCALID_HOENN_DEFENDER, MtChimney_Movement_ArchieExitNorth
	waitmovement 0
	return

MtChimney_EventScript_LavaCookieLady::
	lock
	faceplayer
	showmoneybox 0, 0
	msgbox MtChimney_Text_LavaCookiesJust200, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, MtChimney_EventScript_DeclineLavaCookie
	checkmoney 200
	goto_if_eq VAR_RESULT, FALSE, MtChimney_EventScript_NotEnoughMoney
	msgbox MtChimney_Text_ThankYouDear, MSGBOX_DEFAULT
	checkitemspace ITEM_LAVA_COOKIE
	call_if_eq VAR_RESULT, TRUE, MtChimney_EventScript_RemoveMoney
	hidemoneybox
	giveitem ITEM_LAVA_COOKIE
	goto_if_eq VAR_RESULT, FALSE, MtChimney_EventScript_BagIsFull
	release
	end

MtChimney_EventScript_BagIsFull::
	msgbox gText_TooBadBagIsFull, MSGBOX_DEFAULT
	@hidemoneybox
	release
	end

MtChimney_EventScript_RemoveMoney::
	removemoney 200
	updatemoneybox
	return

MtChimney_EventScript_DeclineLavaCookie::
	msgbox MtChimney_Text_OhFineThen, MSGBOX_DEFAULT
	hidemoneybox
	release
	end

MtChimney_EventScript_NotEnoughMoney::
	msgbox MtChimney_Text_YouveNotGotTheMoney, MSGBOX_DEFAULT
	hidemoneybox
	release
	end

MtChimney_Movement_ArchieApproachPlayerEast:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	step_end

MtChimney_Movement_ArchieExitEast:
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

MtChimney_Movement_ArchieApproachPlayerNorth:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	walk_right
	step_end

MtChimney_Movement_ArchieExitNorth:
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

MtChimney_Movement_Unused1:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	step_end

MtChimney_Movement_Unused2:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_down
	step_end

MtChimney_Movement_Unused3:
	walk_right
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

MtChimney_Movement_Unused4:
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	step_end

MtChimney_Movement_Unused5:
	delay_16
	delay_16
	delay_16
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_down
	step_end

MtChimney_Movement_Unused6:
	delay_16
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

MtChimney_Movement_Unused7:
	delay_16
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	step_end

MtChimney_Movement_Unused8:
	delay_16
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_down
	step_end

MtChimney_Movement_Unused9:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

MtChimney_Movement_Unused10:
	walk_down
	step_end

MtChimney_Movement_Unused11:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

MtChimney_Movement_Unused12:
	delay_16
	delay_16
	walk_in_place_faster_left
	delay_16
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

MtChimney_Movement_Unused13:
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	walk_in_place_faster_left
	delay_16
	walk_down
	walk_down
	step_end

MtChimney_Movement_Unused14:
	lock_facing_direction
	walk_fast_left
	unlock_facing_direction
	face_right
	step_end

MtChimney_Movement_Unused15:
	walk_left
	walk_in_place_faster_right
	delay_16
	step_end

MtChimney_EventScript_BusyAquaGrunt1::
	msgbox MtChimney_Text_MagmaOutnumbersUs, MSGBOX_SIGN
	end

MtChimney_EventScript_BusyAquaGrunt2::
	msgbox MtChimney_Text_LessHabitatForWaterPokemon, MSGBOX_SIGN
	end

MtChimney_EventScript_BusyAquaGrunt3::
	msgbox MtChimney_Text_MagmasNameSimilar, MSGBOX_SIGN
	end

MtChimney_EventScript_Tabitha::
	trainerbattle_single TRAINER_TABITHA_MT_CHIMNEY, MtChimney_Text_TabithaIntro, MtChimney_Text_TabithaDefeat
	msgbox MtChimney_Text_TabithaPostBattle, MSGBOX_AUTOCLOSE
	end

@ Note: TRAINER_TABITHA_MT_CHIMNEY is used here as a Rocket Admin blocking the player.

MtChimney_EventScript_Grunt2::
	trainerbattle_single TRAINER_GRUNT_MT_CHIMNEY_2, MtChimney_Text_Grunt2Intro, MtChimney_Text_Grunt2Defeat
	msgbox MtChimney_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	end

MtChimney_EventScript_BusyMagmaGrunt1::
	msgbox MtChimney_Text_TeamAquaAlwaysMessingWithPlans, MSGBOX_SIGN
	end

MtChimney_EventScript_BusyMagmaGrunt2::
	msgbox MtChimney_Text_MeteoritesPackAmazingPower, MSGBOX_SIGN
	end

MtChimney_EventScript_BusyMagmaGrunt3::
	msgbox MtChimney_Text_YouBetterNotMessWithUs, MSGBOX_SIGN
	end

MtChimney_EventScript_BusyMagmaGrunt4::
	msgbox MtChimney_Text_AquasNameSimilar, MSGBOX_SIGN
	end

MtChimney_EventScript_BusyMagmaGrunt5::
	msgbox MtChimney_Text_DouseThemInFire, MSGBOX_SIGN
	end

MtChimney_EventScript_BusyMagmaGrunt6::
	msgbox MtChimney_Text_KeepMakingMoreLand, MSGBOX_SIGN
	end

MtChimney_EventScript_DefenderLiza::
	msgbox MtChimney_Text_DefenderLiza, MSGBOX_SIGN
	end

MtChimney_EventScript_DefenderTate::
	msgbox MtChimney_Text_DefenderTate, MSGBOX_SIGN
	end

MtChimney_EventScript_DefenderSpenser::
	msgbox MtChimney_Text_DefenderSpenser, MSGBOX_SIGN
	end

MtChimney_EventScript_DefenderSteven::
	msgbox MtChimney_Text_DefenderSteven, MSGBOX_SIGN
	end

MtChimney_EventScript_MagmaPoochyena::
	msgbox MtChimney_Text_Bufoh, MSGBOX_SIGN
	end

MtChimney_EventScript_AquaPoochyena::
	msgbox MtChimney_Text_Bushaa, MSGBOX_SIGN
	end

MtChimney_EventScript_MeteoriteMachine::
	lockall
	goto_if_unset FLAG_DEFEATED_EVIL_TEAM_MT_CHIMNEY, MtChimney_EventScript_MachineOn
	goto_if_set FLAG_RECEIVED_METEORITE, MtChimney_EventScript_MachineOff
	msgbox MtChimney_Text_RemoveTheMeteorite, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, MtChimney_EventScript_LeaveMeteoriteAlone
	msgbox MtChimney_Text_PlayerRemovedMeteorite, MSGBOX_DEFAULT
	giveitem ITEM_METEORITE
	setflag FLAG_RECEIVED_METEORITE
	releaseall
	end

MtChimney_EventScript_LeaveMeteoriteAlone::
	msgbox MtChimney_Text_PlayerLeftMeteorite, MSGBOX_DEFAULT
	releaseall
	end

MtChimney_EventScript_MachineOff::
	msgbox MtChimney_Text_MachineMakesNoResponse, MSGBOX_DEFAULT
	releaseall
	end

MtChimney_EventScript_MachineOn::
	msgbox MtChimney_Text_MetoriteFittedOnMachine, MSGBOX_DEFAULT
	releaseall
	end

MtChimney_EventScript_RouteSign::
	msgbox MtChimney_Text_RouteSign, MSGBOX_SIGN
	end

MtChimney_EventScript_Shelby::
	trainerbattle_single TRAINER_SHELBY_1, MtChimney_Text_ShelbyIntro, MtChimney_Text_ShelbyDefeat, MtChimney_EventScript_DefeatedShelby
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, MtChimney_EventScript_RematchShelby
	msgbox MtChimney_Text_ShelbyPostBattle, MSGBOX_DEFAULT
	release
	end

MtChimney_EventScript_DefeatedShelby::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox MtChimney_Text_ShelbyRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_SHELBY_1
	release
	end

MtChimney_EventScript_RematchShelby::
	trainerbattle_rematch TRAINER_SHELBY_1, MtChimney_Text_ShelbyRematchIntro, MtChimney_Text_ShelbyRematchDefeat
	msgbox MtChimney_Text_ShelbyPostRematch, MSGBOX_AUTOCLOSE
	end

MtChimney_EventScript_Melissa::
	trainerbattle_single TRAINER_MELISSA, MtChimney_Text_MelissaIntro, MtChimney_Text_MelissaDefeat
	msgbox MtChimney_Text_MelissaPostBattle, MSGBOX_AUTOCLOSE
	end

MtChimney_EventScript_Sheila::
	trainerbattle_single TRAINER_SHEILA, MtChimney_Text_SheilaIntro, MtChimney_Text_SheilaDefeat
	msgbox MtChimney_Text_SheilaPostBattle, MSGBOX_AUTOCLOSE
	end

MtChimney_EventScript_Shirley::
	trainerbattle_single TRAINER_SHIRLEY, MtChimney_Text_ShirleyIntro, MtChimney_Text_ShirleyDefeat
	msgbox MtChimney_Text_ShirleyPostBattle, MSGBOX_AUTOCLOSE
	end

MtChimney_EventScript_Grunt1::
	trainerbattle_single TRAINER_GRUNT_MT_CHIMNEY_1, MtChimney_Text_Grunt1Intro, MtChimney_Text_Grunt1Defeat
	msgbox MtChimney_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

MtChimney_EventScript_Sawyer::
	trainerbattle_single TRAINER_SAWYER_1, MtChimney_Text_SawyerIntro, MtChimney_Text_SawyerDefeat, MtChimney_EventScript_SawyerDefeated
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, MtChimney_EventScript_SawyerRematch
	msgbox MtChimney_Text_SawyerPostBattle, MSGBOX_DEFAULT
	release
	end

MtChimney_EventScript_SawyerDefeated::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox MtChimney_Text_SawyerRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_SAWYER_1
	release
	end

MtChimney_EventScript_SawyerRematch::
	trainerbattle_rematch TRAINER_SAWYER_1, MtChimney_Text_SawyerRematchIntro, MtChimney_Text_SawyerRematchDefeat
	msgbox MtChimney_Text_SawyerPostRematch, MSGBOX_AUTOCLOSE
	end

MtChimney_Text_MeteoriteWillActivateVolcano:
	.string "The METEORITE's energy…\p"
	.string "Channelled through this machine,\n"
	.string "it will drive MT. CHIMNEY's volcanic\l"
	.string "activity to a critical point.\p"
	.string "The eruption will level everything\n"
	.string "in this region.\l"
	.string "…And TEAM ROCKET will be there\n"
	.string "to fill the vacuum.$"

MtChimney_Text_MaxieIntro:
	.string "GIOVANNI: Hm.\n"
	.string "A child?\p"
	.string "… … … … … …\p"
	.string "You've been making quite a nuisance\n"
	.string "of yourself, {PLAYER}.\p"
	.string "I was told someone had been\n"
	.string "interfering with TEAM ROCKET's\l"
	.string "operations across HOENN.\p"
	.string "I simply didn't believe a trainer\n"
	.string "your age could be this persistent.\p"
	.string "Let me explain something.\n"
	.string "Power is the only language\l"
	.string "the world understands.\p"
	.string "TEAM ROCKET does not pursue goals.\n"
	.string "We pursue dominance.\p"
	.string "This mountain will erupt.\n"
	.string "HOENN will fracture.\p"
	.string "And in the chaos, TEAM ROCKET\n"
	.string "will rise.\p"
	.string "You should not have come here.\n"
	.string "I will not go easy on you.$"

MtChimney_Text_StevenJoinsGiovanniBattle:
	.string "STEVEN: You won't face him alone,\n"
	.string "{PLAYER}{KUN}.\p"
	.string "LANCE and the HOENN DEFENDERS\n"
	.string "are holding the ROCKET line.\p"
	.string "I'll fight beside you.\n"
	.string "Together, we stop GIOVANNI here!$"

MtChimney_Text_MaxieDefeat:
	.string "What?!\p"
	.string "I, GIOVANNI, lost to a child?!$"

MtChimney_Text_MaxieYouHaventSeenLastOfMagma:
	.string "GIOVANNI: …Enough.\n"
	.string "I'll withdraw for now.\p"
	.string "But don't mistake this for a defeat.\p"
	.string "TEAM ROCKET is patient.\n"
	.string "We have plans within plans.\p"
	.string "Enjoy this moment.\n"
	.string "It won't last.$"

MtChimney_Text_TabithaIntro:
	.string "Heh!\p"
	.string "You made it this far.\n"
	.string "Impressive.\p"
	.string "But the BOSS already has the\n"
	.string "METEORITE powering the machine.\p"
	.string "You're too late to stop anything!\n"
	.string "TEAM ROCKET wins either way!$"

MtChimney_Text_TabithaDefeat:
	.string "Tch…\p"
	.string "Even if I fall here, the BOSS\n"
	.string "will see this through…$"

MtChimney_Text_TabithaPostBattle:
	.string "BOSS, the machine!\n"
	.string "Don't stop now!$"

MtChimney_Text_Grunt2Intro:
	.string "TEAM ROCKET doesn't take orders\n"
	.string "from some kid!\p"
	.string "The BOSS has a plan and we're\n"
	.string "seeing it through!\p"
	.string "Get out of the way!$"

MtChimney_Text_Grunt2Defeat:
	.string "Wha-?!\n"
	.string "That's not possible!$"

MtChimney_Text_Grunt2PostBattle:
	.string "The BOSS won't be pleased…\n"
	.string "But at least the machine's still on.$"

MtChimney_Text_Grunt1Intro:
	.string "You've got nerve, coming up here!\n"
	.string "TEAM ROCKET runs this mountain now!\p"
	.string "I'll show you what that means!$"

MtChimney_Text_Grunt1Defeat:
	.string "No way… Beaten by a trainer?!$"

MtChimney_Text_Grunt1PostBattle:
	.string "You beat me but you won't stop\n"
	.string "what's already in motion!$"

MtChimney_Text_TeamAquaAlwaysMessingWithPlans:
	.string "TEAM ROCKET…\n"
	.string "We knew they'd make a move eventually!$"

MtChimney_Text_MeteoritesPackAmazingPower:
	.string "The METEORITE's energy is immense.\n"
	.string "We can't let ROCKET exploit it!$"

MtChimney_Text_YouBetterNotMessWithUs:
	.string "Hold your ground!\n"
	.string "We defend HOENN here!$"

MtChimney_Text_AquasNameSimilar:
	.string "HOENN's people, land, and sea-\n"
	.string "all of it must be protected.\p"
	.string "That's what the HOENN DEFENDERS\n"
	.string "stand for!$"

MtChimney_Text_DouseThemInFire:
	.string "Push them back!\n"
	.string "Don't let a single ROCKET through!$"

MtChimney_Text_KeepMakingMoreLand:
	.string "We won't let ROCKET turn this\n"
	.string "mountain into a weapon!$"

MtChimney_Text_DefenderLiza:
	.string "LIZA: TATE and I are holding this\n"
	.string "side of the line.\p"
	.string "TEAM ROCKET doesn't get one step\n"
	.string "closer to that machine!$"

MtChimney_Text_DefenderTate:
	.string "TATE: LIZA can feel the pressure\n"
	.string "building from here.\p"
	.string "If ROCKET keeps pushing the\n"
	.string "METEORITE, the volcano could blow!$"

MtChimney_Text_DefenderSpenser:
	.string "SPENSER: Do not mistake defense\n"
	.string "for weakness.\p"
	.string "The HOENN DEFENDERS will weather\n"
	.string "this storm and break ROCKET's line.$"

MtChimney_Text_DefenderSteven:
	.string "STEVEN: I'm keeping these ROCKETS\n"
	.string "from reaching ANABEL.\p"
	.string "When you reach GIOVANNI, I will\n"
	.string "fight beside you.$"

MtChimney_Text_Bufoh:
	.string "Bufoh!$"

MtChimney_Text_ArchieGoStopTeamMagma:
	.string "ANABEL: {PLAYER}!\n"
	.string "Good - I hoped you'd come.\p"
	.string "TEAM ROCKET has stolen a METEORITE\n"
	.string "and fitted it to a machine\l"
	.string "on this summit.\p"
	.string "If they finish, the energy feedback\n"
	.string "will trigger a full eruption.\p"
	.string "GIOVANNI is up there himself.\n"
	.string "Go - stop him!$"

MtChimney_Text_ArchieIHaveMyHandsFull:
	.string "ANABEL: Grrr…\p"
	.string "I need to reach GIOVANNI\n"
	.string "before it's too late!\p"
	.string "But TEAM ROCKET has me pinned!\n"
	.string "I'm fighting three at once!\p"
	.string "You'll have to push through yourself!$"

MtChimney_Text_ArchieThankYou:
	.string "ANABEL: {PLAYER}!\n"
	.string "You did it!\p"
	.string "Thanks to you, GIOVANNI has retreated\n"
	.string "and the machine has been shut down.\p"
	.string "MT. CHIMNEY is safe.\p"
	.string "The HOENN DEFENDERS owe you one,\n"
	.string "{PLAYER}.\p"
	.string "LANCE and I will keep pursuing\n"
	.string "TEAM ROCKET.\p"
	.string "This battle is far from over.$"

MtChimney_Text_LanceFindsOrbAtMachine:
	.string "STEVEN: Wait - what's this?\p"
	.string "GIOVANNI dropped something near\n"
	.string "the machine…\p"
	.string "The CINDER ORB.\n"
	.string "It resonates with Galarian MOLTRES.\p"
	.string "Take it, {PLAYER}.\n"
	.string "It may matter later.$"

MtChimney_Text_MagmaOutnumbersUs:
	.string "These HOENN DEFENDERS are tougher\n"
	.string "than the boss said they'd be!$"

MtChimney_Text_LessHabitatForWaterPokemon:
	.string "Once that machine blows, no one will\n"
	.string "be able to stop TEAM ROCKET!$"

MtChimney_Text_MagmasNameSimilar:
	.string "TEAM ROCKET is the strongest\n"
	.string "organization in the world!\p"
	.string "These HOENN amateurs don't stand\n"
	.string "a chance against us!$"

MtChimney_Text_Bushaa:
	.string "Bushaa!$"

MtChimney_Text_LavaCookiesJust200:
	.string "LAVA COOKIES are MT. CHIMNEY's local\n"
	.string "specialty.\p"
	.string "Try one. It's just ¥200.$"

MtChimney_Text_ThankYouDear:
	.string "Thank you, dear!$"

MtChimney_Text_YouveNotGotTheMoney:
	.string "Oh, dear. You can't buy a thing if\n"
	.string "you've not got the money.$"

MtChimney_Text_OhFineThen:
	.string "Oh, fine then.$"

MtChimney_Text_MetoriteFittedOnMachine:
	.string "A METEORITE is fitted on a mysterious\n"
	.string "machine…\p"
	.string "The machine seems to be storing\n"
	.string "energy in the METEORITE.$"

MtChimney_Text_RemoveTheMeteorite:
	.string "A METEORITE is fitted on a mysterious\n"
	.string "machine…\p"
	.string "Do you want to remove the METEORITE?$"

MtChimney_Text_PlayerRemovedMeteorite:
	.string "{PLAYER} removed the METEORITE from\n"
	.string "the mysterious machine.$"

MtChimney_Text_PlayerLeftMeteorite:
	.string "{PLAYER} left the METEORITE where\n"
	.string "it was.$"

MtChimney_Text_MachineMakesNoResponse:
	.string "This mysterious machine…\n"
	.string "It makes no response whatsoever.$"

MtChimney_Text_RouteSign:
	.string "{DOWN_ARROW} JAGGED PATH\n"
	.string "LAVARIDGE TOWN AHEAD$"

MtChimney_Text_ShelbyIntro:
	.string "I've been to the hot springs and\n"
	.string "refreshed my tired bones.\l"
	.string "Right now I'm feeling strong!$"

MtChimney_Text_ShelbyDefeat:
	.string "Oh, my goodness.\n"
	.string "Now, aren't you something!$"

MtChimney_Text_ShelbyPostBattle:
	.string "Well, well, I've lost. I can't call\n"
	.string "myself an EXPERT now, can I?$"

MtChimney_Text_ShelbyRegister:
	.string "Thank you, child. It was fun, as if\n"
	.string "I were battling my own grandchild.\p"
	.string "Please, come see me again for\n"
	.string "a rematch.$"

MtChimney_Text_ShelbyRematchIntro:
	.string "If you can mesh your heart with those\n"
	.string "of your POKéMON, why, you should be\l"
	.string "able to achieve great things.$"

MtChimney_Text_ShelbyRematchDefeat:
	.string "Oh, my goodness.\n"
	.string "Now, aren't you something!$"

MtChimney_Text_ShelbyPostRematch:
	.string "Perhaps your heart has become one\n"
	.string "with the hearts of your POKéMON.$"

MtChimney_Text_MelissaIntro:
	.string "I've got the fire in me, baby.\n"
	.string "I can't stand it! I have to battle!$"

MtChimney_Text_MelissaDefeat:
	.string "Ooh, that was a scorching-hot match!$"

MtChimney_Text_MelissaPostBattle:
	.string "The heat of MT. CHIMNEY warms\n"
	.string "me up, baby!$"

MtChimney_Text_SheilaIntro:
	.string "I've finally made it to MT. CHIMNEY.\n"
	.string "I want to make my POKéMON battle!$"

MtChimney_Text_SheilaDefeat:
	.string "The way you battle…\n"
	.string "It's like a MT. CHIMNEY eruption!$"

MtChimney_Text_SheilaPostBattle:
	.string "Like I said, I've finally made it to\n"
	.string "MT. CHIMNEY. It would be a shame if\l"
	.string "I only do a little sightseeing…\p"
	.string "I want to get in some battles and buy\n"
	.string "COOKIES as souvenirs.$"

MtChimney_Text_ShirleyIntro:
	.string "Since I bathed in the hot springs,\n"
	.string "I've been feeling great!\l"
	.string "I'm sure I'm going to win!$"

MtChimney_Text_ShirleyDefeat:
	.string "Yowch!\n"
	.string "I'm getting a chill out of the water.$"

MtChimney_Text_ShirleyPostBattle:
	.string "I'll have to take another dip in the\n"
	.string "hot springs. Want to join me?\p"
	.string "Just joking!$"

MtChimney_Text_SawyerIntro:
	.string "This is one fine mountain! Plenty of\n"
	.string "hot people around for company!$"

MtChimney_Text_SawyerDefeat:
	.string "Oh, you're a real firebrand, too!$"

MtChimney_Text_SawyerPostBattle:
	.string "I think I need a dip in LAVARIDGE\n"
	.string "HOT SPRING with the locals!$"

MtChimney_Text_SawyerRegister:
	.string "I like little fireballs like you.\n"
	.string "Let me register you in my POKéNAV.$"

MtChimney_Text_SawyerRematchIntro:
	.string "I'm happily surrounded by hot people\n"
	.string "around these parts. I won't lose!$"

MtChimney_Text_SawyerRematchDefeat:
	.string "Gosh, you're still the same\n"
	.string "firebrand as before!$"

MtChimney_Text_SawyerPostRematch:
	.string "Actually, it really is hot here.\n"
	.string "I'm overdressed for these parts.$"
