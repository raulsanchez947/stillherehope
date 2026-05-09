#include "global.h"
#include "battle_setup.h"
#include "new_game.h"
#include "random.h"
#include "pokemon.h"
#include "roamer.h"
#include "pokemon_size_record.h"
#include "script.h"
#include "lottery_corner.h"
#include "play_time.h"
#include "mauville_old_man.h"
#include "match_call.h"
#include "lilycove_lady.h"
#include "load_save.h"
#include "pokeblock.h"
#include "dewford_trend.h"
#include "berry.h"
#include "rtc.h"
#include "easy_chat.h"
#include "event_data.h"
#include "event_scripts.h"
#include "money.h"
#include "trainer_hill.h"
#include "tv.h"
#include "coins.h"
#include "text.h"
#include "overworld.h"
#include "mail.h"
#include "battle_records.h"
#include "item.h"
#include "pokedex.h"
#include "apprentice.h"
#include "frontier_util.h"
#include "pokedex.h"
#include "save.h"
#include "specials.h"
#include "link_rfu.h"
#include "main.h"
#include "contest.h"
#include "item_menu.h"
#include "pokemon_storage_system.h"
#include "pokemon_jump.h"
#include "decoration_inventory.h"
#include "secret_base.h"
#include "player_pc.h"
#include "field_specials.h"
#include "berry_powder.h"
#include "mystery_gift.h"
#include "union_room_chat.h"
#include "constants/items.h"
#include "quests.h"
#include "tx_randomizer_and_challenges.h"

extern const u8 EventScript_ResetAllMapFlags[];

static void ClearFrontierRecord(void);
static void WarpToTruck(void);
static void ResetMiniGamesRecords(void);

EWRAM_DATA bool8 gDifferentSaveFile = FALSE;
EWRAM_DATA bool8 gEnableContestDebugging = FALSE;

static const struct ContestWinner sContestWinnerPicDummy =
{
    .monName = _(""),
    .trainerName = _("")
};

void SetTrainerId(u32 trainerId, u8 *dst)
{
    dst[0] = trainerId;
    dst[1] = trainerId >> 8;
    dst[2] = trainerId >> 16;
    dst[3] = trainerId >> 24;
}

u32 GetTrainerId(u8 *trainerId)
{
    return (trainerId[3] << 24) | (trainerId[2] << 16) | (trainerId[1] << 8) | (trainerId[0]);
}

void CopyTrainerId(u8 *dst, u8 *src)
{
    s32 i;
    for (i = 0; i < TRAINER_ID_LENGTH; i++)
        dst[i] = src[i];
}

static void InitPlayerTrainerId(void)
{
    u32 trainerId = (Random() << 16) | GetGeneratedTrainerIdLower();
    SetTrainerId(trainerId, gSaveBlock2Ptr->playerTrainerId);
}

// L=A isnt set here for some reason.
static void SetDefaultOptions(void)
{
    gSaveBlock2Ptr->optionsTextSpeed = OPTIONS_TEXT_SPEED_FASTER;
    gSaveBlock2Ptr->optionsWindowFrameType = 23;
    gSaveBlock2Ptr->optionsSound = OPTIONS_SOUND_MONO;
    gSaveBlock2Ptr->optionsBattleStyle = OPTIONS_BATTLE_STYLE_SHIFT;
    gSaveBlock2Ptr->optionsBattleSceneOff = FALSE;
    gSaveBlock2Ptr->regionMapZoom = FALSE;
    gSaveBlock2Ptr->optionsUnitSystem = 0;  //optionsPlus
    gSaveBlock2Ptr->optionsHpBarSpeed = 0;  //optionsPlus
    gSaveBlock2Ptr->optionsExpBarSpeed = 0; //optionsPlus
    gSaveBlock2Ptr->optionsDisableMatchCall = 0;    //optionsPlus
    gSaveBlock2Ptr->optionsCurrentFont = 0;         //optionsPlus
    gSaveBlock1Ptr->tx_Challenges_MegaEvo = 0;
}

static void ClearPokedexFlags(void)
{
    gUnusedPokedexU8 = 0;
    memset(&gSaveBlock1Ptr->dexCaught, 0, sizeof(gSaveBlock1Ptr->dexCaught));
    memset(&gSaveBlock1Ptr->dexSeen, 0, sizeof(gSaveBlock1Ptr->dexSeen));
}

void ClearAllContestWinnerPics(void)
{
    s32 i;

    ClearContestWinnerPicsInContestHall();

    // Clear Museum paintings
    for (i = MUSEUM_CONTEST_WINNERS_START; i < NUM_CONTEST_WINNERS; i++)
        gSaveBlock1Ptr->contestWinners[i] = sContestWinnerPicDummy;
}

static void ClearFrontierRecord(void)
{
    CpuFill32(0, &gSaveBlock2Ptr->frontier, sizeof(gSaveBlock2Ptr->frontier));

    gSaveBlock2Ptr->frontier.opponentNames[0][0] = EOS;
    gSaveBlock2Ptr->frontier.opponentNames[1][0] = EOS;
}

static void WarpToTruck(void)
{
    SaveData_TxRandomizerAndChallenges();
    CheckGameDifficulty();
    SetPlayerRank();
    
    if(gSaveBlock1Ptr->tx_Random_TypeEffectiveness == TRUE)
      FlagSet(FLAG_TYPE_INVERSE_ON);

    if (gSaveBlock1Ptr->tx_Challenges_PerfectIVs == TRUE)
    {
      FlagSet(FLAG_PERFECT_IVS);
    }

    if(gSaveBlock1Ptr->tx_Challenges_OpenWorld == TRUE)
    {
      FlagSet(FLAG_OPEN_WORLD);
      if (gSaveBlock1Ptr->tx_Challenges_StartLoc == START_LOC_LITTLEROOT)
      {
        FlagSet(FLAG_START_LOC_LITTLEROOT); 
      }
      else if(gSaveBlock1Ptr->tx_Challenges_StartLoc == START_LOC_OLDALE)
      {
        FlagSet(FLAG_START_LOC_OLDALE); 
      }
      else if(gSaveBlock1Ptr->tx_Challenges_StartLoc == START_LOC_PETALBURG)
      {
        FlagSet(FLAG_START_LOC_PETALBURG); 
      }
      else if(gSaveBlock1Ptr->tx_Challenges_StartLoc == START_LOC_RUSTBORO)
      {
        FlagSet(FLAG_START_LOC_RUSTBORO); 
      }
      else if(gSaveBlock1Ptr->tx_Challenges_StartLoc == START_LOC_DEWFORD)
      {
        FlagSet(FLAG_START_LOC_DEWFORD); 
      }
      else if(gSaveBlock1Ptr->tx_Challenges_StartLoc == START_LOC_SLATEPORT)
      {
        FlagSet(FLAG_START_LOC_SLATEPORT); 
      }
      else if(gSaveBlock1Ptr->tx_Challenges_StartLoc == START_LOC_MAUVILLE)
      {
        FlagSet(FLAG_START_LOC_MAUVILLE); 
      }
      else if(gSaveBlock1Ptr->tx_Challenges_StartLoc == START_LOC_VERDANTURF)
      {
        FlagSet(FLAG_START_LOC_VERDANTURF); 
      }
      else if(gSaveBlock1Ptr->tx_Challenges_StartLoc == START_LOC_LAVARIDGE)
      {
        FlagSet(FLAG_START_LOC_LAVARIDGE); 
      }
      else if(gSaveBlock1Ptr->tx_Challenges_StartLoc == START_LOC_FALLARBOR)
      {
        FlagSet(FLAG_START_LOC_FALLARBOR); 
      }
      else if(gSaveBlock1Ptr->tx_Challenges_StartLoc == START_LOC_FORTREE)
      {
        FlagSet(FLAG_START_LOC_FORTREE); 
      }
      else if(gSaveBlock1Ptr->tx_Challenges_StartLoc == START_LOC_LILYCOVE)
      {
        FlagSet(FLAG_START_LOC_LILYCOVE); 
      }
      else if(gSaveBlock1Ptr->tx_Challenges_StartLoc == START_LOC_MOSSDEEP)
      {
        FlagSet(FLAG_START_LOC_MOSSDEEP); 
      }
      else if(gSaveBlock1Ptr->tx_Challenges_StartLoc == START_LOC_SOOTOPOLIS)
      {
        FlagSet(FLAG_START_LOC_SOOTOPOLIS); 
      }
      else if(gSaveBlock1Ptr->tx_Challenges_StartLoc == START_LOC_PACIFIDLOG)
      {
        FlagSet(FLAG_START_LOC_PACIFIDLOG); 
      }
      else if(gSaveBlock1Ptr->tx_Challenges_StartLoc == START_LOC_RANDOM)
      {
        FlagSet(FLAG_START_LOC_RANDOM); 
      }
      SetWarpDestination(MAP_GROUP(OPEN_WORLD), MAP_NUM(OPEN_WORLD), WARP_ID_NONE, 10, 7);
      WarpIntoMap(); 
    }
    else
    {
    SetWarpDestination(MAP_GROUP(INSIDE_OF_TRUCK), MAP_NUM(INSIDE_OF_TRUCK), WARP_ID_NONE, -1, -1);
    WarpIntoMap();
    }
}

void Sav2_ClearSetDefault(void)
{
    ClearSav2();
    SetDefaultOptions();
}

void ResetMenuAndMonGlobals(void)
{
    gDifferentSaveFile = FALSE;
    ResetPokedexScrollPositions();
    ZeroPlayerPartyMons();
    ZeroEnemyPartyMons();
    ResetBagScrollPositions();
    ResetPokeblockScrollPositions();
}

void NewGameInitData(void)
{
    if (gSaveFileStatus == SAVE_STATUS_EMPTY || gSaveFileStatus == SAVE_STATUS_CORRUPT)
        RtcReset();

    gDifferentSaveFile = TRUE;
    gSaveBlock2Ptr->_saveSentinel = 0xFF;
    gSaveBlock2Ptr->saveVersion = SAVE_VERSION;
#ifdef RHH_EXPANSION
    gSaveBlock2Ptr->expansionVersion = RHH_EXPANSION_VERSION;
#endif
    gSaveBlock2Ptr->encryptionKey = 0;
    ZeroPlayerPartyMons();
    ZeroEnemyPartyMons();
    ResetPokedex();
    ClearFrontierRecord();
    ClearSav1();
    ClearAllMail();
    gSaveBlock2Ptr->specialSaveWarpFlags = 0;
    gSaveBlock2Ptr->gcnLinkFlags = 0;
    InitPlayerTrainerId();
    PlayTimeCounter_Reset();
    ClearPokedexFlags();
    InitEventData();
    ClearTVShowData();
    ResetGabbyAndTy();
    ClearSecretBases();
    ClearBerryTrees();
    SetMoney(&gSaveBlock1Ptr->money, 10000);
    SetCoins(0);
    ResetLinkContestBoolean();
    ResetGameStats();
    ClearAllContestWinnerPics();
    ClearPlayerLinkBattleRecords();
    InitSeedotSizeRecord();
    InitLotadSizeRecord();
    gPlayerPartyCount = 0;
    ZeroPlayerPartyMons();
    ResetPokemonStorageSystem();
    ClearRoamerData();
    ClearRoamerLocationData();
    gSaveBlock1Ptr->registeredItemCompat = ITEM_NONE;
    memset(gSaveBlock1Ptr->registeredItems, 0, sizeof(gSaveBlock1Ptr->registeredItems));
    ClearBag();
    NewGameInitPCItems();
    ClearPokeblocks();
    ClearDecorationInventories();
    InitEasyChatPhrases();
    SetMauvilleOldMan();
    InitDewfordTrend();
    ResetFanClub();
    ResetLotteryCorner();
    WarpToTruck();
    RunScriptImmediately(EventScript_ResetAllMapFlags);
    ResetMiniGamesRecords();
    InitUnionRoomChatRegisteredTexts();
    InitLilycoveLady();
    ResetAllApprenticeData();
    ClearRankingHallRecords();
    InitMatchCallCounters();
    ClearMysteryGift();
    WipeTrainerNameRecords();
    ResetTrainerHillResults();
    ResetContestLinkResults();
    memset(gSaveBlock1Ptr->dexNavSearchLevels, 0, sizeof(gSaveBlock1Ptr->dexNavSearchLevels));
    gSaveBlock1Ptr->dexNavChain = 0;
    
    memset(&gSaveBlock2Ptr->itemFlags, 0, sizeof(gSaveBlock2Ptr->itemFlags));
    gSaveBlock2Ptr->autoRun = 0;
    QuestMenu_ResetMenuSaveData();
    gSaveBlock1Ptr->in_battle_type_effectiveness = TRUE; // On by default
    gSaveBlock2Ptr->follower = TRUE;
    gSaveBlock2Ptr->ExpShare = FALSE;
    gSaveBlock2Ptr->playerPartyAvg = 5;
    memset(gSaveBlock2Ptr->Legends, 0, sizeof(gSaveBlock2Ptr->Legends));
    memset(gSaveBlock2Ptr->SpecialMysteryGifts, 0, sizeof(gSaveBlock2Ptr->SpecialMysteryGifts));
    gSaveBlock2Ptr->groupNUM = 0;
    gSaveBlock2Ptr->mapNUM = 0;
    gSaveBlock2Ptr->storedIndex = 0;
    gSaveBlock2Ptr->LegendsCount = 0;
    gSaveBlock2Ptr->specialMysteryGiftCount = 0;
    gSaveBlock1Ptr->checks = 0;
    gSaveBlock1Ptr->seasons = 0;
    SetSeasonChangeDay();
}

static void ResetMiniGamesRecords(void)
{
    CpuFill16(0, &gSaveBlock2Ptr->berryCrush, sizeof(struct BerryCrush));
    SetBerryPowder(&gSaveBlock2Ptr->berryCrush.berryPowderAmount, 0);
    ResetPokemonJumpRecords();
    CpuFill16(0, &gSaveBlock2Ptr->berryPick, sizeof(struct BerryPickingResults));
}
