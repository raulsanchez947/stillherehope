EventScript_MysteryGift::
    lockall
	faceplayer
	msgbox Text_Welcome, MSGBOX_AUTOCLOSE
	call Common_EventScript_SaveGame
	delay 20
	fadescreen FADE_TO_BLACK
	special DoGiftNamingScreen
	fadescreen FADE_FROM_BLACK
	special GetCodeInputData
	goto_if_eq VAR_UNUSED_0x8014, 0, EventScript_empty
	goto_if_eq VAR_UNUSED_0x8014, 1, EventScript_GiveMon_1
	goto_if_eq VAR_UNUSED_0x8014, 2, EventScript_GiveMon_2
	goto_if_eq VAR_UNUSED_0x8014, 3, EventScript_GiveMon_3
	goto_if_eq VAR_UNUSED_0x8014, 4, EventScript_GiveMon_4
	goto_if_eq VAR_UNUSED_0x8014, 5, EventScript_GiveMon_5
	goto_if_eq VAR_UNUSED_0x8014, 6, EventScript_GiveMon_6
	goto_if_eq VAR_UNUSED_0x8014, 7, EventScript_GiveMon_7
	goto_if_eq VAR_UNUSED_0x8014, 8, EventScript_GiveMon_8
	goto_if_eq VAR_UNUSED_0x8014, 9, EventScript_GiveMon_9
	goto_if_eq VAR_UNUSED_0x8014, 10, EventScript_GiveMon_10
	goto_if_eq VAR_UNUSED_0x8014, 11, EventScript_GiveMon_11
	goto_if_eq VAR_UNUSED_0x8014, 12, EventScript_GiveMon_12
	goto_if_eq VAR_UNUSED_0x8014, 13, EventScript_GiveMon_13
	goto_if_eq VAR_UNUSED_0x8014, 14, EventScript_GiveMon_14
	goto_if_eq VAR_UNUSED_0x8014, 15, EventScript_GiveMon_15
	goto_if_eq VAR_UNUSED_0x8014, 16, EventScript_GiveMon_16
	goto_if_eq VAR_UNUSED_0x8014, 17, EventScript_GiveMon_17
	goto_if_eq VAR_UNUSED_0x8014, 18, EventScript_GiveMon_18
	goto_if_eq VAR_UNUSED_0x8014, 19, EventScript_GiveMon_19
	goto_if_eq VAR_UNUSED_0x8014, 20, EventScript_GiveMon_20
	goto_if_eq VAR_UNUSED_0x8014, 21, EventScript_GiveMon_21
	goto_if_eq VAR_UNUSED_0x8014, 22, EventScript_GiveMon_22
	goto_if_eq VAR_UNUSED_0x8014, 23, EventScript_GiveMon_23
	goto_if_eq VAR_UNUSED_0x8014, 24, EventScript_GiveMon_24
	goto_if_eq VAR_UNUSED_0x8014, 25, EventScript_GiveMon_25
	goto_if_eq VAR_UNUSED_0x8014, 26, EventScript_GiveMon_26
	goto_if_eq VAR_UNUSED_0x8014, 27, EventScript_GiveMon_Special
	msgbox Text_PleaseEnterCode, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_Special::
	specialvar VAR_RESULT, BadgesObtained
	goto_if_ge VAR_RESULT, 6, EventScript_GiveMon_Special_1
	msgbox Text_Reqnotmet, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_Special_1::
	givemon VAR_TEMP_6, 40
	msgbox Text_GiveMon_Special, MSGBOX_AUTOCLOSE
	setvar VAR_UNUSED_0x8014, 0
	setvar VAR_TEMP_6, 0
	releaseall
	end

EventScript_empty::
	goto_if_set FLAG_VISITED_PETALBURG_CITY, EventScript_empty_1
	msgbox Text_PleaseEnterCode, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_empty_1::
	msgbox Text_PleaseEnterCode, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_1::
	goto_if_set FLAG_RECEIVED_MG_1, Text_RecivedOnce
	goto_if_set FLAG_VISITED_MOSSDEEP_CITY, EventScript_GiveMon_1_A
	msgbox Text_Reqnotmet, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_1_A::
	giveitem ITEM_POKE_BALL, 20
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_BagIsFull
	giveitem ITEM_POTION, 5
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_BagIsFull
	giveitem ITEM_FULL_HEAL, 4
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_BagIsFull
	setflag FLAG_SHINY_CREATION
	givemon SPECIES_MAGIKARP, 5
	setflag FLAG_RECEIVED_MG_1
	msgbox Text_Magikarp, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_2::
	goto_if_set FLAG_RECEIVED_MG_2, Text_RecivedOnce
	specialvar VAR_RESULT, BadgesObtained
	goto_if_ge VAR_RESULT, 7, EventScript_GiveMon_2_A
	msgbox Text_Reqnotmet, MSGBOX_AUTOCLOSE
	releaseall
	end 

EventScript_GiveMon_2_A::
	givemon SPECIES_VICTINI, 50
	playfanfare MUS_OBTAIN_ITEM
	setflag FLAG_RECEIVED_MG_2
	msgbox Text_Victini, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_3::
	goto_if_set FLAG_RECEIVED_MG_3, Text_RecivedOnce
	specialvar VAR_RESULT, BadgesObtained
	goto_if_ge VAR_RESULT, 1, EventScript_GiveMon_3_A
	msgbox Text_Reqnotmet, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_3_A::
	setflag FLAG_SHINY_CREATION
	givemon SPECIES_ROCKRUFF_OWN_TEMPO, 10
	setflag FLAG_RECEIVED_MG_3
	msgbox Text_Rockruff, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_4::
	goto_if_set FLAG_RECEIVED_MG_4, Text_RecivedOnce
	specialvar VAR_RESULT, BadgesObtained
	goto_if_ge VAR_RESULT, 6, EventScript_GiveMon_4_A
	msgbox Text_Reqnotmet, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_4_A::
	giveitem ITEM_ULTRA_BALL, 15
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_BagIsFull
	giveitem ITEM_RARE_CANDY, 5
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_BagIsFull
	givemon SPECIES_MARSHADOW, 40
	setflag FLAG_RECEIVED_MG_4
	msgbox Text_Marshadow, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_26::
	setvar VAR_ROUTE111_STATE, 4
	giveitem ITEM_MASTER_BALL, 99
	setvar VAR_ROUTE111_STATE, 0
	releaseall
	end

EventScript_GiveMon_5::
	goto_if_set FLAG_RECEIVED_MG_5, Text_RecivedOnce
	specialvar VAR_RESULT, BadgesObtained
	goto_if_ge VAR_RESULT, 6, EventScript_GiveMon_5_A
	msgbox Text_Reqnotmet, MSGBOX_AUTOCLOSE
	releaseall
	end
 
EventScript_GiveMon_5_A::
	givemon SPECIES_ABOMASNOW, 40, ITEM_ABOMASITE
	playfanfare MUS_OBTAIN_ITEM
	setflag FLAG_RECEIVED_MG_5
	msgbox Text_Abomasnow, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_6::
	goto_if_set FLAG_RECEIVED_MG_6, Text_RecivedOnce
	specialvar VAR_RESULT, BadgesObtained
	goto_if_ge VAR_RESULT, 3, EventScript_GiveMon_6_A
	msgbox Text_Reqnotmet, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_6_A::
	givemon SPECIES_BEEDRILL, 20, ITEM_BEEDRILLITE
	playfanfare MUS_OBTAIN_ITEM
	setflag FLAG_RECEIVED_MG_6
	msgbox Text_Beedrill, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_7::
	goto_if_set FLAG_RECEIVED_MG_7, Text_RecivedOnce
	goto_if_set FLAG_VISITED_PETALBURG_CITY, EventScript_GiveMon_7_A
	msgbox Text_Reqnotmet, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_7_A::
	setflag FLAG_SHINY_CREATION
	setflag FLAG_RECEIVED_MG_7
	givemon SPECIES_FALINKS, 10
	msgbox Text_Falinks, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_8::
	goto_if_set FLAG_RECEIVED_MG_8, Text_RecivedOnce
	goto_if_set FLAG_VISITED_PETALBURG_CITY, EventScript_GiveMon_8_A
	msgbox Text_Reqnotmet, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_8_A::
	setflag FLAG_SHINY_CREATION
	givemon SPECIES_SKIDDO, 10
	setflag FLAG_RECEIVED_MG_8
	msgbox Text_Skiddo, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_9::
	setvar VAR_ROUTE111_STATE, 4
	giveitem ITEM_RARE_CANDY, 99
	setvar VAR_ROUTE111_STATE, 0
	releaseall
	end

EventScript_GiveMon_10::
	goto_if_set FLAG_RECEIVED_MG_10, Text_RecivedOnce
	specialvar VAR_RESULT, BadgesObtained
	goto_if_ge VAR_RESULT, 6, EventScript_GiveMon_10_A
	msgbox Text_Reqnotmet, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_10_A::
	giveitem ITEM_MAX_REVIVE, 
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_BagIsFull
	givemon SPECIES_TAPU_BULU, 40
	setflag FLAG_RECEIVED_MG_10
	msgbox Text_TapuBulu, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_11::
	goto_if_set FLAG_RECEIVED_MG_11, Text_RecivedOnce
	specialvar VAR_RESULT, BadgesObtained
	goto_if_ge VAR_RESULT, 6, EventScript_GiveMon_11_A
	msgbox Text_Reqnotmet, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_11_A::
	giveitem ITEM_MAX_REVIVE, 
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_BagIsFull
	givemon SPECIES_TAPU_KOKO, 40
	setflag FLAG_RECEIVED_MG_11
	msgbox Text_TapuKoko, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_12::
	goto_if_set FLAG_RECEIVED_MG_12, Text_RecivedOnce
	specialvar VAR_RESULT, BadgesObtained
	goto_if_ge VAR_RESULT, 6, EventScript_GiveMon_12_A
	msgbox Text_Reqnotmet, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_12_A::
	giveitem ITEM_MAX_REVIVE, 
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_BagIsFull
	givemon SPECIES_TAPU_FINI, 40
	setflag FLAG_RECEIVED_MG_12
	msgbox Text_TapuFini, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_13::
	goto_if_set FLAG_RECEIVED_MG_13, Text_RecivedOnce
	specialvar VAR_RESULT, BadgesObtained
	goto_if_ge VAR_RESULT, 6, EventScript_GiveMon_13_A
	msgbox Text_Reqnotmet, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_13_A::
	giveitem ITEM_MAX_REVIVE, 
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_BagIsFull
	givemon SPECIES_TAPU_LELE, 40
	setflag FLAG_RECEIVED_MG_13
	msgbox Text_TapuLele, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_14::
	goto_if_set FLAG_RECEIVED_MG_14, Text_RecivedOnce
	goto_if_set FLAG_LATIOS_OR_LATIAS_ROAMING, EventScript_GiveMon_14_A
	msgbox Text_Reqnotmet, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_14_A::
	giveitem ITEM_EON_TICKET, 
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_BagIsFull
	setflag FLAG_RECEIVED_MG_14
    setflag FLAG_ENABLE_SHIP_SOUTHERN_ISLAND
	releaseall
	end

EventScript_GiveMon_15::
	goto_if_set FLAG_RECEIVED_MG_15, Text_RecivedOnce
    specialvar VAR_RESULT, BadgesObtained
	goto_if_ge VAR_RESULT, 8, EventScript_GiveMon_15_A
	msgbox Text_Reqnotmet, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_15_A::
	giveitem ITEM_MYSTIC_TICKET, 
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_BagIsFull
	setflag FLAG_RECEIVED_MG_15
	setflag FLAG_ENABLE_SHIP_NAVEL_ROCK
	releaseall
	end

EventScript_GiveMon_16::
	goto_if_set FLAG_RECEIVED_MG_16, Text_RecivedOnce
    specialvar VAR_RESULT, BadgesObtained
	goto_if_ge VAR_RESULT, 4, EventScript_GiveMon_16_A
	msgbox Text_Reqnotmet, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_16_A::
	givemon SPECIES_CELEBI, 30 
	setflag FLAG_RECEIVED_MG_16
	msgbox Text_Celebi, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_17::
	goto_if_set FLAG_RECEIVED_MG_17, Text_RecivedOnce
    specialvar VAR_RESULT, BadgesObtained
	goto_if_ge VAR_RESULT, 6, EventScript_GiveMon_17_A
	msgbox Text_Reqnotmet, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_17_A::
	givemon SPECIES_TYPE_NULL, 40, ITEM_ELECTRIC_MEMORY 
	setflag FLAG_RECEIVED_MG_17
	msgbox Text_TypeNull, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_18::
	goto_if_set FLAG_RECEIVED_MG_18, Text_RecivedOnce
    specialvar VAR_RESULT, BadgesObtained
	goto_if_ge VAR_RESULT, 2, EventScript_GiveMon_18_A
	msgbox Text_Reqnotmet, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_18_A::
	setflag FLAG_SHINY_CREATION	
	givemon SPECIES_SNEASLER, 20 
	setflag FLAG_RECEIVED_MG_18
	msgbox Text_Sneasler, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_19::
	goto_if_set FLAG_RECEIVED_MG_19, Text_RecivedOnce
    specialvar VAR_RESULT, BadgesObtained
	goto_if_ge VAR_RESULT, 6, EventScript_GiveMon_19_A
	msgbox Text_Reqnotmet, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_19_A::
	setflag FLAG_SHINY_CREATION	
	givemon SPECIES_RAIKOU, 40 
	setflag FLAG_RECEIVED_MG_19
	msgbox Text_Raikou, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_20::
	goto_if_set FLAG_RECEIVED_MG_20, Text_RecivedOnce
    specialvar VAR_RESULT, BadgesObtained
	goto_if_ge VAR_RESULT, 6, EventScript_GiveMon_20_A
	msgbox Text_Reqnotmet, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_20_A::
	setflag FLAG_SHINY_CREATION	
	givemon SPECIES_SANDY_SHOCKS, 40 
	setflag FLAG_RECEIVED_MG_20
	msgbox Text_SandyShocks, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_21::
	goto_if_set FLAG_RECEIVED_MG_21, Text_RecivedOnce
    specialvar VAR_RESULT, BadgesObtained
	goto_if_ge VAR_RESULT, 6, EventScript_GiveMon_21_A
	msgbox Text_Reqnotmet, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_21_A::
	setflag FLAG_SHINY_CREATION	
	givemon SPECIES_FLUTTER_MANE, 40 
	setflag FLAG_RECEIVED_MG_21
	msgbox Text_FlutterMane, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_22::
	goto_if_set FLAG_RECEIVED_MG_22, Text_RecivedOnce
    specialvar VAR_RESULT, BadgesObtained
	goto_if_ge VAR_RESULT, 6, EventScript_GiveMon_22_A
	msgbox Text_Reqnotmet, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_22_A::
	setflag FLAG_SHINY_CREATION	
	givemon SPECIES_SLITHER_WING, 40 
	setflag FLAG_RECEIVED_MG_22
	msgbox Text_SlitherWing, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_23::
	goto_if_set FLAG_RECEIVED_MG_23, Text_RecivedOnce
    specialvar VAR_RESULT, BadgesObtained
	goto_if_ge VAR_RESULT, 7, EventScript_GiveMon_23_A
	msgbox Text_Reqnotmet, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_23_A::
	setflag FLAG_SHINY_CREATION	
	givemon SPECIES_MEWTWO, 50 
	giveitem ITEM_MEWTWONITE_X
	giveitem ITEM_MEWTWONITE_Y
	setflag FLAG_RECEIVED_MG_23
	msgbox Text_Mewtwo, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_24::
	goto_if_set FLAG_RECEIVED_MG_24, Text_RecivedOnce
    specialvar VAR_RESULT, BadgesObtained
	goto_if_ge VAR_RESULT, 3, EventScript_GiveMon_24_A
	msgbox Text_Reqnotmet, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_24_A::
	setflag FLAG_SHINY_CREATION	
	givemon SPECIES_LOKIX, 20 
	giveitem ITEM_HYPER_POTION, 5
	giveitem ITEM_MASTER_BALL, 2
	setflag FLAG_RECEIVED_MG_24
	msgbox Text_Lokix, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_25::
	goto_if_set FLAG_RECEIVED_MG_25, Text_RecivedOnce
    specialvar VAR_RESULT, BadgesObtained
	goto_if_ge VAR_RESULT, 7, EventScript_GiveMon_25_A
	msgbox Text_Reqnotmet, MSGBOX_AUTOCLOSE
	releaseall
	end

EventScript_GiveMon_25_A::
	givemon SPECIES_BANETTE, 40 
	giveitem ITEM_BANETTITE
	setflag FLAG_RECEIVED_MG_25
	msgbox Text_Banette, MSGBOX_AUTOCLOSE
	releaseall
	end

Text_RecivedOnce::
 msgbox Text_RecivedOnce1, MSGBOX_AUTOCLOSE
 end

Text_Welcome:
.string "Hello, please fill out this form to claim\n"
.string "your MYSTERY GIFT!\p"
.string "Do you wish to save first?\n"
.string "It is {COLOR RED}RECOMMEND{COLOR DARK_GRAY} that you save here.$"

Text_PleaseEnterCode:
.string "CODE NOT VALID...\n"
.string "check discord server for the codes\p"
.string "Discord Server: discord.gg/7DvkRsxSnR$"

Text_Reqnotmet:
 .string "Sorry you don't meet the requirements.$"

Text_RecivedOnce1:
 .string "You have already claimed the gift!$"

Text_GiveMon_Special:
 .string "{PLAYER} recived a Mystery Gift!$"

Text_Magikarp:
 .string "{PLAYER} recived a Magikarp!$"

Text_Victini:
 .string "{PLAYER} recived a Victini!$"

Text_Rockruff:
 .string "{PLAYER} recived a Rockruff!$"

Text_Marshadow:
 .string "{PLAYER} recived a Marshadow!$"

Text_Abomasnow:
 .string "{PLAYER} recived a Abomasnow!$"

Text_Falinks:
 .string "{PLAYER} recived a Falinks!$"

Text_Beedrill:
 .string "{PLAYER} recived a Beedrill!$"

Text_Skiddo:
 .string "{PLAYER} recived a Skiddo!$"

Text_TapuBulu:
 .string "{PLAYER} recived a Tapu Bulu!$"

Text_TapuKoko:
 .string "{PLAYER} recived a Tapu Koko!$"

Text_TapuFini:
 .string "{PLAYER} recived a Tapu Fini!$"

Text_TapuLele:
 .string "{PLAYER} recived a Tapu Lele!$"

Text_Celebi:
 .string "{PLAYER} recived a Celebi!$"

Text_TypeNull:
 .string "{PLAYER} recived a Type: NULL!$"

Text_Sneasler:
 .string "{PLAYER} recived a Sneasler!$"

Text_Raikou:
 .string "{PLAYER} recived a Raikou!$" 

 Text_SlitherWing:
 .string "{PLAYER} recived a Slither Wing!$"

 Text_FlutterMane:
 .string "{PLAYER} recived a Text_FlutterMane!$"

 Text_SandyShocks:
 .string "{PLAYER} recived a Sandy Shocks!$"

 Text_Mewtwo:
 .string "{PLAYER} recived a Mew Two!$"

  Text_Lokix:
 .string "{PLAYER} recived a Lokix!$"

  Text_Banette:
 .string "{PLAYER} recived a Banette!$"
