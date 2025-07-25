#include "sxGL.h"
#include "sxGLObject.h"
#include "sxGLState.h"

#ifdef SXGL_LIB


void pushError(GLenum error)
{
    if (pGLContext->errorFifo.full == 1)
    {
        return;
    }
    
    pGLContext->errorFifo.error[pGLContext->errorFifo.wp] = error;
    if (pGLContext->errorFifo.wp > NUM_OF_ERROR_FIFO)
    {
        pGLContext->errorFifo.wp = 0;
    }
    else
    {
        pGLContext->errorFifo.wp++;
    }
    if (pGLContext->errorFifo.wp == pGLContext->errorFifo.rp)
        pGLContext->errorFifo.full = 1;
    pGLContext->errorFifo.empty = 0;
}

GLenum glGetError(void)
{
    GLenum error;
    if (pGLContext->errorFifo.empty == 1)
    {
        return GL_NO_ERROR;
    }
    else
    {
        error = pGLContext->errorFifo.error[pGLContext->errorFifo.rp];
        if (pGLContext->errorFifo.rp > NUM_OF_ERROR_FIFO)
            pGLContext->errorFifo.rp = 0;
        else
            pGLContext->errorFifo.rp++;
        if (pGLContext->errorFifo.wp == pGLContext->errorFifo.rp)
            pGLContext->errorFifo.empty = 1;
        pGLContext->errorFifo.full = 0;
        return error;
    }
}


void glEnable(GLenum cap)
{
    switch (cap)
    {
    case GL_CULL_FACE:
        pGLContext->state.GLEnableState.bit.GL_STATE_CULL_FACE = 1;
        break;
    case GL_DITHER:
        pGLContext->state.GLEnableState.bit.GL_STATE_DITHER = 1;
        break;
    case GL_LIGHT0:
        pGLContext->state.GLEnableState.bit.GL_STATE_LIGHT0 = 1;
        break;
    case GL_LIGHTING:
        pGLContext->state.GLEnableState.bit.GL_STATE_LIGHTING = 1;
        break;
    case GL_NORMALIZE:
        pGLContext->state.GLEnableState.bit.GL_STATE_NORMALIZE = 1;
        break;
    case GL_RESCALE_NORMAL:
        pGLContext->state.GLEnableState.bit.GL_STATE_RESCALE_NORMAL = 1;
        break;
    case GL_SCISSOR_TEST:
        pGLContext->state.GLEnableState.bit.GL_STATE_SCISSOR_TEST = 1;
        break;
    case GL_TEXTURE_2D:
        pGLContext->state.GLEnableState.bit.GL_STATE_TEXTURE_2D = 1;
        break;
	case GL_CLIP_PLANE_FAR:
		pGLContext->state.GLEnableState.bit.GL_STATE_CLIP_FAR_PLANE = 1;
		break;
	case GL_CLIP_PLANE_LRTB:
		pGLContext->state.GLEnableState.bit.GL_STATE_CLIP_LRTB_PLANE = 1;
		break;
    default:
        pushError(GL_INVALID_ENUM);
        break;
    }
}

void glDisable(GLenum cap)
{
    switch (cap)
    {
    case GL_CULL_FACE:
        pGLContext->state.GLEnableState.bit.GL_STATE_CULL_FACE = 0;
        break;
    case GL_DITHER:
        pGLContext->state.GLEnableState.bit.GL_STATE_DITHER = 0;
        break;
    case GL_LIGHT0:
        pGLContext->state.GLEnableState.bit.GL_STATE_LIGHT0 = 0;
        break;
    case GL_LIGHTING:
        pGLContext->state.GLEnableState.bit.GL_STATE_LIGHTING = 0;
        break;
    case GL_NORMALIZE:
        pGLContext->state.GLEnableState.bit.GL_STATE_NORMALIZE = 0;
        break;
    case GL_RESCALE_NORMAL:
        pGLContext->state.GLEnableState.bit.GL_STATE_RESCALE_NORMAL = 0;
        break;
    case GL_SCISSOR_TEST:
        pGLContext->state.GLEnableState.bit.GL_STATE_SCISSOR_TEST = 0;
        break;
    case GL_TEXTURE_2D:
        pGLContext->state.GLEnableState.bit.GL_STATE_TEXTURE_2D = 0;
        break;
    case GL_CLIP_PLANE_FAR:
        pGLContext->state.GLEnableState.bit.GL_STATE_CLIP_FAR_PLANE = 0;
        break;
    case GL_CLIP_PLANE_LRTB:
        pGLContext->state.GLEnableState.bit.GL_STATE_CLIP_LRTB_PLANE = 0;
        break;
    default:
        pushError(GL_INVALID_ENUM);
        break;
    }
}

void glEnableClientState(GLenum array)
{
    switch (array)
    {
    case GL_VERTEX_ARRAY:
        pGLContext->state.GLEnableClientState.bit.GL_STATE_TEXTURE_COORD_ARRAY = 1;
        break;
    case GL_COLOR_ARRAY:
        pGLContext->state.GLEnableClientState.bit.GL_STATE_COLOR_ARRAY = 1;
        break;
    case GL_TEXTURE_COORD_ARRAY:
        pGLContext->state.GLEnableClientState.bit.GL_STATE_TEXTURE_COORD_ARRAY = 1;
        break;
    case GL_NORMAL_ARRAY:
        pGLContext->state.GLEnableClientState.bit.GL_STATE_NORMAL_ARRAY = 1;
        break;
    case GL_POINT_SIZE_ARRAY_OES:
        pGLContext->state.GLEnableClientState.bit.GL_STATE_POINT_SIZE_ARRAY_OES = 1;
        break;
    default:
        pushError(GL_INVALID_ENUM);
        break;
    }
    
}
void glDisableClientState(GLenum array)
{
    switch (array)
    {
    case GL_VERTEX_ARRAY:
        pGLContext->state.GLEnableClientState.bit.GL_STATE_TEXTURE_COORD_ARRAY = 0;
        break;
    case GL_COLOR_ARRAY:
        pGLContext->state.GLEnableClientState.bit.GL_STATE_COLOR_ARRAY = 0;
        break;
    case GL_TEXTURE_COORD_ARRAY:
        pGLContext->state.GLEnableClientState.bit.GL_STATE_TEXTURE_COORD_ARRAY = 0;
        break;
    case GL_NORMAL_ARRAY:
        pGLContext->state.GLEnableClientState.bit.GL_STATE_NORMAL_ARRAY = 0;
        break;
    case GL_POINT_SIZE_ARRAY_OES:
        pGLContext->state.GLEnableClientState.bit.GL_STATE_POINT_SIZE_ARRAY_OES = 0;
        break;
    default:
        pushError(GL_INVALID_ENUM);
        break;
    }
}

void glClientActiveTexture(GLenum texture)
{
    pGLContext->state.GLClientTextureState = texture;
}



void glPointSizex(GLTypeT size)
{
    pGLContext->pointParam.size = size;
}
void glPointParameter(GLenum pname, GLTypeT param)
{
    switch (pname)
    {
    case GL_POINT_SIZE_MIN:
        pGLContext->pointParam.sizeMin = param;
        break;
    case GL_POINT_SIZE_MAX:
        pGLContext->pointParam.sizeMax = param;
        break;
    case GL_POINT_DISTANCE_ATTENUATION:
        pGLContext->pointParam.distanceAttenuation = param;
    case GL_POINT_FADE_THRESHOLD_SIZE:
        pGLContext->pointParam.fadeThresholdSize = param;
    default:
        pushError(GL_INVALID_ENUM);
        break;
    }
}
void glLineWidthx(GLTypeT width)
{
    pGLContext->lineWidth = width;
}

void glFlush(void)
{
    ; //TODO: do nothing now
}

void glFinish(void)
{
    ; //TODO: do nothing now
}

#endif