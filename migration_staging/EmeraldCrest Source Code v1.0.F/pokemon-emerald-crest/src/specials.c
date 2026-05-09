#include "global.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_ai_main.h"
#include "battle_ai_util.h"
#include "battle_arena.h"
#include "battle_controllers.h"
#include "battle_interface.h"
#include "battle_main.h"
#include "battle_message.h"
#include "battle_pyramid.h"
#include "battle_scripts.h"
#include "battle_setup.h"
#include "battle_tower.h"
#include "battle_util.h"
#include "berry.h"
#include "bg.h"
#include "data.h"
#include "decompress.h"
#include "dexnav.h"
#include "dma3.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "event_scripts.h"
#include "evolution_scene.h"
#include "field_screen_effect.h"
#include "graphics.h"
#include "gpu_regs.h"
#include "international_string_util.h"
#include "item.h"
#include "link.h"
#include "link_rfu.h"
#include "load_save.h"
#include "main.h"
#include "malloc.h"
#include "m4a.h"
#include "palette.h"
#include "party_menu.h"
#include "pokeball.h"
#include "pokedex.h"
#include "pokemon.h"
#include "random.h"
#include "recorded_battle.h"
#include "roamer.h"
#include "safari_zone.h"
#include "scanline_effect.h"
#include "script.h"
#include "sound.h"
#include "specials.h"
#include "sprite.h"
#include "string_util.h"
#include "strings.h"
#include "task.h"
#include "test_runner.h"
#include "text.h"
#include "trig.h"
#include "tv.h"
#include "util.h"
#include "wild_encounter.h"
#include "window.h"
#include "constants/abilities.h"
#include "constants/battle_move_effects.h"
#include "constants/battle_string_ids.h"
#include "constants/event_objects.h"
#include "constants/hold_effects.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/party_menu.h"
#include "constants/rgb.h"
#include "constants/songs.h"
#include "constants/trainers.h"
#include "cable_club.h"
#include "tx_randomizer_and_challenges.h"
#include "overworld.h"

void BadgeCount(void) {
  u8 i;
  u8 numBadges = 0;

  for (i = 0; i < 8; i++) {
    if (FlagGet(FLAG_BADGE01_GET + i)) {
      numBadges++;
    }
  }
  gSaveBlock2Ptr->badges = numBadges;
}

void BadgesObtained(void) {
   gSpecialVar_0x8005 = gSaveBlock2Ptr->badges;
   return; 
}

void DayOrNight(void) {
    
    if (gTimeOfDay == TIME_OF_DAY_NIGHT) {
        gSpecialVar_0x8005 = 1;
    }
    else{
        gSpecialVar_0x8005 = 0;
    }
}


const Flags flagset1[] = {
    {FLAG_SYS_POKEMON_GET, 1},
    {FLAG_RESCUED_BIRCH, 1},
    {FLAG_HIDE_ROUTE_101_BIRCH_ZIGZAGOON_BATTLE, 1},
    {FLAG_ADVENTURE_STARTED, 1},
    {FLAG_HIDE_ROUTE_101_ZIGZAGOON, 1},
    {FLAG_HIDE_ROUTE_103_RIVAL, 1},
    {FLAG_HIDE_LITTLEROOT_TOWN_PLAYERS_HOUSE_VIGOROTH_1, 1},
    {FLAG_HIDE_LITTLEROOT_TOWN_PLAYERS_HOUSE_VIGOROTH_2, 1},
    {FLAG_SYS_DEXNAV_GET, 1},
    {FLAG_SYS_QUEST_MENU_GET, 1},
    {FLAG_HIDE_KIRLIA_MAN, 1},
    {FLAG_SHOW_IV_AND_EV, 1},
    {FLAG_MR_BRINEY_SAILING_INTRO, 1},
    {FLAG_DELIVERED_STEVEN_LETTER, 1},
    {FLAG_DELIVERED_DEVON_GOODS, 1},
    {FLAG_HIDE_PETALBURG_WOODS_DEVON_EMPLOYEE, 1},
};
const Flags flagset2[] = {
    {FLAG_HIDE_PETALBURG_WOODS_AQUA_GRUNT, 1},
    {FLAG_RECOVERED_DEVON_GOODS, 1},
    {FLAG_DEVON_GOODS_STOLEN, 1},
    {FLAG_RETURNED_DEVON_GOODS, 1},
    {FLAG_SYS_POKENAV_GET, 1},
    {FLAG_RECEIVED_POKENAV, 1},
    {FLAG_HIDE_RUSTBORO_CITY_POKEMON_SCHOOL_SCOTT, 1},
    {FLAG_RECEIVED_EXP_SHARE, 1},
    {FLAG_HIDE_GRANITE_CAVE_STEVEN, 1},
    {FLAG_DELIVERED_DEVON_GOODS, 1},
    {FLAG_HIDE_ROUTE_101_BIRCH_STARTERS_BAG, 1},
    {FLAG_HIDE_SLATEPORT_CITY_TEAM_AQUA, 1},
    {FLAG_HIDE_ROUTE_110_TEAM_AQUA, 1},
    {FLAG_HIDE_SLATEPORT_CITY_OCEANIC_MUSEUM_AQUA_GRUNTS, 1},
    {FLAG_HIDE_SLATEPORT_CITY_OCEANIC_MUSEUM_2F_CAPTAIN_STERN, 1},
    {FLAG_HIDE_MAUVILLE_CITY_WALLYS_UNCLE, 1},
};
const Flags flagset3[] = {
    {FLAG_HIDE_MAUVILLE_CITY_WALLY, 1},
    {FLAG_RUSTURF_TUNNEL_OPENED, 1},
    {FLAG_HIDE_RUSTURF_TUNNEL_WANDA, 1},
    {FLAG_HIDE_MT_CHIMNEY_TEAM_AQUA, 1},
    {FLAG_HIDE_MT_CHIMNEY_TEAM_MAGMA, 1},
    {FLAG_HIDE_VERDANTURF_TOWN_WANDAS_HOUSE_WALLY, 1},
    {FLAG_HIDE_ROUTE_112_TEAM_MAGMA, 1},
    {FLAG_HIDE_MT_CHIMNEY_TEAM_AQUA, 1},
    {FLAG_HIDE_MT_CHIMNEY_TEAM_MAGMA, 1},
    {FLAG_RECEIVED_METEORITE, 1},
};

void GetStats(void)
{
  u32 stats[53];
  u8 i;

  const char* statNames[53] = {
      "Saved Game",
      "First Hall of Fame Play Time",
      "Started Trends",
      "Planted Berries",
      "Traded Bikes",
      "Steps",
      "Got Interviewed",
      "Total Battles",
      "Wild Battles",
      "Trainer Battles",
      "Entered Hall of Fame",
      "Pokemon Captures",
      "Fishing Captures",
      "Hatched Eggs",
      "Evolved Pokemon",
      "Used Pokecenter",
      "Rested at Home",
      "Entered Safari Zone",
      "Used Cut",
      "Used Rock Smash",
      "Moved Secret Base",
      "Pokemon Trades",
      "Unknown Stat 22",
      "Link Battle Wins",
      "Link Battle Losses",
      "Link Battle Draws",
      "Used Splash",
      "Used Struggle",
      "Slot Jackpots",
      "Consecutive Roulette Wins",
      "Entered Battle Tower",
      "Unknown Stat 31",
      "Battle Tower Singles Streak",
      "Pokeblocks",
      "Pokeblocks with Friends",
      "Won Link Contest",
      "Entered Contest",
      "Won Contest",
      "Shopped",
      "Used Itemfinder",
      "Got Rained On",
      "Checked Pokedex",
      "Received Ribbons",
      "Jumped Down Ledges",
      "Watched TV",
      "Checked Clock",
      "Won Pokemon Lottery",
      "Used Daycare",
      "Rode Cable Car",
      "Entered Hot Springs",
      "Number of Union Room Battles",
      "Played Berry Crush",
      "Dexnav Scanned"
  };

  for (i = 0; i < 53; i++)
  {
    stats[i] = GetGameStat(i);
  }

  for (i = 0; i < 53; i++)
  {
    #ifndef NDEBUG    
    MgbaPrintf(MGBA_LOG_DEBUG,"\t%s: %d\n", statNames[i], stats[i]);
    #endif
  }
}

void HyperTrainPartyStats(void)
{
    u8 value = TRUE;
    u8 i;
    u8 HP = GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_HP_IV, &value);
    u8 ATK = GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_ATK_IV, &value);
    u8 DEF = GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_DEF_IV, &value);
    u8 SPEED = GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_SPEED_IV, &value);
    u8 SPATK = GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_SPATK_IV, &value);
    u8 SPDEF = GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_SPDEF_IV, &value);

    if(IsStatHyperTrained(&gPlayerParty[gSpecialVar_0x8004], gSpecialVar_0x8006) == TRUE)
    {
      ScriptContext_SetupScript(EventScript_MaxStats);
      return;
    }

    if (gSpecialVar_0x8006 == 6) {
      for (i = 0; i < 5; i++) {
        if (IsStatHyperTrained(&gPlayerParty[gSpecialVar_0x8004], i) == FALSE && IsStatsMax(&gPlayerParty[gSpecialVar_0x8004], i) == FALSE) {
            if (HP != MAX_PER_STAT_IVS) {
            SetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_HYPER_TRAINED_HP, &value);
            } if (ATK != MAX_PER_STAT_IVS) {
            SetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_HYPER_TRAINED_ATK, &value);
            } if (DEF != MAX_PER_STAT_IVS) {
            SetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_HYPER_TRAINED_DEF, &value);
            } if (SPEED != MAX_PER_STAT_IVS) {
            SetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_HYPER_TRAINED_SPEED, &value);
            } if (SPATK != MAX_PER_STAT_IVS) {
            SetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_HYPER_TRAINED_SPATK, &value);
            } if (SPDEF != MAX_PER_STAT_IVS) {
            SetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_HYPER_TRAINED_SPDEF, &value);
            }           
        }
      }
      CalculateMonStats(&gPlayerParty[gSpecialVar_0x8004]);
      return;
    }

    if (gSpecialVar_0x8006 == 0) {
        
        if (HP == MAX_PER_STAT_IVS) {
        ScriptContext_SetupScript(EventScript_MaxStats);
        }
        SetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_HYPER_TRAINED_HP, &value);
        } else if (gSpecialVar_0x8006 == 1) {     
        if (ATK == MAX_PER_STAT_IVS) {
        ScriptContext_SetupScript(EventScript_MaxStats);
        }
        SetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_HYPER_TRAINED_ATK, &value);
        } else if (gSpecialVar_0x8006 == 2) {
        if (DEF == MAX_PER_STAT_IVS) {
        ScriptContext_SetupScript(EventScript_MaxStats);
        }
        SetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_HYPER_TRAINED_DEF, &value);
        } else if (gSpecialVar_0x8006 == 3) {
        if (SPEED == MAX_PER_STAT_IVS) {
        ScriptContext_SetupScript(EventScript_MaxStats);
        }
        SetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_HYPER_TRAINED_SPEED, &value);
        } else if (gSpecialVar_0x8006 == 4) {
        if (SPATK == MAX_PER_STAT_IVS) {
        ScriptContext_SetupScript(EventScript_MaxStats);
        }
        SetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_HYPER_TRAINED_SPATK, &value);
        } else if (gSpecialVar_0x8006 == 5) {
        if (SPDEF == MAX_PER_STAT_IVS) {
        ScriptContext_SetupScript(EventScript_MaxStats);
        }
        SetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_HYPER_TRAINED_SPDEF, &value);
        }
      CalculateMonStats(&gPlayerParty[gSpecialVar_0x8004]);
      return;
} 

void StatusCode(void) {
  u16 flagStatus[3] = {0, 0, 0};
  u8 i;

  for (i = 0; i < sizeof(flagset1) / sizeof(flagset1[0]); i++) {
    if (FlagGet(flagset1[i].flag)) {
      flagStatus[0] |= (1 << i);
      flagStatus[0] += flagset1[i].increment;
    }
  }

  for (i = 0; i < sizeof(flagset2) / sizeof(flagset2[0]); i++) {
    if (FlagGet(flagset2[i].flag)) {
      flagStatus[1] |= (1 << i);
      flagStatus[1] += flagset2[i].increment;
    }
  }

  for (i = 0; i < sizeof(flagset3) / sizeof(flagset3[0]); i++) {
    if (FlagGet(flagset3[i].flag)) {
      flagStatus[2] |= (1 << i);
      flagStatus[2] += flagset3[i].increment;
    }
  }

  gSpecialVar_0x8005 = flagStatus[0];
  gSpecialVar_0x8006 = flagStatus[1];
  gSpecialVar_0x8007 = flagStatus[2];
}

void TryGenerateShinyOW(void)
{
    u8 rand;
    rand = Random() % 100;

    if (rand == 0) {
        FlagSet(FLAG_OW_IS_SHINY);
        FlagSet(FLAG_DYNAMIC_OW);
        return;
    }
    FlagSet(FLAG_DYNAMIC_OW);
    FlagClear(FLAG_OW_IS_SHINY);
    return;
}

u8 GetGameDifficulty(void)
{
  if (gSaveBlock1Ptr->tx_Difficulty == 3) {
    return EXTREME;
  } else if (gSaveBlock1Ptr->tx_Difficulty == 2)
  {
    return HARD;
  } else if (gSaveBlock1Ptr->tx_Difficulty == 1)
  {
    return NORMAL;
  } else {
    return EASY;
  }
}

void CheckGameDifficulty(void)
{
  
  if (gSaveBlock1Ptr->tx_Difficulty == 2)
  {
    if ((gSaveBlock1Ptr->tx_Challenges_Nuzlocke >= 1 || gSaveBlock1Ptr->tx_Challenges_PartyLimit >= 1))
    {
      gSaveBlock1Ptr->tx_Difficulty = 3;
      return;
    }
  }

  if (((gSaveBlock1Ptr->tx_Challenges_LevelCap == 2 &&
       gSaveBlock1Ptr->tx_Challenges_Gym == 1 &&
       gSaveBlock1Ptr->tx_Challenges_NoItemPlayer == 1 &&
       gSaveBlock1Ptr->tx_Challenges_NoItemTrainer == 1 &&
       gSaveBlock1Ptr->tx_Challenges_PerfectIVs == 0 &&
       gSaveBlock1Ptr->tx_Challenges_NoEVs == 0 &&
       gSaveBlock1Ptr->tx_Challenges_TrainerScalingEVs == 2 &&
       gSaveBlock1Ptr->tx_Challenges_TrainerScalingIVs == 2 &&
       gSaveBlock1Ptr->tx_Random_Trainer == 0 &&
       gSaveBlock1Ptr->tx_Random_TypeEffectiveness == 0 &&
       gSaveBlock1Ptr->tx_Random_Chaos == 0 &&
       gSaveBlock1Ptr->tx_Challenges_Nuzlocke >= 1) ||
       (gSaveBlock1Ptr->tx_Challenges_LevelCap == 2 &&
       gSaveBlock1Ptr->tx_Challenges_Gym == 1 &&
       gSaveBlock1Ptr->tx_Challenges_NoItemPlayer == 1 &&
       gSaveBlock1Ptr->tx_Challenges_NoItemTrainer == 1 &&
       gSaveBlock1Ptr->tx_Challenges_PerfectIVs == 0 &&
       gSaveBlock1Ptr->tx_Challenges_NoEVs == 0 &&
       gSaveBlock1Ptr->tx_Challenges_TrainerScalingEVs == 2 &&
       gSaveBlock1Ptr->tx_Challenges_TrainerScalingIVs == 2 &&
       gSaveBlock1Ptr->tx_Random_Trainer == 0 &&
       gSaveBlock1Ptr->tx_Random_TypeEffectiveness == 0 &&
       gSaveBlock1Ptr->tx_Challenges_PartyLimit >= 1) ||
       (gSaveBlock1Ptr->tx_Challenges_LevelCap == 2 &&
       gSaveBlock1Ptr->tx_Challenges_Gym == 1 &&
       gSaveBlock1Ptr->tx_Challenges_NoItemPlayer == 1 &&
       gSaveBlock1Ptr->tx_Challenges_NoItemTrainer == 1 &&
       gSaveBlock1Ptr->tx_Challenges_PerfectIVs == 0 &&
       gSaveBlock1Ptr->tx_Challenges_NoEVs == 0 &&
       gSaveBlock1Ptr->tx_Challenges_TrainerScalingEVs == 2 &&
       gSaveBlock1Ptr->tx_Challenges_TrainerScalingIVs == 2 &&
       gSaveBlock1Ptr->tx_Random_Trainer == 0 &&
       gSaveBlock1Ptr->tx_Random_TypeEffectiveness == 0 &&
       gSaveBlock1Ptr->tx_Challenges_PkmnCenter == 1)))
  {
    gSaveBlock1Ptr->tx_Difficulty = 3;
    return;
  } else if (gSaveBlock1Ptr->tx_Challenges_LevelCap == 2 &&
      gSaveBlock1Ptr->tx_Challenges_Gym == 1 &&
      gSaveBlock1Ptr->tx_Challenges_NoItemPlayer == 1 &&
      gSaveBlock1Ptr->tx_Challenges_NoItemTrainer == 1 &&
      gSaveBlock1Ptr->tx_Challenges_PerfectIVs == 0 &&
      gSaveBlock1Ptr->tx_Challenges_NoEVs == 0 &&
      gSaveBlock1Ptr->tx_Challenges_TrainerScalingEVs == 2 &&
      gSaveBlock1Ptr->tx_Challenges_TrainerScalingIVs == 2 &&
      gSaveBlock1Ptr->tx_Random_Trainer == 0 &&
      gSaveBlock1Ptr->tx_Random_TypeEffectiveness == 0)
  {
    gSaveBlock1Ptr->tx_Difficulty = 2;
    return;
  }

  if (gSaveBlock1Ptr->tx_Difficulty == 1)
  {
    if ((gSaveBlock1Ptr->tx_Challenges_Nuzlocke >= 1 || gSaveBlock1Ptr->tx_Challenges_PartyLimit >= 1))
    {
      gSaveBlock1Ptr->tx_Difficulty = 2;
      return;
    }
  }

  if ((gSaveBlock1Ptr->tx_Challenges_LevelCap == 1 &&
      gSaveBlock1Ptr->tx_Challenges_TrainerScalingEVs == 1 &&
      gSaveBlock1Ptr->tx_Challenges_TrainerScalingIVs == 1) || gSaveBlock1Ptr->tx_Challenges_Nuzlocke >= 1)
  {
    gSaveBlock1Ptr->tx_Difficulty = 1;
    return;
  }
  
  if (gSaveBlock1Ptr->tx_Difficulty == 0)
    {
      if (gSaveBlock1Ptr->tx_Challenges_NoEVs == 1 && gSaveBlock1Ptr->tx_Challenges_TrainerScalingEVs == 0) {
          gSaveBlock1Ptr->tx_Challenges_TrainerScalingEVs = 3;
      }
      gSaveBlock1Ptr->tx_Challenges_NoItemTrainer = 1;
    }
  gSaveBlock1Ptr->tx_Difficulty = 0;
  return;
}

void SetPlayerRank(void)
{
  u16 diff = GetGameDifficulty();
  if (gSaveBlock1Ptr->tx_Challenges_NuzlockeHardcore == 1)
  {
    gSaveBlock2Ptr->playerRank += 5;
    #ifndef NDEBUG
    MgbaPrintf(MGBA_LOG_DEBUG, "rank points:%d", gSaveBlock2Ptr->playerRank);
    #endif
  }

  switch (diff)
  {
    case EASY:
      gSaveBlock2Ptr->playerRank = 5;
      break;
    case NORMAL:
      gSaveBlock2Ptr->playerRank = 10;
      break;
    case HARD:
      gSaveBlock2Ptr->playerRank = 15;
      break;
    case EXTREME:
      gSaveBlock2Ptr->playerRank = 20;
      break;
    default:
      gSaveBlock2Ptr->playerRank = 5;
      break;
  }
}

EWRAM_DATA static bool8 Checks = FALSE;

void CalculatePlayerRank(void)
{
  u32 rank = gSaveBlock2Ptr->playerRank;
  u16 playTimeHours = gSaveBlock2Ptr->playTimeHours;
  
  if (FlagGet(FLAG_OPEN_WORLD) && FlagGet(FLAG_IS_CHAMPION) && Checks == FALSE)
  {
    Checks = TRUE;
    if (playTimeHours <= 3)
    {
      rank += 15;
    } else if (playTimeHours <= 4)
    {
      rank += 10;
    } else if (playTimeHours <= 13)
    {
      rank += 5;
    } else if (playTimeHours <= 23)
    {
      rank += 2;
    } else if (playTimeHours <= 45)
    {
      rank += 1;
    }
  } else if (FlagGet(FLAG_IS_CHAMPION) && Checks == FALSE) {
    Checks = TRUE;
    if (playTimeHours <= 3) {
      rank += 40;
    } else if (playTimeHours <= 4) {
      rank += 30;
    }
    else if (playTimeHours <= 13) {
      rank += 20;
    }
    else if (playTimeHours <= 23) {
      rank += 15;
    }
    else if (playTimeHours <= 45) {
      rank += 8;
    }
  }

  gSaveBlock2Ptr->playerRank = rank;
}

u8 CheckPlayerRank(void)
{
  u8 i;
  u32 stats[53];
  u32 rank = gSaveBlock2Ptr->playerRank;
  CalculatePlayerRank();

  #ifndef NDEBUG
    // for (i = 0; i < 53; i++)
    // {
    //   stats[i] = GetGameStat(i);
    // }
    MgbaPrintf(MGBA_LOG_DEBUG, "rank points:%d", rank);

    // for (i = 0; i < 53; i++)
    // {
    //   MgbaPrintf(MGBA_LOG_DEBUG,"%d\n", stats[i]);
    // }
  #endif

  if (rank <= 0)
  {
    return ROOKIE;
  }
  else if (rank >= 80000) {
    return LEGEND;
  }
  else if (rank >= 60000) {
    return MASTER;
  }
  else if (rank >= 40000) {
    return CHAMPION;
  }
  else if (rank >= 20000) {
    return ELITE;
  }
  else if (rank >= 10000) {
    return ADVENTURER;
  } else {
    return ROOKIE;
  }
}

const u8 gText_Legend[] = _("{COLOR RED}LEGEND");
const u8 sText_Master[] = _("{COLOR BLUE}MASTER");
const u8 gText_Champion[] = _("CHAMPION");
const u8 gText_Elite[] = _("ELITE");
const u8 gText_Adventurer[] = _("ADVENTURER");
const u8 gText_Rookie[] = _("ROOKIE");

void checkpartyhasmon(void)
{
  u8 i;
  gSpecialVar_0x8000 = FALSE;

  for (i = 0; i < PARTY_SIZE; i++)
  {
    if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES, NULL) == gSpecialVar_0x8001)
    {
      gSpecialVar_0x8000 = TRUE;
      break;
    }
  }
}

static const u16 Legends[] = {
  SPECIES_ARTICUNO,
  SPECIES_ARTICUNO_GALARIAN,
  SPECIES_ZAPDOS,
  SPECIES_ZAPDOS_GALARIAN,
  SPECIES_MOLTRES,
  SPECIES_MOLTRES_GALARIAN,
  SPECIES_MEWTWO,
  SPECIES_DIALGA,
  SPECIES_PALKIA,
  SPECIES_HEATRAN,
  SPECIES_REGIGIGAS,
  SPECIES_GIRATINA,
  SPECIES_CRESSELIA,
  SPECIES_XERNEAS,
  SPECIES_YVELTAL,
  SPECIES_NIHILEGO,
  SPECIES_BUZZWOLE,
  SPECIES_PHEROMOSA,
  SPECIES_XURKITREE,
  SPECIES_CELESTEELA,
  SPECIES_KARTANA,
  SPECIES_GUZZLORD,
  SPECIES_POIPOLE,
  SPECIES_STAKATAKA,
  SPECIES_BLACEPHALON,
  SPECIES_COSMOG,
  SPECIES_NECROZMA,
  SPECIES_ZACIAN,
  SPECIES_ZAMAZENTA,
  SPECIES_KUBFU,
  SPECIES_ETERNATUS,
  SPECIES_REGIELEKI,
  SPECIES_REGIDRAGO,
  SPECIES_GLASTRIER,
  SPECIES_SPECTRIER,
  SPECIES_CALYREX,
  SPECIES_ENAMORUS,
  SPECIES_WO_CHIEN,
  SPECIES_CHIEN_PAO,
  SPECIES_TING_LU,
  SPECIES_CHI_YU,
  SPECIES_KORAIDON,
  SPECIES_MIRAIDON,
  SPECIES_PHIONE,
  SPECIES_MANAPHY,
  SPECIES_SHAYMIN,
  SPECIES_KELDEO,
  SPECIES_MELOETTA,
  SPECIES_GENESECT,
  SPECIES_DIANCIE,
  SPECIES_VOLCANION,
  SPECIES_MAGEARNA,
  SPECIES_ARCEUS,
  SPECIES_ZERAORA,
  SPECIES_ZARUDE,
  SPECIES_OKIDOGI,
  SPECIES_MUNKIDORI,
  SPECIES_FEZANDIPITI,
  SPECIES_OGERPON,
  SPECIES_URSALUNA_BLOODMOON,
  SPECIES_WALKING_WAKE,
  SPECIES_IRON_LEAVES,
  SPECIES_GOUGING_FIRE,
  SPECIES_RAGING_BOLT,
  SPECIES_IRON_BOULDER,
  SPECIES_IRON_CROWN,
  SPECIES_TERAPAGOS,
  SPECIES_TYPE_NULL,
  // SPECIES_TERAPAGOS_TERASTAL,
  // SPECIES_TERAPAGOS_STELLAR,
  SPECIES_PECHARUNT,
  SPECIES_VICTINI,
  SPECIES_CELEBI,
  SPECIES_JIRACHI,
  SPECIES_TAPU_KOKO,
  SPECIES_TAPU_LELE, 
  SPECIES_TAPU_BULU,
  SPECIES_TAPU_FINI,
  SPECIES_MARSHADOW, 
  SPECIES_MAGEARNA_ORIGINAL_COLOR,
  SPECIES_ZYGARDE,
};

static const u16 LegendMaps[] = {
  MAP_ROUTE103,
  MAP_ROUTE104,
  MAP_ROUTE105,
  MAP_ROUTE110,
  MAP_ROUTE111,
  MAP_ROUTE113,
  MAP_ROUTE114,
  MAP_ROUTE117,
  MAP_ROUTE119,
  MAP_ROUTE120,
  MAP_ROUTE124,
  MAP_ROUTE126,
  MAP_SKY_PILLAR_OUTSIDE,
  MAP_AQUA_HIDEOUT_B2F,
  MAP_MT_CHIMNEY,
  MAP_NEW_MAUVILLE_INSIDE,
  MAP_ABANDONED_SHIP_UNDERWATER2,
  MAP_GRANITE_CAVE_B2F,
  MAP_MT_PYRE_EXTERIOR,
  MAP_PETALBURG_WOODS,
  MAP_RUSTURF_TUNNEL,
};

// Stores a captured legendary Pokémon's species ID in the save data
void StoreCapturedLegend(void) 
{
    // Get the species ID from temporary variable
    u16 species = VarGet(VAR_TEMP_A);
    
    // Check if we haven't exceeded the maximum number of legends that can be stored
    if (gSaveBlock2Ptr->LegendsCount < ARRAY_COUNT(gSaveBlock2Ptr->Legends)) {
        // Store the species ID in the legends array
        gSaveBlock2Ptr->Legends[gSaveBlock2Ptr->LegendsCount] = species;
        // Increment the count of captured legends
        gSaveBlock2Ptr->LegendsCount++;
    }
}

// Checks if a given legendary Pokémon species has been captured
bool8 CheckLegendIsCaptured(u16 species) 
{
    u8 i;
    // Loop through captured legends array
    for (i = 0; i < gSaveBlock2Ptr->LegendsCount; i++) {
        // Check if species matches any captured legend
        if (gSaveBlock2Ptr->Legends[i] == species) {
            return TRUE;
        }
    }
    return FALSE;
}

// Attempts to generate a legendary Pokémon encounter through Hoopa's portals
void TryGenerateLegend(void)
{
    u8 randomIndex, randomMapIndex;
    // Get player's lead Pokémon species
    u16 species = GetMonData(&gPlayerParty[0], MON_DATA_SPECIES);

    // Return if certain conditions are met
    if (FlagGet(FLAG_UNUSED_0x95D) || gSaveBlock2Ptr->LegendsCount == ARRAY_COUNT(Legends)) {
        return;
    }
    
    // If portal is already visible, set up the legend encounter
    if (FlagGet(FLAG_WORMHOLE_VISIBLE)) {
        VarSet(VAR_TEMP_A, Legends[gSaveBlock2Ptr->storedIndex]);
        FlagClear(FLAG_WORMHOLE_HIDE);
        return;
    }
    
    // Pick a random uncaught legendary
    do {
        randomIndex = Random() % ARRAY_COUNT(Legends);
        gSaveBlock2Ptr->storedIndex = randomIndex;
    } while (CheckLegendIsCaptured(Legends[randomIndex]));
    
    // 1% chance to generate portal if player has Hoopa
    if ((species == SPECIES_HOOPA || species == SPECIES_HOOPA_UNBOUND)) {
        FlagClear(FLAG_WORMHOLE_HIDE);
        FlagSet(FLAG_WORMHOLE_VISIBLE);
        // Pick random map location for portal
        randomMapIndex = Random() % ARRAY_COUNT(LegendMaps);
        gSaveBlock2Ptr->mapNUM = LegendMaps[randomMapIndex] & 0xFF;
        gSaveBlock2Ptr->groupNUM = (LegendMaps[randomMapIndex] >> 8) & 0xFF;
        VarSet(VAR_TEMP_A, Legends[randomIndex]);
    }
}

// Handles warping through Hoopa's portals
void checkForPortal(void) {
    // Get player's lead Pokemon species
    u16 species = GetMonData(&gPlayerParty[0], MON_DATA_SPECIES);
    
    // Return if portal isn't visible
    BadgeCount();
    if (!FlagGet(FLAG_WORMHOLE_VISIBLE) || gSaveBlock2Ptr->badges <= 4) {
        return;
    }
    
    // Only allow warping if player has Hoopa
    if (species == SPECIES_HOOPA || species == SPECIES_HOOPA_UNBOUND) {
        // Special case for specific map coordinates
        if (gSaveBlock2Ptr->mapNUM == 41 && gSaveBlock2Ptr->groupNUM == 0) {
            SetWarpDestination(gSaveBlock2Ptr->groupNUM, gSaveBlock2Ptr->mapNUM, 0, 41, 64);
            DoWarp();
            ResetInitialPlayerAvatarState();
        } else {
            // Normal case - warp to stored map location
            SetWarpDestinationToMapWarp(gSaveBlock2Ptr->groupNUM, gSaveBlock2Ptr->mapNUM, 0);
            DoWarp();
            ResetInitialPlayerAvatarState();
        }
    } else {
        return;
    }
}

bool8 IsMapRandmoized(void)
{
  if (gSaveBlock1Ptr->randomMap == 1){
    gSpecialVar_Result = TRUE;
    return TRUE;
  }
  gSpecialVar_Result = FALSE;
  return FALSE;
}

static const u16 sRockSmashItems[] =
{
  // fossil
  ITEM_HELIX_FOSSIL,
  ITEM_DOME_FOSSIL,
  ITEM_OLD_AMBER,
  ITEM_ROOT_FOSSIL,
  ITEM_CLAW_FOSSIL,
  ITEM_ARMOR_FOSSIL,
  ITEM_SKULL_FOSSIL,
  ITEM_COVER_FOSSIL,
  ITEM_PLUME_FOSSIL,
  ITEM_JAW_FOSSIL,
  ITEM_SAIL_FOSSIL,
  ITEM_FOSSILIZED_BIRD,
  ITEM_FOSSILIZED_DINO,
  ITEM_FOSSILIZED_DRAKE,
  ITEM_FOSSILIZED_FISH,
  // plates
  ITEM_FLAME_PLATE,
  ITEM_SPLASH_PLATE,
  ITEM_ZAP_PLATE,
  ITEM_MEADOW_PLATE,
  ITEM_ICICLE_PLATE,
  ITEM_FIST_PLATE,
  ITEM_TOXIC_PLATE,
  ITEM_EARTH_PLATE,
  ITEM_SKY_PLATE,
  ITEM_MIND_PLATE,
  ITEM_INSECT_PLATE,
  ITEM_STONE_PLATE,
  ITEM_SPOOKY_PLATE,
  ITEM_DRACO_PLATE,
  ITEM_DREAD_PLATE,
  ITEM_IRON_PLATE,
  ITEM_PIXIE_PLATE,
  // drive
  ITEM_DOUSE_DRIVE,
  ITEM_SHOCK_DRIVE,
  ITEM_BURN_DRIVE,
  ITEM_CHILL_DRIVE,
  // memory
  ITEM_FIRE_MEMORY,
  ITEM_WATER_MEMORY,
  ITEM_ELECTRIC_MEMORY,
  ITEM_GRASS_MEMORY,
  ITEM_ICE_MEMORY,
  ITEM_FIGHTING_MEMORY,
  ITEM_POISON_MEMORY,
  ITEM_GROUND_MEMORY,
  ITEM_FLYING_MEMORY,
  ITEM_PSYCHIC_MEMORY,
  ITEM_BUG_MEMORY,
  ITEM_ROCK_MEMORY,
  ITEM_GHOST_MEMORY,
  ITEM_DRAGON_MEMORY,
  ITEM_DARK_MEMORY,
  ITEM_STEEL_MEMORY,
  ITEM_FAIRY_MEMORY,
  // gem
  ITEM_NORMAL_GEM,
  ITEM_FIRE_GEM,
  ITEM_WATER_GEM,
  ITEM_ELECTRIC_GEM,
  ITEM_GRASS_GEM,
  ITEM_ICE_GEM,
  ITEM_FIGHTING_GEM,
  ITEM_POISON_GEM,
  ITEM_GROUND_GEM,
  ITEM_FLYING_GEM,
  ITEM_PSYCHIC_GEM,
  ITEM_BUG_GEM,
  ITEM_ROCK_GEM,
  ITEM_GHOST_GEM,
  ITEM_DRAGON_GEM,
  ITEM_DARK_GEM,
  ITEM_STEEL_GEM,
  ITEM_FAIRY_GEM,
  // misc
  ITEM_EXP_CANDY_XS,
  ITEM_EXP_CANDY_S,
  ITEM_EXP_CANDY_M,
  ITEM_REVIVE,
  ITEM_MAX_REVIVE,
  ITEM_NUGGET,
  ITEM_BIG_NUGGET,
  ITEM_ABILITY_PATCH,
  ITEM_ABILITY_CAPSULE,
  ITEM_ABILITY_SHIELD,
  // feather
  ITEM_HEALTH_FEATHER,
  ITEM_MUSCLE_FEATHER,
  ITEM_RESIST_FEATHER,
  ITEM_GENIUS_FEATHER,
  ITEM_CLEVER_FEATHER,
  ITEM_SWIFT_FEATHER,
  // mega stones
  ITEM_VENUSAURITE,
  ITEM_CHARIZARDITE_X,
  ITEM_CHARIZARDITE_Y,
  ITEM_BLASTOISINITE,
  ITEM_BEEDRILLITE,
  ITEM_PIDGEOTITE,
  ITEM_ALAKAZITE,
  ITEM_SLOWBRONITE,
  ITEM_GENGARITE,
  ITEM_KANGASKHANITE,
  ITEM_PINSIRITE,
  ITEM_GYARADOSITE,
  ITEM_AERODACTYLITE,
  ITEM_MEWTWONITE_X,
  ITEM_MEWTWONITE_Y,
  ITEM_AMPHAROSITE,
  ITEM_STEELIXITE,
  ITEM_SCIZORITE,
  ITEM_HERACRONITE,
  ITEM_HOUNDOOMINITE,
  ITEM_TYRANITARITE,
  ITEM_SCEPTILITE,
  ITEM_BLAZIKENITE,
  ITEM_SWAMPERTITE,
  ITEM_GARDEVOIRITE,
  ITEM_SABLENITE,
  ITEM_MAWILITE,
  ITEM_AGGRONITE,
  ITEM_MEDICHAMITE,
  ITEM_MANECTITE,
  ITEM_SHARPEDONITE,
  ITEM_CAMERUPTITE,
  ITEM_ALTARIANITE,
  ITEM_BANETTITE,
  ITEM_ABSOLITE,
  ITEM_GLALITITE,
  ITEM_SALAMENCITE,
  ITEM_METAGROSSITE,
  ITEM_LATIASITE,
  ITEM_LATIOSITE,
  ITEM_LOPUNNITE,
  ITEM_GARCHOMPITE,
  ITEM_LUCARIONITE,
  ITEM_ABOMASITE,
  ITEM_GALLADITE,
  ITEM_AUDINITE,
  ITEM_DIANCITE,
 // z stones
  ITEM_NORMALIUM_Z,
  ITEM_FIRIUM_Z,
  ITEM_WATERIUM_Z,
  ITEM_ELECTRIUM_Z,
  ITEM_GRASSIUM_Z,
  ITEM_ICIUM_Z,
  ITEM_FIGHTINIUM_Z,
  ITEM_POISONIUM_Z,
  ITEM_GROUNDIUM_Z,
  ITEM_FLYINIUM_Z,
  ITEM_PSYCHIUM_Z,
  ITEM_BUGINIUM_Z,
  ITEM_ROCKIUM_Z,
  ITEM_GHOSTIUM_Z,
  ITEM_DRAGONIUM_Z,
  ITEM_DARKINIUM_Z,
  ITEM_STEELIUM_Z,
  ITEM_FAIRIUM_Z,
  ITEM_PIKANIUM_Z,
  ITEM_EEVIUM_Z,
  ITEM_SNORLIUM_Z,
  ITEM_MEWNIUM_Z,
  ITEM_DECIDIUM_Z,
  ITEM_INCINIUM_Z,
  ITEM_PRIMARIUM_Z,
  ITEM_LYCANIUM_Z,
  ITEM_MIMIKIUM_Z,
  ITEM_KOMMONIUM_Z,
  ITEM_TAPUNIUM_Z,
  ITEM_SOLGANIUM_Z,
  ITEM_LUNALIUM_Z,
  ITEM_MARSHADIUM_Z,
  ITEM_ALORAICHIUM_Z,
  ITEM_PIKASHUNIUM_Z,
  ITEM_ULTRANECROZIUM_Z,
  ITEM_FLAME_ORB,
  ITEM_TOXIC_ORB,
  ITEM_ELECTRIC_SEED,
  ITEM_PSYCHIC_SEED,
  ITEM_MISTY_SEED,
  ITEM_GRASSY_SEED,
};

void RockSmashItems(void)
{
    if (Random() % 5 != 0)
    {
        gSpecialVar_0x8000 = sRockSmashItems[Random() % ARRAY_COUNT(sRockSmashItems)];
        gSpecialVar_Result = TRUE;
    } else {
        gSpecialVar_Result = FALSE;
    }
  return;
}

static const u16 sFossils[] =
{
  // fossil
  ITEM_HELIX_FOSSIL,
  ITEM_DOME_FOSSIL,
  ITEM_OLD_AMBER,
  ITEM_ROOT_FOSSIL,
  ITEM_CLAW_FOSSIL,
  ITEM_ARMOR_FOSSIL,
  ITEM_SKULL_FOSSIL,
  ITEM_COVER_FOSSIL,
  ITEM_PLUME_FOSSIL,
  ITEM_JAW_FOSSIL,
  ITEM_SAIL_FOSSIL,
  ITEM_FOSSILIZED_DINO,
  ITEM_FOSSILIZED_DRAKE,
};

void CheckPlayerHasFossil(void)
{
  u8 i;
  gSpecialVar_Result = FALSE;
  for(i = 0; i < ARRAY_COUNT(sFossils); i++) {
    if (CheckBagHasItem(sFossils[i], 1) == TRUE){
      gSpecialVar_Result = TRUE;
      break;
    }
  }
  return;
}

void ChooseFossil(void)
{
  switch(gSpecialVar_Result) 
  {
    case 0:
      gSpecialVar_0x8000 = ITEM_CLAW_FOSSIL;
      gSaveBlock2Ptr->fossilSpecies = SPECIES_ANORITH;
      break;
    case 1:
      gSpecialVar_0x8000 = ITEM_ROOT_FOSSIL;
      gSaveBlock2Ptr->fossilSpecies = SPECIES_LILEEP;
      break;
    case 2:
      gSpecialVar_0x8000 = ITEM_HELIX_FOSSIL;
      gSaveBlock2Ptr->fossilSpecies = SPECIES_OMANYTE;
      break;
    case 3:
      gSpecialVar_0x8000 = ITEM_ARMOR_FOSSIL;
      gSaveBlock2Ptr->fossilSpecies = SPECIES_SHIELDON;
      break;
    case 4:
      gSpecialVar_0x8000 = ITEM_COVER_FOSSIL;
      gSaveBlock2Ptr->fossilSpecies = SPECIES_TIRTOUGA;
      break;
    case 5:
      gSpecialVar_0x8000 = ITEM_DOME_FOSSIL;
      gSaveBlock2Ptr->fossilSpecies = SPECIES_KABUTO;
      break;
    case 6:
      gSpecialVar_0x8000 = ITEM_JAW_FOSSIL;
      gSaveBlock2Ptr->fossilSpecies = SPECIES_TYRUNT;
      break;
    case 7:
      gSpecialVar_0x8000 = ITEM_PLUME_FOSSIL;
      gSaveBlock2Ptr->fossilSpecies = SPECIES_ARCHEN;
      break;
    case 8:
      gSpecialVar_0x8000 = ITEM_SAIL_FOSSIL;
      gSaveBlock2Ptr->fossilSpecies = SPECIES_AMAURA;
      break;
    case 9:
      gSpecialVar_0x8000 = ITEM_SKULL_FOSSIL;
      gSaveBlock2Ptr->fossilSpecies = SPECIES_CRANIDOS;
      break;
    case 10:
      gSpecialVar_0x8000 = ITEM_FOSSILIZED_DINO;
      if (Random() % 2 == 1)
      {
        gSaveBlock2Ptr->fossilSpecies = SPECIES_ARCTOZOLT;
      } else {
        gSaveBlock2Ptr->fossilSpecies = SPECIES_ARCTOVISH;
      }
      break;
    case 11:
      gSpecialVar_0x8000 = ITEM_FOSSILIZED_DRAKE;
      if (Random() % 2 == 1)
      {
        gSaveBlock2Ptr->fossilSpecies = SPECIES_DRACOZOLT;
      } else {
        gSaveBlock2Ptr->fossilSpecies = SPECIES_DRACOVISH;
      }
      break;
    case 12:
      gSpecialVar_0x8000 = ITEM_OLD_AMBER;
      gSaveBlock2Ptr->fossilSpecies = SPECIES_AERODACTYL;
      break;
  }
  return;
}

void getFossilMon(void)
{
  gSpecialVar_0x8001 = gSaveBlock2Ptr->fossilSpecies;
  return;
}

void TurnOffMeteronomeOnlyMode(void)
{
  gSaveBlock1Ptr->MeteronomeOnlyMode = 0;
  return;
}

void GenerateOWEncounter(void)
{
  u16 species = ReturnSpotlightSpecies();
  if (species != SPECIES_NONE) {
    VarSet(VAR_TEMP_6, species);
  } else if (SpotlightChecks()) {
    VarSet(VAR_TEMP_6, gSpecialVar_Unused_0x8014);
  } 
  else {
    FlagSet(FLAG_TEMP_10);
    FlagSet(FLAG_TEMP_11);
    FlagSet(FLAG_TEMP_12);
    FlagSet(FLAG_TEMP_13);
    FlagSet(FLAG_TEMP_14);
    FlagSet(FLAG_TEMP_15);
    FlagSet(FLAG_TEMP_16);
  }

}
void BattleOWEncounter(void)
{
  u16 species = ReturnSpotlightSpecies();
  u8 level;
  if (FlagGet(FLAG_GENERATED_SPOTLIGHT_EVENT)) {
        species = gSpecialVar_Unused_0x8014;
    }
    
    if (species == SPECIES_NONE)
      return;
    
    level = Random() % 9 + 2; 

    CreateWildMon(species, level);
    BattleSetup_StartWildBattle();
}

void PopupEncounters(void)
{
  u8 rand;
  u8 mon;
  u16 flags[] = {
    FLAG_TEMP_10,
    FLAG_TEMP_11,
    FLAG_TEMP_12,
    FLAG_TEMP_13,
    FLAG_TEMP_14,
    FLAG_TEMP_15,
    FLAG_TEMP_16,
  }; 
  
  rand = Random() % 7;
  FlagSet(flags[rand]);
  
  mon = Random() % 7;
  if (mon != rand)
    FlagSet(flags[mon]);
  
  return;
}

u8 GetSeason(void)
{
  if (!FlagGet(FLAG_SEASONS))
    return SEASON_SUMMER;

  return gSaveBlock1Ptr->seasons;
}
