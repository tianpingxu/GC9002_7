////////////////////////////////////////////////////////////////////////////////
// Company:		 Graphichina Inc
//
// Create Date:   2016/03/21
// File Name:     DynamicPage.h
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2016/03/21 by Zhou Yuzhi 
// Additional Comments:
//    matrix
// 
////////////////////////////////////////////////////////////////////////////////


#ifndef MATRIXCLASS__H
#define MATRIXCLASS__H

#include "publicType.h"
#include "AHMIBasicDefine.h"
#include "publicDefine.h"

#ifdef AHMI_CORE
#ifdef UseFloatMatrix
float _20_To_float(s32 A);
float _13_To_float(s32 A);
float _4_To_float(s32 A);
s32 float_To_20(float A);
s32 float_To_13(float A);
s32 float_To_4(float A);
#endif

//NewMatrix created by yn 2018.10.15
class NewMatrix
{
public:
	//Positive matrix
	s32 A;	  //1.11.20
	s32 B;	  //1.11.20
	s32 C;	  //1.11.20
	s32 D;	  //1.11.20
	s32 E;    //1.11.20
	s32 F;    //1.11.20
	s32 G;	  //1.18.13   Offset
	s32 H;	  //1.18.13	  Offset
	#ifdef VERSION_2
	s32 I;	  //intial 1
	#endif

	//Inverse matrix
	s32 Inverse_A;	  //1.11.20
	s32 Inverse_B;	  //1.11.20
	s32 Inverse_C;	  //1.11.20
	s32 Inverse_D;	  //1.11.20
	s32 Inverse_E;    //1.11.20
	s32 Inverse_F;    //1.11.20
	s32 Inverse_G;	  //1.18.13   Offset
	s32 Inverse_H;	  //1.18.13   Offset
	#ifdef VERSION_2
	s32 Inverse_I;	  //intial 1
	#endif
#ifndef PC_SIM_Gen
	//-----------------------------
	// function name�� NewMatrix_init
	// NewMatrix initial 
	// param list��
	//   @param1 none
	// Remarks(Revision between versions):
	//   created by yn 2018.10.15
	//-----------------------------
	funcStatus NewMatrix_init(void);

	//-----------------------------
	// function name�� NewMatrixInv_init
	// NewMatrixInv initial 
	// param list��
	//   @param1 none
	// Remarks(Revision between versions):
	//   created by yn 2018.10.15
	//-----------------------------
	funcStatus NewMatrixInv_init(void);

	//-----------------------------
	// function name�� copyFrom(normal)
	// NewMatrix copy from : this = M2
	// param list��
	//   @param1 NewMatrix* M2
	// Remarks(Revision between versions):
	//   created by yn 2018.10.15
	//-----------------------------
	funcStatus copyFrom(NewMatrix* M2);

	//-----------------------------
	// function name�� copyFrom(Inverse)
	// NewMatrixInv copy from : this_Inv = M2_Inv
	// param list��
	//   @param1 NewMatrix* M2
	// Remarks(Revision between versions):
	//   created by yn 2018.10.15
	//-----------------------------
	funcStatus copyFromInv(NewMatrix* M2);

	//-----------------------------
	// function name�� copy
	// NewMatrix copy : M2 = this
	// param list��
	//   @param1 NewMatrix* M2
	// Remarks(Revision between versions):
	//   created by yn 2018.10.15
	//-----------------------------
	funcStatus copy(NewMatrix* M2);

	//-----------------------------
	// function name�� copyInv
	// NewMatrixInv copy : M2_Inv = this_Inv
	// param list��
	//   @param1 NewMatrix* M2
	// Remarks(Revision between versions):
	//   created by yn 2018.10.15
	//-----------------------------
	funcStatus copyInv(NewMatrix* M2);

	//-----------------------------
	// function name�� LeftMultiMatrix
	// NewMatrix leftMultiply : this = M2 * this
	// param list��
	//   @param1 NewMatrix *M2
	// Remarks(Revision between versions):
	//	Including the inverse matrix multiplication save in this_Inv
	//   created by yn 2018.10.15
	//-----------------------------
	funcStatus LeftMultiMatrix(NewMatrix *M2);
	funcStatus TranslateLeftMultiMatrix(NewMatrix *M2);
	funcStatus RotateLeftMultiMatrix(NewMatrix *M2);
	funcStatus ShearXLeftMultiMatrix(NewMatrix *M2);
	funcStatus ShearYLeftMultiMatrix(NewMatrix *M2);
	funcStatus ScaleLeftMultiMatrix(NewMatrix *M2);

	//-----------------------------
	// function name�� RightMultiMatrix
	// NewMatrix rightMultiply this = this * M2
	// param list��
	//   @param1 NewMatrix *M2
	// Remarks(Revision between versions):
	//   created by yn 2018.10.15
	//-----------------------------
	funcStatus RightMultiMatrix(NewMatrix *M2);
	funcStatus TranslateRightMultiMatrix(NewMatrix *M2);
	funcStatus RotateRightMultiMatrix(NewMatrix *M2);
	funcStatus ShearXRightMultiMatrix(NewMatrix *M2);
	funcStatus ShearYRightMultiMatrix(NewMatrix *M2);
	funcStatus ScaleRightMultiMatrix(NewMatrix *M2);

	//-----------------------------
	// function name�� TranslateToMatrix
	// Set translation matrix
	// param list��
	//   @param1 s32 MovingX 1.18.13
	//   @param2 s32 MovingY 1.18.13
	// Remarks(Revision between versions):
	//   created by yn 2018.10.15
	//-----------------------------
	funcStatus TranslateToMatrix(s32 MovingX, s32 MovingY);

	//-----------------------------
	// function name�� RotateToMatrix
	// Set Rotation matrix
	// param list��
	//   @param1 s16 degree 1.11.4
	// Remarks(Revision between versions):
	//   created by yn 2018.10.15
	//-----------------------------
	funcStatus RotateToMatrix(s16 RotateAngle);//1.11.4

	//-----------------------------
	// function name�� ShearXToMatrix
	// Set Shear matrix
	// param list��
	//   @param1 s16 ShearAngleX 1.11.4
	// Remarks(Revision between versions):
	//   created by yn 2018.10.15
	//-----------------------------
	funcStatus ShearXToMatrix(s16 ShearAngleX);//1.11.4

	//-----------------------------
	// funtion name�� ShearYToMatrix
	// Set Shear matrix
	// param list��
	//   @param1 s16 ShearAngleY  1.11.4
	// Remarks(Revision between versions):
	//   created by yn 2018.10.15
	//-----------------------------
	funcStatus ShearYToMatrix(s16 ShearAngleY);//1.11.4

	//-----------------------------
	// function name: ScalepToMatrix
	// Set scalep matrix
	// param list:
	//   @param1 s32 Width 
	//   @param2 s32 Height 
	// Remarks(Revision between versions):
	//   created by yn 2020.10.29
	//-----------------------------
	funcStatus ScalepToMatrix(s32 Width, s32 Height);

	//-----------------------------
	// function name: ScalenToMatrix
	// Set scalen matrix
	// param list:
	//   @param1 s32 Width 
	//   @param2 s32 Height 
	// Remarks(Revision between versions):
	//   created by yn 2020.10.29
	//-----------------------------
	funcStatus ScalenToMatrix(s32 Width, s32 Height);

	//-----------------------------
	// function name: RotateZToMatrix
	// Set Rotation Z matrix
	// param list:
	//   @param1 s16 degree 1.11.4
	// Remarks(Revision between versions):
	//   created by yn 2020.10.29
	//-----------------------------
	funcStatus RotateZToMatrix(s16 RotateAngle);

	//-----------------------------
	// funtion name: ShearZToMatrix
	// Set Shear matrix
	// param list??
	//   @param1 s16 ShearAngleZX  1.11.4
	//   @param2 s16 ShearAngleZY  1.11.4
	//   @param3 int Width  
	//   @param4 int Height  
	// Remarks(Revision between versions):
	//   created by yn 2019.03.14
	// for ZX(Positive matrix):| Width  0      0 | | 1		    0  0 | | 1/Width  0        0 | | cosZX	0  0 |   | cosZX		0  0 |  (inverse matrix): | 1/cosZX		 0   0 |
	//						   | 0	    Height 0 |x| 0	        1  0 |x| 0	      1/Height 0 |x| 0	    1  0 | = | 0			1  0 |					  | 0			 1   0 |
	//						   | 0	    0      1 | | -tanZX     0  1 | | 0		  0        1 | | 0	    0  1 |   | -sinZX/Width 0  1 |					  | tanZX/Width  0   1 |
	//
	// for ZY(Positive matrix):| Width  0      0 | | 1	   0       0 | | 1/Width  0        0 | | 1  0      0 |   | 1  0             0 | (inverse matrix): | 1  0             0 |
	//						   | 0	    Height 0 |x| 0	   1       0 |x| 0	      1/Height 0 |x| 0	cosZY  0 | = | 0  cosZY         0 |					  | 0  1/cosZY       0 |
	//						   | 0	    0      1 | | 0     -tanZY  1 | | 0		  0        1 | | 0	0      1 |   | 0  -sinZY/Height 1 |					  | 0  tanZY/Height  1 |
	//-----------------------------
	funcStatus ShearZToMatrix(s16 ShearAngleZX,s16 ShearAngleZY,int TexWidth,int TexHeight);

	//-----------------------------
	// function name�� ScaleToMatrix
	// Set Scale matrix
	// param list��
	//   @param1 s32 ScaleX : the scaler of x  1.6.9
	//   @param2 s32 ScaleY : the scaler of y  1.6.9
	// Remarks(Revision between versions):
	//   created by yn 2018.10.15
	//-----------------------------
	funcStatus ScaleToMatrix(s16 ScaleX, s16 ScaleY);//1.6.9

	funcStatus ScaleZToMatrix(s32 ScaleZ);//Temporarily unrealized

	//-----------------------------
	// function name�� translate
	// translate matrix
	// param list��
	//   @param1 s32 x 1.18.13
	//   @param2 s32 y 1.18.13
	// Remarks(Revision between versions):
	//		| 1 0 x |   | A D G | 
	//		| 0 1 y | * | B E H | 
	//		| 0 0 1 |   | 0 0 1 | 
	//   created by yn 2018.10.16
	//-----------------------------
	funcStatus translate(s32 x, s32 y);

	//-----------------------------
	// function name�� scale
	// scale matrix
	// param list��
	//   @param1 s16 x 1.6.9
	//   @param2 s16 y 1.6.9
	// Remarks(Revision between versions):
	//		| S 0 0 |	| A D G |
	//		| 0 S 0 | *	| B E H |
	//		| 0 0 1 |	| 0 0 1 |
	//   created by yn 2018.10.16
	//-----------------------------
	funcStatus scale(s16 x, s16 y);//1.6.9

	//-----------------------------
	// function name�� rotate
	// rotation matrix
	// param list��
	//   @param1 s16 degree 1.11.4
	// Remarks(Revision between versions):
	//		this = rotate * this
	//			created by yn 2018.10.16
	//-----------------------------
	funcStatus rotate(s16 degree);//1.11.20

	//-----------------------------
	// function name�� shear
	// shear matrix
	// param list��
	//   @param1 s16 degreeX 1.11.4
	//   @param2 s16 degreeY 1.11.4
	// Remarks(Revision between versions):
	//		this = shear * this
	//			created by yn 2018.10.16
	//-----------------------------
	funcStatus shear(s16 degreeX, s16 degreeY);//1.6.9

	funcStatus textureProject(s16 degreeX, s16 degreeY, int TexWidth, int TexHeight, int TexOffsetX, int TexOffsetY);

	//-----------------------------
	// function name�� project
	// project matrix
	// param list��
	//   @param1 s16 degreeX 1.11.4
	//   @param2 s16 degreeY 1.11.4
	//   @param3 int TexWidth 
	//   @param4 int TexHeight
	//   @param5 int TexOffsetX for texture to widget offsetX 1.11.4
	//   @param6 int TexOffsetY for texture to widget offsetY 1.11.4
	// Remarks(Revision between versions):
	//		this = project * this
	//			created by yn 2019.02.26
	//-----------------------------
	funcStatus project(s16 degreeX, s16 degreeY, int TexWidth, int TexHeight);

	//-----------------------------
	// function name?? Matrix_I_Normalization
	// normalization matrix I & Inverse_I
	// param list??
	// Remarks(Revision between versions):
	//			created by yn 2019.03.14
	//-----------------------------
	funcStatus Matrix_I_Normalization(void);
#endif
};

typedef NewMatrix* NewMatrixPtr;

//Coordinate point class
class PointClass
{
public:
	s32 mPointX; //1.27.4
	s32 mPointY; //1.27.4
	PointClass();
	PointClass (s32 x, s32 y);
	funcStatus leftMulMatrix(NewMatrixPtr pMatrix);
	funcStatus rightMulMatrix(NewMatrixPtr pMatrix);
};

//Range of interest


class ROIClass
{
public:
	PointClass lt;
	PointClass rt;
	PointClass lb;
	PointClass rb;

	ROIClass();
	~ROIClass();
};

typedef ROIClass* ROIClassPtr;

#endif

#endif

