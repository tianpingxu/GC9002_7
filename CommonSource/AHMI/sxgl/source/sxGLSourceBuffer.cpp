#include "sxGLSourceBuffer.h"
#include "sxGLObject.h"
#include "stdio.h"
#include "textureClass.h"

#ifdef SXGL_LIB

//only for debug
#ifdef SXGL_OBJECT_DEBUG
GLubyte color_cnt = 0;
GLubyte color[4];
#endif

void writeSourceBuffer(GLVertexPosStruct vertexPos[4], GLMatrix3x3Struct* mat, textureObjectInStruct *textureObject, GLubyte lightEn, GLubyte lightAlpha, GLColorStruct diffuse)
{
    GLuint sourceShift;
    GLubyte textureType;
    GLubyte useAlpha;
    GLubyte alphaBlendType;
    GLubyte i;
    GLubyte boxStartX, boxStartY, boxEndX, boxEndY;
    GLuint texAddr;
	GLushort vertexPosX, vertexPosY;
     
    sourceShift = pGLContext->sourcebufferStruct.numOfBytes/4;
	if(lightEn)
	{
		writeLightPureColor(vertexPos, mat, textureObject, lightEn, lightAlpha, diffuse);
		writeLightSourceBuffer(vertexPos, mat, textureObject, lightEn, lightAlpha, diffuse);
		sourceShift = pGLContext->sourcebufferStruct.numOfBytes/4;
		useAlpha = 1;
		alphaBlendType = 0; //or
	}
	else 
	{
		useAlpha = 0;
		alphaBlendType = 0;
	}
    if (pGLContext->state.GLEnableState.bit.GL_STATE_TEXTURE_2D == 0)
    {
        textureType = 3; //purecolor
		

        texAddr = textureObject->texAddr;
#ifdef SXGL_OBJECT_DEBUG
        color[3] = 0xff;
        //color[2] = color_cnt & 0x4 ? 0x0 : 0xff;
        //color[1] = color_cnt & 0x2 ? 0x0 : 0xff;
        //color[0] = color_cnt & 0x1 ? 0x0 : 0xff;
		color[2] = 0;
		color[1] = 0;
		color[0] = 0xff;
        texAddr = (color[3] << (8 * 3)) + (color[0] << (8 * 2)) + (color[1] << (8 * 1)) + color[2];
        color_cnt = (color_cnt + 1) % 9;
#endif
    }
    else
    {
        textureType = textureObject->type;
		texAddr = textureObject->texAddr;
    }
    //useAlpha = 0;
    //alphaBlendType = 0;
    boxStartX = 0;
    boxStartY = 0;
    boxEndX = 1920 / 32;
    boxEndY = 1080 / 32;

    pGLContext->sourcebufferStruct.pSourceBufferData[sourceShift + 0] = 
        (0 << 31) + //short sourcebuffer 
        (0 << 24) + //back filter
        (0 << 18) + //rgb order
        (0 << 15) + //last
        (0 << 12) + //back color reverse
        (0 << 11) + //front color reverse
        (0 << 8) + //last
        (textureType << 4) +
        (useAlpha << 3) +
        alphaBlendType;

	//set vertex position
	//vertex in sourcebuffer is 1.11.4
	//need to transfer vertex index
    //for (i = 0; i < 4; i++)
    //{
		//vertexPosX = (GLushort)(vertexPos[i].pos[0].value * 16.0);
		//vertexPosY = (GLushort)(vertexPos[i].pos[1].value * 16.0);
        //pGLContext->sourcebufferStruct.pSourceBufferData[sourceShift + 1 + i] = 
        //    ( vertexPosX << 16) + 
        //    ( vertexPosY      );
    //}

	vertexPosX = (GLushort)(vertexPos[0].pos[0] * 16.0);
	vertexPosY = (GLushort)(vertexPos[0].pos[1] * 16.0);
    pGLContext->sourcebufferStruct.pSourceBufferData[sourceShift + 1 + 0] = 
         ( vertexPosX << 16) + 
         ( vertexPosY      );

	vertexPosX = (GLushort)(vertexPos[3].pos[0] * 16.0);
	vertexPosY = (GLushort)(vertexPos[3].pos[1] * 16.0);
    pGLContext->sourcebufferStruct.pSourceBufferData[sourceShift + 1 + 1] = 
         ( vertexPosX << 16) + 
         ( vertexPosY      );

	vertexPosX = (GLushort)(vertexPos[2].pos[0] * 16.0);
	vertexPosY = (GLushort)(vertexPos[2].pos[1] * 16.0);
    pGLContext->sourcebufferStruct.pSourceBufferData[sourceShift + 1 + 2] = 
         ( vertexPosX << 16) + 
         ( vertexPosY      );

	vertexPosX = (GLushort)(vertexPos[1].pos[0] * 16.0);
	vertexPosY = (GLushort)(vertexPos[1].pos[1] * 16.0);
    pGLContext->sourcebufferStruct.pSourceBufferData[sourceShift + 1 + 3] = 
         ( vertexPosX << 16) + 
         ( vertexPosY      );

    for (i = 0; i < 6; i++)
    {
        pGLContext->sourcebufferStruct.pSourceBufferData[sourceShift + 5 + i] = getFixFromTA(mat->matrixA[i]);
    }

	for (i = 0; i < 2; i++)
    {
        pGLContext->sourcebufferStruct.pSourceBufferData[sourceShift + 11 + i] = getFixFromTG(mat->matrixG[i]);
    }

	//if(gExtendEn)
	//	aligned_width = img_align(textureObject->width, textureObject->height, textureType << 4);
	//else 
	//	aligned_width = textureObject->width;
	//pGLContext->sourcebufferStruct.pSourceBufferData[sourceShift + 13] = (textureObject->width << 16) + textureObject->height;
	pGLContext->sourcebufferStruct.pSourceBufferData[sourceShift + 13] = (textureObject->width << 16) +  textureObject->height;
    
    pGLContext->sourcebufferStruct.pSourceBufferData[sourceShift + 14] = texAddr;
    pGLContext->sourcebufferStruct.pSourceBufferData[sourceShift + 15] = boxStartX + (boxStartY << 8) + (boxEndX << 16) + (boxEndY << 24);

	pGLContext->sourcebufferStruct.numOfBytes += 16 * 4;



}

void writeLightSourceBuffer(GLVertexPosStruct vertexPos[4], GLMatrix3x3Struct* mat, textureObjectInStruct *textureObject, GLubyte lightEn, GLubyte lightAlpha, GLColorStruct diffuse)
{
    GLuint sourceShift;
    GLubyte textureType;
    GLubyte useAlpha;
    GLubyte alphaBlendType;
    GLubyte i;
    GLubyte boxStartX, boxStartY, boxEndX, boxEndY;
    GLuint texAddr;
	GLushort vertexPosX, vertexPosY;
	GLubyte color[4];
    sourceShift = pGLContext->sourcebufferStruct.numOfBytes/4;
    textureType = 8; //puremask
    texAddr = lightAlpha << 24;
    useAlpha = 0;
    alphaBlendType = 0;
    boxStartX = 0;
    boxStartY = 0;
    boxEndX = 1920 / 32;
    boxEndY = 1080 / 32;

    pGLContext->sourcebufferStruct.pSourceBufferData[sourceShift + 0] = 
        (0 << 31) + //short sourcebuffer 
        (0 << 24) + //back filter
        (0 << 18) + //rgb order
        (0 << 15) + //last
        (0 << 12) + //back color reverse
        (0 << 11) + //front color reverse
        (0 << 8) + //last
        (textureType << 4) +
        (useAlpha << 3) +
        alphaBlendType;

	//set vertex position
	//vertex in sourcebuffer is 1.11.4
    vertexPosX = (GLushort)(vertexPos[0].pos[0] * 16.0);
	vertexPosY = (GLushort)(vertexPos[0].pos[1] * 16.0);
    pGLContext->sourcebufferStruct.pSourceBufferData[sourceShift + 1 + 0] = 
         ( vertexPosX << 16) + 
         ( vertexPosY      );

	vertexPosX = (GLushort)(vertexPos[3].pos[0] * 16.0);
	vertexPosY = (GLushort)(vertexPos[3].pos[1] * 16.0);
    pGLContext->sourcebufferStruct.pSourceBufferData[sourceShift + 1 + 1] = 
         ( vertexPosX << 16) + 
         ( vertexPosY      );

	vertexPosX = (GLushort)(vertexPos[2].pos[0] * 16.0);
	vertexPosY = (GLushort)(vertexPos[2].pos[1] * 16.0);
    pGLContext->sourcebufferStruct.pSourceBufferData[sourceShift + 1 + 2] = 
         ( vertexPosX << 16) + 
         ( vertexPosY      );

	vertexPosX = (GLushort)(vertexPos[1].pos[0] * 16.0);
	vertexPosY = (GLushort)(vertexPos[1].pos[1] * 16.0);
    pGLContext->sourcebufferStruct.pSourceBufferData[sourceShift + 1 + 3] = 
         ( vertexPosX << 16) + 
         ( vertexPosY      );

	//matrix
    for (i = 0; i < 6; i++)
    {
        pGLContext->sourcebufferStruct.pSourceBufferData[sourceShift + 5 + i] = getFixFromTA(mat->matrixA[i]);
    }

	for (i = 0; i < 2; i++)
    {
        pGLContext->sourcebufferStruct.pSourceBufferData[sourceShift + 11 + i] = getFixFromTG(mat->matrixG[i]);
    }

    pGLContext->sourcebufferStruct.pSourceBufferData[sourceShift + 13] = (textureObject->width << 16) + textureObject->height;
    pGLContext->sourcebufferStruct.pSourceBufferData[sourceShift + 14] = texAddr;
    pGLContext->sourcebufferStruct.pSourceBufferData[sourceShift + 15] = boxStartX + (boxStartY << 8) + (boxEndX << 16) + (boxEndY << 24);

	pGLContext->sourcebufferStruct.numOfBytes += 16 * 4;



}

void writeLightPureColor(GLVertexPosStruct vertexPos[4], GLMatrix3x3Struct* mat, textureObjectInStruct *textureObject, GLubyte lightEn, GLubyte lightAlpha, GLColorStruct diffuse)
{
	 GLuint sourceShift;
    GLubyte textureType;
    GLubyte useAlpha;
    GLubyte alphaBlendType;
    GLubyte i;
    GLubyte boxStartX, boxStartY, boxEndX, boxEndY;
    GLuint texAddr;
	GLushort vertexPosX, vertexPosY;
	GLubyte color[4];
    sourceShift = pGLContext->sourcebufferStruct.numOfBytes/4;
    textureType = 3; //purecolor
    texAddr = (0xff << 24) + (diffuse.red << 16) + (diffuse.green << 8) + diffuse.blue;
    useAlpha = 0;
    alphaBlendType = 0;
    boxStartX = 0;
    boxStartY = 0;
    boxEndX = 1920 / 32;
    boxEndY = 1080 / 32;

    pGLContext->sourcebufferStruct.pSourceBufferData[sourceShift + 0] = 
        (0 << 31) + //short sourcebuffer 
        (0 << 24) + //back filter
        (0 << 18) + //rgb order
        (0 << 15) + //last
        (0 << 12) + //back color reverse
        (0 << 11) + //front color reverse
        (0 << 8) + //last
        (textureType << 4) +
        (useAlpha << 3) +
        alphaBlendType;

	//set vertex position
	//vertex in sourcebuffer is 1.11.4
    vertexPosX = (GLushort)(vertexPos[0].pos[0] * 16.0);
	vertexPosY = (GLushort)(vertexPos[0].pos[1] * 16.0);
    pGLContext->sourcebufferStruct.pSourceBufferData[sourceShift + 1 + 0] = 
         ( vertexPosX << 16) + 
         ( vertexPosY      );

	vertexPosX = (GLushort)(vertexPos[3].pos[0] * 16.0);
	vertexPosY = (GLushort)(vertexPos[3].pos[1] * 16.0);
    pGLContext->sourcebufferStruct.pSourceBufferData[sourceShift + 1 + 1] = 
         ( vertexPosX << 16) + 
         ( vertexPosY      );

	vertexPosX = (GLushort)(vertexPos[2].pos[0] * 16.0);
	vertexPosY = (GLushort)(vertexPos[2].pos[1] * 16.0);
    pGLContext->sourcebufferStruct.pSourceBufferData[sourceShift + 1 + 2] = 
         ( vertexPosX << 16) + 
         ( vertexPosY      );

	vertexPosX = (GLushort)(vertexPos[1].pos[0] * 16.0);
	vertexPosY = (GLushort)(vertexPos[1].pos[1] * 16.0);
    pGLContext->sourcebufferStruct.pSourceBufferData[sourceShift + 1 + 3] = 
         ( vertexPosX << 16) + 
         ( vertexPosY      );

	//matrix
    for (i = 0; i < 6; i++)
    {
        pGLContext->sourcebufferStruct.pSourceBufferData[sourceShift + 5 + i] = getFixFromTA(mat->matrixA[i]);
    }

	for (i = 0; i < 2; i++)
    {
        pGLContext->sourcebufferStruct.pSourceBufferData[sourceShift + 11 + i] = getFixFromTG(mat->matrixG[i]);
    }

    pGLContext->sourcebufferStruct.pSourceBufferData[sourceShift + 13] = (textureObject->width << 16) + textureObject->height;
    pGLContext->sourcebufferStruct.pSourceBufferData[sourceShift + 14] = texAddr;
    pGLContext->sourcebufferStruct.pSourceBufferData[sourceShift + 15] = boxStartX + (boxStartY << 8) + (boxEndX << 16) + (boxEndY << 24);

	pGLContext->sourcebufferStruct.numOfBytes += 16 * 4;
}

#endif