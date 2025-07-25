////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:   2016/03/21
// File Name:     DynamicPage.h
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2016/03/21 by Zhou Yuzhi 
// Additional Comments:
//    definition included ahead to avoid multi-include
// 
////////////////////////////////////////////////////////////////////////////////

#ifndef DYNAMICPAGECLASS_C1__H
#define DYNAMICPAGECLASS_C1__H
#include "publicDefine.h"
class WidgetClass;
class CanvasClass;
class SubCanvasClass;
class TextureClass;
class TagClass;
class TileBoxClass;
class ActionTriggerClass;
class AnimationActionClass;


typedef WidgetClass* WidgetClassPtr;
typedef CanvasClass* CanvasClassPtr;
typedef SubCanvasClass* SubCanvasClassPtr;
typedef TextureClass* TextureClassPtr;
typedef u16 touchWidgetRange;
typedef TagClass* TagClassPtr;
typedef TileBoxClass* TileBoxClassPtr;
typedef ActionTriggerClass* ActionTriggerClassPtr;
typedef AnimationActionClass* AnimationActionClassPtr;
typedef struct AnimationKeyFrame* AnimationKeyFramePtr;

#endif

