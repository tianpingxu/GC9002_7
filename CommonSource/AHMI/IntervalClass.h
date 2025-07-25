////////////////////////////////////////////////////////////////////////////////
//created by Zzen1sS
//2018/12/7
//setInterval(callback, 1000/fps)
////////////////////////////////////////////////////////////////////////////////

#ifndef INTERVALCLASS__H
#define INTERVALCLASS__H
#include "publicType.h"
#include "publicDefine.h"
#include "refreshQueueHandler.h"


#ifdef AHMI_CORE

#define IntervalMaxNum 32
#define IntervalStatus_IDLE 0
#define IntervalStatus_RUNNING 1
typedef void (*IntervalCB)(void*) ;

typedef enum
{
    EM_ErrRetInterval_NULLPointerErr = 1
   
}EM_ErrRetInterval;

class Interval{
public:
	u8 status;
	u16 nowTime;
	u16 intervalTime;//interval to execute
	IntervalCB intervalCB;
	void* arg;

	Interval(IntervalCB cb,void* arg, u16 intervalTime);
	Interval();
	~Interval();

	void execute();
};

class IntervalManager{
public:
	Interval* intervals;

	u32 init();

	int getIDLEInterval();

	int addInterval(IntervalCB cb, void* arg,u16 intervalTime);

	int setInterval(int intervalIdx,IntervalCB cb, void* arg,u16 intervalTime);

	funcStatus addIntervalByIdx(int idx, IntervalCB cb, void* arg, u16 IntervalTime);

	funcStatus clearInterval(int id);

	funcStatus startMaintain(u8 refreshTime);

    funcStatus maintainExe(void);

	funcStatus reset();
};



#endif

#endif


