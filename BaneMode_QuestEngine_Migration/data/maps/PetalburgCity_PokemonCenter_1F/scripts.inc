
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
	goto_if_set FLAG_USED_HOLY_BLUNT_RECENTLY, PetalburgCity_PokemonCenter_1F_EventScript_HolyBluntReactionFatMan
	msgbox PetalburgCity_PokemonCenter_1F_Text_PCStorageSystem, MSGBOX_NPC
	end

PetalburgCity_PokemonCenter_1F_EventScript_Youngster::
	goto_if_set FLAG_USED_HOLY_BLUNT_RECENTLY, PetalburgCity_PokemonCenter_1F_EventScript_HolyBluntReactionYoungster
	msgbox PetalburgCity_PokemonCenter_1F_Text_OranBerryRegainedHP, MSGBOX_NPC
	end

PetalburgCity_PokemonCenter_1F_EventScript_Joe::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_JOE_ROD_PACKAGE, PetalburgCityPokemonCenter1F_EventScript_JoeAlreadyHookedUp
	msgbox PetalburgCityPokemonCenter1F_Text_JoePitch, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, PetalburgCityPokemonCenter1F_EventScript_JoeNoDeal
	special CalculatePlayerPartyCount
	goto_if_eq VAR_RESULT, PARTY_SIZE, PetalburgCityPokemonCenter1F_EventScript_JoePartyFull
	giveitem ITEM_OLD_ROD
	giveitem ITEM_GOOD_ROD
	giveitem ITEM_SUPER_ROD
	special BaneModeGiveJoeMagikarp
	setflag FLAG_RECEIVED_JOE_ROD_PACKAGE
	msgbox PetalburgCityPokemonCenter1F_Text_JoeSignedUp, MSGBOX_DEFAULT
	release
	end

PetalburgCityPokemonCenter1F_EventScript_JoePartyFull::
	msgbox PetalburgCityPokemonCenter1F_Text_JoePartyFull, MSGBOX_DEFAULT
	release
	end

PetalburgCityPokemonCenter1F_EventScript_JoeNoDeal::
	msgbox PetalburgCityPokemonCenter1F_Text_JoeNoDeal, MSGBOX_DEFAULT
	release
	end

PetalburgCityPokemonCenter1F_EventScript_JoeAlreadyHookedUp::
	msgbox PetalburgCityPokemonCenter1F_Text_JoeAlreadySigned, MSGBOX_DEFAULT
	release
	end

PetalburgCity_PokemonCenter_1F_EventScript_Woman::
	lock
	faceplayer
	goto_if_set FLAG_USED_HOLY_BLUNT_RECENTLY, PetalburgCity_PokemonCenter_1F_EventScript_HolyBluntReactionWoman
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

PetalburgCity_PokemonCenter_1F_EventScript_HolyBluntReactionFatMan::
	msgbox PetalburgCity_PokemonCenter_1F_Text_HolyBluntReactionFatMan, MSGBOX_NPC
	end

PetalburgCity_PokemonCenter_1F_EventScript_HolyBluntReactionYoungster::
	msgbox PetalburgCity_PokemonCenter_1F_Text_HolyBluntReactionYoungster, MSGBOX_NPC
	end

PetalburgCity_PokemonCenter_1F_EventScript_HolyBluntReactionWoman::
	msgbox PetalburgCity_PokemonCenter_1F_Text_HolyBluntReactionWoman, MSGBOX_DEFAULT
	clearflag FLAG_USED_HOLY_BLUNT_RECENTLY
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

PetalburgCity_PokemonCenter_1F_Text_HolyBluntReactionFatMan:
	.string "Whoa, pal… Your POKeMON went from\n"
	.string "half-faded to glowing.\p"
	.string "That purple haze was not standard\n"
	.string "healing tech, no shot.$"

PetalburgCity_PokemonCenter_1F_Text_HolyBluntReactionYoungster:
	.string "Yo! Your POKeMON looks mad fresh\n"
	.string "all of a sudden!\p"
	.string "Whatever that blunt thing was,\n"
	.string "it had your whole team sparkling.$"

PetalburgCity_PokemonCenter_1F_Text_HolyBluntReactionWoman:
	.string "I saw that purple aura around your\n"
	.string "POKeMON just now.\p"
	.string "It looks way healthier, like every\n"
	.string "last bit of energy came back.$"

PetalburgCityPokemonCenter1F_Text_StonerPitch:
	.string "Yo, I'm STONER, feel me?\p"
	.string "I move POKeMON insurance for trainers\n"
	.string "that don't like getting lined.\p"
	.string "Sign up today and I'll throw in a\n"
	.string "shiny SCRAFTY for free, easy.\p"
	.string "You in?$"

PetalburgCityPokemonCenter1F_Text_StonerSignedUp:
	.string "Smart move, my guy.\p"
	.string "Your policy is live, and that shiny\n"
	.string "SCRAFTY is all yours.$"

PetalburgCityPokemonCenter1F_Text_StonerPartyFull:
	.string "Your party's packed right now.\p"
	.string "Make room, then come back for your\n"
	.string "insured shiny SCRAFTY.$"

PetalburgCityPokemonCenter1F_Text_StonerNoDeal:
	.string "No pressure, fam.\p"
	.string "Insurance hit different when the\n"
	.string "crit animation get disrespectful.$"

PetalburgCityPokemonCenter1F_Text_StonerAlreadySigned:
	.string "You already covered, bro.\p"
	.string "That shiny SCRAGGY was part of the\n"
	.string "welcome package.$"

PetalburgCityPokemonCenter1F_Text_JoePitch:
	.string "JOE: Ayo, you fish at all?\p"
	.string "I'm Joe from out Queens, and I got\n"
	.string "the full rod pack on me right now.\p"
	.string "Old Rod, Good Rod, Super Rod, plus a\n"
	.string "shiny hidden-ability MAGIKARP.\p"
	.string "You want the starter tackle box?$"

PetalburgCityPokemonCenter1F_Text_JoeSignedUp:
	.string "JOE: Say less.\p"
	.string "You got all three rods, and that shiny\n"
	.string "MAGIKARP is ready to flop with purpose.$"

PetalburgCityPokemonCenter1F_Text_JoePartyFull:
	.string "JOE: Your party jammed up.\p"
	.string "Slide one mon out and I'll hand over\n"
	.string "the shiny MAGIKARP package.$"

PetalburgCityPokemonCenter1F_Text_JoeNoDeal:
	.string "JOE: Copy that.\p"
	.string "Come find me when you're ready to\n"
	.string "fish like somebody from the boroughs.$"

PetalburgCityPokemonCenter1F_Text_JoeAlreadySigned:
	.string "JOE: I already blessed you, fam.\p"
	.string "You got the rods and the shiny hidden-\l"
	.string "ability MAGIKARP already.$"
