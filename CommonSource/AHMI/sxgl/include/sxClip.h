#ifndef __SXCLIP_H_
#define __SXCLIP_H_

#include "sxGL.h"
#include "sxGLObject.h"

void glClipCurrentPrimitive(GLPrimitiveVertexInfo* clippedPrimitives);
void glInterpolition(GLTypeT sign, GLVertexPosStruct * A, GLVertexPosStruct * B, GLVertexPosStruct * gen);

#endif // __SXCLIP_H_