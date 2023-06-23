#include <stdio.h>

#include "P_Graphics.h"

// :::: IMAGES ::::::
#include "CourseA.h"
#include "CourseB.h"
#include "stop.h"
#include "dead.h"
#include "start.h"
#include "good_luck.h"
#include "game_over.h"
#include "bier.h"
#include "win.h"
// :::::::::::::::::::

u16* P_Graphics_mainBuffer;
u16* P_Graphics_subBuffer;
int P_Graphics_mainW;
int P_Graphics_mainH;
int P_Graphics_subW;
int P_Graphics_subH;

//Pointer to the graphic buffer where to store the sprite
u16* gfx;

//This variable tells us which image to render (0,1 or 2)
int run_image = 0;



int x = BILLY_POS_X; 
int y = BILLY_POS_Y; 
int state_billy = RUNNING; 
int billy_behaviour = WAIT;


void P_Graphics_setup_main()
{
	/*
	* Setup the main engine
	* enable the different RAM memory
	* initiallize the sprites
	*/

#ifdef TILES
	// ============ BLOCKS (0) AND BACKGROUND (3) ==============
	//Enable a proper RAM memory bank for the main engine
	VRAM_A_CR = VRAM_ENABLE //Enable
			| VRAM_A_MAIN_BG; //Bank for the main engine

	//Configure the main engine in mode 0 (2D) and activate Backgrounds 3 and 0
	REG_DISPCNT = MODE_0_2D | DISPLAY_BG0_ACTIVE | DISPLAY_BG3_ACTIVE;
#endif

	// ============ BILLY ==============
	//Set up memory bank to work in sprite mode (offset since we are using VRAM A for backgrounds)
	VRAM_B_CR = VRAM_ENABLE | VRAM_B_MAIN_SPRITE_0x06400000;

	// ============ SUB SCREEN ==============
	//Enable a proper RAM memory bank for sub engine
	VRAM_C_CR = VRAM_ENABLE | VRAM_C_SUB_BG;

	REG_DISPCNT_SUB = MODE_5_2D | DISPLAY_BG2_ACTIVE;

	// ========= SPRITES ======================

	//Initialize sprite manager and the engine
	oamInit(&oamMain, SpriteMapping_1D_32, false); 

	//Allocate space for the graphic to show in the sprite
	gfx = oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_256Color); 

}


void P_Graphics_sub_screen(int sub_state)
{
	/*
	* Configure the sub screen and manage the different images to display wrt 
	* to the game state
	*/

	BGCTRL_SUB[2] = BG_BMP_BASE(0) | BgSize_B8_256x256;

	//Affine Marix Transformation
	REG_BG2PA_SUB = 256;
	REG_BG2PC_SUB = 0;
	REG_BG2PB_SUB = 0;
	REG_BG2PD_SUB = 400;

	switch (sub_state)
	{
	    case START_IMG: 
	    	swiCopy(startPal, BG_PALETTE_SUB, startPalLen);
	    	swiCopy(startBitmap, BG_GFX_SUB, startBitmapLen);
	        break;
	    case LUCK_IMG: 
			swiCopy(good_luckPal, BG_PALETTE_SUB, good_luckPalLen/2);
			swiCopy(good_luckBitmap, BG_GFX_SUB, good_luckBitmapLen);
			break;
	    case GAME_OVER_IMG: 
			swiCopy(game_overPal, BG_PALETTE_SUB, game_overPalLen);
			swiCopy(game_overBitmap, BG_GFX_SUB, game_overBitmapLen);
			break;
	    case WIN_IMG: 
			swiCopy(winPal, BG_PALETTE_SUB, winPalLen);
			swiCopy(winBitmap, BG_GFX_SUB, winBitmapLen);
			break;
	}
}

bool P_Graphics_Billy_check_block_below(void)
{
	/*
	* Check if there is a block below Billy
	*
	*/
	POSITION billy_pose;
	billy_pose.position_x = x;
	billy_pose.position_y = y;
	billy_pose.status = state_billy;

	bool contact = Grid_check_contact(billy_pose);
	
	if(contact)
		return true;

	return false;
}


void P_Graphics_Billy_jump_management(void)
{
	/*
	* This fonction manage the differents step of the jump (going up and going down)
	* 
	* There is a feature to allow the character to rebound from a block by detecting 
	* a block below the character. A rebound allows to go upper than a normal jump
	*
	*/

	int speed_jumping = 10; //old value 25

	if(state_billy == GOING_UP)
	{
		y -= speed_jumping; // minus because of the axe orientation

		if ( y <= BILLY_MAX_JUMP_HEIGHT)
		{
			y = BILLY_MAX_JUMP_HEIGHT;
			state_billy = GOING_DOWN; //down
		}
	}
	else if(state_billy == GOING_SUPER_UP)
	{
		y -= speed_jumping; 

		if ( y <= BILLY_MAX_JUMP_HEIGHT - SUPER_SUPPLEMENT)
		{
			state_billy = GOING_DOWN; 
		}

	}
	else if(state_billy == GOING_DOWN)
	{
		
		if(!P_Graphics_Billy_check_block_below())
		{
			y += speed_jumping;

			if ((y >= BILLY_POS_Y))
			{	
				state_billy = RUNNING; //run
			}
		}
		else
		{
			state_billy = GOING_SUPER_UP;
		}
	}
}


//Timer0 ISR for changing the image
void Timer0ISR()
{
	/*
	* Timer to setup the images/sprite for the differents 
	* BIlly behaviour
	*/

	switch (billy_behaviour)
	{
		case WAIT:
			//Copy data for the graphic (palette and bitmap)
			swiCopy(stopPal, SPRITE_PALETTE, stopPalLen/2);
			swiCopy(stopTiles, gfx, stopTilesLen/2);
			break;

		case DEAD:
			swiCopy(deadPal, SPRITE_PALETTE, deadPalLen/2);
			swiCopy(deadTiles, gfx, deadTilesLen/2);
			break;

		case RUN:
			P_Graphics_Billy_jump_management();
			//Change image and copy it to the engin memory
			run_image = (run_image + 1) % 2;
			switch (run_image)
			{
				case 0:
					swiCopy(CourseAPal, SPRITE_PALETTE, CourseAPalLen/2);
					swiCopy(CourseATiles, gfx, CourseATilesLen/2);
					break;

				case 1:
					swiCopy(CourseBPal, SPRITE_PALETTE, CourseBPalLen/2);
					swiCopy(CourseBTiles, gfx, CourseBTilesLen/2);
					break;
			}
			break;
	}

	oamSet(&oamMain, 			// oam handler
				0,				// Number of sprite
				x, y,			// Coordinates
				0,				// Priority
				0,				// Palette to use
				SpriteSize_32x32,			// Sprite size
				SpriteColorFormat_256Color,	// Color format
				gfx,			// Loaded graphic to display
				-1,				// Affine rotation to use (-1 none)
				false,			// Double size if rotating
				false,			// Hide this sprite
				false, false,	// Horizontal or vertical flip
				false			// Mosaic
				);
	oamUpdate(&oamMain);
}

// Configure the jump
void P_Graphics_Billy_start_jump(void)
{
	//if: to avoid multi jump
	if((state_billy != GOING_UP) & (state_billy != GOING_DOWN) & (state_billy != GOING_SUPER_UP))
	{ 
		state_billy = GOING_UP;
	}
}

// Update the graphic when Billy runs on a block
void P_Graphics_Billy_rebound(void)
{
	state_billy = GOING_SUPER_UP;
}


POSITION P_Graphics_Billy_get_position(void)
{
	POSITION billy_pose;
	billy_pose.position_x = x;
	billy_pose.position_y = y;
	billy_pose.status = state_billy;

	return billy_pose;
}



void P_Graphics_Billy_run(int new_behaviour)
{
	/*
	* Configure the timer for changing the running sprite
	*/
	int frequency = 12; 
	billy_behaviour = new_behaviour;
	
	irqSet(IRQ_TIMER0, &Timer0ISR);
	TIMER_DATA(0) = TIMER_FREQ_1024(frequency);
	TIMER0_CR = TIMER_ENABLE | TIMER_DIV_1024 | TIMER_IRQ_REQ;
	irqEnable(IRQ_TIMER0);

}
