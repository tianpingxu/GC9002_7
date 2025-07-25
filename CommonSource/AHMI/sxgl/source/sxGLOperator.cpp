#include "sxGLOperator.h"
//
//#ifdef GL_OPERATION_TYPE_FLOAT

#ifdef SXGL_LIB
GLfixed getFixFromT (GLTypeT a)
{
	GLfixed result;
	result = (GLfixed)(a * GL_FIXED_ONE);
	return result;
}
GLfixed getFixFromTA(GLTypeT a)
{
	long long result;
	result = (long long)(a * 2 * (1 << GL_MATRIX_A_FIXED_POINT));
	result = (result>>1) + (long long)result%2;
	return (GLfixed)result;
}
GLfixed getFixFromTG(GLTypeT a)
{
	long long result;
	result = (a * 2 * (1 << GL_MATRIX_G_FIXED_POINT));
	result = (result>>1) + result%2;
	return (GLfixed)result;
}
//float getFloatFromT (GLTypeT a)
//{
//	return a.value;
//}
//float getFloatFromTA(GLTypeT a)
//{
//	return a.value;
//}
//float getFloatFromTG(GLTypeT a)
//{
//	return a.value;
//}
//#endif

#endif