//created by Zzen1sS
//2019/8/20
#include "products.h"
#include <iostream>

int ProductInfoClass::flush(ConfigBuf* pConfigBuf)
{
	ProductInfoSerialClass ps;
	ps.flashSize = this->flashSize;
	/*ps.displayDirection = this->displayDirection;
	ps.serialBaudRate = this->serialBaudRate;*/
			
	ps.HPIXEL_VALUE = this->HPIXEL_VALUE;
	ps.VLINES_VALUE = this->VLINES_VALUE;
	ps.HSYNCPW_VALUE = this->HSYNCPW_VALUE;
	ps.VSYNCPW_VALUE = this->VSYNCPW_VALUE;
	ps.HFNPRCH_VALUE = this->HFNPRCH_VALUE;
	ps.VFNPRCH_VALUE = this->VFNPRCH_VALUE;
	ps.HBKPRCH_VALUE = this->HBKPRCH_VALUE;
	ps.VBKPRCH_VALUE = this->VBKPRCH_VALUE;
			
	ps.VCOMUTIPLY = this->VCOMUTIPLY;
	ps.CLKINDEVIDE = this->CLKINDEVIDE;
	ps.PLLOUTPUTDIVIDE = this->PLLOUTPUTDIVIDE;

	ps.DisplayRotate = this->DisplayRotate;

	ps.CRC = this->CRC;
	ps.TouchSound = this->TouchSound;
	ps.BacklightOn = this->BacklightOn;
	ps.BacklightNormal = this->BacklightNormal;
	ps.BacklightWait = this->BacklightWait;
	ps.BacklightDuration = this->BacklightDuration;

	
	ps.BuzzerFrequence = this->BuzzerFrequence;
	ps.BuzzerDuty = this->BuzzerDuty;
	ps.BuzzerDutyDiv = this->BuzzerDutyDiv;
	ps.BuzzerDuration = this->BuzzerDuration;


	pConfigBuf->Write(&ps);
	int padding;
	padding = pConfigBuf->size() % 4;
	if (padding!=0)
	{
		//add padding
		for (int i = 0; i < padding; i++)
		{
			pConfigBuf->push_back(u8(0));
		}
	}
	return 0;
}

int ProductInfoClass::writeToFile(ofstream * fid)
{
	ProductInfoSerialClass ps;
	ps.flashSize = this->flashSize;
	/*ps.displayDirection = this->displayDirection;
	ps.serialBaudRate = this->serialBaudRate;*/
			
	ps.HPIXEL_VALUE = this->HPIXEL_VALUE;
	ps.VLINES_VALUE = this->VLINES_VALUE;
	ps.HSYNCPW_VALUE = this->HSYNCPW_VALUE;
	ps.VSYNCPW_VALUE = this->VSYNCPW_VALUE;
	ps.HFNPRCH_VALUE = this->HFNPRCH_VALUE;
	ps.VFNPRCH_VALUE = this->VFNPRCH_VALUE;
	ps.HBKPRCH_VALUE = this->HBKPRCH_VALUE;
	ps.VBKPRCH_VALUE = this->VBKPRCH_VALUE;
			
	ps.VCOMUTIPLY = this->VCOMUTIPLY;
	ps.CLKINDEVIDE = this->CLKINDEVIDE;
	ps.PLLOUTPUTDIVIDE = this->PLLOUTPUTDIVIDE;

	ps.DisplayRotate = this->DisplayRotate;

	ps.CRC = this->CRC;
	ps.TouchSound = this->TouchSound;
	ps.BacklightPWMFrequence = this->BacklightPWMFrequence;
	ps.BacklightOn = this->BacklightOn;
	ps.BacklightNormal = this->BacklightNormal;
	ps.BacklightWait = this->BacklightWait;
	ps.BacklightDuration = this->BacklightDuration;

	ps.BuzzerFrequence = this->BuzzerFrequence;
	ps.BuzzerDuty = this->BuzzerDuty;
	ps.BuzzerDutyDiv = this->BuzzerDutyDiv;
	ps.BuzzerDuration = this->BuzzerDuration;

	ps.TouchScreenOn = this->TouchScreenOn;

	(*fid) << "flashSize:"<<ps.flashSize<<endl;
	(*fid) << "HPIXEL_VALUE:"<<ps.HPIXEL_VALUE<<endl;
	(*fid) << "VLINES_VALUE:"<<ps.VLINES_VALUE<<endl;
	(*fid) << "HSYNCPW_VALUE:"<<ps.HSYNCPW_VALUE<<endl;
	(*fid) << "VSYNCPW_VALUE:"<<ps.VSYNCPW_VALUE<<endl;
	(*fid) << "HFNPRCH_VALUE:"<<ps.HFNPRCH_VALUE<<endl;
	(*fid) << "VFNPRCH_VALUE:"<<ps.VFNPRCH_VALUE<<endl;
	(*fid) << "HBKPRCH_VALUE:"<<ps.HBKPRCH_VALUE<<endl;
	(*fid) << "VBKPRCH_VALUE:"<<ps.VBKPRCH_VALUE<<endl;
	(*fid) << "VCOMUTIPLY:"<<ps.VCOMUTIPLY<<endl;
	(*fid) << "CLKINDEVIDE:"<<ps.CLKINDEVIDE<<endl;
	(*fid) << "PLLOUTPUTDIVIDE:"<<ps.PLLOUTPUTDIVIDE<<endl;
	(*fid) << "DisplayRotate:"<<ps.DisplayRotate<<endl;
	(*fid) << "CRC:"<<ps.CRC<<endl;
	(*fid) << "TouchSound:"<<ps.TouchSound<<endl;
	(*fid) << "BacklightPWMFrequence:"<<ps.BacklightOn<<endl;
	(*fid) << "BacklightOn:"<<ps.BacklightOn<<endl;
	(*fid) << "BacklightNormal:"<<ps.BacklightNormal<<endl;
	(*fid) << "BacklightWait:"<<ps.BacklightWait<<endl;
	(*fid) << "BacklightDuration:"<<ps.BacklightDuration<<endl;
	(*fid) << "BuzzerFrequence:"<<ps.BuzzerFrequence<<endl;
	(*fid) << "BuzzerDuty:"<<ps.BuzzerDuty<<endl;
	(*fid) << "BuzzerDutyDiv:"<<ps.BuzzerDutyDiv<<endl;
	(*fid) << "BuzzerDuration:"<<ps.BuzzerDuration<<endl;
	(*fid) << "TouchScreenOn:"<<ps.BuzzerDuration<<endl;

	return 0;
}