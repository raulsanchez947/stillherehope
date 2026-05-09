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
#include "evolution_scene.h"
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
#include "sound.h"
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
#include "constants/battle_ai.h"
#include "constants/battle_move_effects.h"
#include "constants/battle_string_ids.h"
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

extern struct Evolution gEvolutionTable[][EVOS_PER_MON];

#if B_TRAINER_CLASS_POKE_BALLS >= GEN_7
static const u16 sTrainerBallTable[TRAINER_CLASS_COUNT] =
{
#if B_TRAINER_CLASS_POKE_BALLS == GEN_7
    [TRAINER_CLASS_PKMN_BREEDER] = ITEM_FRIEND_BALL,
#elif B_TRAINER_CLASS_POKE_BALLS == GEN_8
    [TRAINER_CLASS_PKMN_BREEDER] = ITEM_HEAL_BALL,
#endif
    [TRAINER_CLASS_COOLTRAINER] = ITEM_ULTRA_BALL,
    [TRAINER_CLASS_COLLECTOR] = ITEM_PREMIER_BALL,
    [TRAINER_CLASS_SWIMMER_M] = ITEM_DIVE_BALL,
    [TRAINER_CLASS_BLACK_BELT] = ITEM_ULTRA_BALL,
    [TRAINER_CLASS_AQUA_LEADER] = ITEM_MASTER_BALL,
    [TRAINER_CLASS_GENTLEMAN] = ITEM_LUXURY_BALL,
    [TRAINER_CLASS_ELITE_FOUR] = ITEM_ULTRA_BALL,
#if B_TRAINER_CLASS_POKE_BALLS == GEN_7
    [TRAINER_CLASS_FISHERMAN] = ITEM_LURE_BALL,
#elif B_TRAINER_CLASS_POKE_BALLS == GEN_8
    [TRAINER_CLASS_FISHERMAN] = ITEM_DIVE_BALL,
#endif
    [TRAINER_CLASS_SWIMMER_F] = ITEM_DIVE_BALL,
    [TRAINER_CLASS_COOLTRAINER_2] = ITEM_ULTRA_BALL,
    [TRAINER_CLASS_MAGMA_LEADER] = ITEM_MASTER_BALL,
};
#endif

u8 DynamicTarinerParty(struct Pokemon *party, const struct Trainer *trainer, bool32 firstTrainer, u32 battleTypeFlags, u16 trainerNum)
{
    u32 personalityValue;
    u8 fixedIV;
    s32 i, j, k;
    u8 monsCount;
    s32 ball = -1;
    u16 species, move; 
    u8 playerPartyAverageLevel = gSaveBlock2Ptr->playerPartyAvg; //dynamic trainer party
    u8 trainerPartyLevel[6];
    u8 GymLeaderPartyMaxLevel = 0;
    u8 RegularTrainerPartyMaxLevel = 0;
    u8 GymLeaderPartyMinLevel = 0;
    u8 RegularTrainerPartyMinLevel = 0;
    u8 GymLeaderPartySize[] = {3, 3, 2, 2, 1, 1, 0, 0, 0};
    u8 TpartySize;
    u16 evolvedSpecies;
    u8 index;
    const u8 levels[9][4] = {
        {5, 8, 14, 15}, 
        {10, 13, 16, 17}, 
        {16, 17, 23, 24}, 
        {23, 24, 26, 29}, 
        {26, 29, 32, 35}, 
        {32, 35, 38, 40}, 
        {38, 40, 42, 45}, 
        {42, 45, 48, 50}, 
        {48, 50, 52, 56}
    };
    const u8 levelsHard[9][4] = {
        {5, 8, 14, 16}, 
        {14, 15, 16, 18}, 
        {16, 17, 23, 25}, 
        {23, 25, 29, 31}, 
        {29, 31, 35, 37}, 
        {35, 37, 41, 43}, 
        {41, 43, 44, 48},
        {44, 46, 50, 55}, 
        {50, 55, 58, 62}
    };
        const u8 levelsBugFix[9][4] = {
        {5, 8, 14, 16}, 
        {14, 16, 18, 20}, 
        {16, 22, 24, 26}, 
        {22, 32, 34, 35}, 
        {32, 35, 37, 38}, 
        {35, 44, 46, 48}, 
        {44, 52, 54, 56}, 
        {52, 58, 60, 62}, 
        {62, 84, 86, 88}
    };
    const u8 levelsHardBugFix[9][4] = {
        {5, 8, 15, 17}, 
        {15, 17, 19, 21}, 
        {19, 21, 23, 25}, 
        {25, 33, 35, 37}, 
        {37, 39, 41, 43}, 
        {43, 49, 51, 53}, 
        {53, 59, 61, 63}, 
        {63, 67, 69, 71}, 
        {71, 82, 85, 96}
    };

    u8 maxLevel, minLevel, levelRange, randomOffset;
    u8 playerPartySize = 0;
    u16 totalPlayerLevel = 0;

    if (battleTypeFlags & BATTLE_TYPE_TRAINER && !(battleTypeFlags & (BATTLE_TYPE_FRONTIER
                                                                        | BATTLE_TYPE_EREADER_TRAINER
                                                                        | BATTLE_TYPE_TRAINER_HILL)))
    {
        #ifndef NDEBUG
        MgbaPrintf(MGBA_LOG_DEBUG, "******** CreateTrainerParty ********");
        #endif

        if ((gSaveBlock1Ptr->tx_Difficulty >= 2) 
        && (trainer->trainerClass == TRAINER_CLASS_LEADER 
        || trainer->trainerClass == TRAINER_CLASS_ELITE_FOUR 
        || trainer->trainerClass == TRAINER_CLASS_CHAMPION 
        || trainer->trainerClass == TRAINER_CLASS_RIVAL)) {
            //Calculate the average level of the player's party
            for (i = 0; i < 6; i++)
            {
                if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES) != SPECIES_NONE)
                {
                    playerPartySize++;
                    totalPlayerLevel += GetMonData(&gPlayerParty[i], MON_DATA_LEVEL);
                }
            }
            playerPartyAverageLevel = totalPlayerLevel / playerPartySize;
        }

        BadgeCount();
        TpartySize = trainer->partySize - GymLeaderPartySize[gSaveBlock2Ptr->badges];
        index = gSaveBlock2Ptr->badges;
        
        if (FlagGet(FLAG_bugfix_trainer_levels) == FALSE) {
            if (gSaveBlock1Ptr->tx_Difficulty >= 2) {
                RegularTrainerPartyMinLevel = levelsHardBugFix[index][0];
                RegularTrainerPartyMaxLevel = levelsHardBugFix[index][1];
                GymLeaderPartyMinLevel = levelsHardBugFix[index][2];
                GymLeaderPartyMaxLevel = levelsHardBugFix[index][3];
            } else {
                RegularTrainerPartyMinLevel = levelsBugFix[index][0];
                RegularTrainerPartyMaxLevel = levelsBugFix[index][1];
                GymLeaderPartyMinLevel = levelsBugFix[index][2];
                GymLeaderPartyMaxLevel = levelsBugFix[index][3];
            }
        } else {
            if (gSaveBlock1Ptr->tx_Difficulty >= 2) {
                RegularTrainerPartyMinLevel = levelsHard[index][0];
                RegularTrainerPartyMaxLevel = levelsHard[index][1];
                GymLeaderPartyMinLevel = levelsHard[index][2];
                GymLeaderPartyMaxLevel = levelsHard[index][3];
            } else {
                RegularTrainerPartyMinLevel = levels[index][0];
                RegularTrainerPartyMaxLevel = levels[index][1];
                GymLeaderPartyMinLevel = levels[index][2];
                GymLeaderPartyMaxLevel = levels[index][3];
            }
        }

        if (!FlagGet(FLAG_ADVENTURE_STARTED)) {
            RegularTrainerPartyMaxLevel = 5;
        }
        if (firstTrainer == TRUE)
            ZeroEnemyPartyMons();

      if (gSaveBlock1Ptr->tx_Challenges_Gym == 1) {

        if (trainer->trainerClass == TRAINER_CLASS_LEADER)
        {
            TpartySize = 6;
            monsCount = TpartySize;
        }
        else if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
        {
            if (trainer->partySize > PARTY_SIZE / 2)
                monsCount = PARTY_SIZE / 2;
            else
                monsCount = trainer->partySize;
        }
        else
        {
            monsCount = trainer->partySize;
        }  
      } else {          
        if (trainer->trainerClass == TRAINER_CLASS_LEADER)
        {
            monsCount = TpartySize;
        }    
        else if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
        {
            if (trainer->partySize > PARTY_SIZE / 2)
                monsCount = PARTY_SIZE / 2;
            else
                monsCount = trainer->partySize;
        }
        else
        {
            monsCount = trainer->partySize;
        }
      }

        for (i = 0; i < monsCount; i++)
        {

            u32 personalityHash = GeneratePartyHash(trainer, i);
            if (trainer->doubleBattle == TRUE)
                personalityValue = 0x80;
            else if (trainer->encounterMusic_gender & F_TRAINER_FEMALE)
                personalityValue = 0x78; // Use personality more likely to result in a female Pokémon
            else
                personalityValue = 0x88; // Use personality more likely to result in a male Pokémon

            personalityValue += personalityHash << 8;
            switch (trainer->partyFlags)
            {
            case 0:
            {
                const struct TrainerMonNoItemDefaultMoves *partyData = trainer->party.NoItemDefaultMoves;

                fixedIV = partyData[i].iv * MAX_PER_STAT_IVS / 255;                
                if (gSaveBlock1Ptr->tx_Random_Trainer) 
                {
                    species = GetSpeciesRandomSeeded(partyData[i].species, TX_RANDOM_T_TRAINER, trainerNum);
                for (k = 0; k < 6; k++)
                {
                    maxLevel = RegularTrainerPartyMaxLevel;
                    minLevel = RegularTrainerPartyMinLevel;
                    levelRange = maxLevel - playerPartyAverageLevel + 1;
                    randomOffset = Random() % levelRange;
                    trainerPartyLevel[k] = playerPartyAverageLevel + randomOffset;
                    if (trainerPartyLevel[k] > maxLevel) {
                        trainerPartyLevel[k] = maxLevel;
                    } else if (trainerPartyLevel[k] < minLevel) {
                        trainerPartyLevel[k] = minLevel;
                    }               
                    CreateMon(&party[i], species, trainerPartyLevel[k], fixedIV, TRUE, personalityValue, OT_ID_RANDOM_NO_SHINY, 0);
                }
                }
                else
                for (k = 0; k < 6; k++)
                {
                    maxLevel = RegularTrainerPartyMaxLevel;
                    minLevel = RegularTrainerPartyMinLevel;
                    levelRange = maxLevel - playerPartyAverageLevel + 1;
                    randomOffset = Random() % levelRange;
                    trainerPartyLevel[k] = playerPartyAverageLevel + randomOffset;
                    if (trainerPartyLevel[k] > maxLevel) {
                        trainerPartyLevel[k] = maxLevel;
                    } else if (trainerPartyLevel[k] < minLevel) {
                        trainerPartyLevel[k] = minLevel;
                    }
                // Check if the Pokémon can evolve at its current level
                    evolvedSpecies = CheckEvolve(partyData[i].species, trainerPartyLevel[k]);
                    // Create the Pokémon
                    if (evolvedSpecies != 0) {
                        CreateMon(&party[i], evolvedSpecies, trainerPartyLevel[k], fixedIV, TRUE, personalityValue, OT_ID_RANDOM_NO_SHINY, 0);
                    } else {
                        CreateMon(&party[i], partyData[i].species, trainerPartyLevel[k], fixedIV, TRUE, personalityValue, OT_ID_RANDOM_NO_SHINY, 0);
                    }
                }
                break;
            }
            case F_TRAINER_PARTY_CUSTOM_MOVESET:
            {
                const struct TrainerMonNoItemCustomMoves *partyData = trainer->party.NoItemCustomMoves;

                fixedIV = partyData[i].iv * MAX_PER_STAT_IVS / 255;
                if (gSaveBlock1Ptr->tx_Random_Trainer) 
                {
                    species = GetSpeciesRandomSeeded(partyData[i].species, TX_RANDOM_T_TRAINER, trainerNum);
                for (k = 0; k < 6; k++)
                {                    
                    maxLevel = RegularTrainerPartyMaxLevel;
                    minLevel = RegularTrainerPartyMinLevel;
                    levelRange = maxLevel - playerPartyAverageLevel + 1;
                    randomOffset = Random() % levelRange;
                    trainerPartyLevel[k] = playerPartyAverageLevel + randomOffset;
                    if (trainerPartyLevel[k] > maxLevel) {
                        trainerPartyLevel[k] = maxLevel;
                    } else if (trainerPartyLevel[k] < minLevel) {
                        trainerPartyLevel[k] = minLevel;
                    }                   
                    CreateMon(&party[i], species, trainerPartyLevel[k], fixedIV, TRUE, personalityValue, OT_ID_RANDOM_NO_SHINY, 0);
                }
                }
                else
                for (k = 0; k < 6; k++)
                {
                    maxLevel = RegularTrainerPartyMaxLevel;
                    minLevel = RegularTrainerPartyMinLevel;
                    levelRange = maxLevel - playerPartyAverageLevel + 1;
                    randomOffset = Random() % levelRange;
                    trainerPartyLevel[k] = playerPartyAverageLevel + randomOffset;
                    if (trainerPartyLevel[k] > maxLevel) {
                        trainerPartyLevel[k] = maxLevel;
                    } else if (trainerPartyLevel[k] < minLevel) {
                        trainerPartyLevel[k] = minLevel;
                    }
                // Check if the Pokémon can evolve at its current level
                    evolvedSpecies = CheckEvolve(partyData[i].species, trainerPartyLevel[k]);
                    // Create the Pokémon
                    if (evolvedSpecies != 0){
                        CreateMon(&party[i], evolvedSpecies, trainerPartyLevel[k], fixedIV, TRUE, personalityValue, OT_ID_RANDOM_NO_SHINY, 0);
                    } else {
                        CreateMon(&party[i], partyData[i].species, trainerPartyLevel[k], fixedIV, TRUE, personalityValue, OT_ID_RANDOM_NO_SHINY, 0);
                    }
                }
            if (evolvedSpecies != 0) {
                break;
            } else {       
                for (j = 0; j < 4; j++)
                {
                    move = GetMonData(&party[i], MON_DATA_MOVE1 + j);
                    if (move == partyData[i].moves[j])
                        continue;
                    SetMonData(&party[i], MON_DATA_MOVE1 + j, &partyData[i].moves[j]);
                    SetMonData(&party[i], MON_DATA_PP1 + j, &gBattleMoves[partyData[i].moves[j]].pp);
                }
            }
                break;
            }
            case F_TRAINER_PARTY_HELD_ITEM:
            {
                const struct TrainerMonItemDefaultMoves *partyData = trainer->party.ItemDefaultMoves;

                fixedIV = partyData[i].iv * MAX_PER_STAT_IVS / 255;               
                if (gSaveBlock1Ptr->tx_Random_Trainer) 
                {
                    species = GetSpeciesRandomSeeded(partyData[i].species, TX_RANDOM_T_TRAINER, trainerNum);
                for (k = 0; k < 6; k++)
                {                    
                    maxLevel = RegularTrainerPartyMaxLevel;
                    minLevel = RegularTrainerPartyMinLevel;
                    levelRange = maxLevel - playerPartyAverageLevel + 1;
                    randomOffset = Random() % levelRange;
                    trainerPartyLevel[k] = playerPartyAverageLevel + randomOffset;
                    if (trainerPartyLevel[k] > maxLevel) {
                        trainerPartyLevel[k] = maxLevel;
                    } else if (trainerPartyLevel[k] < minLevel) {
                        trainerPartyLevel[k] = minLevel;
                    }                
                    CreateMon(&party[i], species, trainerPartyLevel[k], fixedIV, TRUE, personalityValue, OT_ID_RANDOM_NO_SHINY, 0);
                }
                }
                else
                for (k = 0; k < 6; k++)
                {
                    maxLevel = RegularTrainerPartyMaxLevel;
                    minLevel = RegularTrainerPartyMinLevel;
                    levelRange = maxLevel - playerPartyAverageLevel + 1;
                    randomOffset = Random() % levelRange;
                    trainerPartyLevel[k] = playerPartyAverageLevel + randomOffset;
                    if (trainerPartyLevel[k] > maxLevel) {
                        trainerPartyLevel[k] = maxLevel;
                    } else if (trainerPartyLevel[k] < minLevel) {
                        trainerPartyLevel[k] = minLevel;
                    }
                // Check if the Pokémon can evolve at its current level
                    evolvedSpecies = CheckEvolve(partyData[i].species, trainerPartyLevel[k]);
                    // Create the Pokémon
                    if (evolvedSpecies != 0) {
                        CreateMon(&party[i], evolvedSpecies, trainerPartyLevel[k], fixedIV, TRUE, personalityValue, OT_ID_RANDOM_NO_SHINY, 0);
                    } else {
                        CreateMon(&party[i], partyData[i].species, trainerPartyLevel[k], fixedIV, TRUE, personalityValue, OT_ID_RANDOM_NO_SHINY, 0);
                    }
                }                
                SetMonData(&party[i], MON_DATA_HELD_ITEM, &partyData[i].heldItem);
                break;
            }
            case F_TRAINER_PARTY_HELD_ITEM | F_TRAINER_PARTY_CUSTOM_MOVESET:
            {
                const struct TrainerMonItemCustomMoves *partyData = trainer->party.ItemCustomMoves;
                if (trainer->trainerClass == TRAINER_CLASS_LEADER || 
                    trainer->trainerClass == TRAINER_CLASS_ELITE_FOUR || 
                    trainer->trainerClass == TRAINER_CLASS_CHAMPION) {
                    if (gSaveBlock1Ptr->tx_Difficulty >= 2) {
                        partyData = gTrainers[trainerNum].partyHard.ItemCustomMoves;
                    }
                }
                fixedIV = partyData[i].iv * MAX_PER_STAT_IVS / 255;              
                if (gSaveBlock1Ptr->tx_Random_Trainer) 
                {
                    species = GetSpeciesRandomSeeded(partyData[i].species, TX_RANDOM_T_TRAINER, trainerNum);
                for (k = 0; k < 6; k++)
                {                    
                    if (trainer->trainerClass == TRAINER_CLASS_LEADER || 
                        trainer->trainerClass == TRAINER_CLASS_ELITE_FOUR || 
                        trainer->trainerClass == TRAINER_CLASS_CHAMPION || trainer->trainerClass == TRAINER_CLASS_RIVAL || trainer->trainerClass == TRAINER_CLASS_AQUA_LEADER || trainer->trainerClass == TRAINER_CLASS_MAGMA_LEADER) {
                        maxLevel = GymLeaderPartyMaxLevel;
                        minLevel = GymLeaderPartyMinLevel;
                    } else {
                        maxLevel = RegularTrainerPartyMaxLevel;
                        minLevel = RegularTrainerPartyMinLevel;
                    }
                    if (trainer->trainerClass == TRAINER_CLASS_CHAMPION || trainerNum == TRAINER_STEVEN) {
                        if (gSaveBlock1Ptr->tx_Difficulty >= 2) {
                            minLevel = 93;
                            maxLevel = 96;
                        }
                    } else if (trainer->trainerClass == TRAINER_CLASS_ELITE_FOUR) {
                        if (gSaveBlock1Ptr->tx_Difficulty >= 2) {
                            minLevel = 86;
                            maxLevel = 93;
                        }                
                    }
                    if (k == monsCount)
                    {
                       trainerPartyLevel[k] = maxLevel; 
                    } else {
                    levelRange = maxLevel - playerPartyAverageLevel + 1;
                    randomOffset = Random() % levelRange;
                    trainerPartyLevel[k] = playerPartyAverageLevel + randomOffset;
                    if (trainerPartyLevel[k] > maxLevel) {
                        trainerPartyLevel[k] = maxLevel;
                    } else if (trainerPartyLevel[k] < minLevel) {
                        trainerPartyLevel[k] = minLevel;
                    }
                    }
                    CreateMon(&party[i], species, trainerPartyLevel[k], fixedIV, TRUE, personalityValue, OT_ID_RANDOM_NO_SHINY, 0);
                }
                }
                else
                {
                for (k = 0; k < 6; k++)
                {
                    if (trainer->trainerClass == TRAINER_CLASS_LEADER || 
                        trainer->trainerClass == TRAINER_CLASS_ELITE_FOUR || 
                        trainer->trainerClass == TRAINER_CLASS_CHAMPION || trainer->trainerClass == TRAINER_CLASS_RIVAL || trainer->trainerClass == TRAINER_CLASS_AQUA_LEADER || trainer->trainerClass == TRAINER_CLASS_MAGMA_LEADER) {
                        maxLevel = GymLeaderPartyMaxLevel;
                        minLevel = GymLeaderPartyMinLevel;
                    } else {
                        maxLevel = RegularTrainerPartyMaxLevel;
                        minLevel = RegularTrainerPartyMinLevel;
                    }
                    if (trainer->trainerClass == TRAINER_CLASS_CHAMPION || trainerNum == TRAINER_STEVEN) {
                        if (gSaveBlock1Ptr->tx_Difficulty >= 2) {
                            minLevel = 93;
                            maxLevel = 96;
                        }
                    } else if (trainer->trainerClass == TRAINER_CLASS_ELITE_FOUR) {
                        if (gSaveBlock1Ptr->tx_Difficulty >= 2) {
                            minLevel = 86;
                            maxLevel = 93;
                        }               
                    }
                    if (k == monsCount)
                    {
                       trainerPartyLevel[k] = maxLevel; 
                    } else {
                    levelRange = maxLevel - playerPartyAverageLevel + 1;
                    randomOffset = Random() % levelRange;
                    trainerPartyLevel[k] = playerPartyAverageLevel + randomOffset;
                    if (trainerPartyLevel[k] > maxLevel) {
                        trainerPartyLevel[k] = maxLevel;
                    } else if (trainerPartyLevel[k] < minLevel) {
                        trainerPartyLevel[k] = minLevel;
                    }
                    }
                    // Check if the Pokémon can evolve at its current level
                    evolvedSpecies = CheckEvolve(partyData[i].species, trainerPartyLevel[k]);
                    // Create the Pokémon
                    if (evolvedSpecies != 0) {
                        CreateMon(&party[i], evolvedSpecies, trainerPartyLevel[k], fixedIV, TRUE, personalityValue, OT_ID_RANDOM_NO_SHINY, 0);
                    } else {
                        CreateMon(&party[i], partyData[i].species, trainerPartyLevel[k], fixedIV, TRUE, personalityValue, OT_ID_RANDOM_NO_SHINY, 0);
                    }
                }
                }                     
                SetMonData(&party[i], MON_DATA_HELD_ITEM, &partyData[i].heldItem);

                for (j = 0; j < MAX_MON_MOVES; j++)
                {
                    if (gSaveBlock1Ptr->MeteronomeOnlyMode) {
                        move = MOVE_METRONOME;
                        SetMonData(&party[i], MON_DATA_MOVE1 + j, &move);
                        SetMonData(&party[i], MON_DATA_PP1 + j, &gBattleMoves[move].pp);
                    } else {
                        if (gSaveBlock1Ptr->tx_Random_Moves) 
                        {
                            move = GetRandomMove(partyData[i].moves[j], partyData[i].species);
                            SetMonData(&party[i], MON_DATA_MOVE1 + j, &move);
                            SetMonData(&party[i], MON_DATA_PP1 + j, &gBattleMoves[move].pp);
                        }
                        else
                        {
                            if (evolvedSpecies != 0){
                            break;
                            } else {
                            if (gSaveBlock1Ptr->tx_Random_Trainer) 
                                continue;
                            SetMonData(&party[i], MON_DATA_MOVE1 + j, &partyData[i].moves[j]);
                            SetMonData(&party[i], MON_DATA_PP1 + j, &gBattleMoves[partyData[i].moves[j]].pp);
                            }
                        }
                    }
                }
                break;
            }
            case F_TRAINER_PARTY_EVERYTHING_CUSTOMIZED:
            {
                const struct TrainerMonCustomized *partyData = trainer->party.EverythingCustomized;
                u32 otIdType = OT_ID_RANDOM_NO_SHINY;
                u32 fixedOtId = 0;
                if (partyData[i].gender == TRAINER_MON_MALE)
                    personalityValue = (personalityValue & 0xFFFFFF00) | GeneratePersonalityForGender(MON_MALE, partyData[i].species);
                else if (partyData[i].gender == TRAINER_MON_FEMALE)
                    personalityValue = (personalityValue & 0xFFFFFF00) | GeneratePersonalityForGender(MON_FEMALE, partyData[i].species);
                if (partyData[i].nature != 0)
                    ModifyPersonalityForNature(&personalityValue, partyData[i].nature - 1);
                if (partyData[i].isShiny)
                {
                    otIdType = OT_ID_PRESET;
                    fixedOtId = HIHALF(personalityValue) ^ LOHALF(personalityValue);
                }
                CreateMon(&party[i], partyData[i].species, partyData[i].lvl, 0, TRUE, personalityValue, otIdType, fixedOtId);
                SetMonData(&party[i], MON_DATA_HELD_ITEM, &partyData[i].heldItem);

                // TODO: Figure out a default strategy when moves are not set, to generate a good moveset
                for (j = 0; j < MAX_MON_MOVES; ++j)
                {
                    SetMonData(&party[i], MON_DATA_MOVE1 + j, &partyData[i].moves[j]);
                    SetMonData(&party[i], MON_DATA_PP1 + j, &gBattleMoves[partyData[i].moves[j]].pp);
                }
                SetMonData(&party[i], MON_DATA_IVS, &(partyData[i].iv));
                if (partyData[i].ev != NULL)
                {
                    SetMonData(&party[i], MON_DATA_HP_EV, &(partyData[i].ev[0]));
                    SetMonData(&party[i], MON_DATA_ATK_EV, &(partyData[i].ev[1]));
                    SetMonData(&party[i], MON_DATA_DEF_EV, &(partyData[i].ev[2]));
                    SetMonData(&party[i], MON_DATA_SPATK_EV, &(partyData[i].ev[3]));
                    SetMonData(&party[i], MON_DATA_SPDEF_EV, &(partyData[i].ev[4]));
                    SetMonData(&party[i], MON_DATA_SPEED_EV, &(partyData[i].ev[5]));
                }
                if (partyData[i].ability != ABILITY_NONE)
                {
                    const struct SpeciesInfo *speciesInfo = &gSpeciesInfo[partyData[i].species];
                    u32 maxAbilities = ARRAY_COUNT(speciesInfo->abilities);
                    for (j = 0; j < maxAbilities; ++j)
                    {
                        if (speciesInfo->abilities[j] == partyData[i].ability)
                            break;
                    }
                    if (j < maxAbilities)
                        SetMonData(&party[i], MON_DATA_ABILITY_NUM, &j);
                }
                SetMonData(&party[i], MON_DATA_FRIENDSHIP, &(partyData[i].friendship));
                if (partyData[i].ball != ITEM_NONE)
                {
                    ball = partyData[i].ball;
                    SetMonData(&party[i], MON_DATA_POKEBALL, &ball);
                }
                if (partyData[i].nickname != NULL)
                {
                    SetMonData(&party[i], MON_DATA_NICKNAME, partyData[i].nickname);
                }
                CalculateMonStats(&party[i]);
            }
            }
           #if B_TRAINER_CLASS_POKE_BALLS >= GEN_7
            if (ball == -1)
            {
                ball = (sTrainerBallTable[trainer->trainerClass]) ? sTrainerBallTable[trainer->trainerClass] : ITEM_POKE_BALL;
                SetMonData(&party[i], MON_DATA_POKEBALL, &ball);
            }
        #endif
        }


        
        if (gSaveBlock1Ptr->tx_Challenges_TrainerScalingIVs && !FlagGet(FLAG_IS_CHAMPION))
        {
            u8 iv = GetCurrentTrainerIVs();

            for (i = 0; i < monsCount; i++)
            {
                for (j = 0; j < 6; j++)
                {
                    SetMonData(&party[i], MON_DATA_HP_IV + j, &iv);
                }

                CalculateMonStats(&party[i]);
            }
        }
        if (gSaveBlock1Ptr->tx_Challenges_TrainerScalingEVs && !FlagGet(FLAG_IS_CHAMPION))
        {
            u8 ev = GetCurrentTrainerEVs();

            for (i = 0; i < monsCount; i++)
            {
                // set EVs for HP, speed and the higher of either attack and defense stat
                SetMonData(&party[i], MON_DATA_HP_EV, &ev);
                SetMonData(&party[i], MON_DATA_SPEED_EV, &ev);
                if (GetMonData(&party[i], MON_DATA_ATK) > GetMonData(&party[i], MON_DATA_SPATK))
                    SetMonData(&party[i], MON_DATA_ATK_EV, &ev);
                else
                    SetMonData(&party[i], MON_DATA_SPATK_EV, &ev);
                if (GetMonData(&party[i], MON_DATA_DEF) > GetMonData(&party[i], MON_DATA_SPDEF))
                    SetMonData(&party[i], MON_DATA_DEF_EV, &ev);
                else
                    SetMonData(&party[i], MON_DATA_SPDEF_EV, &ev);

                CalculateMonStats(&party[i]);
            }
        }

    }

    return trainer->partySize;
}

u16 CanMonEvolve(u16 species, u8 level)
{
    // Check if the Pokemon can evolve through leveling up
    if (gEvolutionTable[species][0].method == EVO_LEVEL && gEvolutionTable[species][0].param <= level)
    {
    // Check if the evolved form can evolve further
    u16 evolvedSpecies = gEvolutionTable[species][0].targetSpecies;
        if (CanMonEvolve(evolvedSpecies, level))
        {
        return CanMonEvolve(evolvedSpecies, level);
        }
        return evolvedSpecies;
    }
    // Check if the Pokemon can evolve through an item, friendship, or other methods
    if (level >= 14) {
    switch (gEvolutionTable[species][0].method) {
        case EVO_ITEM:
        case EVO_ITEM_HOLD_DAY:
        case EVO_ITEM_HOLD_NIGHT:
        case EVO_ITEM_MALE:
        case EVO_ITEM_FEMALE:
        case EVO_ITEM_NIGHT:
        case EVO_ITEM_DAY:
        case EVO_ITEM_HOLD:
            if (level >= 20) {
                return gEvolutionTable[species][0].targetSpecies;
            }
            break;
        case EVO_FRIENDSHIP:
        case EVO_FRIENDSHIP_DAY:
        case EVO_FRIENDSHIP_NIGHT:
        case EVO_TRADE_ITEM:
        case EVO_TRADE:
        case EVO_MAPSEC:
        case EVO_SPECIFIC_MAP:
        case EVO_SPECIFIC_MON_IN_PARTY:
        case EVO_MOVE:
        case EVO_LEVEL_DAY:
        case EVO_LEVEL_DUSK:
        case EVO_LEVEL_NIGHT:
        case EVO_LEVEL_NATURE_AMPED:
        case EVO_LEVEL_NATURE_LOW_KEY:
        case EVO_LEVEL_RAIN:
        case EVO_LEVEL_SILCOON:
        case EVO_LEVEL_CASCOON:
        case EVO_LEVEL_NINJASK:
        case EVO_LEVEL_SHEDINJA:
        case EVO_BEAUTY:
        case EVO_CRITICAL_HITS:
        case EVO_LEVEL_DARK_TYPE_MON_IN_PARTY:
        case EVO_SEASONAL:
        case EVO_FRIENDSHIP_MOVE_TYPE:
        case EVO_TRADE_SPECIFIC_MON:
            return gEvolutionTable[species][0].targetSpecies;
        default:
            break;
        }
    }
    // Return 0 if the Pokemon cannot evolve
    return 0;
}

u16 GetPreviousForm(u16 species)
{
    int i, j;

    for (i = 0; i < NUM_SPECIES; i++)
    {
        for (j = 0; j < EVOS_PER_MON; j++)
        {
            if (gEvolutionTable[i][j].targetSpecies == species)
            {
                return i;
            }
        }
    }

    return SPECIES_NONE;
}

u16 CheckEvolve(u16 evo, u8 lvl)
{
    u16 previousForm, prevEvo, evolvedSpecies;
    // Check if the Pokémon can evolve at its current level
    previousForm = GetPreviousForm(evo);
    if (previousForm != 0) {
        prevEvo = CanMonEvolve(previousForm, lvl);
        if (prevEvo != 0) {
            evolvedSpecies = CanMonEvolve(evo, lvl);
        } else {
            evo = previousForm;
            previousForm = GetPreviousForm(evo);
            if (previousForm != 0) {
                evolvedSpecies = CanMonEvolve(previousForm, lvl);
                if (evolvedSpecies != 0)
                {
                    evolvedSpecies =  evo;
                }
                else {
                    evolvedSpecies = previousForm;
                }
            }
            else {
            evolvedSpecies = evo;
            }
        }
        return evolvedSpecies;
    } else {
        evolvedSpecies = CanMonEvolve(evo, lvl);
        return evolvedSpecies;
    }
}
