#include "sxGLObjIn.h"
#include "sxGLObject.h"
#include "stdio.h"
#include "stdlib.h"
#include "sxMatrix.h"
#include "drawimmediately_cd.h"

//#define READ_FROM_FILE

#ifdef SXGL_LIB

sxGLObjIn gsxGLObjIn;


//TODO: only for debug. this parameters should be in flash

GLVertexPosStruct vertexArray[MAX_OBJECT_NUM];
GLVertexPosStruct normArray[MAX_PRIMITIVE_NUM];
textureObjectInStruct TOI[MAX_PRIMITIVE_NUM];
GLMatrixStruct TOM[MAX_PRIMITIVE_NUM];
primitivePointerStruct gPrimitive[MAX_PRIMITIVE_NUM];
GLVertexStructure fromTexToModelVertex[4];

//input from file
double vertexArrayInFile[MAX_OBJECT_NUM][3] = 
{

	{0,0,0},
	{1,0,0},
	{0,1,0},
	{1,1,0},
	{0,0,1},
	{1,0,1},
	{0,1,1},
	{1,1,1},

	{-1,-1,-1},
	{1,-1,-1},
	{-1,1,-1},
	{1,1,-1},
	{-1,-1,1},
	{1,-1,1},
	{-1,1,1},
	{1,1,1},

	{-0.4,-0.4,1},
	{0.4,-0.4,1},
	{-0.4,0.4,1},
	{0.4,0.4,1},

	{-1,-0.5,-0.5},
	{-1,-0.5,0.5},
	{-1,0.5,-0.5},
	{-1,0.5,0.5},

	{0.4,-0.4,-1},
	{-0.4,-0.4,-1},
	{0.4,0.4,-1},
	{-0.4,0.4,-1},
	
	{1,-0.5,0.5},
	{1,-0.5,-0.5},
	{1,0.5,0.5},
	{1,0.5,-0.5},

	{-0.5,-1,-0.5},
	{0.5,-1,-0.5},
	{-0.5,-1,0.5},
	{0.5,-1,0.5},

	{0.5,1,-0.5},
	{-0.5,1,-0.5},
	{0.5,1,0.5},
	{-0.5,1,0.5},

	
}
	;
double texCoordArrayInFile[4][2] = {
	{0,0},
	{1,0},
	{0,1},
	{1,1}
};
double normArrayInFile[6][3] = {
	{0,0,1},
	{1,0,0},
	{0,0,-1},
	{-1,0,0},
	{0,1,0},
	{0,-1,0}
};
float  texCoordArray[MAX_OBJECT_NUM][2];
int    primitiveVertexIndex[MAX_PRIMITIVE_NUM][4] = {
	{5+8,6+8,8+8,7+8},
	{6+8,2+8,4+8,8+8},
	{2+8,1+8,3+8,4+8},
	{1+8,5+8,7+8,3+8},
	{7+8,8+8,4+8,3+8},
	{1+8,2+8,6+8,5+8},

	{5,6,8,7},
	{6,2,4,8},
	{2,1,3,4},
	{1,5,7,3},
	{7,8,4,3},
	{1,2,6,5},

	{5,6,8,7},
	{6,2,4,8},
	{2,1,3,4},
	{1,5,7,3},
	{7,8,4,3},
	{1,2,6,5},

};
int    primitiveTexCoordIndex[MAX_PRIMITIVE_NUM][4] = {
	{1,2,4,3},
	{1,2,4,3},
	{1,2,4,3},
	{1,2,4,3},
	{1,2,4,3},
	{1,2,4,3},

	{2,1,3,4},
	{2,1,3,4},
	{2,1,3,4},
	{2,1,3,4},
	{2,1,3,4},
	{2,1,3,4},

	{2,1,3,4},
	{2,1,3,4},
	{2,1,3,4},
	{2,1,3,4},
	{2,1,3,4},
	{3,4,2,1}
};
int    primitiveVNIndex[MAX_PRIMITIVE_NUM][4] = {
	{1,1,1,1},
	{2,2,2,2},
	{3,3,3,3},
	{4,4,4,4},
	{5,5,5,5},
	{6,6,6,6},

	{1,1,1,1},
	{2,2,2,2},
	{3,3,3,3},
	{4,4,4,4},
	{5,5,5,5},
	{6,6,6,6},

	{1,1,1,1},
	{2,2,2,2},
	{3,3,3,3},
	{4,4,4,4},
	{5,5,5,5},
	{6,6,6,6}
};
unsigned int numOfVertex;
unsigned int numOfTexCoord;
unsigned int numOfPrimitive;
unsigned int numOfNorm;


void readObj(char* filename)
{
    FILE* fp;
    char line[1000];
	GLuint i, j, k;
	GLuint primitive_index;
	char debugInfo[1000];

	numOfPrimitive = 6*3;
    numOfVertex = numOfPrimitive * 4;
    numOfTexCoord = numOfPrimitive * 4;
	numOfNorm = numOfPrimitive * 4;
#ifdef READ_FROM_FILE
	numOfVertex = 0;
    numOfTexCoord = 0;
    numOfPrimitive = 0;
	numOfNorm = 0;
    fopen_s(&fp, filename, "r");
    if (fp == NULL)
    {
        printf("ERROR: cannot open input file %s", filename);
        return;
    }

    while (!feof(fp))
    {
        fgets(line, 1000, fp);
        switch (line[0])
        {
        case 'v':
            if (line[1] == 't')
            {
                sscanf_s(line, "vt %lf %lf", &texCoordArrayInFile[numOfTexCoord][0], &texCoordArrayInFile[numOfTexCoord][1]);
                numOfTexCoord++;
            }
			else if(line[1] == 'n')
			{
				sscanf_s(line, "vt %lf %lf %lf", &normArrayInFile[numOfNorm][0], &normArrayInFile[numOfNorm][1], &normArrayInFile[numOfNorm][2]);
                numOfNorm++;
			}
            else if(line[1] == ' ')
            {
                sscanf_s(line, "v %lf %lf %lf", &vertexArrayInFile[numOfVertex][0], &vertexArrayInFile[numOfVertex][1], &vertexArrayInFile[numOfVertex][2]);
                numOfVertex++;
            }
            break;
        case 'f':
            sscanf_s(line, "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", 
                &primitiveVertexIndex[numOfPrimitive][0], &primitiveTexCoordIndex[numOfPrimitive][0], &primitiveVNIndex[numOfPrimitive][0],
                &primitiveVertexIndex[numOfPrimitive][1], &primitiveTexCoordIndex[numOfPrimitive][1], &primitiveVNIndex[numOfPrimitive][1],
                &primitiveVertexIndex[numOfPrimitive][2], &primitiveTexCoordIndex[numOfPrimitive][2], &primitiveVNIndex[numOfPrimitive][2],
                &primitiveVertexIndex[numOfPrimitive][3], &primitiveTexCoordIndex[numOfPrimitive][3], &primitiveVNIndex[numOfPrimitive][3]
            );
            numOfPrimitive++;
            break;
        default:
            break;
        }
    }
#endif
	
	//reoder for textureObjectIn
	for(i = 0; i < numOfPrimitive; i++)
	{
		for(j = 0; j < 4; j++)//vertex
		{
			for(k = 0; k < 3; k++) //vertex position component
			{
				vertexArray[i*4+j].pos[k] = vertexArrayInFile[primitiveVertexIndex[i][j] - 1][k];
				
			}
			vertexArray[i*4+j].pos[3] = 1.0;
			for(k = 0; k < 2; k++) //texCoord
			{
				texCoordArray[i*4+j][k] = texCoordArrayInFile[primitiveTexCoordIndex[i][j] - 1][k];
			}
		}
		//normal
		for(k = 0; k < 3; k++) //vertex position component
		{
			normArray  [i].pos[k] = normArrayInFile[primitiveVNIndex[i][0] - 1][k];
			
		}
		//TODO: temporary set texture object to preset value
		
	}

	//TODO: temporary set inside and outside cube
	//for(i = 0; i < 6; i++)
	//{
	//	TOI[i].width = gSourcebufferWidth[0];
	//	TOI[i].height = gSourcebufferHeight[0];
	//	TOI[i].texAddr = gTexAddress[0];
	//	TOI[i].type = gTexType[0];
	//}

	//4 walls
	for(i = 0; i < 4; i++)
	{
		TOI[i].width   = 2048;
		TOI[i].height  = 1024;
		//TOI[i].texAddr = (i%2) ? 0x62c000 : 0x14000;
		TOI[i].texAddr = 0x62c000;
		TOI[i].type    = STCRGBA888 >> 4;
	}

	//floor
	TOI[4].width   = 2048;
	TOI[4].height  = 1024;
	TOI[4].texAddr = 0x218000;
	TOI[4].type    = STCRGBA888 >> 4;

	//ceil
	TOI[5].width   = 2048;
	TOI[5].height  = 1024;
	TOI[5].texAddr = 0x41c000;
	TOI[5].type    = STCRGBA888 >> 4;


	//logo cube
	for(i = 0; i < 6; i++)
	{
		TOI[i + 6].width = 256;
		TOI[i + 6].height = 128;
		TOI[i + 6].texAddr = 0x8000;
		TOI[i + 6].type = STCRGBA888 >> 4;
	}

	//left cube
	for(i = 0; i < 6; i++)
	{
		TOI[i + 6 + 6].width   = 256;
		TOI[i + 6 + 6].height  = 128;
		TOI[i + 6 + 6].texAddr = 0x620000;
		TOI[i + 6 + 6].type    = STCRGBA888 >> 4;
	}

	
	

	



	//generate TOM
	
	GLTypeT texWidthTypeT, texHeightTypeT;
	for(i = 0; i < numOfPrimitive; i++)
	{
		for(j = 0; j < 4; j++)
		{
			for(k = 0; k < 3; k++)
			{
				fromTexToModelVertex[j].position[k] = vertexArray[i*4+j].pos[k];
			}
			fromTexToModelVertex[j].position[3] = 1.0;

			for(k = 0; k < 2; k++)
			{
				fromTexToModelVertex[j].texCoord[k] = texCoordArray[i*4+j][k];
			}
			fromTexToModelVertex[j].texCoord[2] = 0.0;
			fromTexToModelVertex[j].texCoord[3] = 1.0;
		}
		//texWidthTypeT = gTexWidth[i/6] - 2.0;
		//texHeightTypeT = gTexHeight[i/6] - 2.0;
		if(i < 6)
		{
			texWidthTypeT = 1024.0;
			texHeightTypeT = 1024.0;
		}
		else 
		{
			texWidthTypeT = 128.0;
			texHeightTypeT = 128.0;
		}
		from_tex_to_model(fromTexToModelVertex, texWidthTypeT, texHeightTypeT, &TOM[i]);
	}

	//TODO: tempory set primitive to preset value
	for(i = 0; i < numOfPrimitive; i++)
	{
		gPrimitive[i].indexType = GL_INDEX_TYPE_INDEX_NUM;//TODO: primitive temprory set to type 0
		gPrimitive[i].primitiveType = GL_QUADS; //TODO: need to support other type
		gPrimitive[i].TOIndex = i; //TODO: support 1 texture in demo
		gPrimitive[i].vertexIndexNum = 4;//TODO: support only quads in demo
		gPrimitive[i].vertexIndex.startIndex = i * 4;
	}

	gsxGLObjIn.numPrimitive = numOfPrimitive;
    gsxGLObjIn.vertexPointer = vertexArray;
	gsxGLObjIn.normalPointer = normArray;
	gsxGLObjIn.TOPointer = TOI;
	gsxGLObjIn.TMPointer = TOM;
	gsxGLObjIn.primitives = gPrimitive;

	
    return;



}

GLTypeT double2fix(double i)
{
    GLTypeT o;
    o = i;
    return o;
}

void from_tex_to_model(GLVertexStructure vertex[4], GLTypeT tex_width, GLTypeT tex_height, GLMatrixStruct* tex_to_modelMatrix)
{
	GLTypeT x0, y0, z0, x1, y1, z1, x2, y2, z2;
	GLTypeT s0, t0, s1, t1, s2, t2;
	GLTypeT u0, v0, u1, v1, u2, v2;
	GLTypeT x0_minus_x1, x0_minus_x2, y0_minus_y1, y0_minus_y2, z0_minus_z1, z0_minus_z2;
	GLTypeT s0_minus_s1, s0_minus_s2, t0_minus_t1, t0_minus_t2;
	GLTypeT u0_minus_u1, u0_minus_u2, v0_minus_v1, v0_minus_v2;
	GLTypeT a, b, c, d, e, f, g, h, i;
	GLTypeT a0, a1, a2, a3;
	x0 = vertex[0].position[0];
	y0 = vertex[0].position[1];
	z0 = vertex[0].position[2];
	s0 = vertex[0].texCoord[0];
	t0 = vertex[0].texCoord[1];
	x1 = vertex[1].position[0];
	y1 = vertex[1].position[1];
	z1 = vertex[1].position[2];
	s1 = vertex[1].texCoord[0];
	t1 = vertex[1].texCoord[1];
	x2 = vertex[2].position[0];
	y2 = vertex[2].position[1];
	z2 = vertex[2].position[2];
	s2 = vertex[2].texCoord[0];
	t2 = vertex[2].texCoord[1];
	u0 = s0 * tex_width ;
	v0 = t0 * tex_height;
	u1 = s1 * tex_width ;
	v1 = t1 * tex_height;
	u2 = s2 * tex_width ;
	v2 = t2 * tex_height;
	if(u0 == 0)
		u0 = u0 + 1.0;
	else if(u0 > tex_width - 2.0)
		u0 = u0 - 1.0;
	if(v0 == 0)
		v0 = v0 + 1;
	else if(v0 > tex_height - 2)
		v0 = v0 - 1;
	if(u1 == 0)
		u1 = u1 + 1;
	else if(u1 > tex_width - 2)
		u1 = u1 - 1;
	if(v1 == 0)
		v1 = v1 + 1;
	else if(v1 > tex_height - 2)
		v1 = v1 - 1;
	if(u2 == 0)
		u2 = u2 + 1;
	else if(u2 > tex_width - 2)
		u2 = u2 - 1;
	if(v2 == 0)
		v2 = v2 + 1;
	else if(v2 > tex_height - 2)
		v2 = v2 - 1;
	x0_minus_x1 = x0 - x1;
	x0_minus_x2 = x0 - x2;
	y0_minus_y1 = y0 - y1;
	y0_minus_y2 = y0 - y2;
	z0_minus_z1 = z0 - z1;
	z0_minus_z2 = z0 - z2;
	s0_minus_s1 = s0 - s1;
	s0_minus_s2 = s0 - s2;
	t0_minus_t1 = t0 - t1;
	t0_minus_t2 = t0 - t2;
	u0_minus_u1 = u0-u1;
	u0_minus_u2 = u0-u2;
	v0_minus_v1 = v0-v1;
	v0_minus_v2 = v0-v2;
	
	a0 = x0_minus_x1 * v0_minus_v2;
	a1 = x0_minus_x2 * v0_minus_v1;
	a2 = u0_minus_u1 * v0_minus_v2;
	a3 = u0_minus_u2 * v0_minus_v1;

	a = (a0 - a1)/(a2 - a3);
	b = (x0_minus_x1 * u0_minus_u2 - x0_minus_x2 * u0_minus_u1) / (v0_minus_v1 * u0_minus_u2 - v0_minus_v2 * u0_minus_u1);
	c = x0 - a*u0 - b* v0;

	d = ( y0_minus_y1 * v0_minus_v2 - y0_minus_y2 * v0_minus_v1) / (u0_minus_u1 * v0_minus_v2 - u0_minus_u2 * v0_minus_v1);
	e = ( y0_minus_y1 * u0_minus_u2 - y0_minus_y2 * u0_minus_u1) / (v0_minus_v1 * u0_minus_u2 - v0_minus_v2 * u0_minus_u1);
	f = y0 - (d * u0) - (e * v0);

	g = (z0_minus_z1 * v0_minus_v2 - z0_minus_z2 * v0_minus_v1) / (u0_minus_u1 * v0_minus_v2 - u0_minus_u2 * v0_minus_v1);
	h = (z0_minus_z1 * u0_minus_u2 - z0_minus_z2 * u0_minus_u1) / (v0_minus_v1 * u0_minus_u2 - v0_minus_v2 * u0_minus_u1);
	i = z0 - g * u0 - h * v0;

	tex_to_modelMatrix->matrixA[0] = a;
	tex_to_modelMatrix->matrixA[4] = b;
	tex_to_modelMatrix->matrixG[0] = c;

	tex_to_modelMatrix->matrixA[1] = d;
	tex_to_modelMatrix->matrixA[5] = e;
	tex_to_modelMatrix->matrixG[1] = f;

	tex_to_modelMatrix->matrixA[2] = g;
	tex_to_modelMatrix->matrixA[6] = h;
	tex_to_modelMatrix->matrixG[2] = i;

	tex_to_modelMatrix->matrixA[3] = 0;
	tex_to_modelMatrix->matrixA[7] = 0;
	tex_to_modelMatrix->matrixA[8] = 0;
	tex_to_modelMatrix->matrixA[9] = 0;
	tex_to_modelMatrix->matrixA[10] = 0;
	tex_to_modelMatrix->matrixA[11] = 0;
	tex_to_modelMatrix->matrixI    = GL_TYPET_ONE; 
}

#endif