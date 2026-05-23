FortreeCity_Gym_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, FortreeCity_Gym_OnTransition
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, FortreeCity_Gym_OnWarp
	.byte 0

FortreeCity_Gym_OnTransition:
	special RotatingGate_InitPuzzle
  setflag FLAG_TEMP_HIDE_FOLLOWER @ Hide follower bc of rotating gates
	end

FortreeCity_Gym_OnWarp:
	map_script_2 VAR_TEMP_0, VAR_TEMP_0, FortreeCity_Gym_EventScript_InitRotatingGates
	.2byte 0

FortreeCity_Gym_EventScript_InitRotatingGates::
	special RotatingGate_InitPuzzleAndGraphics
	end

FortreeCity_Gym_EventScript_Winona::
	trainerbattle_double TRAINER_TATE_AND_LIZA_1, FortreeCity_Gym_Text_WinonaIntro, FortreeCity_Gym_Text_WinonaDefeat, FortreeCity_Gym_Text_WinonaRematchNeedTwoMons, FortreeCity_Gym_EventScript_WinonaDefeated, NO_MUSIC
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, FortreeCity_Gym_EventScript_WinonaRematch
	goto_if_unset FLAG_RECEIVED_TM04, FortreeCity_Gym_EventScript_GiveAerialAce2
	msgbox FortreeCity_Gym_Text_WinonaPostBattle, MSGBOX_DEFAULT
	release
	end

FortreeCity_Gym_EventScript_WinonaDefeated::
	message FortreeCity_Gym_Text_ReceivedFeatherBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox FortreeCity_Gym_Text_ExplainFeatherBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_FORTREE_GYM
	setflag FLAG_BADGE06_GET
	setvar VAR_0x8008, 6
	call Common_EventScript_SetGymTrainers
	call FortreeCity_Gym_EventScript_GiveAerialAce
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox FortreeCity_Gym_Text_RegisteredWinona, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_WINONA_MATCH_CALL
	setvar VAR_SCOTT_FORTREE_CALL_STEP_COUNTER, 0
	setflag FLAG_SCOTT_CALL_FORTREE_GYM
	special BadgeCount
	addrankpoints 5
	release
	end

FortreeCity_Gym_EventScript_GiveAerialAce2::
	giveitem ITEM_LIGHT_CLAY
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	msgbox FortreeCity_Gym_Text_ExplainAerialAce, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM04
	release
	end

FortreeCity_Gym_EventScript_GiveAerialAce::
	giveitem ITEM_LIGHT_CLAY
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_BagIsFull
	msgbox FortreeCity_Gym_Text_ExplainAerialAce, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM04
	return

FortreeCity_Gym_EventScript_WinonaRematch::
	trainerbattle_rematch_double TRAINER_TATE_AND_LIZA_1, FortreeCity_Gym_Text_WinonaPreRematch, FortreeCity_Gym_Text_WinonaRematchDefeat, FortreeCity_Gym_Text_WinonaRematchNeedTwoMons
	msgbox FortreeCity_Gym_Text_WinonaPostRematch, MSGBOX_AUTOCLOSE
	end

FortreeCity_Gym_EventScript_Jared::
	trainerbattle_single TRAINER_JARED, FortreeCity_Gym_Text_JaredIntro, FortreeCity_Gym_Text_JaredDefeat
	msgbox FortreeCity_Gym_Text_JaredPostBattle, MSGBOX_AUTOCLOSE
	end

FortreeCity_Gym_EventScript_Edwardo::
	trainerbattle_single TRAINER_EDWARDO, FortreeCity_Gym_Text_EdwardoIntro, FortreeCity_Gym_Text_EdwardoDefeat
	msgbox FortreeCity_Gym_Text_EdwardoPostBattle, MSGBOX_AUTOCLOSE
	end

FortreeCity_Gym_EventScript_Flint::
	trainerbattle_single TRAINER_FLINT, FortreeCity_Gym_Text_FlintIntro, FortreeCity_Gym_Text_FlintDefeat
	msgbox FortreeCity_Gym_Text_FlintPostBattle, MSGBOX_AUTOCLOSE
	end

FortreeCity_Gym_EventScript_Ashley::
	trainerbattle_single TRAINER_ASHLEY, FortreeCity_Gym_Text_AshleyIntro, FortreeCity_Gym_Text_AshleyDefeat
	msgbox FortreeCity_Gym_Text_AshleyPostBattle, MSGBOX_AUTOCLOSE
	end

FortreeCity_Gym_EventScript_Humberto::
	trainerbattle_single TRAINER_HUMBERTO, FortreeCity_Gym_Text_HumbertoIntro, FortreeCity_Gym_Text_HumbertoDefeat
	msgbox FortreeCity_Gym_Text_HumbertoPostBattle, MSGBOX_AUTOCLOSE
	end

FortreeCity_Gym_EventScript_Darius::
	trainerbattle_single TRAINER_DARIUS, FortreeCity_Gym_Text_DariusIntro, FortreeCity_Gym_Text_DariusDefeat
	msgbox FortreeCity_Gym_Text_DariusPostBattle, MSGBOX_AUTOCLOSE
	end

FortreeCity_Gym_EventScript_GymGuide::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_FORTREE_GYM, FortreeCity_Gym_EventScript_GymGuidePostVictory
	msgbox FortreeCity_Gym_Text_GymGuideAdvice, MSGBOX_DEFAULT
	release
	end

FortreeCity_Gym_EventScript_GymGuidePostVictory::
	msgbox FortreeCity_Gym_Text_GymGuidePostVictory, MSGBOX_DEFAULT
	release
	end

FortreeCity_Gym_EventScript_LeftGymStatue::
	lockall
	goto_if_set FLAG_BADGE06_GET, FortreeCity_Gym_EventScript_GymStatueCertified
	goto FortreeCity_Gym_EventScript_GymStatue
	end

FortreeCity_Gym_EventScript_RightGymStatue::
	lockall
	goto_if_set FLAG_BADGE06_GET, FortreeCity_Gym_EventScript_GymStatueCertified
	goto FortreeCity_Gym_EventScript_GymStatue
	end

FortreeCity_Gym_EventScript_GymStatueCertified::
	msgbox FortreeCity_Gym_Text_GymStatueCertified, MSGBOX_DEFAULT
	releaseall
	end

FortreeCity_Gym_EventScript_GymStatue::
	msgbox FortreeCity_Gym_Text_GymStatue, MSGBOX_DEFAULT
	releaseall
	end

FortreeCity_Gym_Text_GymGuideAdvice:
	.string "Yo, how's it going, CHAMPION-\n"
	.string "bound {PLAYER}?\p"
	.string "FORTREE'S gym got mean.\n"
	.string "BLUE and GREEN run this floor now.\p"
	.string "They're waiting at the back of this GYM,\n"
	.string "behind the rotating doors.\p"
	.string "This ain't a solo flex anymore.\n"
	.string "They fight as a doubles unit.\p"
	.string "Okay, go for it!$"

FortreeCity_Gym_Text_GymGuidePostVictory:
	.string "You did it!\n"
	.string "You've achieved liftoff!$"

FortreeCity_Gym_Text_JaredIntro:
	.string "You made it this far?\n"
	.string "Then show me you can think in pairs.$"

FortreeCity_Gym_Text_JaredDefeat:
	.string "You…\n"
	.string "You're strong…$"

FortreeCity_Gym_Text_JaredPostBattle:
	.string "BLUE and GREEN punish bad turns.\n"
	.string "Don't walk in there half-asleep.$"

FortreeCity_Gym_Text_EdwardoIntro:
	.string "These two made the gym nastier on\n"
	.string "purpose.\p"
	.string "As a member of FORTREE GYM,\n"
	.string "I'm not allowed to fold!$"

FortreeCity_Gym_Text_EdwardoDefeat:
	.string "It was too much of a load for me\n"
	.string "to bear…$"

FortreeCity_Gym_Text_EdwardoPostBattle:
	.string "The world is huge, and there are\n"
	.string "countless tough TRAINERS.\p"
	.string "I'm going to keep training and make\n"
	.string "myself even stronger.$"

FortreeCity_Gym_Text_FlintIntro:
	.string "There's no need for BLUE and GREEN\n"
	.string "to deal with you!\p"
	.string "I'm plenty good enough for you!$"

FortreeCity_Gym_Text_FlintDefeat:
	.string "LEADERS, I…\n"
	.string "I lost!$"

FortreeCity_Gym_Text_FlintPostBattle:
	.string "BLUE calls the pace.\n"
	.string "GREEN covers every crack.\p"
	.string "It's brutal to watch.$"

FortreeCity_Gym_Text_AshleyIntro:
	.string "GREEN taught me personally!\n"
	.string "You can't beat me easily!$"

FortreeCity_Gym_Text_AshleyDefeat:
	.string "I was beaten…$"

FortreeCity_Gym_Text_AshleyPostBattle:
	.string "Thanks to BLUE and GREEN, the people\n"
	.string "of FORTREE stay sharp.$"

FortreeCity_Gym_Text_HumbertoIntro:
	.string "When BLUE and GREEN lock in,\n"
	.string "the whole room changes.\p"
	.string "I'm not letting you witness that\n"
	.string "for free!$"

FortreeCity_Gym_Text_HumbertoDefeat:
	.string "Urk!\n"
	.string "I couldn't stop you.$"

FortreeCity_Gym_Text_HumbertoPostBattle:
	.string "Don't get distracted trying to read\n"
	.string "just one side of the field.\p"
	.string "That's how they bury people.$"

FortreeCity_Gym_Text_DariusIntro:
	.string "You'd better know there are all kinds\n"
	.string "of ways to lose a doubles fight.\p"
	.string "You do know that, right?$"

FortreeCity_Gym_Text_DariusDefeat:
	.string "You seem to know your stuff!$"

FortreeCity_Gym_Text_DariusPostBattle:
	.string "Sure, you beat me all right.\n"
	.string "But our leaders don't waste turns.\l"
	.string "BLUE and GREEN are all business.$"

FortreeCity_Gym_Text_WinonaIntro:
	.string "BLUE: You made it through the maze.\n"
	.string "Good.\p"
	.string "GREEN: Then stop gawking and lock in,\n"
	.string "my guy.\p"
	.string "We don't do sloppy turns in here.\p"
	.string "BLUE: If your team blinks once,\n"
	.string "we tax that instantly.\p"
	.string "GREEN: Show us something clean.\n"
	.string "And don't scuff the Timbs.$"

FortreeCity_Gym_Text_WinonaDefeat:
	.string "BLUE: Hah. You stayed solid.\p"
	.string "GREEN: Fine. You earned this badge.\p"
	.string "Don't get cute about it.$"

FortreeCity_Gym_Text_ReceivedFeatherBadge:
	.string "{PLAYER} received the FEATHER BADGE\n"
	.string "from BLUE and GREEN.$"

FortreeCity_Gym_Text_ExplainFeatherBadgeTakeThis:
	.string "With the FEATHER BADGE, all POKéMON up\n"
	.string "to LV 70, even those received through\l"
	.string "trades, will obey your every command.\p"
	.string "You will also be able to use\n"
	.string "the HM move FLY outside of battle.\p"
	.string "And this…\n"
	.string "Call it a little respect.$"

FortreeCity_Gym_Text_ExplainAerialAce:
	.string "LIGHT CLAY keeps screens up longer.\p"
	.string "Psychic pressure is angles and timing.\n"
	.string "Make the setup count.$"

FortreeCity_Gym_Text_RegisteredWinona:
	.string "This number is blocked!$"

FortreeCity_Gym_Text_WinonaPostBattle:
	.string "BLUE: You earned the room.\p"
	.string "GREEN: Next time, don't expect us\n"
	.string "to be this polite.$"

FortreeCity_Gym_Text_GymStatue:
	.string "FORTREE CITY POKéMON GYM$"

FortreeCity_Gym_Text_GymStatueCertified:
	.string "FORTREE CITY POKéMON GYM\p"
	.string "BLUE & GREEN'S CERTIFIED TRAINERS:\n"
	.string "{PLAYER}$"

FortreeCity_Gym_Text_WinonaPreRematch:
	.string "BLUE: Back again? Good.\p"
	.string "GREEN: Then don't waste our time.\p"
	.string "Let's see if your champ talk is real.$"

FortreeCity_Gym_Text_WinonaRematchDefeat:
	.string "BLUE: Still solid.\n"
	.string "GREEN: Annoyingly solid.$"

FortreeCity_Gym_Text_WinonaPostRematch:
	.string "BLUE: We sharpen people here.\p"
	.string "GREEN: You survived the lesson.\n"
	.string "That's enough for today.$"

FortreeCity_Gym_Text_WinonaRematchNeedTwoMons:
	.string "BLUE: This floor is doubles.\p"
	.string "GREEN: Come back when you have at\n"
	.string "least two POKéMON ready to work.$"
