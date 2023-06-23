#pragma once

//remember, hud is pixel aligned from top, bottom, left or right 
//but center is dynamic depending on screen size

//constants

//GAME
//frames per second
#define FRAME_RATE 60
#define DIFFICULTY 3
//delay before reupdating
#define OUT_OF_PAUSE_DELAY (FRAME_RATE*2)
//only first keypress counts for game actions.
#define DISREGARD_MULTIPLE_KEYPRESSED true

//game actions
#define ACTION_NONE 0


//HUD
//does playfield leave space for a margin
#define PLAYFIELD_USE_MARGINS true
//in px 
#define MARGIN_TOP 36
#define MARGIN_LEFT 12
#define MARGIN_RIGHT 64
#define MARGIN_BOTTOM 36


//PLAYFIELD
//grid size
#define PLAYFIELD_COLS 12
#define PLAYFIELD_ROWS 16
#define PLAYFIELD_CELLNB (PLAYFIELD_ROWS * PLAYFIELD_COLS)

//ticks before engine calls gravity
#define GRAVITY_RATE (FRAME_RATE/2)

//cell where new block apear
#define START_CELL -2 // -6
//cellscroll directions
#define C_CENTER 0
#define C_LEFT (-1)
#define C_UP (-PLAYFIELD_COLS)
#define C_RIGHT 1
#define C_DOWN PLAYFIELD_COLS


// key input 
enum ACTION {NONE, END, UP, RESTART};


//states
#define STATE_USER    0
#define STATE_LOCKED  1
#define STATE_FALLING 2

//mode
enum Gameplay_mode {START_MODE, PLAY_MODE, GAME_OVER_MODE, WIN_MODE};


//Gameplay
#define GAMEPLAY_UDPATE_TIME 15
#define GAMEPLAY_BLOCK_START_TIME 20

//level difficulty
#define NO_CONSTRAINT  0
#define LITTLE_CONTRAINT  3
#define MEDIUM_CONTRAINT  6
#define BIG_CONTRAINT  6

//sub engine images
enum Sub_images {START_IMG, LUCK_IMG, GAME_OVER_IMG, WIN_IMG};

//platform
#define PLATFORM_START 	704


//bloc
#define ELEM_BASE  0
#define MAX_ELEMS  1
#define BLOCK_ELEMS 1
enum Block_spawn_state {MODE_IDLE, MODE_STD_SPAWN, MODE_INTENSE_SPAWN};

enum Block_move_state {VALIDMOVE, MOVEOUT, ERRORMOVE};

//start
enum Start_level {ZERO, ONE, TWO, THREE};

// Billy 
enum Billy_status {RUNNING, GOING_UP, GOING_DOWN, GOING_SUPER_UP};
enum Billy_behaviour {WAIT, RUN, DEAD};

#define BILLY_POS_X 100
#define BILLY_POS_Y 150
#define BILLY_MAX_JUMP_HEIGHT 90 
#define SUPER_SUPPLEMENT 20


//structs
typedef struct POSITION POSITION;
struct POSITION
{
    int position_x;
    int position_y;
    int status;

};

typedef struct CHARACTER CHARACTER;
struct CHARACTER
{
    POSITION pose;
    int status;

};
