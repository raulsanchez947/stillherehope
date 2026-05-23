MagmaHideout_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, MagmaHideout_1F_OnTransition
	.byte 0

MagmaHideout_1F_OnTransition:
	setvar VAR_JAGGED_PASS_ASH_WEATHER, 0
	end

MagmaHideout_1F_EventScript_Grunt1::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_1, MagmaHideout_1F_Text_Grunt1Intro, MagmaHideout_1F_Text_Grunt1Defeat
	msgbox MagmaHideout_1F_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_1F_EventScript_Grunt2::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_2, MagmaHideout_1F_Text_Grunt2Intro, MagmaHideout_1F_Text_Grunt2Defeat
	msgbox MagmaHideout_1F_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_1F_Text_Grunt1Intro:
	.string "STEVEN put us on hideout sweep detail,\n"
	.string "and I still drew the dusty corner.\p"
	.string "Whole Hoenn Defenders unit moving and\n"
	.string "I'm the one catching cave ash on my\l"
	.string "Timbs. Unreal.$"

MagmaHideout_1F_Text_Grunt1Defeat:
	.string "Nah, I got cooked.\p"
	.string "Tell STEVEN I at least made contact.$"

MagmaHideout_1F_Text_Grunt1PostBattle:
	.string "Okay, okay. I'm tightening up the\n"
	.string "rotation before the next Rocket push.$"

MagmaHideout_1F_Text_Grunt2Intro:
	.string "We're not here to crack the mountain\n"
	.string "open, we're here to keep Rocket from\l"
	.string "doing exactly that.\p"
	.string "STEVEN said nobody touches the deep\n"
	.string "find till the whole defender line is\l"
	.string "set. So if you want through, earn it.$"

MagmaHideout_1F_Text_Grunt2Defeat:
	.string "Arrgh!\n"
	.string "Taken down!$"

MagmaHideout_1F_Text_Grunt2PostBattle:
	.string "Keep moving then.\n"
	.string "The real story's deeper in, and the\l"
	.string "Hoenn Defenders are already on it.$"
