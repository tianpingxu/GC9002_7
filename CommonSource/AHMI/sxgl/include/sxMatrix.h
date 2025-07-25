#ifndef SX_MATRIX__H
#define SX_MATRIX__H

#include "sxGL.h"

#define NUM_OF_SX_MATRIX_FIFO 16

typedef struct _GLMatrixFifo {
    GLMatrixStruct data[NUM_OF_SX_MATRIX_FIFO];
    GLubyte wp, rp;
    GLubyte full, empty;
}GLMatrixFifo;

void matrixFifoInit();
void glSetIndetityMatrix(GLMatrixStruct* m);
void glGenViewportMatrix(GLMatrixStruct* m);
void glMatrixLeftMul(GLMatrixStruct* m0, GLMatrixStruct* m1, GLMatrixStruct* m_out); //m_out = m0 * m1
void glSetMatrix (GLMatrixStruct* m_out, const GLTypeT *m);
void glSetMatrixStruct (GLMatrixStruct* m_out, const GLMatrixStruct *m);
void glSetMatrixStructFrom3x3 (GLMatrixStruct* m_out, const GLMatrix3x3InStruct *m);


#endif