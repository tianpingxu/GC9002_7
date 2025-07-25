//created by Zzen1sS
//2018/12/6
#include "WaveFilter.h"
#include "em_ct.h"

int WaveFilter::filter(WaveFilterStatus* status){
	int k;
	int k1; 
	int k2;
	int threshold;
	int step;
	int delay;
	switch (this->type)
	{
	case WaveFilter_Damping_Linear:
		step = this->args[0];
		delay = this->args[1];
		if (status->target > status->last)
		{
			status->delayCount++;
			if (status->delayCount>delay)
			{
				status->delayCount = 0;
				status->last += step;
				if (status->last > status->target)
				{
					status->last = status->target;
				}
			}
		}else if (status->target < status->last)
		{
			status->delayCount++;
			if (status->delayCount>delay)
			{
				status->delayCount = 0;
				status->last -= step;
				if (status->last < status->target)
				{
					status->last = status->target;
				}
			}
		}
		break;
	case WaveFilter_Damping_PT1:
		k = this->args[0];
		threshold = this->args[1];
		step = this->args[2];
		delay = this->args[3];
		if (status->target >= status->last && status->target <= status->last + threshold)
		{
			status->delayCount++;
			if (status->delayCount>delay)
			{
				status->delayCount = 0;
				status->last += step;
				if (status->last > status->target)
				{
					status->last = status->target;
				}
			}
		}else if (status->target <= status->last && status->last <= status->target + threshold)
		{
			status->delayCount++;
			if (status->delayCount>delay)
			{
				status->delayCount = 0;
				status->last -= step;
				if (status->last < status->target)
				{
					status->last = status->target;
				}
			}
		}else{
			 if(k >= 1){
               status->last = (status->target + (k - 1) * (status->last)) / k;
            }
		}
		break;
	case WaveFilter_Damping_PT2:
		k1 = this->args[0];
		k2 = this->args[1];
		threshold = this->args[2];
		step = this->args[3];
		delay = this->args[4];
		if (status->target >= status->last && status->target <= status->last + threshold)
		{
			status->delayCount++;
			if (status->delayCount>delay)
			{
				status->delayCount = 0;
				status->last += step;
				if (status->last > status->target)
				{
					status->last = status->target;
				}
			}
		}else if (status->target <= status->last && status->last <= status->target + threshold)
		{
			status->delayCount++;
			if (status->delayCount>delay)
			{
				status->delayCount = 0;
				status->last -= step;
				if (status->last < status->target)
				{
					status->last = status->target;
				}
			}
		}else{
			    
                if(k1 != 0 && k2 != 0){
                    status->temp = (status->target + (k1 - 1) * (status->temp)) / k1;
                    status->last = (status->temp + (k2 - 1) * (status->last)) / k2;
                }
		}
		break;
	default:
		return (EM_enErrRetModularWaveFilter | EM_ErrRetWaveFilter_FilterTypeErr);
	}
    
	return AHMI_FUNC_SUCCESS;

}

int WaveFilter::getDuration(){
	switch (type)
	{
		case WaveFilter_Damping_Linear:
			return args[3];
		case WaveFilter_Damping_PT1:
			return args[5];
		case WaveFilter_Damping_PT2:
			return args[6];
		default:
			return (EM_enErrRetModularWaveFilter | EM_ErrRetWaveFilter_FilterTypeErr);
	}
}

int WaveFilter::getFrequence(){
	switch (type)
	{
		case WaveFilter_Damping_Linear:
			return args[2];
		case WaveFilter_Damping_PT1:
			return args[4];
		case WaveFilter_Damping_PT2:
			return args[5];
		default:
			return (EM_enErrRetModularWaveFilter | EM_ErrRetWaveFilter_FilterTypeErr);
	}
}

int WaveFilter::getStep(){
	switch (type)
	{
		case WaveFilter_Damping_Linear:
			return args[0];
		case WaveFilter_Damping_PT1:
			return args[2];
		case WaveFilter_Damping_PT2:
			return args[3];
		default:
			return (EM_enErrRetModularWaveFilter | EM_ErrRetWaveFilter_FilterTypeErr);
	}
}
