#include "Easing.h"
#include "em_ct.h"

Easing::Easing(){

}

Easing::~Easing(){

}



//easeInCubic: function (t) { return t*t*t },
   
//easeOutCubic: function (t) { return (--t)*t*t+1 },
   
//easeInOutCubic: function (t) { return t<.5 ? 4*t*t*t : (t-1)*(2*t-2)*(2*t-2)+1 },
u32 Easing::calculateEasingFunc(u16 in, u16 &out, u8 type){
	switch (type)
	{
	case EASINGTYPE_Linear:
			out = in;
			return AHMI_FUNC_SUCCESS;
//			break;
		case EASINGTYPE_EasingInCubic:
			out = ((long long)in * in * in) >> 20;
			return AHMI_FUNC_SUCCESS;
//			break;
		case EASINGTYPE_EasingOutCubic:
			out = 1024 - (((long long)(1024 - in) * (1024 - in) * (1024 - in)) >> 20) ;
			return AHMI_FUNC_SUCCESS;
//			break;
		case EASINGTYPE_EasingInOutCubic:
			if(in < 512)
			{
				out = ((long long)in * in * in) >> 18;
				
			}
			else if(in <= 1024)
			{

				out = 1024 - (((long long)(1024 - in) * (1024 - in) * (1024 - in)) >> 18) ;
				
			}
			return AHMI_FUNC_SUCCESS;
//			break;
		default:
			break;
	}

	out = 0;
	return (EM_enErrRetModularEasing | EM_ErrRetEasing_EasingTypeErr);
}
