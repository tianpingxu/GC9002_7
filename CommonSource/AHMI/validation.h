//Created by Zzen1sS
//Created at 2020/7/15

#ifndef VALIDATION__H
#define VALIDATION__H
#include "publicType.h"
#include "publicDefine.h"

class Validator
{
public:
	Validator();
	~Validator();

	 u16 calcCRC16(u8 *data, u32 datalen);

private:

};

u32 XORCheck(u32 *data, u32 datalen);


#endif
