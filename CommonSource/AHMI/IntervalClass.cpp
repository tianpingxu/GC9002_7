#include <string.h>
#include "IntervalClass.h"
#include "publicInclude.h"

Interval::Interval(){
	nowTime = 0;
}

Interval::Interval(IntervalCB cb,void* arg, u16 intervalTime){
	intervalCB = cb;
	arg = arg;
	intervalTime = intervalTime;
	nowTime = 0;
}

Interval::~Interval(){
}

void Interval::execute(){
	intervalCB(arg);
}

u32 IntervalManager::init(){
	intervals = (Interval*)csi_kernel_malloc(sizeof(Interval)* IntervalMaxNum, NULL);
	if(intervals == NULL)
	{
		ERROR_PRINT("animation Class Initial failed.\r\n");
		return (EM_enErrRetModularInterval | EM_ErrRetInterval_NULLPointerErr);
	}
    
	/* Init animation massage array. */
	memset((void *)intervals, 0 , sizeof(Interval)* IntervalMaxNum);

	return AHMI_FUNC_SUCCESS;
}

int IntervalManager::getIDLEInterval(){

	for (int i = 0; i < IntervalMaxNum; i++)
	{
		if (intervals[i].status == IntervalStatus_IDLE)
		{
			return i;
		}
	}
	return -1;
}

int IntervalManager::addInterval(IntervalCB cb, void* arg,u16 intervalTime){
	for (int i = 0; i < IntervalMaxNum; i++)
	{
		if (intervals[i].status == IntervalStatus_IDLE)
		{
			intervals[i].intervalCB = cb;
			intervals[i].arg = arg;
			intervals[i].intervalTime = intervalTime;
			intervals[i].nowTime = 0;
			intervals[i].status = IntervalStatus_RUNNING;
			return i;
		}
	}
	return -1;
}

int IntervalManager::setInterval(int intervalIdx,IntervalCB cb, void* arg,u16 intervalTime){
	if (intervalIdx>=0 && intervalIdx<IntervalMaxNum)
	{
		intervals[intervalIdx].intervalCB = cb;
		intervals[intervalIdx].arg = arg;
		intervals[intervalIdx].intervalTime = intervalTime;
		intervals[intervalIdx].nowTime = 0;
		intervals[intervalIdx].status = IntervalStatus_RUNNING;
		return intervalIdx;
	}
	return -1;
}

funcStatus IntervalManager::addIntervalByIdx(int idx, IntervalCB cb, void* arg, u16 intervalTime){
	if (idx>=0 && idx < IntervalMaxNum && intervals[idx].status == IntervalStatus_IDLE)
	{
		intervals[idx].intervalCB = cb;
		intervals[idx].arg = arg;
		intervals[idx].intervalTime = intervalTime;
		intervals[idx].nowTime = 0;
		intervals[idx].status = IntervalStatus_RUNNING;
		return 1;
	}
	return 0;
}

funcStatus IntervalManager::clearInterval(int id){
	if (id >=0 && id < IntervalMaxNum)
	{
		intervals[id].status = IntervalStatus_IDLE;
		return 1;
	}
	return 0;
}

funcStatus IntervalManager::startMaintain(u8 refreshTime){
	for (int i = 0; i < IntervalMaxNum; i++)
	{
		if (intervals[i].status != IntervalStatus_IDLE)
		{
			if (intervals[i].nowTime < intervals[i].intervalTime)
			{
				intervals[i].nowTime += refreshTime;
			}
		}
	}
	return 1;
}

funcStatus IntervalManager::maintainExe(void){
	for (int i = 0; i < IntervalMaxNum; i++)
	{
		if (intervals[i].status != IntervalStatus_IDLE)
		{
			if (intervals[i].nowTime >= intervals[i].intervalTime)
			{
				//time to execute
				intervals[i].nowTime = 0;
				intervals[i].execute();
			}
		}
	}
	return 1;
}

funcStatus IntervalManager::reset(){
	
	for (int i = 0; i < IntervalMaxNum; i++)
	{
		intervals[i].status = IntervalStatus_IDLE;
	}
	return 1;
}
