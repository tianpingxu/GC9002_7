////////////////////////////////////////////////////////////////////////////////
// Company:      SX
// Engineer:     AHMI小组成员
//
// Create Date:   2018/11/20
// File Name:     VideoCaptureDriver.cpp
// Project Name:  ahmiSimulator_v0304
// 
// Revision:
// Revision 2.00 - File Created 2018/11/20 by Yu Ning
// Additional Comments:
//    the VideoCapture driver api for VideoCapture module
////////////////////////////////////////////////////////////////////////////////

#include "VideoCaptureDriver.h"
#include "VideoCaptureModuleClass.h"


#ifdef VERSION_2
extern VideoCaptureModuleClass mVideoCaptureModuleClass;
#endif

/**
  * @brief  VCAP field sample mode select.
  * @param  Mode: field sample mode select.
  *         This parameter can be: VCAP_SAMPLE_ODD, VCAP_SAMPLE_EVEN, VCAP_SAMPLE_BOTH.
  * @retval None
  */
void VideoCaptureDriver::VideoCapture_SetSmpMode(uint32_t Mode)
{
	this->mVIDEOCAPTURE_CONTROL_Type.SMP_Mode = Mode;
#ifdef VERSION_1
	//TODO
#endif
}

/**
  * @brief  VCAP color bit select.
  * @param  Bit: color bit.
  *         This parameter can be: VCAP_COLOR_16, VCAP_COLOR_32.
  * @retval None
  */
void VideoCaptureDriver::VideoCapture_SetColBit(uint32_t Bit)
{
	this->mVIDEOCAPTURE_CONTROL_Type.COL_Bit = Bit;
#ifdef VERSION_1
	//TODO
#endif
}

/**
  * @brief  set max field size.
  * @param  Size: max field size.
  * @retval None
  */
void VideoCaptureDriver::VideoCapture_SetMFSize(uint32_t Size)
{
	this->mVIDEOCAPTURE_CONTROL_Type.MAXFIELD_Size = Size;
#ifdef VERSION_1
	//TODO
#endif
}

/**
  * @brief  VCAP input channels number select.
  * @param  CHANUM: input channels number select.
  *         This parameter can be: VCAP_RXCH_1, VCAP_RXCH_2, VCAP_RXCH_4.
  * @retval None
  */
void VideoCaptureDriver::VideoCapture_SetChannel(uint32_t ChannelNumber)
{
	this->mVIDEOCAPTURE_CONTROL_Type.RXCH_Num = ChannelNumber;
#ifdef VERSION_1
	//TODO
#endif
}

/**
  * @brief  Set specified channel field write base addr.
  * @param  CHx: video capture input channel number.
  *         This parameter can be CH0, CH1, CH2, CH3.
  *	@param  BassAdress: field write base address.
  * @retval None
  */
void VideoCaptureDriver::VideoCapture_SetCHxBaseAddress(uint8_t CHx, uint32_t BaseAddress)
{
	if(CHx == CH0)
	{
		this->mVIDEOCAPTURE_CONTROL_Type.CH0_BaseAddress = BaseAddress;
	}
	else if(CHx == CH1)
	{
		this->mVIDEOCAPTURE_CONTROL_Type.CH1_BaseAddress = BaseAddress;
	}
	else if(CHx == CH2)
	{
		this->mVIDEOCAPTURE_CONTROL_Type.CH2_BaseAddress = BaseAddress;
	}
	else if(CHx == CH3)
	{
		this->mVIDEOCAPTURE_CONTROL_Type.CH3_BaseAddress = BaseAddress;
	}
#ifdef VERSION_1
	//TODO
#endif
}

/**
  * @brief  Set specified channel field count.
  * @param  CHx: video capture input channel number.
  *         This parameter can be CH0, CH1, CH2, CH3.
  *	@param  FieldCount: field count.
  * @retval None
  */
void VideoCaptureDriver::VideoCapture_SetCHxFieldCount(uint8_t CHx, uint32_t FieldCount)
{
	if(CHx == CH0)
	{
		this->mVIDEOCAPTURE_CONTROL_Type.CH0_FieldCount = FieldCount;
	}
	else if(CHx == CH1)
	{
		this->mVIDEOCAPTURE_CONTROL_Type.CH1_FieldCount = FieldCount;
	}
	else if(CHx == CH2)
	{
		this->mVIDEOCAPTURE_CONTROL_Type.CH2_FieldCount = FieldCount;
	}
	else if(CHx == CH3)
	{
		this->mVIDEOCAPTURE_CONTROL_Type.CH3_FieldCount = FieldCount;
	}
#ifdef VERSION_1
	//TODO
#endif
}

/**
  * @brief  Set Video Capture registers.
  *	@param  None
  * @retval None
  */
void VideoCaptureDriver::VideoCapture_SetRegs(void)
{
#ifdef VERSION_1
	//TODO
#endif
#ifdef VERSION_2
	mVideoCaptureModuleClass.mVIDEOCAPTURE_REGS.control = this->mVIDEOCAPTURE_CONTROL_Type.SMP_Mode| this->mVIDEOCAPTURE_CONTROL_Type.RXCH_Num | this->mVIDEOCAPTURE_CONTROL_Type.COL_Bit;
	mVideoCaptureModuleClass.mVIDEOCAPTURE_REGS.maxfieldsize = this->mVIDEOCAPTURE_CONTROL_Type.MAXFIELD_Size;
	mVideoCaptureModuleClass.mVIDEOCAPTURE_REGS.CH0BaseAddr = this->mVIDEOCAPTURE_CONTROL_Type.CH0_BaseAddress;
	mVideoCaptureModuleClass.mVIDEOCAPTURE_REGS.CH1BaseAddr = this->mVIDEOCAPTURE_CONTROL_Type.CH1_BaseAddress;
	mVideoCaptureModuleClass.mVIDEOCAPTURE_REGS.CH2BaseAddr = this->mVIDEOCAPTURE_CONTROL_Type.CH2_BaseAddress;
	mVideoCaptureModuleClass.mVIDEOCAPTURE_REGS.CH3BaseAddr = this->mVIDEOCAPTURE_CONTROL_Type.CH3_BaseAddress;
	mVideoCaptureModuleClass.mVIDEOCAPTURE_REGS.CH0FieldCount = this->mVIDEOCAPTURE_CONTROL_Type.CH0_FieldCount;
	mVideoCaptureModuleClass.mVIDEOCAPTURE_REGS.CH1FieldCount = this->mVIDEOCAPTURE_CONTROL_Type.CH1_FieldCount;
	mVideoCaptureModuleClass.mVIDEOCAPTURE_REGS.CH2FieldCount = this->mVIDEOCAPTURE_CONTROL_Type.CH2_FieldCount;
	mVideoCaptureModuleClass.mVIDEOCAPTURE_REGS.CH3FieldCount = this->mVIDEOCAPTURE_CONTROL_Type.CH3_FieldCount;
#endif

}

