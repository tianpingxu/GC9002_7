////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:   2015/11/17
// File Name:     ahmiv3.h
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2016/03/22 by Arthas 
// Additional Comments:
//    the definition to draw animation
// 
////////////////////////////////////////////////////////////////////////////////
#ifndef ANIMATIONDEFINE__H
#define ANIMATIONDEFINE__H
#include "publicDefine.h"
#ifdef AHMI_CORE

#define NO_ANIMATION                       (0)      //none animation
//page animation
#define PAGE_ANIMATION_SHIFTING_L          (1)      //shift from left
#define PAGE_ANIMATION_SHIFTING_R          (2)      //shift from right
#define PAGE_ANIMATION_SHIFTING_UP         (3)      //shift from up
#define PAGE_ANIMATION_SHIFTING_DOWN       (4)      //shift from down
#define PAGE_ANIMATION_ZOOM                (5)      //zoom from center
#define PAGE_ANIMATION_SWIPE_H             (6)      //swipe horizontal
#define PAGE_ANIMATION_SWIPE_V             (7)      //swipe vertical
#define PAGE_ANIMATION_MULTI_CARD          (8)      //multi card

//multi card page animation status
#define PAGE_ANIMATION_MULTI_CARD_Idle          (0)
#define PAGE_ANIMATION_MULTI_CARD_PullUp        (1)
#define PAGE_ANIMATION_MULTI_CARD_Swipe         (2)
#define PAGE_ANIMATION_MULTI_CARD_Focus         (3)
#define PAGE_ANIMATION_MULTI_CARD_Auto          (4)
#define PAGE_ANIMATION_MULTI_CARD_Init          (0xFF)

//subcanvas animation
#define SUBCANVAS_ANIMATION_SHIFTING_L          (1)      //shift from left
#define SUBCANVAS_ANIMATION_SHIFTING_R          (2)      //shift from right
#define SUBCANVAS_ANIMATION_SHIFTING_UP         (3)      //shift from up
#define SUBCANVAS_ANIMATION_SHIFTING_DOWN       (4)      //shift from down
#define SUBCANVAS_ANIMATION_ZOOM                (5)      //zoom from center
#define SUBCANVAS_ANIMATION_SWIPE_H             (6)      //swipe horizontal
#define SUBCANVAS_ANIMATION_SWIPE_V             (7)      //swipe vertical
#define SUBCANVAS_ANIMATION_PULL_DOWN           (8)      //pull from top edge to bottom
#define SUBCANVAS_ANIMATION_PULL_UP             (9)      //pull from bottom edge to top
#define SUBCANVAS_ANIMATION_PULL_RIGHT          (10)     //pull from left edge to right
#define SUBCANVAS_ANIMATION_PULL_LEFT           (11)     //pull from right edge to left
 
#define ANIMATION_HERIZONTAL_SHIFTING_L    (1)      //herizontal left animation 
#define ANIMATION_HERIZONTAL_SHIFTING_R    (2)      //herizontal right animation 
#define ANIMATION_VERTICAL_SHIFTING_UP     (3)      //vertical up animation 
#define ANIMATION_VERTICAL_SHIFTING_DOWN   (4)      //vertical down animation
#define ANIMATION_ZOOM_IN                  (5)      //zoom small
#define ANIMATION_ZOOM_OUT                 (6)      //zoom big

#define ANIMATION_REFRESH_NULL			0     //none element needs refresh
#define ANIMATION_REFRESH_PAGE			1     //page needs refresh
#define ANIMATION_MATRIXREFRESH_PAGE	7     //page position needs refresh
#define ANIMATION_REFRESH_SUBCANVAS		2     //subcanvas needs refresh
#define ANIMATION_REFRESH_CANVAS		3     //user canvas animation
#define ANIMATION_REFRESH_WIDGET		4     //widget animation
#define ANIMATION_REFRESH_DOUBLE_BUFFER 5	  //render to screen
#define ANIMAITON_REFRESH_STATIC_BUFFER 6	  //render to static buffer
#define ANIMAITON_REFRESH_LAST_FRAME 	8	  //special type for the lase refresh

#define AnimationMsg_Maxnum   32  //the maxium size of animation list
#define NULL_Animation        4  //element type of null animation
#define Animation_Frame       5 //the frame count for animation
#ifdef PC_SIM
#define Animation_Time        3 //time for every frame 
#endif
#ifdef EMBEDDED
#define Animation_Time        3 //time for every frame 
#endif

#define PAGE_NEED_REFRESH    1  //refresh page or canvas and display animation
#define PAGE_NOT_REFRESH     0  //only refresh the element itself

#define ZOOM_SCALE       3     //ratio of zoom 

//action
#define ANIMATION_DIRECTION_ADD 0 //indicating that the animation is from start to stop
#define ANIMATION_DIRECTION_MINUS 1 //indicating that the animation is from stop to start


#endif

#endif
