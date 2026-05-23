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
	.string "NACHO: This whole hill is holy, bro.\n"
	.string "Rocket wanna touch it like they own it.$"

Route122_Text_NachoDefeat:
	.string "NACHO: Aight, you can keep up.\n"
	.string "Good looks.$"

Route122_Text_NachoPostBattle:
	.string "NACHO: Move quick, but don't move\n"
	.string "stupid. Rocket already up the block.$"

Route122_Text_DouglasIntro:
	.string "DOUGLAS: River quiet 'cause everybody\n"
	.string "too shook to say the real.$"

Route122_Text_DouglasDefeat:
	.string "DOUGLAS: Aight, you broke the hush.$"

Route122_Text_DouglasPostBattle:
	.string "DOUGLAS: Mew ain't loot, fam.\n"
	.string "Rocket don't get to own miracles.$"

Route122_Text_SpecialIntro:
	.string "SHAMUS: {PLAYER}, I'm sturdy.\n"
	.string "Just don't sell the play.$"

Route122_Text_SpecialDefeat:
	.string "SHAMUS: Then stay sharp.\n"
	.string "Don't slow me down.$"

Route122_Text_SpecialPostBattle:
	.string "SHAMUS: Keep it pushing.\n"
	.string "Mt. Pyre don't pause for nobody.$"
