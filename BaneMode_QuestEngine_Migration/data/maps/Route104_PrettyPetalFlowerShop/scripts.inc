.set LOCALID_OWNER, 1

Route104_PrettyPetalFlowerShop_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route104_PrettyPetalFlowerShop_OnTransition
	.byte 0

Route104_PrettyPetalFlowerShop_OnTransition:
	setflag FLAG_LANDMARK_FLOWER_SHOP
	goto_if_unset FLAG_MET_PRETTY_PETAL_SHOP_OWNER, Route104_PrettyPetalFlowerShop_EventScript_MoveShopOwner
	goto_if_unset FLAG_BADGE03_GET, Route104_PrettyPetalFlowerShop_EventScript_MoveShopOwner
	setflag FLAG_TEMP_1
	end

Route104_PrettyPetalFlowerShop_EventScript_MoveShopOwner::
	setobjectxyperm LOCALID_OWNER, 4, 6
	end

Route104_PrettyPetalFlowerShop_EventScript_ShopOwner::
	lock
	faceplayer
	goto_if_set FLAG_TEMP_1, Route104_PrettyPetalFlowerShop_EventScript_SellDecorations
	msgbox Route104_PrettyPetalFlowerShop_Text_ThisIsPrettyPetalFlowerShop, MSGBOX_DEFAULT
	goto_if_set FLAG_MET_PRETTY_PETAL_SHOP_OWNER, Route104_PrettyPetalFlowerShop_EventScript_AlreadyMet
	setflag FLAG_MET_PRETTY_PETAL_SHOP_OWNER
	msgbox Route104_PrettyPetalFlowerShop_Text_IntroLearnAboutBerries, MSGBOX_YESNO
	call_if_eq VAR_RESULT, YES, Route104_PrettyPetalFlowerShop_EventScript_ExplainBerries
	call_if_eq VAR_RESULT, NO, Route104_PrettyPetalFlowerShop_EventScript_DontExplainBerries
	release
	end

Route104_PrettyPetalFlowerShop_EventScript_AlreadyMet::
	msgbox Route104_PrettyPetalFlowerShop_Text_LearnAboutBerries, MSGBOX_YESNO
	call_if_eq VAR_RESULT, YES, Route104_PrettyPetalFlowerShop_EventScript_ExplainBerries
	call_if_eq VAR_RESULT, NO, Route104_PrettyPetalFlowerShop_EventScript_DontExplainBerries
	release
	end

Route104_PrettyPetalFlowerShop_EventScript_ExplainBerries::
	msgbox Route104_PrettyPetalFlowerShop_Text_BerriesExplanation, MSGBOX_DEFAULT
	return

Route104_PrettyPetalFlowerShop_EventScript_DontExplainBerries::
	msgbox Route104_PrettyPetalFlowerShop_Text_FlowersBringHappiness, MSGBOX_DEFAULT
	return

Route104_PrettyPetalFlowerShop_EventScript_SellDecorations::
	message gText_PlayerWhatCanIDoForYou
	waitmessage
	pokemartdecoration2 Route104_PrettyPetalFlowerShop_Pokemart_Plants
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
Route104_PrettyPetalFlowerShop_Pokemart_Plants:
	.2byte DECOR_RED_PLANT
	.2byte DECOR_TROPICAL_PLANT
	.2byte DECOR_PRETTY_FLOWERS
	.2byte DECOR_COLORFUL_PLANT
	.2byte DECOR_BIG_PLANT
	.2byte DECOR_GORGEOUS_PLANT
	.2byte DECOR_NONE
	release
	end

Route104_PrettyPetalFlowerShop_EventScript_WailmerPailGirl::
	lock
	faceplayer
	goto_if_unset FLAG_RECEIVED_WAILMER_PAIL, Route104_PrettyPetalFlowerShop_EventScript_GiveWailmerPail
	msgbox Route104_PrettyPetalFlowerShop_Text_WailmerPailExplanation, MSGBOX_DEFAULT
	release
	end

Route104_PrettyPetalFlowerShop_EventScript_GiveWailmerPail::
	msgbox Route104_PrettyPetalFlowerShop_Text_YouCanHaveThis, MSGBOX_DEFAULT
	giveitem ITEM_WAILMER_PAIL
	msgbox Route104_PrettyPetalFlowerShop_Text_WailmerPailExplanation, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_WAILMER_PAIL
	release
	end

Route104_PrettyPetalFlowerShop_EventScript_RandomBerryGirl::
	lock
	faceplayer
	dotimebasedevents
	goto_if_set FLAG_DAILY_FLOWER_SHOP_RECEIVED_BERRY, Route104_PrettyPetalFlowerShop_EventScript_AlreadyReceivedBerry
	msgbox Route104_PrettyPetalFlowerShop_Text_ImGrowingFlowers, MSGBOX_DEFAULT
	random 8
	addvar VAR_RESULT, FIRST_BERRY_INDEX
	giveitem VAR_RESULT
	goto_if_eq VAR_RESULT, 0, Common_EventScript_ShowBagIsFull
	setflag FLAG_DAILY_FLOWER_SHOP_RECEIVED_BERRY
	msgbox Route104_PrettyPetalFlowerShop_Text_MachineMixesBerries, MSGBOX_DEFAULT
	release
	end

Route104_PrettyPetalFlowerShop_EventScript_AlreadyReceivedBerry::
	msgbox Route104_PrettyPetalFlowerShop_Text_MachineMixesBerries, MSGBOX_DEFAULT
	release
	end

EventScript_Quest2::
	lockall
	faceplayer
	goto_if_quest_active QUEST_2, Eventsctipt_Checksq2
	msgbox Text_MySkittyIsSick, MSGBOX_DEFAULT
	msgbox Text_willyouhelpme, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Eventscript_Startquest2
	msgbox Text_ISeeHowItIs, MSGBOX_DEFAULT
	releaseall
	end

Eventsctipt_Checksq2::
	faceplayer
	checkitem ITEM_LUM_BERRY
    goto_if_eq VAR_RESULT, TRUE, Eventsctipt_Rewardq2
	msgbox Text_Incorrectq2, MSGBOX_DEFAULT
	releaseall
	end

Eventsctipt_Rewardq2::
    goto_if_quest_complete QUEST_2, EventScript_QuestComplete2
    msgbox Text_YouGotTheBerry, MSGBOX_DEFAULT
	setvar VAR_0x8004, ITEM_LUM_BERRY
	call Common_EventScript_PlayerHandedOverTheItem
	completequest QUEST_2
    msgbox Text_Thxq2, MSGBOX_DEFAULT
	giveitem ITEM_ASSAULT_VEST
	setflag FLAG_GAVE_LUM_BERRY
	addrankpoints 10
	releaseall
	end

EventScript_QuestComplete2::
	msgbox Text_canbecured, MSGBOX_DEFAULT
	releaseall
	end

Eventscript_Startquest2::
	startquest QUEST_2
	openquestmenu
	releaseall
	end

Text_MySkittyIsSick::
    .string "I am looking for some Lum berry.\n"
	.string "It's for my little SKITTY she needs it.\p"
	.string "A merchent from Rustboro city told me he\n"
	.string "could get some, but his offers were way\l"
	.string "to expensive.$"

Text_willyouhelpme:
    .string "You love your POKéMON right?\n"
	.string "will you get those berries for my\l"
	.string "little SKITTY?$"

Text_ISeeHowItIs:
	.string "Oh.. okay, that's okay...$"

Text_Thxq2:
    .string "Thank you very much, you're a kind soul.\n"
	.string "This is not much but this all I have\l"
	.string "right now, here take it.$"

Text_Incorrectq2:
	.string "The store doesn't have them in stock.$"

Text_YouGotTheBerry:
	.string "Is that a Lum Berry?, you found one\n"
	.string "thank you!$"

Text_canbecured:
    .string "I hope this berry will cure my SKITTY.$"
