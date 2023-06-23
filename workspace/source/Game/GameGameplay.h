#ifndef GAMEGAMEPLAY_H
#define GAMEGAMEPLAY_H

#include <nds.h>
#include "GameDefines.h"
#include "GameGrid.h"
#include "GameBloc.h"
#include "GameGameplay.cpp"

bool Gameplay_billy_contact(POSITION billy_pose);
void Gameplay_set_difficulty(int contraint);

void Gameplay_Load(void);
void Gameplay_init(void);
void Gameplay_handleInput(enum ACTION action);
void Gameplay_update(void);
void Gameplay_set_win(void);







#endif
