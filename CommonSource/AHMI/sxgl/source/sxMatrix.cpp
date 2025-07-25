#include "sxGL.h"
#include "sxGLObject.h"
#include "sxMatrix.h"
#include "sxGLState.h"
#include "sxGLMath.h"

#ifdef SXGL_LIB

//GLContextStruct GLContext;

// vertex mul  matrix
// (A[0] A[4] A[8]  G[0])   (x)
// (A[1] A[5] A[9]  G[1]) * (y) 
// (A[2] A[6] A[10] G[2])   (z)
// (A[3] A[7] A[11] I   )   (w)


//right hand coordinate system
//x is right, y is top and z is front

//matrix fifo
GLMatrixFifo mGLMatrixFifo;

void matrixFifoInit()
{
	mGLMatrixFifo.full = 0;
	mGLMatrixFifo.empty = 0;
	mGLMatrixFifo.wp = 0;
	mGLMatrixFifo.rp = 0;
}

void glPushMatrix(void)
{
	int i;
	if (mGLMatrixFifo.full)
	{
		pushError(GL_STACK_OVERFLOW);
		return;
	}
	else
	{
		mGLMatrixFifo.data[mGLMatrixFifo.wp] = pGLContext->scene.matrix.pCurrentMatrix[i];
		mGLMatrixFifo.wp = (mGLMatrixFifo.wp + 1) % NUM_OF_SX_MATRIX_FIFO ;
		if (mGLMatrixFifo.wp == mGLMatrixFifo.rp)
		{
			mGLMatrixFifo.full = 1;
		}
	}
}
void glPopMatrix(void)//TODO
{
	int i;
	if (mGLMatrixFifo.empty)
	{
		pushError(GL_STACK_UNDERFLOW);
		return;
	}
	else
	{
		pGLContext->scene.matrix.pCurrentMatrix[i] = mGLMatrixFifo.data[mGLMatrixFifo.rp] ;
		mGLMatrixFifo.rp = (mGLMatrixFifo.rp + 1) % NUM_OF_SX_MATRIX_FIFO;
		if (mGLMatrixFifo.wp == mGLMatrixFifo.rp)
		{
			mGLMatrixFifo.empty = 1;
		}
	}
}



void glLoadMatrixx (const GLTypeT *m)
{
	glSetMatrix(pGLContext->scene.matrix.pCurrentMatrix, m);
}

void glSetMatrix (GLMatrixStruct* m_out, const GLTypeT *m)
{
	int i;
    for(i=0;i<4*3;i++)
	{
    	m_out->matrixA[i] = m[i];	
    } 

	for(i=0;i<3*1;i++)
	{
    	m_out->matrixG[i] = m[i + 12];	
    }  

	m_out->matrixI = m[15];
}

void glSetMatrixStruct (GLMatrixStruct* m_out, const GLMatrixStruct *m)
{
	int i;
    for(i=0;i<4*3;i++)
	{
    	m_out->matrixA[i] = m->matrixA[i];	
    } 

	for(i=0;i<3*1;i++)
	{
    	m_out->matrixG[i] = m->matrixG[i];	
    }  

	m_out->matrixI = m->matrixI;
}

void glSetMatrixStructFrom3x3 (GLMatrixStruct* m_out, const GLMatrix3x3InStruct *m)
{
	int i;
    m_out->matrixA[0] = m->matrixA[0];
	m_out->matrixA[1] = m->matrixA[1];
	m_out->matrixA[2] = 0;
	m_out->matrixA[3] = m->matrixA[2];

	m_out->matrixA[4] = m->matrixA[3];
	m_out->matrixA[5] = m->matrixA[4];
	m_out->matrixA[6] = 0;
	m_out->matrixA[7] = m->matrixA[5];

	m_out->matrixA[8] = 0;
	m_out->matrixA[9] = 0;
	m_out->matrixA[10] = GL_TYPET_ONE;
	m_out->matrixA[11] = 0;


	for(i=0;i<2*1;i++)
	{
    	m_out->matrixG[i] = m->matrixG[i];	
    }  
	m_out->matrixG[2] = 0;

	m_out->matrixI = GL_TYPET_ONE;
}

void glLoadIdentity (void)
{
    pGLContext->scene.matrix.pCurrentMatrix->matrixA[0]  = GL_TYPET_ONE;
	pGLContext->scene.matrix.pCurrentMatrix->matrixA[1]  = 0x0;
	pGLContext->scene.matrix.pCurrentMatrix->matrixA[2]  = 0x0;
	pGLContext->scene.matrix.pCurrentMatrix->matrixA[3]  = 0x0;
	pGLContext->scene.matrix.pCurrentMatrix->matrixA[4]  = 0x0;
	pGLContext->scene.matrix.pCurrentMatrix->matrixA[5]  = GL_TYPET_ONE;
	pGLContext->scene.matrix.pCurrentMatrix->matrixA[6]  = 0x0;
	pGLContext->scene.matrix.pCurrentMatrix->matrixA[7]  = 0x0;
	pGLContext->scene.matrix.pCurrentMatrix->matrixA[8]  = 0x0;
	pGLContext->scene.matrix.pCurrentMatrix->matrixA[9]  = 0x0;
	pGLContext->scene.matrix.pCurrentMatrix->matrixA[10] = GL_TYPET_ONE;
	pGLContext->scene.matrix.pCurrentMatrix->matrixA[11] = 0x0;
	pGLContext->scene.matrix.pCurrentMatrix->matrixG[0]  = 0x0;
	pGLContext->scene.matrix.pCurrentMatrix->matrixG[1]  = 0x0;
	pGLContext->scene.matrix.pCurrentMatrix->matrixG[2]  = 0x0;
	pGLContext->scene.matrix.pCurrentMatrix->matrixI     = GL_TYPET_ONE;
	if(pGLContext->scene.matrix.pCurrentMatrix == &pGLContext->scene.matrix.modelviewMatrix)
	{
		pGLContext->scene.curPrimitive.rotateMatrix.matrixA[0]  = GL_TYPET_ONE;
		pGLContext->scene.curPrimitive.rotateMatrix.matrixA[1]  = 0x0;
		pGLContext->scene.curPrimitive.rotateMatrix.matrixA[2]  = 0x0;
		pGLContext->scene.curPrimitive.rotateMatrix.matrixA[3]  = 0x0;
		pGLContext->scene.curPrimitive.rotateMatrix.matrixA[4]  = 0x0;
		pGLContext->scene.curPrimitive.rotateMatrix.matrixA[5]  = GL_TYPET_ONE;
		pGLContext->scene.curPrimitive.rotateMatrix.matrixA[6]  = 0x0;
		pGLContext->scene.curPrimitive.rotateMatrix.matrixA[7]  = 0x0;
		pGLContext->scene.curPrimitive.rotateMatrix.matrixA[8]  = 0x0;
		pGLContext->scene.curPrimitive.rotateMatrix.matrixA[9]  = 0x0;
		pGLContext->scene.curPrimitive.rotateMatrix.matrixA[10] = GL_TYPET_ONE;
		pGLContext->scene.curPrimitive.rotateMatrix.matrixA[11] = 0x0;
		pGLContext->scene.curPrimitive.rotateMatrix.matrixG[0]  = 0x0;
		pGLContext->scene.curPrimitive.rotateMatrix.matrixG[1]  = 0x0;
		pGLContext->scene.curPrimitive.rotateMatrix.matrixG[2]  = 0x0;
		pGLContext->scene.curPrimitive.rotateMatrix.matrixI     = GL_TYPET_ONE;
	}
}

void glViewport (GLint x, GLint y, GLsizei width, GLsizei height)
{
	pGLContext->viewportBox.x      = x;
	pGLContext->viewportBox.y      = y;
	pGLContext->viewportBox.width  = width;
	pGLContext->viewportBox.height = height;
}

void glMultMatrixx(const GLTypeT *m)
{
	GLMatrixStruct m0, m1;
	glSetMatrix(&m0, m);
	glSetMatrixStruct(&m1, pGLContext->scene.matrix.pCurrentMatrix);
	glMatrixLeftMul(&m0, &m1, pGLContext->scene.matrix.pCurrentMatrix);
}

void glRotatex (GLTypeT angle, GLTypeT x, GLTypeT y, GLTypeT z)
{
    GLTypeT s;
	GLTypeT c;
	GLTypeT c_minus_1;
	GLTypeT x2,xy,xz,y2,yz,z2,xs,ys,zs;
	GLTypeT rotateMatrix[16];
	GLTypeT xyc_minus1,xzc_minus1,yzc_minus1;
	GLMatrixStruct m0, m1;
	GLTypeT one;
	s = glSin(angle);
	c = glCos(angle);
	one = GL_TYPET_ONE;
	c_minus_1 = one - c;
	x2 = (x*x);
	xy = (x*y);
	xz = (x*z);
	y2 = (y*y);
	yz = (y*z);
	z2 = (z*z);
	xs = (x*s);
	ys = (y*s);
	zs = (z*s);
	xyc_minus1=(xy*c_minus_1);
	xzc_minus1=(xz*c_minus_1);
	yzc_minus1=(yz*c_minus_1);
	
	rotateMatrix[0]  = (x2*c_minus_1)+c;
	rotateMatrix[1]  = xyc_minus1+zs;
	rotateMatrix[2]  = xzc_minus1-ys;
	rotateMatrix[3]  = 0x0;
	rotateMatrix[4]  = xyc_minus1-zs ;
	rotateMatrix[5]  = y2*c_minus_1+c;
	rotateMatrix[6]  = yzc_minus1+xs ;
	rotateMatrix[7]  = 0x0;
	rotateMatrix[8]  = xzc_minus1+ys  ;
	rotateMatrix[9]  = yzc_minus1-xs  ;
	rotateMatrix[10] = z2*c_minus_1+c ;
	rotateMatrix[11] = 0x0;
	rotateMatrix[12] = 0x0;
	rotateMatrix[13] = 0x0;
	rotateMatrix[14] = 0x0;
	rotateMatrix[15] = one;

	glSetMatrix(&m0, rotateMatrix);
	glSetMatrixStruct(&m1, pGLContext->scene.matrix.pCurrentMatrix);
	glMatrixLeftMul(&m0, &m1, pGLContext->scene.matrix.pCurrentMatrix);

	if(pGLContext->scene.matrix.pCurrentMatrix == &pGLContext->scene.matrix.modelviewMatrix)
	{
		glSetMatrixStruct(&m1, &pGLContext->scene.curPrimitive.rotateMatrix);
		glMatrixLeftMul(&m0, &m1, &pGLContext->scene.curPrimitive.rotateMatrix);
	}


	
}
void glScalex (GLTypeT x, GLTypeT y, GLTypeT z)
{
	int i, j;
	GLTypeT scale[3];
	scale[0] = x;
	scale[1] = y;
	scale[2] = z;
	//pGLContext->scene.matrix.pCurrentMatrix[0]  = fix_mul_A(pGLContext->scene.matrix.pCurrentMatrix[0]  , x); 
	//pGLContext->scene.matrix.pCurrentMatrix[1]  = fix_mul_A(pGLContext->scene.matrix.pCurrentMatrix[1]  , y);
	//pGLContext->scene.matrix.pCurrentMatrix[2]  = fix_mul_A(pGLContext->scene.matrix.pCurrentMatrix[2]  , z); 
	//pGLContext->scene.matrix.pCurrentMatrix[4]  = fix_mul_A(pGLContext->scene.matrix.pCurrentMatrix[4]  , x);
	//pGLContext->scene.matrix.pCurrentMatrix[5]  = fix_mul_A(pGLContext->scene.matrix.pCurrentMatrix[5]  , y);
	//pGLContext->scene.matrix.pCurrentMatrix[6]  = fix_mul_A(pGLContext->scene.matrix.pCurrentMatrix[6]  , z);
	//pGLContext->scene.matrix.pCurrentMatrix[8]  = fix_mul_A(pGLContext->scene.matrix.pCurrentMatrix[8]  , x);
	//pGLContext->scene.matrix.pCurrentMatrix[9]  = fix_mul_A(pGLContext->scene.matrix.pCurrentMatrix[9]  , y);
	//pGLContext->scene.matrix.pCurrentMatrix[10] = fix_mul_A(pGLContext->scene.matrix.pCurrentMatrix[10] , z);
	//pGLContext->scene.matrix.pCurrentMatrix[12] = fix_mul_A(pGLContext->scene.matrix.pCurrentMatrix[12] , x);
	//pGLContext->scene.matrix.pCurrentMatrix[13] = fix_mul_A(pGLContext->scene.matrix.pCurrentMatrix[13] , y);
	//pGLContext->scene.matrix.pCurrentMatrix[14] = fix_mul_A(pGLContext->scene.matrix.pCurrentMatrix[14] , z);
	for(i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			pGLContext->scene.matrix.pCurrentMatrix->matrixA[i * 4 + j] = pGLContext->scene.matrix.pCurrentMatrix->matrixA[i * 4 + j] * scale[i];
		}
		pGLContext->scene.matrix.pCurrentMatrix->matrixG[i] = pGLContext->scene.matrix.pCurrentMatrix->matrixG[i] * scale[i];
	}
}

void glTranslatex (GLTypeT x, GLTypeT y, GLTypeT z)
{
	 GLubyte i;
	 GLTypeT translate[3];
	 translate[0] = x;
	 translate[1] = y;
	 translate[2] = z;
	for (i = 0; i < 3; i++)
	{
		pGLContext->scene.matrix.pCurrentMatrix->matrixG[i] = pGLContext->scene.matrix.pCurrentMatrix->matrixG[i] + translate[i];
	}
}

void glOrthox(GLTypeT left, GLTypeT right, GLTypeT buttom, GLTypeT top, GLTypeT zNear, GLTypeT zFar)
{
	GLMatrixStruct m0, m1;
	GLTypeT one;
	GLTypeT two;
	one = GL_TYPET_ONE;
	two = one + one;
	m0.matrixA[0] = two / (left - right);
	m0.matrixA[1] = 0;
	m0.matrixA[2] = 0;
	m0.matrixA[3] = 0;

	m0.matrixA[4] = 0;
	m0.matrixA[5] = two / (top - buttom);
	m0.matrixA[6] = 0;
	m0.matrixA[7] = 0;

	m0.matrixA[8] = 0;
	m0.matrixA[9] = 0;
	m0.matrixA[10] = two / (zNear - zFar);
	m0.matrixA[11] = 0;

	m0.matrixG[0] = -(right + left) / two;
	m0.matrixG[1] = -(top + buttom) / two;
	m0.matrixG[2] = -(zNear + zFar) / two;
	m0.matrixI = one;

	glSetMatrixStruct(&m1, pGLContext->scene.matrix.pCurrentMatrix);

	glMatrixLeftMul(&m0, &m1, pGLContext->scene.matrix.pCurrentMatrix);

}
void glFrustumx(GLTypeT left, GLTypeT right, GLTypeT buttom, GLTypeT top, GLTypeT zNear, GLTypeT zFar)
{
	GLMatrixStruct m0, m1;
	GLTypeT one, two;
	one = GL_TYPET_ONE;
	two = one + one;
	m0.matrixA[0] = two * zNear/ (right - left);
	m0.matrixA[1] = 0;
	m0.matrixA[2] = 0;
	m0.matrixA[3] = 0;

	m0.matrixA[4] = 0;
	m0.matrixA[5] = two * zNear / (top - buttom);
	m0.matrixA[6] = 0;
	m0.matrixA[7] = 0;

	m0.matrixA[8] = -(right + left) / (right - left);
	m0.matrixA[9] = -(top + buttom) / (top - buttom);
	m0.matrixA[10] = (zNear + zFar) / (zFar - zNear );
	m0.matrixA[11] = one;

	m0.matrixG[0] = 0;
	m0.matrixG[1] = 0;
	m0.matrixG[2] = -two * zFar * zNear / (zFar - zNear);
	m0.matrixI = 0;

	glSetMatrixStruct(&m1, pGLContext->scene.matrix.pCurrentMatrix);

	glMatrixLeftMul(&m0, &m1, pGLContext->scene.matrix.pCurrentMatrix);
}

void glSetIndetityMatrix(GLMatrixStruct* m)
{
	m->matrixA[0] = GL_TYPET_ONE;
	m->matrixA[1] = 0;
	m->matrixA[2] = 0;
	m->matrixA[3] = 0;
	m->matrixA[4] = 0;
	m->matrixA[5] = GL_TYPET_ONE;
	m->matrixA[6] = 0;
	m->matrixA[7] = 0;
	m->matrixA[8] = 0;
	m->matrixA[9] = 0;
	m->matrixA[10] = GL_TYPET_ONE;
	m->matrixA[11] = 0;
	m->matrixG[0]  = 0;
	m->matrixG[1]  = 0;
	m->matrixG[2]  = 0;
	m->matrixI  = GL_TYPET_ONE;

}

void glGenViewportMatrix(GLMatrixStruct* m)
{
	m->matrixA[0] = (float)( pGLContext->viewportBox.width * 1.0 / 2);
	m->matrixA[1] = 0;
	m->matrixA[2] = 0;
	m->matrixA[3] = 0;
	m->matrixA[4] = 0;
	m->matrixA[5] = (float)( pGLContext->viewportBox.height * 1.0 / 2);
	m->matrixA[6] = 0;
	m->matrixA[7] = 0;
	m->matrixA[8] = 0;
	m->matrixA[9] = 0;
	m->matrixA[10] = GL_TYPET_ONE;
	m->matrixA[11] = 0;
	m->matrixG[0]  = (float)( (pGLContext->viewportBox.width * 1.0 + pGLContext->viewportBox.x * 2.0 ) / 2);
	m->matrixG[1]  = (float)( (pGLContext->viewportBox.height * 1.0 + pGLContext->viewportBox.y * 2.0) / 2);
	m->matrixG[2]  = 0;
	m->matrixI = GL_TYPET_ONE;
}

void glMatrixLeftMul(GLMatrixStruct* m0, GLMatrixStruct* m1, GLMatrixStruct* m_out) //m_out = m0 * m1
{
	int i;
	int j;
	//3x3
	for(i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
		{
			m_out->matrixA[j*4+i] =
				m0->matrixA[i + 0 ] * m1->matrixA[j * 4 + 0] +
				m0->matrixA[i + 4 ] * m1->matrixA[j * 4 + 1] +
				m0->matrixA[i + 8 ] * m1->matrixA[j * 4 + 2] +
				m0->matrixG[i     ] * m1->matrixA[j * 4 + 3];
		}
	
	//3x1
	for (i = 0; i < 3; i++)
	{
		j = 3;
		m_out->matrixG[i] =
			m0->matrixA[i + 0 ] * m1->matrixG[0] +
			m0->matrixA[i + 4 ] * m1->matrixG[1] +
			m0->matrixA[i + 8 ] * m1->matrixG[2] +
			m0->matrixG[i     ] * m1->matrixI;
	}

	//1x3
	i = 3;
	for (j = 0; j < 3; j++)
	{
		m_out->matrixA[j * 4 + i] =
			m0->matrixA[i + 0 ] * m1->matrixA[j * 4 + 0] +
			m0->matrixA[i + 4 ] * m1->matrixA[j * 4 + 1] +
			m0->matrixA[i + 8 ] * m1->matrixA[j * 4 + 2] +
			m0->matrixI * m1->matrixA[j * 4 + 3];
	}

	//1x1
	i = 3;
	j = 3;
	m_out->matrixI =
		m0->matrixA[i + 0 ] * m1->matrixG[0] +
		m0->matrixA[i + 4 ] * m1->matrixG[1] +
		m0->matrixA[i + 8 ] * m1->matrixG[2] +
		m0->matrixI * m1->matrixI;
}

void glMatrixMode(GLenum mode)
{
	switch(mode)
	{
	case GL_MODELVIEW_MATRIX:
		pGLContext->scene.matrix.pCurrentMatrix = &pGLContext->scene.matrix.modelviewMatrix;
		break;
	case GL_PROJECTION_MATRIX:
		pGLContext->scene.matrix.pCurrentMatrix = &pGLContext->scene.matrix.projectionMatrix;
		break;
	case GL_TEXTURE_MATRIX:
		pGLContext->scene.matrix.pCurrentMatrix = &pGLContext->scene.matrix.textureMatrix;
		break;
	default:
		pushError(GL_INVALID_ENUM);
		return;
	}
}

#endif
