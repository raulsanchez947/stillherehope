.set LOCALID_SCOTT, 1

BattleFrontier_ScottsHouse_MapScripts::
	.byte 0

BattleFrontier_ScottsHouse_EventScript_Scott::
	lock
	faceplayer
	goto_if_set FLAG_TEMP_4, BattleFrontier_ScottsHouse_EventScript_GivenBerry
	goto_if_set FLAG_TEMP_3, BattleFrontier_ScottsHouse_EventScript_GivenShield
	goto_if_set FLAG_TEMP_2, BattleFrontier_ScottsHouse_EventScript_GivenBattlePoints
	goto BattleFrontier_ScottsHouse_EventScript_CheckGiveItems
	end

BattleFrontier_ScottsHouse_EventScript_CheckGiveItems::
	goto_if_unset FLAG_SCOTT_GIVES_BATTLE_POINTS, BattleFrontier_ScottsHouse_EventScript_WelcomeToFrontier
	goto_if_unset FLAG_COLLECTED_ALL_SILVER_SYMBOLS, BattleFrontier_ScottsHouse_EventScript_CheckSilverSymbols
	goto_if_unset FLAG_COLLECTED_ALL_GOLD_SYMBOLS, BattleFrontier_ScottsHouse_EventScript_CheckGoldSymbols
	goto BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	end

BattleFrontier_ScottsHouse_EventScript_CheckGiveShield::
	goto_if_unset FLAG_RECEIVED_SILVER_SHIELD, BattleFrontier_ScottsHouse_EventScript_CheckGiveSilverShield
	goto_if_unset FLAG_RECEIVED_GOLD_SHIELD, BattleFrontier_ScottsHouse_EventScript_CheckGiveGoldShield
	goto BattleFrontier_ScottsHouse_EventScript_RandomComment
	end

BattleFrontier_ScottsHouse_EventScript_CheckSilverSymbols::
	goto_if_unset FLAG_SYS_TOWER_SILVER, BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	goto_if_unset FLAG_SYS_DOME_SILVER, BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	goto_if_unset FLAG_SYS_PALACE_SILVER, BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	goto_if_unset FLAG_SYS_ARENA_SILVER, BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	goto_if_unset FLAG_SYS_FACTORY_SILVER, BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	goto_if_unset FLAG_SYS_PIKE_SILVER, BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	goto_if_unset FLAG_SYS_PYRAMID_SILVER, BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	msgbox BattleFrontier_ScottsHouse_Text_YouveCollectedAllSilverSymbols, MSGBOX_DEFAULT
	giveitem ITEM_LANSAT_BERRY
	goto_if_eq VAR_RESULT, FALSE, BattleFrontier_ScottsHouse_EventScript_BerryPocketFull
	setflag FLAG_COLLECTED_ALL_SILVER_SYMBOLS
	setflag FLAG_TEMP_4
	release
	end

BattleFrontier_ScottsHouse_EventScript_CheckGoldSymbols::
	goto_if_unset FLAG_SYS_TOWER_GOLD, BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	goto_if_unset FLAG_SYS_DOME_GOLD, BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	goto_if_unset FLAG_SYS_PALACE_GOLD, BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	goto_if_unset FLAG_SYS_ARENA_GOLD, BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	goto_if_unset FLAG_SYS_FACTORY_GOLD, BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	goto_if_unset FLAG_SYS_PIKE_GOLD, BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	goto_if_unset FLAG_SYS_PYRAMID_GOLD, BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	msgbox BattleFrontier_ScottsHouse_Text_YouveCollectedAllGoldSymbols, MSGBOX_DEFAULT
	giveitem ITEM_STARF_BERRY
	goto_if_eq VAR_RESULT, FALSE, BattleFrontier_ScottsHouse_EventScript_BerryPocketFull
	setflag FLAG_COLLECTED_ALL_GOLD_SYMBOLS
	setflag FLAG_TEMP_4
	release
	end

BattleFrontier_ScottsHouse_EventScript_BerryPocketFull::
	msgbox BattleFrontier_ScottsHouse_Text_BerryPocketStuffed, MSGBOX_DEFAULT
	release
	end

BattleFrontier_ScottsHouse_EventScript_GivenBerry::
	msgbox BattleFrontier_ScottsHouse_Text_SoGladIBroughtYouHere, MSGBOX_DEFAULT
	release
	end

BattleFrontier_ScottsHouse_EventScript_RandomComment::
	random 3
	goto_if_eq VAR_RESULT, 1, BattleFrontier_ScottsHouse_EventScript_FrontierBrainComment
	goto_if_eq VAR_RESULT, 2, BattleFrontier_ScottsHouse_EventScript_ArtisanCaveComment
	msgbox BattleFrontier_ScottsHouse_Text_WhyIGoSeekingTrainers, MSGBOX_DEFAULT
	release
	end

BattleFrontier_ScottsHouse_EventScript_FrontierBrainComment::
	msgbox BattleFrontier_ScottsHouse_Text_HaveYouMetFrontierBrain, MSGBOX_DEFAULT
	release
	end

BattleFrontier_ScottsHouse_EventScript_ArtisanCaveComment::
	msgbox BattleFrontier_ScottsHouse_Text_MayFindWildMonsInFrontier, MSGBOX_DEFAULT
	release
	end

BattleFrontier_ScottsHouse_EventScript_CheckGiveSilverShield::
	setvar VAR_FRONTIER_BATTLE_MODE, FRONTIER_MODE_SINGLES
	frontier_set FRONTIER_DATA_LVL_MODE, FRONTIER_LVL_50
	tower_get TOWER_DATA_WIN_STREAK
	goto_if_ge VAR_RESULT, 50, BattleFrontier_ScottsHouse_EventScript_GiveSilverShield
	frontier_set FRONTIER_DATA_LVL_MODE, FRONTIER_LVL_OPEN
	tower_get TOWER_DATA_WIN_STREAK
	goto_if_ge VAR_RESULT, 50, BattleFrontier_ScottsHouse_EventScript_GiveSilverShield
	goto BattleFrontier_ScottsHouse_EventScript_RandomComment
	end

BattleFrontier_ScottsHouse_EventScript_GiveSilverShield::
	msgbox BattleFrontier_ScottsHouse_Text_Beat50TrainersInARow, MSGBOX_DEFAULT
	givedecoration DECOR_SILVER_SHIELD
	goto_if_eq VAR_RESULT, FALSE, BattleFrontier_ScottsHouse_EventScript_NoRoomForShield
	setflag FLAG_RECEIVED_SILVER_SHIELD
	setflag FLAG_TEMP_3
	goto BattleFrontier_ScottsHouse_EventScript_GivenShield
	end

BattleFrontier_ScottsHouse_EventScript_NoRoomForShield::
	msgbox BattleFrontier_ScottsHouse_Text_ComeBackForThisLater, MSGBOX_DEFAULT
	release
	end

BattleFrontier_ScottsHouse_EventScript_GivenShield::
	msgbox BattleFrontier_ScottsHouse_Text_ExpectingToHearEvenGreaterThings, MSGBOX_DEFAULT
	release
	end

BattleFrontier_ScottsHouse_EventScript_CheckGiveGoldShield::
	setvar VAR_FRONTIER_BATTLE_MODE, FRONTIER_MODE_SINGLES
	frontier_set FRONTIER_DATA_LVL_MODE, FRONTIER_LVL_50
	tower_get TOWER_DATA_WIN_STREAK
	goto_if_ge VAR_RESULT, 100, BattleFrontier_ScottsHouse_EventScript_GiveGoldShield
	frontier_set FRONTIER_DATA_LVL_MODE, FRONTIER_LVL_OPEN
	tower_get TOWER_DATA_WIN_STREAK
	goto_if_ge VAR_RESULT, 100, BattleFrontier_ScottsHouse_EventScript_GiveGoldShield
	goto BattleFrontier_ScottsHouse_EventScript_RandomComment
	end

BattleFrontier_ScottsHouse_EventScript_GiveGoldShield::
	msgbox BattleFrontier_ScottsHouse_Text_Beat100TrainersInARow, MSGBOX_DEFAULT
	givedecoration DECOR_GOLD_SHIELD
	goto_if_eq VAR_RESULT, FALSE, BattleFrontier_ScottsHouse_EventScript_NoRoomForShield
	setflag FLAG_RECEIVED_GOLD_SHIELD
	setflag FLAG_TEMP_3
	goto BattleFrontier_ScottsHouse_EventScript_GivenShield
	end

BattleFrontier_ScottsHouse_EventScript_GivenBattlePoints::
	msgbox BattleFrontier_ScottsHouse_Text_ExpectingGreatThings, MSGBOX_DEFAULT
	release
	end

BattleFrontier_ScottsHouse_EventScript_WelcomeToFrontier::
	msgbox BattleFrontier_ScottsHouse_Text_WelcomeToBattleFrontier, MSGBOX_DEFAULT
	closemessage
	delay 30
	call_if_eq VAR_FACING, DIR_NORTH, BattleFrontier_ScottsHouse_EventScript_ScottFaceAwayNorth
	call_if_eq VAR_FACING, DIR_SOUTH, BattleFrontier_ScottsHouse_EventScript_ScottFaceAwaySouth
	call_if_eq VAR_FACING, DIR_EAST, BattleFrontier_ScottsHouse_EventScript_ScottFaceAwayEast
	call_if_eq VAR_FACING, DIR_WEST, BattleFrontier_ScottsHouse_EventScript_ScottFaceAwayWest
	msgbox BattleFrontier_ScottsHouse_Text_HowMuchEffortItTookToMakeReal, MSGBOX_DEFAULT
	applymovement LOCALID_SCOTT, Common_Movement_FacePlayer
	waitmovement 0
	msgbox BattleFrontier_ScottsHouse_Text_HaveThisAsMementoOfOurPathsCrossing, MSGBOX_DEFAULT
	goto_if_eq VAR_SCOTT_STATE, 13, BattleFrontier_ScottsHouse_EventScript_Give4BattlePoints
	goto_if_ge VAR_SCOTT_STATE, 9, BattleFrontier_ScottsHouse_EventScript_Give3BattlePoints
	goto_if_ge VAR_SCOTT_STATE, 6, BattleFrontier_ScottsHouse_EventScript_Give2BattlePoints
	goto BattleFrontier_ScottsHouse_EventScript_Give1BattlePoint
	end

BattleFrontier_ScottsHouse_EventScript_Give4BattlePoints::
	buffernumberstring STR_VAR_1, 4
	setvar VAR_0x8004, 4
	goto BattleFrontier_ScottsHouse_EventScript_GiveBattlePoints
	end

BattleFrontier_ScottsHouse_EventScript_Give3BattlePoints::
	buffernumberstring STR_VAR_1, 3
	setvar VAR_0x8004, 3
	goto BattleFrontier_ScottsHouse_EventScript_GiveBattlePoints
	end

BattleFrontier_ScottsHouse_EventScript_Give2BattlePoints::
	buffernumberstring STR_VAR_1, 2
	setvar VAR_0x8004, 2
	goto BattleFrontier_ScottsHouse_EventScript_GiveBattlePoints
	end

BattleFrontier_ScottsHouse_EventScript_Give1BattlePoint::
	buffernumberstring STR_VAR_1, 1
	setvar VAR_0x8004, 1
	goto BattleFrontier_ScottsHouse_EventScript_GiveBattlePoints
	end

BattleFrontier_ScottsHouse_EventScript_GiveBattlePoints::
	special GiveFrontierBattlePoints
	msgbox BattleFrontier_ScottsHouse_Text_ObtainedXBattlePoints, MSGBOX_GETPOINTS
	msgbox BattleFrontier_ScottsHouse_Text_ExplainBattlePoints, MSGBOX_DEFAULT
	setflag FLAG_SCOTT_GIVES_BATTLE_POINTS
	setflag FLAG_TEMP_2
	release
	end

BattleFrontier_ScottsHouse_EventScript_ScottFaceAwayNorth::
	applymovement LOCALID_SCOTT, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	return

BattleFrontier_ScottsHouse_EventScript_ScottFaceAwaySouth::
	applymovement LOCALID_SCOTT, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	return

BattleFrontier_ScottsHouse_EventScript_ScottFaceAwayEast::
	applymovement LOCALID_SCOTT, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	return

BattleFrontier_ScottsHouse_EventScript_ScottFaceAwayWest::
	applymovement LOCALID_SCOTT, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	return

BattleFrontier_ScottsHouse_Text_WelcomeToBattleFrontier:
	.string "BRENDAN: Yo, welcome in!\n"
	.string "Heh… Sorry it's tight in here.\p"
	.string "Anyway, {PLAYER}{KUN}, welcome for real\n"
	.string "to the BATTLE FRONTIER.\p"
	.string "This place is the dream.\n"
	.string "Took years, but now it's real.$"

BattleFrontier_ScottsHouse_Text_HowMuchEffortItTookToMakeReal:
	.string "Looking back, it was a long road…\p"
	.string "I left home looking for strong\n"
	.string "TRAINERS everywhere.\p"
	.string "Nobody knows how much time and\n"
	.string "work made this place real.$"

BattleFrontier_ScottsHouse_Text_HaveThisAsMementoOfOurPathsCrossing:
	.string "But that's old news.\n"
	.string "No need to dwell on it.\p"
	.string "I want you battling here till your\n"
	.string "heart's full, deadass.\p"
	.string "{PLAYER}{KUN}, hold this as a memory\n"
	.string "of all the times our paths crossed.$"

BattleFrontier_ScottsHouse_Text_ObtainedXBattlePoints:
	.string "{PLAYER} obtained\n"
	.string "{STR_VAR_1} Battle Point(s).$"

BattleFrontier_ScottsHouse_Text_ExplainBattlePoints:
	.string "BRENDAN: Check your Battle Points\n"
	.string "on your FRONTIER PASS.\p"
	.string "The more you win around here,\n"
	.string "the more Battle Points you stack.\p"
	.string "Spend them how you want,\n"
	.string "like trading them for items.$"

BattleFrontier_ScottsHouse_Text_ExpectingGreatThings:
	.string "I'm expecting big things from you!$"

BattleFrontier_ScottsHouse_Text_WhyIGoSeekingTrainers:
	.string "BRENDAN: Every TRAINER's got their\n"
	.string "own life, their own story.\p"
	.string "But in battle, none of that matters.\p"
	.string "Pressure makes everybody equal.\p"
	.string "That's why I look everywhere for\n"
	.string "TRAINERS serious about battling,\l"
	.string "then bring them here.$"

BattleFrontier_ScottsHouse_Text_HaveYouMetFrontierBrain:
	.string "BRENDAN: You meet any of\n"
	.string "the FRONTIER BRAINS yet?\p"
	.string "Better question--you take any\n"
	.string "symbols from them?\p"
	.string "I picked those BRAINS because\n"
	.string "they're tough as anybody.\p"
	.string "But you? You'll make even them\n"
	.string "look twice.$"

BattleFrontier_ScottsHouse_Text_MayFindWildMonsInFrontier:
	.string "BRENDAN: You don't just train for battle,\n"
	.string "right?\p"
	.string "I remember you working on\n"
	.string "a POKéDEX too.\p"
	.string "Well, guess what?\p"
	.string "You might run into wild POKéMON\n"
	.string "somewhere in the FRONTIER…\l"
	.string "Word.$"

BattleFrontier_ScottsHouse_Text_YouveCollectedAllSilverSymbols:
	.string "BRENDAN: You enjoying the\n"
	.string "BATTLE FRONTIER?\p"
	.string "…Wait.\n"
	.string "Your FRONTIER PASS!\p"
	.string "You got every silver symbol!\p"
	.string "That's serious work.\n"
	.string "You're tough, just like I figured.\p"
	.string "I don't usually do this,\n"
	.string "but this moment's special.\p"
	.string "Take this.\n"
	.string "You'll put it to good use.$"

BattleFrontier_ScottsHouse_Text_YouveCollectedAllGoldSymbols:
	.string "BRENDAN: Hope you're enjoying\n"
	.string "everything in the BATTLE FRONTIER.\p"
	.string "…Wait.\n"
	.string "Hold up, for real?\p"
	.string "Your FRONTIER PASS!\n"
	.string "You got every gold symbol!\p"
	.string "That's unreal.\n"
	.string "You're elite, just like I figured.\p"
	.string "… … … … … …\p"
	.string "{PLAYER}, you gotta take this!\p"
	.string "You'll understand what this gift\n"
	.string "is worth.$"

BattleFrontier_ScottsHouse_Text_SoGladIBroughtYouHere:
	.string "I know a good TRAINER when\n"
	.string "I see one.\p"
	.string "I'm glad I brought you here,\n"
	.string "for real.$"

BattleFrontier_ScottsHouse_Text_BerryPocketStuffed:
	.string "Your BERRY POCKET seems to\n"
	.string "be stuffed.$"

BattleFrontier_ScottsHouse_Text_Beat50TrainersInARow:
	.string "BRENDAN: Yo, I heard about you!\n"
	.string "You ran the BATTLE TOWER!\l"
	.string "Over 50 TRAINERS in a row?\p"
	.string "That's wild.\n"
	.string "Here, take this!$"

BattleFrontier_ScottsHouse_Text_Beat100TrainersInARow:
	.string "BRENDAN: Yo, I heard about you!\n"
	.string "You owned the BATTLE TOWER!\l"
	.string "Over 100 TRAINERS in a row?\p"
	.string "That's ridiculous.\n"
	.string "You gotta have this!$"

BattleFrontier_ScottsHouse_Text_ExpectingToHearEvenGreaterThings:
	.string "Now I'm expecting to hear even\n"
	.string "bigger things about you!$"

BattleFrontier_ScottsHouse_Text_ComeBackForThisLater:
	.string "Oops, well, if you have too much\n"
	.string "stuff, come back for this later.$"
