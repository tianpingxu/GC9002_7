#ifndef WAVEFILTERTICKARG__H
#define WAVEFILTERTICKARG__H
#include "TagClass.h"
#include "WaveFilter.h"
typedef struct{
	int intervalID;
	int totalDuration;
	TagClassPtr tag;
	WaveFilterStatus* status;
	WaveFilter* waveFilter;
}WaveFilterTickArg;
#endif // !WAVEFILTERTICKARG__H
