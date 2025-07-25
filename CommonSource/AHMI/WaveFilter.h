//created by Zzen1sS
//2018/12/6
#ifndef WAVEFILTER__H
#define WAVEFILTER__H

#include "publicDefine.h"
#include "IntervalClass.h"
#define WaveFilterMaxArgs 7

// case waveFilterTypes.Damping_Linear:
//     args = ['Step','Delay','Freqence','Duration']
// break
// case waveFilterTypes.Damping_PT1:
//     args = ['K','Threshold','Step','Delay','Frequence','Duration']
// break
// case waveFilterTypes.Damping_PT2:
//     args = ['K1','K2','Threshold','Step','Delay','Frequence','Duration']
// break


//hanlder

typedef enum
{
    EM_ErrRetWaveFilter_FilterTypeErr = 1
   
}EM_ErrRetWaveFilter;

typedef struct{
	u16 waveFilterIdx;
	u16 tagID;
	int last;
	int target;
	int delayCount;
	int temp;//reserved
}WaveFilterStatus;

class WaveFilter{
public:
	u8 type;
	int args[WaveFilterMaxArgs];

	//calculate wavefilterstatus with this.args
	int filter(WaveFilterStatus* status);

	//get wavefilter duration
	int getDuration();
	
	int getFrequence();

	int getStep();
};





#endif // !WAVEFILTER__H
