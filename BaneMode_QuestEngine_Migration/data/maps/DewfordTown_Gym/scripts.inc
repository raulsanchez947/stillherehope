.set LOCALID_FLANNERY, 1
.set LOCALID_COLE, 2
.set LOCALID_GERALD, 3
.set LOCALID_AXLE, 4
.set LOCALID_DANIELLE, 5
.set LOCALID_GYM_GUIDE, 6

DewfordTown_Gym_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, DewfordTown_Gym_OnTransition
	.byte 0

DewfordTown_Gym_OnTransition:
	call DewfordTown_Gym_EventScript_ResetObjects
	call LavaridgeTown_Gym_1F_EventScript_SetTrainerTempVars
	call LavaridgeTown_Gym_1F_EventScript_CheckBuryTrainers
	end

DewfordTown_Gym_EventScript_ResetObjects::
	setobjectxyperm LOCALID_FLANNERY, 13, 9
	setobjectmovementtype LOCALID_FLANNERY, MOVEMENT_TYPE_FACE_DOWN
	setobjectxyperm LOCALID_COLE, 3, 14
	setobjectxyperm LOCALID_GERALD, 2, 15
	setobjectxyperm LOCALID_AXLE, 3, 10
	setobjectxyperm LOCALID_DANIELLE, 5, 2
	setobjectmovementtype LOCALID_COLE, MOVEMENT_TYPE_BURIED
	setobjectmovementtype LOCALID_GERALD, MOVEMENT_TYPE_BURIED
	setobjectmovementtype LOCALID_AXLE, MOVEMENT_TYPE_BURIED
	setobjectmovementtype LOCALID_DANIELLE, MOVEMENT_TYPE_BURIED
	setobjectxyperm LOCALID_GYM_GUIDE, 12, 16
	setobjectmovementtype LOCALID_GYM_GUIDE, MOVEMENT_TYPE_FACE_RIGHT
	return

DewfordTown_Gym_EventScript_Flannery::
	trainerbattle_single TRAINER_FLANNERY_1, DewfordTown_Gym_Text_FlanneryIntro, DewfordTown_Gym_Text_FlanneryDefeat, DewfordTown_Gym_EventScript_FlanneryDefeated, NO_MUSIC
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, DewfordTown_Gym_EventScript_FlanneryRematch
	goto_if_unset FLAG_RECEIVED_TM50, DewfordTown_Gym_EventScript_GiveOverheat2
	msgbox DewfordTown_Gym_Text_FlanneryPostBattle, MSGBOX_DEFAULT
	release
	end

DewfordTown_Gym_EventScript_FlanneryDefeated::
	message DewfordTown_Gym_Text_ReceivedHeatBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox DewfordTown_Gym_Text_HeatBadgeInfoTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_DEWFORD_GYM
	setflag FLAG_BADGE02_GET
	addvar VAR_PETALBURG_GYM_STATE, 1
	call_if_eq VAR_PETALBURG_GYM_STATE, 6, Common_EventScript_ReadyPetalburgGymForBattle
	setvar VAR_0x8008, 2
	call Common_EventScript_SetGymTrainers
	call DewfordTown_Gym_EventScript_GiveOverheat
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox DewfordTown_Gym_Text_RegisteredFlannery, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_FLANNERY_MATCH_CALL
	setvar VAR_ROXANNE_CALL_STEP_COUNTER, 0
	setflag FLAG_ENABLE_ROXANNE_FIRST_CALL
	release
	end

DewfordTown_Gym_EventScript_GiveOverheat::
	giveitem ITEM_TM50
	goto_if_eq VAR_RESULT, 0, Common_EventScript_BagIsFull
	msgbox DewfordTown_Gym_Text_ExplainOverheat, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM50
	return

DewfordTown_Gym_EventScript_GiveOverheat2:
	giveitem ITEM_TM50
	goto_if_eq VAR_RESULT, 0, Common_EventScript_ShowBagIsFull
	msgbox DewfordTown_Gym_Text_ExplainOverheat, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM50
	release
	end

DewfordTown_Gym_EventScript_FlanneryRematch::
	trainerbattle_rematch_double TRAINER_FLANNERY_1, DewfordTown_Gym_Text_FlanneryPreRematch, DewfordTown_Gym_Text_FlanneryRematchDefeat, DewfordTown_Gym_Text_FlanneryRematchNeedTwoMons
	msgbox DewfordTown_Gym_Text_FlanneryPostRematch, MSGBOX_AUTOCLOSE
	end

DewfordTown_Gym_EventScript_GymGuide::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_DEWFORD_GYM, DewfordTown_Gym_EventScript_GymGuidePostVictory
	msgbox DewfordTown_Gym_Text_GymGuideAdvice, MSGBOX_DEFAULT
	release
	end

DewfordTown_Gym_EventScript_GymGuidePostVictory::
	msgbox DewfordTown_Gym_Text_GymGuidePostVictory, MSGBOX_DEFAULT
	release
	end

DewfordTown_Gym_EventScript_LeftGymStatue::
	lockall
	goto_if_set FLAG_BADGE02_GET, DewfordTown_Gym_EventScript_GymStatueCertified
	goto DewfordTown_Gym_EventScript_GymStatue
	end

DewfordTown_Gym_EventScript_RightGymStatue::
	lockall
	goto_if_set FLAG_BADGE02_GET, DewfordTown_Gym_EventScript_GymStatueCertified
	goto DewfordTown_Gym_EventScript_GymStatue
	end

DewfordTown_Gym_EventScript_GymStatueCertified::
	msgbox DewfordTown_Gym_Text_GymStatueCertified, MSGBOX_DEFAULT
	releaseall
	end

DewfordTown_Gym_EventScript_GymStatue::
	msgbox DewfordTown_Gym_Text_GymStatue, MSGBOX_DEFAULT
	releaseall
	end

DewfordTown_Gym_Text_GymGuideAdvice:
	.string "Yo, CHAMPION-bound {PLAYER}!\p"
	.string "This place belongs to FLANNERY now,\n"
	.string "and he turned it into a furnace.\p"
	.string "The buried trainers downstairs are here\n"
	.string "to wear you down before you reach him.\p"
	.string "Bring something that can handle the sun\n"
	.string "and don't let the heat rush you.$"

DewfordTown_Gym_Text_GymGuidePostVictory:
	.string "Hot stuff! You cooled FLANNERY off\n"
	.string "without losing your nerve.$"

DewfordTown_Gym_Text_FlanneryIntro:
	.string "FLANNERY: What's good, youngblood?\p"
	.string "I keep this room broiling on purpose.\n"
	.string "No shade, no mercy, no cheap exits.\p"
	.string "If your Buttas start bubbling,\n"
	.string "that means you came in undercooked.\p"
	.string "Stand tall or get smoked.$"

DewfordTown_Gym_Text_FlanneryDefeat:
	.string "FLANNERY: Hah!\n"
	.string "Okay, your squad can really stand heat.$"

DewfordTown_Gym_Text_ReceivedHeatBadge:
	.string "{PLAYER} received the HEAT BADGE\n"
	.string "from FLANNERY.$"

DewfordTown_Gym_Text_HeatBadgeInfoTakeThis:
	.string "The HEAT BADGE proves you kept your\n"
	.string "focus under pressure.\p"
	.string "Take this, too. It fits a battle where\n"
	.string "the temperature never lets up.$"

DewfordTown_Gym_Text_ExplainOverheat:
	.string "That TM contains OVERHEAT.\p"
	.string "It hits like a blast furnace, but you\n"
	.string "need the nerve to use it well.$"

DewfordTown_Gym_Text_RegisteredFlannery:
	.string "Registered FLANNERY in the POKéNAV.$"

DewfordTown_Gym_Text_FlanneryPostBattle:
	.string "FLANNERY: The fire in your team is real.\p"
	.string "Keep it under control and you'll keep\n"
	.string "making people fold.$"

DewfordTown_Gym_Text_FlanneryPreRematch:
	.string "Back again?\n"
	.string "Good. Let's turn the heat up.$"

DewfordTown_Gym_Text_FlanneryRematchDefeat:
	.string "You handled the pressure again!$"

DewfordTown_Gym_Text_FlanneryPostRematch:
	.string "You've got the kind of grit I respect.\p"
	.string "Come back any time you want another\n"
	.string "battle in the fire.$"

DewfordTown_Gym_Text_FlanneryRematchNeedTwoMons:
	.string "I want a full proper rematch.\p"
	.string "Bring at least two ready POKeMON.$"

DewfordTown_Gym_Text_GymStatue:
	.string "DEWFORD TOWN POKeMON GYM$"

DewfordTown_Gym_Text_GymStatueCertified:
	.string "DEWFORD TOWN POKeMON GYM\p"
	.string "FLANNERY'S CERTIFIED TRAINERS:\n"
	.string "{PLAYER}$"
