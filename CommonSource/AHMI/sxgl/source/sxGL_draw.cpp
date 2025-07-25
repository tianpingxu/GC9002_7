#include "sxGL.h"
#include "sxGLObject.h"
#include "sxGLState.h"
#include "sxGL_draw.h"
#include "sxGLPipeline.h"
#include "sxGLBuffer.h"
#include "sxMatrix.h"
#include "sxGLSourceBuffer.h"
#include "sxLight.h"
#include "stdlib.h"
#include "sxGLBuffer.h"
#include "sxClip.h"
#include "sxCull.h"

#ifdef SXGL_LIB

GLVertexPosStruct finalVertexPosition[4];
GLMatrixStruct finalMatrix4x4;
GLMatrixStruct finalMatrix4x4_temp;

void getDataFromPointer(GLint size, GLenum type, GLsizei stride, const GLvoid* pointer, GLuint index, GLTypeT* pointerOut)
{
    GLuint indexIn; //index multiply stride
	GLubyte i;
    indexIn = (stride + 1) * size * index;
	GLTypeT value;
    //set z = 0, w =1
    if (size <= 3) 
    {
        *((GLTypeT*)pointerOut + 3) = 1.0;
    }
    if (size == 2)
    {
        *((GLTypeT*)pointerOut + 2) = 0.0;
    }
    switch (type)
    {
    case GL_BYTE:
        for (i = 0; i < size; i++)
        {
			value = (GLfloat)(*((GLbyte*)pointer + indexIn + i) * 1.0);
            *((GLTypeT*)pointerOut + i) = value;
        }
        break;
    case GL_SHORT:
        for (i = 0; i < size; i++)
        {
			value = (*((GLshort*)pointer + indexIn + i) * 1.0);
            *((GLTypeT*)pointerOut + i) = value;
        }
        break;
	case GL_INT:
		for (i = 0; i < size; i++)
        {
			value = float(*((GLint*)pointer + indexIn + i) * 1.0 );
			*((GLTypeT*)pointerOut + i) = value;
		}
		break;
    case GL_FIXED:
    
        for (i = 0; i < size; i++)
        {
            *((GLTypeT*)pointerOut + i) = *((GLTypeT*)pointer + indexIn + i);
        }
        break;
    default:
        pushError(GL_INVALID_ENUM);
        break;
    }
}


void genBufferFromPointer(GLint size, GLenum type, GLsizei stride, const GLvoid* pointer, GLuint minIndex, GLuint maxIndex, GLvoid* pointerOut)
{
    GLubyte i;
    GLubyte dataByte;
    GLuint indexNum;
    GLuint indexI;
    GLuint byteBase;
	GLuint byteBasePointer;
    indexNum = maxIndex - minIndex + 1;
    switch (type)
    {
    case GL_BYTE:
        dataByte = 1;
        break;
    case GL_SHORT:
        dataByte = 2;
        break;
    case GL_FIXED:
    case GL_INT:
        dataByte = 4;
        break;
    default:
        dataByte = 0;
        pushError(GL_INVALID_ENUM);
        return;
    }
    //*pointerOut = glGenBuffer(dataByte * size * indexNum);
    for (indexI = 0; indexI < indexNum; indexI++)
    {
        byteBase = indexI * dataByte * size;
		byteBasePointer = (indexI + minIndex) * dataByte * size;
        for (i = 0; i < dataByte * size; i++)
        {
            *((GLubyte*)(pointerOut) + byteBase + i) = *((GLubyte*)pointer + i + byteBasePointer * (stride + 1));
        }
    }
    

}

//set pirimitve param
void setPrimitiveParam(GLint size, GLenum type, GLsizei stride, const GLvoid* pointer, drawPointerParamStruct* primitiveParam)
{
    primitiveParam->size = size;
    primitiveParam->type = type;
    primitiveParam->stride = stride;
    primitiveParam->pointer = (GLvoid *)pointer;
}

void drawPrimitiveIndex(GLenum mode, GLuint pointerIndex, GLuint primitiveIndex, GLuint modelIndex, GLMatrixStruct* mvpMat) //take a new primitive index and assemble
{
    GLuint curVertexIndex;
    

    switch (mode)
    {
    case GL_TRIANGLES:
        if (pGLContext->scene.curPrimitive.vertexInfo[0].numOfVertex == 2)
        {
            curVertexIndex = primitiveIndex % 3;
            pGLContext->scene.curPrimitive.vertexInfo[0].numOfVertex = 3;
            setPrimitiveVertex(pointerIndex, curVertexIndex);
            setPrimitiveModelVertex(modelIndex, curVertexIndex);
            drawCurrentPrimitive();
            pGLContext->scene.curPrimitive.vertexInfo[0].numOfVertex = 0;
        }
        else
        {
            curVertexIndex = primitiveIndex % 3;
            setPrimitiveVertex(pointerIndex, curVertexIndex);
            setPrimitiveModelVertex(modelIndex, curVertexIndex);
            pGLContext->scene.curPrimitive.vertexInfo[0].numOfVertex++;
        }
        break;
    case GL_TRIANGLE_STRIP: //each new point replace the oldest point
        if (pGLContext->scene.curPrimitive.vertexInfo[0].numOfVertex == 0 || pGLContext->scene.curPrimitive.vertexInfo[0].numOfVertex == 1)
        {
            curVertexIndex = primitiveIndex % 3;
            setPrimitiveVertex(pointerIndex, curVertexIndex);
            setPrimitiveModelVertex(modelIndex, curVertexIndex);
            pGLContext->scene.curPrimitive.vertexInfo[0].numOfVertex++;
        }
        else
        {
            curVertexIndex = primitiveIndex % 3;
            pGLContext->scene.curPrimitive.vertexInfo[0].numOfVertex = 3;
            setPrimitiveVertex(pointerIndex, curVertexIndex);
            setPrimitiveModelVertex(modelIndex, curVertexIndex);
            drawCurrentPrimitive();
        }        
        break;
    case GL_TRIANGLE_FAN:
        if (pGLContext->scene.curPrimitive.vertexInfo[0].numOfVertex == 0 || pGLContext->scene.curPrimitive.vertexInfo[0].numOfVertex == 1)
        {
            curVertexIndex = primitiveIndex;
            setPrimitiveVertex(pointerIndex, curVertexIndex);
            setPrimitiveModelVertex(modelIndex, curVertexIndex);
            pGLContext->scene.curPrimitive.vertexInfo[0].numOfVertex++;
        }
        else
        {
            curVertexIndex = ((primitiveIndex - 1) % 2) + 1;
            setPrimitiveVertex(pointerIndex, curVertexIndex);
            setPrimitiveModelVertex(modelIndex, curVertexIndex);
            pGLContext->scene.curPrimitive.vertexInfo[0].numOfVertex = 3;
            drawCurrentPrimitive();
        }
        break;
    case GL_QUADS:
        if (pGLContext->scene.curPrimitive.vertexInfo[0].numOfVertex == 3)
        {
            curVertexIndex = pGLContext->scene.curPrimitive.vertexInfo[0].numOfVertex;
            pGLContext->scene.curPrimitive.vertexInfo[0].numOfVertex = 4;
            setPrimitiveVertex(pointerIndex, curVertexIndex);
            setPrimitiveModelVertex(modelIndex, curVertexIndex);
            drawCurrentPrimitive();
            pGLContext->scene.curPrimitive.vertexInfo[0].numOfVertex = 0;
        }
        else
        {
            curVertexIndex = pGLContext->scene.curPrimitive.vertexInfo[0].numOfVertex;
            setPrimitiveVertex(pointerIndex, curVertexIndex);
            setPrimitiveModelVertex(modelIndex, curVertexIndex);
            pGLContext->scene.curPrimitive.vertexInfo[0].numOfVertex++;
        }
        break;
    default:
        pushError(GL_INVALID_ENUM);
        break;
    }

    
}

void setPrimitiveVertex(GLsizei index, GLubyte curVertexIndex) //set primitive's vertex with index
{
	//set vertex position
    getDataFromPointer(
        pGLContext->vertexPointerParam.size,
        pGLContext->vertexPointerParam.type,
        0,
        pGLVSO->vertexPointer,
        index,
        pGLContext->scene.curPrimitive.vertexInfo[0].vertex[curVertexIndex].pos
    );
}

void setPrimitiveNormal(GLsizei index) //set primitive's normal
{
	if(pGLContext->state.GLEnableClientState.bit.GL_STATE_NORMAL_ARRAY == 0) //set normal as 0
	{
		pGLContext->scene.curPrimitive.normal.pos[0] = 0;
		pGLContext->scene.curPrimitive.normal.pos[1] = 0;
		pGLContext->scene.curPrimitive.normal.pos[2] = 0;
	}
	else 
	{
		getDataFromPointer(
			pGLContext->normalPointerParam.size,
			pGLContext->normalPointerParam.type,
			0,
			pGLContext->normalPointerParam.pointer,
			index,
			pGLContext->scene.curPrimitive.normal.pos
		);
	}
}

void setPrimitiveTextureMatrix(GLsizei index) //set primitive's original vertex
{
	GLubyte i;
	if(pGLContext->state.GLEnableClientState.bit.GL_STATE_TEXTURE_COORD_ARRAY == 0) //set texture matrix as 1
	{
		glSetIndetityMatrix(&pGLContext->scene.curPrimitive.textureMatrix);
	}
	else 
	{
		//glSetMatrixStructFrom3x3(&pGLContext->scene.curPrimitive.textureMatrix, &pGLContext->TMPointer[index]);
		for(i = 0; i < 12; i++)
		{
			pGLContext->scene.curPrimitive.textureMatrix.matrixA[i] = pGLContext->TMPointer[index].matrixA[i];
		}
		for( i = 0; i < 3; i++)
		{
			pGLContext->scene.curPrimitive.textureMatrix.matrixG[i] = pGLContext->TMPointer[index].matrixG[i];
		}
		pGLContext->scene.curPrimitive.textureMatrix.matrixI = pGLContext->TMPointer[index].matrixI;
	}
}

void setPrimitiveTextureObject(GLsizei index) //set primitive's original vertex
{
	
	pGLContext->scene.curPrimitive.textureObject = pGLContext->TOPointer[index];
	
}


void setPrimitiveMVPMatrix(GLsizei index, const GLMatrixStruct* mvp) //set primitive's original vertex
{
	glSetMatrixStruct(&pGLContext->scene.curPrimitive.mvpMatrix, mvp);
}

void setPrimitiveModelVertex(GLsizei index, GLubyte curVertexIndex) //set primitive's original vertex
{
    getDataFromPointer(
        pGLContext->vertexPointerParam.size,
        pGLContext->vertexPointerParam.type,
        pGLContext->vertexPointerParam.stride,
        pGLContext->vertexPointerParam.pointer,
        index,
        pGLContext->scene.curPrimitive.modelVertex[curVertexIndex].pos
    );
}

void generateMVPMat(GLMatrixStruct* mvpMat)
{
    GLMatrixStruct mvpMat_temp;
	GLubyte i;
    glSetIndetityMatrix(mvpMat);
    glSetIndetityMatrix(&mvpMat_temp);
    glMatrixLeftMul(&pGLContext->scene.matrix.modelviewMatrix, &mvpMat_temp, mvpMat);
    glSetMatrixStruct(&mvpMat_temp, mvpMat);

    glMatrixLeftMul(&pGLContext->scene.matrix.projectionMatrix, &mvpMat_temp, mvpMat);
    glSetMatrixStruct(&mvpMat_temp, mvpMat);

}

void drawCurrentPrimitive() //draw current context's primitive
{
    
    
    GLubyte i, j, pri_i;	
	GLubyte lightAlpha;
	GLTypeT two;

	//set primitive value
	setPrimitiveNormal(pGLContext->scene.curPrimitive.primitiveIndex); //set primitive's normal
	setPrimitiveTextureMatrix(pGLContext->scene.curPrimitive.primitiveIndex); //set primitive's texture matrix
	setPrimitiveTextureObject(pGLContext->scene.curPrimitive.primitiveIndex); //set primitive's texture information
	pGLContext->scene.curPrimitive.primitiveIndex ++;

    

	//light
	lightAlpha = dirctional_light_for_primitive(&pGLContext->scene.curPrimitive.normal, &pGLContext->scene.curPrimitive.rotateMatrix, &pGLContext->scene.light.spotDirection);
	if(pGLContext->state.GLEnableState.bit.GL_STATE_LIGHT0 == 0) //no current light
		lightAlpha = 0;

	//clip and cull
	pGLContext->scene.curPrimitive.vertexInfo[1].numOfVertex = 0;
	pGLContext->scene.curPrimitive.vertexInfo[2].numOfVertex = 0;
	pGLContext->scene.curPrimitive.vertexInfo[3].numOfVertex = 0;
	glClipCurrentPrimitive(pGLContext->scene.curPrimitive.vertexInfo);
	for(pri_i = 0; pri_i < 4; pri_i = pri_i + 1)
	{
		if(pGLContext->scene.curPrimitive.vertexInfo[pri_i].numOfVertex != 0)
		{
			glCullCurrentPrimitive(&pGLContext->scene.curPrimitive.vertexInfo[pri_i]);
		}
	}
	


    //viewport 
    glGenViewportMatrix(&pGLContext->scene.curPrimitive.viewPortMatrix);
    
    //compute final position
	for(pri_i = 0; pri_i < 4; pri_i = pri_i + 1)
	{
		if(pGLContext->scene.curPrimitive.vertexInfo[pri_i].numOfVertex != 0)
		{
			for (i = 0; i < pGLContext->scene.curPrimitive.vertexInfo[pri_i].numOfVertex; i++)
			{
			    position_mul_m4(
					&pGLContext->scene.curPrimitive.viewPortMatrix, 
					&pGLContext->scene.curPrimitive.vertexInfo[pri_i].vertex[i], 
					&finalVertexPosition[i]
				);
				//for(j = 0; j < 4; j++)
				//{
				//	finalVertexPosition[i].pos[j] = finalVertexPosition[i].pos[j] / finalVertexPosition[i].pos[3];
				//}
			}

			if(pGLContext->scene.curPrimitive.vertexInfo[pri_i].numOfVertex == 3)
			{
				//set vertex 4 as vertex 3
				two = 2.0;
				finalVertexPosition[3].pos[0] = finalVertexPosition[2].pos[0];
				finalVertexPosition[3].pos[1] = finalVertexPosition[2].pos[1];
				finalVertexPosition[3].pos[2] = 0;
				finalVertexPosition[3].pos[3] = GL_TYPET_ONE;
				//set vertex 3 as middle point of 2 and 4
				finalVertexPosition[2].pos[0] = (finalVertexPosition[1].pos[0] + finalVertexPosition[3].pos[0]) / two;
				finalVertexPosition[2].pos[1] = (finalVertexPosition[1].pos[1] + finalVertexPosition[3].pos[1]) / two;
				finalVertexPosition[2].pos[2] = 0;
				finalVertexPosition[2].pos[3] = GL_TYPET_ONE;
			}
			if(pGLContext->scene.curPrimitive.vertexInfo[pri_i].numOfVertex == 2)
			{
				;//TODO
			}
			
			//compute final matrix 
			glMatrixLeftMul(
				&pGLContext->scene.curPrimitive.mvpMatrix, 
				&pGLContext->scene.curPrimitive.textureMatrix, 
				&finalMatrix4x4_temp
				);

			glMatrixLeftMul(&pGLContext->scene.curPrimitive.viewPortMatrix, &finalMatrix4x4_temp, &finalMatrix4x4);
			GLVertexPosStruct vertex0 = {1,1,0,1};
			GLVertexPosStruct vertex0_t ;
			position_mul_m4(
					&finalMatrix4x4, 
					&vertex0, 
					&vertex0_t
				);
			for(j = 0; j < 4; j++)
				{
					vertex0_t.pos[j] = vertex0_t.pos[j] / vertex0_t.pos[3];
				}
			GLVertexPosStruct vertex1 = {253,1,0,1};
			GLVertexPosStruct vertex1_t ;
			position_mul_m4(
					&finalMatrix4x4, 
					&vertex1, 
					&vertex1_t
				);
			GLVertexPosStruct vertex2 = {253,253,0,1};
			GLVertexPosStruct vertex2_t ;
			position_mul_m4(
					&finalMatrix4x4, 
					&vertex2, 
					&vertex2_t
				);
			GLVertexPosStruct vertex3 = {1,253,0,1};
			GLVertexPosStruct vertex3_t ;
			position_mul_m4(
					&finalMatrix4x4, 
					&vertex3, 
					&vertex3_t
				);
			for(j = 0; j < 4; j++)
				{
					vertex1_t.pos[j] = vertex1_t.pos[j] / vertex1_t.pos[3];
				}
			for(j = 0; j < 4; j++)
				{
					vertex2_t.pos[j] = vertex2_t.pos[j] / vertex2_t.pos[3];
				}
			for(j = 0; j < 4; j++)
				{
					vertex3_t.pos[j] = vertex3_t.pos[j] / vertex3_t.pos[3];
				}
			m4tom3(&finalMatrix4x4, &pGLContext->scene.curPrimitive.finalMatrix);

			//write source buffer
 			writeSourceBuffer(finalVertexPosition, &pGLContext->scene.curPrimitive.finalMatrix, &pGLContext->scene.curPrimitive.textureObject, pGLContext->state.GLEnableState.bit.GL_STATE_LIGHTING, lightAlpha, pGLContext->scene.light.diffuse);
		}
	}

}

//draw primitive
void glVertexPointer(GLint size, GLenum type, GLsizei stride, const GLvoid* pointer)
{
    setPrimitiveParam(size, type, stride, pointer, &pGLContext->vertexPointerParam);
}


void glNormPointer(GLint size, GLenum type, GLsizei stride, const GLvoid* pointer)
{
    setPrimitiveParam(size, type, stride, pointer, &pGLContext->normalPointerParam);
}

void glPointSizePointerOES(GLenum type, GLsizei stride, const GLvoid* pointer)
{
    GLint size = 1;
    setPrimitiveParam(size, type, stride, pointer, &pGLContext->pointSizePointerParam);
}

void glTOPointer(const GLvoid *pointer) //set texture matrix
{
	pGLContext->TOPointer = (textureObjectInStruct*) pointer;
}
void glTMPointer(const GLvoid *pointer) //set texture object pointer
{
	pGLContext->TMPointer = (GLMatrixStruct*) pointer;
}

void glDrawArrays(GLenum mode, GLint first, GLsizei count)
{
	GLsizei i;
    //vertex shading
    generateMVPMat(&pGLContext->scene.curPrimitive.mvpMatrix);
	

    vertexShadingIndex(
        &pGLContext->scene.curPrimitive.mvpMatrix,
        first,
        first + count - 1
    );

    for ( i = 0; i < count; i++)
    {
        drawPrimitiveIndex(mode, i, i, i+first, &pGLContext->scene.curPrimitive.mvpMatrix);
    }
    pGLContext->scene.curPrimitive.vertexInfo[0].numOfVertex = 0;
	//glDeleteBuffer(pGLVSO->vertexPointer);
}

void glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices)
{
    GLuint curIndex;
	GLsizei i;
    GLuint minIndex = MAX_NUM_OF_SHADED_VERTEX;
    GLuint maxIndex = 0;

	generateMVPMat(&pGLContext->scene.curPrimitive.mvpMatrix);

    //find min and max index
    for (i = 0; i < count; i++)
    {
        switch (type)
        {
        case GL_UNSIGNED_BYTE:
            curIndex = (GLuint)(*((GLubyte*)indices + i));
            break;
        case GL_UNSIGNED_SHORT:
            curIndex = (GLuint)(*((GLushort*)indices + i));
            break;
        case GL_UNSIGNED_INT:
            curIndex = (GLuint)(*((GLuint*)indices + i));
            break;
        default:
            curIndex = 0;
            pushError(GL_INVALID_ENUM);
            return;
        }
        if (curIndex < minIndex)
            minIndex = curIndex;
        if (curIndex > maxIndex)
            maxIndex = curIndex;
    }
    //vertex shading
    vertexShadingIndex(
        &pGLContext->scene.curPrimitive.mvpMatrix,
        minIndex,
        maxIndex
    );


    for ( i = 0; i < count; i++)
    {
        switch (type)
        {
        case GL_UNSIGNED_BYTE:
            curIndex = (GLuint)(*((GLubyte*)indices + i));
            break;
        case GL_UNSIGNED_SHORT:
            curIndex = (GLuint)(*((GLushort*)indices + i));
            break;
        case GL_UNSIGNED_INT:
            curIndex = (GLuint)(*((GLuint*)indices + i));
            break;
        default:
            curIndex = 0;
            pushError(GL_INVALID_ENUM);
            return;
        }
        drawPrimitiveIndex(mode, curIndex - minIndex, i, curIndex, &pGLContext->scene.curPrimitive.mvpMatrix);
    }
    pGLContext->scene.curPrimitive.vertexInfo[0].numOfVertex = 0;
    
}

void vertexShadingIndex(
    GLMatrixStruct *mvpMat,
    GLuint minIndex,
    GLuint maxIndex
    )
{
    GLuint i;
    GLuint curVertexIndex;
    GLint vertexSize;
    GLVertexPosStruct vertexPos;
    GLVertexPosStruct vertexPosOut;
    GLuint indexNum;
	GLvoid* vertexPosBuffer;

    indexNum = maxIndex - minIndex + 1;

    genBufferFromPointer(
        pGLContext->vertexPointerParam.size,
        pGLContext->vertexPointerParam.type,
        pGLContext->vertexPointerParam.stride,
        pGLContext->vertexPointerParam.pointer,
        minIndex,
        maxIndex,
        (void*)pGLVSO->vertexPointer
		//&vertexPosBuffer
    );
		
	//pGLVSO->vertexPointer = (GLVertexPosStruct*)vertexPosBuffer;


    //vertex shading
    vertexSize = pGLContext->vertexPointerParam.size;
    for (i = 0; i < maxIndex - minIndex + 1; i++)
    {
        curVertexIndex = i * vertexSize;
        vertexPos.pos[0] = pGLVSO->vertexPointer[i].pos[0];
        vertexPos.pos[1] = pGLVSO->vertexPointer[i].pos[1];
        switch (vertexSize)
        {
        case 2:
            vertexPos.pos[2] = 0;
            vertexPos.pos[3] = GL_FIXED_ONE;
            break;
        case 3:
            vertexPos.pos[2] = pGLVSO->vertexPointer[i].pos[2];
            vertexPos.pos[3] = GL_FIXED_ONE;
            break;
        case 4:
            vertexPos.pos[2] = pGLVSO->vertexPointer[i].pos[2];
            vertexPos.pos[3] = pGLVSO->vertexPointer[i].pos[3];
			break;
        default:
            pushError(GL_INVALID_ENUM);
            return;
        }
        vertexShading(
            mvpMat,
            &vertexPos,
            &vertexPosOut
        );
        pGLVSO->vertexPointer[i].pos[0] = vertexPosOut.pos[0];
        pGLVSO->vertexPointer[i].pos[1] = vertexPosOut.pos[1];
        switch (vertexSize)
        {
        case 2:
            break;
        case 3:
            pGLVSO->vertexPointer[i].pos[2] = vertexPosOut.pos[2];
            break;
        case 4:
            pGLVSO->vertexPointer[i].pos[2] = vertexPosOut.pos[2];
            pGLVSO->vertexPointer[i].pos[3] = vertexPosOut.pos[3];
			break;
        default:
            pushError(GL_INVALID_ENUM);
            return;
        }
    }

    

}

void glBegin(GLenum mode)
{
    ;//TODO: temporary empty
}

void glEnd(void)
{
    ;//TODO: temporary empty
}

void glVertex3i(GLint x, GLint y, GLint z)
{
    ;//TODO: temporary empty
}
void glVertex3iv(const GLint* v)
{
    ;//TODO: temporary empty
}
void glVertex3x(GLTypeT x, GLTypeT y, GLTypeT z)
{
    ;//TODO: temporary empty
}
void glVertex3xv(const GLTypeT* v)
{
    ;//TODO: temporary empty
}

#endif