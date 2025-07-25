#include "sxGL.h"
#include "sxGLObject.h"
#include "sxMatrix.h"

#define SXGL_FLASH_OFFSET 0x30000
#define SXGL_DDR_OFFSET   0x60000000


#ifndef SXGL_PRO__H
#define SXGL_PRO__H

typedef enum {
	IDLE = 0, //aotu rotating
	CAMERA = 1, //rotating camera
	BACK = 2 //rotating back
} sxgl_rotate_state;

#ifdef __cplusplus
 extern "C" {
#endif

//each time rotate the cube in 15 degree
//then generate the sourcebuffer
//*sourceBufferAddr is the address of sourcebuffer output
//*sourceShift is the input of current sourcebuffer shift
int generate3dSourcebuffer(
	unsigned int *sourceBufferAddr, 
	unsigned int* sourceShift,
	//rotate
	unsigned char refresh_en,
	unsigned char touch_en,
	unsigned char press_en,
	unsigned short x,
	unsigned short y
	);

//set the address and width/height of input texture
void setTexAttr(
	unsigned int texIndex, //0-inside cube, 1-outside cube
	unsigned int type    , //@ref sourcebuffer texture type
	unsigned int width   , // original texture width
	unsigned int height  , // original texture height
	unsigned int address , //need to set as sourcebuffer's address
	unsigned int source  , //0-flash, 1-ddr
	unsigned int sbWidth , //extended texture width
	unsigned int sbHeight  //extended texture height
	);

//internal function

//calculate rotate angle according to current state
void rotateCntl(
	//rotate
	unsigned char touch_en,
	unsigned char press_en, //press the screen
	unsigned short x,
	unsigned short y
	);

void rotateCamera(
	GLTypeT rotateAngleX,
	GLTypeT rotateAngleY
	);

#ifdef __cplusplus
 }
#endif

#endif