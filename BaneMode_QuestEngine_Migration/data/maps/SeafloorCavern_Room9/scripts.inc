.set LOCALID_G_ZAPDOS, 1
.set LOCALID_GIOVANNI, 2
.set LOCALID_WALLACE, 3
.set LOCALID_GRUNT_1, 4
.set LOCALID_GRUNT_2, 5
.set LOCALID_G_ZAPDOS_SLEEPING, 7

SeafloorCavern_Room9_MapScripts::
	.byte 0

SeafloorCavern_Room9_EventScript_ArchieAwakenKyogre::
	lockall
	setvar VAR_0x8004, LOCALID_GIOVANNI
	setvar VAR_0x8005, LOCALID_WALLACE
	setvar VAR_0x8006, LOCALID_GRUNT_1
	setvar VAR_0x8007, LOCALID_GRUNT_2
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, SeafloorCavern_Room9_Movement_Delay32
	waitmovement 0
	playbgm MUS_ENCOUNTER_AQUA, FALSE
	msgbox SeafloorCavern_Room9_Text_ArchieHoldItRightThere, MSGBOX_DEFAULT
	closemessage
	addobject VAR_0x8004
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	applymovement VAR_0x8004, SeafloorCavern_Room9_Movement_ArchieApproachPlayer
	waitmovement 0
	msgbox SeafloorCavern_Room9_Text_ArchieSoItWasYou, MSGBOX_DEFAULT
	applymovement VAR_0x8004, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox SeafloorCavern_Room9_Text_ArchieBeholdKyogre, MSGBOX_DEFAULT
	applymovement VAR_0x8004, Common_Movement_FacePlayer
	waitmovement 0
	msgbox SeafloorCavern_Room9_Text_ArchieYouMustDisappear, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_ARCHIE, SeafloorCavern_Room9_Text_ArchieDefeat
	msgbox SeafloorCavern_Room9_Text_ArchieWithThisRedOrb, MSGBOX_DEFAULT
	setweather WEATHER_NONE
	doweather
	special Script_FadeOutMapMusic
	waitstate
	msgbox SeafloorCavern_Room9_Text_RedOrbShinesByItself, MSGBOX_DEFAULT
	special WaitWeather
	waitstate
	setvar VAR_RESULT, 1
	playse SE_M_DETECT
	dofieldeffectsparkle 16, 42, 0
	waitfieldeffect FLDEFF_SPARKLE
	closemessage
	setvar VAR_RESULT, 0
	playfanfare MUS_AWAKEN_LEGEND
	playse SE_ORB
	special DoOrbEffect
	applymovement VAR_0x8004, Common_Movement_WalkInPlaceFasterUp
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	delay 150
	removeobject LOCALID_G_ZAPDOS_SLEEPING
	addobject LOCALID_G_ZAPDOS
	waitstate
	delay 60
	applymovement LOCALID_G_ZAPDOS, SeafloorCavern_Room9_Movement_G_ZapdosApproach
	waitmovement 0
	special FadeOutOrbEffect
	waitstate
	setvar VAR_0x8004, 1  @ vertical pan
	setvar VAR_0x8005, 1  @ horizontal pan
	setvar VAR_0x8006, 8  @ num shakes
	setvar VAR_0x8007, 5  @ shake delay
	special ShakeCamera
	waitstate
	applymovement LOCALID_G_ZAPDOS, SeafloorCavern_Room9_Movement_G_ZapdosExit
	waitmovement 0
	removeobject LOCALID_G_ZAPDOS
	delay 4
	setvar VAR_0x8004, 2  @ vertical pan
	setvar VAR_0x8005, 2  @ horizontal pan
	setvar VAR_0x8006, 8  @ num shakes
	setvar VAR_0x8007, 5  @ shake delay
	special ShakeCamera
	waitstate
	delay 30
	setvar VAR_0x8004, LOCALID_GIOVANNI
	setvar VAR_0x8005, LOCALID_WALLACE
	setvar VAR_0x8006, LOCALID_GRUNT_1
	setvar VAR_0x8007, LOCALID_GRUNT_2
	msgbox SeafloorCavern_Room9_Text_ArchieWhereDidKyogreGo, MSGBOX_DEFAULT
	playse SE_PC_LOGIN
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	msgbox SeafloorCavern_Room9_Text_ArchieAMessageFromOutside, MSGBOX_DEFAULT
	closemessage
	applymovement VAR_0x8004, SeafloorCavern_Room9_Movement_ArchieListenMessage
	waitmovement 0
	msgbox SeafloorCavern_Room9_Text_ArchieWhatRainingTooHard, MSGBOX_DEFAULT
	closemessage
	playse SE_PC_OFF
	delay 20
	applymovement VAR_0x8004, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	msgbox SeafloorCavern_Room9_Text_ArchieWhyDidKyogreDisappear, MSGBOX_DEFAULT
	closemessage
	addobject VAR_0x8005
	addobject VAR_0x8006
	addobject VAR_0x8007
	applymovement VAR_0x8007, SeafloorCavern_Room9_Movement_MagmaGruntArrive
	applymovement VAR_0x8006, SeafloorCavern_Room9_Movement_MagmaGruntArrive
	applymovement VAR_0x8005, SeafloorCavern_Room9_Movement_MaxieArrive
	waitmovement 0
	applymovement VAR_0x8004, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	msgbox SeafloorCavern_Room9_Text_MaxieWhatHaveYouWrought, MSGBOX_DEFAULT
	playse SE_PIN
	applymovement VAR_0x8004, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement VAR_0x8004, Common_Movement_Delay48
	waitmovement 0
	msgbox SeafloorCavern_Room9_Text_ArchieDontGetAllHighAndMighty, MSGBOX_DEFAULT
	msgbox SeafloorCavern_Room9_Text_MaxieWeDontHaveTimeToArgue, MSGBOX_DEFAULT
	closemessage
	applymovement VAR_0x8005, SeafloorCavern_Room9_Movement_MaxieExit
	applymovement VAR_0x8004, SeafloorCavern_Room9_Movement_ArchieExit
	waitmovement 0
	msgbox SeafloorCavern_Room9_Text_MaxieComeOnPlayer, MSGBOX_DEFAULT
	msgbox SeafloorCavern_Room9_Text_WallaceGivesThunderOrb, MSGBOX_DEFAULT
	giveitem ITEM_BLUE_ORB
	setvar VAR_ROUTE128_STATE, 1
	setvar VAR_SOOTOPOLIS_CITY_STATE, 1
	clearflag FLAG_HIDE_SOOTOPOLIS_CITY_STEVEN
	clearflag FLAG_HIDE_SOOTOPOLIS_CITY_ARCHIE
	clearflag FLAG_HIDE_SOOTOPOLIS_CITY_MAXIE
	clearflag FLAG_HIDE_SOOTOPOLIS_CITY_RESIDENTS
	clearflag FLAG_HIDE_SOOTOPOLIS_CITY_GROUDON
	clearflag FLAG_HIDE_SOOTOPOLIS_CITY_KYOGRE
	setflag FLAG_HIDE_SOOTOPOLIS_CITY_MAN_1
	setflag FLAG_LEGENDARIES_IN_SOOTOPOLIS
	clearflag FLAG_HIDE_ROUTE_128_ARCHIE
	clearflag FLAG_HIDE_ROUTE_128_MAXIE
	setflag FLAG_SYS_WEATHER_CTRL
	setflag FLAG_KYOGRE_ESCAPED_SEAFLOOR_CAVERN
	setflag FLAG_HIDE_MOSSDEEP_CITY_STEVENS_HOUSE_STEVEN
	setvar VAR_STEVENS_HOUSE_STATE, 2
	setvar VAR_SEAFLOOR_CAVERN_STATE, 1
	setflag FLAG_HIDE_SEAFLOOR_CAVERN_ROOM_9_ARCHIE
	setflag FLAG_HIDE_SEAFLOOR_CAVERN_ROOM_9_MAXIE
	setflag FLAG_HIDE_SEAFLOOR_CAVERN_ROOM_9_MAGMA_GRUNTS
	setflag FLAG_HIDE_SEAFLOOR_CAVERN_ROOM_9_KYOGRE
	setflag FLAG_HIDE_SEAFLOOR_CAVERN_AQUA_GRUNTS
	setflag FLAG_HIDE_MAP_NAME_POPUP
	warp MAP_ROUTE128, 38, 22
	waitstate
	releaseall
	end

SeafloorCavern_Room9_Movement_ArchieApproachPlayer:
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

SeafloorCavern_Room9_Movement_Unused1:
	walk_left
	walk_left
	step_end

SeafloorCavern_Room9_Movement_ArchieListenMessage:
	walk_left
	delay_16
	step_end

SeafloorCavern_Room9_Movement_Unused2:
	walk_right
	step_end

SeafloorCavern_Room9_Movement_ArchieExit:
	face_up
	lock_facing_direction
	walk_down
	unlock_facing_direction
	walk_in_place_faster_right
	step_end

SeafloorCavern_Room9_Movement_G_ZapdosApproach:
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

SeafloorCavern_Room9_Movement_G_ZapdosExit:
	slide_up
	slide_up
	step_end

SeafloorCavern_Room9_Movement_MaxieArrive:
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	step_end

SeafloorCavern_Room9_Movement_MaxieExit:
	walk_right
	walk_right
	step_end

SeafloorCavern_Room9_Movement_MagmaGruntArrive:
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	step_end

SeafloorCavern_Room9_Movement_Delay32:
	delay_16
	delay_16
	step_end

SeafloorCavern_Room9_Text_ArchieHoldItRightThere:
	.string "GIOVANNI: Stop.$"

SeafloorCavern_Room9_Text_ArchieSoItWasYou:
	.string "GIOVANNI: …\n"
	.string "You followed me all the way here.$"

SeafloorCavern_Room9_Text_ArchieBeholdKyogre:
	.string "GIOVANNI: Look at it.\p"
	.string "ZAPDOS - the ancient bird of lightning.\n"
	.string "Sleeping.\l"
	.string "Helpless.\p"
	.string "A weapon waiting to be claimed.\p"
	.string "And TEAM ROCKET will claim it.$"

SeafloorCavern_Room9_Text_ArchieYouMustDisappear:
	.string "GIOVANNI: You've done well to chase\n"
	.string "me this far.\p"
	.string "But it ends here.\p"
	.string "I cannot afford any witnesses to\n"
	.string "what happens next.$"

SeafloorCavern_Room9_Text_ArchieDefeat:
	.string "What?!\n"
	.string "I lost to a child?! Impossible!$"

SeafloorCavern_Room9_Text_ArchieWithThisRedOrb:
	.string "GIOVANNI: …Impressive.\n"
	.string "You are genuinely talented.\p"
	.string "But it changes nothing.\p"
	.string "I still have the THUNDER ORB.\n"
	.string "And with it, ZAPDOS answers to me…$"

SeafloorCavern_Room9_Text_RedOrbShinesByItself:
	.string "The THUNDER ORB suddenly blazed with\n"
	.string "electric light - all on its own!$"

SeafloorCavern_Room9_Text_ArchieWhereDidKyogreGo:
	.string "GIOVANNI: What?!\p"
	.string "I did nothing wrong.\n"
	.string "Why is the THUNDER ORB…\p"
	.string "Where- Where did ZAPDOS go?!$"

SeafloorCavern_Room9_Text_ArchieAMessageFromOutside:
	.string "GIOVANNI: …A report from the\n"
	.string "operatives on the surface.$"

SeafloorCavern_Room9_Text_ArchieWhatRainingTooHard:
	.string "GIOVANNI: Report.\p"
	.string "Hm.\n"
	.string "Storm conditions on the coast?\p"
	.string "Good. That was expected.\n"
	.string "ZAPDOS's power was meant to cause\l"
	.string "disruption - to destabilize HOENN.\p"
	.string "What?!\p"
	.string "The lightning is beyond projections?\n"
	.string "Operatives are in danger?!\p"
	.string "That is not…\n"
	.string "That should not be possible.\p"
	.string "Maintain your positions.\n"
	.string "Continue monitoring!$"

SeafloorCavern_Room9_Text_ArchieWhyDidKyogreDisappear:
	.string "GIOVANNI: Something is wrong.\p"
	.string "The THUNDER ORB was meant to bind\n"
	.string "ZAPDOS to my will.\p"
	.string "But ZAPDOS is… gone.\n"
	.string "Out of control.\p"
	.string "WHY?!$"

SeafloorCavern_Room9_Text_MaxieWhatHaveYouWrought:
	.string "WALLACE: What have you done?!\p"
	.string "GIOVANNI… You've unleashed ZAPDOS.\p"
	.string "If this lightning storm isn't stopped,\n"
	.string "HOENN's grid will be shattered!$"

SeafloorCavern_Room9_Text_ArchieDontGetAllHighAndMighty:
	.string "GIOVANNI: Don't lecture me!\p"
	.string "The HOENN DEFENDERS drove ARTICUNO\n"
	.string "into chaos first!\p"
	.string "The THUNDER ORB should give control-\n"
	.string "I should be able to contain this…\p"
	.string "I just need more time…$"

SeafloorCavern_Room9_Text_MaxieWeDontHaveTimeToArgue:
	.string "WALLACE: There is no more time!\p"
	.string "Get to the surface.\n"
	.string "See what TEAM ROCKET has done!\p"
	.string "ZAPDOS and ARTICUNO are both\n"
	.string "loose. Is this what you wanted?!$"

SeafloorCavern_Room9_Text_MaxieComeOnPlayer:
	.string "WALLACE: {PLAYER}!\n"
	.string "Get out of here - now!$"

SeafloorCavern_Room9_Text_WallaceGivesThunderOrb:
	.string "WALLACE: Wait - GIOVANNI dropped\n"
	.string "the THUNDER ORB when he fled.\p"
	.string "Take it.\n"
	.string "Something tells me you'll need it.$"
