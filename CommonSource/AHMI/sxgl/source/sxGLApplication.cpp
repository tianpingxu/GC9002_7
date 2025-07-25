// sxGLPro.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <stdio.h>
#include "sxGL.h"
#include "sxGLObject.h"
#include "sxGLObjIn.h"
#include "sxGLApplication.h"
#include "sxGLBuffer.h"
#include "publicDefine.h"
#ifdef PC_SIM
#include "trace.h"
#endif

#ifdef SXGL_LIB

GLubyte rotateFlag = 0;
GLTypeT rotateAngle; //rotate angle of outside cube
GLushort rotateAngleShort;
GLTypeT cubeRotateAngle;
GLushort cubeRotateAngleShort;
GLTypeT gCameraRotateAngleX;
GLTypeT gCameraRotateAngleY;
GLTypeT gCameraPreRotateAngleX;
GLTypeT gCameraPreRotateAngleY;
GLuint gTexAddress           [3];
GLuint gTexWidth             [3];
GLuint gTexHeight            [3];
GLuint gSourcebufferWidth    [3];
GLuint gSourcebufferHeight   [3];
GLuint gTexType              [3]; 
GLuint readObjeFlag = 0;
unsigned short preTouchX;
unsigned short preTouchY;
GLubyte movingBackCnt;
GLubyte untouchCnt;
sxgl_rotate_state state = IDLE;
GLTypeT backRotateAngleX;
GLTypeT backRotateAngleY;

#define MOVING_THRESHOLD 3
#define TOTAL_BACK_CNT 30 //step of animation from pos to 0
#define UNTOUCH_THRESHOLD 30
#define MOVING_ROTATE_ANGLE 1
#define IDLE_ROTATE_ANGLE 1


int generate3dSourcebuffer(
	unsigned int *sourceBufferAddr, 
	unsigned int* sourceShift,
	//rotate
	unsigned char refresh_en,
	unsigned char touch_en,
	unsigned char press_en,
	unsigned short x,
	unsigned short y
	)
{
	GLuint i;
    GLuint textureName = 0;
	GLint color[4];
	GLTypeT left; 
	GLTypeT right;
	GLTypeT buttom; 
	GLTypeT top;
	GLTypeT zNear; 
	GLTypeT zFar;
	GLTypeT rotateX;
	GLTypeT rotateY;
	GLTypeT rotateZ;
	
	GLTypeT rotateAngleY;
	GLTypeT scaleX;
	GLTypeT scaleY;
	GLTypeT scaleZ;
	GLTypeT lightSpot[4];
	GLTypeT transX, transY, transZ;
	char filename[100] = {"simple.obj"};
    //printf("Hello World!\n");
    //pGLContext = (GLContextStruct*)malloc(sizeof(GLContextStruct));
    
	//setVideoWidthAndHeight(720, 576);
	GLContextInit();
	if(readObjeFlag == 0)
	{
		
		pGLContext->sourcebufferStruct.pSourceBufferData = (GLint*)glGenBuffer(MAX_BYTE_OF_3D_SOURCEBUFFER);
	    pGLVSO->vertexPointer = (GLVertexPosStruct*)glGenBuffer(MAX_NUM_OF_SHADED_VERTEX);
		readObjeFlag = 1;
		cubeRotateAngle = 0.0;
	}
	readObj(filename);

	rotateCntl(
		//rotate
			touch_en, press_en, x, y
		);

	if(!refresh_en)
	{
		return 0;
	}


	
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
	glEnable(GL_CLIP_PLANE_FAR);
	glEnable(GL_CLIP_PLANE_LRTB);
    glVertexPointer(4, GL_FIXED, 0, (GLvoid*)gsxGLObjIn.vertexPointer);
	glNormPointer(4, GL_FIXED, 0, (GLvoid*)gsxGLObjIn.normalPointer);
	glTOPointer(gsxGLObjIn.TOPointer);
	glTMPointer(gsxGLObjIn.TMPointer);
	glViewport(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);

	//set light
	lightSpot[0] = 0;
	lightSpot[1] = -1;
	lightSpot[2] = 0;
	glLightiv(GL_LIGHT0, GL_SPOT_DIRECTION, lightSpot);
	

	glGenTextures(1, &textureName);
    glBindTexture(GL_TEXTURE_2D, textureName);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 800, 480, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	color[0] = 255;
	color[1] = 255;
	color[2] = 255;
	color[3] = 255;
	glTexEnviv(GL_TEXTURE_2D, GL_TEXTURE_ENV_COLOR, color);
	
	
	

	//draw outside cube
	glMatrixMode(GL_MODELVIEW_MATRIX);
	glLoadIdentity();

	rotateX = GL_TYPET_ONE;
	rotateY = 0;
	rotateZ = 0;
	rotateAngleY = -20.0;
	//glRotatex(rotateAngleY, rotateX, rotateY, rotateZ);

	rotateX = 0;
	rotateY = GL_TYPET_ONE;
	rotateZ = 0;
	rotateAngleY = -20.0;
	glRotatex(rotateAngle, rotateX, rotateY, rotateZ);

	rotateCamera(gCameraRotateAngleX, gCameraRotateAngleY);
	
	scaleX = 8;
	scaleY = 8;
	scaleZ = 8;
	glScalex(scaleX, scaleY, scaleZ);
	transX = 0;
	transY = 1.8;
	transZ = 13.0;
	glTranslatex(transX,transY,transZ);

	

	left = -1.2 * SCREEN_WIDTH / SCREEN_HEIGHT ;
	right = 1.2 * SCREEN_WIDTH / SCREEN_HEIGHT;
	buttom = -1.2;
	top = 1.2;
	zNear = 3.0;
	zFar = 100.0;
	glFrustumx(left, right, buttom, top, zNear, zFar); 
	

	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glDrawArrays(GL_QUADS, 0, 6 * 4);


	//draw inside cube
	glMatrixMode(GL_MODELVIEW_MATRIX);
	glDisable(GL_LIGHTING);
	glLoadIdentity();
	//glDisable(GL_TEXTURE_2D);

	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	transX = -0.5;
	transY = -0.5;
	transZ = -0.5;
	glTranslatex(transX,transY,transZ);

	rotateX = 0;
	rotateY = 0;
	rotateZ = GL_TYPET_ONE;
	rotateAngleY = 45.0;
	glRotatex(rotateAngleY, rotateX, rotateY, rotateZ);

	rotateX = GL_TYPET_ONE;
	rotateY = 0;
	rotateZ = 0;
	rotateAngleY = 35;
	glRotatex(rotateAngleY, rotateX, rotateY, rotateZ);

	rotateX = 0;
	rotateY = GL_TYPET_ONE;
	rotateZ = 0;
	glRotatex(cubeRotateAngle, rotateX, rotateY, rotateZ);

	rotateCamera(gCameraRotateAngleX, gCameraRotateAngleY);
	
	
	
	transX = 1.0;
	transY = 0.0;
	transZ = 6.0;
	glTranslatex(transX,transY,transZ);

	

	left = -1.2 * SCREEN_WIDTH / SCREEN_HEIGHT;
	right = 1.2 * SCREEN_WIDTH / SCREEN_HEIGHT;
	buttom = -1.2;
	top = 1.2;
	zNear = 3.0;
	zFar = 100.0;
	glFrustumx(left, right, buttom, top, zNear, zFar); 
	


    glDisable(GL_CLIP_PLANE_FAR);
	glDisable(GL_CLIP_PLANE_LRTB);
    glDrawArrays(GL_QUADS, 6*4, 6*4);

	//draw inside cube
	glMatrixMode(GL_MODELVIEW_MATRIX);
	glEnable(GL_LIGHTING);
	glLoadIdentity();
	//glDisable(GL_TEXTURE_2D);

	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	transX = -0.5;
	transY = -0.5;
	transZ = -0.5;
	glTranslatex(transX,transY,transZ);

	scaleX = 1.0;
	scaleY = 1.0;
	scaleZ = 1.0;
	//glScalex(scaleX, scaleY, scaleZ);

	rotateX = 0;
	rotateY = 0;
	rotateZ = GL_TYPET_ONE;
	rotateAngleY = 45.0;
	glRotatex(rotateAngleY, rotateX, rotateY, rotateZ);

	rotateX = GL_TYPET_ONE;
	rotateY = 0;
	rotateZ = 0;
	rotateAngleY = 35;
	glRotatex(rotateAngleY, rotateX, rotateY, rotateZ);

	rotateX = 0;
	rotateY = GL_TYPET_ONE;
	rotateZ = 0;
	glRotatex(cubeRotateAngle, rotateX, rotateY, rotateZ);
	
	rotateCamera(gCameraRotateAngleX, gCameraRotateAngleY);
	
	transX = -1.0;
	transY = 0.0;
	transZ = 6.0;
	glTranslatex(transX,transY,transZ);

	

	left = -1.2 * SCREEN_WIDTH / SCREEN_HEIGHT;
	right = 1.2 * SCREEN_WIDTH / SCREEN_HEIGHT;
	buttom = -1.2;
	top = 1.2;
	zNear = 3.0;
	zFar = 100.0;
	glFrustumx(left, right, buttom, top, zNear, zFar); 
	


    glDisable(GL_CLIP_PLANE_FAR);
	glDisable(GL_CLIP_PLANE_LRTB);
    glDrawArrays(GL_QUADS, 12*4, 6*4);

	

	
	

	

	
	//copy context sourcebuffer data into input address
	for(i = 0; i < pGLContext->sourcebufferStruct.numOfBytes/4; i++)
	{
		sourceBufferAddr[*sourceShift/4 + i] = pGLContext->sourcebufferStruct.pSourceBufferData[i];
	}
	*sourceShift += i * 4;
	//write end
	//sourceBufferAddr[*sourceShift/4] = 0x80000030;
	//*sourceShift += 4;
	//sourceBufferAddr[*sourceShift/4] = 0;
	//*sourceShift += 4;
	//sourceBufferAddr[*sourceShift/4] = 0;
	//*sourceShift += 4;
	//sourceBufferAddr[*sourceShift/4] = 0;
	//*sourceShift += 4;
	//sourceBufferAddr[*sourceShift/4] = 0;
	//*sourceShift += 4;
	//sourceBufferAddr[*sourceShift/4] = 0;
	//*sourceShift += 4;
    return 0;
}

//set the address and width/height of input texture
void setTexAttr(
	unsigned int texIndex, //0-outside cube, 1-video 2-inside cube
	unsigned int type    , //@ref sourcebuffer texture type
	unsigned int width   ,
	unsigned int height  ,
	unsigned int address , //need to set as sourcebuffer's address
	unsigned int source  , //0-flash, 1-ddr
	unsigned int sbWidth ,
	unsigned int sbHeight
	)
{
	gTexType[texIndex] = type;
	gTexWidth[texIndex] = width;
	gTexHeight[texIndex] = height;
	gSourcebufferWidth[texIndex] = sbWidth;
	gSourcebufferHeight[texIndex] = sbHeight;
	if(source == 0) //flash
		gTexAddress[texIndex] = address;
	else 
		gTexAddress[texIndex] = address + SXGL_DDR_OFFSET;

}

void rotateCntl(
	//rotate
	unsigned char touch_en,
	unsigned char press_en, //press the screen
	unsigned short x,
	unsigned short y
	)
{
	GLTypeT idleRotateAngle, movingRotateAngle; 
#ifdef PC_SIM
	char debug_code[1000];
#endif
	
	short movingX, movingY;
	idleRotateAngle = 2.0;
	movingRotateAngle = 10.0;
	if(rotateFlag == 0)
	{
		rotateAngle = 0.0;
		rotateAngleShort = 0;
		cubeRotateAngleShort = 0;
		rotateFlag = 1;
		state = IDLE;
		gCameraRotateAngleX = 0;
		gCameraRotateAngleY = 0;
		preTouchX = 0;
		preTouchY = 0;
		movingBackCnt = 0;
		backRotateAngleX = 0;
		backRotateAngleY = 0;
		cubeRotateAngle = 0;
		cubeRotateAngleShort = 0;

	}
	
	if(state == CAMERA) //moving camera according to touch
	{
		backRotateAngleX = 0;
		backRotateAngleY = 0;
		movingBackCnt = 0;
		
		if(touch_en)
		{
			untouchCnt = 0;
			if(press_en)
			{
				preTouchX = x;
				preTouchY = y;
				gCameraPreRotateAngleX = gCameraRotateAngleX;
				gCameraPreRotateAngleY = gCameraRotateAngleY;
				movingX = 0;
				movingY = 0;
			}
			else 
			{
				movingX = x - preTouchX;
				movingY = y - preTouchY;
			}
			gCameraRotateAngleX = gCameraPreRotateAngleX + movingX * 180 / SCREEN_WIDTH;
			gCameraRotateAngleY = gCameraPreRotateAngleY + movingY * 180 / SCREEN_HEIGHT;
		}
		else 
		{
			untouchCnt ++;
		}
	}
	else if(state == BACK) //rotate from current camera postion to 0
	{
		if(touch_en == 1)
		{
			preTouchX = x;
			preTouchY = y;
			gCameraPreRotateAngleX = 0;
			gCameraPreRotateAngleY = 0;
			return;
		}
		else if(movingBackCnt < TOTAL_BACK_CNT)
		{
			gCameraRotateAngleX = gCameraRotateAngleX - backRotateAngleX;
			gCameraRotateAngleY = gCameraRotateAngleY - backRotateAngleY;
			movingBackCnt = movingBackCnt + 1;
		}
	}
	else //idle
	{
		rotateAngleShort = (rotateAngleShort + IDLE_ROTATE_ANGLE) % 360;
		rotateAngle = rotateAngleShort * 1.0;
		gCameraRotateAngleX = 0;
		gCameraRotateAngleY = 0;
		backRotateAngleX = 0;
		backRotateAngleY = 0;
		untouchCnt = 0;
		movingBackCnt = 0;
		if(touch_en && press_en)
		{
			preTouchX = x;
			preTouchY = y;
			gCameraPreRotateAngleX = gCameraRotateAngleX;
			gCameraPreRotateAngleY = gCameraRotateAngleY;
		}
		else if(!touch_en)
		{
			preTouchX = 0;
			preTouchY = 0;
			gCameraPreRotateAngleX = 0;
			gCameraPreRotateAngleY = 0;
		}
	}

	//state change
	if(state == CAMERA)
	{
		if(untouchCnt >= UNTOUCH_THRESHOLD)
		{
			state = BACK;
			untouchCnt = 0;
			backRotateAngleX = gCameraRotateAngleX / TOTAL_BACK_CNT;
			backRotateAngleY = gCameraRotateAngleY / TOTAL_BACK_CNT;
		}
	}
	else if(state == BACK)
	{
		if(movingBackCnt >= TOTAL_BACK_CNT)
		{
			state = IDLE;
			movingBackCnt = 0;
		}
	}
	else 
	{
		if(touch_en)
		{
			state = CAMERA;
		}
	}
	cubeRotateAngleShort = (cubeRotateAngleShort + (GLushort)IDLE_ROTATE_ANGLE) % 360;
	cubeRotateAngle = -cubeRotateAngleShort * 1.0;

#ifdef PC_SIM
	sprintf(debug_code, "state : %1d", state);
	//ERROR_PRINT(debug_code);
#endif

}

void rotateCamera(
	GLTypeT rotateAngleX,
	GLTypeT rotateAngleY
	)
{
	GLTypeT rotateX, rotateY, rotateZ;
	//rotate camera
	rotateX = 0;
	rotateY = GL_TYPET_ONE;
	rotateZ = 0;
	glRotatex(rotateAngleX, rotateX, rotateY, rotateZ);

	rotateX = GL_TYPET_ONE;
	rotateY = 0;
	rotateZ = 0;
	glRotatex(-rotateAngleY, rotateX, rotateY, rotateZ);
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件

#endif