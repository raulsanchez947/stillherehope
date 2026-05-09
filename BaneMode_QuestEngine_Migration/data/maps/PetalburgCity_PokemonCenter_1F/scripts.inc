
.set LOCALID_NURSE, 1

PetalburgCity_PokemonCenter_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PetalburgCity_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

PetalburgCity_PokemonCenter_1F_OnTransition:
	setrespawn HEAL_LOCATION_PETALBURG_CITY
	call Common_EventScript_UpdateBrineyLocation
	end

PetalburgCity_PokemonCenter_1F_EventScript_Nurse::
	setvar VAR_0x800B, LOCALID_NURSE
	call Common_EventScript_PkmnCenterNurse
	waitmessage
	waitbuttonpress
	release
	end

PetalburgCity_PokemonCenter_1F_EventScript_FatMan::
	msgbox PetalburgCity_PokemonCenter_1F_Text_PCStorageSystem, MSGBOX_NPC
	end

PetalburgCity_PokemonCenter_1F_EventScript_Youngster::
	msgbox PetalburgCity_PokemonCenter_1F_Text_OranBerryRegainedHP, MSGBOX_NPC
	end

PetalburgCity_PokemonCenter_1F_EventScript_Woman::
	lock
	faceplayer
	msgbox PetalburgCity_PokemonCenter_1F_Text_ManyTypesOfPokemon, MSGBOX_DEFAULT
	specialvar VAR_RESULT, IsStarterInParty
	goto_if_eq VAR_RESULT, TRUE, PetalburgCity_PokemonCenter_1F_EventScript_SayStarterTypeInfo
	release
	end

PetalburgCity_PokemonCenter_1F_EventScript_SayStarterTypeInfo::
	call_if_eq VAR_STARTER_MON, 0, PetalburgCity_PokemonCenter_1F_EventScript_SayPoliwagType
	call_if_eq VAR_STARTER_MON, 1, PetalburgCity_PokemonCenter_1F_EventScript_SayLileepType
	call_if_eq VAR_STARTER_MON, 2, PetalburgCity_PokemonCenter_1F_EventScript_SayHoundourType
	release
	end

PetalburgCity_PokemonCenter_1F_EventScript_SayPoliwagType::
	msgbox PetalburgCity_PokemonCenter_1F_Text_PoliwagIsWaterType, MSGBOX_DEFAULT
	return

PetalburgCity_PokemonCenter_1F_EventScript_SayLileepType::
	msgbox PetalburgCity_PokemonCenter_1F_Text_LileepIsRockGrassType, MSGBOX_DEFAULT
	return

PetalburgCity_PokemonCenter_1F_EventScript_SayHoundourType::
	msgbox PetalburgCity_PokemonCenter_1F_Text_HoundourIsDarkFireType, MSGBOX_DEFAULT
	return

PetalburgCity_PokemonCenter_1F_EventScript_Gentleman::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_STONER_INSURANCE_SCRAGGY, PetalburgCityPokemonCenter1F_EventScript_AlreadyCovered
	msgbox PetalburgCityPokemonCenter1F_Text_StonerPitch, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, PetalburgCityPokemonCenter1F_EventScript_NoDeal
	special CalculatePlayerPartyCount
	goto_if_eq VAR_RESULT, PARTY_SIZE, PetalburgCityPokemonCenter1F_EventScript_PartyFull
	special BaneModeGiveShinyScraggyInsurance
	setflag FLAG_RECEIVED_STONER_INSURANCE_SCRAGGY
	msgbox PetalburgCityPokemonCenter1F_Text_StonerSignedUp, MSGBOX_DEFAULT
	release
	end

PetalburgCityPokemonCenter1F_EventScript_PartyFull::
	msgbox PetalburgCityPokemonCenter1F_Text_StonerPartyFull, MSGBOX_DEFAULT
	release
	end

PetalburgCityPokemonCenter1F_EventScript_NoDeal::
	msgbox PetalburgCityPokemonCenter1F_Text_StonerNoDeal, MSGBOX_DEFAULT
	release
	end

PetalburgCityPokemonCenter1F_EventScript_AlreadyCovered::
	msgbox PetalburgCityPokemonCenter1F_Text_StonerAlreadySigned, MSGBOX_DEFAULT
	release
	end

PetalburgCity_PokemonCenter_1F_Text_PCStorageSystem:
	.string "That PC-based POKéMON Storage\n"
	.string "System…\p"
	.string "Whoever made it must be some kind\n"
	.string "of a scientific wizard!$"

PetalburgCity_PokemonCenter_1F_Text_OranBerryRegainedHP:
	.string "When my POKéMON ate an\n"
	.string "ORAN BERRY, it regained HP!$"

PetalburgCity_PokemonCenter_1F_Text_ManyTypesOfPokemon:
	.string "There are many types of POKéMON.\p"
	.string "All types have their strengths and\n"
	.string "weaknesses against other types.\p"
	.string "Depending on the types of POKéMON,\n"
	.string "a battle could be easy or hard.$"

PetalburgCity_PokemonCenter_1F_Text_PoliwagIsWaterType:
	.string "For example, your POLIWAG\n"
	.string "is a WATER type.\p"
	.string "It's strong against FIRE, GROUND,\n"
	.string "and ROCK types.\p"
	.string "But, it has to watch out for GRASS\n"
	.string "and ELECTRIC moves.$"

PetalburgCity_PokemonCenter_1F_Text_LileepIsRockGrassType:
	.string "For example, your LILEEP\n"
	.string "is both ROCK and GRASS type.\p"
	.string "It can check WATER and GROUND\n"
	.string "POKéMON pretty well.\p"
	.string "But, FIGHTING, BUG, and ICE moves\n"
	.string "can hit it hard.$"

PetalburgCity_PokemonCenter_1F_Text_HoundourIsDarkFireType:
	.string "For example, your HOUNDOUR\n"
	.string "is both DARK and FIRE type.\p"
	.string "It's nasty into GHOST, GRASS,\n"
	.string "and STEEL types.\p"
	.string "But, WATER, FIGHTING, GROUND,\n"
	.string "and ROCK are scary.$"

PetalburgCityPokemonCenter1F_Text_StonerPitch:
	.string "Yo, I'm STONER.\p"
	.string "I sell POKeMON insurance.\p"
	.string "Sign up today and I'll throw in a\n"
	.string "shiny SCRAGGY for free.\p"
	.string "You in?$"

PetalburgCityPokemonCenter1F_Text_StonerSignedUp:
	.string "Smart move.\p"
	.string "Your policy is live, and that shiny\n"
	.string "SCRAGGY is all yours.$"

PetalburgCityPokemonCenter1F_Text_StonerPartyFull:
	.string "Your party's packed.\p"
	.string "Make room, then come back for your\n"
	.string "insured shiny SCRAGGY.$"

PetalburgCityPokemonCenter1F_Text_StonerNoDeal:
	.string "No pressure.\p"
	.string "Insurance hits different when the\n"
	.string "crit animation starts.$"

PetalburgCityPokemonCenter1F_Text_StonerAlreadySigned:
	.string "You're already covered.\p"
	.string "That shiny SCRAGGY was part of the\n"
	.string "welcome package.$"
