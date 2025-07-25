#ifndef SXGL_PIPE__H
#define SXGL_PIPE__H

#include "sxGL.h"
#include "sxGLObject.h"


void m4tom3(GLMatrixStruct *m4, GLMatrix3x3Struct* m3);
void position_mul_m4(GLMatrixStruct* m4, GLVertexPosStruct* position_in, GLVertexPosStruct* position_out);
void vector_mul_m4(GLMatrixStruct* m4, GLVectorStruct* position_in, GLVectorStruct* position_out);
void vertexShading(GLMatrixStruct* mvpMat, GLVertexPosStruct* position_in, GLVertexPosStruct* position_out);

#endif