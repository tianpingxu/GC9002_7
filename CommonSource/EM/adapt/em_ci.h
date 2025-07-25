/**
  ******************************************************************************
  * @file    em_ci.h
  * @author  Zhu Jiahai
  * @version V1.0.0
  * @date    12-07-2021
  * @brief   The internal configuration head file of Error Management module.
  *          
  ******************************************************************************
	*/
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EM_CI_H
#define __EM_CI_H

#ifdef __cplusplus
 extern "C" {
#endif
	
/* Includes ------------------------------------------------------------------*/

/* All errors in different modules need to be configured here , combined by enum and string */
#define EM_ALL_ERROR_INFO_TABLE \
    /* Err Type Enum,							Error Information */\
	CFG(EM_enErrWidgetTagValueOverflow,			"ERROR: widget bindTag value exceeds the boundary")\
    CFG(EM_enErrNULLPointer,					"ERROR: for NULL pointer")\
    CFG(EM_enErrWidgetExceedBoundary,			"ERROR: widget offset\\width\\height exceeds the boundary")\
    CFG(EM_enErrWidgetStartTexExceedBoundary,	"ERROR: start texture number more than total number of texture")\
    CFG(EM_enErrWidgetActionTriggerIllegal,		"ERROR: illegal action trigger button widget")\
    CFG(EM_enErrDividedByZero,					"ERROR: divided by zero")\
    CFG(EM_enErrNumberLengthError,				"ERROR: don't support such length of number")\
    CFG(EM_enErrAmimationDurationExceedBoundary,"ERROR: flash duration exceeds the limit")\
    CFG(EM_enErrpPCExceedBoundary,				"ERROR: no ending instructions detected, the pPC is out of baundary")\
    CFG(EM_enErrTagIDExceedBoundary,			"ERROR in function SetTag: tagID out of baundary")\
    CFG(EM_enErrTagTypeInconsistent,			"ERROR in function SetTag: tagType inconsistent")\
    CFG(EM_enErrPageIDExceedBoundary,			"ERROR in gotoPage Function: page out of baundary")\
	CFG(EM_enErrCanvasIDExceedBoundary,			"ERROR in drawPage: canvas tag num out of baundary")\
	CFG(EM_enErrSubcanvasIDExceedBoundary,		"ERROR in CanvasActionTrigger: cannot goto current subcanvas")\
    CFG(EM_enErrDelayFunction,					"ERROR in delay Function: no idle status")\
    CFG(EM_enErrPageAnimation,					"ERROR when creating page animation: page pointer null or out of boundary")\
    CFG(EM_enErrCanvasAnimation,				"ERROR when creating canvas animation: canvas pointer null or out of boundary")\
    CFG(EM_enErrSubcanvasAnimation,				"ERROR when creating subcanvas animation: subcanvas pointer null or out of boundary")\
    CFG(EM_enErrWidgetAnimation,				"ERROR when creating widget animation: widget pointer null or out of boundary")\
	CFG(EM_enErrPageAnimationTag,				"ERROR in setting page animaion tag: value out of baundary")\
	CFG(EM_enErrCanvasAnimationTag,				"ERROR in setting canvas animation tag, value out of baundary")\
	CFG(EM_enErrSubcanvasAnimationTag,			"ERROR when setting subcanvas animation tag, value out of baundary")\
	CFG(EM_enErrSoundIDInexistent,				"ERROR: cannot find soundID")\

/* configured end */

#define EM_ERROR_COUNT_MAX                  (65535)

#define EM_ERROR_MESSAGE_ONEPAGE_NUMBER     (10)
#define EM_ERROR_MESSAGE_COUNT_MAX          (100)
#define EM_ERROR_MESSAGE_PAGE_COUNT_MAX     (EM_ERROR_MESSAGE_COUNT_MAX / EM_ERROR_MESSAGE_ONEPAGE_NUMBER)

#ifdef __cplusplus
}
#endif

#endif /* __EM_CI_H */




