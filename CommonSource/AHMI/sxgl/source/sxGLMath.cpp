#include "AHMICfgDefinition.h"
#include "sxGLMath.h"
#include "sxGLOperator.h"
#include "myMathClass.h"

#ifdef SXGL_LIB

//GLTypeT fix_mul_A(GLTypeT a, GLTypeT b)
//{
//	GLTypeT value;
//	value = ( (long long)(a) * (long long)(b) ) >> GL_MATRIX_A_FIXED_POINT;
//	return value;
//}
//
//GLTypeT fix_mul_G(GLTypeT a, GLTypeT b)
//{
//	GLTypeT value;
//	value = ((long long)(a) * (long long)(b)) >> GL_MATRIX_G_FIXED_POINT;
//	return value;
//}
//
//GLTypeT fix_mul_GGA(GLTypeT a, GLTypeT b)
//{
//	GLTypeT value;
//	value = ((long long)(a) * (long long)(b)) >> (GL_MATRIX_G_FIXED_POINT + GL_MATRIX_G_FIXED_POINT - GL_MATRIX_A_FIXED_POINT);
//	return value;
//}
//
//GLTypeT fix_mul_AAG(GLTypeT a, GLTypeT b)
//{
//	GLTypeT value;
//	value = ((long long)(a) * (long long)(b)) >> (GL_MATRIX_A_FIXED_POINT + GL_MATRIX_A_FIXED_POINT - GL_MATRIX_G_FIXED_POINT);
//	return value;
//}
//
//GLTypeT fix_mul(GLTypeT a, GLTypeT b)
//{
//	GLTypeT value;
//	value = ((long long)(a) * (long long)(b)) >> GL_VALUE_FIXED_POINT;
//	return value;
//}
//
//long long fix_mul_longlong (GLTypeT a, GLTypeT b)
//{
//	long long value;
//	value = ((long long)(a) * (long long)(b)) >> GL_VALUE_FIXED_POINT;
//	return value;
//}

GLTypeT glCos(GLTypeT angle)
{
	GLTypeT cos, sin;
	glCosSin(angle, &cos, &sin);
	return cos;
}

GLTypeT glSin(GLTypeT angle)
{
	GLTypeT cos, sin;
	glCosSin(angle, &cos, &sin);
	return sin;
}

void glCosSin(GLTypeT angle, GLTypeT* cos, GLTypeT* sin)
{
	myMathClass mMath;
	GLint cos_int = 1024*1024, sin_int = 0;
	GLint s16_degrees;
	float cos_float, sin_float;
	s16_degrees = ((GLint)angle) << 4;
	mMath.CORDIC_32(s16_degrees, &cos_int, &sin_int);
	cos_float = (float)cos_int * 1.0 / (1 << 20);
	sin_float = (float)sin_int * 1.0 / (1 << 20);
	*cos = cos_float;
	*sin = sin_float;
}

//GLTypeT fix_div(GLTypeT a, GLTypeT b)
//{
//	GLTypeT result;
//	//TODO: only for simulation
//	float a_float;
//	float b_float;
//	float result_float;
//	a_float = a * (float)1.0 / GL_FIXED_ONE;
//	b_float = b * (float)1.0 / GL_FIXED_ONE;
//	result_float = a_float / b_float;
//	result = (GLTypeT)(result_float * GL_FIXED_ONE);
//	return result;
//}
//
//long long fix_div_longlong(long long a, long long b)
//{
//	GLTypeT result;
//	//TODO: only for simulation
//	float a_float;
//	float b_float;
//	float result_float;
//	a_float = a * (float)1.0 / GL_FIXED_ONE;
//	b_float = b * (float)1.0 / GL_FIXED_ONE;
//	result_float = a_float / b_float;
//	result = (GLTypeT)(result_float * GL_FIXED_ONE);
//	return result;
//}
//
//long long fix_div_AA_longlong(long long a, long long b)
//{
//	GLTypeT result;
//	//TODO: only for simulation
//	float a_float;
//	float b_float;
//	float result_float;
//	a_float = a * (float)1.0 / (1 << GL_MATRIX_A_FIXED_POINT);
//	b_float = b * (float)1.0 / (1 << GL_MATRIX_A_FIXED_POINT);
//	result_float = a_float / b_float;
//	result = (GLTypeT)(result_float * (1 << GL_MATRIX_A_FIXED_POINT));
//	return result;
//}
//
//long long fix_div_GA_longlong(long long a, long long b)
//{
//	GLTypeT result;
//	//TODO: only for simulation
//	float a_float;
//	float b_float;
//	float result_float;
//	a_float = a * (float)1.0 / (1 << GL_MATRIX_G_FIXED_POINT);
//	b_float = b * (float)1.0 / (1 << GL_MATRIX_A_FIXED_POINT);
//	result_float = a_float / b_float;
//	result = (GLTypeT)(result_float * (1 << GL_MATRIX_G_FIXED_POINT));
//	return result;
//}

#endif