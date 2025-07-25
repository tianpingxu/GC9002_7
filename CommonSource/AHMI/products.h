//created by Zzen1sS
//2019/8/19
#ifndef PRODUCTS__H
#define PRODUCTS__H

#include "publicDefine.h"
#include <string>
#include "ConfigBuf.h"
class ProductInfoSerialClass{
public:
	u32 flashSize;//flash size, MB
	//u8 displayDirection;//display direction
	//u16 serialBaudRate;//serial rate
	//disply parameters
	u32 HPIXEL_VALUE;
	u32 VLINES_VALUE;
	u32 HSYNCPW_VALUE;
	u32 VSYNCPW_VALUE;
	u32 HFNPRCH_VALUE;
	u32 VFNPRCH_VALUE;
	u32 HBKPRCH_VALUE;
	u32 VBKPRCH_VALUE;
	//display clock parameters
	u32 VCOMUTIPLY;
	u32 CLKINDEVIDE;
	u32 PLLOUTPUTDIVIDE;

	u32 DisplayRotate;

	u32 CRC;
	u32 TouchSound;
	u32 BacklightPWMFrequence;
	u32 BacklightOn;
	u32 BacklightNormal;
	u32 BacklightWait;
	u32 BacklightDuration;

	
	u32 BuzzerFrequence;
    u32 BuzzerDuty;
	u32 BuzzerDutyDiv;
	u32 BuzzerDuration;

	u32 TouchScreenOn;

};


class ProductInfoClass{
public:
	std::string name;
	u32 flashSize;//flash size, MB
	u8 displayDirection;//display direction
	u16 serialBaudRate;//serial rate
	//disply parameters
	u32 HPIXEL_VALUE;
	u32 VLINES_VALUE;
	u32 HSYNCPW_VALUE;
	u32 VSYNCPW_VALUE;
	u32 HFNPRCH_VALUE;
	u32 VFNPRCH_VALUE;
	u32 HBKPRCH_VALUE;
	u32 VBKPRCH_VALUE;
	//display clock parameters
	u32 VCOMUTIPLY;
	u32 CLKINDEVIDE;
	u32 PLLOUTPUTDIVIDE;

	u32 DisplayRotate;
	
	u32 CRC;
	u32 TouchSound;
	u32 BacklightPWMFrequence;
	u32 BacklightOn;
	u32 BacklightNormal;
	u32 BacklightWait;
	u32 BacklightDuration;

	u32 BuzzerFrequence;
    u32 BuzzerDuty;
	u32 BuzzerDutyDiv;
	u32 BuzzerDuration;

	u32 TouchScreenOn;

	int ProductInfoClass::flush(ConfigBuf* pConfigBuf);
	int ProductInfoClass::writeToFile(ofstream * fid);
};



#endif
