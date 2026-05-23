.set LOCALID_G_ARTICUNO, 1
.set LOCALID_GRUNT_1, 2
.set LOCALID_GRUNT_2, 3
.set LOCALID_GRUNT_3, 4
.set LOCALID_ROCKET_ADMIN, 5
.set LOCALID_WALLACE, 6
.set LOCALID_G_ARTICUNO_SLEEPING, 7

MagmaHideout_4F_MapScripts::
	.byte 0

MagmaHideout_4F_EventScript_Maxie::
	lockall
	goto_if_set FLAG_OPEN_WORLD, EventScript_MagmaHideout_4F_EventScript_MaxieOP
	playbgm MUS_ENCOUNTER_MAGMA, FALSE
	msgbox MagmaHideout_4F_Text_MaxieAwakenGroudon, MSGBOX_DEFAULT
	closemessage
	delay 20
	setvar VAR_RESULT, 1
	playse SE_M_DETECT
	dofieldeffectsparkle 18, 42, 0
	waitfieldeffect FLDEFF_SPARKLE
	setvar VAR_RESULT, 1
	playfanfare MUS_AWAKEN_LEGEND
	playse SE_ORB
	special DoOrbEffect
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	delay 150
	removeobject LOCALID_G_ARTICUNO_SLEEPING
	addobject LOCALID_G_ARTICUNO
	waitstate
	delay 60
	applymovement LOCALID_G_ARTICUNO, MagmaHideout_4F_Movement_G_ArticunoApproach
	waitmovement 0
	special FadeOutOrbEffect
	waitstate
	setvar VAR_0x8004, 1  @ vertical pan
	setvar VAR_0x8005, 1  @ horizontal pan
	setvar VAR_0x8006, 8  @ num shakes
	setvar VAR_0x8007, 5  @ shake delay
	special ShakeCamera
	waitstate
	applymovement LOCALID_G_ARTICUNO, MagmaHideout_4F_Movement_G_ArticunoExit
	waitmovement 0
	removeobject LOCALID_G_ARTICUNO
	delay 4
	setvar VAR_0x8004, 2  @ vertical pan
	setvar VAR_0x8005, 2  @ horizontal pan
	setvar VAR_0x8006, 8  @ num shakes
	setvar VAR_0x8007, 5  @ shake delay
	special ShakeCamera
	waitstate
	delay 30
	applymovement LOCALID_WALLACE, MagmaHideout_4F_Movement_MaxieLookAround
	waitmovement 0
	msgbox MagmaHideout_4F_Text_MaxieGroudonWhatsWrong, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	delay 30
	applymovement LOCALID_WALLACE, Common_Movement_FacePlayer
	waitmovement 0
	msgbox MagmaHideout_4F_Text_MaxieOhItWasYou, MSGBOX_DEFAULT
	closemessage
	msgbox MagmaHideout_4F_Text_MaxieImGoingAfterGroudon, MSGBOX_DEFAULT
	closemessage
	clearflag FLAG_HIDE_SLATEPORT_CITY_CAPTAIN_STERN
	clearflag FLAG_HIDE_SLATEPORT_CITY_GABBY_AND_TY
	setvar VAR_SLATEPORT_CITY_STATE, 1
	setflag FLAG_GROUDON_AWAKENED_MAGMA_HIDEOUT
	setvar VAR_SLATEPORT_HARBOR_STATE, 1
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_WALLACE
	removeobject LOCALID_GRUNT_1
	removeobject LOCALID_GRUNT_2
	removeobject LOCALID_GRUNT_3
	removeobject LOCALID_ROCKET_ADMIN
	setflag FLAG_HIDE_MAGMA_HIDEOUT_GRUNTS
	fadescreen FADE_FROM_BLACK
	releaseall
	end

EventScript_MagmaHideout_4F_EventScript_MaxieOP::
	msgbox MagmaHideout_4F_Text_MaxieOhItWasYou, MSGBOX_DEFAULT
	closemessage
	msgbox MagmaHideout_4F_Text_MaxieImGoingAfterGroudon, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_WALLACE
	removeobject LOCALID_GRUNT_1
	removeobject LOCALID_GRUNT_2
	removeobject LOCALID_GRUNT_3
	removeobject LOCALID_ROCKET_ADMIN
	setflag FLAG_HIDE_MAGMA_HIDEOUT_GRUNTS
	fadescreen FADE_FROM_BLACK
	releaseall
	end

MagmaHideout_4F_Movement_G_ArticunoApproach:
	delay_16
	delay_16
	walk_slow_down
	delay_16
	delay_16
	delay_16
	walk_slow_down
	delay_16
	delay_16
	delay_16
	step_end

MagmaHideout_4F_Movement_G_ArticunoExit:
	slide_up
	slide_up
	step_end

MagmaHideout_4F_Movement_MaxieLookAround:
	face_left
	delay_16
	face_right
	delay_16
	face_left
	delay_16
	face_right
	delay_16
	face_up
	delay_16
	delay_16
	step_end

MagmaHideout_4F_EventScript_Grunt11::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_11, MagmaHideout_4F_Text_Grunt11Intro, MagmaHideout_4F_Text_Grunt11Defeat
	msgbox MagmaHideout_4F_Text_Grunt11PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_4F_EventScript_Grunt12::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_12, MagmaHideout_4F_Text_Grunt12Intro, MagmaHideout_4F_Text_Grunt12Defeat
	msgbox MagmaHideout_4F_Text_Grunt12PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_4F_EventScript_Grunt13::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_13, MagmaHideout_4F_Text_Grunt13Intro, MagmaHideout_4F_Text_Grunt13Defeat
	msgbox MagmaHideout_4F_Text_Grunt13PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_4F_EventScript_Tabitha::
	trainerbattle_single TRAINER_TABITHA_MAGMA_HIDEOUT, MagmaHideout_4F_Text_TabithaIntro, MagmaHideout_4F_Text_TabithaDefeat
	msgbox MagmaHideout_4F_Text_TabithaPostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_4F_Text_Grunt11Intro:
	.string "TEAM ROCKET seized this base!\n"
	.string "You don't belong here!\p"
	.string "The BOSS is taking ARTICUNO,\n"
	.string "and nothing's going to stop us!$"

MagmaHideout_4F_Text_Grunt11Defeat:
	.string "How… did I get beaten so fast?!$"

MagmaHideout_4F_Text_Grunt11PostBattle:
	.string "TEAM ROCKET never runs from\n"
	.string "a fight… but maybe today…$"

MagmaHideout_4F_Text_Grunt12Intro:
	.string "Fuhaha!\n"
	.string "ARTICUNO is almost ours!\l"
	.string "TEAM ROCKET's plan is unstoppable!$"

MagmaHideout_4F_Text_Grunt12Defeat:
	.string "Grrr…\n"
	.string "I've never seen a trainer like you!$"

MagmaHideout_4F_Text_Grunt12PostBattle:
	.string "BOSS! An intruder is past me!\n"
	.string "Heading your way!$"

MagmaHideout_4F_Text_Grunt13Intro:
	.string "You're not done yet!\n"
	.string "I'm not letting you through!$"

MagmaHideout_4F_Text_Grunt13Defeat:
	.string "Wh-was I really that weak?!$"

MagmaHideout_4F_Text_Grunt13PostBattle:
	.string "Just you wait…\n"
	.string "The BOSS won't lose like I did…$"

MagmaHideout_4F_Text_TabithaIntro:
	.string "Heh!\n"
	.string "You've gotten this deep, huh?\p"
	.string "Up ahead - ARTICUNO's chamber.\p"
	.string "The BOSS went there to claim it\n"
	.string "just moments ago.\p"
	.string "By now it's too late to stop him!\n"
	.string "ARTICUNO belongs to TEAM ROCKET!$"

MagmaHideout_4F_Text_TabithaDefeat:
	.string "Tch…\n"
	.string "You're something else…$"

MagmaHideout_4F_Text_TabithaPostBattle:
	.string "…While you wasted time on me,\n"
	.string "the BOSS had his way with ARTICUNO…$"

MagmaHideout_4F_Text_MaxieAwakenGroudon:
	.string "WALLACE: ARTICUNO…\p"
	.string "Resting here in the heart of\n"
	.string "the magma…\p"
	.string "TEAM ROCKET breached this chamber.\n"
	.string "They tried to seize the ORB.\p"
	.string "I used it to try to calm you,\n"
	.string "to keep you from their reach-\p"
	.string "But now… ARTICUNO!\n"
	.string "Please, hear me!$"

MagmaHideout_4F_Text_MaxieGroudonWhatsWrong:
	.string "WALLACE: ARTICUNO!\p"
	.string "It's frightened… or enraged.\p"
	.string "TEAM ROCKET's interference\n"
	.string "destabilised the connection…\p"
	.string "ARTICUNO!\n"
	.string "Wait- where are you going?!$"

MagmaHideout_4F_Text_MaxieOhItWasYou:
	.string "WALLACE: {PLAYER}!\n"
	.string "Thank goodness you're here.\p"
	.string "TEAM ROCKET stormed this base.\n"
	.string "We fought them off, but the chaos\l"
	.string "disturbed ARTICUNO's rest.\p"
	.string "It fled before I could calm it.\p"
	.string "This is bad.\n"
	.string "A distressed ARTICUNO is dangerous.$"

MagmaHideout_4F_Text_MaxieDefeat:
	.string "WALLACE: …$"

MagmaHideout_4F_Text_MaxieImGoingAfterGroudon:
	.string "WALLACE: I have to go after ARTICUNO\n"
	.string "before it reaches populated areas.\p"
	.string "The HOENN DEFENDERS will handle\n"
	.string "what's left of TEAM ROCKET here.\p"
	.string "But {PLAYER}-\p"
	.string "TEAM ROCKET is escalating.\n"
	.string "They've made their move on HOENN.\p"
	.string "Stay alert.\n"
	.string "We'll need every ally we have.$"
