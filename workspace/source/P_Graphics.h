#ifndef P_GRAPHICS_H
#define P_GRAPHICS_H

#include <nds.h>
#include "Game/GameDefines.h"

#define TILES

enum BUFFER_TYPE
{
    MAIN,
    SUB,
};



//members
extern u16* P_Graphics_mainBuffer;
extern u16* P_Graphics_subBuffer;
extern int P_Graphics_mainW;
extern int P_Graphics_mainH;
extern int P_Graphics_subW;
extern int P_Graphics_subH;

void P_Graphics_Billy_run();

void P_Graphics_Billy_rebound(void);
bool P_Graphics_Billy_check_block_below(void);

void P_Graphics_setup_main();
void P_Graphics_sub_screen(int sub_state);
void P_Graphics_Billy_start_jump(void); //update the variable to make Billy jump
void P_Graphics_Billy_jump_management(void); // manage the differents steps of the jumpe

POSITION P_Graphics_Billy_get_position(void);



#endif
