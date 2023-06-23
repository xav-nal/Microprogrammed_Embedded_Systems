
#include "P_Map16x16.h"
#include "background.h"
#include <nds.h>
#include <stdio.h>

u16* mapMemory;
int MapCols;
int MapRows;
int star_level = ZERO;

//Declare the different tiles used for the platform - block - grass -empty space
u8 emptyTile[64] =
{
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0
};

u8 fullTile[64] =
{
	253,0,253,253,253,253,253,253,
	253,253,253,253,253,253,253,253,
	253,253,253,253,253,253,253,253,
	255,254,255,255,255,255,254,255,
	255,254,255,255,255,255,254,255,
	255,254,255,255,255,255,254,255,
	255,254,254,254,254,254,254,255,
	255,255,255,255,255,255,255,255
};

//PLATEFORM GREEN
u8 platformUpTile[64] =
{
	0,253,253,253,253,253,253,253, //0 to give the effect of grass
	253,253,253,253,253,253,253,253,
	253,253,253,253,253,253,253,253,
	253,253,253,253,253,253,253,253,
	252,252,252,252,252,252,252,252,
	252,252,252,252,252,252,252,252,
	252,252,252,252,252,252,252,252,
	252,252,252,252,252,252,252,252
	
};

u8 platformLowTile[64] =
{

	252,252,252,252,252,252,252,252,
	252,252,252,252,252,252,252,252,
	252,252,252,252,252,252,252,252,
	252,252,252,252,252,252,252,252,
	252,252,252,252,252,252,252,252,
	252,252,252,252,252,252,252,252,
	252,252,252,252,252,252,252,252,
	252,252,252,252,252,252,252,252
};


u8 starTile[64] =
{
	251,251,251,251,251,251,251,251,
	251,251,251,251,251,251,251,251,
	251,251,251,251,251,251,251,251,
	251,251,251,251,251,251,251,251,
	251,251,251,251,251,251,251,251,
	251,251,251,251,251,251,251,251,
	251,251,251,251,251,251,251,251,
	251,251,251,251,251,251,251,251	
};



void P_Map16x16_configureBG0()
{
	/*
	 * This function is called in the initialization of the P_Map16x16 to
	 * configure the Background 0 (on the top).
	 *
	 *
	 * The function does not return anything and it does not have any input
	 * arguments. Background 0 (BG0) should be configured in the tiled mode
	 * as a 32x32 tile grid and using one general color palette of 256
	 * components (256 colors). The pointer to the map 'mapMemory' must be
	 * correctly assigned before returning from the function
	 */

	//Configure BG 0 to represent the game field
	BGCTRL[0] = BG_32x32 | BG_COLOR_256 | BG_MAP_BASE(25) | BG_TILE_BASE(4);

	//Copy the different tiles to the corresponding RAM location
	//according to the chosen TILE_BASE. 
	dmaCopy(emptyTile, (u8*)BG_TILE_RAM(4), 64);
	dmaCopy(fullTile, (u8*)BG_TILE_RAM(4) + 64, 64);
	dmaCopy(platformUpTile, (u8*)BG_TILE_RAM(4) + 128, 64);
	dmaCopy(platformLowTile, (u8*)BG_TILE_RAM(4) + 192, 64);
	dmaCopy(starTile, (u8*)BG_TILE_RAM(4) + 256 , 64);

	//Assign color components 
	BG_PALETTE[251] = ARGB16(0,63,63,0); // yellow
	BG_PALETTE[252] = ARGB16(1,88,41,0);
	BG_PALETTE[253] = ARGB16(0,0,50,0); // green
	BG_PALETTE[254] = ARGB16(1,0,0,0);
	BG_PALETTE[255] = ARGB16(1,31,15,0);

	//Set the pointer mapMemory to the RAM location of the chosen MAP_BASE
	mapMemory = (u16*)BG_MAP_RAM(25);
}



void P_Map16x16_configureBG3()
{
	/*GRAPHICS III
	*
	* Background 3 (BG3) should be configured in the tiled mode
	* as a 32x32 tile grid and using one general color palette of 256
	* components (256 colors).
	*/


	//Configure BG 3 for the background image as explained before
	BGCTRL[3] = BG_32x32 | BG_COLOR_256 | BG_MAP_BASE(26) | BG_TILE_BASE(0);

	//Copy tiles, map and palette in the memory (use swicopy or memcpy)
	swiCopy(backgroundTiles, BG_TILE_RAM(0), backgroundTilesLen);
	swiCopy(backgroundMap, BG_MAP_RAM(26), backgroundMapLen);
	swiCopy(backgroundPal, BG_PALETTE, backgroundPalLen);

}


void P_Map16x16_Init( int cols, int rows ) 
{
	/*
	* Configure the different value 
	* Col = 12 and rows = 16 by default
	*/
    MapCols=cols;
    MapRows=rows;
	star_level = ZERO;

#ifdef TILES
    //Configure BG3 for background image
    P_Map16x16_configureBG3();
    //Configure BG0 for game
    P_Map16x16_configureBG0();

#endif

}


// adding a new star to display 
void Setstar(void)
{
	star_level++;
}

// init the star
void Start_level_init(void)
{
	star_level=ZERO;
}


// set value in the Map
void SetMap16x16To(int index16, bool full)
{

	//switch x and y
	int x = index16 % MapCols;
	int y = index16 / MapCols;
	//now inverse x
	y = -(y-MapRows+1);
	index16 = x * MapRows + y;
	

	int i;
	for (i=0; i<32; i++){
		mapMemory[PLATFORM_START+i] = 2; //platformUpTile
		mapMemory[PLATFORM_START+i+32] = 3; 
	}


#ifdef TILES
    int value = full ? 1 : 0; // if full is true the value will 1 otherwise it will be 0


    //convert index16x16 to 32x32
    //also make it fall right->left rather than top->bottom
    //
    //  0  1  2  3     12  8  4  0
    //  4  5  6  7  => 13  9  5  1
    //  8  9 10 11  => 14 10  6  2
    // 12 13 14 15     15 11  7  3

    //16x16->32*32 CONVERSION
    int index32;
    index32 =  index16*2;
    index32 += (index16/16)*32;
    mapMemory[index32] = value;
    mapMemory[index32+1] = value;
    mapMemory[index32+32] = value;
    mapMemory[index32+32+1] = value;
#endif

	// switch to show the correct number of star wrt respect to the game state
	switch(star_level)
	{
		case ZERO:
			mapMemory[62]=0;
			mapMemory[60]=0;
			mapMemory[58]=0;
			break;
		
		case ONE:
			mapMemory[62]=4;
			break;

		case TWO:
			mapMemory[62]=4;
			mapMemory[60]=4;
			break;

		case THREE:
			mapMemory[62]=4;
			mapMemory[60]=4;
			mapMemory[58]=4;
			break;

		default:
			break;
	}


}


