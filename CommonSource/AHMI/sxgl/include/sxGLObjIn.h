#ifndef SXGL_OBJ_IN__H
#define SXGL_OBJ_IN__H

#include "sxGL.h"
#include "sxGLObject.h"

#define MAX_OBJECT_NUM 128
#define MAX_PRIMITIVE_NUM 18


typedef struct _primitivePointerStruct {
    GLubyte primitiveType;
	GLubyte indexType;
	GLushort vertexIndexNum;
	GLushort TOIndex;
	union {
		GLuint* indexAddr;
		GLuint startIndex;
	} vertexIndex;
}primitivePointerStruct;



typedef struct _sxGLObjIn{
	GLuint numPrimitive;
	GLVertexPosStruct* vertexPointer;
	GLVertexPosStruct* normalPointer;
	primitivePointerStruct* primitives;
	textureObjectInStruct* TOPointer;
	GLMatrixStruct* TMPointer;
}sxGLObjIn;

void from_tex_to_model(GLVertexStructure vertex[4], GLTypeT tex_width, GLTypeT tex_height, GLMatrixStruct* tex_to_modelMatrix);

void readObj(char* filename);
GLTypeT double2fix(double i);

extern sxGLObjIn gsxGLObjIn;

#endif