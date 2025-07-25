//created by LPJ
//2019/05/27
#include "SdlPlayer.h"
#include "publicDefine.h"

#if defined(EMBEDDED) && defined(VERSION_2)
#include "csi_ahmi_i2s.h"
#endif

extern SoundClass*          gSoundPtr;
extern  ConfigInfoClass		ConfigData;

#ifdef PC_SIM
extern u8				TextureBuffer[TEXBUFSIZE];
#endif

#ifdef __cplusplus
extern "C" {
#endif
uint8_t  audio_should_pause_till_cycle_end;
uint8_t  audio_should_stop_till_cycle_end;
s32  audio_play_num; // -1 no play_num
#ifdef __cplusplus
}
#endif

MyPlayer::MyPlayer(){

}

MyPlayer::~MyPlayer(){


}

void MyPlayer::PlayerInit(){//init player
#ifdef PC_SIM
	//Init
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {  
		printf( "Could not initialize SDL - %s\n", SDL_GetError()); 
		return ;
	}
	wanted_spec.freq = ConfigData.audioSampleRate;
	wanted_spec.format = AUDIO_S16SYS;
	wanted_spec.channels = 2;
	wanted_spec.callback = fill_audio;
	wanted_spec.samples = 40960;
	wanted_spec.silence = 0;
#endif
#if defined(EMBEDDED) && defined(VERSION_2)
	if(ConfigData.NumofSound == 0)
	{
		return;
	}
	csi_ahmi_i2s_init(ConfigData.audioSampleRate);
#endif
}

void MyPlayer::play(u16 soundID, u16 playNum, u16 soundmaxID){//play with id
#ifdef PC_SIM
	SDL_CloseAudio();
	
	if(ConfigData.NumofSound == 0)
	{
		return;
	}

	int flag = -1;
	for(int i = 0;i <= soundmaxID;i++){
		if(soundID == gSoundPtr[i].mSoundID){
			flag = i;
			
			break;
		}
	}
	if(flag == -1){
		ERROR_PRINT("can not find soundID");
		return;
	}
	/*else
	{
		int err = fopen_s(&mfp,AHMI_SOUND_FILE_NAME,"rb");
		if(err != 0) ERROR_PRINT("can not find audio file");
	}
	fseek(mfp,gSoundPtr[flag].mStartAddr,SEEK_SET);*/
	wanted_spec.samples = gSoundPtr[flag].mSize/4;
	if (SDL_OpenAudio(&wanted_spec, NULL)<0){ 
				
		ERROR_PRINT("Failed to open audio, call SDL_GetError() for more information"); 
		return ; 
	}
	u8 *outbuff;
	u32 lastsize;
	lastsize = gSoundPtr[flag].mSize;
	/*outbuff = (u8 *)malloc(lastsize);*/
	outbuff = TextureBuffer+gSoundPtr[flag].mStartAddr;
	SDL_PauseAudio(0);
	//fread(outbuff,lastsize,1,mfp);
	audio_pos_og = (u8 *)outbuff;
	audio_len_og = lastsize;
	audio_pos = (u8 *)outbuff;
	audio_len = lastsize;
	audio_loop = gSoundPtr[flag].loop;
	audio_should_pause_till_cycle_end = 0;
	audio_should_stop_till_cycle_end = 0;
	audio_play_num = playNum>0?playNum:-1;
	
#endif
#if defined(EMBEDDED) && defined(VERSION_2)
	u32 flag = 0xFFFFFFFF, i;
	
	if(ConfigData.NumofSound == 0)
	{
		return;
	}
	
	for(i = 0;i <= soundmaxID;i++){
		if(soundID == gSoundPtr[i].mSoundID){
			flag = i;
			break;
		}
	}
	
	if(flag == 0xFFFFFFFF)
	{
		return;
	}
	
	csi_ahmi_i2s_play((uint16_t *)gSoundPtr[flag].mStartAddr, gSoundPtr[flag].mSize, gSoundPtr[flag].loop, playNum);
#endif
}

void MyPlayer::pause(u8 tillCycleEnd){
#ifdef PC_SIM
	if (tillCycleEnd)
	{
		audio_should_pause_till_cycle_end = 1;
	}else
	{
		SDL_PauseAudio(1);
	}
	
#endif
#if defined(EMBEDDED) && defined(VERSION_2)
	if(ConfigData.NumofSound == 0)
	{
		return;
	}
	
	if (tillCycleEnd)
	{
		audio_should_pause_till_cycle_end = 1;
	}
	else
	{
		csi_ahmi_i2s_pause();
	}
#endif

}

void MyPlayer::resume(){
#ifdef PC_SIM
	SDL_PauseAudio(0);
#endif
#if defined(EMBEDDED) && defined(VERSION_2)
	if(ConfigData.NumofSound == 0)
	{
		return;
	}
	
	csi_ahmi_i2s_resume();
#endif
}

void MyPlayer::stop(u8 tillCycleEnd){
#ifdef PC_SIM
	if (tillCycleEnd)
	{
		audio_should_stop_till_cycle_end = 1;
	}else
	{
		SDL_CloseAudio();//Close SDL
		SDL_Quit();
	}
	
	//fclose(mfp);
#endif
#if defined(EMBEDDED) && defined(VERSION_2)
	if(ConfigData.NumofSound == 0)
	{
		return;
	}
	
	if (tillCycleEnd)
	{
		audio_should_stop_till_cycle_end = 1;
	}
	else
	{
		csi_ahmi_i2s_stop();
	}
#endif
}

#ifdef PC_SIM
void  fill_audio(void *udata,u8 *stream,int len){
	//SDL 2.0
	SDL_memset(stream, 0, len);
	if(audio_len==0)
		return; 
 
	len=(len>audio_len?audio_len:len);	/*  Mix  as  much  data  as  possible  */ 
 
	SDL_MixAudio(stream,audio_pos,len,SDL_MIX_MAXVOLUME);
	audio_pos += len; 
	audio_len -= len; 
	if (audio_len <= 0)
	{
		if (audio_play_num!=-1)
		{
			audio_play_num--;
		}
		if (audio_should_pause_till_cycle_end)
		{
			audio_should_pause_till_cycle_end = 0;
			SDL_PauseAudio(1);
		}else if (audio_should_stop_till_cycle_end)
		{
			audio_should_stop_till_cycle_end = 0;
			SDL_CloseAudio();//Close SDL
			SDL_Quit();
		}else
		{
			//consider playNum
			if (audio_play_num!=-1)
			{
				if (audio_play_num == 0)
				{
					//end
				}else
				{
					audio_pos = audio_pos_og;
					audio_len = audio_len_og;
				}
			}else
			{
				//consider loop
				if (audio_loop)
				{
					//loop
					audio_pos = audio_pos_og;
					audio_len = audio_len_og;
				}
			}
		}
	}
}
#endif

