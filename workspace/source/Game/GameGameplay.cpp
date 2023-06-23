#pragma once

#include "GameGameplay.h"
#include "../P_Controls.h"
#include "../P_Graphics.h"


//game specific
int Gameplay_mode;
int difficulty;

//members
unsigned int Gameplay_ticks; 	    //time stamp when next update can be called
unsigned int Gameplay_block_last_update;	//allow updates of shape independently from gravity by checking this



void Gameplay_init(void)
{
    /*
    * Initialize members
    */
    
    Gameplay_ticks = 0;
    Gameplay_block_last_update = 0;
    difficulty = NO_CONSTRAINT;
 
    Grid_init();
    P_Control_timer_initialisation();
    Start_level_init();

    // setup standard mode with block spawn on the map
    Blocks_spawn_mode_change(MODE_STD_SPAWN);
}


void Gameplay_Load(void)
{
    /*
    * Call in the main to load the map
    */
    Gameplay_mode = START_MODE;

    Grid_setBackGroundDimentions(256,192);
    Blocks_init();

    P_Graphics_Billy_run(WAIT);
}

void Gameplay_handleInput(enum ACTION action)
{
    int actionToSend = ACTION_NONE;
    switch (action)
    {
        // if up key pushed
        case UP:
            actionToSend = UP;
            P_Graphics_Billy_start_jump();
            break;

        default :
            actionToSend = ACTION_NONE;
    }
}

bool Gameplay_billy_contact(POSITION billy)
{
    int status = billy.status;
 
    //if Billy is going down when it touches the bloc, it means it jumped on it (because of delay, it is already down if he missed the bloc)
    if(status == GOING_DOWN)
    {
        P_Graphics_Billy_rebound();
        play_sound(SFX_RESULT);
        return false;
    }
    else if(status == RUNNING)
    {
        play_sound(SFX_LASER);
        P_Graphics_Billy_run(DEAD);
        P_Graphics_sub_screen(GAME_OVER_IMG); 
        return true;
    }
    else
        return false;
}

void Gameplay_contact_handle(void)
{
    /*
    * this function handle rebound and mortal contact between billy and a block
    */
 
    POSITION billy_pose;
    billy_pose = P_Graphics_Billy_get_position();   
    bool contact = Grid_check_contact(billy_pose);

    if(contact)
    {
        // check if game over or just a jump going down
        bool game_over = Gameplay_billy_contact(billy_pose);
        if(game_over)
        {
            Gameplay_mode = GAME_OVER_MODE;
        }
    }
}


void Gameplay_update(void) 
{
    /**
    * Update user actions
    * Update engine actions (gravity and line check)
    */
    switch (Gameplay_mode)
    { 
        case START_MODE:

            P_Graphics_sub_screen(START_IMG);

            /* Check if the touchpad was touched
            If touches, it starts the game and maintain it*/
            if(touched())
            {
                Gameplay_init();
                play_music(MOD_MUSIC);
                P_Graphics_Billy_run(RUN);
                P_Graphics_sub_screen(LUCK_IMG);
                timer_win();
                Gameplay_mode = PLAY_MODE;
            }

            break;
        
        case PLAY_MODE:
            Gameplay_block_last_update ++;

            // update the block move wiht period, the difficulty increase the move update (so blocks are faster)
            if(Gameplay_block_last_update > GAMEPLAY_UDPATE_TIME - difficulty)
            {
                Gameplay_block_last_update = 0;

                Blocks_movement_manager(); 
            }

            // check billy and block contact
            Gameplay_contact_handle();
            break;
        
        case GAME_OVER_MODE:
            // to restart - the sub image to display is directly 
            // manage in the contact detection
            if(touched())
            {
                blocks_remove_all_blocks();
                Gameplay_mode = START_MODE;
            }
            break;

        case WIN_MODE:
            P_Graphics_sub_screen(WIN_IMG);
            P_Graphics_Billy_run(WAIT); 

            // to restart
            if(touched())
            {
                blocks_remove_all_blocks();
                Gameplay_mode = START_MODE;
            }
            break;

	}
    return;
}

void Gameplay_set_win(void)
{
    /*
    * Fix the bug of winning when the character already loss
    */
    if(Gameplay_mode!=GAME_OVER_MODE)
        Gameplay_mode = WIN_MODE;
}

void Gameplay_set_difficulty(int contraint)
{
    // to increase the bloc speed
    difficulty = contraint;
}




