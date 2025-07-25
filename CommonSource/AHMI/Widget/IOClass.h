////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     AHMIС���Ա
//
// Create Date:   2016/03/21
// File Name:     CanvasClass.cpp
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 3.00 - File Created 2016/04/13 by �ڴ�Ӫ
// Additional Comments:
//   
// 
////////////////////////////////////////////////////////////////////////////////
#ifndef IOCLASS_H
#define IOCLASS_H
#include "widgetClassInterface.h"

#ifdef AHMI_CORE
class IOClass
{
public:
	IOClass();
	~IOClass();

	//�ؼ����ƺ���
	funcStatus widgetCtrl(
		 WidgetClassPtr p_wptr,  //�ؼ�ָ��
		 ActionTriggerClassPtr ActionPtr,
		 u8 u8_pageRefresh      //ҳ��ˢ��
		 );
	//��ʼ��
	 funcStatus initWidget(
		WidgetClassPtr p_wptr,   //�ؼ�ָ��
		u32 *u32p_sourceShift,   //sourcebufferָ��
		NewMatrixPtr pWidgetMatrix,
		u8 u8_pageRefresh,       //ҳ��ˢ��
		TileBoxClassPtr pTileBox, //��Χ��
		u8 staticTextureEn          //�Ƿ���Ƶ���̬�洢�ռ�
		 );
};

funcStatus openIOWithWidget(u16 IOx, u16 ctrlValue, u16 pinMusk);
funcStatus setPinValueWithWidget(u16 IOx, u16 openPin, u16 pinValue, u16 pinMusk);


#endif
#endif
