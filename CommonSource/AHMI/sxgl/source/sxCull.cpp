#include "sxGL.h"
#include "sxGLObject.h"
#include "sxCull.h"

#ifdef SXGL_LIB

void glCullFace (GLenum mode) {
    //assert((mode==GL_FRONT) || (mode==GL_BACK));
    pGLContext->cull.cullFace = mode;
}

void glFrontFace (GLenum mode) {
    //assert((mode==GL_CW) || (mode==GL_CCW));
    pGLContext->cull.frontFace = mode;
}

void glCullCurrentPrimitive(GLPrimitiveVertexInfo* cullPrimitive) {
    if (pGLContext->state.GLEnableState.bit.GL_STATE_CULL_FACE == 0) {
        return;
    }
    if (cullPrimitive->numOfVertex < 3) {
        return;
    }
    GLVertexPosStruct* vertex = cullPrimitive->vertex;
    GLTypeT directionalArea;
    directionalArea =   vertex[0].pos[0] * vertex[1].pos[1] +
                        vertex[0].pos[1] * vertex[2].pos[0] +
                        vertex[1].pos[0] * vertex[2].pos[1] -
                        vertex[0].pos[0] * vertex[2].pos[1] -
                        vertex[0].pos[1] * vertex[1].pos[0] -
                        vertex[1].pos[1] * vertex[2].pos[0];
    //sxglDebug("Directional area: %f\n", directionalArea);
    typedef enum { CULL_FRONT=-1, CULL_DEGENERATE=0, CULL_BACK=1 } CullResult_t;
    CullResult_t face;
	GLTypeT zero;
	zero = 0;
    if (directionalArea > zero) {
        face = CULL_FRONT;
    }
    else if(directionalArea == zero) {
        face = CULL_DEGENERATE;
    }
    else {
        face = CULL_BACK;
    }
    if (pGLContext->cull.frontFace == GL_CW) {
        face = (CullResult_t)(-(int)face);
    }
    if (pGLContext->cull.cullFace == GL_BACK) {
        if (face != CULL_FRONT) {
            cullPrimitive->numOfVertex = 0;
        }
		if(pGLContext->cull.frontFace == GL_CCW) //need to reverse point
		{
			glCullRevrseFace(cullPrimitive);
		}
    }
    else {
        if (face != CULL_BACK) {
            cullPrimitive->numOfVertex = 0;
        }
		if(pGLContext->cull.frontFace == GL_CW) //need to reverse point
		{
			glCullRevrseFace(cullPrimitive);
		}
    }
}

inline void glCullRevrseFace(GLPrimitiveVertexInfo* cullPrimitive)
{
	if(cullPrimitive->numOfVertex == 3)
	{
		glCullRevrsePoint(&cullPrimitive->vertex[1], &cullPrimitive->vertex[2]);
	}
	else if(cullPrimitive->numOfVertex == 4)
	{
		glCullRevrsePoint(&cullPrimitive->vertex[1], &cullPrimitive->vertex[3]);
	}
}

inline void glCullRevrsePoint(GLVertexPosStruct* p0, GLVertexPosStruct* p1)
{
	GLubyte i;
	GLVertexPosStruct p_temp;
	for(i = 0; i < 4; i++)
	{
		p_temp.pos[i] = p0->pos[i];
		p0->pos[i] = p1->pos[i];
		p1->pos[i] = p_temp.pos[i];
	}
}

#endif