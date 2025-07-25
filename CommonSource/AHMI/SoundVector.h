//created by LPJ
//2019/05/24
#ifndef SOUNDVECTOR__H
#define SOUNDVECTOR__H
#include <vector>

#include "publicInclude.h"

#ifdef PC_SIM_Gen
#include "TexBuf.h"
#include "ConfigBuf.h"
#include "DelayCalcValue.h"
#endif
using namespace std;
class SoundClass
{
public:
	u8 mSoundID;
	u8 loop;
	u8 genSineFreq;
	u32 mStartAddr;
	u32 mSize;
	u32 sineFreq;
	u32 sineDuration;
};

class SoundVector:public vector<SoundClass>
{
public:
#ifdef PC_SIM_Gen
	SoundVector(TexBuf*);
#endif
	~SoundVector(void);

	void AddSound(u8 SoundID,u8 genSineFreq, u32 StartAddr, u32 size, u8 loop=0, u32 sineFreq=0, u32 sineDuration=0);
	
#ifdef PC_SIM_Gen
	int SoundVector::generateFromFreq(u32 feq,u32 audioSampleRate,u32 *Start,u32 *size);
	int generateFromSoundFile(std::string sSoundName,u32 audioSampleRate,u32 *Start,u32 *size);
	int flush(ConfigBuf * pConfigBuf);
	int flushAndRecordTextureAddr(ConfigBuf * pConfigBuf, vector<struct DelayCalcValue>* delayCalcValueList);
#endif
private:
#ifdef PC_SIM_Gen
	TexBuf *texbuf;
#endif
};


#endif
