#ifndef GUARD_SCRIPT_H
#define GUARD_SCRIPT_H

struct ScriptContext;

typedef bool8 (*ScrCmdFunc)(struct ScriptContext *);
typedef u8 Script[];

struct ScriptContext
{
    u8 stackDepth;
    u8 mode;
    u8 comparisonResult;
    u8 (*nativePtr)(void);
    const u8 *scriptPtr;
    const u8 *stack[20];
    ScrCmdFunc *cmdTable;
    ScrCmdFunc *cmdTableEnd;
    u32 data[4];
};

#define ScriptReadByte(ctx) (*(ctx->scriptPtr++))

void InitScriptContext(struct ScriptContext *ctx, void *cmdTable, void *cmdTableEnd);
u8 SetupBytecodeScript(struct ScriptContext *ctx, const u8 *ptr);
void SetupNativeScript(struct ScriptContext *ctx, bool8 (*ptr)(void));
void StopScript(struct ScriptContext *ctx);
bool8 RunScriptCommand(struct ScriptContext *ctx);
void ScriptJump(struct ScriptContext *ctx, const u8 *ptr);
void ScriptCall(struct ScriptContext *ctx, const u8 *ptr);
void ScriptReturn(struct ScriptContext *ctx);
u16 ScriptReadHalfword(struct ScriptContext *ctx);
u32 ScriptReadWord(struct ScriptContext *ctx);
u32 ScriptPeekWord(struct ScriptContext *ctx);
void LockPlayerFieldControls(void);
void UnlockPlayerFieldControls(void);
bool8 ArePlayerFieldControlsLocked(void);
void ScriptContext_Init(void);
bool8 ScriptContext_IsEnabled(void);
bool8 ScriptContext_RunScript(void);
void ScriptContext_SetupScript(const u8 *ptr);
void ScriptContext_Stop(void);
void ScriptContext_Enable(void);
void RunScriptImmediately(const u8 *ptr);
u8 *MapHeaderGetScriptTable(u8 tag);
void MapHeaderRunScriptType(u8 tag);
u8 *MapHeaderCheckScriptTable(u8 tag);
void RunOnLoadMapScript(void);
void RunOnTransitionMapScript(void);
void RunOnResumeMapScript(void);
void RunOnReturnToFieldMapScript(void);
void RunOnDiveWarpMapScript(void);
bool8 TryRunOnFrameMapScript(void);
void TryRunOnWarpIntoMapScript(void);
u32 CalculateRamScriptChecksum(void);
void ClearRamScript(void);
bool8 InitRamScript(const u8 *script, u16 scriptSize, u8 mapGroup, u8 mapNum, u8 objectId);
const u8 *GetRamScript(u8 objectId, const u8 *script);
bool32 ValidateSavedRamScript(void);
u8 *GetSavedRamScriptIfValid(void);
void InitRamScript_NoObjectEvent(u8 *script, u16 scriptSize);

// srccmd.h
void SetMovingNpcId(u16 npcId);

/* Custom Stuff */
void GetCurrentDateAndTime(void);
void GetCurrentSeason(void);

// Spotlight Species feature for the Safari Zone
struct __attribute__((packed)) SpotlightSpecies
{
    // 2 bytes
    u16 startHour:5;              // Max of 24 hours
    u16 endHour:5;                // Max of 24 hours
    u16 day:5;                    // Max of 31 days
    u16 isSpotlightMonCaptured:1; // Self Explanatory
    // 2 bytes
    u16 species:11;               // For up to 2048 species
    u16 month:4;                  // Max of 12 months
    u16 spotlightToggle:1;        // Toggles if a spotlight mon is accesible or not
    // 1 byte
    u8 year:7;
    u8 isStructInitialized:1;     // Used to basically make sure that I don't infinitely refresh the struct in ReturnSpotlightMonIndex
                                  // The "clean way" would've been to make a new EWRAM variable but this way doesn't eat up more space :D
};
extern struct SpotlightSpecies spotlightSpecies[];

#define SPOTLIGHT_NO_POKEMON        0xFFFF
#define SPOTLIGHT_SPECIES_TOTAL     24             // Change this to the number of sptlight species you have

#define SPOTLIGHT_SPECIES(Day, Month, Year, startingHour, endingHour, Species, Index) \
    spotlightSpecies[Index].startHour = startingHour;          \
    spotlightSpecies[Index].endHour = endingHour;              \
    spotlightSpecies[Index].day = Day;                         \
    spotlightSpecies[Index].month = Month;                     \
    spotlightSpecies[Index].year = Year;                       \
    spotlightSpecies[Index].species = Species;                 \
    spotlightSpecies[Index].spotlightToggle = TRUE;            \
    spotlightSpecies[Index].isSpotlightMonCaptured = FALSE; 

u16 ReturnSpotlightSpecies(void);       // Returns SPECIES_NONE if no spotlight mon can be found or is already Caught
u16 ReturnSpotlightMonIndex(void);      // Returns SPOTLIGHT_NO_POKEMON if no spotlight mon can be found
void ReturnCurrentSeasonIndex(void);

enum DayOfWeek
{
    SUNDAY,
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY
};

#endif // GUARD_SCRIPT_H
