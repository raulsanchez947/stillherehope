#ifndef GUARD_START_MENU_H
#define GUARD_START_MENU_H

//For multi start menus
//ROUND_UP_DIVISION_RESULT is used to appropriately get the value of NUM_OF_START_MENUS and round it up if needed
#define ROUND_UP_DIVISION_RESULT(number, divisor) (number % divisor == 0 ? number / divisor : (number + (divisor - (number % divisor))) / divisor)

#define NUM_OF_START_MENUS                ROUND_UP_DIVISION_RESULT(FINAL_START_MENU_OPTION, NUM_OF_START_MENU_OPTIONS)     //For more/less start menus, the macro "multistartmenu" sets this to 2 by default but can be specified
#define NUM_OF_START_MENU_OPTIONS         7                                                                                //Max number of options per menu. (Best to leave it as 8)
#define FINAL_START_MENU_OPTION           MENU_ACTION_EXIT                                                                 //Set this to the last startmenu option you want to print out
#define IS_MULTI_START_MENU_ENABLED       FlagGet(FLAG_IS_MULTI_START_MENU_ENABLED)                                        //The macro "multistartmenu" lets you turn this on or off if you want the multi menu
//#define FILL_STARTMENU_PAGE               TRUE                                                                              //Determines if the pages should be filled if possible or can be loaded with an incomplete amount of options

// For Start Menu Icons
#define ICON_WINDOW_OFFSET               2         // The extra space allocated to fit icons
#define TEXT_WINDOW_OFFSET               22        // The gap added to shift the text rightwards for the icons
#define COLOR_ICON_OFFSET                11        // In "start_menu_icons.png", its the number of 32x32 blocks till the colored likewise sprite


extern bool8 (*gMenuCallback)(void);


void ShowReturnToFieldStartMenu(void);
void Task_ShowStartMenu(u8 taskId);
void ShowStartMenu(void);
void ShowBattlePyramidStartMenu(void);
void SaveGame(void);
void CB2_SetUpSaveAfterLinkBattle(void);
void SaveForBattleTowerLink(void);
void HideStartMenu(void);
void AppendToList(u8 *list, u8 *pos, u8 newEntry);

// For Start Menu Icons
extern bool8 gShouldStartMenuIconsBePrinted;        // TRUE when the start menu is open. FALSE when it's closed
bool8 IsAStartMenuIconAtPosition(u8 position);      // Takes an index in the startmenu and checks if an icon has been drawn at that spot
extern u8 gStartMenuIconPaletteNum;                 // Stores the palette number the start menu icons use

#endif // GUARD_START_MENU_H
