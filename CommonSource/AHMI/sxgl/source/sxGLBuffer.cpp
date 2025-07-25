#include "sxGLBuffer.h"
#include <stdlib.h>
#include "platform.h"
#include "SXGLState.h"
#ifdef EMBEDDED
#include "gc90x_heap.h"
#endif

#ifdef SXGL_LIB

GLint mNumber = 0;
GLint fNumber = 0;
GLvoid* glGenBuffer(GLuint size)
{
    GLvoid* buffer;
	#ifdef PC_SIM
    buffer = (GLvoid*)malloc((size_t)size);
	#endif
	#ifdef EMBEDDED
	buffer = (GLvoid*)gcMalloc((size_t)size);
	#endif
	if(buffer == NULL)
	{
		pushError(GL_INVALID_VALUE);
	}
	mNumber ++;
    return buffer;
}

void glDeleteBuffer(GLvoid* buffer)
{
	#ifdef PC_SIM
    free(buffer);
	#endif
	#ifdef EMBEDDED
	gcFree(buffer);
	#endif
	fNumber++;
}

#endif

