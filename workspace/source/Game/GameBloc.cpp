/*
*
* \date: 28.12.2022
* \brief: implementation of all code for blocks management
*
*/
#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "GameBloc.h"


// use during training phase for log
bool Bloc_isInited = false;

int spawn_state = MODE_IDLE;


typedef struct BLOCK BLOCK;
struct BLOCK
{
    int id;
    int indexMap;
    int nextIndexMap;
    BLOCK *next;
};

int nb_blocks = 0;
int next_block_id = 0;
static BLOCK *blocks_list = NULL;


void block_insertion(int id)
{
    /*
    * We insert a block at the end of the list
    */
    int elements = 0;
	BLOCK *current = blocks_list;
	BLOCK *previous = blocks_list;
	BLOCK *last = blocks_list;
	BLOCK *new_block = (BLOCK *)malloc(sizeof(BLOCK));
    
    
    while (current != NULL)
    {
        previous = current;
        current = current->next;

        if(current != NULL) last = current;

        elements++;
    }

    new_block->id = id;
    new_block->next = NULL;
    new_block->indexMap = START_CELL;
    new_block->nextIndexMap = 10;

    
    if(elements > 0) last->next = new_block;	
    else blocks_list = new_block; 

}

void first_block_remove()
{
    /*
    * We remove the first block of the list
    */
	BLOCK *toRemove = NULL;
	
    if (blocks_list == NULL) 
    {
        //printf('Error remove block');
        return;
    }

    if (blocks_list != NULL)
    {
        toRemove = blocks_list;
        blocks_list = blocks_list->next;
        free(toRemove);
    }
}

// This function remove all the blocks
void blocks_remove_all_blocks(void)
{

    while(blocks_list != NULL)
    {
        first_block_remove();
    }
}

// remove a precise block
void block_remove( int block_id)
{

    BLOCK *current = blocks_list;
    BLOCK *previous = blocks_list;

    if(current->id == block_id)
    {
        first_block_remove(blocks_list);
        return;
    }
        
        
	// search for the block to delete
	while(current->id != block_id)
	{	
		previous = current;
		current = current->next;	
	}
	
    // delete the block
    previous->next = current->next;
    free(current);
}


//spawn a block in the map 
bool Block_spawn(void)
{
    //new element
    block_insertion(next_block_id);
    nb_blocks++;
    next_block_id++;

    BLOCK *current = blocks_list;
    int stateMoved = Grid_tryToMoveCells(current->indexMap, current->nextIndexMap);
       
    //update indexMap with the new location
    if (stateMoved == VALIDMOVE)
        current->indexMap = current->nextIndexMap;

    return true;
}

bool Blocks_tryToMove(void)
{
    /*
    * loop on all existing blocks
    * move the block in the new locations
    * update block position and remove it if out of the map
    */
    BLOCK *current = blocks_list;

    while (current != NULL)
    {
        // 
        int stateMoved = Grid_tryToMoveCells(current->indexMap, current->nextIndexMap);
        
        if (stateMoved == VALIDMOVE)
            current->indexMap = current->nextIndexMap;
        else if(stateMoved == MOVEOUT)
        {
            block_remove(current->id);
            nb_blocks--;

        }
        
        current = current->next;
    }

    return true;
}

// move the block in its new location
void Blocks_shiftCells(int cellscrool)
{
    BLOCK *current = blocks_list;
    bool move_done = false;

    // define the next cell for all existing blocks
    while (current != NULL)
    {
        current->nextIndexMap = current->indexMap + cellscrool;
        current = current->next;
    }

    // move the blocks
    move_done = Blocks_tryToMove();

    //if(!move_done) // for log
}

// Init the blocks'list and add a first elements
void Blocks_init(void)
{
    Bloc_isInited = false;
    bool first_block = true;
    spawn_state = MODE_IDLE;

    // we add a first element in the block list
    first_block = Block_spawn();
 
}


void Blocks_movement_manager(void)
{
    /*
    * mode standard: one block at the time on the map
    * when the block is out the map a need block is spawned
    * mode intense: can have multiple block at the same time on the
    * map. We spawned the block in a random way using rand()
    */
    // check spawn state Blocks_State
    
    switch (spawn_state)
    {
        case MODE_STD_SPAWN:
            if (blocks_list != NULL)
            {
                Blocks_shiftCells(C_DOWN);

                return;
            }
            else
            {
                Bloc_isInited = Block_spawn();

            }
            break;

        case MODE_INTENSE_SPAWN:
            if (blocks_list != NULL)
            {
                Blocks_shiftCells(C_DOWN);
                
                // add more block in a random time
                int spawn_random = (rand() % 15);

                if(spawn_random == 0)
                    Bloc_isInited = Block_spawn();

                return;
            }
            else
            {
                // spawn a block if no more block on the map
                Bloc_isInited = Block_spawn();

            }

            break;
        
        default:
            break;
    }
}

// function used to change the spawn mode of the blocks
void Blocks_spawn_mode_change(int new_spawn_state)
{
    spawn_state = new_spawn_state;
}

