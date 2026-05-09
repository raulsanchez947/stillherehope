#ifndef SPECIALS_H
#define SPECIALS_H

#define EASY 0
#define NORMAL 1
#define HARD 2
#define EXTREME 3

#define ROOKIE 0
#define ADVENTURER 1
#define ELITE 2
#define CHAMPION 3
#define MASTER 4
#define LEGEND 5

typedef struct {
    u16 flag;
    u8 increment;
} Flags;

extern const Flags flagset1[];
extern const Flags flagset2[];
extern const Flags flagset3[];

u8 GetGameDifficulty(void);
void CheckGameDifficulty(void);
void SetPlayerRank(void);
u8 CheckPlayerRank(void);
bool8 IsMapRandmoized(void);
u8 GetSeason(void);

extern const u8 gText_Legend[];
extern const u8 sText_Master[];
extern const u8 gText_Champion[];
extern const u8 gText_Elite[];
extern const u8 gText_Adventurer[];
extern const u8 gText_Rookie[];

#endif  // SPECIALS_H
