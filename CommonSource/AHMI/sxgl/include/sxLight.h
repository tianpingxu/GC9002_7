

#ifndef SX_LIGHT__H
#define SX_LIGHT__H

#include "sxGL.h"
#include "sxGLObJect.h"
#include "sxGLOperator.h"

GLubyte dirctional_light_for_primitive(GLVectorStruct* primitive_normal,GLMatrixStruct* primitiveMatrix,GLVectorStruct* light_dir);

#endif