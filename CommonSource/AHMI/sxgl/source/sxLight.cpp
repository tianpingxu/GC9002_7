#include "sxGL.h"
#include "sxGLObject.h"
#include "sxLight.h"
#include "sxGLPipeline.h"
#include "sxGLState.h"

#ifdef SXGL_LIB

GLubyte dirctional_light_for_primitive(GLVectorStruct* primitive_normal,GLMatrixStruct* primitiveMatrix,GLVectorStruct* light_dir)
{
	GLVectorStruct p_normal_trans;
	GLTypeT cos_theta,color_percent;
	GLTypeT color_percent_mul;
	GLTypeT color_percent_minus;
	GLubyte color_percent_byte;
	GLTypeT zero;
	zero = 0.0;
    //p_normal_trans.pos[0] = primitive_normal-¡·pos[0] * primitiveMatrix[0] + primitive_normal.pos[1] * primitiveMatrix[1] +primitive_normal.pos[2] * primitiveMatrix[2];
	//p_normal_trans.pos[1] = primitive_normal-¡·pos[0] * primitiveMatrix[4] + primitive_normal.pos[1] * primitiveMatrix[5] +primitive_normal.pos[2] * primitiveMatrix[6];
	//p_normal_trans.pos[2] = primitive_normal-¡·pos[0] * primitiveMatrix[8] + primitive_normal.pos[1] * primitiveMatrix[9] +primitive_normal.pos[2] * primitiveMatrix[10];	
	vector_mul_m4(primitiveMatrix, primitive_normal, &p_normal_trans);
	cos_theta = p_normal_trans.pos[0] * light_dir->pos[0] + p_normal_trans.pos[1] * light_dir->pos[1] + p_normal_trans.pos[2] * light_dir->pos[2];
	//if(cos_theta < zero)
	//	cos_theta = - cos_theta;
	//color_percent = cos_theta;
	color_percent_mul = (float)(0.25);
	color_percent_minus = (float)(0.75);
	color_percent = color_percent_minus + cos_theta * color_percent_mul;
	//color_percent_byte = (GLubyte)(color_percent.value * 255);
	color_percent_byte = (GLubyte)(color_percent * 255);
	return color_percent_byte;
}

void glLightModeli (GLenum pname, GLint param)
{
	switch(pname)
	{
		case GL_LIGHT_MODEL_AMBIENT:
			break;
		case GL_LIGHT_MODEL_TWO_SIDE:
			break;
		case GL_LIGHT_MODEL_LOCAL_VIEWER:
			break;
		default:
			pushError(GL_INVALID_ENUM);
			break;
	}
}
void glLightModeliv (GLenum pname, const GLint *params)
{
	switch(pname)
	{
		case GL_LIGHT_MODEL_AMBIENT:
			break;
		case GL_LIGHT_MODEL_TWO_SIDE:
			break;
		case GL_LIGHT_MODEL_LOCAL_VIEWER:
			break;
		default:
			pushError(GL_INVALID_ENUM);
			return;
	}
}
void glLighti (GLenum light, GLenum pname, GLTypeT param)
{
	if(light != GL_LIGHT0) //only support 1 light
	{
		pushError(GL_INVALID_ENUM);
		return;
	}
	switch(pname)
	{
	case GL_AMBIENT:
		pGLContext->scene.light.ambient.red = (GLubyte)param;
		pGLContext->scene.light.ambient.green = (GLubyte)param;
		pGLContext->scene.light.ambient.blue = (GLubyte)param;
		pGLContext->scene.light.ambient.alpha = (GLubyte)param;
		break;
	case GL_DIFFUSE:
		pGLContext->scene.light.diffuse.red = (GLubyte)param;
		pGLContext->scene.light.diffuse.green = (GLubyte)param;
		pGLContext->scene.light.diffuse.blue = (GLubyte)param;
		pGLContext->scene.light.diffuse.alpha = (GLubyte)param;
		break;
	case GL_SPOT_DIRECTION:
		pGLContext->scene.light.spotDirection.pos[0] = param;
		pGLContext->scene.light.spotDirection.pos[1] = param;
		pGLContext->scene.light.spotDirection.pos[2] = param;
		break;
	default:
		pushError(GL_INVALID_ENUM);
		return;
	}
}
void glLightiv (GLenum light, GLenum pname, const GLTypeT *params)
{
	if(light != GL_LIGHT0) //only support 1 light
	{
		pushError(GL_INVALID_ENUM);
		return;
	}
	switch(pname)
	{
	case GL_AMBIENT:
		pGLContext->scene.light.ambient.red = (GLubyte)params[0];
		pGLContext->scene.light.ambient.green = (GLubyte)params[1];
		pGLContext->scene.light.ambient.blue = (GLubyte)params[2];
		pGLContext->scene.light.ambient.alpha = (GLubyte)params[3];
		break;
	case GL_DIFFUSE:
		pGLContext->scene.light.diffuse.red = (GLubyte)params[0];
		pGLContext->scene.light.diffuse.green = (GLubyte)params[1];
		pGLContext->scene.light.diffuse.blue = (GLubyte)params[2];
		pGLContext->scene.light.diffuse.alpha = (GLubyte)params[3];
		break;
	case GL_SPOT_DIRECTION:
		pGLContext->scene.light.spotDirection.pos[0] = params[0];
		pGLContext->scene.light.spotDirection.pos[1] = params[1];
		pGLContext->scene.light.spotDirection.pos[2] = params[2];
		break;
	default:
		pushError(GL_INVALID_ENUM);
		return;
	}
}
void glMateriali (GLenum face, GLenum pname, GLint param);
void glMaterialiv (GLenum face, GLenum pname, const GLint *params);

#endif