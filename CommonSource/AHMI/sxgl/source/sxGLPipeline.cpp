#include "sxGL.h"
#include "sxGLObject.h"
#include "sxMatrix.h"

#ifdef SXGL_LIB

void m4tom3(GLMatrixStruct *m4, GLMatrix3x3Struct* m3)
{
	GLTypeT a, b, c, d, e, f, g, h, i;
	GLTypeT matrix_3x3i;
	GLubyte j;
	a = m4->matrixA[0];
	b = m4->matrixA[1];
	c = m4->matrixA[3];
	d = m4->matrixA[4];
	e = m4->matrixA[5];
	f = m4->matrixA[7];
	g = m4->matrixG[0];
	h = m4->matrixG[1];
	i = m4->matrixI;
	unsigned char overflow_a[6];
	unsigned char overflow_g[2];

	m3->matrixA[0] = i * e - h* f;
	m3->matrixA[1] = c * h - i* b;
	m3->matrixA[2] = b * f - e* c;
	m3->matrixA[3] = f * g - i* d;
	m3->matrixA[4] = a * i - c* g;
	m3->matrixA[5] = c * d - a* f;
	m3->matrixG[0] = h * d - g* e;
	m3->matrixG[1] = g * b - h* a;
	matrix_3x3i    = a * e - d* b;
	//normalize
	for( j = 0; j < 6; j++)
	{
		m3->matrixA[j] = m3->matrixA[j] /  matrix_3x3i;
		if(m3->matrixA[j] > ( (1<<11) - 1) || (m3->matrixA[j] < 1-(1<<11)))
			overflow_a[j] = 1;
		else
			overflow_a[j] = 0;
	}
	for(j = 0; j < 2; j++)
	{
		m3->matrixG[j] = m3->matrixG[j] / matrix_3x3i;
		if(m3->matrixG[j] > ( (1<<18) - 1) || (m3->matrixG[j] < 1-(1<<18)))
			overflow_g[j] = 1;
		else
			overflow_g[j] = 0;
	}
	while(overflow_a[0] | overflow_a[1] | overflow_a[2] | overflow_a[3] | overflow_a[4] | overflow_a[5] | overflow_g[0] | overflow_g[1])
	{
        for( j = 0; j < 6; j++)
	    {
	    	m3->matrixA[j] = m3->matrixA[j] /  2;
	    	if(m3->matrixA[j] > ( (1<<11) - 1) || (m3->matrixA[j] < 1-(1<<11)))
	    		overflow_a[j] = 1;
	    	else
	    		overflow_a[j] = 0;
	    }
	    for(j = 0; j < 2; j++)
	    {
	    	m3->matrixG[j] = m3->matrixG[j] / 2;
	    	if(m3->matrixG[j] > ( (1<<18) - 1) || (m3->matrixG[j] < 1-(1<<18)))
	    		overflow_g[j] = 1;
	    	else
	    		overflow_g[j] = 0;
	    }
	}
}


void position_mul_m4(GLMatrixStruct* m4, GLVertexPosStruct* position_in, GLVertexPosStruct* position_out)
{
	position_out->pos[0] = 
		position_in->pos[0] * m4->matrixA[0] + 
		position_in->pos[1] * m4->matrixA[4] +
		position_in->pos[2] * m4->matrixA[8] + 
		position_in->pos[3] * m4->matrixG[0];
	position_out->pos[1] = 
		position_in->pos[0] * m4->matrixA[1] + 
		position_in->pos[1] * m4->matrixA[5] +
		position_in->pos[2] * m4->matrixA[9] + 
		position_in->pos[3] * m4->matrixG[1];
	position_out->pos[2] = 
		position_in->pos[0] * m4->matrixA[2] + 
		position_in->pos[1] * m4->matrixA[6] +
		position_in->pos[2] * m4->matrixA[10] + 
		position_in->pos[3] * m4->matrixG[2];
	position_out->pos[3] = 
		position_in->pos[0] * m4->matrixA[3] + 
		position_in->pos[1] * m4->matrixA[7] +
		position_in->pos[2] * m4->matrixA[11] + 
		position_in->pos[3] * m4->matrixI;
}

void vector_mul_m4(GLMatrixStruct* m4, GLVectorStruct* position_in, GLVectorStruct* position_out)
{
	position_out->pos[0] = 
		position_in->pos[0] * m4->matrixA[0] + 
		position_in->pos[1] * m4->matrixA[4] +
		position_in->pos[2] * m4->matrixA[8] + 
		m4->matrixG[0];
	position_out->pos[1] = 
		position_in->pos[0] * m4->matrixA[1] + 
		position_in->pos[1] * m4->matrixA[5] +
		position_in->pos[2] * m4->matrixA[9] + 
		m4->matrixG[1];
	position_out->pos[2] = 
		position_in->pos[0] * m4->matrixA[2] + 
		position_in->pos[1] * m4->matrixA[6] +
		position_in->pos[2] * m4->matrixA[10] + 
		m4->matrixG[2];
}

void vertexShading(GLMatrixStruct* mvpMat, GLVertexPosStruct* position_in, GLVertexPosStruct* position_out)
{
	position_mul_m4(mvpMat, position_in, position_out);
}


#endif
