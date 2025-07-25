#ifndef SXGl_SOURCE_BUFFER__H
#define SXGl_SOURCE_BUFFER__H

#define AHMI_TILE_SIZE 32

#include "sxGLObject.h"

void writeSourceBuffer(GLVertexPosStruct vertexPos[4], GLMatrix3x3Struct* mat, textureObjectInStruct *textureObject, GLubyte lightEn, GLubyte lightAlpha, GLColorStruct diffuse);
void writeLightSourceBuffer(GLVertexPosStruct vertexPos[4], GLMatrix3x3Struct* mat, textureObjectInStruct *textureObject, GLubyte lightEn, GLubyte lightAlpha, GLColorStruct diffuse);
void writeLightPureColor(GLVertexPosStruct vertexPos[4], GLMatrix3x3Struct* mat, textureObjectInStruct *textureObject, GLubyte lightEn, GLubyte lightAlpha, GLColorStruct diffuse);
void writeLightPureColor(GLVertexPosStruct vertexPos[4], GLMatrix3x3Struct* mat, textureObjectInStruct *textureObject, GLubyte lightEn, GLubyte lightAlpha, GLColorStruct diffuse);

#endif