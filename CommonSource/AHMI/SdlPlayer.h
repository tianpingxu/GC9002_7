//created by LPJ
//2019/05/27
#ifndef SDLPLAYER__H
#define SDLPLAYER__H
#include "publicInclude.h"
#include "SoundVector.h"

#ifdef PC_SIM
extern "C"{
#include "stdint.h"
#include "SDL.h"
};
#endif // PC_SIM

static  uint32_t  audio_len_og;//original when played 
static  uint8_t  *audio_pos_og;//original when played
static  uint32_t  audio_len; 
static  uint8_t  *audio_pos; 
static  uint8_t  audio_loop;
#ifdef PC_SIM
//static MyPlayer* audio_player;//current used player
void  fill_audio(void *udata,u8 *stream,int len);
#endif

class MyPlayer{
public:
	MyPlayer();
	~MyPlayer();

#ifdef PC_SIM
	SDL_AudioSpec wanted_spec;
#endif
	

	u32 StartAddr;
	u32 Size;
#ifdef PC_SIM
	FILE*  mfp;
#endif

	void PlayerInit();
	void play(u16 soundID, u16 playNum, u16 soundmaxID);
	void pause(u8 tillCycleEnd);
	void stop(u8 tillCycleEnd);
	void resume();

};


#endif
