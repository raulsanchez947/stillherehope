.set LOCALID_WINONA, 1
.set LOCALID_GYM_GUIDE, 2

LavaridgeTown_Gym_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, LavaridgeTown_Gym_1F_OnTransition
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, LavaridgeTown_Gym_1F_OnWarp
	.byte 0

LavaridgeTown_Gym_1F_OnTransition:
	special RotatingGate_InitPuzzle
	setflag FLAG_TEMP_HIDE_FOLLOWER
	end

LavaridgeTown_Gym_1F_OnWarp:
	map_script_2 VAR_TEMP_0, VAR_TEMP_0, LavaridgeTown_Gym_1F_EventScript_InitRotatingGates
	.2byte 0

LavaridgeTown_Gym_1F_EventScript_InitRotatingGates::
	special RotatingGate_InitPuzzleAndGraphics
	end

@ Unclear where/if these temp vars are getting checked
LavaridgeTown_Gym_1F_EventScript_SetTrainerTempVars::
	setvar VAR_TEMP_B, 0
	setvar VAR_TEMP_C, 0
	setvar VAR_TEMP_D, 0
	setvar VAR_TEMP_E, 0
	setvar VAR_TEMP_F, 0
	goto_if_defeated TRAINER_COLE, LavaridgeTown_Gym_1F_EventScript_SetGeraldTempVar
	setvar VAR_TEMP_B, 1
LavaridgeTown_Gym_1F_EventScript_SetGeraldTempVar::
	goto_if_defeated TRAINER_GERALD, LavaridgeTown_Gym_1F_EventScript_SetAxleTempVar
	setvar VAR_TEMP_C, 1
LavaridgeTown_Gym_1F_EventScript_SetAxleTempVar::
	goto_if_defeated TRAINER_AXLE, LavaridgeTown_Gym_1F_EventScript_SetDanielleTempVar
	setvar VAR_TEMP_D, 1
LavaridgeTown_Gym_1F_EventScript_SetDanielleTempVar::
	goto_if_defeated TRAINER_DANIELLE, LavaridgeTown_Gym_1F_EventScript_EndSetTrainerTempVars
	setvar VAR_TEMP_E, 1
LavaridgeTown_Gym_1F_EventScript_EndSetTrainerTempVars::
	return

LavaridgeTown_Gym_1F_EventScript_CheckBuryTrainers::
	goto_if_defeated TRAINER_COLE, LavaridgeTown_Gym_1F_EventScript_CheckBuryGerald
	setobjectmovementtype LOCALID_COLE, MOVEMENT_TYPE_BURIED
LavaridgeTown_Gym_1F_EventScript_CheckBuryGerald::
	goto_if_defeated TRAINER_GERALD, LavaridgeTown_Gym_1F_EventScript_CheckBuryAxle
	setobjectmovementtype LOCALID_GERALD, MOVEMENT_TYPE_BURIED
LavaridgeTown_Gym_1F_EventScript_CheckBuryAxle::
	goto_if_defeated TRAINER_AXLE, LavaridgeTown_Gym_1F_EventScript_CheckBuryDanielle
	setobjectmovementtype LOCALID_AXLE, MOVEMENT_TYPE_BURIED
LavaridgeTown_Gym_1F_EventScript_CheckBuryDanielle::
	goto_if_defeated TRAINER_DANIELLE, LavaridgeTown_Gym_1F_EventScript_EndCheckBuryTrainers
	setobjectmovementtype LOCALID_DANIELLE, MOVEMENT_TYPE_BURIED
LavaridgeTown_Gym_1F_EventScript_EndCheckBuryTrainers::
	return

LavaridgeTown_Gym_1F_EventScript_Winona::
	trainerbattle_single TRAINER_WINONA_1, LavaridgeTown_Gym_1F_Text_WinonaIntro, LavaridgeTown_Gym_1F_Text_WinonaDefeat, LavaridgeTown_Gym_1F_EventScript_WinonaDefeated, NO_MUSIC
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, LavaridgeTown_Gym_1F_EventScript_WinonaRematch
	goto_if_unset FLAG_RECEIVED_TM40, LavaridgeTown_Gym_1F_EventScript_GiveAerialAce2
	msgbox LavaridgeTown_Gym_1F_Text_WinonaPostBattle, MSGBOX_DEFAULT
	release
	end

LavaridgeTown_Gym_1F_EventScript_WinonaDefeated::
	message LavaridgeTown_Gym_1F_Text_ReceivedFeatherBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LavaridgeTown_Gym_1F_Text_ExplainFeatherBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_WHITEOUT_TO_LAVARIDGE
	setflag FLAG_DEFEATED_LAVARIDGE_GYM
	setflag FLAG_BADGE04_GET
	addvar VAR_PETALBURG_GYM_STATE, 1
	call_if_eq VAR_PETALBURG_GYM_STATE, 6, Common_EventScript_ReadyPetalburgGymForBattle
	setvar VAR_0x8008, 4
	call Common_EventScript_SetGymTrainers
	setflag FLAG_HIDE_VERDANTURF_TOWN_WANDAS_HOUSE_WALLY
	setvar VAR_LAVARIDGE_TOWN_STATE, 1
	call LavaridgeTown_Gym_1F_EventScript_GiveAerialAce
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LavaridgeTown_Gym_1F_Text_RegisteredWinona, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_WINONA_MATCH_CALL
	special BadgeCount
	addrankpoints 5
	call_if_set FLAG_OPEN_WORLD, EventScript_OPLavaridge
	release
	end

EventScript_OPLavaridge::
	call_if_unset FLAG_BADGE05_GET, Common_EventScript_ReadyPetalburgGymForBattleOP
	return

LavaridgeTown_Gym_1F_EventScript_GiveAerialAce2::
	giveitem ITEM_HEAVY_DUTY_BOOTS
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	msgbox LavaridgeTown_Gym_1F_Text_ExplainAerialAce, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM40
	release
	end

LavaridgeTown_Gym_1F_EventScript_GiveAerialAce::
	giveitem ITEM_HEAVY_DUTY_BOOTS
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_BagIsFull
	msgbox LavaridgeTown_Gym_1F_Text_ExplainAerialAce, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM40
	return

LavaridgeTown_Gym_1F_EventScript_WinonaRematch::
	trainerbattle_rematch_double TRAINER_WINONA_1, LavaridgeTown_Gym_1F_Text_WinonaPreRematch, LavaridgeTown_Gym_1F_Text_WinonaRematchDefeat, LavaridgeTown_Gym_1F_Text_WinonaRematchNeedTwoMons
	msgbox LavaridgeTown_Gym_1F_Text_WinonaPostRematch, MSGBOX_AUTOCLOSE
	end

LavaridgeTown_Gym_1F_EventScript_Cole::
	trainerbattle TRAINER_BATTLE_CONTINUE_SCRIPT, TRAINER_COLE, LOCALID_COLE, LavaridgeTown_Gym_1F_Text_ColeIntro, LavaridgeTown_Gym_1F_Text_ColeDefeat, LavaridgeTown_Gym_EventScript_CheckTrainerScript
	msgbox LavaridgeTown_Gym_1F_Text_ColePostBattle, MSGBOX_AUTOCLOSE
	end

LavaridgeTown_Gym_EventScript_CheckTrainerScript::
	call LavaridgeTown_Gym_1F_EventScript_SetTrainerTempVars
	release
	special ShouldTryGetTrainerScript
	goto_if_eq VAR_RESULT, 1, EventScript_GotoTrainerScript
	end

LavaridgeTown_Gym_1F_EventScript_Axle::
	trainerbattle TRAINER_BATTLE_CONTINUE_SCRIPT, TRAINER_AXLE, LOCALID_AXLE, LavaridgeTown_Gym_1F_Text_AxleIntro, LavaridgeTown_Gym_1F_Text_AxleDefeat, LavaridgeTown_Gym_EventScript_CheckTrainerScript
	msgbox LavaridgeTown_Gym_1F_Text_AxlePostBattle, MSGBOX_AUTOCLOSE
	end

LavaridgeTown_Gym_B1F_EventScript_Keegan::
	trainerbattle TRAINER_BATTLE_CONTINUE_SCRIPT, TRAINER_KEEGAN, LOCALID_KEEGAN, LavaridgeTown_Gym_B1F_Text_KeeganIntro, LavaridgeTown_Gym_B1F_Text_KeeganDefeat, LavaridgeTown_Gym_EventScript_CheckTrainerScript
	msgbox LavaridgeTown_Gym_B1F_Text_KeeganPostBattle, MSGBOX_AUTOCLOSE
	end

LavaridgeTown_Gym_1F_EventScript_Danielle::
	trainerbattle TRAINER_BATTLE_CONTINUE_SCRIPT, TRAINER_DANIELLE, LOCALID_DANIELLE, LavaridgeTown_Gym_1F_Text_DanielleIntro, LavaridgeTown_Gym_1F_Text_DanielleDefeat, LavaridgeTown_Gym_EventScript_CheckTrainerScript
	msgbox LavaridgeTown_Gym_1F_Text_DaniellePostBattle, MSGBOX_AUTOCLOSE
	end

LavaridgeTown_Gym_1F_EventScript_Gerald::
	trainerbattle TRAINER_BATTLE_CONTINUE_SCRIPT, TRAINER_GERALD, LOCALID_GERALD, LavaridgeTown_Gym_1F_Text_GeraldIntro, LavaridgeTown_Gym_1F_Text_GeraldDefeat, LavaridgeTown_Gym_EventScript_CheckTrainerScript
	msgbox LavaridgeTown_Gym_1F_Text_GeraldPostBattle, MSGBOX_AUTOCLOSE
	end

LavaridgeTown_Gym_B1F_EventScript_Jace::
	trainerbattle TRAINER_BATTLE_CONTINUE_SCRIPT, TRAINER_JACE, LOCALID_JACE, LavaridgeTown_Gym_B1F_Text_JaceIntro, LavaridgeTown_Gym_B1F_Text_JaceDefeat, LavaridgeTown_Gym_EventScript_CheckTrainerScript
	msgbox LavaridgeTown_Gym_B1F_Text_JacePostBattle, MSGBOX_AUTOCLOSE
	end

LavaridgeTown_Gym_B1F_EventScript_Jeff::
	trainerbattle TRAINER_BATTLE_CONTINUE_SCRIPT, TRAINER_JEFF, LOCALID_JEFF, LavaridgeTown_Gym_B1F_Text_JeffIntro, LavaridgeTown_Gym_B1F_Text_JeffDefeat, LavaridgeTown_Gym_EventScript_CheckTrainerScript
	msgbox LavaridgeTown_Gym_B1F_Text_JeffPostBattle, MSGBOX_AUTOCLOSE
	end

LavaridgeTown_Gym_B1F_EventScript_Eli::
	trainerbattle TRAINER_BATTLE_CONTINUE_SCRIPT, TRAINER_ELI, LOCALID_ELI, LavaridgeTown_Gym_B1F_Text_EliIntro, LavaridgeTown_Gym_B1F_Text_EliDefeat, LavaridgeTown_Gym_EventScript_CheckTrainerScript
	msgbox LavaridgeTown_Gym_B1F_Text_EliPostBattle, MSGBOX_AUTOCLOSE
	end

LavaridgeTown_Gym_1F_EventScript_GymGuide::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_LAVARIDGE_GYM, LavaridgeTown_Gym_1F_EventScript_GymGuidePostVictory
	msgbox LavaridgeTown_Gym_1F_Text_GymGuideAdvice, MSGBOX_DEFAULT
	release
	end

LavaridgeTown_Gym_1F_EventScript_GymGuidePostVictory::
	msgbox LavaridgeTown_Gym_1F_Text_GymGuidePostVictory, MSGBOX_DEFAULT
	release
	end

LavaridgeTown_Gym_1F_EventScript_LeftGymStatue::
	lockall
	goto_if_set FLAG_BADGE04_GET, LavaridgeTown_Gym_1F_EventScript_GymStatueCertified
	goto LavaridgeTown_Gym_1F_EventScript_GymStatue
	end

LavaridgeTown_Gym_1F_EventScript_RightGymStatue::
	lockall
	goto_if_set FLAG_BADGE04_GET, LavaridgeTown_Gym_1F_EventScript_GymStatueCertified
	goto LavaridgeTown_Gym_1F_EventScript_GymStatue
	end

LavaridgeTown_Gym_1F_EventScript_GymStatueCertified::
	msgbox LavaridgeTown_Gym_1F_Text_GymStatueCertified, MSGBOX_DEFAULT
	releaseall
	end

LavaridgeTown_Gym_1F_EventScript_GymStatue::
	msgbox LavaridgeTown_Gym_1F_Text_GymStatue, MSGBOX_DEFAULT
	releaseall
	end

LavaridgeTown_Gym_1F_Text_GymGuideAdvice:
	.string "Hey, how's it going, CHAMPION-\n"
	.string "bound {PLAYER}?\p"
	.string "LAVARIDGE's GYM LEADER WINONA\n"
	.string "turned this place into an air maze.\p"
	.string "Those rotating doors will swing the whole\n"
	.string "battle path if you lose your bearings.\p"
	.string "Keep your footing, read the angles,\n"
	.string "and don't get blown off course.$"

LavaridgeTown_Gym_1F_Text_GymGuidePostVictory:
	.string "Yow! That was a high-flying battle!$"

LavaridgeTown_Gym_1F_Text_ColeIntro:
	.string "Owowowowow!\n"
	.string "Yikes, it's hot!$"

LavaridgeTown_Gym_1F_Text_ColeDefeat:
	.string "I'm blinded by sweat in my eyes…$"

LavaridgeTown_Gym_1F_Text_ColePostBattle:
	.string "Being buried in hot sand promotes\n"
	.string "circulation.\p"
	.string "It's effective for healing pain in\n"
	.string "your joints.$"

LavaridgeTown_Gym_1F_Text_AxleIntro:
	.string "I'm trying to relieve my stress.\n"
	.string "Don't come along and stress me out!$"

LavaridgeTown_Gym_1F_Text_AxleDefeat:
	.string "I hope WINONA clips your rhythm good!$"

LavaridgeTown_Gym_1F_Text_AxlePostBattle:
	.string "Haaah… Whew…\p"
	.string "If you lose track of the angles in\n"
	.string "here, WINONA will pick you apart.$"

LavaridgeTown_Gym_B1F_Text_KeeganIntro:
	.string "You must be getting tired by now.\n"
	.string "You'd like to rest in the hot sand,\l"
	.string "wouldn't you?\p"
	.string "But you should know that maintaining\n"
	.string "your willpower is an important ability\l"
	.string "for all TRAINERS.$"

LavaridgeTown_Gym_B1F_Text_KeeganDefeat:
	.string "I got caught in the crosswind…$"

LavaridgeTown_Gym_B1F_Text_KeeganPostBattle:
	.string "Your skill is real…\n"
	.string "But our LEADER WINONA is strong.\p"
	.string "If you don't watch your spacing, she'll\n"
	.string "pick your team apart.$"

LavaridgeTown_Gym_1F_Text_GeraldIntro:
	.string "Can your POKéMON handle a battle that\n"
	.string "never lets them touch the ground?$"

LavaridgeTown_Gym_1F_Text_GeraldDefeat:
	.string "I never got my footing…$"

LavaridgeTown_Gym_1F_Text_GeraldPostBattle:
	.string "WINONA turns every opening into air\n"
	.string "you can't quite reach.\p"
	.string "If you beat me, maybe you've got the\n"
	.string "timing to pin her down.$"

LavaridgeTown_Gym_1F_Text_DanielleIntro:
	.string "Um…\n"
	.string "Okay, I'll battle with you.$"

LavaridgeTown_Gym_1F_Text_DanielleDefeat:
	.string "Oh, but you're too strong.$"

LavaridgeTown_Gym_1F_Text_DaniellePostBattle:
	.string "I'm going to be a pretty and strong\n"
	.string "TRAINER just like WINONA.$"

LavaridgeTown_Gym_B1F_Text_JaceIntro:
	.string "Come on, get with it!\n"
	.string "Let's go before my feelings cool!$"

LavaridgeTown_Gym_B1F_Text_JaceDefeat:
	.string "I got frozen trying to keep up…$"

LavaridgeTown_Gym_B1F_Text_JacePostBattle:
	.string "The way the battling spirit burns\n"
	.string "within you, you may stand a chance\l"
	.string "against our LEADER.$"

LavaridgeTown_Gym_B1F_Text_JeffIntro:
	.string "See how the wind snaps through here?\n"
	.string "It knows I'm about to win!$"

LavaridgeTown_Gym_B1F_Text_JeffDefeat:
	.string "Something didn't go right.$"

LavaridgeTown_Gym_B1F_Text_JeffPostBattle:
	.string "Well, so what? I say so what?\n"
	.string "I can ride a gale without losing my hat!\p"
	.string "…Don't even think about trying it!$"

LavaridgeTown_Gym_B1F_Text_EliIntro:
	.string "As much as I love mountains,\n"
	.string "I especially love open skies.$"

LavaridgeTown_Gym_B1F_Text_EliDefeat:
	.string "Well, it seems to me I lost without\n"
	.string "ever being in control.$"

LavaridgeTown_Gym_B1F_Text_EliPostBattle:
	.string "I stay here because I became a fan\n"
	.string "of WINONA's power.\p"
	.string "Hehehehe.$"

LavaridgeTown_Gym_1F_Text_WinonaIntro:
	.string "WINONA: You made it through the maze.\n"
	.string "Nice.\p"
	.string "Now keep those Buttas planted and try\n"
	.string "not to get spun by the airflow.\p"
	.string "I don't just control the pace.\n"
	.string "I own the skies above it.$"

LavaridgeTown_Gym_1F_Text_WinonaDefeat:
	.string "WINONA: Clean timing.\p"
	.string "You never let the wind steal the pace.\n"
	.string "That earns a badge.$"

LavaridgeTown_Gym_1F_Text_ReceivedFeatherBadge:
	.string "{PLAYER} received the FEATHER BADGE\n"
	.string "from WINONA.$"

LavaridgeTown_Gym_1F_Text_ExplainFeatherBadgeTakeThis:
	.string "If you have a FEATHER BADGE, all POKéMON\n"
	.string "up to Level 50, even those you get in\l"
	.string "trades from other people, will obey\l"
	.string "you completely.\p"
	.string "And, it lets POKéMON use the HM move\n"
	.string "STRENGTH outside of battle.\p"
	.string "This is a token of my appreciation.\n"
	.string "Don't be shy about taking it!$"

LavaridgeTown_Gym_1F_Text_ExplainAerialAce:
	.string "Those HEAVY-DUTY BOOTS keep your\n"
	.string "switch-ins clean.\p"
	.string "Flying teams live by movement.\n"
	.string "Don't let hazards tax the trip.$"

LavaridgeTown_Gym_1F_Text_RegisteredWinona:
	.string "This number is blocked!$"

LavaridgeTown_Gym_1F_Text_WinonaPostBattle:
	.string "WINONA: Sharp work.\p"
	.string "You broke the air lanes open and never\n"
	.string "let me settle into the flow.$"

LavaridgeTown_Gym_1F_Text_GymStatue:
	.string "LAVARIDGE TOWN POKéMON GYM$"

LavaridgeTown_Gym_1F_Text_GymStatueCertified:
	.string "LAVARIDGE TOWN POKéMON GYM\p"
	.string "WINONA'S CERTIFIED TRAINERS:\n"
	.string "{PLAYER}$"

LavaridgeTown_Gym_1F_Text_WinonaPreRematch:
	.string "WINONA: Losing a battle isn't going\n"
	.string "to shake me.\p"
	.string "I love POKéMON.\n"
	.string "I love to battle.\l"
	.string "And I adore this airspace.\p"
	.string "Let's test each other in another\n"
	.string "battle of clean turns and open skies!$"

LavaridgeTown_Gym_1F_Text_WinonaRematchDefeat:
	.string "Whew!\n"
	.string "You grounded me again!$"

LavaridgeTown_Gym_1F_Text_WinonaPostRematch:
	.string "WINONA: I lost the match,\n"
	.string "but I'm completely satisfied.\p"
	.string "It's not often I get to enjoy a battle\n"
	.string "this sharp.\p"
	.string "Let's have another one like this\n"
	.string "again sometime!$"

LavaridgeTown_Gym_1F_Text_WinonaRematchNeedTwoMons:
	.string "WINONA: Losing a battle isn't going\n"
	.string "to shake me.\p"
	.string "I love POKéMON.\n"
	.string "I love to battle.\l"
	.string "And I adore this GYM!\p"
	.string "Let's exchange another clean aerial\n"
	.string "battle when you're ready!\p"
	.string "Oh, wait. Do you only have one POKéMON\n"
	.string "that can battle?\p"
	.string "I hate to say this, but when you come\n"
	.string "see me, bring at least two POKéMON.$"
