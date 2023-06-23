
#include <nds.h>
// SOUND
#include <maxmod9.h>
#include "soundbank.h"
#include "soundbank_bin.h"
//:::::::


void sound_init(){
	//Init the sound library
	mmInitDefaultMem((mm_addr)soundbank_bin);
	//Load module
	mmLoad(MOD_MUSIC);
	//Load effect
	mmLoadEffect(SFX_POP);
	mmLoadEffect(SFX_LASER);
	mmLoadEffect(SFX_RESULT);
}

void play_music(int track){
	mmStart(track,MM_PLAY_LOOP);
}

void play_sound(int track){
	mmEffect(track);
}
