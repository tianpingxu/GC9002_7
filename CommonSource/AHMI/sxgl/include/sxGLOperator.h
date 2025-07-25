#include "sxGLDefine.h"


#ifndef SXGL_OPERATOR__H
#define SXGL_OPERATOR__H

typedef int GLfixed;

#ifdef GL_OPERATION_TYPE_FLOAT

//typedef struct _GLTypeT{
//	float value;
//	void operator = (const struct _GLTypeT a)
//	{
//		value = a.value;
//	}
//	void operator = (const GLfixed a)
//	{
//		value = (float)(a * 1.0) / GL_FIXED_ONE;
//	}
//	void operator = (const float a)
//	{
//		value = a;
//	}
//	void operator = (const double a)
//	{
//		value = (float)a;
//	}
//	struct _GLTypeT operator + (const struct _GLTypeT a)
//	{
//		struct _GLTypeT result;
//		result.value = value + a.value;
//		return result;
//	}
//	struct _GLTypeT operator - (const struct _GLTypeT a)
//	{
//		struct _GLTypeT result;
//		result.value = value - a.value;
//		return result;
//	}
//	struct _GLTypeT operator*(const struct _GLTypeT a)
//	{
//		struct _GLTypeT result;
//		result.value = value * a.value;
//		return result;
//	}
//	struct _GLTypeT operator/(const struct _GLTypeT a)
//	{
//		struct _GLTypeT result;
//		result.value = value / a.value;
//		return result;
//	}
//	struct _GLTypeT operator-()
//	{
//		struct _GLTypeT result;
//		result.value = -value;
//		return result;
//	}
//	bool operator>(const struct _GLTypeT a)
//	{
//		return (value > a.value);
//	}
//	bool operator<(const struct _GLTypeT a)
//	{
//		return (value < a.value);
//	}
//	bool operator==(const struct _GLTypeT a)
//	{
//		return (value == a.value);
//	}
//	bool operator!=(const struct _GLTypeT a)
//	{
//		return (value != a.value);
//	}
//}GLTypeT;

typedef float   GLTypeT;
typedef GLTypeT GLTypeTA;
typedef GLTypeT GLTypeTG;

#define GL_TYPET_ONE 1.0 

#endif

#ifdef GL_OPERATION_TYPE_FIXED
typedef struct _GLTypeT{
	GLTypeT value;
}GLTypeT;
typedef GLTypeT GLtypeTA;
typedef GLTypeT GLtypeTG;

#define GL_TYPET_ONE GL_FIXED_ONE

#endif

GLfixed getFixFromT (GLTypeT a);
GLfixed getFixFromTA(GLTypeTA a);
GLfixed getFixFromTG(GLTypeTG a);
float   getFloatFromT (GLTypeT a);
float   getFloatFromTA(GLTypeTA a);
float   getFloatFromTG(GLTypeTG a);

#endif