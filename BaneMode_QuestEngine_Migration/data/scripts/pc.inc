EventScript_PC::
	lockall
	setvar VAR_0x8004, PC_LOCATION_OTHER
	special DoPCTurnOnEffect
	playse SE_PC_ON
	setflag FLAG_POKEMONPCMENU
	msgbox Text_BootUpPC, MSGBOX_DEFAULT
	goto EventScript_PCMainMenu
	end

EventScript_Box::
	lockall
	goto_if_set FLAG_LOCK_BOX, EventScript_EndBox
	playse SE_PC_ON
	setflag FLAG_POKEMONPCMENU
	goto EventScript_AccessBox
	end

EventScript_EndBox::
releaseall
end

EventScript_PCMainMenu::
	message gText_WhichPCShouldBeAccessed
	waitmessage
	special ScriptMenu_CreatePCMultichoice
	waitstate
	goto EventScript_AccessPC
	end

EventScript_AccessPC::
	switch VAR_RESULT
	case 0, EventScript_AccessPokemonStorage
	case 1, EventScript_AccessPlayersPC
	case 2, EventScript_AccessBaneModePC
	case 3, EventScript_AccessHallOfFame
	case 4, EventScript_TurnOffPC
	case MULTI_B_PRESSED, EventScript_TurnOffPC
	end

EventScript_AccessBaneModePC::
	playse SE_PC_LOGIN
	msgbox BaneMode_PC_Text_Accessed, MSGBOX_DEFAULT
	goto BaneMode_PC_EventScript_Menu
	end

EventScript_AccessPlayersPC::
	playse SE_PC_LOGIN
	msgbox gText_AccessedPlayersPC, MSGBOX_DEFAULT
	special PlayerPC
	waitstate
	goto EventScript_PCMainMenu
	end

EventScript_AccessPokemonStorage::
	playse SE_PC_LOGIN
	call_if_unset FLAG_SYS_PC_LANETTE, EventScript_AccessSomeonesPC
	call_if_set FLAG_SYS_PC_LANETTE, EventScript_AccessLanettesPC
	msgbox gText_StorageSystemOpened, MSGBOX_DEFAULT
	special ShowPokemonStorageSystemPC
	waitstate
	goto EventScript_PCMainMenu
	end

EventScript_AccessBox::
	playse SE_PC_LOGIN
	special ShowPokemonStorageSystemPC
	waitstate
	goto EventScript_TurnOffBox
	end

EventScript_AccessSomeonesPC::
	msgbox gText_AccessedSomeonesPC, MSGBOX_DEFAULT
	return

EventScript_AccessLanettesPC::
	msgbox gText_AccessedLanettesPC, MSGBOX_DEFAULT
	return

EventScript_TurnOffPC::
	setvar VAR_0x8004, PC_LOCATION_OTHER
	playse SE_PC_OFF
	special DoPCTurnOffEffect
	clearflag FLAG_POKEMONPCMENU
	releaseall
	end

EventScript_TurnOffBox::
	playse SE_PC_OFF
	clearflag FLAG_POKEMONPCMENU
	releaseall
	end

EventScript_AccessHallOfFame::
	goto_if_unset FLAG_SYS_GAME_CLEAR, EventScript_TurnOffPC
	playse SE_PC_LOGIN
	special AccessHallOfFamePC
	waitstate
	goto EventScript_AccessPC
	end

EventScript_AccessPokemonBoxLink::
	playse SE_PC_LOGIN
	msgbox gText_StorageSystemOpened, MSGBOX_DEFAULT
	special ShowPokemonStorageSystemPC
	waitstate
	releaseall
	end

BaneMode_PC_EventScript_Menu::
	message BaneMode_PC_Text_SelectService
	waitmessage
	dynmultipush BaneMode_PC_Text_LevelUpTutor, 0
	dynmultipush BaneMode_PC_Text_EggMoveTutor, 1
	dynmultipush BaneMode_PC_Text_TMOverflowTutor, 2
	dynmultipush BaneMode_PC_Text_NatureChanger, 3
	dynmultipush BaneMode_PC_Text_Shiny, 4
	dynmultipush BaneMode_PC_Text_Return, 5
	dynmultistack 0, 0, FALSE, 6, 0, 0, DYN_MULTICHOICE_CB_NONE
	closemessage
	switch VAR_RESULT
	case 0, BaneMode_PC_EventScript_LevelUpTutor
	case 1, BaneMode_PC_EventScript_EggMoveTutor
	case 2, BaneMode_PC_EventScript_TMOverflowTutor
	case 3, BaneMode_PC_EventScript_NatureChanger
	case 4, BaneMode_PC_EventScript_ShinyRate
	case 5, EventScript_PCMainMenu
	case MULTI_B_PRESSED, EventScript_PCMainMenu
	end

BaneMode_PC_EventScript_LevelUpTutor::
	clearflag FLAG_EGG_MOVES_TUTOR
	clearflag FLAG_TM_OVERFLOW_TUTOR
	msgbox BaneMode_Text_LevelUpTutorIntro, MSGBOX_DEFAULT
	goto BaneMode_PC_EventScript_ChooseMonForTutor
	end

BaneMode_PC_EventScript_EggMoveTutor::
	clearflag FLAG_TM_OVERFLOW_TUTOR
	setflag FLAG_EGG_MOVES_TUTOR
	msgbox BaneMode_Text_EggMoveTutorIntro, MSGBOX_DEFAULT
	goto BaneMode_PC_EventScript_ChooseMonForTutor
	end

BaneMode_PC_EventScript_TMOverflowTutor::
	clearflag FLAG_EGG_MOVES_TUTOR
	setflag FLAG_TM_OVERFLOW_TUTOR
	msgbox BaneMode_Text_TMOverflowTutorIntro, MSGBOX_DEFAULT
	goto BaneMode_PC_EventScript_ChooseMonForTutor
	end

BaneMode_PC_EventScript_ChooseMonForTutor::
	msgbox BaneMode_Text_MoveTutorChooseMon, MSGBOX_DEFAULT
	special ChooseMonForMoveRelearner
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, BaneMode_PC_EventScript_ClearEggTutorAndMenu
	goto_if_eq VAR_0x8005, 0, BaneMode_PC_EventScript_NoMoveToTeach
	goto BaneMode_PC_EventScript_ChooseMove
	end

BaneMode_PC_EventScript_ChooseMove::
	msgbox BaneMode_Text_MoveTutorChooseMove, MSGBOX_DEFAULT
	special TeachMoveRelearnerMove
	waitstate
	goto_if_eq VAR_0x8004, 0, BaneMode_PC_EventScript_ChooseMonForTutor
	msgbox BaneMode_Text_MoveTutorDone, MSGBOX_DEFAULT
	goto BaneMode_PC_EventScript_ClearEggTutorAndMenu
	end

BaneMode_PC_EventScript_NoMoveToTeach::
	msgbox BaneMode_Text_MoveTutorNoMoves, MSGBOX_DEFAULT
	goto BaneMode_PC_EventScript_ChooseMonForTutor
	end

BaneMode_PC_EventScript_ClearEggTutorAndMenu::
	clearflag FLAG_EGG_MOVES_TUTOR
	clearflag FLAG_TM_OVERFLOW_TUTOR
	goto BaneMode_PC_EventScript_Menu
	end

BaneMode_PC_EventScript_NatureChanger::
	msgbox BaneMode_Text_NatureChangerIntro, MSGBOX_DEFAULT
	special ChoosePartyMon
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, BaneMode_PC_EventScript_Menu
	goto BaneMode_PC_EventScript_NatureCategoryMenu
	end

BaneMode_PC_EventScript_NatureCategoryMenu::
	message BaneMode_Text_NatureCategoryPrompt
	waitmessage
	dynmultipush BaneMode_PC_Text_NatureAttack, 0
	dynmultipush BaneMode_PC_Text_NatureDefense, 1
	dynmultipush BaneMode_PC_Text_NatureSpeed, 2
	dynmultipush BaneMode_PC_Text_NatureSpAtk, 3
	dynmultipush BaneMode_PC_Text_NatureSpDef, 4
	dynmultipush BaneMode_PC_Text_NatureNeutral, 5
	dynmultipush BaneMode_PC_Text_Return, 6
	dynmultistack 0, 0, FALSE, 7, 0, 0, DYN_MULTICHOICE_CB_NONE
	closemessage
	switch VAR_RESULT
	case 0, BaneMode_PC_EventScript_NatureAttackMenu
	case 1, BaneMode_PC_EventScript_NatureDefenseMenu
	case 2, BaneMode_PC_EventScript_NatureSpeedMenu
	case 3, BaneMode_PC_EventScript_NatureSpAtkMenu
	case 4, BaneMode_PC_EventScript_NatureSpDefMenu
	case 5, BaneMode_PC_EventScript_NatureNeutralMenu
	case 6, BaneMode_PC_EventScript_Menu
	case MULTI_B_PRESSED, BaneMode_PC_EventScript_Menu
	end

BaneMode_PC_EventScript_NatureAttackMenu::
	message BaneMode_Text_NatureChoosePrompt
	waitmessage
	dynmultipush BaneMode_PC_Text_Adamant, 0
	dynmultipush BaneMode_PC_Text_Brave, 1
	dynmultipush BaneMode_PC_Text_Lonely, 2
	dynmultipush BaneMode_PC_Text_Naughty, 3
	dynmultipush BaneMode_PC_Text_Return, 4
	dynmultistack 0, 0, FALSE, 5, 0, 0, DYN_MULTICHOICE_CB_NONE
	closemessage
	switch VAR_RESULT
	case 0, BaneMode_PC_EventScript_SetNatureAdamant
	case 1, BaneMode_PC_EventScript_SetNatureBrave
	case 2, BaneMode_PC_EventScript_SetNatureLonely
	case 3, BaneMode_PC_EventScript_SetNatureNaughty
	case 4, BaneMode_PC_EventScript_NatureCategoryMenu
	case MULTI_B_PRESSED, BaneMode_PC_EventScript_NatureCategoryMenu
	end

BaneMode_PC_EventScript_NatureDefenseMenu::
	message BaneMode_Text_NatureChoosePrompt
	waitmessage
	dynmultipush BaneMode_PC_Text_Bold, 0
	dynmultipush BaneMode_PC_Text_Relaxed, 1
	dynmultipush BaneMode_PC_Text_Impish, 2
	dynmultipush BaneMode_PC_Text_Lax, 3
	dynmultipush BaneMode_PC_Text_Return, 4
	dynmultistack 0, 0, FALSE, 5, 0, 0, DYN_MULTICHOICE_CB_NONE
	closemessage
	switch VAR_RESULT
	case 0, BaneMode_PC_EventScript_SetNatureBold
	case 1, BaneMode_PC_EventScript_SetNatureRelaxed
	case 2, BaneMode_PC_EventScript_SetNatureImpish
	case 3, BaneMode_PC_EventScript_SetNatureLax
	case 4, BaneMode_PC_EventScript_NatureCategoryMenu
	case MULTI_B_PRESSED, BaneMode_PC_EventScript_NatureCategoryMenu
	end

BaneMode_PC_EventScript_NatureSpeedMenu::
	message BaneMode_Text_NatureChoosePrompt
	waitmessage
	dynmultipush BaneMode_PC_Text_Timid, 0
	dynmultipush BaneMode_PC_Text_Hasty, 1
	dynmultipush BaneMode_PC_Text_Jolly, 2
	dynmultipush BaneMode_PC_Text_Naive, 3
	dynmultipush BaneMode_PC_Text_Return, 4
	dynmultistack 0, 0, FALSE, 5, 0, 0, DYN_MULTICHOICE_CB_NONE
	closemessage
	switch VAR_RESULT
	case 0, BaneMode_PC_EventScript_SetNatureTimid
	case 1, BaneMode_PC_EventScript_SetNatureHasty
	case 2, BaneMode_PC_EventScript_SetNatureJolly
	case 3, BaneMode_PC_EventScript_SetNatureNaive
	case 4, BaneMode_PC_EventScript_NatureCategoryMenu
	case MULTI_B_PRESSED, BaneMode_PC_EventScript_NatureCategoryMenu
	end

BaneMode_PC_EventScript_NatureSpAtkMenu::
	message BaneMode_Text_NatureChoosePrompt
	waitmessage
	dynmultipush BaneMode_PC_Text_Modest, 0
	dynmultipush BaneMode_PC_Text_Quiet, 1
	dynmultipush BaneMode_PC_Text_Mild, 2
	dynmultipush BaneMode_PC_Text_Rash, 3
	dynmultipush BaneMode_PC_Text_Return, 4
	dynmultistack 0, 0, FALSE, 5, 0, 0, DYN_MULTICHOICE_CB_NONE
	closemessage
	switch VAR_RESULT
	case 0, BaneMode_PC_EventScript_SetNatureModest
	case 1, BaneMode_PC_EventScript_SetNatureQuiet
	case 2, BaneMode_PC_EventScript_SetNatureMild
	case 3, BaneMode_PC_EventScript_SetNatureRash
	case 4, BaneMode_PC_EventScript_NatureCategoryMenu
	case MULTI_B_PRESSED, BaneMode_PC_EventScript_NatureCategoryMenu
	end

BaneMode_PC_EventScript_NatureSpDefMenu::
	message BaneMode_Text_NatureChoosePrompt
	waitmessage
	dynmultipush BaneMode_PC_Text_Calm, 0
	dynmultipush BaneMode_PC_Text_Sassy, 1
	dynmultipush BaneMode_PC_Text_Gentle, 2
	dynmultipush BaneMode_PC_Text_Careful, 3
	dynmultipush BaneMode_PC_Text_Return, 4
	dynmultistack 0, 0, FALSE, 5, 0, 0, DYN_MULTICHOICE_CB_NONE
	closemessage
	switch VAR_RESULT
	case 0, BaneMode_PC_EventScript_SetNatureCalm
	case 1, BaneMode_PC_EventScript_SetNatureSassy
	case 2, BaneMode_PC_EventScript_SetNatureGentle
	case 3, BaneMode_PC_EventScript_SetNatureCareful
	case 4, BaneMode_PC_EventScript_NatureCategoryMenu
	case MULTI_B_PRESSED, BaneMode_PC_EventScript_NatureCategoryMenu
	end

BaneMode_PC_EventScript_NatureNeutralMenu::
	message BaneMode_Text_NatureChoosePrompt
	waitmessage
	dynmultipush BaneMode_PC_Text_Hardy, 0
	dynmultipush BaneMode_PC_Text_Docile, 1
	dynmultipush BaneMode_PC_Text_Serious, 2
	dynmultipush BaneMode_PC_Text_Bashful, 3
	dynmultipush BaneMode_PC_Text_Quirky, 4
	dynmultipush BaneMode_PC_Text_Return, 5
	dynmultistack 0, 0, FALSE, 6, 0, 0, DYN_MULTICHOICE_CB_NONE
	closemessage
	switch VAR_RESULT
	case 0, BaneMode_PC_EventScript_SetNatureHardy
	case 1, BaneMode_PC_EventScript_SetNatureDocile
	case 2, BaneMode_PC_EventScript_SetNatureSerious
	case 3, BaneMode_PC_EventScript_SetNatureBashful
	case 4, BaneMode_PC_EventScript_SetNatureQuirky
	case 5, BaneMode_PC_EventScript_NatureCategoryMenu
	case MULTI_B_PRESSED, BaneMode_PC_EventScript_NatureCategoryMenu
	end

BaneMode_PC_EventScript_SetNatureHardy::
	setvar VAR_0x8005, 0
	goto BaneMode_PC_EventScript_ApplyNature
BaneMode_PC_EventScript_SetNatureLonely::
	setvar VAR_0x8005, 1
	goto BaneMode_PC_EventScript_ApplyNature
BaneMode_PC_EventScript_SetNatureBrave::
	setvar VAR_0x8005, 2
	goto BaneMode_PC_EventScript_ApplyNature
BaneMode_PC_EventScript_SetNatureAdamant::
	setvar VAR_0x8005, 3
	goto BaneMode_PC_EventScript_ApplyNature
BaneMode_PC_EventScript_SetNatureNaughty::
	setvar VAR_0x8005, 4
	goto BaneMode_PC_EventScript_ApplyNature
BaneMode_PC_EventScript_SetNatureBold::
	setvar VAR_0x8005, 5
	goto BaneMode_PC_EventScript_ApplyNature
BaneMode_PC_EventScript_SetNatureDocile::
	setvar VAR_0x8005, 6
	goto BaneMode_PC_EventScript_ApplyNature
BaneMode_PC_EventScript_SetNatureRelaxed::
	setvar VAR_0x8005, 7
	goto BaneMode_PC_EventScript_ApplyNature
BaneMode_PC_EventScript_SetNatureImpish::
	setvar VAR_0x8005, 8
	goto BaneMode_PC_EventScript_ApplyNature
BaneMode_PC_EventScript_SetNatureLax::
	setvar VAR_0x8005, 9
	goto BaneMode_PC_EventScript_ApplyNature
BaneMode_PC_EventScript_SetNatureTimid::
	setvar VAR_0x8005, 10
	goto BaneMode_PC_EventScript_ApplyNature
BaneMode_PC_EventScript_SetNatureHasty::
	setvar VAR_0x8005, 11
	goto BaneMode_PC_EventScript_ApplyNature
BaneMode_PC_EventScript_SetNatureSerious::
	setvar VAR_0x8005, 12
	goto BaneMode_PC_EventScript_ApplyNature
BaneMode_PC_EventScript_SetNatureJolly::
	setvar VAR_0x8005, 13
	goto BaneMode_PC_EventScript_ApplyNature
BaneMode_PC_EventScript_SetNatureNaive::
	setvar VAR_0x8005, 14
	goto BaneMode_PC_EventScript_ApplyNature
BaneMode_PC_EventScript_SetNatureModest::
	setvar VAR_0x8005, 15
	goto BaneMode_PC_EventScript_ApplyNature
BaneMode_PC_EventScript_SetNatureMild::
	setvar VAR_0x8005, 16
	goto BaneMode_PC_EventScript_ApplyNature
BaneMode_PC_EventScript_SetNatureQuiet::
	setvar VAR_0x8005, 17
	goto BaneMode_PC_EventScript_ApplyNature
BaneMode_PC_EventScript_SetNatureBashful::
	setvar VAR_0x8005, 18
	goto BaneMode_PC_EventScript_ApplyNature
BaneMode_PC_EventScript_SetNatureRash::
	setvar VAR_0x8005, 19
	goto BaneMode_PC_EventScript_ApplyNature
BaneMode_PC_EventScript_SetNatureCalm::
	setvar VAR_0x8005, 20
	goto BaneMode_PC_EventScript_ApplyNature
BaneMode_PC_EventScript_SetNatureGentle::
	setvar VAR_0x8005, 21
	goto BaneMode_PC_EventScript_ApplyNature
BaneMode_PC_EventScript_SetNatureSassy::
	setvar VAR_0x8005, 22
	goto BaneMode_PC_EventScript_ApplyNature
BaneMode_PC_EventScript_SetNatureCareful::
	setvar VAR_0x8005, 23
	goto BaneMode_PC_EventScript_ApplyNature
BaneMode_PC_EventScript_SetNatureQuirky::
	setvar VAR_0x8005, 24
	goto BaneMode_PC_EventScript_ApplyNature

BaneMode_PC_EventScript_ApplyNature::
	special BaneModeSetSelectedMonNature
	msgbox BaneMode_Text_NatureChangerDone, MSGBOX_DEFAULT
	goto BaneMode_PC_EventScript_Menu
	end

BaneMode_PC_EventScript_ShinyRate::
	message BaneMode_Text_ShinyRateIntro
	waitmessage
	dynmultipush BaneMode_Shiny_Text_Normal, 0
	dynmultipush BaneMode_Shiny_Text_High, 1
	dynmultipush BaneMode_Shiny_Text_UltraHigh, 2
	dynmultipush BaneMode_PC_Text_Return, 3
	dynmultistack 0, 0, FALSE, 4, 0, 0, DYN_MULTICHOICE_CB_NONE
	closemessage
	switch VAR_RESULT
	case 0, BaneMode_EventScript_ShinyRateNormal
	case 1, BaneMode_EventScript_ShinyRateHigh
	case 2, BaneMode_EventScript_ShinyRateUltraHigh
	case 3, EventScript_PCMainMenu
	case MULTI_B_PRESSED, EventScript_PCMainMenu
	end

BaneMode_EventScript_ShinyRateNormal::
	setvar VAR_BANEMODE_SHINY_RATE, 0
	msgbox BaneMode_Text_ShinyRateNormal, MSGBOX_DEFAULT
	goto EventScript_PCMainMenu
	end

BaneMode_EventScript_ShinyRateHigh::
	setvar VAR_BANEMODE_SHINY_RATE, 1
	msgbox BaneMode_Text_ShinyRateHigh, MSGBOX_DEFAULT
	goto EventScript_PCMainMenu
	end

BaneMode_EventScript_ShinyRateUltraHigh::
	setvar VAR_BANEMODE_SHINY_RATE, 2
	msgbox BaneMode_Text_ShinyRateUltraHigh, MSGBOX_DEFAULT
	goto EventScript_PCMainMenu
	end

PC_Locked:
  .string "{PLAYER}'s PC is locked... access from\n"
  .string "personal computer$"

BaneMode_PC_Text_Accessed:
	.string "Accessed the BANE MODE PC.$"

BaneMode_PC_Text_SelectService:
	.string "Which service do you want?$"

BaneMode_PC_Text_LevelUpTutor:
	.string "LEVEL-UP MOVES$"

BaneMode_PC_Text_EggMoveTutor:
	.string "EGG MOVES$"

BaneMode_PC_Text_TMOverflowTutor:
	.string "SV TM TUTOR$"

BaneMode_PC_Text_NatureChanger:
	.string "NATURE$"

BaneMode_PC_Text_Shiny:
	.string "SHINY$"

BaneMode_PC_Text_Return:
	.string "RETURN$"

BaneMode_Text_LevelUpTutorIntro:
	.string "This station reteaches every level-up\n"
	.string "move in your line.$"

BaneMode_Text_EggMoveTutorIntro:
	.string "This station also unlocks egg moves\n"
	.string "your POKeMON can inherit.$"

BaneMode_Text_TMOverflowTutorIntro:
	.string "Past TM100 lives here.\p"
	.string "This station teaches the Scarlet /\n"
	.string "Violet overflow set.$"

BaneMode_Text_MoveTutorChooseMon:
	.string "Which POKeMON needs tutoring?$"

BaneMode_Text_MoveTutorChooseMove:
	.string "Which move should I teach?$"

BaneMode_Text_MoveTutorNoMoves:
	.string "That POKeMON doesn't have a move\n"
	.string "ready to relearn right now.$"

BaneMode_Text_MoveTutorDone:
	.string "Locked in.\p"
	.string "Your POKeMON is sharper already.$"

BaneMode_Text_NatureChangerIntro:
	.string "Pick a POKeMON and I'll tune its\n"
	.string "nature exactly how you want.$"

BaneMode_Text_NatureCategoryPrompt:
	.string "Which kind of nature do you want?$"

BaneMode_Text_NatureChoosePrompt:
	.string "Choose the exact nature.$"

BaneMode_Text_NatureChangerDone:
	.string "Done.\p"
	.string "That nature is locked in.$"

BaneMode_PC_Text_NatureAttack:
	.string "ATTACK$"

BaneMode_PC_Text_NatureDefense:
	.string "DEFENSE$"

BaneMode_PC_Text_NatureSpeed:
	.string "SPEED$"

BaneMode_PC_Text_NatureSpAtk:
	.string "SP. ATK$"

BaneMode_PC_Text_NatureSpDef:
	.string "SP. DEF$"

BaneMode_PC_Text_NatureNeutral:
	.string "NEUTRAL$"

BaneMode_PC_Text_Hardy:
	.string "HARDY$"
BaneMode_PC_Text_Lonely:
	.string "LONELY$"
BaneMode_PC_Text_Brave:
	.string "BRAVE$"
BaneMode_PC_Text_Adamant:
	.string "ADAMANT$"
BaneMode_PC_Text_Naughty:
	.string "NAUGHTY$"
BaneMode_PC_Text_Bold:
	.string "BOLD$"
BaneMode_PC_Text_Docile:
	.string "DOCILE$"
BaneMode_PC_Text_Relaxed:
	.string "RELAXED$"
BaneMode_PC_Text_Impish:
	.string "IMPISH$"
BaneMode_PC_Text_Lax:
	.string "LAX$"
BaneMode_PC_Text_Timid:
	.string "TIMID$"
BaneMode_PC_Text_Hasty:
	.string "HASTY$"
BaneMode_PC_Text_Serious:
	.string "SERIOUS$"
BaneMode_PC_Text_Jolly:
	.string "JOLLY$"
BaneMode_PC_Text_Naive:
	.string "NAIVE$"
BaneMode_PC_Text_Modest:
	.string "MODEST$"
BaneMode_PC_Text_Mild:
	.string "MILD$"
BaneMode_PC_Text_Quiet:
	.string "QUIET$"
BaneMode_PC_Text_Bashful:
	.string "BASHFUL$"
BaneMode_PC_Text_Rash:
	.string "RASH$"
BaneMode_PC_Text_Calm:
	.string "CALM$"
BaneMode_PC_Text_Gentle:
	.string "GENTLE$"
BaneMode_PC_Text_Sassy:
	.string "SASSY$"
BaneMode_PC_Text_Careful:
	.string "CAREFUL$"
BaneMode_PC_Text_Quirky:
	.string "QUIRKY$"

BaneMode_Text_ShinyRateIntro:
	.string "Shiny POKeMON?\p"
	.string "Choose the encounter sparkle rate.$"

BaneMode_Shiny_Text_Normal:
	.string "NORMAL$"

BaneMode_Shiny_Text_High:
	.string "HIGH$"

BaneMode_Shiny_Text_UltraHigh:
	.string "ULTRA HIGH$"

BaneMode_Text_ShinyRateNormal:
	.string "Shiny rate set to NORMAL.\p"
	.string "Classic odds are restored.$"

BaneMode_Text_ShinyRateHigh:
	.string "Shiny rate set to HIGH.\p"
	.string "New encounters get many more\n"
	.string "sparkle checks.$"

BaneMode_Text_ShinyRateUltraHigh:
	.string "Shiny rate set to ULTRA HIGH.\p"
	.string "If it shines, it fights.$"
