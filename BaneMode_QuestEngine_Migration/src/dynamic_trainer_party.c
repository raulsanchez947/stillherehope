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

struct BaneModeMonSet
{
    u16 species;
    u16 item;
    u16 ability;
    u8 nature;
    u16 moves[MAX_MON_MOVES];
};

static const struct BaneModeMonSet sBaneModeRocketMaleTeams[][PARTY_SIZE] =
{
    {
        {SPECIES_NIDOKING, ITEM_LIFE_ORB, ABILITY_SHEER_FORCE, NATURE_MODEST, {MOVE_EARTH_POWER, MOVE_SLUDGE_WAVE, MOVE_ICE_BEAM, MOVE_THUNDERBOLT}},
        {SPECIES_HONCHKROW, ITEM_SCOPE_LENS, ABILITY_MOXIE, NATURE_ADAMANT, {MOVE_BRAVE_BIRD, MOVE_SUCKER_PUNCH, MOVE_NIGHT_SLASH, MOVE_ROOST}},
        {SPECIES_WEEZING, ITEM_BLACK_SLUDGE, ABILITY_LEVITATE, NATURE_BOLD, {MOVE_SLUDGE_BOMB, MOVE_WILL_O_WISP, MOVE_FLAMETHROWER, MOVE_PAIN_SPLIT}},
        {SPECIES_CROBAT, ITEM_HEAVY_DUTY_BOOTS, ABILITY_INNER_FOCUS, NATURE_JOLLY, {MOVE_BRAVE_BIRD, MOVE_CROSS_POISON, MOVE_LEECH_LIFE, MOVE_ROOST}},
        {SPECIES_HOUNDOOM, ITEM_FOCUS_SASH, ABILITY_FLASH_FIRE, NATURE_TIMID, {MOVE_NASTY_PLOT, MOVE_DARK_PULSE, MOVE_FLAMETHROWER, MOVE_SLUDGE_BOMB}},
        {SPECIES_DRAPION, ITEM_SCOPE_LENS, ABILITY_SNIPER, NATURE_JOLLY, {MOVE_CROSS_POISON, MOVE_KNOCK_OFF, MOVE_EARTHQUAKE, MOVE_SWORDS_DANCE}},
    },
    {
        {SPECIES_PERSIAN, ITEM_SILK_SCARF, ABILITY_TECHNICIAN, NATURE_JOLLY, {MOVE_FAKE_OUT, MOVE_KNOCK_OFF, MOVE_PLAY_ROUGH, MOVE_DOUBLE_EDGE}},
        {SPECIES_MUK, ITEM_ASSAULT_VEST, ABILITY_POISON_TOUCH, NATURE_ADAMANT, {MOVE_GUNK_SHOT, MOVE_KNOCK_OFF, MOVE_ICE_PUNCH, MOVE_SHADOW_SNEAK}},
        {SPECIES_CROBAT, ITEM_HEAVY_DUTY_BOOTS, ABILITY_INNER_FOCUS, NATURE_JOLLY, {MOVE_BRAVE_BIRD, MOVE_CROSS_POISON, MOVE_TAUNT, MOVE_LEECH_LIFE}},
        {SPECIES_HOUNDOOM, ITEM_EXPERT_BELT, ABILITY_FLASH_FIRE, NATURE_MODEST, {MOVE_FLAMETHROWER, MOVE_DARK_PULSE, MOVE_SLUDGE_BOMB, MOVE_OVERHEAT}},
        {SPECIES_SKUNTANK, ITEM_BLACK_SLUDGE, ABILITY_AFTERMATH, NATURE_JOLLY, {MOVE_POISON_JAB, MOVE_SUCKER_PUNCH, MOVE_TAUNT, MOVE_CRUNCH}},
        {SPECIES_NIDOKING, ITEM_EXPERT_BELT, ABILITY_SHEER_FORCE, NATURE_MODEST, {MOVE_EARTH_POWER, MOVE_SLUDGE_WAVE, MOVE_FIRE_BLAST, MOVE_ICE_BEAM}},
    },
};

static const struct BaneModeMonSet sBaneModeRocketFemaleTeams[][PARTY_SIZE] =
{
    {
        {SPECIES_NIDOQUEEN, ITEM_LIFE_ORB, ABILITY_SHEER_FORCE, NATURE_MODEST, {MOVE_EARTH_POWER, MOVE_SLUDGE_WAVE, MOVE_ICE_BEAM, MOVE_THUNDERBOLT}},
        {SPECIES_ARBOK, ITEM_BLACK_SLUDGE, ABILITY_INTIMIDATE, NATURE_JOLLY, {MOVE_GUNK_SHOT, MOVE_CRUNCH, MOVE_EARTHQUAKE, MOVE_GLARE}},
        {SPECIES_MISMAGIUS, ITEM_FOCUS_SASH, ABILITY_LEVITATE, NATURE_TIMID, {MOVE_NASTY_PLOT, MOVE_SHADOW_BALL, MOVE_MYSTICAL_FIRE, MOVE_THUNDERBOLT}},
        {SPECIES_CROBAT, ITEM_HEAVY_DUTY_BOOTS, ABILITY_INNER_FOCUS, NATURE_JOLLY, {MOVE_BRAVE_BIRD, MOVE_CROSS_POISON, MOVE_LEECH_LIFE, MOVE_ROOST}},
        {SPECIES_SKUNTANK, ITEM_BLACK_SLUDGE, ABILITY_AFTERMATH, NATURE_JOLLY, {MOVE_POISON_JAB, MOVE_SUCKER_PUNCH, MOVE_TAUNT, MOVE_FIRE_BLAST}},
        {SPECIES_WEAVILE, ITEM_CHOICE_BAND, ABILITY_PRESSURE, NATURE_JOLLY, {MOVE_KNOCK_OFF, MOVE_ICICLE_CRASH, MOVE_ICE_SHARD, MOVE_LOW_KICK}},
    },
    {
        {SPECIES_GENGAR, ITEM_FOCUS_SASH, ABILITY_CURSED_BODY, NATURE_TIMID, {MOVE_SHADOW_BALL, MOVE_SLUDGE_BOMB, MOVE_THUNDERBOLT, MOVE_DAZZLING_GLEAM}},
        {SPECIES_NIDOQUEEN, ITEM_BLACK_SLUDGE, ABILITY_SHEER_FORCE, NATURE_BOLD, {MOVE_EARTH_POWER, MOVE_SLUDGE_WAVE, MOVE_STEALTH_ROCK, MOVE_ICE_BEAM}},
        {SPECIES_CROBAT, ITEM_HEAVY_DUTY_BOOTS, ABILITY_INNER_FOCUS, NATURE_JOLLY, {MOVE_BRAVE_BIRD, MOVE_CROSS_POISON, MOVE_TAUNT, MOVE_ROOST}},
        {SPECIES_MUK, ITEM_ASSAULT_VEST, ABILITY_POISON_TOUCH, NATURE_ADAMANT, {MOVE_GUNK_SHOT, MOVE_KNOCK_OFF, MOVE_FIRE_PUNCH, MOVE_SHADOW_SNEAK}},
        {SPECIES_MISMAGIUS, ITEM_LIFE_ORB, ABILITY_LEVITATE, NATURE_TIMID, {MOVE_NASTY_PLOT, MOVE_SHADOW_BALL, MOVE_DAZZLING_GLEAM, MOVE_MYSTICAL_FIRE}},
        {SPECIES_WEAVILE, ITEM_HEAVY_DUTY_BOOTS, ABILITY_PRESSURE, NATURE_JOLLY, {MOVE_KNOCK_OFF, MOVE_ICICLE_CRASH, MOVE_ICE_SHARD, MOVE_SWORDS_DANCE}},
    },
};

static bool32 IsBaneModeRocketTrainer(const struct Trainer *trainer)
{
    return trainer->trainerClass == TRAINER_CLASS_TEAM_AQUA
        || trainer->trainerClass == TRAINER_CLASS_TEAM_MAGMA;
}

static bool32 IsBaneModeRouteMinibossTrainer(u16 trainerNum)
{
    switch (trainerNum)
    {
    case TRAINER_BANEMODE_AEG:
    case TRAINER_CALVIN_1:
    case TRAINER_RICK:
    case TRAINER_TIANA:
    case TRAINER_MAY_ROUTE_103_TREECKO:
    case TRAINER_MAY_ROUTE_103_TORCHIC:
    case TRAINER_MAY_ROUTE_103_MUDKIP:
    case TRAINER_MAY_RUSTBORO_TREECKO:
    case TRAINER_MAY_RUSTBORO_TORCHIC:
    case TRAINER_MAY_RUSTBORO_MUDKIP:
    case TRAINER_FOSTER:
    case TRAINER_LUIS:
    case TRAINER_DOMINIK:
    case TRAINER_DOUGLAS:
    case TRAINER_KYLA:
    case TRAINER_ELLIOT_1:
    case TRAINER_DARRIN:
    case TRAINER_TONY_1:
    case TRAINER_DENISE:
    case TRAINER_JEROME:
    case TRAINER_MATTHEW:
    case TRAINER_TARA:
    case TRAINER_DAVID:
    case TRAINER_ALICE:
    case TRAINER_HUEY:
    case TRAINER_EDWARD:
    case TRAINER_JACLYN:
    case TRAINER_EDWIN_1:
    case TRAINER_VICTOR:
    case TRAINER_VICTORIA:
    case TRAINER_VIVI:
    case TRAINER_BRICE:
    case TRAINER_TRENT_1:
    case TRAINER_LARRY:
    case TRAINER_JAYLEN:
    case TRAINER_DILLON:
    case TRAINER_MADELINE_1:
    case TRAINER_LENNY:
    case TRAINER_LUCAS_1:
    case TRAINER_SHANE:
    case TRAINER_TIMOTHY_1:
    case TRAINER_KOICHI:
    case TRAINER_NOB_1:
    case TRAINER_JOEY:
    case TRAINER_JOSE:
    case TRAINER_JERRY_1:
    case TRAINER_ISAAC_1:
    case TRAINER_LYDIA_1:
    case TRAINER_DYLAN_1:
    case TRAINER_ROSE_1:
    case TRAINER_BARNY:
    case TRAINER_WADE:
    case TRAINER_MAY_ROUTE_119_TREECKO:
    case TRAINER_MAY_ROUTE_119_TORCHIC:
    case TRAINER_MAY_ROUTE_119_MUDKIP:
    case TRAINER_COLIN:
    case TRAINER_ROBERT_1:
    case TRAINER_LORENZO:
    case TRAINER_VANESSA:
    case TRAINER_WALTER_1:
    case TRAINER_TAMMY:
    case TRAINER_BANEMODE_ROUTE122_NACHO:
    case TRAINER_BANEMODE_ROUTE122_DOUGLAS:
    case TRAINER_BANEMODE_ROUTE122_SPECIAL:
    case TRAINER_BANEMODE_WINSTRATE_BANE:
    case TRAINER_WENDY:
    case TRAINER_BRAXTON:
    case TRAINER_VIOLET:
    case TRAINER_SPENCER:
    case TRAINER_ROLAND:
    case TRAINER_JENNY_1:
    case TRAINER_NOLEN:
    case TRAINER_STAN:
    case TRAINER_TANYA:
    case TRAINER_BARRY:
    case TRAINER_DEAN:
    case TRAINER_NIKKI:
    case TRAINER_CAMDEN:
    case TRAINER_DONNY:
    case TRAINER_JONAH:
    case TRAINER_ISAIAH_1:
    case TRAINER_KATELYN_1:
    case TRAINER_ALEXA:
    case TRAINER_CHASE:
    case TRAINER_ALLISON:
    case TRAINER_REED:
    case TRAINER_RODNEY:
    case TRAINER_KATIE:
    case TRAINER_SANTIAGO:
    case TRAINER_RICHARD:
    case TRAINER_HERMAN:
    case TRAINER_SUSIE:
    case TRAINER_GILBERT:
    case TRAINER_DANA:
    case TRAINER_RONALD:
    case TRAINER_FRANKLIN:
    case TRAINER_DEBRA:
    case TRAINER_LINDA:
    case TRAINER_JACK:
    case TRAINER_LAUREL:
    case TRAINER_ALEX:
        return TRUE;
    default:
        return FALSE;
    }
}

static bool32 IsBaneModeKaizoTrainer(u16 trainerNum, const struct Trainer *trainer)
{
    if (IsBaneModeRouteMinibossTrainer(trainerNum))
        return TRUE;

    switch (trainer->trainerClass)
    {
    case TRAINER_CLASS_LEADER:
    case TRAINER_CLASS_ELITE_FOUR:
    case TRAINER_CLASS_CHAMPION:
    case TRAINER_CLASS_RIVAL:
    case TRAINER_CLASS_AQUA_LEADER:
    case TRAINER_CLASS_MAGMA_LEADER:
        return TRUE;
    default:
        return trainerNum == TRAINER_PAUL || FlagGet(FLAG_ADVENTURE_STARTED);
    }
}

static u8 GetHighestPlayerPartyLevel(void)
{
    u8 highestLevel = 1;
    s32 i;

    for (i = 0; i < PARTY_SIZE; i++)
    {
        if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES) == SPECIES_NONE)
            continue;
        if (GetMonData(&gPlayerParty[i], MON_DATA_HP) == 0 && GetMonData(&gPlayerParty[i], MON_DATA_IS_EGG))
            continue;

        highestLevel = max(highestLevel, GetMonData(&gPlayerParty[i], MON_DATA_LEVEL));
    }

    return highestLevel;
}

static u8 GetBaneModeTargetTrainerLevel(u16 trainerNum, const struct Trainer *trainer)
{
    u16 targetLevel = GetHighestPlayerPartyLevel();

    if (trainerNum == TRAINER_ARCHIE)
        targetLevel += 1;
    else if (trainerNum == TRAINER_GARY_DEFAULT)
        targetLevel += 2;
    else if (trainer->trainerClass == TRAINER_CLASS_LEADER)
        targetLevel += 2;

    if (targetLevel > MAX_LEVEL)
        targetLevel = MAX_LEVEL;

    return targetLevel;
}

static void NormalizeBaneModePartyLevels(struct Pokemon *party, u8 monsCount, u8 targetLevel)
{
    s32 i;

    for (i = 0; i < monsCount && i < PARTY_SIZE; i++)
    {
        u16 species = GetMonData(&party[i], MON_DATA_SPECIES);
        u32 exp;
        u16 hp;

        if (species == SPECIES_NONE || species == SPECIES_EGG)
            continue;

        exp = gExperienceTables[gSpeciesInfo[species].growthRate][targetLevel];
        SetMonData(&party[i], MON_DATA_EXP, &exp);
        CalculateMonStats(&party[i]);
        hp = GetMonData(&party[i], MON_DATA_MAX_HP);
        SetMonData(&party[i], MON_DATA_HP, &hp);
    }
}

static void GetBaneModeKaizoEvs(u16 species, u8 evs[NUM_STATS])
{
    const struct SpeciesInfo *speciesInfo = &gSpeciesInfo[species];
    bool32 isPhysical = speciesInfo->baseAttack >= speciesInfo->baseSpAttack;
    bool32 isFast = speciesInfo->baseSpeed >= 85;
    bool32 isDefensive = max(speciesInfo->baseDefense, speciesInfo->baseSpDefense) >= max(speciesInfo->baseAttack, speciesInfo->baseSpAttack) + 20;

    memset(evs, 0, sizeof(u8) * NUM_STATS);

    if (isDefensive)
    {
        evs[STAT_HP] = 252;
        if (speciesInfo->baseDefense >= speciesInfo->baseSpDefense)
            evs[STAT_DEF] = 252;
        else
            evs[STAT_SPDEF] = 252;

        if (isPhysical)
            evs[STAT_ATK] = 4;
        else
            evs[STAT_SPATK] = 4;
        return;
    }

    if (isPhysical)
    {
        evs[STAT_ATK] = 252;
        if (isFast)
        {
            evs[STAT_SPEED] = 252;
            evs[STAT_HP] = 4;
        }
        else
        {
            evs[STAT_HP] = 252;
            evs[STAT_SPEED] = 4;
        }
    }
    else
    {
        evs[STAT_SPATK] = 252;
        if (isFast)
        {
            evs[STAT_SPEED] = 252;
            evs[STAT_HP] = 4;
        }
        else
        {
            evs[STAT_HP] = 252;
            evs[STAT_SPEED] = 4;
        }
    }
}

static u16 GetBaneModeKaizoHeldItem(u16 species)
{
    const struct SpeciesInfo *speciesInfo = &gSpeciesInfo[species];
    bool32 isPhysical = speciesInfo->baseAttack >= speciesInfo->baseSpAttack;
    bool32 isFast = speciesInfo->baseSpeed >= 95;
    bool32 isDefensive = max(speciesInfo->baseDefense, speciesInfo->baseSpDefense) >= max(speciesInfo->baseAttack, speciesInfo->baseSpAttack) + 20;

    if (isDefensive)
        return ITEM_LEFTOVERS;
    if (isFast)
        return ITEM_LIFE_ORB;
    if (isPhysical)
        return ITEM_CHOICE_BAND;
    return ITEM_CHOICE_SPECS;
}

static void GetBaneModeRocketEvs(u16 species, u8 evs[NUM_STATS])
{
    const struct SpeciesInfo *speciesInfo = &gSpeciesInfo[species];
    bool32 isPhysical = speciesInfo->baseAttack >= speciesInfo->baseSpAttack;
    bool32 isFast = speciesInfo->baseSpeed >= 100;
    bool32 isDefensive = max(speciesInfo->baseDefense, speciesInfo->baseSpDefense) >= max(speciesInfo->baseAttack, speciesInfo->baseSpAttack) + 20;

    memset(evs, 0, sizeof(u8) * NUM_STATS);

    if (isDefensive)
    {
        evs[STAT_HP] = 252;
        if (speciesInfo->baseDefense >= speciesInfo->baseSpDefense)
            evs[STAT_DEF] = 252;
        else
            evs[STAT_SPDEF] = 252;

        if (isPhysical)
            evs[STAT_ATK] = 4;
        else
            evs[STAT_SPATK] = 4;
        return;
    }

    if (isPhysical)
    {
        evs[STAT_ATK] = 252;
        if (isFast)
        {
            evs[STAT_SPEED] = 252;
            evs[STAT_HP] = 4;
        }
        else
        {
            evs[STAT_HP] = 252;
            evs[STAT_SPEED] = 4;
        }
    }
    else
    {
        evs[STAT_SPATK] = 252;
        if (isFast)
        {
            evs[STAT_SPEED] = 252;
            evs[STAT_HP] = 4;
        }
        else
        {
            evs[STAT_HP] = 252;
            evs[STAT_SPEED] = 4;
        }
    }
}

static void CreateBaneModeCustomMon(
    struct Pokemon *mon,
    const struct BaneModeMonSet *set,
    u8 level,
    u32 personalityHash,
    bool32 preferFemale)
{
    const struct SpeciesInfo *speciesInfo = &gSpeciesInfo[set->species];
    u32 perfectIvs = 0x3FFFFFF;
    u32 personalityValue = preferFemale ? 0x78 : 0x88;
    u16 moves[MAX_MON_MOVES];
    u8 evs[NUM_STATS];
    u32 abilityNum;
    s32 ball = ITEM_DUSK_BALL;

    personalityValue += personalityHash << 8;
    ModifyPersonalityForNature(&personalityValue, set->nature);
    CreateMon(mon, set->species, level, 0, TRUE, personalityValue, OT_ID_RANDOM_NO_SHINY, 0);

    SetMonData(mon, MON_DATA_HELD_ITEM, &set->item);
    moves[0] = set->moves[0];
    moves[1] = set->moves[1];
    moves[2] = set->moves[2];
    moves[3] = set->moves[3];
    for (abilityNum = 0; abilityNum < MAX_MON_MOVES; abilityNum++)
    {
        SetMonData(mon, MON_DATA_MOVE1 + abilityNum, &moves[abilityNum]);
        SetMonData(mon, MON_DATA_PP1 + abilityNum, &gBattleMoves[moves[abilityNum]].pp);
    }

    SetMonData(mon, MON_DATA_IVS, &perfectIvs);
    GetBaneModeRocketEvs(set->species, evs);
    SetMonData(mon, MON_DATA_HP_EV, &evs[STAT_HP]);
    SetMonData(mon, MON_DATA_ATK_EV, &evs[STAT_ATK]);
    SetMonData(mon, MON_DATA_DEF_EV, &evs[STAT_DEF]);
    SetMonData(mon, MON_DATA_SPATK_EV, &evs[STAT_SPATK]);
    SetMonData(mon, MON_DATA_SPDEF_EV, &evs[STAT_SPDEF]);
    SetMonData(mon, MON_DATA_SPEED_EV, &evs[STAT_SPEED]);

    for (abilityNum = 0; abilityNum < ARRAY_COUNT(speciesInfo->abilities); abilityNum++)
    {
        if (speciesInfo->abilities[abilityNum] == set->ability)
            break;
    }
    if (abilityNum >= ARRAY_COUNT(speciesInfo->abilities))
        abilityNum = 0;
    SetMonData(mon, MON_DATA_ABILITY_NUM, &abilityNum);
    SetMonData(mon, MON_DATA_FRIENDSHIP, &(u8){255});
    SetMonData(mon, MON_DATA_POKEBALL, &ball);
    CalculateMonStats(mon);
}

static u8 CreateBaneModeRocketParty(struct Pokemon *party, const struct Trainer *trainer, bool32 firstTrainer, u32 battleTypeFlags)
{
    const struct BaneModeMonSet (*team)[PARTY_SIZE];
    u8 teamCount;
    u8 teamIndex = 0;
    u8 level = max(GetHighestPlayerPartyLevel(), 6);
    bool32 preferFemale = (trainer->encounterMusic_gender & F_TRAINER_FEMALE) != 0;
    u8 monsCount = (battleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS) ? PARTY_SIZE / 2 : PARTY_SIZE;
    s32 i;

    if (firstTrainer == TRUE)
        ZeroEnemyPartyMons();

    if (preferFemale)
    {
        team = sBaneModeRocketFemaleTeams;
        teamCount = ARRAY_COUNT(sBaneModeRocketFemaleTeams);
    }
    else
    {
        team = sBaneModeRocketMaleTeams;
        teamCount = ARRAY_COUNT(sBaneModeRocketMaleTeams);
    }

    if (trainer->partySize > 0)
        teamIndex = trainer->party.NoItemDefaultMoves[0].species % teamCount;

    for (i = 0; i < monsCount; i++)
    {
        u8 slot = i;
        if (battleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
            slot = firstTrainer ? i : i + (PARTY_SIZE / 2);
        CreateBaneModeCustomMon(&party[i], &team[teamIndex][slot], level, GeneratePartyHash(trainer, slot), preferFemale);
    }

    NormalizeBaneModePartyLevels(party, monsCount, GetBaneModeTargetTrainerLevel(TRAINER_NONE, trainer));

    return monsCount;
}

static void ApplyBaneModeKaizoTrainerBuffs(struct Pokemon *party, u8 monsCount, u16 trainerNum, const struct Trainer *trainer)
{
    u32 perfectIvs = 0x3FFFFFF;
    s32 i;

    if (!IsBaneModeKaizoTrainer(trainerNum, trainer))
        return;

    for (i = 0; i < monsCount && i < PARTY_SIZE; i++)
    {
        u16 species = GetMonData(&party[i], MON_DATA_SPECIES);
        u8 evs[NUM_STATS];

        if (species == SPECIES_NONE || species == SPECIES_EGG)
            continue;

        SetMonData(&party[i], MON_DATA_IVS, &perfectIvs);
        GetBaneModeKaizoEvs(species, evs);
        SetMonData(&party[i], MON_DATA_HP_EV, &evs[STAT_HP]);
        SetMonData(&party[i], MON_DATA_ATK_EV, &evs[STAT_ATK]);
        SetMonData(&party[i], MON_DATA_DEF_EV, &evs[STAT_DEF]);
        SetMonData(&party[i], MON_DATA_SPATK_EV, &evs[STAT_SPATK]);
        SetMonData(&party[i], MON_DATA_SPDEF_EV, &evs[STAT_SPDEF]);
        SetMonData(&party[i], MON_DATA_SPEED_EV, &evs[STAT_SPEED]);
        if (GetMonData(&party[i], MON_DATA_HELD_ITEM) == ITEM_NONE)
        {
            u16 heldItem = GetBaneModeKaizoHeldItem(species);
            SetMonData(&party[i], MON_DATA_HELD_ITEM, &heldItem);
        }
        CalculateMonStats(&party[i]);
    }
}

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

        if (IsBaneModeRocketTrainer(trainer))
            return CreateBaneModeRocketParty(party, trainer, firstTrainer, battleTypeFlags);

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
            if (playerPartySize > 0)
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
                    levelRange = (maxLevel >= playerPartyAverageLevel) ? (maxLevel - playerPartyAverageLevel + 1) : 1;
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
                    levelRange = (maxLevel >= playerPartyAverageLevel) ? (maxLevel - playerPartyAverageLevel + 1) : 1;
                    randomOffset = Random() % levelRange;
                    trainerPartyLevel[k] = playerPartyAverageLevel + randomOffset;
                    if (trainerPartyLevel[k] > maxLevel) {
                        trainerPartyLevel[k] = maxLevel;
                    } else if (trainerPartyLevel[k] < minLevel) {
                        trainerPartyLevel[k] = minLevel;
                    }
                // Check if the Pokémon can evolve at its current level
                    CreateMon(&party[i], partyData[i].species, trainerPartyLevel[k], fixedIV, TRUE, personalityValue, OT_ID_RANDOM_NO_SHINY, 0);
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
                    levelRange = (maxLevel >= playerPartyAverageLevel) ? (maxLevel - playerPartyAverageLevel + 1) : 1;
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
                    levelRange = (maxLevel >= playerPartyAverageLevel) ? (maxLevel - playerPartyAverageLevel + 1) : 1;
                    randomOffset = Random() % levelRange;
                    trainerPartyLevel[k] = playerPartyAverageLevel + randomOffset;
                    if (trainerPartyLevel[k] > maxLevel) {
                        trainerPartyLevel[k] = maxLevel;
                    } else if (trainerPartyLevel[k] < minLevel) {
                        trainerPartyLevel[k] = minLevel;
                    }
                // Check if the Pokémon can evolve at its current level
                    CreateMon(&party[i], partyData[i].species, trainerPartyLevel[k], fixedIV, TRUE, personalityValue, OT_ID_RANDOM_NO_SHINY, 0);
                }
                for (j = 0; j < 4; j++)
                {
                    move = GetMonData(&party[i], MON_DATA_MOVE1 + j);
                    if (move == partyData[i].moves[j])
                        continue;
                    SetMonData(&party[i], MON_DATA_MOVE1 + j, &partyData[i].moves[j]);
                    SetMonData(&party[i], MON_DATA_PP1 + j, &gBattleMoves[partyData[i].moves[j]].pp);
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
                    levelRange = (maxLevel >= playerPartyAverageLevel) ? (maxLevel - playerPartyAverageLevel + 1) : 1;
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
                    levelRange = (maxLevel >= playerPartyAverageLevel) ? (maxLevel - playerPartyAverageLevel + 1) : 1;
                    randomOffset = Random() % levelRange;
                    trainerPartyLevel[k] = playerPartyAverageLevel + randomOffset;
                    if (trainerPartyLevel[k] > maxLevel) {
                        trainerPartyLevel[k] = maxLevel;
                    } else if (trainerPartyLevel[k] < minLevel) {
                        trainerPartyLevel[k] = minLevel;
                    }
                // Check if the Pokémon can evolve at its current level
                    CreateMon(&party[i], partyData[i].species, trainerPartyLevel[k], fixedIV, TRUE, personalityValue, OT_ID_RANDOM_NO_SHINY, 0);
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
                    if (gSaveBlock1Ptr->tx_Difficulty >= 2 && gTrainers[trainerNum].partyHard.ItemCustomMoves != NULL) {
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
                    levelRange = (maxLevel >= playerPartyAverageLevel) ? (maxLevel - playerPartyAverageLevel + 1) : 1;
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
                    levelRange = (maxLevel >= playerPartyAverageLevel) ? (maxLevel - playerPartyAverageLevel + 1) : 1;
                    randomOffset = Random() % levelRange;
                    trainerPartyLevel[k] = playerPartyAverageLevel + randomOffset;
                    if (trainerPartyLevel[k] > maxLevel) {
                        trainerPartyLevel[k] = maxLevel;
                    } else if (trainerPartyLevel[k] < minLevel) {
                        trainerPartyLevel[k] = minLevel;
                    }
                    }
                    CreateMon(&party[i], partyData[i].species, trainerPartyLevel[k], fixedIV, TRUE, personalityValue, OT_ID_RANDOM_NO_SHINY, 0);
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
                            if (gSaveBlock1Ptr->tx_Random_Trainer) 
                                continue;
                            SetMonData(&party[i], MON_DATA_MOVE1 + j, &partyData[i].moves[j]);
                            SetMonData(&party[i], MON_DATA_PP1 + j, &gBattleMoves[partyData[i].moves[j]].pp);
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

        ApplyBaneModeKaizoTrainerBuffs(party, monsCount, trainerNum, trainer);
        NormalizeBaneModePartyLevels(party, monsCount, GetBaneModeTargetTrainerLevel(trainerNum, trainer));

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
