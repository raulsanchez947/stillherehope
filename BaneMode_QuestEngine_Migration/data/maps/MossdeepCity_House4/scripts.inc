MossdeepCity_House4_MapScripts::
	.byte 0

MossdeepCity_House4_EventScript_Woman::
	lock
	faceplayer
	goto_if_set FLAG_SYS_GAME_CLEAR, MossdeepCity_House4_EventScript_CanBattleAtSecretBases
	msgbox MossdeepCity_House4_Text_BrotherLikesToFindBases, MSGBOX_DEFAULT
	release
	end

MossdeepCity_House4_EventScript_CanBattleAtSecretBases::
	msgbox MossdeepCity_House4_Text_BrotherLikesToVisitBasesAndBattle, MSGBOX_DEFAULT
	release
	end

MossdeepCity_House4_EventScript_NinjaBoy::
	lock
	faceplayer
	msgbox MossdeepCity_House4_Text_IhavegettingBadDreams, MSGBOX_DEFAULT
	release
	end

MossdeepCity_House4_EventScript_Bed::
	lockall
	msgbox Route119_WeatherInstitute_1F_Text_TakeRestInBed, MSGBOX_DEFAULT
	closemessage
	call Common_EventScript_OutOfCenterPartyHeal
	setflag FLAG_UNLOCK_NIGHTMARES
	releaseall
	end

MossdeepCity_House4_EventScript_NoSecretBase::
	msgbox MossdeepCity_House4_Text_MakeSecretBase, MSGBOX_DEFAULT
	release
	end

MossdeepCity_House4_EventScript_Skitty::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_SKITTY, CRY_MODE_NORMAL
	msgbox MossdeepCity_House4_Text_Skitty, MSGBOX_DEFAULT
	waitmoncry
	release
	end

MossdeepCity_House4_Text_BrotherLikesToFindBases:
	.string "My little brother says he likes to go\n"
	.string "find people's SECRET BASES.$"

MossdeepCity_House4_Text_BrotherLikesToVisitBasesAndBattle:
	.string "My little brother says he likes to\n"
	.string "visit people's SECRET BASES and have\l"
	.string "POKéMON battles.$"

MossdeepCity_House4_Text_YouMadeSecretBaseNearX:
	.string "Was it you who made a SECRET BASE\n"
	.string "near {STR_VAR_1}?$"

MossdeepCity_House4_Text_MakeSecretBase:
	.string "You should make a SECRET BASE\n"
	.string "somewhere. I'll go find it!$"

MossdeepCity_House4_Text_Skitty:
	.string "SKITTY: Miyaan?$"

MossdeepCity_House4_Text_IhavegettingBadDreams:
	.string "I recently started having these\n"
	.string "nightmares, It's a truly bizarre dream.\p"
	.string "Perhaps I am the chosen one! Hahaha.$"
