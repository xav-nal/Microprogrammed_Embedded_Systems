#ifndef GAMEBLOC_H
#define GAMEBLOC_H

#include <stdlib.h>
#include "GameDefines.h"
#include "GameGrid.h"
#include "../Tools/ArrayTools.h"
#include "GameBloc.cpp"



bool Block_spawn(void);

bool Blocks_tryToMove(void);

void Blocks_shiftCells(int cellscrool);

void Blocks_init(void);

void Blocks_movement_manager(void);

void Blocks_spawn_mode_change(int new_spawn_state);

void blocks_remove_all_blocks(void);

#endif