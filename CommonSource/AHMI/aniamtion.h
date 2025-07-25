////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:   2016/03/21
// File Name:     animation.h
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2016/03/21 by Zhou Yuzhi 
// Additional Comments:
//    
// 
////////////////////////////////////////////////////////////////////////////////

#ifndef ANIMATION__H
#define ANIMATION__H
#include "publicType.h"
#include "publicDefine.h"
#include "animationDefine.h"
#include "refreshQueueHandler.h"


#ifdef AHMI_CORE

#ifdef PC_SIM
#define AnimationIntervalTime 10
#endif
#ifdef EMBEDDED
#define AnimationIntervalTime 30
#endif

union ElementPtr;
struct Animation;
//typedef void * TimerHandle_t;

typedef enum
{
    EM_ErrRetAnimation_NULLPointerErr = 1,
    EM_ErrRetAnimation_DataOverrun,
    EM_ErrRetAnimation_RefreshTypeErr,
    EM_ErrRetAnimation_SpaceOverrun,
	EM_ErrRetAnimation_QueueSendErr
   
}EM_ErrRetAnimation;

class AnimationClass
{
	//变量
private:
	AnimationMsgPtr pAnimationMsgPtr;
//	TimerHandle_t   aTimer;   //动画时钟

	//函数
public:
	//-----------------------------
	// name： initAnimationMsg
	// 初始化队列
	//  
	// addtional comments :
	//   none
	//-----------------------------
	u32 initAnimationMsg();


	//-----------------------------
	// name： initAnimationTimer
	// 初始化队列
	//  
	// addtional comments :
	//   none
	//-----------------------------
	//funcStatus initAnimationTimer(u16 TimerID);

	//-----------------------------
	// name： Create_AnimatianData
	// 创建动画列表
	//  
	// @param1 u8 A_RefreshType      元件类型，宏定义见"animationDefine.h"
	// @prarm2 ElementPtr A_New_ElementPtr 新绘制元件指针
	// @parma3  ElementPtr   A_Old_ElementPtr  旧元件指针
	// addtional comments :
	//   none
	//-----------------------------
	u32 Create_AnimatianData(	
		u8           A_RefreshType,    //元件类型   
		ElementPtr   A_New_ElementPtr, //新元件指针
		ElementPtr   A_Old_ElementPtr  //旧元件指针
		); 


	//-----------------------------
	// name： Get_AnimationData_Index
	// 创建动画列表
	//  
	// @param1 u8 A_RefreshType      元件类型，宏定义见"animationDefine.h"
	// @prarm2 ElementPtr A_New_ElementPtr 新绘制元件指针
	// @parma3  ElementPtr   A_Old_ElementPtr  旧元件指针
	// addtional comments :
	//   none
	//-----------------------------
	s32 Get_AnimatianData_Index(	
		u8           A_RefreshType,    //元件类型   
		ElementPtr   A_New_ElementPtr, //新元件指针
		ElementPtr   A_Old_ElementPtr  //旧元件指针
		); 
    
	//-----------------------------
	// name�� clearAnimationWithIdx
	// clear animation
	//  
	// addtional comments :
	//   none
	//-----------------------------
	funcStatus clearAnimationWithIdx(	
		s32 idx
		); 

    //-----------------------------
	// name: AnimationTrigger
	//
	//  
	// @param1 uint32_t refreshTime      元件类型，宏定义见"animationDefine.h"
	// addtional comments :
	//   none
	//-----------------------------    
    void AnimationTrigger(uint32_t refreshTime);

	//-----------------------------
	// name: Animation_maintain
	// 维护动画列表
	//  
	// @param1 u8 A_RefreshType      元件类型，宏定义见"animationDefine.h"
	// @prarm2 ElementPtr A_New_ElementPtr 新绘制元件指针
	// @parma3  ElementPtr   A_Old_ElementPtr  旧元件指针
	// addtional comments :
	//   none
	//-----------------------------
	funcStatus Animation_maintain(uint32_t id, uint32_t refreshTime);

	//-----------------------------
	// name： animationTaskQueueHandle
	// 动画队列处理任务
	//  
	//     @param1 animationTaskQueueMsg i 队列消息
	// addtional comments :
	//   
	//-----------------------------
	u32 animationTaskQueueHandle(animationTaskQueueMsg i);

	//-----------------------------
	// name： executeAnimationWithElapsedTime
	// 动画队列处理任务
	//  
	//     @param1 animationTaskQueueMsg i 队列消息
	// addtional comments :
	//   
	//-----------------------------
	funcStatus executeAnimationWithElapsedTime(animationTaskQueueMsg i);

	//-----------------------------
	// name： animationFinishHanlde
	// 处理动画完成
	//  
	//     @param1 animationTaskQueueMsg i 队列消息
	// addtional comments :
	//   
	//-----------------------------
	u32 animationFinishHanlde(animationTaskQueueMsg i);

	//-----------------------------
	// name： animationQueueReset
	// 动画队列清空
	//  
	//     void
	// addtional comments :
	//   
	//-----------------------------
	funcStatus animationQueueReset(void);
    
    //-----------------------------
	// name: sendPlayerInstruction
	// send instruction to player command queue
	//  
	//     uint32_t
	// addtional comments :
	//   
	//-----------------------------
	funcStatus sendPlayerInstruction(uint32_t id, uint32_t refreshTime, uint32_t nowtime);

    u32 getAnimationRefreshType(u32 id);
	
	WidgetClassPtr getAnimationRefreshElementPtr(u32 id);
};



#endif

#endif


