////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:   2015/11/17
// File Name:     ActionTriggerClass.h
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2015/11/17 by...
// Revision 2.04 - C++, 20160321 by ÓÚ´ºÓª
// Additional Comments:
//    
// 
////////////////////////////////////////////////////////////////////////////////

#ifndef ACTION_TRIGGER_CLASS__H
#define ACTION_TRIGGER_CLASS__H
#include "publicType.h"
#include "ActionTriggerClass_c1.h"
#include "AHMIBasicStructure.h"
#include "ActionTriggerClass_cd.h"
#include "publicDefine.h"

#ifdef AHMI_CORE
class ActionTriggerClass {
public:
    PIDState mMousePID;				//mouse pointer, only used when mouse sends action
    TagClassPtr mTagPtr;			    //tag pointer, used when tag change action occurs
	u8	mInputType;					//the type of action
	                                //0 none action
                                    //1 MousePress
                                    //2 MouseRelease
	                                //3 MouseHolding
                                    //4 MouseCancle 
                                    //5 KeyboardPre 
                                    //6 KeyboardNext
                                    //7 KeyboardOK
                                    //8 TagSetValue

	//construct and deconstruct
	ActionTriggerClass();
	~ActionTriggerClass();

//-----------------------------
// name£º initActionTriggerClass
// initialize the class
//  
//   none
// addtional comments :
//   none
//-----------------------------
	funcStatus initActionTrigger(void);

	//Mouse/Touch action event
	u32 MouseTouch(PIDPoint* pPressPoint,PIDPoint* pReleasePoint,  RefreshScreenTypes screenType);
    
	//KeyBoard action event
	u32 keyboardTouch();

	u32 triggerWidgetHighlight(WidgetClassPtr widgetClassPtr);
	
	u32 widgetActionTrigger(
		DynamicPageClassPtr pageptr,
		 WidgetClassPtr pWidgetPtr,
		 u8 u8_widgetRefresh    //added by zuz 20160622
		 );
         
    u32 widgetMouseTouchActionTrigger(
		DynamicPageClassPtr pageptr,
        WidgetClassPtr pWidgetPtr,
        PIDPoint* pPressPoint,
        PIDPoint* pReleasePoint,
		PIDPoint* pAbsolutePressPoint,
        PIDPoint* pAbsoluteReleasePoint,
        u8 u8_widgetRefresh    //
		 );
         
//    funcStatus widgetKeyBoardActionTrigger(
//        WidgetClassPtr pWidgetPtr,
//        u8 u8_widgetRefresh    //
//		 );
};

typedef class ActionTriggerClass* ActionTriggerClassPtr;

void initTagTrigger(void);

#endif

#endif

