#ifndef GAMEGRID_H
#define GAMEGRID_H

#include <stdlib.h>
#include "GameDefines.h"
#include "../Tools/ArrayTools.h"
#include "GameGrid.cpp"
#include "../P_Map16x16.h"



void Grid_init(void);
void Grid_setBackGroundDimentions(int w, int h);


bool Grid_IsCellValid(int index);
bool Grid_IsCellFree(int index);

int Grid_tryToMoveCells(int from, int to);
bool Grid_check_contact(POSITION billy_pose);

#endif


