//module : sxGLObject.h
//author : zhouyuzhi
//description: internal api for developer
//revision:
//   2024.7.25 initital version by zhouyuzhi
//

#ifndef SXGL_OBJECT__H
#define SXGL_OBJECT__H

//#include "sxGL.h"
#include "sxGLMath.h"

#define NUM_OF_TEXTURE_OBJECT 8
#define NUM_OF_ERROR_FIFO 128
#define MAX_NUM_OF_SHADED_VERTEX 1000
#define MAX_BYTE_OF_3D_SOURCEBUFFER 2000
#define SXGL_OBJECT_DEBUG

typedef struct _GLColorStruct{
    GLubyte red;
    GLubyte green;
    GLubyte blue;
    GLubyte alpha;
}GLColorStruct;

typedef union _GLEnableState{
    GLint u32_value;
    struct {
        GLint GL_STATE_CULL_FACE        : 1  ;      
        GLint GL_STATE_DITHER           : 1  ;   
        GLint GL_STATE_LIGHT0           : 1  ;   
        GLint GL_STATE_LIGHTING         : 1  ;     
        GLint GL_STATE_NORMALIZE        : 1  ;      
        GLint GL_STATE_RESCALE_NORMAL   : 1  ;           
        GLint GL_STATE_SCISSOR_TEST     : 1  ;         
        GLint GL_STATE_TEXTURE_2D       : 1  ;
        GLint GL_STATE_CLIP_FAR_PLANE   : 1  ;
        GLint GL_STATE_CLIP_LRTB_PLANE  : 1  ; // Left, Right, Top, Bottom clip
        GLint reserved                  : 22 ;      
    } bit;
}GLEnableStateUnion;

typedef union _GLEnableClientState{
    GLubyte u8_value;
    struct {
        GLint GL_STATE_COLOR_ARRAY          : 1 ; 
        GLint GL_STATE_NORMAL_ARRAY         : 1 ;  
        GLint GL_STATE_TEXTURE_COORD_ARRAY  : 1 ;         
        GLint GL_STATE_VERTEX_ARRAY         : 1 ;  
        GLint GL_STATE_POINT_SIZE_ARRAY_OES : 1 ;  
        GLint reserved                      : 3 ;      
    } bit;
}GLEnableClientStateUnion;

typedef struct _GLState{
    GLEnableStateUnion GLEnableState;
    GLEnableClientStateUnion GLEnableClientState;
    GLubyte GLClientTextureState;
    GLenum  GLDrawMode;
}GLState;

typedef struct _GLPointParamStruct{
    GLTypeT size;
    GLTypeT sizeMin;
    GLTypeT sizeMax;
    GLTypeT distanceAttenuation;
    GLTypeT fadeThresholdSize;
} GLPointParamStruct;

typedef struct _GLMaterialStruct{
    GLubyte face;
    GLColorStruct ambient;
    GLColorStruct diffuse;
}GLMaterialStruct;

typedef struct _GLVertexStructure {
    GLTypeT position[4];
    GLubyte color[4];
    GLTypeT texCoord[4];
}GLVertexStructure;

typedef struct _GLVertexPosStruct{
    GLTypeT pos[4];
}GLVertexPosStruct;

typedef struct _GLNormalDirectionStruct{
    GLTypeT pos[3]; //[4] must be 1, and module must be 1
}GLNormalDirectionStruct;

typedef struct _textureObjectInStruct{
	GLuint  type;
	GLushort width;
	GLushort height;
	GLuint   texAddr;
}textureObjectInStruct;



typedef struct _GLVectorStruct{
    GLTypeT pos[3];
}GLVectorStruct;

typedef struct _GLTexVertexStruct{
    GLTypeT pos[2];
}GLTexVertexStruct;

typedef struct _GLLightStruct{
    GLubyte name;
    GLColorStruct ambient;
    GLColorStruct diffuse;
    GLVertexStructure vertex;
    GLVectorStruct spotDirection;
}GLLightStruct;

typedef struct _GLMatrixStruct{   //matrix 4x4
	GLTypeTA matrixA[12];
	GLTypeTG matrixG[3];
	GLTypeTA matrixI;
}GLMatrixStruct;

typedef struct _GLMatrix3x3Struct{   //matrix 3x3
	GLTypeTA matrixA[6];
	GLTypeTG matrixG[2];
}GLMatrix3x3Struct;

typedef struct _GLSceneMatrixStruct{
    GLMatrixStruct modelviewMatrix;
    GLMatrixStruct projectionMatrix;
	GLMatrixStruct textureMatrix;
    GLMatrixStruct *pCurrentMatrix;
}GLSceneMatrixStruct;

typedef struct _GLPrimitiveVertexInfo{
	GLubyte numOfVertex;
	GLVertexPosStruct vertex[4];      //vertex after vertex shading
}GLPrimitiveVertexInfo;

typedef struct _GLPrimitiveStruct{
	GLushort primitiveIndex;
    GLVertexPosStruct modelVertex[4]; //vertex before vertex shading
    GLPrimitiveVertexInfo vertexInfo[4];
	GLVectorStruct    normal;
    GLMaterialStruct material;
    GLMatrixStruct   textureMatrix;
	GLMatrixStruct   mvpMatrix;
	GLMatrixStruct   viewPortMatrix;
	GLMatrixStruct   rotateMatrix; //only record rotation in model matrix, for lighting
	GLMatrix3x3Struct finalMatrix;
	textureObjectInStruct textureObject;
} GLPrimitiveStruct;

typedef struct _GLSceneStruct{
    GLPrimitiveStruct curPrimitive;
    GLLightStruct light;
    _GLSceneMatrixStruct matrix;
}GLSceneStruct;

typedef struct _GLClipStruct{
    GLTypeT frontEqn[4];
    GLTypeT backEqn[4];
    GLTypeT leftEqn[4];
    GLTypeT rightEqn[4];
    GLTypeT topEqn[4];
    GLTypeT buttomEqn[4];
}GLClipStruct;

typedef struct _GLCullStruct{
    GLenum frontFace;
	GLenum cullFace;
    GLTypeT depthRangeF;
    GLTypeT depthRangeN;
}GLCullStruct;

typedef struct _GLTextureObjectStruct{
    GLTypeT textureLodBias     ;
    GLubyte textureEnvMode     ;
    GLubyte textureEnvColor[4] ;
    GLubyte combineRGB         ;
    GLubyte combineAlpha       ;         
    GLubyte minFilter          ;      
    GLubyte magFilter          ;      
    GLubyte wraps              ;  
    GLubyte wrapt              ; 
    GLubyte internalFormat     ;           
    GLsizei width              ;  
    GLsizei height             ;   
    GLint boader               ; 
    GLubyte type               ; 
    void* pixels               ; 
}GLTextureObjectStruct;

typedef struct _GLScissiorStruct{
    GLint x;
    GLint y;
    GLsizei width              ;  
    GLsizei height             ;   
}GLScissiorStruct;

typedef struct _GLBlendFuncSturct{
    GLubyte sFactor;
    GLubyte dFactor;
}GLBlendFuncSturct;

typedef struct _GLRopStruct{
    GLScissiorStruct scissor;
    GLBlendFuncSturct blendFunc;
    GLubyte logicOP         ;  
    GLubyte ColorMask       ;    
    GLColorStruct clearColor;

}GLRopStruct;

typedef struct _GLDrawBox{
	GLubyte startX, startY, stopX, stopY;
}GLDrawBox;

typedef struct _GLSourceBufferStruct{
	GLuint numOfBytes;
    GLint* pSourceBufferData;
}GLSourceBufferStruct;

typedef struct _drawPointerParamStruct {
    GLint size;
    GLenum type;
    GLsizei stride;
    GLvoid* pointer;
}drawPointerParamStruct;

typedef struct _GLErrorFifo {
    GLuint error[NUM_OF_ERROR_FIFO];
    GLubyte wp, rp;
    GLubyte full, empty;
}GLErrorFifo;

typedef struct _GLScissorBox {
    GLint x, y, width, height;
}GLScissorBox;



typedef struct _GLMatrix3x3InStruct{   //matrix 3x3-1. because matrixI must be 1
	GLTypeTA matrixA[6];
	GLTypeTG matrixG[2];
}GLMatrix3x3InStruct;

typedef struct _vertexPosInStruct{
	GLTypeT pos[3];
}vertexPosInStruct;


typedef struct _GLContextStruct{
    GLState state;
    GLPointParamStruct pointParam;
    GLTypeT lineWidth;
    drawPointerParamStruct vertexPointerParam;
    drawPointerParamStruct pointSizePointerParam;
	drawPointerParamStruct normalPointerParam;
	textureObjectInStruct* TOPointer;
	GLMatrixStruct* TMPointer;
    GLSceneStruct scene;
    GLClipStruct clip;
    GLCullStruct cull;
    GLubyte curNumOfTextureObject;
    GLubyte curActiveTextureObject;
	GLTextureObjectStruct textureObject[NUM_OF_TEXTURE_OBJECT];
    GLRopStruct rop;
    GLScissorBox viewportBox;
    GLSourceBufferStruct sourcebufferStruct;
    GLErrorFifo errorFifo;
}GLContextStruct;

typedef struct _GLVertexShadingObjectStructure {
    GLVertexPosStruct* vertexPointer;
}GLVertexShadingObjectStructure;

typedef struct _GLTexInfoStruct{
    GLubyte EnableBackFilter;
    GLubyte RGBOrder;
    GLubyte EnableBackReverse;
	GLubyte EnableFrontReverse;
	GLubyte EnableForceRender;
    GLubyte TexType;
	GLubyte EnableAlphaBuffer;
	GLubyte AlphaBufferBlendType;
	GLshort TexWidth;
	GLshort TexHeight;
	GLuint  TexAddr;
}GLTexInfoStruct;

union _TexHead
{
    GLuint TexHead;                  
    struct
    {
        GLuint MaskType               : 2;
		GLuint                        : 1;
		GLuint MaskUsage              : 1;
		GLuint TexType                : 4;
		GLuint ForceRender            : 1;
		GLuint                        : 2;
		GLuint ForeInvColor           : 1;
		GLuint BackInvColor           : 1;
		GLuint                        : 2;
		GLuint LastTex                : 1;
		GLuint                        : 2;
		GLuint TexRGBOrder            : 1;
		GLuint                        : 5;
        GLuint BackFilterEn           : 1;
        GLuint                        : 6;
        GLuint ShortSb                : 1;
    }bit;
};

typedef struct _GLTexVertexArrayStruct{
    GLTexVertexStruct pos[4];	
}GLTexVertexArrayStruct;

typedef struct _GLTexMatrixStruct{
    GLTypeT Matrix[9];	
}GLTexMatrixStruct;


#ifdef __cplusplus
extern "C" {
#endif

extern GLContextStruct* pGLContext           ;
extern GLVertexShadingObjectStructure* pGLVSO; //vertex shading object
extern GLuint gTexAddress           [3];
extern GLuint gTexWidth             [3];
extern GLuint gTexHeight            [3];
extern GLuint gSourcebufferWidth    [3];
extern GLuint gSourcebufferHeight   [3];
extern GLuint gTexType              [3];  

void GLContextInit();

#ifdef __cplusplus
}
#endif

#endif