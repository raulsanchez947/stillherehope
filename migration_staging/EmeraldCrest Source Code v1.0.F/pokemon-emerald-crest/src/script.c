#include "global.h"
#include "script.h"
#include "event_data.h"
#include "trainer_see.h"
#include "mystery_gift.h"
#include "util.h"
#include "constants/event_objects.h"
#include "constants/region_map_sections.h"
#include "constants/map_scripts.h"
#include "constants/species.h"
#include "field_specials.h"

#define RAM_SCRIPT_MAGIC 51

enum {
    SCRIPT_MODE_STOPPED,
    SCRIPT_MODE_BYTECODE,
    SCRIPT_MODE_NATIVE,
};

enum {
    CONTEXT_RUNNING,
    CONTEXT_WAITING,
    CONTEXT_SHUTDOWN,
};

extern const u8 *gRamScriptRetAddr;

static u8 sGlobalScriptContextStatus;
static struct ScriptContext sGlobalScriptContext;
static struct ScriptContext sImmediateScriptContext;
static bool8 sLockFieldControls;

extern ScrCmdFunc gScriptCmdTable[];
extern ScrCmdFunc gScriptCmdTableEnd[];
extern void *gNullScriptPtr;

void InitScriptContext(struct ScriptContext *ctx, void *cmdTable, void *cmdTableEnd)
{
    s32 i;

    ctx->mode = SCRIPT_MODE_STOPPED;
    ctx->scriptPtr = NULL;
    ctx->stackDepth = 0;
    ctx->nativePtr = NULL;
    ctx->cmdTable = cmdTable;
    ctx->cmdTableEnd = cmdTableEnd;

    for (i = 0; i < (int)ARRAY_COUNT(ctx->data); i++)
        ctx->data[i] = 0;

    for (i = 0; i < (int)ARRAY_COUNT(ctx->stack); i++)
        ctx->stack[i] = NULL;
}

u8 SetupBytecodeScript(struct ScriptContext *ctx, const u8 *ptr)
{
    ctx->scriptPtr = ptr;
    ctx->mode = SCRIPT_MODE_BYTECODE;
    return 1;
}

void SetupNativeScript(struct ScriptContext *ctx, bool8 (*ptr)(void))
{
    ctx->mode = SCRIPT_MODE_NATIVE;
    ctx->nativePtr = ptr;
}

void StopScript(struct ScriptContext *ctx)
{
    ctx->mode = SCRIPT_MODE_STOPPED;
    ctx->scriptPtr = NULL;
}

bool8 RunScriptCommand(struct ScriptContext *ctx)
{
    if (ctx->mode == SCRIPT_MODE_STOPPED)
        return FALSE;

    switch (ctx->mode)
    {
    case SCRIPT_MODE_STOPPED:
        return FALSE;
    case SCRIPT_MODE_NATIVE:
        // Try to call a function in C
        // Continue to bytecode if no function or it returns TRUE
        if (ctx->nativePtr)
        {
            if (ctx->nativePtr() == TRUE)
                ctx->mode = SCRIPT_MODE_BYTECODE;
            return TRUE;
        }
        ctx->mode = SCRIPT_MODE_BYTECODE;
        // fallthrough
    case SCRIPT_MODE_BYTECODE:
        while (1)
        {
            u8 cmdCode;
            ScrCmdFunc *func;

            if (!ctx->scriptPtr)
            {
                ctx->mode = SCRIPT_MODE_STOPPED;
                return FALSE;
            }

            if (ctx->scriptPtr == gNullScriptPtr)
            {
                while (1)
                    asm("svc 2"); // HALT
            }

            cmdCode = *(ctx->scriptPtr);
            ctx->scriptPtr++;
            func = &ctx->cmdTable[cmdCode];

            if (func >= ctx->cmdTableEnd)
            {
                ctx->mode = SCRIPT_MODE_STOPPED;
                return FALSE;
            }

            if ((*func)(ctx) == TRUE)
                return TRUE;
        }
    }

    return TRUE;
}

static bool8 ScriptPush(struct ScriptContext *ctx, const u8 *ptr)
{
    if (ctx->stackDepth + 1 >= (int)ARRAY_COUNT(ctx->stack))
    {
        return TRUE;
    }
    else
    {
        ctx->stack[ctx->stackDepth] = ptr;
        ctx->stackDepth++;
        return FALSE;
    }
}

static const u8 *ScriptPop(struct ScriptContext *ctx)
{
    if (ctx->stackDepth == 0)
        return NULL;

    ctx->stackDepth--;
    return ctx->stack[ctx->stackDepth];
}

void ScriptJump(struct ScriptContext *ctx, const u8 *ptr)
{
    ctx->scriptPtr = ptr;
}

void ScriptCall(struct ScriptContext *ctx, const u8 *ptr)
{
    ScriptPush(ctx, ctx->scriptPtr);
    ctx->scriptPtr = ptr;
}

void ScriptReturn(struct ScriptContext *ctx)
{
    ctx->scriptPtr = ScriptPop(ctx);
}

u16 ScriptReadHalfword(struct ScriptContext *ctx)
{
    u16 value = *(ctx->scriptPtr++);
    value |= *(ctx->scriptPtr++) << 8;
    return value;
}

u32 ScriptReadWord(struct ScriptContext *ctx)
{
    u32 value0 = *(ctx->scriptPtr++);
    u32 value1 = *(ctx->scriptPtr++);
    u32 value2 = *(ctx->scriptPtr++);
    u32 value3 = *(ctx->scriptPtr++);
    return (((((value3 << 8) + value2) << 8) + value1) << 8) + value0;
}

u32 ScriptPeekWord(struct ScriptContext *ctx)
{
    u32 value0 = *(ctx->scriptPtr);
    u32 value1 = *(ctx->scriptPtr + 1);
    u32 value2 = *(ctx->scriptPtr + 2);
    u32 value3 = *(ctx->scriptPtr + 3);
    return (((((value3 << 8) + value2) << 8) + value1) << 8) + value0;
}

void LockPlayerFieldControls(void)
{
    sLockFieldControls = TRUE;
}

void UnlockPlayerFieldControls(void)
{
    sLockFieldControls = FALSE;
}

bool8 ArePlayerFieldControlsLocked(void)
{
    return sLockFieldControls;
}

// The ScriptContext_* functions work with the primary script context,
// which yields control back to native code should the script make a wait call.

// Checks if the global script context is able to be run right now.
bool8 ScriptContext_IsEnabled(void)
{
    if (sGlobalScriptContextStatus == CONTEXT_RUNNING)
        return TRUE;
    else
        return FALSE;
}

// Re-initializes the global script context to zero.
void ScriptContext_Init(void)
{
    InitScriptContext(&sGlobalScriptContext, gScriptCmdTable, gScriptCmdTableEnd);
    sGlobalScriptContextStatus = CONTEXT_SHUTDOWN;
}

// Runs the script until the script makes a wait* call, then returns true if
// there's more script to run, or false if the script has hit the end.
// This function also returns false if the context is finished
// or waiting (after a call to _Stop)
bool8 ScriptContext_RunScript(void)
{
    if (sGlobalScriptContextStatus == CONTEXT_SHUTDOWN)
        return FALSE;

    if (sGlobalScriptContextStatus == CONTEXT_WAITING)
        return FALSE;

    LockPlayerFieldControls();

    if (!RunScriptCommand(&sGlobalScriptContext))
    {
        sGlobalScriptContextStatus = CONTEXT_SHUTDOWN;
        UnlockPlayerFieldControls();
        return FALSE;
    }

    return TRUE;
}

// Sets up a new script in the global context and enables the context
void ScriptContext_SetupScript(const u8 *ptr)
{
    InitScriptContext(&sGlobalScriptContext, gScriptCmdTable, gScriptCmdTableEnd);
    SetupBytecodeScript(&sGlobalScriptContext, ptr);
    LockPlayerFieldControls();
    sGlobalScriptContextStatus = CONTEXT_RUNNING;
}

// Puts the script into waiting mode; usually called from a wait* script command.
void ScriptContext_Stop(void)
{
    sGlobalScriptContextStatus = CONTEXT_WAITING;
}

// Puts the script into running mode.
void ScriptContext_Enable(void)
{
    sGlobalScriptContextStatus = CONTEXT_RUNNING;
    LockPlayerFieldControls();
}

// Sets up and runs a script in its own context immediately. The script will be
// finished when this function returns. Used mainly by all of the map header
// scripts (except the frame table scripts).
void RunScriptImmediately(const u8 *ptr)
{
    InitScriptContext(&sImmediateScriptContext, gScriptCmdTable, gScriptCmdTableEnd);
    SetupBytecodeScript(&sImmediateScriptContext, ptr);
    while (RunScriptCommand(&sImmediateScriptContext) == TRUE);
}

u8 *MapHeaderGetScriptTable(u8 tag)
{
    const u8 *mapScripts = gMapHeader.mapScripts;

    if (!mapScripts)
        return NULL;

    while (1)
    {
        if (!*mapScripts)
            return NULL;
        if (*mapScripts == tag)
        {
            mapScripts++;
            return T2_READ_PTR(mapScripts);
        }
        mapScripts += 5;
    }
}

void MapHeaderRunScriptType(u8 tag)
{
    u8 *ptr = MapHeaderGetScriptTable(tag);
    if (ptr)
        RunScriptImmediately(ptr);
}

u8 *MapHeaderCheckScriptTable(u8 tag)
{
    u8 *ptr = MapHeaderGetScriptTable(tag);

    if (!ptr)
        return NULL;

    while (1)
    {
        u16 varIndex1;
        u16 varIndex2;

        // Read first var (or .2byte terminal value)
        varIndex1 = T1_READ_16(ptr);
        if (!varIndex1)
            return NULL; // Reached end of table
        ptr += 2;

        // Read second var
        varIndex2 = T1_READ_16(ptr);
        ptr += 2;

        // Run map script if vars are equal
        if (VarGet(varIndex1) == VarGet(varIndex2))
            return T2_READ_PTR(ptr);
        ptr += 4;
    }
}

void RunOnLoadMapScript(void)
{
    MapHeaderRunScriptType(MAP_SCRIPT_ON_LOAD);
}

void RunOnTransitionMapScript(void)
{
    MapHeaderRunScriptType(MAP_SCRIPT_ON_TRANSITION);
}

void RunOnResumeMapScript(void)
{
    MapHeaderRunScriptType(MAP_SCRIPT_ON_RESUME);
}

void RunOnReturnToFieldMapScript(void)
{
    MapHeaderRunScriptType(MAP_SCRIPT_ON_RETURN_TO_FIELD);
}

void RunOnDiveWarpMapScript(void)
{
    MapHeaderRunScriptType(MAP_SCRIPT_ON_DIVE_WARP);
}

bool8 TryRunOnFrameMapScript(void)
{
    u8 *ptr = MapHeaderCheckScriptTable(MAP_SCRIPT_ON_FRAME_TABLE);

    if (!ptr)
        return FALSE;

    ScriptContext_SetupScript(ptr);
    return TRUE;
}

void TryRunOnWarpIntoMapScript(void)
{
    u8 *ptr = MapHeaderCheckScriptTable(MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE);
    if (ptr)
        RunScriptImmediately(ptr);
}

u32 CalculateRamScriptChecksum(void)
{
    return CalcCRC16WithTable((u8 *)(&gSaveBlock1Ptr->ramScript.data), sizeof(gSaveBlock1Ptr->ramScript.data));
}

void ClearRamScript(void)
{
    CpuFill32(0, &gSaveBlock1Ptr->ramScript, sizeof(struct RamScript));
}

bool8 InitRamScript(const u8 *script, u16 scriptSize, u8 mapGroup, u8 mapNum, u8 objectId)
{
    struct RamScriptData *scriptData = &gSaveBlock1Ptr->ramScript.data;

    ClearRamScript();

    if (scriptSize > sizeof(scriptData->script))
        return FALSE;

    scriptData->magic = RAM_SCRIPT_MAGIC;
    scriptData->mapGroup = mapGroup;
    scriptData->mapNum = mapNum;
    scriptData->objectId = objectId;
    memcpy(scriptData->script, script, scriptSize);
    gSaveBlock1Ptr->ramScript.checksum = CalculateRamScriptChecksum();
    return TRUE;
}

const u8 *GetRamScript(u8 objectId, const u8 *script)
{
    struct RamScriptData *scriptData = &gSaveBlock1Ptr->ramScript.data;
    gRamScriptRetAddr = NULL;
    if (scriptData->magic != RAM_SCRIPT_MAGIC)
        return script;
    if (scriptData->mapGroup != gSaveBlock1Ptr->location.mapGroup)
        return script;
    if (scriptData->mapNum != gSaveBlock1Ptr->location.mapNum)
        return script;
    if (scriptData->objectId != objectId)
        return script;
    if (CalculateRamScriptChecksum() != gSaveBlock1Ptr->ramScript.checksum)
    {
        ClearRamScript();
        return script;
    }
    else
    {
        gRamScriptRetAddr = script;
        return scriptData->script;
    }
}

#define NO_OBJECT OBJ_EVENT_ID_PLAYER

bool32 ValidateSavedRamScript(void)
{
    struct RamScriptData *scriptData = &gSaveBlock1Ptr->ramScript.data;
    if (scriptData->magic != RAM_SCRIPT_MAGIC)
        return FALSE;
    if (scriptData->mapGroup != MAP_GROUP(UNDEFINED))
        return FALSE;
    if (scriptData->mapNum != MAP_NUM(UNDEFINED))
        return FALSE;
    if (scriptData->objectId != NO_OBJECT)
        return FALSE;
    if (CalculateRamScriptChecksum() != gSaveBlock1Ptr->ramScript.checksum)
        return FALSE;
    return TRUE;
}

u8 *GetSavedRamScriptIfValid(void)
{
    struct RamScriptData *scriptData = &gSaveBlock1Ptr->ramScript.data;
    if (!ValidateSavedWonderCard())
        return NULL;
    if (scriptData->magic != RAM_SCRIPT_MAGIC)
        return NULL;
    if (scriptData->mapGroup != MAP_GROUP(UNDEFINED))
        return NULL;
    if (scriptData->mapNum != MAP_NUM(UNDEFINED))
        return NULL;
    if (scriptData->objectId != NO_OBJECT)
        return NULL;
    if (CalculateRamScriptChecksum() != gSaveBlock1Ptr->ramScript.checksum)
    {
        ClearRamScript();
        return NULL;
    }
    else
    {
        return scriptData->script;
    }
}

void InitRamScript_NoObjectEvent(u8 *script, u16 scriptSize)
{
    if (scriptSize > sizeof(gSaveBlock1Ptr->ramScript.data.script))
        scriptSize = sizeof(gSaveBlock1Ptr->ramScript.data.script);
    InitRamScript(script, scriptSize, MAP_GROUP(UNDEFINED), MAP_NUM(UNDEFINED), NO_OBJECT);
}

bool8 LoadTrainerObjectScript(void)
{
    sGlobalScriptContext.scriptPtr = gApproachingTrainers[gNoOfApproachingTrainers - 1].trainerScriptPtr;
    return TRUE;
}

// Stores the current day, month and year in VAR_0x8001, VAR_0x8002, and VAR_0x8003.
// Also stores the hours, minutes and seconds in VAR_0x8004, VAR_0x8005 and VAR_0x8006 respectively.
// Please note that this might not work as intended on real hardware.
// In scripts, use "callnative(GetCurrentDateAndTime)"
#include "siirtc.h"
#include "rtc.h"
#include "event_data.h"
#include "string_util.h"
void GetCurrentDateAndTime(void)
{
    struct SiiRtcInfo rtc;
    u8 seconds, minutes, hours, day, month, dayOfWeek;
    u16 year;

    RtcGetDateTime(&rtc);

    seconds = ConvertBcdToBinary(rtc.second);
    minutes = ConvertBcdToBinary(rtc.minute);
    hours = ConvertBcdToBinary(rtc.hour);
    day = ConvertBcdToBinary(rtc.day);
    month = ConvertBcdToBinary(rtc.month);
    year = ConvertBcdToBinary(rtc.year) + 2000;
    dayOfWeek = ConvertBcdToBinary(rtc.dayOfWeek);
    // Store regular numbers
    VarSet(VAR_UNUSED_0x40FB, day);
    VarSet(VAR_UNUSED_0x40FC, month);
    VarSet(VAR_UNUSED_0x40FF, year);
    VarSet(VAR_UNUSED_0x40FD, hours);
    VarSet(VAR_UNUSED_0x40E5, minutes);
    VarSet(VAR_UNUSED_0x40DB, seconds);
    VarSet(VAR_UNUSED_0x40DC, dayOfWeek);
    // Store day, month and year as letters in STR_VAR_1, STR_VAR_2, and STR_VAR_3
    ConvertIntToDecimalStringN(gStringVar1, day, STR_CONV_MODE_LEADING_ZEROS, 2);
    ConvertIntToDecimalStringN(gStringVar2, month, STR_CONV_MODE_LEADING_ZEROS, 2);
    ConvertIntToDecimalStringN(gStringVar3, year, STR_CONV_MODE_LEADING_ZEROS, 4);

    return;
}

void GetCurrentSeason(void)
{
    u8 option = 3;
    u16 seasonChange = GetDaysUntilSeasonChange();

    if (gSaveBlock2Ptr->seasons == 1)
        option = 8;

    if (seasonChange == 0 || seasonChange >= option) {
        SetSeasonChangeDay();
        gSaveBlock1Ptr->seasons += 1;

        if (gSaveBlock1Ptr->seasons >= 5)
            gSaveBlock1Ptr->seasons = 0;
    }

    if (gSaveBlock2Ptr->seasons == 2)
        ReturnCurrentSeasonIndex();
    else if (gSaveBlock2Ptr->seasons == 3)
        gSaveBlock1Ptr->seasons = 0;

    return;
}

void ChangeToNextSeason(void)
{
    gSaveBlock1Ptr->seasons += 1;

    if (gSaveBlock1Ptr->seasons >= 5)
         gSaveBlock1Ptr->seasons = 0;
    SetSeasonChangeDay();
    return;    
}

// Spotlight
EWRAM_DATA struct SpotlightSpecies spotlightSpecies[SPOTLIGHT_SPECIES_TOTAL] = {0};

static void InitSpotlightSpeciesData(void)
{
    // Add/replace new spotlight species here and edit SPOTLIGHT_SPECIES_TOTAL in "script.h" to reflect the change.
    // And the order is Day, Month, Year, StartHour, EndHour, Species, Index in array
    SPOTLIGHT_SPECIES(6, 6, 25, 0,  23, SPECIES_DIALGA, 0);
    SPOTLIGHT_SPECIES(10, 6, 25, 0, 23, SPECIES_PALKIA, 1);
    SPOTLIGHT_SPECIES(16, 6, 25, 0, 23, SPECIES_MEW, 2);
    SPOTLIGHT_SPECIES(20, 6, 25, 0, 23, SPECIES_DRIFLOON, 3);
    SPOTLIGHT_SPECIES(27, 6, 25, 0, 23, SPECIES_FROAKIE, 4);
    SPOTLIGHT_SPECIES(30, 6, 25, 0, 23, SPECIES_MEWTWO, 5);
    SPOTLIGHT_SPECIES(1, 7, 25, 0, 23, SPECIES_RAGING_BOLT, 6);
    SPOTLIGHT_SPECIES(15,  7, 25, 0, 23, SPECIES_LATIOS, 7);
    SPOTLIGHT_SPECIES(19, 7, 25, 0, 23, SPECIES_LATIAS, 8)
    SPOTLIGHT_SPECIES(20, 7, 25, 0, 23, SPECIES_REGIDRAGO, 9);
    SPOTLIGHT_SPECIES(26, 7, 25, 0, 23, SPECIES_PANCHAM, 10);
    SPOTLIGHT_SPECIES(27, 7, 25, 0, 23, SPECIES_REGIGIGAS, 11);
    SPOTLIGHT_SPECIES(30, 7, 25, 0, 23, SPECIES_FALINKS, 12);
    SPOTLIGHT_SPECIES(2,  8, 25, 0, 23, SPECIES_TERAPAGOS, 13);
    SPOTLIGHT_SPECIES(5,  8, 25, 0, 23, SPECIES_PECHARUNT, 14);
    SPOTLIGHT_SPECIES(14, 8, 25, 0, 23, SPECIES_COSMOG, 15);
    SPOTLIGHT_SPECIES(23, 8, 25, 0, 23, SPECIES_TING_LU, 16);
    SPOTLIGHT_SPECIES(9, 9, 25, 0, 23, SPECIES_PIKACHU_PH_D, 17);
    SPOTLIGHT_SPECIES(12, 9, 25, 0, 23, SPECIES_LUCARIO, 18);
    SPOTLIGHT_SPECIES(23, 9, 25, 0, 23, SPECIES_RAYQUAZA, 19);
    SPOTLIGHT_SPECIES(4, 10, 25, 0, 23, SPECIES_ROWLET, 20);
    SPOTLIGHT_SPECIES(13, 10, 25, 0, 23, SPECIES_ROTOM, 21);
    SPOTLIGHT_SPECIES(17,  10, 25, 0, 23, SPECIES_MAGEARNA_ORIGINAL_COLOR, 22);
    SPOTLIGHT_SPECIES(25, 10, 25, 0, 23, SPECIES_OKIDOGI, 23);
}

u16 ReturnSpotlightSpecies(void)
{
    u16 spotlightIndex = ReturnSpotlightMonIndex();

    if (spotlightIndex != SPOTLIGHT_NO_POKEMON)
    {
        FlagSet(FLAG_IS_SPOTLIGHT_EVENT);
        VarSet(VAR_UNUSED_0x40A8, 1);
        if (spotlightSpecies[spotlightIndex].spotlightToggle == TRUE)
            return spotlightSpecies[spotlightIndex].species;
    }
    FlagClear(FLAG_IS_SPOTLIGHT_EVENT);
    return SPECIES_NONE;
}

u16 ReturnSpotlightMonIndex(void)
{
    u8 i;
    u16 DAY, MONTH, YEAR, HOUR;
    
    if (!spotlightSpecies[0].isStructInitialized)
    {
        InitSpotlightSpeciesData();
        spotlightSpecies[0].isStructInitialized = TRUE;
    }

    GetCurrentDateAndTime();
    DAY = VarGet(VAR_UNUSED_0x40FB);
    MONTH = VarGet(VAR_UNUSED_0x40FC);
    YEAR = VarGet(VAR_UNUSED_0x40FF) - 2000;
    HOUR = VarGet(VAR_UNUSED_0x40FD);
    
    for (i = 0; i < SPOTLIGHT_SPECIES_TOTAL; i++) {
        if (DAY == spotlightSpecies[i].day && MONTH == spotlightSpecies[i].month
        && YEAR == spotlightSpecies[i].year
        && HOUR >= spotlightSpecies[i].startHour && HOUR <= spotlightSpecies[i].endHour) {
            return i;
        }
    }

    // No spotlight mon found on the current day
    return SPOTLIGHT_NO_POKEMON;
}

void ReturnCurrentSeasonIndex(void)
{
    u16 DAY, MONTH;

    GetCurrentDateAndTime();
    DAY = VarGet(VAR_UNUSED_0x40FB);
    MONTH = VarGet(VAR_UNUSED_0x40FC);
    
    if ((MONTH == 9 && DAY >= 22) || MONTH == 10 || MONTH == 11 || (MONTH == 12 && DAY < 21))
    {
        gSaveBlock1Ptr->seasons = 1; // fall
    }
    else if ((MONTH == 12 && DAY >= 21) || MONTH == 1 || MONTH == 2 || (MONTH == 3 && DAY < 20))
    {
        gSaveBlock1Ptr->seasons = 2; // winter
    }
    else if ((MONTH == 3 && DAY >= 20) || MONTH == 4 || MONTH == 5 || (MONTH == 6 && DAY < 20))
    {
        gSaveBlock1Ptr->seasons = 3; // spring
    }
    else // (MONTH == 6 && DAY >= 20) || MONTH == 7 || MONTH == 8 || (MONTH == 9 && DAY < 22)
    {
        gSaveBlock1Ptr->seasons = 0; // summer
    }
}
