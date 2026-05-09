#include "global.h"
#include "walda_phrase.h"
#include "string_util.h"
#include "event_data.h"
#include "naming_screen.h"
#include "main.h"
#include "text.h"
#include "new_game.h"
#include "overworld.h"
#include "pokemon_storage_system.h"
#include "field_screen_effect.h"
#include "random.h"
#include "battle_main.h"

static void CB2_HandleGivenGiftCode(void);
static void CB2_HandleGivenSeed(void);

// Create an id for the mystery gift in the enum
// Create the text for the mystery gift
// Add it to sMysteryGiftTextPtrArray using the enum created as a designator

// Mystery Gift Ids. Should make it easier to keep track of mystery gifts
enum 
{
    MYSTERY_GIFT_NONE,
    MYSTERY_GIFT_MAGIKARP_SHINY,             // Lvl 5,  special.
    MYSTERY_GIFT_VICTINI,                    // Lvl 50, 7th gym.
    MYSTERY_GIFT_ROCKRUFF_OWNTEMPO_SHINY,    // Lvl 10, 1st gym.
    MYSTERY_GIFT_MARSHADOW,                  // Lvl 40, 6th gym.
    MYSTERY_GIFT_ABOMASNOW_MEGA,             // Lvl 40, 6th gym.
    MYSTERY_GIFT_BEEDRILL_MEGA,              // Lvl 20, 3rd gym. 
    MYSTERY_GIFT_FALINKS_SHINY,              // Lvl 10, visit Petalburg.
    MYSTERY_GIFT_SKIDDO_SHINY,               // Lvl 10, visit Petalburg.
    MYSTERY_GIFT_RARE_CANDY,                 // rare candy x99
    MYSTERY_GIFT_TAPUBULU,                   // Lvl 40, 6th gym.
    MYSTERY_GIFT_TAPUKOKO,                   // Lvl 40, 6th gym.
    MYSTERY_GIFT_TAPUFINI,                   // Lvl 40, 6th gym.
    MYSTERY_GIFT_TAPULELE,                   // Lvl 40, 6th gym.
    MYSTERY_GIFT_EONTICKET,                  // Catch the other Lati-twin.
    MYSTERY_GIFT_MYSTICTICKET,               // Lvl 50, 8th gym.
    MYSTERY_GIFT_CELEBI,                     // Lvl 30, 4th gym.
    MYSTERY_GIFT_TYPENULL,                   // Lvl 40, 6th gym.
    MYSTERY_GIFT_SNEASEL_HISUI_SHINY,        // Lvl 20, 2nd gym.
    MYSTERY_GIFT_RAIKOU_SHINY,               // Lvl 40, 6th gym.
    MYSTERY_GIFT_SANDY_SHOCKS_SHINY,         // Lvl 40, 6th gym.
    MYSTERY_GIFT_FLUTTER_MANE_SHINY,         // Lvl 40, 6th gym.
    MYSTERY_GIFT_SLITHER_WING_SHINY,         // Lvl 40, 6th gym.
    MYSTERY_GIFT_MEWTWO_MEGA_SHINY,          // Lvl 50, 7th gym.
    MYSTERY_GIFT_LOKIX_SHINY,                // Lvl 20, 3rd gym.
    MYSTERY_GIFT_BANETTE_MEGA,               // Lvl 40, 6th gym. 
    MYSTERY_GIFT_MASTER_BALL,
};

// Mystery Gifts
static const u8 sText_Append[] = _("00");
static const u8 sText_Empty[] = _("");
static const u8 sText_Magikarp[] = _("SHIfRWC"); 
static const u8 sText_Victini[] = _("B89Zal2"); 
static const u8 sText_RockRuffOwnTempo[] = _("Finalm1"); 
static const u8 sText_Marshadow[] = _("4oHru4q"); 
static const u8 sText_Abomasnow[] = _("WmVgfmr"); 
static const u8 sText_Beedrill[] = _("HSrwdwN"); 
static const u8 sText_ShinyFalinks[] = _("mEme567"); 
static const u8 sText_ShinySkiddo[] = _("A6543D7"); 
static const u8 sText_RareCandy[] = _("BaDIdeA"); 
static const u8 sText_TapuBulu[] = _("TNPPX87"); 
static const u8 sText_TapuKoko[] = _("DNPP785"); 
static const u8 sText_TapuFini[] = _("D88PXGB"); 
static const u8 sText_TapuLele[] = _("DNPPXmq"); 
static const u8 sText_EonTicket[] = _("EonTKrb"); 
static const u8 sText_MysticTicket[] = _("OhLUHga"); 
static const u8 sText_Celebi[] = _("TIMe120"); 
static const u8 sText_TypeNull[] = _("TNuLL07"); 
static const u8 sText_Sneasel_H[] = _("HIsuiEL"); 
static const u8 sText_Raikou[] = _("BooM000"); 
static const u8 sText_ShinySandyShocks[] = _("SZIPu2q");
static const u8 sText_ShinyFlutterMane[] = _("12FuUNe");
static const u8 sText_ShinySlitherWings[] = _("WoTPKsz");
static const u8 sText_ShinyMewTwo[] = _("MrKipv7");
static const u8 sText_ShinyLokix[] = _("BUgS87p");
static const u8 sText_BanetteMega[] = _("GAulk66");
static const u8 sText_Masterball[] = _("-------");

// Array of pointers to the mystery gift text
static const u8 *const sMysteryGiftTextPtrArray[] =
{
    [MYSTERY_GIFT_NONE]                         = sText_Empty,
    [MYSTERY_GIFT_MAGIKARP_SHINY]               = sText_Magikarp,
    [MYSTERY_GIFT_VICTINI]                      = sText_Victini,
    [MYSTERY_GIFT_ROCKRUFF_OWNTEMPO_SHINY]      = sText_RockRuffOwnTempo,
    [MYSTERY_GIFT_MARSHADOW]                    = sText_Marshadow,
    [MYSTERY_GIFT_ABOMASNOW_MEGA]               = sText_Abomasnow,
    [MYSTERY_GIFT_BEEDRILL_MEGA]                = sText_Beedrill,
    [MYSTERY_GIFT_FALINKS_SHINY]                = sText_ShinyFalinks,
    [MYSTERY_GIFT_SKIDDO_SHINY]                 = sText_ShinySkiddo,
    [MYSTERY_GIFT_RARE_CANDY]                   = sText_RareCandy,
    [MYSTERY_GIFT_TAPUBULU]                     = sText_TapuBulu,
    [MYSTERY_GIFT_TAPUKOKO]                     = sText_TapuKoko,
    [MYSTERY_GIFT_TAPUFINI]                     = sText_TapuFini,
    [MYSTERY_GIFT_TAPULELE]                     = sText_TapuLele,
    [MYSTERY_GIFT_EONTICKET]                    = sText_EonTicket,
    [MYSTERY_GIFT_MYSTICTICKET]                 = sText_MysticTicket,
    [MYSTERY_GIFT_CELEBI]                       = sText_Celebi,
    [MYSTERY_GIFT_TYPENULL]                     = sText_TypeNull,
    [MYSTERY_GIFT_SNEASEL_HISUI_SHINY]          = sText_Sneasel_H,
    [MYSTERY_GIFT_RAIKOU_SHINY]                 = sText_Raikou,
    [MYSTERY_GIFT_SANDY_SHOCKS_SHINY]           = sText_ShinySandyShocks,
    [MYSTERY_GIFT_FLUTTER_MANE_SHINY]           = sText_ShinyFlutterMane,
    [MYSTERY_GIFT_SLITHER_WING_SHINY]           = sText_ShinySlitherWings,
    [MYSTERY_GIFT_MEWTWO_MEGA_SHINY]            = sText_ShinyMewTwo,
    [MYSTERY_GIFT_LOKIX_SHINY]                  = sText_ShinyLokix,
    [MYSTERY_GIFT_BANETTE_MEGA]                 = sText_BanetteMega,
    [MYSTERY_GIFT_MASTER_BALL]                  = sText_Masterball,
};
  
void DoGiftNamingScreen(void)
{
    DoNamingScreen(NAMING_SCREEN_GIFT, gStringVar2, 0, 0, 0, CB2_HandleGivenGiftCode);
}

void DoSeedNamingScreen(void)
{
    DoNamingScreen(NAMING_SCREEN_SEED, gStringVar2, 0, 0, 0, CB2_HandleGivenSeed);
}

static void CB2_HandleGivenSeed(void)
{
    u8 i;
    u16 value;
    if (!StringCompare(gStringVar2, sText_Empty)) {
        value = gStringVar2[0] + gStringVar2[1] + gStringVar2[2] + gStringVar2[3];
        gSaveBlock1Ptr->SeedForShuffle[4] = value;
        gSaveBlock1Ptr->checks = TRUE;
    }
    gFieldCallback = FieldCB_ContinueScriptHandleMusic;
    SetMainCallback2(CB2_ReturnToField);
}

static void CB2_HandleGivenGiftCode(void)
{
    gFieldCallback = FieldCB_ContinueScriptHandleMusic;
    SetMainCallback2(CB2_ReturnToField);
}

#define MAX_SPECIAL_MYSTERY_GIFTS 100

void GetCodeInputData(void)
{ 
    u32 i, j;
    s32 seed;
    s32 value;
    s32 shinyValue;
    bool8 preDefinedCode = FALSE;
    bool8 speciesAlreadyStored = FALSE;
    bool8 shiny = FALSE;

    // Check for predefined codes
    for (i = 0; i < ARRAY_COUNT(sMysteryGiftTextPtrArray); i++)
    {
        if (!StringCompare(gStringVar2, sMysteryGiftTextPtrArray[i]))
        {
            gSpecialVar_Unused_0x8014 = i;
            preDefinedCode = TRUE;
            break;
        }          
    }
    
    if (!preDefinedCode)
    {           
        for (i = 0; i < NUM_SPECIES; i++)
        {
            seed = RandomSeededUni(i);
            seed = abs(seed);

            if (seed < 10000) {
                value = seed * 10;
            } else {
                value = seed;
            }

            // Generate the shiny value
            shinyValue = value * 15;

            ConvertIntToDecimalStringN(gStringVar1, value, STR_CONV_MODE_RIGHT_ALIGN, 5);
            ConvertIntToDecimalStringN(gStringVar3, shinyValue, STR_CONV_MODE_RIGHT_ALIGN, 6); // gStringVar3 for shiny value comparison

            BadgeCount();
            if (gSaveBlock2Ptr->badges >= 6) 
            {            
                if (!StringCompare(gStringVar2, gStringVar1) || !StringCompare(gStringVar2, gStringVar3)) 
                {
                    // Check if the species is already stored            
                    for (j = 0; j < gSaveBlock2Ptr->specialMysteryGiftCount; j++) 
                    {
                        if (gSaveBlock2Ptr->SpecialMysteryGifts[j] == i) 
                        {
                            speciesAlreadyStored = TRUE;
                            break;
                        }
                    }

                    // If the species is not already stored and there's space, add it
                    if (!speciesAlreadyStored && gSaveBlock2Ptr->specialMysteryGiftCount < MAX_SPECIAL_MYSTERY_GIFTS) 
                    {
                        gSaveBlock2Ptr->SpecialMysteryGifts[gSaveBlock2Ptr->specialMysteryGiftCount] = i;
                        gSaveBlock2Ptr->specialMysteryGiftCount++;

                        // Determine if the entered code is for a shiny species
                        if (!StringCompare(gStringVar2, gStringVar3)) 
                        {
                            shiny = TRUE;
                            FlagSet(FLAG_SHINY_CREATION);
                        }

                        VarSet(VAR_TEMP_6, i);
                        gSpecialVar_Unused_0x8014 = 27;
                    }
                    else if (speciesAlreadyStored) 
                    {
                        gSpecialVar_Unused_0x8014 = 28;
                    } 
                    else 
                    {
                        gSpecialVar_Unused_0x8014 = 28;
                    }
                    break;
                }
            } 
            else 
            {
                gSpecialVar_Unused_0x8014 = 28;
                break;
            }
        }
    }
}

u16 RandomSeededSpotlight(u16 value)
{
    u16 result;

    result = ISO_RANDOMIZE1(value) >> 16;
    return result;
}

extern struct Evolution gEvolutionTable[][EVOS_PER_MON];

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
    SPECIES_HOOPA,
    SPECIES_RESHIRAM,
    SPECIES_ZEKROM,
    SPECIES_KYUREM,
    SPECIES_DARKRAI,
    SPECIES_LANDORUS,
    SPECIES_THUNDURUS,
    SPECIES_TORNADUS,
    SPECIES_PIKACHU_COSPLAY   ,
    SPECIES_PIKACHU_ROCK_STAR ,
    SPECIES_PIKACHU_BELLE     ,
    SPECIES_PIKACHU_POP_STAR  ,
    SPECIES_PIKACHU_PH_D      ,
    SPECIES_PIKACHU_LIBRE     ,
    SPECIES_PIKACHU_ORIGINAL_CAP ,
    SPECIES_PIKACHU_HOENN_CAP ,
    SPECIES_PIKACHU_SINNOH_CAP ,
    SPECIES_PIKACHU_UNOVA_CAP ,
    SPECIES_PIKACHU_KALOS_CAP ,
    SPECIES_PIKACHU_ALOLA_CAP ,
    SPECIES_PIKACHU_PARTNER_CAP ,
    SPECIES_PIKACHU_WORLD_CAP ,
    SPECIES_GRENINJA_BATTLE_BOND ,
    SPECIES_GRENINJA_ASH      ,
    SPECIES_VIVILLON_POLAR    ,
    SPECIES_VIVILLON_TUNDRA   ,
    SPECIES_VIVILLON_CONTINENTAL ,
    SPECIES_VIVILLON_GARDEN   ,
    SPECIES_VIVILLON_ELEGANT  ,
    SPECIES_VIVILLON_MEADOW   ,
    SPECIES_VIVILLON_MODERN   ,
    SPECIES_VIVILLON_MARINE   ,
    SPECIES_VIVILLON_ARCHIPELAGO ,
    SPECIES_VIVILLON_HIGH_PLAINS ,
    SPECIES_VIVILLON_SANDSTORM ,
    SPECIES_VIVILLON_RIVER    ,
    SPECIES_VIVILLON_MONSOON  ,
    SPECIES_VIVILLON_SAVANNA  ,
    SPECIES_VIVILLON_SUN      ,
    SPECIES_VIVILLON_OCEAN    ,
    SPECIES_VIVILLON_JUNGLE   ,
    SPECIES_VIVILLON_FANCY    ,
    SPECIES_VIVILLON_POKE_BALL ,
    SPECIES_FLABEBE_YELLOW_FLOWER ,
    SPECIES_FLABEBE_ORANGE_FLOWER ,
    SPECIES_FLABEBE_BLUE_FLOWER ,
    SPECIES_FLABEBE_WHITE_FLOWER ,
    SPECIES_FLOETTE_YELLOW_FLOWER ,
    SPECIES_FLOETTE_ORANGE_FLOWER ,
    SPECIES_FLOETTE_BLUE_FLOWER ,
    SPECIES_FLOETTE_WHITE_FLOWER ,
    SPECIES_FLOETTE_ETERNAL_FLOWER ,
    SPECIES_FLORGES_YELLOW_FLOWER ,
    SPECIES_FLORGES_ORANGE_FLOWER ,
    SPECIES_FLORGES_BLUE_FLOWER ,
    SPECIES_FLORGES_WHITE_FLOWER ,
    SPECIES_FURFROU_HEART_TRIM ,
    SPECIES_FURFROU_STAR_TRIM ,
    SPECIES_FURFROU_DIAMOND_TRIM ,
    SPECIES_FURFROU_DEBUTANTE_TRIM ,
    SPECIES_FURFROU_MATRON_TRIM ,
    SPECIES_FURFROU_DANDY_TRIM ,
    SPECIES_FURFROU_LA_REINE_TRIM ,
    SPECIES_FURFROU_KABUKI_TRIM ,
    SPECIES_FURFROU_PHARAOH_TRIM ,
    SPECIES_RATTATA_ALOLAN    ,
    SPECIES_RATICATE_ALOLAN   ,
    SPECIES_RAICHU_ALOLAN     ,
    SPECIES_SANDSHREW_ALOLAN  ,
    SPECIES_SANDSLASH_ALOLAN  ,
    SPECIES_VULPIX_ALOLAN     ,
    SPECIES_NINETALES_ALOLAN  ,
    SPECIES_DIGLETT_ALOLAN    ,
    SPECIES_DUGTRIO_ALOLAN    ,
    SPECIES_MEOWTH_ALOLAN     ,
    SPECIES_PERSIAN_ALOLAN    ,
    SPECIES_GEODUDE_ALOLAN    ,
    SPECIES_GRAVELER_ALOLAN   ,
    SPECIES_GOLEM_ALOLAN      ,
    SPECIES_GRIMER_ALOLAN     ,
    SPECIES_MUK_ALOLAN        ,
    SPECIES_EXEGGUTOR_ALOLAN  ,
    SPECIES_MAROWAK_ALOLAN    ,
    SPECIES_MEOWTH_GALARIAN   ,
    SPECIES_PONYTA_GALARIAN   ,
    SPECIES_RAPIDASH_GALARIAN ,
    SPECIES_SLOWPOKE_GALARIAN ,
    SPECIES_SLOWBRO_GALARIAN  ,
    SPECIES_FARFETCHD_GALARIAN ,
    SPECIES_WEEZING_GALARIAN  ,
    SPECIES_MR_MIME_GALARIAN  ,
    SPECIES_ARTICUNO_GALARIAN ,
    SPECIES_ZAPDOS_GALARIAN   ,
    SPECIES_MOLTRES_GALARIAN  ,
    SPECIES_SLOWKING_GALARIAN ,
    SPECIES_CORSOLA_GALARIAN  ,
    SPECIES_ZIGZAGOON_GALARIAN ,
    SPECIES_LINOONE_GALARIAN  ,
    SPECIES_DARUMAKA_GALARIAN ,
    SPECIES_DARMANITAN_GALARIAN ,
    SPECIES_YAMASK_GALARIAN   ,
    SPECIES_STUNFISK_GALARIAN ,
    SPECIES_GROWLITHE_HISUIAN,
    SPECIES_ARCANINE_HISUIAN,             
    SPECIES_VOLTORB_HISUIAN,              
    SPECIES_ELECTRODE_HISUIAN,            
    SPECIES_TYPHLOSION_HISUIAN,           
    SPECIES_QWILFISH_HISUIAN,
    SPECIES_SNEASLER,            
    SPECIES_SNEASEL_HISUIAN,            
    SPECIES_SAMUROTT_HISUIAN,             
    SPECIES_LILLIGANT_HISUIAN,            
    SPECIES_ZORUA_HISUIAN,                
    SPECIES_ZOROARK_HISUIAN,             
    SPECIES_BRAVIARY_HISUIAN,             
    SPECIES_SLIGGOO_HISUIAN,            
    SPECIES_GOODRA_HISUIAN,               
    SPECIES_AVALUGG_HISUIAN,
    SPECIES_DECIDUEYE_HISUIAN,
    SPECIES_TAUROS_PALDEAN  ,
    SPECIES_TAUROS_PALDEAN_AQUA_BREED  ,
    SPECIES_TAUROS_PALDEAN_BLAZE_BREED  ,
    SPECIES_DIPPLIN,
    SPECIES_POLTCHAGEIST_COUNTERFEIT,
    SPECIES_POLTCHAGEIST_ARTISAN,
    SPECIES_SINISTCHA_UNREMARKABLE,
    SPECIES_SINISTCHA_MASTERPIECE,
    SPECIES_OKIDOGI,
    SPECIES_MUNKIDORI,
    SPECIES_FEZANDIPITI,
    SPECIES_OGERPON,
    SPECIES_OGERPON_WELLSPRING,
    SPECIES_OGERPON_HEARTHFLAME,
    SPECIES_OGERPON_CORNERSTONE,
    SPECIES_URSALUNA_BLOODMOON,
    SPECIES_WALKING_WAKE,
    SPECIES_IRON_LEAVES,
    SPECIES_ARCHALUDON,
    SPECIES_HYDRAPPLE,
    SPECIES_GOUGING_FIRE,
    SPECIES_RAGING_BOLT,
    SPECIES_IRON_BOULDER,
    SPECIES_IRON_CROWN,
    SPECIES_TERAPAGOS,
    SPECIES_TERAPAGOS_TERASTAL,
    SPECIES_TERAPAGOS_STELLAR,
    SPECIES_PECHARUNT,
};

void GenerateSpotlightSpeciesRHS(void)
{
    u16 species; 
    s32 seed, value;
    u8 i;

    for (species = 0; species < NUM_SPECIES; species++)
    {

        seed = RandomSeededSpotlight(species);

        seed = abs(seed);

        if (seed <= 9999) 
        { 
            if (seed <= 999)
                value = seed * 100;
            else
                value = seed * 10;
        } 
        else 
        {
            value = seed;
        }

        if (gSpeciesInfo[species].flags & (SPECIES_FLAG_LEGENDARY | SPECIES_FLAG_MYTHICAL | SPECIES_FLAG_PARADOX))
            value -= 254;

        ConvertIntToDecimalStringN(gStringVar1, value, STR_CONV_MODE_RIGHT_ALIGN, 5);

        if (!StringCompare(gStringVar2, gStringVar1)) {          
            // if (GetPreviousForm(species) != SPECIES_NONE) {
            //     break;
            // }
            // for(i = 0; i < ARRAY_COUNT(Legends); i++) {
            //     if (species == Legends[i]) {
            //         gSpecialVar_Unused_0x8014 = species;
            //         FlagSet(FLAG_GENERATED_SPOTLIGHT_EVENT);
            //         break;
            //     }
            // }
            // switch (gSaveBlock2Ptr->badges) {
            //     case 0:
            //         if (species >= 152) { // gen 1
            //             break;
            //         }
            //         gSpecialVar_Unused_0x8014 = species;
            //         FlagSet(FLAG_GENERATED_SPOTLIGHT_EVENT);
            //         break;
            //     case 1:
            //         if (species >= 252) { // gen 2
            //             break;
            //         }
            //         gSpecialVar_Unused_0x8014 = species;
            //         FlagSet(FLAG_GENERATED_SPOTLIGHT_EVENT);
            //         break;
            //     case 2:
            //         if (species >= 387) { // gen 3
            //             break;
            //         }
            //         gSpecialVar_Unused_0x8014 = species;
            //         FlagSet(FLAG_GENERATED_SPOTLIGHT_EVENT);
            //         break;
            //     case 3:
            //         if (species >= 494) { // gen 4
            //             break;
            //         }
            //         gSpecialVar_Unused_0x8014 = species;
            //         FlagSet(FLAG_GENERATED_SPOTLIGHT_EVENT);
            //         break;
            //     case 4:
            //         if (species >= 650) { // gen 5
            //             break;
            //         }
            //         gSpecialVar_Unused_0x8014 = species;
            //         FlagSet(FLAG_GENERATED_SPOTLIGHT_EVENT);
            //         break;
            //     case 5:
            //         if (species >= 722) { // gen 6
            //             break;
            //         }
            //         gSpecialVar_Unused_0x8014 = species;
            //         FlagSet(FLAG_GENERATED_SPOTLIGHT_EVENT);
            //         break;
            //     case 6:
            //         if (species >= 810) { // gen 7
            //             break;
            //         }
            //         gSpecialVar_Unused_0x8014 = species;
            //         FlagSet(FLAG_GENERATED_SPOTLIGHT_EVENT);
            //         break;
            //     case 7:
            //         if (species >= 906) { // gen 8
            //             break;
            //         }
            //         gSpecialVar_Unused_0x8014 = species;
            //         FlagSet(FLAG_GENERATED_SPOTLIGHT_EVENT);
            //         break;
            //     case 8:
            //         if (species >= 1010) { // gen 9
            //             break;
            //         }
            if (!(gSpeciesInfo[species].flags & (SPECIES_FLAG_MEGA_EVOLUTION | SPECIES_FLAG_ULTRA_BURST | SPECIES_FLAG_PRIMAL_REVERSION | SPECIES_FLAG_CANNOT_BE_TRADED))  || FlagGet(FLAG_ROAMING_MON)) {
                gSpecialVar_Unused_0x8014 = species;
                FlagSet(FLAG_GENERATED_SPOTLIGHT_EVENT);
            }
            //         break;
            //     }
            // break;
        }
    }
    gFieldCallback = FieldCB_ContinueScriptHandleMusic;
    SetMainCallback2(CB2_ReturnToField);
}

void DoSpotlightNamingScreen(void)
{
    DoNamingScreen(NAMING_SCREEN_SPOTLIGHT, gStringVar2, 0, 0, 0, GenerateSpotlightSpeciesRHS);
}
