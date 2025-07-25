#include "sxGL.h"
#include "sxGLObject.h"
#include "sxGLState.h"

#ifdef SXGL_LIB

//texture
void glGenTextures(GLsizei n, GLuint* textures)
{
    if (pGLContext->curNumOfTextureObject + n >= NUM_OF_TEXTURE_OBJECT)
    {
        pushError(GL_STACK_OVERFLOW);
    }
    else
    {
        pGLContext->curNumOfTextureObject += n;
    }
}

void glBindTexture(GLenum target, GLuint texture)
{
    if (target != GL_TEXTURE_2D)
    {
        pushError(GL_INVALID_ENUM);
        return;
    }
    if (texture > NUM_OF_TEXTURE_OBJECT)
    {
        pushError(GL_INVALID_VALUE);
        return;
    }
    pGLContext->curActiveTextureObject = texture;
}

void glTexEnvi(GLenum target, GLenum pname, GLint param)
{
    if (target != GL_TEXTURE_2D)
    {
        pushError(GL_INVALID_ENUM);
        return;
    }
    switch (pname)
    {
    case GL_TEXTURE_ENV_MODE:
        pGLContext->textureObject[pGLContext->curActiveTextureObject].textureEnvMode = param;
    default:
        pushError(GL_INVALID_ENUM);
        return;
    }
}
void glTexEnviv(GLenum target, GLenum pname, const GLint* params)
{
    if (target != GL_TEXTURE_2D)
    {
        pushError(GL_INVALID_ENUM);
        return;
    }
    switch (pname)
    {
    case GL_TEXTURE_ENV_MODE:
        pGLContext->textureObject[pGLContext->curActiveTextureObject].textureEnvMode = params[0];
    case GL_TEXTURE_ENV_COLOR:
        pGLContext->textureObject[pGLContext->curActiveTextureObject].textureEnvColor[0] = params[0];
        pGLContext->textureObject[pGLContext->curActiveTextureObject].textureEnvColor[1] = params[1];
        pGLContext->textureObject[pGLContext->curActiveTextureObject].textureEnvColor[2] = params[2];
        pGLContext->textureObject[pGLContext->curActiveTextureObject].textureEnvColor[3] = params[3];
    default:
        pushError(GL_INVALID_ENUM);
        return;
    }
}

void glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels)
{
    if (target != GL_TEXTURE_2D)
    {
        pushError(GL_INVALID_ENUM);
        return;
    }
    pGLContext->textureObject[pGLContext->curActiveTextureObject].internalFormat = internalformat;
    pGLContext->textureObject[pGLContext->curActiveTextureObject].width = width;
    pGLContext->textureObject[pGLContext->curActiveTextureObject].height = height;
    pGLContext->textureObject[pGLContext->curActiveTextureObject].boader = border;
    pGLContext->textureObject[pGLContext->curActiveTextureObject].type = type;
    pGLContext->textureObject[pGLContext->curActiveTextureObject].pixels = (GLvoid*)pixels;

}
void glTexParameteri(GLenum target, GLenum pname, GLint param)
{
    if (target != GL_TEXTURE_2D)
    {
        pushError(GL_INVALID_ENUM);
        return;
    }
    switch (pname)
    {
    case GL_TEXTURE_MIN_FILTER:
        if (param != GL_LINEAR && param != GL_NEAREST_MIPMAP_LINEAR)
        {
            pushError(GL_INVALID_ENUM);
            return;
        }
        else
        {
            pGLContext->textureObject[pGLContext->curActiveTextureObject].minFilter = param;
        }
        break;
    case GL_TEXTURE_MAG_FILTER:
        if (param != GL_LINEAR && param != GL_NEAREST_MIPMAP_LINEAR)
        {
            pushError(GL_INVALID_ENUM);
            return;
        }
        else
        {
            pGLContext->textureObject[pGLContext->curActiveTextureObject].magFilter = param;
        }
        break;
    case GL_TEXTURE_WRAP_S:
        if (param != GL_CLIP_TO_ZERO)
        {
            pushError(GL_INVALID_ENUM);
            return;
        }
        else
        {
            pGLContext->textureObject[pGLContext->curActiveTextureObject].wraps = param;
        }
        break;
    case GL_TEXTURE_WRAP_T:
        if (param != GL_CLIP_TO_ZERO)
        {
            pushError(GL_INVALID_ENUM);
            return;
        }
        else
        {
            pGLContext->textureObject[pGLContext->curActiveTextureObject].wrapt = param;
        }
        break;
    default:
        pushError(GL_INVALID_ENUM);
        break;
    }
}
void glTexParameteriv(GLenum target, GLenum pname, const GLint* params)
{
    if (target != GL_TEXTURE_2D)
    {
        pushError(GL_INVALID_ENUM);
        return;
    }
    switch (pname)
    {
    case GL_TEXTURE_MIN_FILTER:
        if (params[0] != GL_LINEAR && params[0] != GL_NEAREST_MIPMAP_LINEAR)
        {
            pushError(GL_INVALID_ENUM);
            return;
        }
        else
        {
            pGLContext->textureObject[pGLContext->curActiveTextureObject].minFilter = params[0];
        }
        break;
    case GL_TEXTURE_MAG_FILTER:
        if (params[0] != GL_LINEAR && params[0] != GL_NEAREST_MIPMAP_LINEAR)
        {
            pushError(GL_INVALID_ENUM);
            return;
        }
        else
        {
            pGLContext->textureObject[pGLContext->curActiveTextureObject].magFilter = params[0];
        }
        break;
    case GL_TEXTURE_WRAP_S:
        if (params[0] != GL_CLIP_TO_ZERO)
        {
            pushError(GL_INVALID_ENUM);
            return;
        }
        else
        {
            pGLContext->textureObject[pGLContext->curActiveTextureObject].wraps = params[0];
        }
        break;
    case GL_TEXTURE_WRAP_T:
        if (params[0] != GL_CLIP_TO_ZERO)
        {
            pushError(GL_INVALID_ENUM);
            return;
        }
        else
        {
            pGLContext->textureObject[pGLContext->curActiveTextureObject].wrapt = params[0];
        }
        break;
    default:
        pushError(GL_INVALID_ENUM);
        break;
    }
}
void glCopyTexImage2D(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
{
    ; //TODO
}
void glComparessTexImage2D(GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid* pixels)
{
    ; //TODO
}

#endif