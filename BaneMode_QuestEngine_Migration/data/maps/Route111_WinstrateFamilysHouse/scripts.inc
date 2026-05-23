.set LOCALID_VIVI, 1
.set LOCALID_VICTOR, 2
.set LOCALID_VICTORIA, 3
.set LOCALID_VICKY, 4
.set LOCALID_BANE, 5

Route111_WinstrateFamilysHouse_MapScripts::
	.byte 0

Route111_WinstrateFamilysHouse_EventScript_Victor::
	lock
	faceplayer
	setvar VAR_0x8008, LOCALID_VICTOR
	msgbox Route111_WinstrateFamilysHouse_Text_MySonIsStrongerThanYou, MSGBOX_DEFAULT
	goto Route111_WinstrateFamilysHouse_EventScript_FaceOriginalDirection
	end

@ Uses lockall so we can transition directly into Bane's approach scene
@ after giving the Macho Brace without requiring the player to re-enter.
Route111_WinstrateFamilysHouse_EventScript_Victoria::
	lockall
	faceplayer
	setvar VAR_0x8008, LOCALID_VICTORIA
	goto_if_set FLAG_RECEIVED_MACHO_BRACE, Route111_WinstrateFamilysHouse_EventScript_ReceivedMachoBrace
	msgbox Route111_WinstrateFamilysHouse_Text_LikeYouToHaveMachoBrace, MSGBOX_DEFAULT
	giveitem ITEM_MACHO_BRACE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_MACHO_BRACE
	closemessage
	applymovement LOCALID_VICTORIA, Common_Movement_FaceOriginalDirection
	waitmovement 0
	applymovement LOCALID_BANE, Route111_WinstrateFamilysHouse_Movement_BaneApproach
	waitmovement 0
	applymovement LOCALID_BANE, Common_Movement_FacePlayer
	waitmovement 0
	goto Route111_WinstrateFamilysHouse_EventScript_BaneChallenge
	end

Route111_WinstrateFamilysHouse_EventScript_ReceivedMachoBrace::
	msgbox Route111_WinstrateFamilysHouse_Text_PassionateAboutBattles, MSGBOX_DEFAULT
	goto Route111_WinstrateFamilysHouse_EventScript_FaceOriginalDirection
	end

Route111_WinstrateFamilysHouse_EventScript_Vivi::
	lock
	faceplayer
	setvar VAR_0x8008, LOCALID_VIVI
	msgbox Route111_WinstrateFamilysHouse_Text_StrongerFamilyMembers, MSGBOX_DEFAULT
	goto Route111_WinstrateFamilysHouse_EventScript_FaceOriginalDirection
	end

Route111_WinstrateFamilysHouse_EventScript_Vicky::
	lock
	faceplayer
	setvar VAR_0x8008, LOCALID_VICKY
	goto_if_set FLAG_TEMP_4, Route111_WinstrateFamilysHouse_EventScript_AlreadySpokenTo
	msgbox Route111_WinstrateFamilysHouse_Text_GrandsonStrong, MSGBOX_DEFAULT
	setflag FLAG_TEMP_4
	goto Route111_WinstrateFamilysHouse_EventScript_FaceOriginalDirection
	end

Route111_WinstrateFamilysHouse_EventScript_AlreadySpokenTo::
	msgbox Route111_WinstrateFamilysHouse_Text_GrandsonStrongShort, MSGBOX_DEFAULT
	goto Route111_WinstrateFamilysHouse_EventScript_FaceOriginalDirection
	end

Route111_WinstrateFamilysHouse_EventScript_Bane::
	lock
	faceplayer
	setvar VAR_0x8008, LOCALID_BANE
	goto_if_unset FLAG_RECEIVED_MACHO_BRACE, Route111_WinstrateFamilysHouse_EventScript_BaneNotYet
	goto_if_set FLAG_TEMP_5, Route111_WinstrateFamilysHouse_EventScript_BaneCooldown
	checktrainerflag TRAINER_BANEMODE_WINSTRATE_BANE
	goto_if_eq VAR_RESULT, TRUE, Route111_WinstrateFamilysHouse_EventScript_BaneAfterBattle
	goto Route111_WinstrateFamilysHouse_EventScript_BaneChallenge
	end

@ Shared entry point for both direct Bane interaction and Victoria-triggered scene
Route111_WinstrateFamilysHouse_EventScript_BaneChallenge::
	setvar VAR_0x8008, LOCALID_BANE
	msgbox Route111_WinstrateFamilysHouse_Text_BaneIntro, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Route111_WinstrateFamilysHouse_EventScript_BattleBane
	msgbox Route111_WinstrateFamilysHouse_Text_BaneDecline, MSGBOX_DEFAULT
	goto Route111_WinstrateFamilysHouse_EventScript_FaceOriginalDirection
	end

Route111_WinstrateFamilysHouse_EventScript_BaneNotYet::
	msgbox Route111_WinstrateFamilysHouse_Text_BaneNotYet, MSGBOX_DEFAULT
	goto Route111_WinstrateFamilysHouse_EventScript_FaceOriginalDirection
	end

Route111_WinstrateFamilysHouse_EventScript_BaneCooldown::
	msgbox Route111_WinstrateFamilysHouse_Text_BaneCooldown, MSGBOX_DEFAULT
	goto Route111_WinstrateFamilysHouse_EventScript_FaceOriginalDirection
	end

@ trainerbattle_no_intro skips the walk-up animation - correct for player-initiated battles
Route111_WinstrateFamilysHouse_EventScript_BattleBane::
	msgbox Route111_WinstrateFamilysHouse_Text_BaneAccept, MSGBOX_DEFAULT
	closemessage
	trainerbattle_no_intro TRAINER_BANEMODE_WINSTRATE_BANE, Route111_WinstrateFamilysHouse_Text_BaneDefeat
	setflag FLAG_TEMP_5
	msgbox Route111_WinstrateFamilysHouse_Text_BaneAfterBattle, MSGBOX_DEFAULT
	goto Route111_WinstrateFamilysHouse_EventScript_FaceOriginalDirection
	end

Route111_WinstrateFamilysHouse_EventScript_BaneAfterBattle::
	msgbox Route111_WinstrateFamilysHouse_Text_BaneAfterBattle, MSGBOX_DEFAULT
	goto Route111_WinstrateFamilysHouse_EventScript_FaceOriginalDirection
	end

Route111_WinstrateFamilysHouse_EventScript_FaceOriginalDirection::
	closemessage
	applymovement VAR_0x8008, Common_Movement_FaceOriginalDirection
	waitmovement 0
	releaseall
	end

Route111_WinstrateFamilysHouse_Movement_BaneApproach:
	walk_down
	walk_down
	step_end

Route111_WinstrateFamilysHouse_Text_MySonIsStrongerThanYou:
	.string "You're the first TRAINER I've seen who\n"
	.string "deploys POKéMON so masterfully.\p"
	.string "But, I should tell you--my son is\n"
	.string "stronger than you.\p"
	.string "He even took the POKéMON LEAGUE\n"
	.string "challenge, I'll have you know.$"

Route111_WinstrateFamilysHouse_Text_LikeYouToHaveMachoBrace:
	.string "We use this MACHO BRACE to more\n"
	.string "effectively strengthen our POKéMON\l"
	.string "in training.\p"
	.string "Since you've beaten all of us here,\n"
	.string "I don't know if you need it, but we\l"
	.string "would like you to have our MACHO BRACE.$"

Route111_WinstrateFamilysHouse_Text_PassionateAboutBattles:
	.string "When it comes to POKéMON battles,\n"
	.string "we tend to be pretty passionate.$"

Route111_WinstrateFamilysHouse_Text_StrongerFamilyMembers:
	.string "Mommy is stronger than Daddy.\p"
	.string "I'm stronger than Mommy.\p"
	.string "And Grandma's stronger than me!\p"
	.string "But my big brother is even stronger\n"
	.string "than Grandma.$"

Route111_WinstrateFamilysHouse_Text_GrandsonStrong:
	.string "There's no question that you're strong.\p"
	.string "But if you were to battle my grandson,\n"
	.string "you'd end up crying in frustration.\p"
	.string "He's much stronger than any TRAINER\n"
	.string "our family knows.\p"
	.string "He must be challenging the POKéMON\n"
	.string "LEAGUE CHAMPION by now.\p"
	.string "Knowing my grandson, he could be the\n"
	.string "CHAMPION already!$"

Route111_WinstrateFamilysHouse_Text_GrandsonStrongShort:
	.string "My grandson must be challenging the\n"
	.string "POKéMON LEAGUE CHAMPION by now.\p"
	.string "Knowing my grandson, he could be the\n"
	.string "CHAMPION already!$"

Route111_WinstrateFamilysHouse_Text_BaneNotYet:
	.string "Yo, I'm BANE. I just moved up from\n"
	.string "Florida, and this whole house is still\l"
	.string "doing intake before they let me cook.\p"
	.string "Beat the fam first, then we can talk\n"
	.string "real pressure, my guy.$"

Route111_WinstrateFamilysHouse_Text_BaneIntro:
	.string "Aight, so you're the one folding the\n"
	.string "WINSTRATES? Respect.\p"
	.string "I'm BANE. Bit of a nerd, bit of a\n"
	.string "Florida mutant, trying to sound less\l"
	.string "Tampa and more New York every week.\p"
	.string "You wanna see six SCRAFTY run the\n"
	.string "house, or you ducking smoke?$"

Route111_WinstrateFamilysHouse_Text_BaneDecline:
	.string "No pressure, no panic. When you ready,\n"
	.string "pull back up and I'll still be here\l"
	.string "optimizing the whole room.$"

Route111_WinstrateFamilysHouse_Text_BaneAccept:
	.string "Bet. Pocket protector on, pressure up.\n"
	.string "Let's get weird, kid.$"

Route111_WinstrateFamilysHouse_Text_BaneDefeat:
	.string "Nah, that's nasty work...\n"
	.string "you really broke the stack.$"

Route111_WinstrateFamilysHouse_Text_BaneAfterBattle:
	.string "You got it. Florida in the lungs,\n"
	.string "New York in the talk, six SCRAFTY in\l"
	.string "the notes, and still you packed me.\p"
	.string "That's official. You're built for the\n"
	.string "next wave, no cap.$"

Route111_WinstrateFamilysHouse_Text_BaneCooldown:
	.string "You already clipped me, fam.\n"
	.string "Now I'm just rebuilding lines and\l"
	.string "pretending that wasn't on stream.$"
