#include <nds.h>
#include <stdio.h>
#include "P_Controls.h"
#include "P_Graphics.h"

// SOUND
#include <maxmod9.h>
#include "soundbank.h"
#include "soundbank_bin.h"

int count = 0;

void handleInput()
{
    //Scan the keys
	scanKeys();

	/* Identify the keys and map the action according to the table given in
	 * the manual. Call the function Gameplay_handleInput(..) to send an action
	 * to the logic of the game */
	u16 keys = keysDown();

	if(keys & KEY_UP)
	{
		Gameplay_handleInput(UP);
        mmEffect(SFX_POP);
    
	}
}

bool touched()
{

	touchPosition touch;
	//Read the touchscreen
	touchRead(&touch);
	//Identify a valid touched coordinates and print them
	if(touch.px | touch.py){
		return true;
	}else{
		return false;
		}
}

void Timer02SR()
{
	/*
	* Timer to manage the different phase of the game (star level)
	* It is this timer that indicates the came is win 
	*/
	count ++;

	// level 1
	if (count == 10)
	{
		Setstar();
		Gameplay_set_difficulty(LITTLE_CONTRAINT);
	}

	// level 2
	if (count == 20)
	{
		Setstar();
		Gameplay_set_difficulty(MEDIUM_CONTRAINT);
		Blocks_spawn_mode_change(MODE_INTENSE_SPAWN);
	}

	// level 3
	if (count == 30)
	{
		Setstar();
		Gameplay_set_difficulty(BIG_CONTRAINT);
	}

	// Winning the game!
	if (count >= 32) 
	{
		Gameplay_set_win();
	}
}


void timer_win()
{
	
	//Configure the timer correctly, associate the handler and enable the interrupt
	TIMER_DATA(2) = (TIMER_FREQ_1024(1/2));
	TIMER2_CR = TIMER_ENABLE | TIMER_DIV_1024 | TIMER_IRQ_REQ;
	irqSet(IRQ_TIMER2, &Timer02SR);
	irqEnable(IRQ_TIMER2);
}

// to init the timer to allow restart
void P_Control_timer_initialisation(void)
{
	count=0;
}