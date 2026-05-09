Route122_MapScripts::
	.byte 0

Route122_EventScript_NachoMandatory::
	goto_if_defeated TRAINER_BANEMODE_ROUTE122_NACHO, Route122_EventScript_NachoMandatoryEnd
	setvar VAR_LAST_TALKED, 1
	goto Route122_EventScript_Nacho

Route122_EventScript_NachoMandatoryEnd::
	end

Route122_EventScript_DouglasMandatory::
	goto_if_defeated TRAINER_BANEMODE_ROUTE122_DOUGLAS, Route122_EventScript_DouglasMandatoryEnd
	setvar VAR_LAST_TALKED, 2
	goto Route122_EventScript_Douglas

Route122_EventScript_DouglasMandatoryEnd::
	end

Route122_EventScript_SpecialMandatory::
	goto_if_defeated TRAINER_BANEMODE_ROUTE122_SPECIAL, Route122_EventScript_SpecialMandatoryEnd
	setvar VAR_LAST_TALKED, 3
	goto Route122_EventScript_Special

Route122_EventScript_SpecialMandatoryEnd::
	end

Route122_EventScript_Nacho::
	trainerbattle_single TRAINER_BANEMODE_ROUTE122_NACHO, Route122_Text_NachoIntro, Route122_Text_NachoDefeat
	msgbox Route122_Text_NachoPostBattle, MSGBOX_AUTOCLOSE
	end

Route122_EventScript_Douglas::
	trainerbattle_single TRAINER_BANEMODE_ROUTE122_DOUGLAS, Route122_Text_DouglasIntro, Route122_Text_DouglasDefeat
	msgbox Route122_Text_DouglasPostBattle, MSGBOX_AUTOCLOSE
	end

Route122_EventScript_Special::
	trainerbattle_single TRAINER_BANEMODE_ROUTE122_SPECIAL, Route122_Text_SpecialIntro, Route122_Text_SpecialDefeat
	msgbox Route122_Text_SpecialPostBattle, MSGBOX_AUTOCLOSE
	end

Route122_Text_NachoIntro:
	.string "NACHO: Mt. Pyre is sacred ground.\n"
	.string "Rocket wants what sleeps here.$"

Route122_Text_NachoDefeat:
	.string "NACHO: You can keep pace.\n"
	.string "Good.$"

Route122_Text_NachoPostBattle:
	.string "NACHO: Move fast, but don't move\n"
	.string "careless. Rocket is already upstream.$"

Route122_Text_DouglasIntro:
	.string "DOUGLAS: The river is quiet because\n"
	.string "everyone is afraid to speak.$"

Route122_Text_DouglasDefeat:
	.string "DOUGLAS: You broke the silence.$"

Route122_Text_DouglasPostBattle:
	.string "DOUGLAS: Mew is not a prize.\n"
	.string "It is the thing Rocket cannot own.$"

Route122_Text_SpecialIntro:
	.string "SHAMUS: {PLAYER}, I can handle it.$"

Route122_Text_SpecialDefeat:
	.string "SHAMUS: Then don't slow me down.$"

Route122_Text_SpecialPostBattle:
	.string "SHAMUS: Keep moving.\n"
	.string "Mt. Pyre won't wait for either of us.$"
