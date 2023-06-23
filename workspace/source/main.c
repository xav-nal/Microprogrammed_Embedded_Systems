/*---------------------------------------------------------------------------------

	date 01/2023

	author alicia mauroux - xavier nal

---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "P_Controls.h"
#include "P_Graphics.h"
// SOUND
#include "sound.h"
#include "soundbank.h"
//:::::::

#include "Game/GameGameplay.h"


//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	
    P_InitNDS();
    sound_init();
    srand(clock());

    Gameplay_Load();

	while(true)
	{
		handleInput();
		Gameplay_update();
		swiWaitForVBlank();
	}

	return 0;
}
