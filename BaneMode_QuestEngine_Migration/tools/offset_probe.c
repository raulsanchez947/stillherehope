#include <stdio.h>
#include <stddef.h>
#include "global.h"
int main(void){
    printf("SB1_SIZE %zu\n", sizeof(struct SaveBlock1));
    printf("SB2_SIZE %zu\n", sizeof(struct SaveBlock2));
    printf("SB2_ENCKEY 0x%zx\n", offsetof(struct SaveBlock2, encryptionKey));
    printf("SB1_MONEY 0x%zx\n", offsetof(struct SaveBlock1, money));
    printf("SB1_COINS 0x%zx\n", offsetof(struct SaveBlock1, coins));
    printf("SB1_REGCOMPAT 0x%zx\n", offsetof(struct SaveBlock1, registeredItemCompat));
    printf("SB1_PC 0x%zx\n", offsetof(struct SaveBlock1, pcItems));
    printf("SB1_ITEMS 0x%zx\n", offsetof(struct SaveBlock1, bagPocket_Items));
    printf("SB1_KEY 0x%zx\n", offsetof(struct SaveBlock1, bagPocket_KeyItems));
    printf("SB1_BALL 0x%zx\n", offsetof(struct SaveBlock1, bagPocket_PokeBalls));
    printf("SB1_TM 0x%zx\n", offsetof(struct SaveBlock1, bagPocket_TMHM));
    printf("SB1_BERRIES 0x%zx\n", offsetof(struct SaveBlock1, bagPocket_Berries));
    printf("SB1_MED 0x%zx\n", offsetof(struct SaveBlock1, bagPocket_Medicine));
    printf("SB1_BATTLE 0x%zx\n", offsetof(struct SaveBlock1, bagPocket_BattleItems));
    printf("SB1_POWER 0x%zx\n", offsetof(struct SaveBlock1, bagPocket_PowerUp));
    printf("SB1_MEGA 0x%zx\n", offsetof(struct SaveBlock1, bagPocket_MegaStones));
    printf("SB1_Z 0x%zx\n", offsetof(struct SaveBlock1, bagPocket_ZCrystals));
    printf("ITEMSLOT %zu\n", sizeof(struct ItemSlot));
    return 0;
}
