#include "sxGL.h"
#include "sxGLObject.h"
#include "sxMatrix.h"
#include "stdio.h"
#include "sxGLBuffer.h"

#ifdef SXGL_LIB

GLContextStruct  gGLContext;
GLContextStruct* pGLContext;
GLVertexShadingObjectStructure gGLVSO;
GLVertexShadingObjectStructure* pGLVSO;
//GLint pSourceBufferData[MAX_BYTE_OF_3D_SOURCEBUFFER];

void GLContextInit()
{
	pGLContext = &gGLContext;
	pGLVSO     = &gGLVSO;
    pGLContext->state.GLEnableState.u32_value = 0;
    pGLContext->state.GLEnableClientState.u8_value = 0;
    pGLContext->state.GLClientTextureState = 0;
    pGLContext->state.GLDrawMode = 0;
    pGLContext->pointParam.distanceAttenuation = 0;
    pGLContext->pointParam.fadeThresholdSize = 0;
    pGLContext->pointParam.size = 0;
    pGLContext->pointParam.sizeMax = 0;
    pGLContext->pointParam.sizeMin = 0;
    pGLContext->lineWidth = 0;
    pGLContext->vertexPointerParam.type = 0;
    pGLContext->vertexPointerParam.stride = 0;
    pGLContext->vertexPointerParam.size = 0;
    pGLContext->vertexPointerParam.pointer = NULL;
	pGLContext->normalPointerParam.type = 0;
    pGLContext->normalPointerParam.stride = 0;
    pGLContext->normalPointerParam.size = 0;
    pGLContext->normalPointerParam.pointer = NULL;
    pGLContext->pointSizePointerParam.type = 0;
    pGLContext->pointSizePointerParam.stride = 0;
    pGLContext->pointSizePointerParam.size = 0;
    pGLContext->pointSizePointerParam.pointer = NULL;
	pGLContext->TOPointer = NULL;
	pGLContext->TMPointer = NULL;
    pGLContext->scene.curPrimitive.vertexInfo[0].numOfVertex = 0;
	pGLContext->scene.curPrimitive.vertexInfo[1].numOfVertex = 0;
	pGLContext->scene.curPrimitive.primitiveIndex = 0;
    pGLContext->scene.light.vertex.position[0] = 0;
    pGLContext->scene.light.vertex.position[1] = 0;
    pGLContext->scene.light.vertex.position[2] = 0;
    pGLContext->scene.light.vertex.position[3] = GL_FIXED_ONE;
    glSetIndetityMatrix(&pGLContext->scene.curPrimitive.textureMatrix);
    glSetIndetityMatrix(&pGLContext->scene.matrix.modelviewMatrix);
    glSetIndetityMatrix(&pGLContext->scene.matrix.textureMatrix);
    glSetIndetityMatrix(&pGLContext->scene.matrix.projectionMatrix);
	glSetIndetityMatrix(&pGLContext->scene.curPrimitive.rotateMatrix);
	pGLContext->scene.matrix.pCurrentMatrix = &pGLContext->scene.matrix.modelviewMatrix;
    pGLContext->state.GLEnableState.bit.GL_STATE_CLIP_FAR_PLANE = 1;
    pGLContext->state.GLEnableState.bit.GL_STATE_CLIP_LRTB_PLANE = 1;
    pGLContext->cull.frontFace = 0;
    pGLContext->curNumOfTextureObject = 0;
    pGLContext->curActiveTextureObject = 0;
    //pGLContext->sourcebufferStruct.pSourceBufferData = NULL;
    //pGLContext->sourcebufferStruct.pSourceBufferData = (GLint*)glGenBuffer(MAX_BYTE_OF_3D_SOURCEBUFFER);
	//pGLContext->sourcebufferStruct.pSourceBufferData = pSourceBufferData;
    pGLContext->sourcebufferStruct.numOfBytes = 0;
    pGLContext->errorFifo.empty = 1;
    pGLContext->errorFifo.full = 0;
    pGLContext->errorFifo.wp = 0;
    pGLContext->errorFifo.rp = 0;
    glViewport(0, 0, 800, 480);

}

#endif