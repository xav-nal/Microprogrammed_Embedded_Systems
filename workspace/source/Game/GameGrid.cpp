#pragma once

#include "GameGrid.h"
#include "../P_Graphics.h"

// SOUND
#include "../sound.h"
#include "soundbank.h"
//:::::::

bool Grid_Cells[PLAYFIELD_CELLNB];
int Grid_TileW;
int Grid_TileH;
int Grid_Left;
int Grid_Top;
int Grid_Right;
int Grid_Bottom;

void Grid_init(void)
{
    /*
    * Setup the map with default values
    */
    int i;
    // upper left to down left and after second column from left to down
    for (i = 0; i < PLAYFIELD_CELLNB; i++)
    { 
        Grid_Cells[i] = false;
        SetMap16x16To(i,false);
    }

}

void Grid_setBackGroundDimentions(int w, int h)
{
	Grid_Left = MARGIN_LEFT;
	Grid_Top = MARGIN_TOP;
	w -= Grid_Left + MARGIN_RIGHT;
	h -= Grid_Top  + MARGIN_BOTTOM;
	Grid_Right  = Grid_Left + w;
	Grid_Bottom = Grid_Top  + h;
	
	Grid_TileW = w / PLAYFIELD_COLS;
	Grid_TileH = h / PLAYFIELD_ROWS;
}


bool Grid_IsCellValid(int index)
{
    return (index >= 0 && index < PLAYFIELD_CELLNB);
}

bool Grid_IsCellFree(int index)
{
    if (Grid_IsCellValid(index))
        return !Grid_Cells[index];
    else
        return false;
}


int Grid_tryToMoveCells(int from, int to)
{  
    /*
    * Check if the next cell is valid for the block
    * check if the block is out of the map
    */  
    bool validMove = false;
    bool outPlayField = false;

    outPlayField = (to >= PLAYFIELD_CELLNB );
    
    if ((to != from))
    {
        if(!outPlayField)
        {
            validMove = true;
        }
    }
    
    //write to grid
    if (validMove)
    {
        Grid_Cells[from] = false;
        SetMap16x16To(from,false);

        Grid_Cells[to] = true;
        SetMap16x16To(to,true);

        return VALIDMOVE;
    }
    else if(outPlayField)  // delete the block when it get out of the player field
    {  
        Grid_Cells[from] = false;
        SetMap16x16To(from,false);
        return MOVEOUT;
    }
    else 
        return ERRORMOVE;
    
}

bool Grid_check_contact(POSITION billy_pose)
{
    /*
    * This function check contact with Billy and a bloc
    * It check at Billy position, in front of billy position and below Billy
    * return True if contact
    */

    bool contact = false;

    //switch x and y
    int x = (256 - billy_pose.position_x)/16;
    int y = billy_pose.position_y / 16;

    int index16_blockmiddle = x*12 + y + 1;
    int index16_blockfront = index16_blockmiddle - 12;
    int index16_blockbelow = index16_blockmiddle + 1;
    
    contact = Grid_Cells[index16_blockmiddle] || Grid_Cells[index16_blockfront] || Grid_Cells[index16_blockbelow];

    return contact;
}












