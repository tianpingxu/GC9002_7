////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     AHMIС���Ա
//
// Create Date:   2017/06/14
// File Name:     animationTest.cpp
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2017/06/14 by Zhou Yuzhi(������)
// Additional Comments:
//   test animation function
// 
////////////////////////////////////////////////////////////////////////////////

#include "publicInclude.h"
#include "moduleTest.h"
#include "ButtonClass.h"
#include "testFunc.h"
#include "animationTest.h"
#include <iostream>
#include <fstream>

extern AnimationClass  gAnimationClass;
extern u8 animationDuration;
extern u8 animationExist;
extern DynamicPageClassPtr gPagePtr;
extern TagClassPtr     TagPtr;
extern u16             WorkingPageID;

#ifdef AHMI_CORE

//-----------------------------
// �������� animationTest
// ����animationClass����غ���
// �����б�
// ��ע(�����汾֮����޸�):
//   
//-----------------------------
void animationTest(void)
{
	std::ofstream animationTestLogFile;
	animationTestClass animationTest;
	animationTestLogFile.open("testLog\\animationTest.Log",std::ios::out);
	if(!animationTestLogFile.is_open())
	{
		ERROR_PRINT("ERROR in animation test: connot open file buttonTest.Log");
		return;
	}
	animationTest.createAnimationTest(&animationTestLogFile);
	animationTest.animationTaskQueueHandleTest(&animationTestLogFile);
	animationTest.animationMainTainTest(&animationTestLogFile);
	animationTest.animationActionTest(&animationTestLogFile);

}

//-----------------------------
// �������� createAnimationTest
// ����createAnimation����
// �����б�
// ��ע(�����汾֮����޸�):
//   
//-----------------------------
void animationTestClass::createAnimationTest(std::ofstream* pAnimationTestLogFile)
{
	u8 ahmi_func_status = AHMI_FUNC_SUCCESS;
	u8           A_RefreshType;  //Ԫ������   
	ElementPtr   A_New_ElementPtr; //��Ԫ��ָ��
	ElementPtr   A_Old_ElementPtr; //��Ԫ��ָ��
	ERROR_PRINT("testing create animation function\n");
	*pAnimationTestLogFile << "testing create animation function\n";

	ERROR_PRINT("test illegal data\n");
	*pAnimationTestLogFile << "test illegal data\n";

	//illegal input
	ERROR_PRINT("test illegal input\n");
	*pAnimationTestLogFile << "test illegal input\n";
	//set refresh type
	ERROR_PRINT("set refresh Type out of boundary\n");
	*pAnimationTestLogFile << "set refresh Type out of boundary\n";
	A_RefreshType = 50;
	A_New_ElementPtr.pageptr = gPagePtr;
	A_Old_ElementPtr.pageptr = gPagePtr;
	gAnimationClass.Create_AnimatianData(A_RefreshType, A_New_ElementPtr, A_Old_ElementPtr);
	//set element pointer
	ERROR_PRINT("set element pointer out of boundary or null\n");
	*pAnimationTestLogFile << "set element pointer out of boundary of null\n";
	A_RefreshType = ANIMATION_REFRESH_PAGE;
	A_New_ElementPtr.cptr = &gPagePtr[WorkingPageID].pCanvasList[0];
	A_Old_ElementPtr.cptr = &gPagePtr[WorkingPageID].pCanvasList[0];
	gAnimationClass.Create_AnimatianData(A_RefreshType, A_New_ElementPtr, A_Old_ElementPtr);
	A_RefreshType = ANIMATION_REFRESH_PAGE;
	A_New_ElementPtr.cptr = &gPagePtr[WorkingPageID].pCanvasList[0];
	A_Old_ElementPtr.cptr = &gPagePtr[WorkingPageID].pCanvasList[0];
	gAnimationClass.Create_AnimatianData(A_RefreshType, A_New_ElementPtr, A_Old_ElementPtr);
	A_RefreshType = ANIMATION_REFRESH_PAGE;
	A_New_ElementPtr.pageptr = &gPagePtr[WorkingPageID + 10];
	A_Old_ElementPtr.pageptr = &gPagePtr[WorkingPageID + 10];
	gAnimationClass.Create_AnimatianData(A_RefreshType, A_New_ElementPtr, A_Old_ElementPtr);
	A_RefreshType = ANIMATION_REFRESH_CANVAS;
	A_New_ElementPtr.cptr = &gPagePtr[WorkingPageID].pCanvasList[10];
	A_Old_ElementPtr.cptr = &gPagePtr[WorkingPageID].pCanvasList[10];
	gAnimationClass.Create_AnimatianData(A_RefreshType, A_New_ElementPtr, A_Old_ElementPtr);
	A_RefreshType = ANIMATION_REFRESH_SUBCANVAS;
	A_New_ElementPtr.cptr = &gPagePtr[WorkingPageID].pCanvasList[10];
	A_Old_ElementPtr.cptr = &gPagePtr[WorkingPageID].pCanvasList[10];
	gAnimationClass.Create_AnimatianData(A_RefreshType, A_New_ElementPtr, A_Old_ElementPtr);
	A_RefreshType = ANIMATION_REFRESH_WIDGET;
	A_New_ElementPtr.wptr = &gPagePtr[WorkingPageID].pWidgetList[10];
	A_Old_ElementPtr.wptr = &gPagePtr[WorkingPageID].pWidgetList[10];
	gAnimationClass.Create_AnimatianData(A_RefreshType, A_New_ElementPtr, A_Old_ElementPtr);

	*pAnimationTestLogFile << "check whether the trace window print error information. only when error information is printed is this test passed\n";
	

	//boundary
	ERROR_PRINT("test boundary data\n");
	*pAnimationTestLogFile << "test boundary data\n";
	for(int i = 0; i < 10; i++)
	{
		A_RefreshType = ANIMATION_REFRESH_PAGE;
		A_New_ElementPtr.pageptr = gPagePtr;
		A_Old_ElementPtr.pageptr = gPagePtr;
		gAnimationClass.Create_AnimatianData(A_RefreshType, A_New_ElementPtr, A_Old_ElementPtr);
		A_RefreshType = ANIMATION_REFRESH_CANVAS;
		A_New_ElementPtr.cptr = &gPagePtr[WorkingPageID].pCanvasList[0];
		A_Old_ElementPtr.cptr = &gPagePtr[WorkingPageID].pCanvasList[0];
		gAnimationClass.Create_AnimatianData(A_RefreshType, A_New_ElementPtr, A_Old_ElementPtr);
		A_RefreshType = ANIMATION_REFRESH_SUBCANVAS;
		A_New_ElementPtr.cptr = &gPagePtr[WorkingPageID].pCanvasList[0];
		A_Old_ElementPtr.cptr = &gPagePtr[WorkingPageID].pCanvasList[0];
		gAnimationClass.Create_AnimatianData(A_RefreshType, A_New_ElementPtr, A_Old_ElementPtr);
		A_RefreshType = ANIMATION_REFRESH_WIDGET;
		A_New_ElementPtr.wptr = &gPagePtr[WorkingPageID].pWidgetList[0];
		A_Old_ElementPtr.wptr = &gPagePtr[WorkingPageID].pWidgetList[0];
		gAnimationClass.Create_AnimatianData(A_RefreshType, A_New_ElementPtr, A_Old_ElementPtr);
	}
	ERROR_PRINT("boundary data test passed\n");
	*pAnimationTestLogFile << "test boundary data test passed\n";



}


//-----------------------------
// �������� animationTaskQueueHandleTest
// ����animationTaskQueueHandleTest����
// �����б�
// ��ע(�����汾֮����޸�):
//   
//-----------------------------
void animationTestClass::animationTaskQueueHandleTest(std::ofstream* pAnimationTestLogFile)
{
	u8 ahmi_func_status = AHMI_FUNC_SUCCESS;
	animationTaskQueueMsg i; //the number of the array
	u8           A_RefreshType;  //Ԫ������   
	ElementPtr   A_New_ElementPtr; //��Ԫ��ָ��
	ElementPtr   A_Old_ElementPtr; //��Ԫ��ָ��
	ERROR_PRINT("testing animationTaskQueueHandleTest function\n");
	*pAnimationTestLogFile << "testing animationTaskQueueHandleTest function\n";

	ERROR_PRINT("test illegal data\n");
	*pAnimationTestLogFile << "test illegal data\n";

	//illegal input
	ERROR_PRINT("test illegal input\n");
	*pAnimationTestLogFile << "test illegal input\n";
	//set refresh type
	ERROR_PRINT("set number out of boundary\n");
	*pAnimationTestLogFile << "set number out of boundary\n";
	gAnimationClass.animationTaskQueueHandle(20);

	*pAnimationTestLogFile << "check whether the trace window print error information. only when error information is printed is this test passed\n";
	

	//boundary
	ERROR_PRINT("test boundary data\n");
	*pAnimationTestLogFile << "test boundary data\n";
	gAnimationClass.animationQueueReset();
	//create the animation data
	for(i = 0; i < 10; i++)
	{
		//A_RefreshType = ANIMATION_REFRESH_PAGE;
		//A_New_ElementPtr.pageptr = gPagePtr;
		//A_Old_ElementPtr.pageptr = gPagePtr;
		//gAnimationClass.Create_AnimatianData(A_RefreshType, A_New_ElementPtr, A_Old_ElementPtr);
		A_RefreshType = ANIMATION_REFRESH_CANVAS;
		A_New_ElementPtr.cptr = &gPagePtr[WorkingPageID].pCanvasList[0];
		A_Old_ElementPtr.cptr = &gPagePtr[WorkingPageID].pCanvasList[0];
		gAnimationClass.Create_AnimatianData(A_RefreshType, A_New_ElementPtr, A_Old_ElementPtr);
		A_RefreshType = ANIMATION_REFRESH_SUBCANVAS;
		A_New_ElementPtr.cptr = &gPagePtr[WorkingPageID].pCanvasList[0];
		A_Old_ElementPtr.cptr = &gPagePtr[WorkingPageID].pCanvasList[0];
		gAnimationClass.Create_AnimatianData(A_RefreshType, A_New_ElementPtr, A_Old_ElementPtr);
		A_RefreshType = ANIMATION_REFRESH_WIDGET;
		A_New_ElementPtr.wptr = &gPagePtr[WorkingPageID].pWidgetList[0];
		A_Old_ElementPtr.wptr = &gPagePtr[WorkingPageID].pWidgetList[0];
		gAnimationClass.Create_AnimatianData(A_RefreshType, A_New_ElementPtr, A_Old_ElementPtr);
	}
	//handler the data
	for(i = 0; i < AnimationMsg_Maxnum; i++)
		gAnimationClass.animationTaskQueueHandle(i);

	ERROR_PRINT("boundary data test passed\n");
	*pAnimationTestLogFile << "test boundary data test passed\n";
}


//-----------------------------
// �������� animationMainTainTest
// ����animationMainTainTest����
// �����б�
// ��ע(�����汾֮����޸�):
//   
//-----------------------------
void animationTestClass::animationMainTainTest(std::ofstream* pAnimationTestLogFile)
{
	u8 ahmi_func_status = AHMI_FUNC_SUCCESS;
	u8           A_RefreshType;  //Ԫ������   
	ElementPtr   A_New_ElementPtr; //��Ԫ��ָ��
	ElementPtr   A_Old_ElementPtr; //��Ԫ��ָ��
	ERROR_PRINT("testing animationMainTain function\n");
	*pAnimationTestLogFile << "testing animationMainTain function\n";

	//boundary
	ERROR_PRINT("test boundary data\n");
	*pAnimationTestLogFile << "test boundary data\n";
	gAnimationClass.animationQueueReset();
	//create the animation data
	for(int i = 0; i < 255; i++)
	{
		//A_RefreshType = ANIMATION_REFRESH_PAGE;
		//A_New_ElementPtr.pageptr = gPagePtr;
		//A_Old_ElementPtr.pageptr = gPagePtr;
		//gAnimationClass.Create_AnimatianData(A_RefreshType, A_New_ElementPtr, A_Old_ElementPtr);
		A_RefreshType = ANIMATION_REFRESH_CANVAS;
		A_New_ElementPtr.cptr = &gPagePtr[WorkingPageID].pCanvasList[0];
		A_Old_ElementPtr.cptr = &gPagePtr[WorkingPageID].pCanvasList[0];
		gAnimationClass.Create_AnimatianData(A_RefreshType, A_New_ElementPtr, A_Old_ElementPtr);
		A_RefreshType = ANIMATION_REFRESH_SUBCANVAS;
		A_New_ElementPtr.cptr = &gPagePtr[WorkingPageID].pCanvasList[0];
		A_Old_ElementPtr.cptr = &gPagePtr[WorkingPageID].pCanvasList[0];
		gAnimationClass.Create_AnimatianData(A_RefreshType, A_New_ElementPtr, A_Old_ElementPtr);
		A_RefreshType = ANIMATION_REFRESH_WIDGET;
		A_New_ElementPtr.wptr = &gPagePtr[WorkingPageID].pWidgetList[0];
		A_Old_ElementPtr.wptr = &gPagePtr[WorkingPageID].pWidgetList[0];
		gAnimationClass.Create_AnimatianData(A_RefreshType, A_New_ElementPtr, A_Old_ElementPtr);
	}
	//handler the data
	for(int i = 0; i < 255; i++)
		gAnimationClass.Animation_maintain(i);

	ERROR_PRINT("boundary data test passed\n");
	*pAnimationTestLogFile << "test boundary data test passed\n";
}

void animationTestClass::animationActionTest(std::ofstream* pAnimationTestLogFile)
{
	u8 ahmi_func_status = AHMI_FUNC_SUCCESS;
	DynamicPageClassPtr pPage;
	CanvasClassPtr pCanvas;
	SubCanvasClassPtr pSubcanvas;
	WidgetClassPtr pWidget;
	u8 aTag;


	ERROR_PRINT("testing animation action\n");
	*pAnimationTestLogFile << "testing animation action\n";

	ERROR_PRINT("test illegal data\n");
	*pAnimationTestLogFile << "test illegal data\n";

	//illegal input
	ERROR_PRINT("test illegal input\n");
	*pAnimationTestLogFile << "test illegal input\n";
	//set page animation tag
	ERROR_PRINT("set page animation tag out of boundary\n");
	*pAnimationTestLogFile << "set page animation tag out of boundary\n";
	pPage = gPagePtr;
	pPage->setATag(100);
	pPage->setATag(0);
	//set canvas animation tag
	ERROR_PRINT("set canvas animation tag out of boundary\n");
	*pAnimationTestLogFile << "set canvas animation tag out of boundary\n";
	pCanvas = &gPagePtr[WorkingPageID].pCanvasList[0];
	pCanvas->curCustomAnimationPtr = 0;
	pCanvas->setATag(100);
	pCanvas->setATag(0);
	//set subcanvas animation tag
	ERROR_PRINT("set subcanvas animation tag out of boundary\n");
	*pAnimationTestLogFile << "set subcanvas animation tag out of boundary\n";
	pSubcanvas = &gPagePtr[WorkingPageID].pSubCanvasList[0];
	pSubcanvas->setAtag(100);
	pSubcanvas->setAtag(0);
	//set widget animation tag
	ERROR_PRINT("set widget animation tag out of boundary\n");
	*pAnimationTestLogFile << "set widget animation tag out of boundary\n";
	pWidget = &gPagePtr[WorkingPageID].pWidgetList[0];
	pWidget->setATag(100);
	pWidget->setATag(0);

	*pAnimationTestLogFile << "check whether the trace window print error information. only when error information is printed is this test passed\n";
	

	//boundary
	ERROR_PRINT("test boundary data\n");
	*pAnimationTestLogFile << "test boundary data\n";
	for(aTag = 1; aTag <= pPage->mtotoalFrame; aTag++)
	{
		if(pPage->setATag(aTag) == AHMI_FUNC_FAILURE)
			ahmi_func_status = AHMI_FUNC_FAILURE;
	}
	
	for(aTag = 1; aTag <= gPagePtr[WorkingPageID].pAnimationActionList[0].mTotalFrame; aTag++)
	{
		if(pCanvas->setATag(aTag) == AHMI_FUNC_FAILURE)
			ahmi_func_status = AHMI_FUNC_FAILURE;
	}
	for(aTag = 1; aTag <= pCanvas->subcanvasAnimationFrame; aTag++)
	{
		if(pSubcanvas->setAtag(aTag) == AHMI_FUNC_FAILURE)
			ahmi_func_status = AHMI_FUNC_FAILURE;
	}
	for(aTag = 1; aTag <= ( (pWidget->TOTALFRAME_AND_NOWFRAME) >> 8); aTag++)
	{
		if(pWidget->setATag(aTag) == AHMI_FUNC_FAILURE)
			ahmi_func_status = AHMI_FUNC_FAILURE;
	}

	ERROR_PRINT("boundary data test passed\n");
	*pAnimationTestLogFile << "test boundary data test passed\n";
}


#endif
