//created by Zzen1sS
//2018/11/19
//Class EASING for calculate easing functions

#ifndef EASING__H
#define EASING__H
#include "publicDefine.h"
#include "publicType.h"

#define EASINGTYPE_Linear 0
#define EASINGTYPE_EasingInCubic 1
#define EASINGTYPE_EasingOutCubic 2
#define EASINGTYPE_EasingInOutCubic 3

typedef enum
{
    EM_ErrRetEasing_EasingTypeErr = 1,
   
}EM_ErrRetEasing;

class Easing{
	

public:
	Easing();
	~Easing();
	u32 calculateEasingFunc(
		u16 in,   //0.10
		u16 &out,  //0.10)
		u8 type=EASINGTYPE_EasingInOutCubic
	);


};


#endif
