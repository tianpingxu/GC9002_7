#ifndef DISPLAY_MODULE_CLASS__H
#define DISPLAY_MODULE_CLASS__H

#include "simulatorParameters.h"
#include "ahmiv3.h"

class DisplayModuleClass
{
public:
	DISPLAY_REGS mDisplayRegs; // the reg list of display module
	/**
	*	@brief start display rendering current frame
	*	@param pFrameBuffer : the address of the frame buffer
	*	@retval None.
	*/
	void startDrawing(FramBufferPtr pFrameBuffer);
};

#endif
