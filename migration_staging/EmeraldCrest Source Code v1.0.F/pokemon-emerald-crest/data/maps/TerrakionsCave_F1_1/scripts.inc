TerrakionsCave_F1_1_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, TerrakionsCave_F1_1_OnLoad
	.byte 0

TerrakionsCave_F1_1_OnLoad:
	call_if_unset FLAG_PUZZLE_1_SOLVED, EventScript_HideEntranceMiddle
	call_if_unset FLAG_PUZZLE_2_SOLVED, EventScript_HideEntranceLeft
	call_if_unset FLAG_PUZZLE_3_SOLVED, EventScript_HideEntranceRight
	end

EventScript_HideEntranceLeft::
	setmetatile 9, 2, METATILE_Cave_EntranceCover, TRUE
	setmetatile 10, 2, METATILE_Cave_EntranceCover, TRUE
	setmetatile 11, 2, METATILE_Cave_EntranceCover, TRUE
	setmetatile 9, 3, METATILE_Cave_SealedChamberBraille_Mid, TRUE
	setmetatile 10, 3, METATILE_Cave_SealedChamberBraille_Mid, TRUE
	setmetatile 11, 3, METATILE_Cave_SealedChamberBraille_Mid, TRUE
	return

EventScript_HideEntranceMiddle::
	setmetatile 16, 2, METATILE_Cave_EntranceCover, TRUE
	setmetatile 17, 2, METATILE_Cave_EntranceCover, TRUE
	setmetatile 18, 2, METATILE_Cave_EntranceCover, TRUE
	setmetatile 16, 3, METATILE_Cave_SealedChamberBraille_Mid, TRUE
	setmetatile 17, 3, METATILE_Cave_SealedChamberBraille_Mid, TRUE
	setmetatile 18, 3, METATILE_Cave_SealedChamberBraille_Mid, TRUE
	return

EventScript_HideEntranceRight::
	setmetatile 23, 2, METATILE_Cave_EntranceCover, TRUE
	setmetatile 24, 2, METATILE_Cave_EntranceCover, TRUE
	setmetatile 25, 2, METATILE_Cave_EntranceCover, TRUE
	setmetatile 23, 3, METATILE_Cave_SealedChamberBraille_Mid, TRUE
	setmetatile 24, 3, METATILE_Cave_SealedChamberBraille_Mid, TRUE
	setmetatile 25, 3, METATILE_Cave_SealedChamberBraille_Mid, TRUE
	return

EventScript_PuzzleMiddle::
	lock
	setvar VAR_0x8004, SLIDING_PUZZLE_OMANYTE
	setvar VAR_0x8005, FALSE
	special DoSlidingPuzzle
	waitstate
	goto_if_eq VAR_RESULT, TRUE, EventScript_PuzzleSolvedMiddle
	release
	end

EventScript_PuzzleSolvedMiddle::
	setmetatile 16, 2, METATILE_Cave_SealedChamberEntrance_TopLeft, TRUE
	setmetatile 17, 2, METATILE_Cave_SealedChamberEntrance_TopMid, TRUE
	setmetatile 18, 2, METATILE_Cave_SealedChamberEntrance_TopRight, TRUE
	setmetatile 16, 3, METATILE_Cave_SealedChamberEntrance_BottomLeft, TRUE
	setmetatile 17, 3, METATILE_Cave_SealedChamberEntrance_BottomMid, FALSE
	setmetatile 18, 3, METATILE_Cave_SealedChamberEntrance_BottomRight, TRUE
	special DrawWholeMapView
	playse SE_BANG
	setflag FLAG_PUZZLE_1_SOLVED
	release
	end

EventScript_PuzzleLeft::
	lock
	setvar VAR_0x8004, SLIDING_PUZZLE_HO_OH
	setvar VAR_0x8005, FALSE
	special DoSlidingPuzzle
	waitstate
	goto_if_eq VAR_RESULT, TRUE, EventScript_PuzzleSolvedLeft
	release
	end

EventScript_PuzzleSolvedLeft::
	setmetatile 9, 2, METATILE_Cave_SealedChamberEntrance_TopLeft, TRUE
	setmetatile 10, 2, METATILE_Cave_SealedChamberEntrance_TopMid, TRUE
	setmetatile 11, 2, METATILE_Cave_SealedChamberEntrance_TopRight, TRUE
	setmetatile 9, 3, METATILE_Cave_SealedChamberEntrance_BottomLeft, TRUE
	setmetatile 10, 3, METATILE_Cave_SealedChamberEntrance_BottomMid, FALSE
	setmetatile 11, 3, METATILE_Cave_SealedChamberEntrance_BottomRight, TRUE
	special DrawWholeMapView
	playse SE_BANG
	setflag FLAG_PUZZLE_2_SOLVED
	release
	end

EventScript_PuzzleRight::
	lock
	setvar VAR_0x8004, SLIDING_PUZZLE_AERODACTYL
	setvar VAR_0x8005, FALSE
	special DoSlidingPuzzle
	waitstate
	goto_if_eq VAR_RESULT, TRUE, EventScript_PuzzleSolvedRight
	release
	end

EventScript_PuzzleSolvedRight::
	setmetatile 23, 2, METATILE_Cave_SealedChamberEntrance_TopLeft, TRUE
	setmetatile 24, 2, METATILE_Cave_SealedChamberEntrance_TopMid, TRUE
	setmetatile 25, 2, METATILE_Cave_SealedChamberEntrance_TopRight, TRUE
	setmetatile 23, 3, METATILE_Cave_SealedChamberEntrance_BottomLeft, TRUE
	setmetatile 24, 3, METATILE_Cave_SealedChamberEntrance_BottomMid, FALSE
	setmetatile 25, 3, METATILE_Cave_SealedChamberEntrance_BottomRight, TRUE
	special DrawWholeMapView
	playse SE_BANG
	setflag FLAG_PUZZLE_3_SOLVED
	release
	end
