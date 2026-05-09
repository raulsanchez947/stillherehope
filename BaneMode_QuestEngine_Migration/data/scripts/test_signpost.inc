Text_ThisIsATestSignpostMsg::
	.string "This is a test message.\n"
	.string "This is a signpost.$"

EventScript_TestSignpostMsg::
	msgbox Text_ThisIsATestSignpostMsg, MSGBOX_SIGN
	end

EventScript_FollowerMsg::
	faceplayer
	random 8
	goto_if_eq VAR_RESULT, 0, EventScript_FollowerMsg1
	goto_if_eq VAR_RESULT, 1, EventScript_FollowerMsg2
	goto_if_eq VAR_RESULT, 2, EventScript_FollowerMsg3
	goto_if_eq VAR_RESULT, 3, EventScript_FollowerMsg4
	goto_if_eq VAR_RESULT, 4, EventScript_FollowerMsg5
	goto_if_eq VAR_RESULT, 5, EventScript_FollowerMsg6
	goto_if_eq VAR_RESULT, 6, EventScript_FollowerMsg7
	goto_if_eq VAR_RESULT, 7, EventScript_FollowerMsg8
	end

EventScript_FollowerMsg1::
	msgbox Text_FollowerMsg, MSGBOX_AUTOCLOSE
	end

EventScript_FollowerMsg2::
	msgbox Text_FollowerMsg1, MSGBOX_AUTOCLOSE
	end

EventScript_FollowerMsg3::
	msgbox Text_FollowerMsg3, MSGBOX_AUTOCLOSE
	end

EventScript_FollowerMsg4::
	msgbox Text_FollowerMsg4, MSGBOX_AUTOCLOSE
	end

EventScript_FollowerMsg5::
	msgbox Text_FollowerMsg5, MSGBOX_AUTOCLOSE
	end

EventScript_FollowerMsg6::
	msgbox Text_FollowerMsg6, MSGBOX_AUTOCLOSE
	end

EventScript_FollowerMsg7::
	msgbox Text_FollowerMsg7, MSGBOX_AUTOCLOSE
	end

EventScript_FollowerMsg8::
	msgbox Text_FollowerMsg8, MSGBOX_AUTOCLOSE
	end

Text_FollowerMsg:
	.string "Your Pokémon is hot and cheerful!$"

Text_FollowerMsg1:
	.string "Your Pokémon seems somehow unhappy.$"

Text_FollowerMsg2:
	.string "Your Pokémon is loafing around.$"

Text_FollowerMsg3:
	.string "Your Pokémon is feeling alert because of\n"
	.string "something...$"

Text_FollowerMsg4:
	.string "Your Pokémon looks excited!$"

Text_FollowerMsg5:
	.string "Pokémon is staring at something.$"

Text_FollowerMsg6:
	.string "Pokémon is having fun rolling around!$"

Text_FollowerMsg7:
	.string "Pokémon is having fun swaying back and\n"
	.string "forth.$"

Text_FollowerMsg8:
	.string "Your Pokémon isn’t moving.$"
