#ifndef __SXCULL_H_
#define __SXCULL_H_

#include "sxGL.h"
#include "sxGLObject.h"

void glCullCurrentPrimitive(GLPrimitiveVertexInfo* cullPrimitive);
inline void glCullRevrseFace(GLPrimitiveVertexInfo* cullPrimitive);
inline void glCullRevrsePoint(GLVertexPosStruct* p0, GLVertexPosStruct* p1);

#endif // __SXCULL_H_