////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:   2015/11/17
// File Name:     ActionProcessor.h
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2015/11/17 by...
// Revision 2.04 - C++, 20160321 by YuChunYing
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

#ifndef  ACTIONPROCESSOR__H
#define  ACTIONPROCESSOR__H

#include "publicDefine.h"
#include  "publicType.h"
#ifdef PC_SIM
#include <iostream>
#include <fstream>
#endif

#ifdef AHMI_CORE
//**********************************
// Instruction Type for source1&source2
// opCode: 14~15bit  @see following 
//**********************************
#define		SOURCE1IMM_SOURCE2IMM			0x0000
#define		SOURCE1TAG_SOURCE2IMM			0x4000 
#define		SOURCE1IMM_SOURCE2TAG			0x8000
#define		SOURCE1TAG_SOURCE2TAG			0xC000

#define		SOURCE1_IMM				0x0000
#define		SOURCE1_TAG				0x4000 
#define		SOURCE2_IMM				0x0000
#define		SOURCE2_TAG				0x8000

//**********************************
// Instruction Format
//
// {u8_CdRun ,u8_CdRCd ,opCode}
// u8_CdRun : 1bit whether to run this code
// u8_CdRCd : 1bit the status of running this code
// opCode: 14bit  @see following 
//**********************************
#define		END_CODE				0x0000	//End
#define		BLANK_CODE				0x0001	//blank 
#define		JUMP_CODE				0x0002	//jump to instruction

#define		SET_CODE				0x0003	//Set Number Tag
#define		SET_STR_CODE			0x0004  //Set String Tag
#define		INC_CODE   				0x0005	//Inc Tag by Tag  
#define		DEC_CODE   				0x0006	//Dec Tag by Tag  
#define		MUL_CODE   				0x0007	//Mul Tag by Tag     
#define		DIV_CODE   				0x0008	//Div Tag by Tag   
#define		MOD_CODE   				0x0009   //Mod Tag by Tag     
#define		AND_CODE   				0x000a	//And Tag by Tag   
#define     OR_CODE  				0x000b	//Or Tag by Tag     
#define		XOR_CODE   				0x000c	//Xor Tag by Tag   
#define		NOT_CODE   				0x000d	//Not Tag   
#define     SL_CODE     			0x000e	//SL Tag by Tag     
#define     SR_CODE     			0x000f	//SR Tag by Tag     
#define		SETIMM_CODE				0x0010	//Set Tag by Imm
#define		INCIMM_CODE				0x0011	//Inc Tag by Imm
#define		DECIMM_CODE				0x0012	//Dec Tag by Imm
#define		MULIMM_CODE				0x0013	//Mul Tag by Imm
#define		DIVIMM_CODE				0x0014	//Div Tag by Imm
#define		MODIMM_CODE				0x0015  //Mod Tag by Imm
#define		ANDIMM_CODE				0x0016	//And Tag by Imm
#define     ORIMM_CODE 				0x0017	//Or Tag by Imm
#define		XORIMM_CODE				0x0018	//Xor Tag by Imm
#define     SLIMM_CODE 				0x0019	//SL Tag by Imm
#define     SRIMM_CODE 				0x001a	//SR Tag by Imm

#define     BRCHEQU_CODE    		0x001b	//BranchEQU Tag to Tag
#define		BRCHEQUIMM_CODE 		0x001c	//BranchEQU Tag to Imm
#define     BRCHNEQ_CODE    		0x001d	//BranchNEQ Tag to Tag
#define     BRCHNEQIMM_CODE 		0x001e	//BranchNEQ Tag to Imm
#define     BRCHMORE_CODE   		0x001f	//BranchMore Tag to Tag
#define		BRCHMOREIMM_CODE		0x0020	//BranchMore Tag to Imm
#define     BRCHLESS_CODE   		0x0021	//BranchLess Tag to Tag
#define		BRCHLESSIMM_CODE		0x0022	//BranchLess Tag to Imm

#define		RAND_INT_CODE			0x0023	//Rand int num to Tag

#define		GOTO_CODE				0x0040	//Goto u8_page  
#define		RET_CODE				0x0041	//Return u8_page
#define		DELAY_CODE				0x0042	//delay
#define		DELAY_TAG_CODE			0x0043	//delay

#define		ANIMATION_CODE			0x0044  //trigger animation
#define		ANIMATION_TAG_CODE		0x0045  //trigger animation of tag

#define		SET_TIMER_CODE			0x0046  //set timer
#define		SET_TIMER_TAG_CODE		0x0047  //set timer with tag as second param

#define		SET_COLOR_CODE			0x0048  //set color by tag value
#define		SET_COLOR_IMM_CODE		0x0049  //set color by imm
#define		SET_KB_CODE				0x004a  //set kb by tag value
#define		SET_KB_IMM_CODE			0x004b  //set kb by imm
#define		GET_KB_CODE				0x004c  //get kb

#define		KEYBOARD_LEFT_CODE		0x004d
#define		KEYBOARD_RIGHT_CODE		0x004e
#define		KEYBOARD_ENTERED_CODE	0x004f

#define		SET_MOVE_CODE						0x0060
#define		SET_ZOOM_CODE						0x0061
#define		SET_ROTATION_CENTER_CODE			0x0062
#define		SET_ROTATION_ANGLE_CODE				0x0063
#define		SET_PROJECTION_CODE					0x0064
#define		SET_MOVE_TAG_CODE					0x0065
#define		SET_ZOOM_TAG_CODE					0x0066
#define		SET_ROTATION_CENTER_TAG_CODE		0x0067
#define		SET_ROTATION_ANGLE_TAG_CODE			0x0068
#define		SET_PROJECTION_TAG_CODE				0x0069
#define		CHANGE_MOVE_CODE					0x0070
#define		CHANGE_ZOOM_CODE					0x0071
#define		CHANGE_ROTATION_CENTER_CODE			0x0072
#define		CHANGE_ROTATION_ANGLE_CODE			0x0073
#define		CHANGE_PROJECTION_CODE				0x0074
#define		CHANGE_MOVE_TAG_CODE				0x0075
#define		CHANGE_ZOOM_TAG_CODE				0x0076
#define		CHANGE_ROTATION_CENTER_TAG_CODE		0x0077
#define		CHANGE_ROTATION_ANGLE_TAG_CODE		0x0078
#define		CHANGE_PROJECTION_TAG_CODE			0x0079
#define		SET_MOVE_X_CODE						0x0080
#define		SET_MOVE_Y_CODE						0x0081
#define		SET_ZOOM_X_CODE						0x0082
#define		SET_ZOOM_Y_CODE						0x0083
#define		SET_ROTATION_CENTER_X_CODE			0x0084
#define		SET_ROTATION_CENTER_Y_CODE			0x0085
#define		SET_PROJECTION_X_CODE				0x0086
#define		SET_PROJECTION_Y_CODE				0x0087
#define		SET_MOVE_X_TAG_CODE					0x0088
#define		SET_MOVE_Y_TAG_CODE					0x0089
#define		SET_ZOOM_X_TAG_CODE					0x008A
#define		SET_ZOOM_Y_TAG_CODE					0x008B
#define		SET_ROTATION_CENTER_X_TAG_CODE		0x008C
#define		SET_ROTATION_CENTER_Y_TAG_CODE		0x008D
#define		SET_PROJECTION_X_TAG_CODE			0x008E
#define		SET_PROJECTION_Y_TAG_CODE			0x008F
#define		CHANGE_MOVE_X_CODE					0x0090
#define		CHANGE_MOVE_Y_CODE					0x0091
#define		CHANGE_ZOOM_X_CODE					0x0092
#define		CHANGE_ZOOM_Y_CODE					0x0093
#define		CHANGE_ROTATION_CENTER_X_CODE		0x0094
#define		CHANGE_ROTATION_CENTER_Y_CODE		0x0095
#define		CHANGE_PROJECTION_X_CODE			0x0096
#define		CHANGE_PROJECTION_Y_CODE			0x0097
#define		CHANGE_MOVE_X_TAG_CODE				0x0098
#define		CHANGE_MOVE_Y_TAG_CODE				0x0099
#define		CHANGE_ZOOM_X_TAG_CODE				0x009A
#define		CHANGE_ZOOM_Y_TAG_CODE				0x009B
#define		CHANGE_ROTATION_CENTER_X_TAG_CODE	0x009C
#define		CHANGE_ROTATION_CENTER_Y_TAG_CODE	0x009D
#define		CHANGE_PROJECTION_X_TAG_CODE		0x009E
#define		CHANGE_PROJECTION_Y_TAG_CODE		0x009F
#define		SET_SCALE_CENTER_CODE			0x00A0
#define		SET_SCALE_CENTER_TAG_CODE		0x00A1
#define		SET_SCALE_CENTER_X_CODE			0x00A2
#define		SET_SCALE_CENTER_X_TAG_CODE		0x00A3
#define		SET_SCALE_CENTER_Y_CODE			0x00A4
#define		SET_SCALE_CENTER_Y_TAG_CODE		0x00A5
#define		CHANGE_SCALE_CENTER_CODE			0x00A6
#define		CHANGE_SCALE_CENTER_TAG_CODE		0x00A7
#define		CHANGE_SCALE_CENTER_X_CODE			0x00A8
#define		CHANGE_SCALE_CENTER_X_TAG_CODE		0x00A9
#define		CHANGE_SCALE_CENTER_Y_CODE			0x00AA
#define		CHANGE_SCALE_CENTER_Y_TAG_CODE		0x00AB

#define		PLAY_SOUND_CODE			0x0100	//play sound by imm
#define		PAUSE_SOUND_CODE		0x0101	//pause sound by imm
#define		RESUME_SOUND_CODE		0x0102	//resume sound by imm
#define		STOP_SOUND_CODE			0x0103	//stop sound by imm
#define		PLAY_SOUND_TAG_CODE		0x0104	//play sound by tag
#define		PAUSE_SOUND_TAG_CODE	0x0105	//pause sound by tag
#define		RESUME_SOUND_TAG_CODE	0x0106	//resume sound by tag
#define		STOP_SOUND_TAG_CODE		0x0107	//stop sound by tag
#define		CLEAR_DATA_CODE			0x0108	//clear chart data by tag 
#define     SET_ARR_CODE            0x0109  //set array 

#define		WRITE_LOG_CODE			0x0200	//write log (two tags value)
#define		WRITE_REG_CODE			0x0201  //write reg
#define		WRITE_FLASH_CODE		0x0202  //write flash

#define		COPY_BACKGROUND_1_CODE	0x0203
#define		COPY_BACKGROUND_2_CODE	0x0204
#define		PASTE_BACKGROUND_CODE	0x0205

#define		READ_ERRMESSAGE_CODE	0x0206

#define    SET_POINTER_TAG_IMM_CODE     0x0207

#define    SET_POINTER_TAG_TAG_CODE     0x0208
#define     SET_KB_HINT_CODE        0x0209
#define     SET_KB_HINT_IMM_CODE    0x020a

#define     PREV_KB_HINTS_CODE        0x020b
#define     PREV_KB_HINTS_IMM_CODE    0x020c

#define     NEXT_KB_HINTS_CODE        0x020d
#define     NEXT_KB_HINTS_IMM_CODE    0x020e

#define     APPEND_ARR_CODE           0x020f
#define     DEL_ARR_BY_IDX_CODE       0x0210
#define     DEL_ARR_BY_IDX_IMM_CODE   0x0211
#define     GET_ARRAY_ELEM_CODE       0x0212

#define     SNAPSHOT_CODE			0x0213
#define     BLUR_SNAPSHOT_CODE		0x0214
#define     BLUR_SNAPSHOT_TAG_CODE	0x0215
#define     CLEAR_SNAPSHOT_CODE		0x0216

#define		RWDATA_CODE				0x00ff  //read and write external data //not used

#define     ACTIONEXE_CODE_MUSK         0x0000FFFF
#define     MODIFY_VIEW_ELEMENT_MUSK    0xFFFF0000
#define     MODIFY_VIEW_ELEMENT_ONE     0x00000
#define     MODIFY_VIEW_ELEMENT_X       0x10000
#define     MODIFY_VIEW_ELEMENT_Y       0x20000

typedef enum
{
    EM_ErrRetActionProcessor_LengthOverrun = 1,
    EM_ErrRetActionProcessor_GotoErrorPage,
    EM_ErrRetActionProcessor_NoIdleForDelay,
    EM_ErrRetActionProcessor_TagIDOverrun,
    EM_ErrRetActionProcessor_TagTypeErr,
	EM_ErrRetActionProcessor_DataFrameErr,
    EM_ErrRetActionProcessor_SoundIDOverrun,
	EM_ErrRetActionProcessor_ElementIndexErr,
	EM_ErrRetActionProcessor_NULLPointerErr,
	EM_ErrRetActionProcessor_MoveOverrun,
	EM_ErrRetActionProcessor_ZoomOverrun,
	EM_ErrRetActionProcessor_RotateCenterOverrun,
	EM_ErrRetActionProcessor_ScaleCenterOverrun,
	EM_ErrRetActionProcessor_QueueSendErr
   
}EM_ErrRetActionProcessor;


/** 
  * @action frame format
  */
typedef struct
{
  uint16_t Opcode;
  uint16_t DestID;
  uint16_t SourceID1;
  uint16_t SourceID2;
} ActionFormat_TagID;

//typedef struct
//{
//  uint16_t Opcode;
//  uint16_t DestID;
//  uint32_t ImmNumber;
//} ActionFormat_Imm;

#define  ActionInstructionQueueSize		80

#define  ACTIONPROCESSOR_CONTINUE	0

#define  ActionFormat_Length		8


#define   DEPTH   10
#define   PARANUMBER   32

typedef struct
{
  uint16_t SourcePageID;
  uint16_t DestWidgetID;
  uint16_t LTPointX;
  uint16_t LTPointY;
  uint16_t RBPointX;
  uint16_t RBPointY;
  uint16_t PastePointX;
  uint16_t PastePointY;
} CopyPaste_Struct;

/**
* class for handling the action triggered by widget
* get data from ActionInstructionQueue
* runs until END_CODE action
*/
class ActionProcessorClass{
public:
	u8* pPC;              //pointer of PC
	u8	mStatusRegister;  //the result of comparsion
	u8	mStackPtr;        //the pointer of stack
	u8	mPosOfPageParaStack; //pointer for stack of page parameters
	u16	mPageStack[DEPTH];   //statck for page ID, used in return code
	u16 mPageParaStack[PARANUMBER]; //statck for page parameters

	ActionProcessorClass();
	~ActionProcessorClass();

	//actionָ���: 
//the function for page action
	// @param u8_CdRun : 1bit whether to run this code
    // @param u8_CdRCd : 1bit the status of running this code
	// @param u8 u8_page : the ID of page
	// @param u8 u8_nPara : the parameters of page
	// @param u16 u16_WidgetID : the ID of widget. specify the return value to which widget.
	u32 GotoPage   (u8 u8_CdRun , u8 u8_CdRCd , u16 u8_page,u8 u8_nPara,u16 u16_WidgetID); 
    u32 GotoPageExe(u16 u8_page,u8 u8_nPara,u16 u16_WidgetID);
	void SetPointerTag(u8 u8_CdRun , u8 u8_CdRCd , u32 tagRegister, u16 workingPageId, bool isBeforeLoadAction = false);
	u32 Delay   (u8 u8_CdRun , u8 u8_CdRCd , u32 instructionAddr,u16 delayTime, u16 workingPageId); 
	void ReturnPage (u8 u8_CdRun , u8 u8_CdRCd , u8 *pretPageID);
    void ReturnPageExe(void);
//function for tag action	
	// two format, FUNC TAG IMM or FUNC TAG TAG
	u32 SetTag   (u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagClassListTag, u16 SourceTagClassListTag, bool isBeforeLoadAction = false);
	u32 RandTag  (u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet, bool isBeforeLoadAction = false);
	u32 IncTag   (u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet, bool isBeforeLoadAction = false);
	u32 DecTag   (u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet, bool isBeforeLoadAction = false);
	u32 MulTag   (u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet, bool isBeforeLoadAction = false);
	u32 DivTag   (u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet, bool isBeforeLoadAction = false);
	u32 ModTag   (u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet, bool isBeforeLoadAction = false);
	u32 AndTag   (u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet, bool isBeforeLoadAction = false);
	u32 OrTag    (u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet, bool isBeforeLoadAction = false);
	u32 XorTag   (u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet, bool isBeforeLoadAction = false);
	u32 NotTag   (u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagClassListTag, u16 SourceTagClassListTag, bool isBeforeLoadAction = false);
	u32 SLTag    (u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet, bool isBeforeLoadAction = false);
	u32 SRTag    (u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet, bool isBeforeLoadAction = false);
	u32 SetTagImm(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagClassListTag, u32 u32_ImmValue, bool isBeforeLoadAction = false);
	u32 IncTagImm(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagClassListTag, u32 u32_ImmValue, bool isBeforeLoadAction = false);
	u32 DecTagImm(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagClassListTag, u32 u32_ImmValue, bool isBeforeLoadAction = false);
	u32 MulTagImm(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagClassListTag, u32 u32_ImmValue, bool isBeforeLoadAction = false);
	u32 DivTagImm(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagClassListTag, u32 u32_ImmValue, bool isBeforeLoadAction = false);
	u32 ModTagImm(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagClassListTag, u32 u32_ImmValue, bool isBeforeLoadAction = false);
	u32 AndTagImm(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagClassListTag, u32 u32_ImmValue, bool isBeforeLoadAction = false);
	u32 OrTagImm (u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagClassListTag, u32 u32_ImmValue, bool isBeforeLoadAction = false);
	u32 XorTagImm(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagClassListTag, u32 u32_ImmValue, bool isBeforeLoadAction = false);
	u32 SLTagImm (u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagClassListTag, u32 u32_ImmValue, bool isBeforeLoadAction = false);
	u32 SRTagImm (u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagClassListTag, u32 u32_ImmValue, bool isBeforeLoadAction = false);
//branch code
	u32 BranchEQU    (u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagClassListTag, u16 SourceTagClassListTag);
	u32 BranchEQUImm (u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagClassListTag, u32 u32_ImmValue);
	u32 BranchNEQ    (u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagClassListTag, u16 SourceTagClassListTag);
	u32 BranchNEQImm (u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagClassListTag, u32 u32_ImmValue);
	u32 BranchMore   (u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagClassListTag, u16 SourceTagClassListTag);
	u32 BranchMoreImm(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagClassListTag, u32 u32_ImmValue);
	u32 BranchLess   (u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagClassListTag, u16 SourceTagClassListTag);
    u32 BranchLessImm(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagClassListTag, u32 u32_ImmValue);
	void Jump         (u8 u8_CdRun , u8 u8_CdRCd , u16 u32_ImmValue);

	u32 AppendArray  (u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet);
	u32 DelArrayByIdx(u8 u8_CdRun , u8 u8_CdRCd , u16 tagId, u32 idx);
	u32 GetArrayElem  (u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet, bool isBeforeLoadAction = false);

//MODBUS code
	void RW_DATA(u8 u8_CdRun , u8 u8_CdRCd, u16 u16_DestTagClassListTag, u32 u32_ImmValue);
//timer code
	void set_timer(u8 u8_CdRun , u8 u8_CdRCd , u16 source, u32 u32_ImmValue);
// animation code
	u32 triggerAnimation(u8 u8_CdRun , u8 u8_CdRCd , u16 tagID, u8 u8_animationID);
	u32 triggerAnimationTag(u8 u8_CdRun , u8 u8_CdRCd , u16 tagID, u16 u8_animationTagID);

	u32 setTagString			(u8 u8_CdRun , u8 u8_CdRCd , u16 tagID, u16 stringID, bool isBeforeLoadAction = false);
	void setTagArr			(u8 u8_CdRun , u8 u8_CdRCd , u16 tagID, u16 arrID);

    u32 SetColorExe             (u16 tagID,u32 color);
	u32 SetColor				(u8 u8_CdRun , u8 u8_CdRCd , u16 tagId, u32 color);

	u32 SetKBExe				(u16 tagId, u32 keyCode);
	u32 SetKB					(u8 u8_CdRun , u8 u8_CdRCd , u16 tagId, u32 keyCode);

	u32 SetKBHintExe			(u16 tagId, u32 idx);
	u32 SetKBHint				(u8 u8_CdRun , u8 u8_CdRCd , u16 tagId, u32 idx);

	u32 ChangeKBHintsPageExe	(u16 tagId, u32 step, u8 direction);
	u32 ChangeKBHintsPage		(u8 u8_CdRun , u8 u8_CdRCd , u16 tagId, u32 step, u8 direction = 0);
    
	u32 GetKBExe				(u16 kbTagId, u16 targetTagId);
	u32 GetKB					(u8 u8_CdRun , u8 u8_CdRCd , u16 kbTagId, u16 targetTagId);

    u32 SetMoveExe              (uint32_t mode, u16 DestTagID, u16 offsetX,u16 offsetY);
	u32 SetMove					(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, uint32_t mode, u16 offsetX, u16 offsetY);
	u32 SetMove					(u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet, bool isBeforeLoadAction = false);

    u32 SetZoomExe              (uint32_t mode, u16 DestTagID, u16 zoomX, u16 zoomY);
	u32 SetZoom					(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, uint32_t mode, u16 zoomX, u16 zoomY);
	u32 SetZoom					(u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet, bool isBeforeLoadAction = false);

	u32 SetScaleCenterExe    (uint32_t mode, u16 DestTagID, u16 centerX, u16 centerY);
	u32 SetScaleCenter		(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, uint32_t mode, u16 centerX, u16 centerY);
	u32 SetScaleCenter		(u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet, bool isBeforeLoadAction = false);

    u32 SetRotationCenterExe    (uint32_t mode, u16 DestTagID, u16 centerX, u16 centerY);
	u32 SetRotationCenter		(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, uint32_t mode, u16 centerX, u16 centerY);
	u32 SetRotationCenter		(u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet, bool isBeforeLoadAction = false);

    u32 SetRotationAngleExe     (u16 DestTagID, u16 angle);
	u32 SetRotationAngle		(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 angle);
    
    u32 SetProjectionExe        (uint32_t mode, u16 DestTagID, u16 projectionX,u16 projectionY);
	u32 SetProjection			(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, uint32_t mode, u16 projectionX, u16 projectionY);
	u32 SetProjection			(u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet, bool isBeforeLoadAction = false);

	u32 SetMoveTag				(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 offsetTagID, bool isBeforeLoadAction = false);
	u32 SetZoomTag				(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 zoomTagID, bool isBeforeLoadAction = false);
	u32 SetScaleCenterTag	(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 centerTagID, bool isBeforeLoadAction = false);
	u32 SetRotationCenterTag	(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 centerTagID, bool isBeforeLoadAction = false);
	u32 SetRotationAngleTag		(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 angleTagID, bool isBeforeLoadAction = false);
	u32 SetProjectionTag		(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 projectionTagID, bool isBeforeLoadAction = false);
    
    u32 ChangeMoveExe           (u16 DestTagID, u16 offsetX,u16 offsetY);
	u32 ChangeMove				(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 offsetX,	 u16 offsetY);
	u32 ChangeMove				(u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet, bool isBeforeLoadAction = false);

	u32 ChangeZoomExe			(u16 DestTagID, u16 zoomX,				u16 zoomY);
	u32 ChangeZoom				(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 zoomX, u16 zoomY);
	u32 ChangeZoom				(u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet, bool isBeforeLoadAction = false);

	u32 ChangeScaleCenterExe	(u16 DestTagID, u16 centerX,			u16 centerY);
	u32 ChangeScaleCenter	(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 centerX, u16 centerY);
	u32 ChangeScaleCenter	(u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet, bool isBeforeLoadAction = false);

    u32 ChangeRotationCenterExe	(u16 DestTagID, u16 centerX,			u16 centerY);
	u32 ChangeRotationCenter	(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 centerX, u16 centerY);
	u32 ChangeRotationCenter	(u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet, bool isBeforeLoadAction = false);

    u32 ChangeRotationAngleExe	(u16 DestTagID, u16 angle);
	u32 ChangeRotationAngle		(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 angle);
    
	u32 ChangeProjectionExe		(u16 DestTagID, u16 projectionX,		u16 projectionY);
    u32 ChangeProjection		(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 projectionX,	 u16 projectionY);
	u32 ChangeProjection		(u8 u8_CdRun , u8 u8_CdRCd , u8 *pPC, u8 *pOffSet, bool isBeforeLoadAction = false);

	u32 ChangeMoveTag			(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 offsetTagID, bool isBeforeLoadAction = false);
	u32 ChangeZoomTag			(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 zoomTagID, bool isBeforeLoadAction = false);
	u32 ChangeScaleCenterTag	(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 centerTagID, bool isBeforeLoadAction = false);
	u32 ChangeRotationCenterTag	(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 centerTagID, bool isBeforeLoadAction = false);
	u32 ChangeRotationAngleTag	(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 angleTagID, bool isBeforeLoadAction = false);
	u32 ChangeProjectionTag		(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 projectionTagID, bool isBeforeLoadAction = false);
	
	u32 SetMoveX					(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 offsetX, bool isBeforeLoadAction = false);
	u32 SetMoveY					(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 offsetY, bool isBeforeLoadAction = false);
	u32 SetZoomX					(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 zoomX, bool isBeforeLoadAction = false);
	u32 SetZoomY					(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 zoomY, bool isBeforeLoadAction = false);
	u32 SetRotationCenterX			(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 centerX, bool isBeforeLoadAction = false);
	u32 SetRotationCenterY			(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 centerY, bool isBeforeLoadAction = false);
	u32 SetScaleCenterX			(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 centerX, bool isBeforeLoadAction = false);
	u32 SetScaleCenterY			(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 centerY, bool isBeforeLoadAction = false);
	u32 SetProjectionX				(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 projectionX, bool isBeforeLoadAction = false);
	u32 SetProjectionY				(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 projectionY, bool isBeforeLoadAction = false);
	u32 SetMoveXTag				(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 offsetXTagID, bool isBeforeLoadAction = false);
	u32 SetMoveYTag				(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 offsetYTagID, bool isBeforeLoadAction = false);
	u32 SetZoomXTag				(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 zoomXTagID, bool isBeforeLoadAction = false);
	u32 SetZoomYTag				(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 zoomYTagID, bool isBeforeLoadAction = false);
	u32 SetScaleCenterXTag  	(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 centerXTagID, bool isBeforeLoadAction = false);
	u32 SetScaleCenterYTag  	(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 centerYTagID, bool isBeforeLoadAction = false);
	u32 SetRotationCenterXTag  	(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 centerXTagID, bool isBeforeLoadAction = false);
	u32 SetRotationCenterYTag  	(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 centerYTagID, bool isBeforeLoadAction = false);
	u32 SetProjectionXTag			(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 projectionXTagID, bool isBeforeLoadAction = false);
	u32 SetProjectionYTag			(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 projectionYTagID, bool isBeforeLoadAction = false);
	u32 ChangeMoveX				(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 offsetX, bool isBeforeLoadAction = false);
	u32 ChangeMoveY				(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 offsetY, bool isBeforeLoadAction = false);
	u32 ChangeZoomX				(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 zoomX, bool isBeforeLoadAction = false);
	u32 ChangeZoomY				(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 zoomY, bool isBeforeLoadAction = false);
	u32 ChangeScaleCenterX		(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 centerX, bool isBeforeLoadAction = false);
	u32 ChangeScaleCenterY		(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 centerY, bool isBeforeLoadAction = false);
	u32 ChangeRotationCenterX		(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 centerX, bool isBeforeLoadAction = false);
	u32 ChangeRotationCenterY		(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 centerY, bool isBeforeLoadAction = false);
	u32 ChangeProjectionX			(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 projectionX, bool isBeforeLoadAction = false);
	u32 ChangeProjectionY			(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 projectionY, bool isBeforeLoadAction = false);
	u32 ChangeMoveXTag				(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 offsetXTagID, bool isBeforeLoadAction = false);
	u32 ChangeMoveYTag				(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 offsetYTagID, bool isBeforeLoadAction = false);
	u32 ChangeZoomXTag				(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 zoomXTagID, bool isBeforeLoadAction = false);
	u32 ChangeZoomYTag				(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 zoomYTagID, bool isBeforeLoadAction = false);
	u32 ChangeScaleCenterXTag	(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 centerXTagID, bool isBeforeLoadAction = false);
	u32 ChangeScaleCenterYTag	(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 centerYTagID, bool isBeforeLoadAction = false);
	u32 ChangeRotationCenterXTag	(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 centerXTagID, bool isBeforeLoadAction = false);
	u32 ChangeRotationCenterYTag	(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 centerYTagID, bool isBeforeLoadAction = false);
	u32 ChangeProjectionXTag		(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 projectionXTagID, bool isBeforeLoadAction = false);
	u32 ChangeProjectionYTag		(u8 u8_CdRun , u8 u8_CdRCd , u16 DestTagID, u16 projectionYTagID, bool isBeforeLoadAction = false);

	
	//void SetColorImm(u8 u8_CdRun , u8 u8_CdRCd , u8 DestTagClassListTag, u32 u32_ImmValue);

//keyboard function
#ifdef KEYBOARD_DEBUG
	static void keyboardLeft();
	static void keyboardRight();
	static void keyboardEntered();
#endif

	u32 clearData(u8 u8_CdRun , u8 u8_CdRCd , u16 tagID);

	u32 playSound(u32 index);

	void pauseSound(u32 index);

	void resumeSound(u32 index);

	void stopSound(u32 index);

	u32 writeLog(u8 u8_CdRun , u8 u8_CdRCd ,u16 tag1, u16 tag2);

	u32 writeReg(u8 u8_CdRun , u8 u8_CdRCd ,u16 tag, u32 option);

	u32 writeFlash(u8 u8_CdRun , u8 u8_CdRCd ,u16 tag, u32 u32_ImmValue);

	u32 readErrMessage(u8 u8_CdRun , u8 u8_CdRCd ,u16 tag);

	void copyBackground_1(u32 u32_ImmValue);

	void copyBackground_2(u32 u32_ImmValue);

	void pasteBackground(u16 u16_WidgetTagID, u32 u32_ImmValue);

	u32 Snapshot(u32 index);
	u32 BlurSnapshot(u32 index);
	u32 ClearSnapshot(u32 index);

	u32 ActionImplement(bool isBeforeLoadAction = false);//actionSpace address
	int ActionDecoder(bool isBeforeLoadAction = false);

};

	void tickDelayOP(void* _arg);

	void tickGifOP(void* _arg);

//-----------------------------
// name�� ActionProcessorTask
// handle the instruction from widget 
//  
// @param1 void* pvParameters The instruction parameter list
// addtional comments :
// none
//-----------------------------
	void ActionProcessorTask(void* pvParameters);
	
	

	void SuspendOtherTask(void);
	void ResumeOtherTask(void);

//end code
	void End(void);//remained...




#endif //end of iddef AHMI_CORE
#endif //

