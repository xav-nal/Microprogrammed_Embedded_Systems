#ifndef P_MAP_H
#define P_MAP_H
#include <nds.h>
#include "Game/GameDefines.h"
#define TILES

extern u16* mapMemory;
extern int MapCols;
extern int MapRows;

void P_Map16x16_configureBG0();
void P_Map16x16_configureBG3();
void Setstar(void);
void SetMap16x16To(int index16, bool full);
void Start_level_init(void);

#endif
