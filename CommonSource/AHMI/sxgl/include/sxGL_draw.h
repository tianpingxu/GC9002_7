#ifndef SXGL_DARW__H
#define SXGL_DRWA__H

#include "sxGL.h"
#include "sxGLObject.h"

void getDataFromPointer(GLint size, GLenum type, GLsizei stride, const GLvoid* pointer, GLuint index, GLTypeT* pointerOut);
void genBufferFromPointer(GLint size, GLenum type, GLsizei stride, const GLvoid* pointer, GLuint minIndex, GLuint maxIndex, GLvoid* pointerOut);
void setPrimitiveParam(GLint size, GLenum type, GLsizei stride, const GLvoid* pointer, drawPointerParamStruct* primitiveParam);
void drawPrimitiveIndex(GLenum mode, GLuint pointerIndex, GLuint primitiveIndex, GLuint modelIndex, GLMatrixStruct* mvpMat); //take a new primitive index and assemble
void setPrimitiveModelVertex(GLsizei index, GLubyte curVertexIndex); //set primitive's original vertex
void setPrimitiveVertex(GLsizei index, GLubyte curVertexIndex); //set primitive's vertex with index
void setPrimitiveNormal(GLsizei index); //set primitive's normal
void setPrimitiveTextureMatrix(GLsizei index); //set primitive's texture matrix
void setPrimitiveTextureObject(GLsizei index); //set primitive's texture information
void setPrimitiveMVPMatrix(GLsizei index, const GLMatrixStruct* mvp); //set primitive's MVP matrix
void drawCurrentPrimitive();
void generateMVPMat(GLMatrixStruct* mvpMat);
void vertexShadingIndex(
    GLMatrixStruct* mvpMat,
    GLuint minIndex,
    GLuint maxIndex
);


#endif