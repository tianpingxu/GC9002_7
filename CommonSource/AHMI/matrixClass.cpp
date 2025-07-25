////////////////////////////////////////////////////////////////////////////////
// Company:		Graphichina Inc
//
// Create Date:   2016/03/21
// File Name:     myMathClass.cpp
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2016/03/21 by Zhou Yuzhi 
// Additional Comments:
//    
// 
////////////////////////////////////////////////////////////////////////////////

#include "publicInclude.h"
#include "myMathClass.h"
#if (defined VERSION_2) && (defined EMBEDDED) 
#include "csi_ahmi_fix.h"
#endif
#ifdef AHMI_CORE
/* only used for simulator debug */
#ifdef UseFloatMatrix
float _20_To_float(s32 A)
{
	return (float)A / (1 << 20);
}

float _13_To_float(s32 A)
{
	return (float)A / (1 << 13);
}

float _4_To_float(s32 A)
{
	return (float)A / (1 << 4);
}

s32 float_To_20(float A)
{
	return A * (1 << 20);
}

s32 float_To_13(float A)
{
	return A * (1 << 13); 
}
s32 float_To_4(float A)
{
	return A * (1 << 4); 
}
#endif

PointClass::PointClass()
{
	this->mPointX = 0;
	this->mPointY = 0;
}

//-----------------------------
// name: PointClass
// Constructor of coordinate class
//  
//   @param1 x X direction coordinates
//   @param2 y Y direction coordinates
// addtional comments :
//   created by arthas 2016506
//-----------------------------
PointClass::PointClass(s32 x, s32 y)
{
	this->mPointX = x;
	this->mPointY = y;
}

//-----------------------------
// name: leftMulMatrix
// Left multiplicative coordinates of matrices
//  
//   @param1 matrixClassPtr Matrix pointer
// addtional comments :
//   created by arthas 20160506
//-----------------------------
funcStatus PointClass::leftMulMatrix(NewMatrixPtr pMatrix)
{
	s32 pointX, pointY;//fixed-point number 1.27.4(1 means sign bit, 27 means integer bits, 4 means decimal bits)
	s32 z;//fixed-point number 1.19.12(1 means sign bit, 19 means integer bits, 12 means decimal bits)

	#if (defined PC_SIM)
	//Used to judge whether normalization is required
	z = (((long long)(pMatrix->Inverse_C) * (this->mPointX)) + ((long long)(pMatrix->Inverse_F) * (this->mPointY)) + ((long long)(pMatrix->Inverse_I) << 11)) >> 12;
	
	//x=Ax+Cy+G
	//y=Bx+Dy+H
	pointX = (((long long)(pMatrix->Inverse_A) * (this->mPointX)) + ((long long)(pMatrix->Inverse_D) * (this->mPointY)) + ((long long)(pMatrix->Inverse_G) << 11)) >> 20;
	pointY = (((long long)(pMatrix->Inverse_B) * (this->mPointX)) + ((long long)(pMatrix->Inverse_E) * (this->mPointY)) + ((long long)(pMatrix->Inverse_H) << 11)) >> 20;
	#endif
	
	#if (defined VERSION_2) && (defined EMBEDDED)
	//Used to judge whether normalization is required
	z = (csi_fix_mult((pMatrix->Inverse_C), (this->mPointX), 0) + csi_fix_mult((pMatrix->Inverse_F), (this->mPointY), 0) + ((long long)(pMatrix->Inverse_I) << 11)) >> 12;
	
	//x=Ax+Cy+G
	//y=Bx+Dy+H
	pointX = (csi_fix_mult((pMatrix->Inverse_A), (this->mPointX), 0) + csi_fix_mult((pMatrix->Inverse_D), (this->mPointY), 0) + ((long long)(pMatrix->Inverse_G) << 11)) >> 20;
    pointY = (csi_fix_mult((pMatrix->Inverse_B), (this->mPointX), 0) + csi_fix_mult((pMatrix->Inverse_E), (this->mPointY), 0) + ((long long)(pMatrix->Inverse_H) << 11)) >> 20;
	#endif
	
	//Used to judge whether normalization is required
	if(z != 1 << 12)
	{
		//normalization to min
		if(z == 0)
			z = 1;

		//normalization
		pointX = (pointX << 12)/ z;
		pointY = (pointY << 12)/ z;
	}

	this->mPointX = pointX;
	this->mPointY = pointY;
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name: rightMulMatrix
// Left multiplicative coordinates of matrices
//  
//   @param1 matrixClassPtr Matrix pointer
// addtional comments :
//   created by arthas 20160506
//-----------------------------
funcStatus PointClass::rightMulMatrix(NewMatrixPtr pMatrix)
{
	s32 pointX, pointY;//fixed-point number 1.27.4(1 means sign bit, 27 means integer bits, 4 means decimal bits)
	s32 z;//fixed-point number 1.19.12(1 means sign bit, 19 means integer bits, 12 means decimal bits)

	#if (defined PC_SIM)
	//Used to judge whether normalization is required
	z = (((long long)(pMatrix->C) * (this->mPointX)) + ((long long)(pMatrix->F) * (this->mPointY)) + ((long long)(pMatrix->I) << 11)) >> 12;
	
	//x=Ax+Cy+G
	//y=Bx+Dy+H
	pointX = (((long long)(pMatrix->A) * (this->mPointX)) + ((long long)(pMatrix->D) * (this->mPointY)) + ((long long)(pMatrix->G) << 11)) >> 20;
	pointY = (((long long)(pMatrix->B) * (this->mPointX)) + ((long long)(pMatrix->E) * (this->mPointY)) + ((long long)(pMatrix->H) << 11)) >> 20;
	#endif
	
	#if (defined VERSION_2) && (defined EMBEDDED)
	//Used to judge whether normalization is required
	z = (csi_fix_mult((pMatrix->C), (this->mPointX), 0) + csi_fix_mult((pMatrix->F), (this->mPointY), 0) + ((long long)(pMatrix->I) << 11)) >> 12;
	
	//x=Ax+Cy+G
	//y=Bx+Dy+H
	pointX = (csi_fix_mult((pMatrix->A), (this->mPointX), 0) + csi_fix_mult((pMatrix->D), (this->mPointY), 0) + ((long long)(pMatrix->G) << 11)) >> 20;
    pointY = (csi_fix_mult((pMatrix->B), (this->mPointX), 0) + csi_fix_mult((pMatrix->E), (this->mPointY), 0) + ((long long)(pMatrix->H) << 11)) >> 20;
	#endif
	
	//Used to judge whether normalization is required
	if(z != 1 << 12)
	{
		//normalization to min
		if(z == 0)
			z = 1;

		//normalization
		pointX = (pointX << 12)/ z;
		pointY = (pointY << 12)/ z;
	}

	this->mPointX = pointX;
	this->mPointY = pointY;
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// NewMatrix:
//        | A D G | 
//        | B E H | 
//        | C F I | 
// A,B,C,D,E,F is fixed-point number by 1.11.20(1 means sign bit, 11 means integer bits, 20 means decimal bits)
//       G,H,I is fixed-point number by 1.18.13(1 means sign bit, 18 means integer bits, 13 means decimal bits)
//-----------------------------
//-----------------------------
// function name: NewMatrix_init
// NewMatrix initial 
// param list:
//   @param1 none
// Remarks(Revision between versions):
//   created by yn 2018.10.15
//-----------------------------
funcStatus NewMatrix::NewMatrix_init(void)
{
	this->A = (1 << 20);
	this->B = 0;
	this->C = 0;
	this->D = 0;
	this->E = (1 << 20);
	this->F = 0;
	this->G = 0;
	this->H = 0;
	#ifdef VERSION_2
	this->I = (1 << 13);
	#endif

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function name: NewMatrixInv_init
// NewMatrixInv initial 
// param list:
//   @param1 none
// Remarks(Revision between versions):
//   created by yn 2018.10.15
//-----------------------------
funcStatus NewMatrix::NewMatrixInv_init(void)
{
	this->Inverse_A = (1 << 20);
	this->Inverse_B = 0;
	this->Inverse_C = 0;
	this->Inverse_D = 0;
	this->Inverse_E = (1 << 20);
	this->Inverse_F = 0;
	this->Inverse_G = 0;
	this->Inverse_H = 0;
	#ifdef VERSION_2
	this->Inverse_I = (1 << 13);
	#endif

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function name: copyFrom(normal)
// NewMatrix copy from: this = M2
// param list:
//   @param1 NewMatrix* M2
// Remarks(Revision between versions):
//   created by yn 2018.10.15
//-----------------------------
funcStatus NewMatrix::copyFrom(NewMatrix* M2)
{
	this->A = M2->A;
	this->B = M2->B;
	this->C = M2->C;
	this->D = M2->D;
	this->E = M2->E;
	this->F = M2->F;
	this->G = M2->G;
	this->H = M2->H;
	#ifdef VERSION_2
	this->I = M2->I;
	#endif

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function name: copyFrom(Inverse)
// NewMatrixInv copy from: this_Inv = M2_Inv
// param list:
//   @param1 NewMatrix* M2
// Remarks(Revision between versions):
//   created by yn 2018.10.15
//-----------------------------
funcStatus NewMatrix::copyFromInv(NewMatrix* M2)
{
	this->Inverse_A = M2->Inverse_A;
	this->Inverse_B = M2->Inverse_B;
	this->Inverse_C = M2->Inverse_C;
	this->Inverse_D = M2->Inverse_D;
	this->Inverse_E = M2->Inverse_E;
	this->Inverse_F = M2->Inverse_F;
	this->Inverse_G = M2->Inverse_G;
	this->Inverse_H = M2->Inverse_H;
	#ifdef VERSION_2
	this->Inverse_I = M2->Inverse_I;
	#endif

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function name: copy
// NewMatrix copy: M2 = this
// param list:
//   @param1 NewMatrix* M2
// Remarks(Revision between versions):
//   created by yn 2018.10.15
//-----------------------------
funcStatus NewMatrix::copy(NewMatrix* M2)
{
	M2->A = this->A;
	M2->B = this->B;
	M2->C = this->C;
	M2->D = this->D;
	M2->E = this->E;
	M2->F = this->F;
	M2->G = this->G;
	M2->H = this->H;
	#ifdef VERSION_2
	M2->I = this->I;
	#endif

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function name: copyInv
// NewMatrixInv copy : M2_Inv = this_Inv
// param list:
//   @param1 NewMatrix* M2
// Remarks(Revision between versions):
//   created by yn 2018.10.15
//-----------------------------
funcStatus NewMatrix::copyInv(NewMatrix* M2)
{
	M2->Inverse_A = this->Inverse_A;
	M2->Inverse_B = this->Inverse_B;
	M2->Inverse_C = this->Inverse_C;
	M2->Inverse_D = this->Inverse_D;
	M2->Inverse_E = this->Inverse_E;
	M2->Inverse_F = this->Inverse_F;
	M2->Inverse_G = this->Inverse_G;
	M2->Inverse_H = this->Inverse_H;
	#ifdef VERSION_2
	M2->Inverse_I = this->Inverse_I;
	#endif

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function name: LeftMultiMatrix
// NewMatrix leftMultiply : this = M2 * this
// M2:				this(input):
//   | A D G | 				   | a d g | 
//   | B E H |       x   	   | b e h | 
//   | C F I |          	   | c f i | 
// this(output):
// 	 | A*a+D*b+G*c A*d+D*e+G*f A*g+D*h+G*i | 
//   | B*a+E*b+H*c B*d+E*e+H*f B*g+E*h+H*i | 
//   | C*a+F*b+I*c C*d+F*e+I*f C*g+F*h+I*i | 
//
// param list:
//   @param1 NewMatrix *M2
// Remarks(Revision between versions):
//	Including the inverse matrix multiplication save in this_Inv
//   created by yn 2018.10.15
//   change by XT, 20200410
//-----------------------------
funcStatus NewMatrix::LeftMultiMatrix(NewMatrix *M2)
{
	NewMatrix tempMatrix;
	tempMatrix.NewMatrixInv_init();
#ifdef UseFloatMatrix
	#ifdef VERSION_2
	tempMatrix.A = float_To_20(_20_To_float(M2->A) * _20_To_float(this->A) + _20_To_float(M2->D) * _20_To_float(this->B) + _13_To_float(M2->G) * _20_To_float(this->C));
	tempMatrix.B = float_To_20(_20_To_float(M2->B) * _20_To_float(this->A) + _20_To_float(M2->E) * _20_To_float(this->B) + _13_To_float(M2->H) * _20_To_float(this->C));
	tempMatrix.C = float_To_20(_20_To_float(M2->C) * _20_To_float(this->A) + _20_To_float(M2->F) * _20_To_float(this->B) + _13_To_float(M2->I) * _20_To_float(this->C));
	tempMatrix.D = float_To_20(_20_To_float(M2->A) * _20_To_float(this->D) + _20_To_float(M2->D) * _20_To_float(this->E) + _13_To_float(M2->G) * _20_To_float(this->F));
	tempMatrix.E = float_To_20(_20_To_float(M2->B) * _20_To_float(this->D) + _20_To_float(M2->E) * _20_To_float(this->E) + _13_To_float(M2->H) * _20_To_float(this->F));
	tempMatrix.F = float_To_20(_20_To_float(M2->C) * _20_To_float(this->D) + _20_To_float(M2->F) * _20_To_float(this->E) + _13_To_float(M2->I) * _20_To_float(this->F));
	tempMatrix.G = float_To_13(_20_To_float(M2->A) * _13_To_float(this->G) + _20_To_float(M2->D) * _13_To_float(this->H) + _13_To_float(M2->G) * _13_To_float(this->I));
	tempMatrix.H = float_To_13(_20_To_float(M2->B) * _13_To_float(this->G) + _20_To_float(M2->E) * _13_To_float(this->H) + _13_To_float(M2->H) * _13_To_float(this->I));
	tempMatrix.I = float_To_13(_20_To_float(M2->C) * _13_To_float(this->G) + _20_To_float(M2->F) * _13_To_float(this->H) + _13_To_float(M2->I) * _13_To_float(this->I));
	#endif
#else
	#if (defined PC_SIM) 
	tempMatrix.A = (((long long)(M2->A)*(this->A)) + ((long long)(M2->D)*(this->B)) + (((long long)(M2->G)*(this->C))<<7)>>20);
	tempMatrix.B = (((long long)(M2->B)*(this->A)) + ((long long)(M2->E)*(this->B)) + (((long long)(M2->H)*(this->C))<<7)>>20);
	tempMatrix.C = (((long long)(M2->C)*(this->A)) + ((long long)(M2->F)*(this->B)) + (((long long)(M2->I)*(this->C))<<7)>>20);
	tempMatrix.D = (((long long)(M2->A)*(this->D)) + ((long long)(M2->D)*(this->E)) + (((long long)(M2->G)*(this->F))<<7)>>20);
	tempMatrix.E = (((long long)(M2->B)*(this->D)) + ((long long)(M2->E)*(this->E)) + (((long long)(M2->H)*(this->F))<<7)>>20);
	tempMatrix.F = (((long long)(M2->C)*(this->D)) + ((long long)(M2->F)*(this->E)) + (((long long)(M2->I)*(this->F))<<7)>>20);
	tempMatrix.G = (((long long)(M2->A)*(this->G)) + ((long long)(M2->D)*(this->H)) + (((long long)(M2->G)*(this->I))<<7)>>20);
	tempMatrix.H = (((long long)(M2->B)*(this->G)) + ((long long)(M2->E)*(this->H)) + (((long long)(M2->H)*(this->I))<<7)>>20);
	#ifdef VERSION_2
	tempMatrix.I = (((long long)(M2->C)*(this->G)) + ((long long)(M2->F)*(this->H)) + (((long long)(M2->I)*(this->I))<<7)>>20);
	#endif
	#endif
	#if (defined VERSION_2) && (defined EMBEDDED) 
//    tempMatrix.A = csi_fix_mult20x20(M2->A, this->A) + csi_fix_mult20x20(M2->D, this->B) + csi_fix_mult13x20(M2->G,  this->C);
//	tempMatrix.B = csi_fix_mult20x20(M2->B, this->A) + csi_fix_mult20x20(M2->E, this->B) + csi_fix_mult13x20(M2->H,  this->C);
//	tempMatrix.C = csi_fix_mult20x20(M2->C, this->A) + csi_fix_mult20x20(M2->F, this->B) + csi_fix_mult13x20(M2->I,  this->C);
//	tempMatrix.D = csi_fix_mult20x20(M2->A, this->D) + csi_fix_mult20x20(M2->D, this->E) + csi_fix_mult13x20(M2->G,  this->F);
//	tempMatrix.E = csi_fix_mult20x20(M2->B, this->D) + csi_fix_mult20x20(M2->E, this->E) + csi_fix_mult13x20(M2->H,  this->F);
//	tempMatrix.F = csi_fix_mult20x20(M2->C, this->D) + csi_fix_mult20x20(M2->F, this->E) + csi_fix_mult13x20(M2->I,  this->F);
//	tempMatrix.G = csi_fix_mult20x13(M2->A, this->G) + csi_fix_mult20x13(M2->D, this->H) + csi_fix_mult13x13(M2->G,  this->I);
//	tempMatrix.H = csi_fix_mult20x13(M2->B, this->G) + csi_fix_mult20x13(M2->E, this->H) + csi_fix_mult13x13(M2->H,  this->I);
//	tempMatrix.I = csi_fix_mult20x13(M2->C, this->G) + csi_fix_mult20x13(M2->F, this->H) + csi_fix_mult13x13(M2->I,  this->I);

	csi_fix_mult_vec20x20(&(tempMatrix.A),&(M2->A),this->A,0);
	csi_fix_mult_vec20x20(&(tempMatrix.A),&(M2->D),this->B,1);
	csi_fix_mult_vec13x20(&(tempMatrix.A),&(M2->G),this->C,1);


	csi_fix_mult_vec20x20(&(tempMatrix.D),&(M2->A),this->D,0);
	csi_fix_mult_vec20x20(&(tempMatrix.D),&(M2->D),this->E,1);
	csi_fix_mult_vec13x20(&(tempMatrix.D),&(M2->G),this->F,1);

	
	csi_fix_mult_vec20x13(&(tempMatrix.G),&(M2->A),this->G,0);
	csi_fix_mult_vec20x13(&(tempMatrix.G),&(M2->D),this->H,1);
	csi_fix_mult_vec13x13(&(tempMatrix.G),&(M2->G),this->I,1);	

	#endif

#endif
	this->copyFrom(&tempMatrix);
	this->RightMultiMatrix(M2);

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function name: TranslateLeftMultiMatrix
// M2:				this(input):
//   | 1 0 G | 				   | a d g | 
//   | 0 1 H |       x   	   | b e h | 
//   | 0 0 1 |          	   | c f i | 
// this(output):
// 	 | 1*a+0*b+G*c 1*d+0*e+G*f 1*g+0*h+G*i | 
//   | 0*a+1*b+H*c 0*d+1*e+H*f 0*g+1*h+H*i | 
//   | 0*a+0*b+1*c 0*d+0*e+1*f 0*g+0*h+1*i | 
//-----------------------------
funcStatus NewMatrix::TranslateLeftMultiMatrix(NewMatrix *M2)
{
	NewMatrix tempMatrix;
	tempMatrix.NewMatrixInv_init();
#ifdef UseFloatMatrix
	tempMatrix.A = float_To_20(_20_To_float(this->A) + _13_To_float(M2->G)*_20_To_float(this->C));
	tempMatrix.B = float_To_20(_20_To_float(this->B) + _13_To_float(M2->H)*_20_To_float(this->C));
	tempMatrix.C = (this->C);
	tempMatrix.D = float_To_20(_20_To_float(this->D) + _13_To_float(M2->G)*_20_To_float(this->F));
	tempMatrix.E = float_To_20(_20_To_float(this->E) + _13_To_float(M2->H)*_20_To_float(this->F));
	tempMatrix.F = (this->F);
	tempMatrix.G = float_To_13(_13_To_float(this->G) + _13_To_float(M2->G)*_20_To_float(this->I));
	tempMatrix.H = float_To_13(_13_To_float(this->H) + _13_To_float(M2->H)*_20_To_float(this->I));
	#ifdef VERSION_2
	tempMatrix.I = (this->I);
	#endif
#else
	#if (defined PC_SIM) 
	tempMatrix.A = (this->A) + (((long long)(M2->G)*(this->C))>>13);
	tempMatrix.B = (this->B) + (((long long)(M2->H)*(this->C))>>13);
	tempMatrix.C = (this->C);
	tempMatrix.D = (this->D) + (((long long)(M2->G)*(this->F))>>13);
	tempMatrix.E = (this->E) + (((long long)(M2->H)*(this->F))>>13);
	tempMatrix.F = (this->F);
	tempMatrix.G = (this->G) + (((long long)(M2->G)*(this->I))>>13);
	tempMatrix.H = (this->H) + (((long long)(M2->H)*(this->I))>>13);
	#ifdef VERSION_2
	tempMatrix.I = (this->I);
	#endif
	#endif
	#if (defined VERSION_2) && (defined EMBEDDED)  
  	tempMatrix.A = (this->A) + csi_fix_mult13x20(M2->G, this->C);
	tempMatrix.B = (this->B) + csi_fix_mult13x20(M2->H, this->C);
	tempMatrix.C = (this->C);
	tempMatrix.D = (this->D) + csi_fix_mult13x20(M2->G, this->F);
	tempMatrix.E = (this->E) + csi_fix_mult13x20(M2->H, this->F);
	tempMatrix.F = (this->F);
	tempMatrix.G = (this->G) + csi_fix_mult13x20(M2->G, this->I);
	tempMatrix.H = (this->H) + csi_fix_mult13x20(M2->H, this->I);
	tempMatrix.I = (this->I);
	#endif
#endif

	this->copyFrom(&tempMatrix);
	this->TranslateRightMultiMatrix(M2);

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function name: RotateLeftMultiMatrix
// M2:				this(input):
//   | A D 0 | 				   | a d g | 
//   | B E 0 |       x   	   | b e h | 
//   | 0 0 1 |          	   | c f i | 
// this(output):
// 	 | A*a+D*b+0*c A*d+D*e+0*f A*g+D*h+0*i | 
//   | B*a+E*b+0*c B*d+E*e+0*f B*g+E*h+0*i | 
//   | 0*a+0*b+1*c 0*d+0*e+1*f 0*g+0*h+1*i | 
//-----------------------------
funcStatus NewMatrix::RotateLeftMultiMatrix(NewMatrix *M2)
{
	NewMatrix tempMatrix;
	tempMatrix.NewMatrixInv_init();
#ifdef UseFloatMatrix
	tempMatrix.A = float_To_20(_20_To_float(M2->A)*_20_To_float(this->A) + _20_To_float(M2->D)*_20_To_float(this->B));
	tempMatrix.B = float_To_20(_20_To_float(M2->B)*_20_To_float(this->A) + _20_To_float(M2->E)*_20_To_float(this->B));
	tempMatrix.C = (this->C);
	tempMatrix.D = float_To_20(_20_To_float(M2->A)*_20_To_float(this->D) + _20_To_float(M2->D)*_20_To_float(this->E));
	tempMatrix.E = float_To_20(_20_To_float(M2->B)*_20_To_float(this->D) + _20_To_float(M2->E)*_20_To_float(this->E));
	tempMatrix.F = (this->F);
	tempMatrix.G = float_To_13(_20_To_float(M2->A)*_13_To_float(this->G) + _20_To_float(M2->D)*_13_To_float(this->H));
	tempMatrix.H = float_To_13(_20_To_float(M2->B)*_13_To_float(this->G) + _20_To_float(M2->E)*_13_To_float(this->H));
	#ifdef VERSION_2
	tempMatrix.I = (this->I);
	#endif
#else
	#if (defined PC_SIM) 
	tempMatrix.A = (((long long)(M2->A)*(this->A))>>20) + (((long long)(M2->D)*(this->B))>>20);
	tempMatrix.B = (((long long)(M2->B)*(this->A))>>20) + (((long long)(M2->E)*(this->B))>>20);
	tempMatrix.C = (this->C);
	tempMatrix.D = (((long long)(M2->A)*(this->D))>>20) + (((long long)(M2->D)*(this->E))>>20);
	tempMatrix.E = (((long long)(M2->B)*(this->D))>>20) + (((long long)(M2->E)*(this->E))>>20);
	tempMatrix.F = (this->F);
	tempMatrix.G = (((long long)(M2->A)*(this->G))>>20) + (((long long)(M2->D)*(this->H))>>20);
	tempMatrix.H = (((long long)(M2->B)*(this->G))>>20) + (((long long)(M2->E)*(this->H))>>20);
	#ifdef VERSION_2
	tempMatrix.I = (this->I);
	#endif
	#endif
	#if (defined VERSION_2) && (defined EMBEDDED) 
    tempMatrix.A = csi_fix_mult20x20(M2->A, this->A) + csi_fix_mult20x20(M2->D, this->B);
	tempMatrix.B = csi_fix_mult20x20(M2->B, this->A) + csi_fix_mult20x20(M2->E, this->B);
	tempMatrix.C = (this->C);
	tempMatrix.D = csi_fix_mult20x20(M2->A, this->D) + csi_fix_mult20x20(M2->D, this->E);
	tempMatrix.E = csi_fix_mult20x20(M2->B, this->D) + csi_fix_mult20x20(M2->E, this->E);
	tempMatrix.F = (this->F);
	tempMatrix.G = csi_fix_mult20x13(M2->A, this->G) + csi_fix_mult20x13(M2->D, this->H);
	tempMatrix.H = csi_fix_mult20x13(M2->B, this->G) + csi_fix_mult20x13(M2->E, this->H);
	tempMatrix.I = (this->I);
	#endif
#endif
	this->copyFrom(&tempMatrix);
	this->RotateRightMultiMatrix(M2);

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function name: ShearXLeftMultiMatrix
// M2:				this(input):
//   | 1 D 0 | 				   | a d g | 
//   | 0 1 0 |       x   	   | b e h | 
//   | 0 0 1 |          	   | c f i | 
// this(output):
// 	 | 1*a+D*b+0*c 1*d+D*e+0*f 1*g+D*h+0*i | 
//   | 0*a+1*b+0*c 0*d+1*e+0*f 0*g+1*h+0*i | 
//   | 0*a+0*b+1*c 0*d+0*e+1*f 0*g+0*h+1*i | 
//-----------------------------
funcStatus NewMatrix::ShearXLeftMultiMatrix(NewMatrix *M2)
{
	NewMatrix tempMatrix;
	tempMatrix.NewMatrixInv_init();
#ifdef UseFloatMatrix
	tempMatrix.A = float_To_20(_20_To_float(this->A) + _20_To_float(M2->D)*_20_To_float(this->B));
	tempMatrix.B = (this->B);
	tempMatrix.C = (this->C);
	tempMatrix.D = float_To_20(_20_To_float(this->D) + _20_To_float(M2->D)*_20_To_float(this->E));
	tempMatrix.E = (this->E);
	tempMatrix.F = (this->F);
	tempMatrix.G = float_To_13(_13_To_float(this->G) + _20_To_float(M2->D)*_13_To_float(this->H));
	tempMatrix.H = (this->H);
	#ifdef VERSION_2
	tempMatrix.I = (this->I);
	#endif
#else
	#if (defined PC_SIM) 
	tempMatrix.A = (this->A) + (((long long)(M2->D)*(this->B))>>20);
	tempMatrix.B = (this->B);
	tempMatrix.C = (this->C);
	tempMatrix.D = (this->D) + (((long long)(M2->D)*(this->E))>>20);
	tempMatrix.E = (this->E);
	tempMatrix.F = (this->F);
	tempMatrix.G = (this->G) + (((long long)(M2->D)*(this->H))>>20);
	tempMatrix.H = (this->H);
	#ifdef VERSION_2
	tempMatrix.I = (this->I);
	#endif
	#endif
	#if (defined VERSION_2) && (defined EMBEDDED) 
    tempMatrix.A = (this->A) + csi_fix_mult20x20(M2->D, this->B);
	tempMatrix.B = (this->B);
	tempMatrix.C = (this->C);
	tempMatrix.D = (this->D) + csi_fix_mult20x20(M2->D, this->E);
	tempMatrix.E = (this->E);
	tempMatrix.F = (this->F);
	tempMatrix.G = (this->G) + csi_fix_mult20x13(M2->D, this->H);
	tempMatrix.H = (this->H);
	tempMatrix.I = (this->I);
	#endif
#endif
	this->copyFrom(&tempMatrix);
	this->ShearXRightMultiMatrix(M2);

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function name: ShearYLeftMultiMatrix
// M2:				this(input):
//   | 1 0 0 | 				   | a d g | 
//   | B 1 0 |       x   	   | b e h | 
//   | 0 0 1 |          	   | c f i | 
// this(output):
// 	 | 1*a+0*b+0*c 1*d+0*e+0*f 1*g+0*h+0*i | 
//   | B*a+1*b+0*c B*d+1*e+0*f B*g+1*h+0*i | 
//   | 0*a+0*b+1*c 0*d+0*e+1*f 0*g+0*h+1*i | 
//-----------------------------
funcStatus NewMatrix::ShearYLeftMultiMatrix(NewMatrix *M2)
{
	NewMatrix tempMatrix;
	tempMatrix.NewMatrixInv_init();
#ifdef UseFloatMatrix
	tempMatrix.A = (this->A);
	tempMatrix.B = float_To_20(_20_To_float(M2->B)*_20_To_float(this->A) + _20_To_float(this->B));
	tempMatrix.C = (this->C);
	tempMatrix.D = (this->D);
	tempMatrix.E = float_To_20(_20_To_float(M2->B)*_20_To_float(this->D) + _20_To_float(this->E));
	tempMatrix.F = (this->F);
	tempMatrix.G = (this->G);
	tempMatrix.H = float_To_13(_20_To_float(M2->B)*_13_To_float(this->G) + _13_To_float(this->H));
	#ifdef VERSION_2
	tempMatrix.I = (this->I);
	#endif
#else
	#if (defined PC_SIM) 
	tempMatrix.A = (this->A);
	tempMatrix.B = (((long long)(M2->B)*(this->A))>>20) + (this->B);
	tempMatrix.C = (this->C);
	tempMatrix.D = (this->D);
	tempMatrix.E = (((long long)(M2->B)*(this->D))>>20) + (this->E);
	tempMatrix.F = (this->F);
	tempMatrix.G = (this->G);
	tempMatrix.H = (((long long)(M2->B)*(this->G))>>20) + (this->H);
	#ifdef VERSION_2
	tempMatrix.I = (this->I);
	#endif
	#endif
	#if (defined VERSION_2) && (defined EMBEDDED) 
	tempMatrix.A = (this->A);
	tempMatrix.B = csi_fix_mult20x20(M2->B, this->A) + (this->B);
	tempMatrix.C = (this->C);
	tempMatrix.D = (this->D);
	tempMatrix.E = csi_fix_mult20x20(M2->B, this->D) + (this->E);
	tempMatrix.F = (this->F);
	tempMatrix.G = (this->G);
	tempMatrix.H = csi_fix_mult20x13(M2->B, this->G) + (this->H);
	tempMatrix.I = (this->I);
	#endif
#endif
	this->copyFrom(&tempMatrix);
	this->ShearYRightMultiMatrix(M2);

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function name: ScaleLeftMultiMatrix
// M2:				this(input):
//   | A 0 0 | 				   | a d g | 
//   | 0 E 0 |       x   	   | b e h | 
//   | 0 0 1 |          	   | c f i | 
// this(output):
// 	 | A*a+0*b+0*c A*d+0*e+0*f A*g+0*h+0*i | 
//   | 0*a+E*b+0*c 0*d+E*e+0*f 0*g+E*h+0*i | 
//   | 0*a+0*b+1*c 0*d+0*e+1*f 0*g+0*h+1*i | 
//-----------------------------
funcStatus NewMatrix::ScaleLeftMultiMatrix(NewMatrix *M2)
{
	NewMatrix tempMatrix;
	tempMatrix.NewMatrixInv_init();
#ifdef UseFloatMatrix
	tempMatrix.A = float_To_20(_20_To_float(M2->A)*_20_To_float(this->A));
	tempMatrix.B = float_To_20(_20_To_float(M2->E)*_20_To_float(this->B));
	tempMatrix.C = (this->C);
	tempMatrix.D = float_To_20(_20_To_float(M2->A)*_20_To_float(this->D));
	tempMatrix.E = float_To_20(_20_To_float(M2->E)*_20_To_float(this->E));
	tempMatrix.F = (this->F);
	tempMatrix.G = float_To_13(_20_To_float(M2->A)*_13_To_float(this->G));
	tempMatrix.H = float_To_13(_20_To_float(M2->E)*_13_To_float(this->H));
	#ifdef VERSION_2
	tempMatrix.I = (this->I);
	#endif
#else
	#if (defined PC_SIM) 
	tempMatrix.A = (((long long)(M2->A)*(this->A))>>20);
	tempMatrix.B = (((long long)(M2->E)*(this->B))>>20);
	tempMatrix.C = (this->C);
	tempMatrix.D = (((long long)(M2->A)*(this->D))>>20);
	tempMatrix.E = (((long long)(M2->E)*(this->E))>>20);
	tempMatrix.F = (this->F);
	tempMatrix.G = (((long long)(M2->A)*(this->G))>>20);
	tempMatrix.H = (((long long)(M2->E)*(this->H))>>20);
	#ifdef VERSION_2
	tempMatrix.I = (this->I);
	#endif
	#endif
	#if (defined VERSION_2) && (defined EMBEDDED)
	tempMatrix.A = csi_fix_mult20x20(M2->A, this->A);
	tempMatrix.B = csi_fix_mult20x20(M2->E, this->B);
	tempMatrix.C = (this->C);
	tempMatrix.D = csi_fix_mult20x20(M2->A, this->D);
	tempMatrix.E = csi_fix_mult20x20(M2->E, this->E);
	tempMatrix.F = (this->F);
	tempMatrix.G = csi_fix_mult20x13(M2->A, this->G);
	tempMatrix.H = csi_fix_mult20x13(M2->E, this->H);
	tempMatrix.I = (this->I);
	#endif
#endif
	this->copyFrom(&tempMatrix);
	this->ScaleRightMultiMatrix(M2);

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function name: RightMultiMatrix
// NewMatrix rightMultiply this = this * M2 for Inverse
// this(input):										M2:
//   | Inverse_A Inverse_D Inverse_G | 				   | Inverse_a Inverse_d Inverse_g | 
//   | Inverse_B Inverse_E Inverse_H |       x   	   | Inverse_b Inverse_e Inverse_h | 
//   | Inverse_C Inverse_F Inverse_I |          	   | Inverse_c Inverse_f Inverse_i | 
// this(output):
// 	 | Inverse_A*Inverse_a+Inverse_D*Inverse_b+Inverse_G*Inverse_c  Inverse_A*Inverse_d+Inverse_D*Inverse_e+Inverse_G*Inverse_f  Inverse_A*Inverse_g+Inverse_D*Inverse_h+Inverse_G*Inverse_i | 
//   | Inverse_B*Inverse_a+Inverse_E*Inverse_b+Inverse_H*Inverse_c  Inverse_B*Inverse_d+Inverse_E*Inverse_e+Inverse_H*Inverse_f  Inverse_B*Inverse_g+Inverse_E*Inverse_h+Inverse_H*Inverse_i | 
//   | Inverse_C*Inverse_a+Inverse_F*Inverse_b+Inverse_I*Inverse_c  Inverse_C*Inverse_d+Inverse_F*Inverse_e+Inverse_I*Inverse_f  Inverse_C*Inverse_g+Inverse_F*Inverse_h+Inverse_I*Inverse_i | 
//
// param list:
//   @param1 NewMatrix *M2
// Remarks(Revision between versions):
//   created by yn 2018.10.15
//   change by XT, 20200410
//-----------------------------
funcStatus NewMatrix::RightMultiMatrix(NewMatrix *M2)
{
	NewMatrix tempMatrix;
#ifdef UseFloatMatrix
	#ifdef VERSION_2
	tempMatrix.Inverse_A = float_To_20(_20_To_float(this->Inverse_A) * _20_To_float(M2->Inverse_A) + _20_To_float(this->Inverse_D) * _20_To_float(M2->Inverse_B) + _13_To_float(this->Inverse_G) * _20_To_float(M2->Inverse_C));
	tempMatrix.Inverse_B = float_To_20(_20_To_float(this->Inverse_B) * _20_To_float(M2->Inverse_A) + _20_To_float(this->Inverse_E) * _20_To_float(M2->Inverse_B) + _13_To_float(this->Inverse_H) * _20_To_float(M2->Inverse_C));
	tempMatrix.Inverse_C = float_To_20(_20_To_float(this->Inverse_C) * _20_To_float(M2->Inverse_A) + _20_To_float(this->Inverse_F) * _20_To_float(M2->Inverse_B) + _13_To_float(this->Inverse_I) * _20_To_float(M2->Inverse_C));
	tempMatrix.Inverse_D = float_To_20(_20_To_float(this->Inverse_A) * _20_To_float(M2->Inverse_D) + _20_To_float(this->Inverse_D) * _20_To_float(M2->Inverse_E) + _13_To_float(this->Inverse_G) * _20_To_float(M2->Inverse_F));
	tempMatrix.Inverse_E = float_To_20(_20_To_float(this->Inverse_B) * _20_To_float(M2->Inverse_D) + _20_To_float(this->Inverse_E) * _20_To_float(M2->Inverse_E) + _13_To_float(this->Inverse_H) * _20_To_float(M2->Inverse_F));
	tempMatrix.Inverse_F = float_To_20(_20_To_float(this->Inverse_C) * _20_To_float(M2->Inverse_D) + _20_To_float(this->Inverse_F) * _20_To_float(M2->Inverse_E) + _13_To_float(this->Inverse_I) * _20_To_float(M2->Inverse_F));
	tempMatrix.Inverse_G = float_To_13(_20_To_float(this->Inverse_A) * _13_To_float(M2->Inverse_G) + _20_To_float(this->Inverse_D) * _13_To_float(M2->Inverse_H) + _13_To_float(this->Inverse_G) * _13_To_float(M2->Inverse_I));
	tempMatrix.Inverse_H = float_To_13(_20_To_float(this->Inverse_B) * _13_To_float(M2->Inverse_G) + _20_To_float(this->Inverse_E) * _13_To_float(M2->Inverse_H) + _13_To_float(this->Inverse_H) * _13_To_float(M2->Inverse_I));
	tempMatrix.Inverse_I = float_To_13(_20_To_float(this->Inverse_C) * _13_To_float(M2->Inverse_G) + _20_To_float(this->Inverse_F) * _13_To_float(M2->Inverse_H) + _13_To_float(this->Inverse_I) * _13_To_float(M2->Inverse_I));
	#endif
#else
	#if (defined PC_SIM)
	tempMatrix.Inverse_A = (((long long)(this->Inverse_A)*(M2->Inverse_A)) + ((long long)(this->Inverse_D)*(M2->Inverse_B)) + (((long long)(this->Inverse_G)*(M2->Inverse_C))<< 7)>>20);
	tempMatrix.Inverse_B = (((long long)(this->Inverse_B)*(M2->Inverse_A)) + ((long long)(this->Inverse_E)*(M2->Inverse_B)) + (((long long)(this->Inverse_H)*(M2->Inverse_C))<< 7)>>20);
	tempMatrix.Inverse_C = (((long long)(this->Inverse_C)*(M2->Inverse_A)) + ((long long)(this->Inverse_F)*(M2->Inverse_B)) + (((long long)(this->Inverse_I)*(M2->Inverse_C))<< 7)>>20);
	tempMatrix.Inverse_D = (((long long)(this->Inverse_A)*(M2->Inverse_D)) + ((long long)(this->Inverse_D)*(M2->Inverse_E)) + (((long long)(this->Inverse_G)*(M2->Inverse_F))<< 7)>>20);
	tempMatrix.Inverse_E = (((long long)(this->Inverse_B)*(M2->Inverse_D)) + ((long long)(this->Inverse_E)*(M2->Inverse_E)) + (((long long)(this->Inverse_H)*(M2->Inverse_F))<< 7)>>20);
	tempMatrix.Inverse_F = (((long long)(this->Inverse_C)*(M2->Inverse_D)) + ((long long)(this->Inverse_F)*(M2->Inverse_E)) + (((long long)(this->Inverse_I)*(M2->Inverse_F))<< 7)>>20);
	tempMatrix.Inverse_G = (((long long)(this->Inverse_A)*(M2->Inverse_G)) + ((long long)(this->Inverse_D)*(M2->Inverse_H)) + (((long long)(this->Inverse_G)*(M2->Inverse_I))<< 7)>>20);
	tempMatrix.Inverse_H = (((long long)(this->Inverse_B)*(M2->Inverse_G)) + ((long long)(this->Inverse_E)*(M2->Inverse_H)) + (((long long)(this->Inverse_H)*(M2->Inverse_I))<< 7)>>20);
	#ifdef VERSION_2
	tempMatrix.Inverse_I = (((long long)(this->Inverse_C)*(M2->Inverse_G)) + ((long long)(this->Inverse_F)*(M2->Inverse_H)) + (((long long)(this->Inverse_I)*(M2->Inverse_I))<< 7)>>20);
	#endif
	#endif
	#if (defined VERSION_2) && (defined EMBEDDED)
//    tempMatrix.Inverse_A = csi_(this->Inverse_A, M2->Inverse_A) + csi_fix_mult20x20(this->Inverse_D, M2->Inverse_B) + csi_fix_mult13x20(this->Inverse_G, M2->Inverse_C);
//	tempMatrix.Inverse_B = csi_fix_mult20x20(this->Inverse_B, M2->Inverse_A) + csi_fix_mult20x20(this->Inverse_E, M2->Inverse_B) + csi_fix_mult13x20(this->Inverse_H, M2->Inverse_C);
//	tempMatrix.Inverse_C = csi_fix_mult20x20(this->Inverse_C, M2->Inverse_A) + csi_fix_mult20x20(this->Inverse_F, M2->Inverse_B) + csi_fix_mult13x20(this->Inverse_I, M2->Inverse_C);
//	tempMatrix.Inverse_D = csi_fix_mult20x20(this->Inverse_A, M2->Inverse_D) + csi_fix_mult20x20(this->Inverse_D, M2->Inverse_E) + csi_fix_mult13x20(this->Inverse_G, M2->Inverse_F);
//	tempMatrix.Inverse_E = csi_fix_mult20x20(this->Inverse_B, M2->Inverse_D) + csi_fix_mult20x20(this->Inverse_E, M2->Inverse_E) + csi_fix_mult13x20(this->Inverse_H, M2->Inverse_F);
//	tempMatrix.Inverse_F = csi_fix_mult20x20(this->Inverse_C, M2->Inverse_D) + csi_fix_mult20x20(this->Inverse_F, M2->Inverse_E) + csi_fix_mult13x20(this->Inverse_I, M2->Inverse_F);
//	tempMatrix.Inverse_G = csi_fix_mult20x13(this->Inverse_A, M2->Inverse_G) + csi_fix_mult20x13(this->Inverse_D, M2->Inverse_H) + csi_fix_mult13x13(this->Inverse_G, M2->Inverse_I);
//	tempMatrix.Inverse_H = csi_fix_mult20x13(this->Inverse_B, M2->Inverse_G) + csi_fix_mult20x13(this->Inverse_E, M2->Inverse_H) + csi_fix_mult13x13(this->Inverse_H, M2->Inverse_I);
//	tempMatrix.Inverse_I = csi_fix_mult20x13(this->Inverse_C, M2->Inverse_G) + csi_fix_mult20x13(this->Inverse_F, M2->Inverse_H) + csi_fix_mult13x13(this->Inverse_I, M2->Inverse_I);

	//   change by XT, 20200410
	csi_fix_mult_vec20x20(&(tempMatrix.Inverse_A),&(this->Inverse_A),M2->Inverse_A,0);
	csi_fix_mult_vec20x20(&(tempMatrix.Inverse_A),&(this->Inverse_D),M2->Inverse_B,1);
	csi_fix_mult_vec13x20(&(tempMatrix.Inverse_A),&(this->Inverse_G),M2->Inverse_C,1);


	csi_fix_mult_vec20x20(&(tempMatrix.Inverse_D),&(this->Inverse_A),M2->Inverse_D,0);
	csi_fix_mult_vec20x20(&(tempMatrix.Inverse_D),&(this->Inverse_D),M2->Inverse_E,1);
	csi_fix_mult_vec13x20(&(tempMatrix.Inverse_D),&(this->Inverse_G),M2->Inverse_F,1);

	
	csi_fix_mult_vec20x13(&(tempMatrix.Inverse_G),&(this->Inverse_A),M2->Inverse_G,0);
	csi_fix_mult_vec20x13(&(tempMatrix.Inverse_G),&(this->Inverse_D),M2->Inverse_H,1);
	csi_fix_mult_vec13x13(&(tempMatrix.Inverse_G),&(this->Inverse_G),M2->Inverse_I,1);	

	
	#endif
#endif
	this->copyFromInv(&tempMatrix);

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function name: TranslateRightMultiMatrix
// this(input):										M2:
//   | Inverse_A Inverse_D Inverse_G | 				   | 		 1 		   0 Inverse_g | 
//   | Inverse_B Inverse_E Inverse_H |       x   	   | 		 0 		   1 Inverse_h | 
//   | Inverse_C Inverse_F Inverse_I |          	   | 		 0 		   0 	   	 1 | 
// this(output):
// 	 | Inverse_A*1+Inverse_D*0+Inverse_G*0  Inverse_A*0+Inverse_D*1+Inverse_G*0  Inverse_A*Inverse_g+Inverse_D*Inverse_h+Inverse_G*1 | 
//   | Inverse_B*1+Inverse_E*0+Inverse_H*0  Inverse_B*0+Inverse_E*1+Inverse_H*0  Inverse_B*Inverse_g+Inverse_E*Inverse_h+Inverse_H*1 | 
//   | Inverse_C*1+Inverse_F*0+Inverse_I*0  Inverse_C*0+Inverse_F*1+Inverse_I*0  Inverse_C*Inverse_g+Inverse_F*Inverse_h+Inverse_I*1 | 
//-----------------------------
funcStatus NewMatrix::TranslateRightMultiMatrix(NewMatrix *M2)
{
	NewMatrix tempMatrix;
#ifdef UseFloatMatrix
	tempMatrix.Inverse_A = (this->Inverse_A);
	tempMatrix.Inverse_B = (this->Inverse_B);
	tempMatrix.Inverse_C = (this->Inverse_C);
	tempMatrix.Inverse_D = (this->Inverse_D);
	tempMatrix.Inverse_E = (this->Inverse_E);
	tempMatrix.Inverse_F = (this->Inverse_F);
	tempMatrix.Inverse_G = float_To_13(_20_To_float(this->Inverse_A)*_13_To_float(M2->Inverse_G) + _20_To_float(this->Inverse_D)*_13_To_float(M2->Inverse_H) + _13_To_float(this->Inverse_G));
	tempMatrix.Inverse_H = float_To_13(_20_To_float(this->Inverse_B)*_13_To_float(M2->Inverse_G) + _20_To_float(this->Inverse_E)*_13_To_float(M2->Inverse_H) + _13_To_float(this->Inverse_H));
	#ifdef VERSION_2
	tempMatrix.Inverse_I = float_To_13(_20_To_float(this->Inverse_C)*_13_To_float(M2->Inverse_G) + _20_To_float(this->Inverse_F)*_13_To_float(M2->Inverse_H) + _13_To_float(this->Inverse_I));
	#endif
#else
	#if (defined PC_SIM)
	tempMatrix.Inverse_A = (this->Inverse_A);
	tempMatrix.Inverse_B = (this->Inverse_B);
	tempMatrix.Inverse_C = (this->Inverse_C);
	tempMatrix.Inverse_D = (this->Inverse_D);
	tempMatrix.Inverse_E = (this->Inverse_E);
	tempMatrix.Inverse_F = (this->Inverse_F);
	tempMatrix.Inverse_G = (((long long)(this->Inverse_A)*(M2->Inverse_G)) + ((long long)(this->Inverse_D)*(M2->Inverse_H)) + ((long long)(this->Inverse_G) << 20) >> 20);
	tempMatrix.Inverse_H = (((long long)(this->Inverse_B)*(M2->Inverse_G)) + ((long long)(this->Inverse_E)*(M2->Inverse_H)) + ((long long)(this->Inverse_H) << 20) >> 20);
	#ifdef VERSION_2
	tempMatrix.Inverse_I = (((long long)(this->Inverse_C)*(M2->Inverse_G)) + ((long long)(this->Inverse_F)*(M2->Inverse_H)) + ((long long)(this->Inverse_I) << 20) >> 20);
	#endif
	#endif
	#if (defined VERSION_2) && (defined EMBEDDED)
	tempMatrix.Inverse_A = (this->Inverse_A);
	tempMatrix.Inverse_B = (this->Inverse_B);
	tempMatrix.Inverse_C = (this->Inverse_C);
	tempMatrix.Inverse_D = (this->Inverse_D);
	tempMatrix.Inverse_E = (this->Inverse_E);
	tempMatrix.Inverse_F = (this->Inverse_F);
	tempMatrix.Inverse_G = csi_fix_mult20x13(this->Inverse_A, M2->Inverse_G) + csi_fix_mult20x13(this->Inverse_D, M2->Inverse_H) + (this->Inverse_G);
	tempMatrix.Inverse_H = csi_fix_mult20x13(this->Inverse_B, M2->Inverse_G) + csi_fix_mult20x13(this->Inverse_E, M2->Inverse_H) + (this->Inverse_H);
	tempMatrix.Inverse_I = csi_fix_mult20x13(this->Inverse_C, M2->Inverse_G) + csi_fix_mult20x13(this->Inverse_F, M2->Inverse_H) + (this->Inverse_I);
	#endif
#endif
	this->copyFromInv(&tempMatrix);

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function name: RotateRightMultiMatrix
// this(input):										M2:
//   | Inverse_A Inverse_D Inverse_G | 				   | Inverse_a Inverse_d 		   0 | 
//   | Inverse_B Inverse_E Inverse_H |       x   	   | Inverse_b Inverse_e 		   0 | 
//   | Inverse_C Inverse_F Inverse_I |          	   | 		 0 		   0 	   	   1 | 
// this(output):
// 	 | Inverse_A*Inverse_a+Inverse_D*Inverse_b+Inverse_G*0  Inverse_A*Inverse_d+Inverse_D*Inverse_e+Inverse_G*0  Inverse_A*0+Inverse_D*0+Inverse_G*1 | 
//   | Inverse_B*Inverse_a+Inverse_E*Inverse_b+Inverse_H*0  Inverse_B*Inverse_d+Inverse_E*Inverse_e+Inverse_H*0  Inverse_B*0+Inverse_E*0+Inverse_H*1 | 
//   | Inverse_C*Inverse_a+Inverse_F*Inverse_b+Inverse_I*0  Inverse_C*Inverse_d+Inverse_F*Inverse_e+Inverse_I*0  Inverse_C*0+Inverse_F*0+Inverse_I*1 | 
//-----------------------------
funcStatus NewMatrix::RotateRightMultiMatrix(NewMatrix *M2)
{
	NewMatrix tempMatrix;
#ifdef UseFloatMatrix
	tempMatrix.Inverse_A = float_To_20(_20_To_float(this->Inverse_A)*_20_To_float(M2->Inverse_A) + _20_To_float(this->Inverse_D)*_20_To_float(M2->Inverse_B));
	tempMatrix.Inverse_B = float_To_20(_20_To_float(this->Inverse_B)*_20_To_float(M2->Inverse_A) + _20_To_float(this->Inverse_E)*_20_To_float(M2->Inverse_B));
	tempMatrix.Inverse_C = float_To_20(_20_To_float(this->Inverse_C)*_20_To_float(M2->Inverse_A) + _20_To_float(this->Inverse_F)*_20_To_float(M2->Inverse_B));
	tempMatrix.Inverse_D = float_To_20(_20_To_float(this->Inverse_A)*_20_To_float(M2->Inverse_D) + _20_To_float(this->Inverse_D)*_20_To_float(M2->Inverse_E));
	tempMatrix.Inverse_E = float_To_20(_20_To_float(this->Inverse_B)*_20_To_float(M2->Inverse_D) + _20_To_float(this->Inverse_E)*_20_To_float(M2->Inverse_E));
	tempMatrix.Inverse_F = float_To_20(_20_To_float(this->Inverse_C)*_20_To_float(M2->Inverse_D) + _20_To_float(this->Inverse_F)*_20_To_float(M2->Inverse_E));
	tempMatrix.Inverse_G = (this->Inverse_G);
	tempMatrix.Inverse_H = (this->Inverse_H);
	#ifdef VERSION_2
	tempMatrix.Inverse_I = (this->Inverse_I);
	#endif
#else
	#if (defined PC_SIM)
	tempMatrix.Inverse_A = (((long long)(this->Inverse_A)*(M2->Inverse_A))>>20) + (((long long)(this->Inverse_D)*(M2->Inverse_B))>>20);
	tempMatrix.Inverse_B = (((long long)(this->Inverse_B)*(M2->Inverse_A))>>20) + (((long long)(this->Inverse_E)*(M2->Inverse_B))>>20);
	tempMatrix.Inverse_C = (((long long)(this->Inverse_C)*(M2->Inverse_A))>>20) + (((long long)(this->Inverse_F)*(M2->Inverse_B))>>20);
	tempMatrix.Inverse_D = (((long long)(this->Inverse_A)*(M2->Inverse_D))>>20) + (((long long)(this->Inverse_D)*(M2->Inverse_E))>>20);
	tempMatrix.Inverse_E = (((long long)(this->Inverse_B)*(M2->Inverse_D))>>20) + (((long long)(this->Inverse_E)*(M2->Inverse_E))>>20);
	tempMatrix.Inverse_F = (((long long)(this->Inverse_C)*(M2->Inverse_D))>>20) + (((long long)(this->Inverse_F)*(M2->Inverse_E))>>20);
	tempMatrix.Inverse_G = (this->Inverse_G);
	tempMatrix.Inverse_H = (this->Inverse_H);
	#ifdef VERSION_2
	tempMatrix.Inverse_I = (this->Inverse_I);
	#endif
	#endif
	#if (defined VERSION_2) && (defined EMBEDDED)
//	tempMatrix.Inverse_A = csi_fix_mult20x20(this->Inverse_A, M2->Inverse_A) + csi_fix_mult20x20(this->Inverse_D, M2->Inverse_B);
//	tempMatrix.Inverse_B = csi_fix_mult20x20(this->Inverse_B, M2->Inverse_A) + csi_fix_mult20x20(this->Inverse_E, M2->Inverse_B);
//	tempMatrix.Inverse_C = csi_fix_mult20x20(this->Inverse_C, M2->Inverse_A) + csi_fix_mult20x20(this->Inverse_F, M2->Inverse_B);
//	tempMatrix.Inverse_D = csi_fix_mult20x20(this->Inverse_A, M2->Inverse_D) + csi_fix_mult20x20(this->Inverse_D, M2->Inverse_E);
//	tempMatrix.Inverse_E = csi_fix_mult20x20(this->Inverse_B, M2->Inverse_D) + csi_fix_mult20x20(this->Inverse_E, M2->Inverse_E);
//	tempMatrix.Inverse_F = csi_fix_mult20x20(this->Inverse_C, M2->Inverse_D) + csi_fix_mult20x20(this->Inverse_F, M2->Inverse_E);

	//   change by XT, 20200410
	csi_fix_mult_vec20x20(&(tempMatrix.Inverse_A),&(this->Inverse_A),M2->Inverse_A,0);
	csi_fix_mult_vec20x20(&(tempMatrix.Inverse_A),&(this->Inverse_D),M2->Inverse_B,1);
	csi_fix_mult_vec20x20(&(tempMatrix.Inverse_D),&(this->Inverse_A),M2->Inverse_D,0);
	csi_fix_mult_vec20x20(&(tempMatrix.Inverse_D),&(this->Inverse_D),M2->Inverse_E,1);

	tempMatrix.Inverse_G = (this->Inverse_G);
	tempMatrix.Inverse_H = (this->Inverse_H);
	tempMatrix.Inverse_I = (this->Inverse_I);
	#endif
#endif	
	this->copyFromInv(&tempMatrix);

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function name: ShearXRightMultiMatrix
// this(input):										M2:
//   | Inverse_A Inverse_D Inverse_G | 				   | 		 1 Inverse_d 		   0 | 
//   | Inverse_B Inverse_E Inverse_H |       x   	   | 		 0 		   1 		   0 | 
//   | Inverse_C Inverse_F Inverse_I |          	   | 		 0 		   0 	   	   1 | 
// this(output):
// 	 | Inverse_A*1+Inverse_D*0+Inverse_G*0  Inverse_A*Inverse_d+Inverse_D*1+Inverse_G*0  Inverse_A*0+Inverse_D*0+Inverse_G*1 | 
//   | Inverse_B*1+Inverse_E*0+Inverse_H*0  Inverse_B*Inverse_d+Inverse_E*1+Inverse_H*0  Inverse_B*0+Inverse_E*0+Inverse_H*1 | 
//   | Inverse_C*1+Inverse_F*0+Inverse_I*0  Inverse_C*Inverse_d+Inverse_F*1+Inverse_I*0  Inverse_C*0+Inverse_F*0+Inverse_I*1 | 
//-----------------------------
funcStatus NewMatrix::ShearXRightMultiMatrix(NewMatrix *M2)
{
	NewMatrix tempMatrix;
#ifdef UseFloatMatrix
	tempMatrix.Inverse_A = (this->Inverse_A);
	tempMatrix.Inverse_B = (this->Inverse_B);
	tempMatrix.Inverse_C = (this->Inverse_C);
	tempMatrix.Inverse_D = float_To_20(_20_To_float(this->Inverse_A)*_20_To_float(M2->Inverse_D) + _20_To_float(this->Inverse_D));
	tempMatrix.Inverse_E = float_To_20(_20_To_float(this->Inverse_B)*_20_To_float(M2->Inverse_D) + _20_To_float(this->Inverse_E));
	tempMatrix.Inverse_F = float_To_20(_20_To_float(this->Inverse_C)*_20_To_float(M2->Inverse_D) + _20_To_float(this->Inverse_F));
	tempMatrix.Inverse_G = (this->Inverse_G);
	tempMatrix.Inverse_H = (this->Inverse_H);
	#ifdef VERSION_2
	tempMatrix.Inverse_I = (this->Inverse_I);
	#endif
#else
	#if (defined PC_SIM)
	tempMatrix.Inverse_A = (this->Inverse_A);
	tempMatrix.Inverse_B = (this->Inverse_B);
	tempMatrix.Inverse_C = (this->Inverse_C);
	tempMatrix.Inverse_D = (((long long)(this->Inverse_A)*(M2->Inverse_D))>>20) + (this->Inverse_D);
	tempMatrix.Inverse_E = (((long long)(this->Inverse_B)*(M2->Inverse_D))>>20) + (this->Inverse_E);
	tempMatrix.Inverse_F = (((long long)(this->Inverse_C)*(M2->Inverse_D))>>20) + (this->Inverse_F);
	tempMatrix.Inverse_G = (this->Inverse_G);
	tempMatrix.Inverse_H = (this->Inverse_H);
	#ifdef VERSION_2
	tempMatrix.Inverse_I = (this->Inverse_I);
	#endif
	#endif
	#if (defined VERSION_2) && (defined EMBEDDED)
	tempMatrix.Inverse_A = (this->Inverse_A);
	tempMatrix.Inverse_B = (this->Inverse_B);
	tempMatrix.Inverse_C = (this->Inverse_C);
	tempMatrix.Inverse_D = csi_fix_mult20x20(this->Inverse_A, M2->Inverse_D) + (this->Inverse_D);
	tempMatrix.Inverse_E = csi_fix_mult20x20(this->Inverse_B, M2->Inverse_D) + (this->Inverse_E);
	tempMatrix.Inverse_F = csi_fix_mult20x20(this->Inverse_C, M2->Inverse_D) + (this->Inverse_F);
	tempMatrix.Inverse_G = (this->Inverse_G);
	tempMatrix.Inverse_H = (this->Inverse_H);
	tempMatrix.Inverse_I = (this->Inverse_I);
	#endif
#endif
	this->copyFromInv(&tempMatrix);

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function name: ShearYRightMultiMatrix
// this(input):										M2:
//   | Inverse_A Inverse_D Inverse_G | 				   | 		 1 		   0 		   0 | 
//   | Inverse_B Inverse_E Inverse_H |       x   	   | Inverse_b 		   1 		   0 | 
//   | Inverse_C Inverse_F Inverse_I |          	   | 		 0 		   0 	   	   1 | 
// this(output):
// 	 | Inverse_A*1+Inverse_D*Inverse_b+Inverse_G*0  Inverse_A*0+Inverse_D*1+Inverse_G*0  Inverse_A*0+Inverse_D*0+Inverse_G*1 | 
//   | Inverse_B*1+Inverse_E*Inverse_b+Inverse_H*0  Inverse_B*0+Inverse_E*1+Inverse_H*0  Inverse_B*0+Inverse_E*0+Inverse_H*1 | 
//   | Inverse_C*1+Inverse_F*Inverse_b+Inverse_I*0  Inverse_C*0+Inverse_F*1+Inverse_I*0  Inverse_C*0+Inverse_F*0+Inverse_I*1 | 
//-----------------------------
funcStatus NewMatrix::ShearYRightMultiMatrix(NewMatrix *M2)
{
	NewMatrix tempMatrix;
#ifdef UseFloatMatrix
	tempMatrix.Inverse_A = float_To_20(_20_To_float(this->Inverse_A) + _20_To_float(this->Inverse_D)*_20_To_float(M2->Inverse_B));
	tempMatrix.Inverse_B = float_To_20(_20_To_float(this->Inverse_B) + _20_To_float(this->Inverse_E)*_20_To_float(M2->Inverse_B));
	tempMatrix.Inverse_C = float_To_20(_20_To_float(this->Inverse_C) + _20_To_float(this->Inverse_F)*_20_To_float(M2->Inverse_B));
	tempMatrix.Inverse_D = (this->Inverse_D);
	tempMatrix.Inverse_E = (this->Inverse_E);
	tempMatrix.Inverse_F = (this->Inverse_F);
	tempMatrix.Inverse_G = (this->Inverse_G);
	tempMatrix.Inverse_H = (this->Inverse_H);
	#ifdef VERSION_2
	tempMatrix.Inverse_I = (this->Inverse_I);
	#endif
#else
	#if (defined PC_SIM)
	tempMatrix.Inverse_A = (this->Inverse_A) + (((long long)(this->Inverse_D)*(M2->Inverse_B))>>20);
	tempMatrix.Inverse_B = (this->Inverse_B) + (((long long)(this->Inverse_E)*(M2->Inverse_B))>>20);
	tempMatrix.Inverse_C = (this->Inverse_C) + (((long long)(this->Inverse_F)*(M2->Inverse_B))>>20);
	tempMatrix.Inverse_D = (this->Inverse_D);
	tempMatrix.Inverse_E = (this->Inverse_E);
	tempMatrix.Inverse_F = (this->Inverse_F);
	tempMatrix.Inverse_G = (this->Inverse_G);
	tempMatrix.Inverse_H = (this->Inverse_H);
	#ifdef VERSION_2
	tempMatrix.Inverse_I = (this->Inverse_I);
	#endif
	#endif
	#if (defined VERSION_2) && (defined EMBEDDED)
	tempMatrix.Inverse_A = (this->Inverse_A) + csi_fix_mult20x20(this->Inverse_D, M2->Inverse_B);
	tempMatrix.Inverse_B = (this->Inverse_B) + csi_fix_mult20x20(this->Inverse_E, M2->Inverse_B);
	tempMatrix.Inverse_C = (this->Inverse_C) + csi_fix_mult20x20(this->Inverse_F, M2->Inverse_B);
	tempMatrix.Inverse_D = (this->Inverse_D);
	tempMatrix.Inverse_E = (this->Inverse_E);
	tempMatrix.Inverse_F = (this->Inverse_F);
	tempMatrix.Inverse_G = (this->Inverse_G);
	tempMatrix.Inverse_H = (this->Inverse_H);
	tempMatrix.Inverse_I = (this->Inverse_I);
	#endif
#endif
	
	this->copyFromInv(&tempMatrix);

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function name: ScaleRightMultiMatrix
// this(input):										M2:
//   | Inverse_A Inverse_D Inverse_G | 				   | Inverse_a 		   0 		   0 | 
//   | Inverse_B Inverse_E Inverse_H |       x   	   | 		 0 Inverse_e 		   0 | 
//   | Inverse_C Inverse_F Inverse_I |          	   | 		 0 		   0 	   	   1 | 
// this(output):
// 	 | Inverse_A*Inverse_a+Inverse_D*0+Inverse_G*0  Inverse_A*0+Inverse_D*Inverse_e+Inverse_G*0  Inverse_A*0+Inverse_D*0+Inverse_G*1 | 
//   | Inverse_B*Inverse_a+Inverse_E*0+Inverse_H*0  Inverse_B*0+Inverse_E*Inverse_e+Inverse_H*0  Inverse_B*0+Inverse_E*0+Inverse_H*1 | 
//   | Inverse_C*Inverse_a+Inverse_F*0+Inverse_I*0  Inverse_C*0+Inverse_F*Inverse_e+Inverse_I*0  Inverse_C*0+Inverse_F*0+Inverse_I*1 | 
//-----------------------------
funcStatus NewMatrix::ScaleRightMultiMatrix(NewMatrix *M2)
{
	NewMatrix tempMatrix;
#ifdef UseFloatMatrix
	tempMatrix.Inverse_A = float_To_20(_20_To_float(this->Inverse_A)*_20_To_float(M2->Inverse_A));
	tempMatrix.Inverse_B = float_To_20(_20_To_float(this->Inverse_B)*_20_To_float(M2->Inverse_A));
	tempMatrix.Inverse_C = float_To_20(_20_To_float(this->Inverse_C)*_20_To_float(M2->Inverse_A));
	tempMatrix.Inverse_D = float_To_20(_20_To_float(this->Inverse_D)*_20_To_float(M2->Inverse_E));
	tempMatrix.Inverse_E = float_To_20(_20_To_float(this->Inverse_E)*_20_To_float(M2->Inverse_E));
	tempMatrix.Inverse_F = float_To_20(_20_To_float(this->Inverse_F)*_20_To_float(M2->Inverse_E));
	tempMatrix.Inverse_G = (this->Inverse_G);
	tempMatrix.Inverse_H = (this->Inverse_H);
	#ifdef VERSION_2
	tempMatrix.Inverse_I = (this->Inverse_I);
	#endif
#else
	#if (defined PC_SIM)
	tempMatrix.Inverse_A = (((long long)(this->Inverse_A)*(M2->Inverse_A))>>20);
	tempMatrix.Inverse_B = (((long long)(this->Inverse_B)*(M2->Inverse_A))>>20);
	tempMatrix.Inverse_C = (((long long)(this->Inverse_C)*(M2->Inverse_A))>>20);
	tempMatrix.Inverse_D = (((long long)(this->Inverse_D)*(M2->Inverse_E))>>20);
	tempMatrix.Inverse_E = (((long long)(this->Inverse_E)*(M2->Inverse_E))>>20);
	tempMatrix.Inverse_F = (((long long)(this->Inverse_F)*(M2->Inverse_E))>>20);
	tempMatrix.Inverse_G = (this->Inverse_G);
	tempMatrix.Inverse_H = (this->Inverse_H);
	#ifdef VERSION_2
	tempMatrix.Inverse_I = (this->Inverse_I);
	#endif
	#endif
	#if (defined VERSION_2) && (defined EMBEDDED)
	tempMatrix.Inverse_A = csi_fix_mult20x20(this->Inverse_A, M2->Inverse_A);
	tempMatrix.Inverse_B = csi_fix_mult20x20(this->Inverse_B, M2->Inverse_A);
	tempMatrix.Inverse_C = csi_fix_mult20x20(this->Inverse_C, M2->Inverse_A);
	tempMatrix.Inverse_D = csi_fix_mult20x20(this->Inverse_D, M2->Inverse_E);
	tempMatrix.Inverse_E = csi_fix_mult20x20(this->Inverse_E, M2->Inverse_E);
	tempMatrix.Inverse_F = csi_fix_mult20x20(this->Inverse_F, M2->Inverse_E);
	tempMatrix.Inverse_G = (this->Inverse_G);
	tempMatrix.Inverse_H = (this->Inverse_H);
	tempMatrix.Inverse_I = (this->Inverse_I);
	#endif
#endif
	this->copyFromInv(&tempMatrix);

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function name: TranslateToMatrix
// Set translation matrix
// param list:
//   @param1 s32 MovingX 1.18.13
//   @param2 s32 MovingY 1.18.13
// Remarks(Revision between versions):
//   created by yn 2018.10.15
//   change by XT, 20200410
//-----------------------------
funcStatus NewMatrix::TranslateToMatrix(s32 MovingX, s32 MovingY)
{
	//| 1 0 MovingX |
	//| 0 1 MovingY |
	//| 0 0    1	|
	this->A = 1 << 20;
	this->B = 0;
	this->C = 0;
	this->D = 0;
	this->E = 1 << 20;
	this->F = 0;
	this->G = MovingX;
	this->H = MovingY;
	#ifdef VERSION_2
	this->I = 1 << 13;
	#endif

	//| 1 0 -MovingX |
	//| 0 1 -MovingY |
	//| 0 0    1	 |
	this->Inverse_A = 1 << 20;
	this->Inverse_B = 0;
	this->Inverse_C = 0;
	this->Inverse_D = 0;
	this->Inverse_E = 1 << 20;
	this->Inverse_F = 0;
	this->Inverse_G = -MovingX;
	this->Inverse_H = -MovingY;
	#ifdef VERSION_2
	this->Inverse_I = 1 << 13;
	#endif

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function name: RotateToMatrix
// Set Rotation matrix
// param list:
//   @param1 s16 RotateAngle 1.11.4
// Remarks(Revision between versions):
//   created by yn 2018.10.15
//   change by XT, 20200410
//-----------------------------
funcStatus NewMatrix::RotateToMatrix(s16 RotateAngle)
{
	myMathClass mymath;
	//The value of COS is expanded by 1024*1024 times.
	s32 cosDegree = 1024*1024, sinDegree = 0;

	#ifdef Use_CORDIC
	mymath.CORDIC_32(RotateAngle, &cosDegree, &sinDegree);
	#endif

	#ifdef Use_SinTanTable
	mymath.SinTable(RotateAngle, &cosDegree, &sinDegree);
	#endif

	// | A C 0 |   | cos -sin 0 |
	// | B D 0 | = | sin  cos 0 | 
	// | 0 0 1 |   |  0	   0  1 |
	this->A = cosDegree;
	this->B = sinDegree;
//	this->C = 0;
	this->D = -sinDegree;
	this->E = cosDegree;
//	this->F = 0;
//	this->G = 0;
//	this->H = 0;
	#ifdef VERSION_2
	this->I = 1 << 13;
	#endif

	// | A C 0 |-1   | cos  sin 0 |
	// | B D 0 |   = | -sin cos 0 | 
	// | 0 0 1 |     |  0    0  1 |
	this->Inverse_A = cosDegree;
	this->Inverse_B = -sinDegree;
//	this->Inverse_C = 0;
	this->Inverse_D = sinDegree;
	this->Inverse_E = cosDegree;
//	this->Inverse_F = 0;
//	this->Inverse_G = 0;
	this->Inverse_H = 0;
	#ifdef VERSION_2
//	this->Inverse_I = 1 << 13;
	#endif

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function name: ShearXToMatrix
// Set ShearX matrix
// param list:
//   @param1 s16 ShearAngleX 1.11.4
// Remarks(Revision between versions):
//   created by yn 2018.10.15
//   change by XT, 20200410
//-----------------------------
funcStatus NewMatrix::ShearXToMatrix(s16 ShearAngleX)
{
	myMathClass mymath;
	s32 shearTanX = 0;
	//The value of COS is expanded by 1024 times.
	s16 cos = 1024, sin = 0;
	
	#ifdef Use_CORDIC
	mymath.CORDIC(ShearAngleX, &cos, &sin);
	if(cos == 0) shearTanX = -32768;//Negative maximum
	else shearTanX = 512 * sin /cos ;// 1.6.9 transform 1.11.20 (Left shift 11bit)
	#endif

	#ifdef Use_SinTanTable
	mymath.TanTable(ShearAngleX,&shearTanX);
	#endif

	// | 1  shearTanX  0 |
	// | 0    1		   0 |
	// | 0    0		   1 |
//	this->A = 1 << 20;
//	this->B = 0;
//	this->C = 0;
	this->D = (shearTanX << 11);
//	this->E = 1 << 20;
//	this->F = 0;
//	this->G = 0;
//	this->H = 0;
	#ifdef VERSION_2
//	this->I = 1 << 13;
	#endif

	// | 1  -shearTanX  0 |
	// | 0    1			0 |
	// | 0    0			1 |
//	this->Inverse_A = 1 << 20;
//	this->Inverse_B = 0;
//	this->Inverse_C = 0;
	this->Inverse_D = -(shearTanX << 11);
//	this->Inverse_E = 1 << 20;
//	this->Inverse_F = 0;
//	this->Inverse_G = 0;
//	this->Inverse_H = 0;
	#ifdef VERSION_2
//	this->Inverse_I = 1 << 13;
	#endif

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// funtion name: ShearYToMatrix
// Set ShearY matrix
// param list:
//   @param1 s16 ShearAngleY  1.11.4
// Remarks(Revision between versions):
//   created by yn 2018.10.15
//   change by XT, 20200410
//-----------------------------
funcStatus NewMatrix::ShearYToMatrix(s16 ShearAngleY)
{
	myMathClass mymath;
	s32 shearTanY = 0;
	//The value of COS is expanded by 1024 times.
	s16 cos = 1024, sin = 0;
	
	#ifdef Use_CORDIC
	mymath.CORDIC(ShearAngleY, &cos, &sin);
	if(cos == 0) shearTanY = -32768;//Negative maximum
	else shearTanY = 512 * sin /cos ;// 1.6.9 transform 1.11.20 (Left shift 11bit)
	#endif

	#ifdef Use_SinTanTable
	mymath.TanTable(ShearAngleY,&shearTanY);
	#endif

	// | 1			  0  0 |
	// | shearTanY	  1  0 |
	// | 0			  0  1 |
//	this->A = 1 << 20;
	this->B = (shearTanY << 11);
//	this->C = 0;
//	this->D = 0;
//	this->E = 1 << 20;
//	this->F = 0;
//	this->G = 0;
//	this->H = 0;
	#ifdef VERSION_2
//	this->I = 1 << 13;
	#endif

	// | 1			   0  0 |
	// | -shearTanY    1  0 |
	// | 0			   0  1 |
//	this->Inverse_A = 1 << 20;
	this->Inverse_B = -(shearTanY << 11);
//	this->Inverse_C = 0;
//	this->Inverse_D = 0;
//	this->Inverse_E = 1 << 20;
//	this->Inverse_F = 0;
//	this->Inverse_G = 0;
//	this->Inverse_H = 0;
	#ifdef VERSION_2
//	this->Inverse_I = 1 << 13;
	#endif

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function name: ScalepToMatrix
// Set scalep matrix in project
// param list:
//   @param1 s32 Width 
//   @param2 s32 Height 
// Remarks(Revision between versions):
//   created by yn 2020.10.29
//-----------------------------
funcStatus NewMatrix::ScalepToMatrix(s32 Width, s32 Height)
{
	//| Width   0		0 |
	//| 0		Height  0 |
	//| 0       0		1 |
	this->A = Width << 20;
//	this->B = 0;
//	this->C = 0;
//	this->D = 0;
	this->E = Height << 20;
//	this->F = 0;
//	this->G = 0;
//	this->H = 0;
	#ifdef VERSION_2
//	this->I = 1 << 13;
	#endif

	//| 1/Width    0		0 |
	//| 0		   1/Height 0 |
	//| 0          0		1 |
	this->Inverse_A = (1 << 20) / Width;
//	this->Inverse_B = 0;
//	this->Inverse_C = 0;
//	this->Inverse_D = 0;
	this->Inverse_E = (1 << 20) / Height;
//	this->Inverse_F = 0;
//	this->Inverse_G = 0;
//	this->Inverse_H = 0;
	#ifdef VERSION_2
//	this->Inverse_I = 1 << 13;
	#endif

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function name: ScalenToMatrix
// Set scalen matrix in project
// param list:
//   @param1 s32 Width 
//   @param2 s32 Height 
// Remarks(Revision between versions):
//   created by yn 2020.10.29
//-----------------------------
funcStatus NewMatrix::ScalenToMatrix(s32 Width, s32 Height)
{
	//| 1/Width    0		0 |
	//| 0		   1/Height 0 |
	//| 0          0		1 |
	this->A = (1 << 20) / Width;
//	this->B = 0;
//	this->C = 0;
//	this->D = 0;
	this->E = (1 << 20) / Height;
//	this->F = 0;
//	this->G = 0;
//	this->H = 0;
	#ifdef VERSION_2
//	this->I = 1 << 13;
	#endif

	//| Width   0		0 |
	//| 0		Height  0 |
	//| 0       0		1 |
	this->Inverse_A = Width << 20;
//	this->Inverse_B = 0;
//	this->Inverse_C = 0;
//	this->Inverse_D = 0;
	this->Inverse_E = Height << 20;
//	this->Inverse_F = 0;
//	this->Inverse_G = 0;
//	this->Inverse_H = 0;
	#ifdef VERSION_2
//	this->Inverse_I = 1 << 13;
	#endif

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function name: RotateZToMatrix
// Set Rotation Z matrix
// param list:
//   @param1 s16 degree 1.11.4
// Remarks(Revision between versions):
//   created by yn 2020.10.29
//-----------------------------
funcStatus NewMatrix::RotateZToMatrix(s16 RotateAngle)
{
	myMathClass mymath;
	//The value of COS is expanded by 1024*1024 times.
	s32 cosDegree = 1024*1024, sinDegree = 0;
	
	//Set Rotation Z matrix to max
	if(RotateAngle == 90 * 16)
	{
		this->A = 0x7FFFFFFF;
		this->B = 0;
		this->C = 0;
		this->D = 0;
		this->E = (1 << 20);
		this->F = 0x80000000;
		this->G = 0;
		this->H = 0;
		#ifdef VERSION_2
		this->I = 0x7FFFFFFF;
		#endif

		this->Inverse_A = 1;
		this->Inverse_B = 0;
		this->Inverse_C = 0;
		this->Inverse_D = 0;
		this->Inverse_E = (1 << 20);
		this->Inverse_F = (1 << 20);
		this->Inverse_G = 0;
		this->Inverse_H = 0;
		#ifdef VERSION_2
		this->Inverse_I = 1;
		#endif

		return AHMI_FUNC_SUCCESS;
	}
	//Set Rotation Z matrix to min
	else if(RotateAngle == -90 * 16)
	{
		this->A = 0x7FFFFFFF;
		this->B = 0;
		this->C = 0;
		this->D = 0;
		this->E = (1 << 20);
		this->F = 0x7FFFFFFF;
		this->G = 0;
		this->H = 0;
		#ifdef VERSION_2
		this->I = 0x7FFFFFFF;
		#endif

		this->Inverse_A = 1;
		this->Inverse_B = 0;
		this->Inverse_C = 0;
		this->Inverse_D = 0;
		this->Inverse_E = (1 << 20);
		this->Inverse_F = (-1 << 20);
		this->Inverse_G = 0;
		this->Inverse_H = 0;
		#ifdef VERSION_2
		this->Inverse_I = 1;
		#endif

		return AHMI_FUNC_SUCCESS;
	}

	#ifdef Use_CORDIC
	mymath.CORDIC_32(RotateAngle, &cosDegree, &sinDegree);
	#endif

	#ifdef Use_SinTanTable
	mymath.SinTable(RotateAngle, &cosDegree, &sinDegree);
	#endif

	//set cosDegree to min
	if(cosDegree == 0)
	{
		cosDegree = 1;
	}

	// | A D G |   | 1/cos 0         0     |
	// | B E H | = | 0     1		 0     | 
	// | C F I |   | 0	   -sin/cos  1/cos |
	this->A = (long long)(((long long)1 << 40) / cosDegree);
	this->B = 0;
	this->C = 0;
	this->D = 0;
	this->E = (1 << 20);
	this->F = (long long)(((long long)(-sinDegree) << 20) / cosDegree);
	this->G = 0;
	this->H = 0;
	#ifdef VERSION_2
	this->I = (long long)(((long long)1 << 33) / cosDegree);
	#endif

	// | A D G |   | cos   0    0   |
	// | B E H | = | 0     1	0   | 
	// | C F I |   | 0	   sin  cos |
	this->Inverse_A = (cosDegree);
	this->Inverse_B = 0;
	this->Inverse_C = 0;
	this->Inverse_D = 0;
	this->Inverse_E = (1 << 20);
	this->Inverse_F = (sinDegree);
	this->Inverse_G = 0;
	this->Inverse_H = 0;
	#ifdef VERSION_2
	this->Inverse_I = (cosDegree >> 7);
	#endif

	return AHMI_FUNC_SUCCESS;
}

/*//-----------------------------
// funtion name: ShearZToMatrix
// Set Shear matrix
// param list:
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
//-----------------------------*/
funcStatus NewMatrix::ShearZToMatrix(s16 ShearAngleZX, s16 ShearAngleZY,int TexWidth, int TexHeight)
{
	myMathClass mymath;
	int64_t temp = 0;
	//The value of COS is expanded by 1024 times.
	s32 shearTanX = 0,shearTanY = 0,Dx,Dy;
	s16 cos_degreeX = 1024, sin_degreeX = 0, cos_degreeY = 1024, sin_degreeY = 0;
	
	Dx = ShearAngleZX;
	//set shearTanX to min
	if( Dx == (0 << 4))
	{
		shearTanX = 0;
	}
	else 
	{
		mymath.CORDIC(Dx, &cos_degreeX, &sin_degreeX);
		if(cos_degreeX == 0) shearTanX = -32768;//Negative maximum
		else shearTanX = 512 * sin_degreeX /cos_degreeX ;// 1.6.9 transform 1.11.20 (Left shift 11bit)
	}

	Dy = ShearAngleZY;
	//set shearTanY to min
	if( Dy == (0 << 4))
	{
		shearTanY = 0;
	}
	else 
	{
		mymath.CORDIC(Dy, &cos_degreeY, &sin_degreeY);
		if(cos_degreeY == 0) shearTanY = -32768;//Negative maximum
		else shearTanY = 512 * sin_degreeY /cos_degreeY ;// 1.6.9 transform 1.11.20 (Left shift 11bit)
	}

	//set cos_degreeX to min
	if(cos_degreeX == 0)
	{
		cos_degreeX = 1;
	}

	//set cos_degreeY to min
	if(cos_degreeY == 0)
	{
		cos_degreeY = 1;
	}

	#if (defined VERSION_2) && (defined EMBEDDED)
	
	this->A = (long long)(cos_degreeX << 10);
	this->B = 0;
	temp = (-(int64_t)sin_degreeX) * 1024 ;
	this->C = csi_fix_div64((temp >> 32), temp ,TexWidth);
	this->D = 0;
	this->E = (long long)(cos_degreeY << 10);
	temp = (-(int64_t)sin_degreeY) * 1024 ;
	this->F = csi_fix_div64((temp >> 32), temp ,TexHeight);
	this->G = 0;
	this->H = 0;
	#ifdef VERSION_2
	this->I = (1 << 13);
	#endif

	this->Inverse_A = csi_fix_div(1, cos_degreeX, 30);
	this->Inverse_B = 0;
	temp = ((int64_t)shearTanX) * 2048 ;
	this->Inverse_C = csi_fix_div64((temp >> 32), temp ,TexWidth);
	this->Inverse_D = 0;
	this->Inverse_E = csi_fix_div(1, cos_degreeY, 30);
	temp = ((int64_t)shearTanY) * 2048 ;
	this->Inverse_F = csi_fix_div64((temp >> 32), temp ,TexHeight);
	this->Inverse_G = 0;
	this->Inverse_H = 0;
	#ifdef VERSION_2
	this->Inverse_I = (1 << 13);
	#endif
	
	#endif

	#ifdef PC_SIM
	
	this->A = (long long)(cos_degreeX << 10);
	this->B = 0;
	this->C = (long long)((long long)(-sin_degreeX << 10) / TexWidth);
	this->D = 0;
	this->E = (long long)(cos_degreeY << 10);
	this->F = (long long)((long long)(-sin_degreeY << 10) / TexHeight);
	this->G = 0;
	this->H = 0;
	#ifdef VERSION_2
	this->I = (1 << 13);
	#endif

	this->Inverse_A = (long long)((1 << 30) / cos_degreeX);
	this->Inverse_B = 0;
	this->Inverse_C = ((long long)(shearTanX << 11) / TexWidth);
	this->Inverse_D = 0;
	this->Inverse_E = (long long)((1 << 30) / cos_degreeY);
	this->Inverse_F = ((long long)(shearTanY << 11) / TexHeight);
	this->Inverse_G = 0;
	this->Inverse_H = 0;
	#ifdef VERSION_2
	this->Inverse_I = (1 << 13);
	#endif
    
    #endif
	////for shearZ only
	//this->A = (1 << 20);
	//this->B = 0;
	//this->C = (long long)(-shearTanX << 11) / TexWidth;
	//this->D = 0;
	//this->E = (1 << 20);
	//this->F = (long long)(-shearTanY << 11) / TexHeight;
	//this->G = 0;
	//this->H = 0;
	//this->I = (1 << 13);

	//this->Inverse_A = (1 << 20);
	//this->Inverse_B = 0;
	//this->Inverse_C = (long long)(shearTanX << 11) / TexWidth;
	//this->Inverse_D = 0;
	//this->Inverse_E = (1 << 20);
	//this->Inverse_F = (long long)(shearTanY << 11) / TexHeight;
	//this->Inverse_G = 0;
	//this->Inverse_H = 0;
	//this->Inverse_I = (1 << 13);

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function name: ScaleToMatrix
// Set Scale matrix
// param list:
//   @param1 s32 ScaleX : the scaler of x  1.6.9
//   @param2 s32 ScaleY : the scaler of y  1.6.9
// Remarks(Revision between versions):
//   created by yn 2018.10.15
//   change by XT, 20200410
//-----------------------------
funcStatus NewMatrix::ScaleToMatrix(s16 ScaleX, s16 ScaleY)
{

	// | Sx  0   0 |
	// | 0   Sy  0 |
	// | 0   0   1 |
	this->A = ScaleX << 11;//TODO:change into 1.11.20 (Left shift 11bit)
//	this->B = 0;
//	this->C = 0;
//	this->D = 0;
	this->E = ScaleY << 11;
//	this->F = 0;
//	this->G = 0;
//	this->H = 0;
	#ifdef VERSION_2
//	this->I = 1 << 13;
	#endif

	// | x/S  0   0 |
	// | 0   y/S  0 |
	// | 0    0   1 |
	#if (defined PC_SIM)
	this->Inverse_A = (1 << 29) / ScaleX;//change into 1.11.20, (1<<29)/scalerx
	#endif
	#if (defined VERSION_2) && (defined EMBEDDED)
	this->Inverse_A = csi_fix_div(1, ScaleX, 29);//change into 1.11.20, (1<<29)/scalerx
	#endif
//	this->Inverse_B = 0;
//	this->Inverse_C = 0;
//	this->Inverse_D = 0;
	#if (defined PC_SIM)
	this->Inverse_E = (1 << 29) / ScaleY;
	#endif
	#if (defined VERSION_2) && (defined EMBEDDED)
	this->Inverse_E = csi_fix_div(1, ScaleY, 29);
	#endif
//	this->Inverse_F = 0;
//	this->Inverse_G = 0;
//	this->Inverse_H = 0;
	#ifdef VERSION_2
//	this->Inverse_I = 1 << 13;
	#endif

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function name: translate
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
funcStatus NewMatrix::translate(s32 x, s32 y)
{
	NewMatrix matrixMoving;
	if((x==0) && (y==0) ) return AHMI_FUNC_SUCCESS; //add by XT, 20200410
	//Positive matrix operations (left multiplication)
	matrixMoving.TranslateToMatrix(x,y);
	this->TranslateLeftMultiMatrix(&matrixMoving);

	//Inverse matrix operation (right multiplication)
	//matrixMoving.copyInv(&matrixMovingInv);
	//this->RightMultiMatrix(&matrixMovingInv);

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function name: scale
// scale matrix
// param list:
//   @param1 s16 x 1.6.9
//   @param2 s16 y 1.6.9
// Remarks(Revision between versions):
//		| S 0 0 |	| A D G |
//		| 0 S 0 | *	| B E H |
//		| 0 0 1 |	| 0 0 1 |
//   created by yn 2018.10.16
//-----------------------------
funcStatus NewMatrix::scale(s16 x, s16 y)
{
	NewMatrix matrixScale;

	if((x == (1<<9)) && (y == (1<<9))) return AHMI_FUNC_SUCCESS; //add by XT, 20200410
	//Positive matrix operations (left multiplication)
	matrixScale.ScaleToMatrix(x,y);
	this->ScaleLeftMultiMatrix(&matrixScale);

	//Inverse matrix operation (right multiplication)
	//matrixScale.copyInv(&matrixScaleInv);
	//this->RightMultiMatrix(&matrixScaleInv);

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function name: rotate
// rotation matrix
// param list:
//   @param1 s16 degree 1.11.4
// Remarks(Revision between versions):
//		this = rotate * this
//			created by yn 2018.10.16
//-----------------------------
funcStatus NewMatrix::rotate(s16 degree)//1.11.4
{
	NewMatrix matrixRotate;

	if(degree == 0) return AHMI_FUNC_SUCCESS; //add by XT, 20200410
	
	//Positive matrix operations (left multiplication)
    matrixRotate.RotateToMatrix(degree);
	this->RotateLeftMultiMatrix(&matrixRotate);

	//Inverse matrix operation (right multiplication)
	//matrixRotate.copyInv(&matrixRotateInv);
	//this->RightMultiMatrix(&matrixRotateInv);

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function name: shear
// shear matrix
// param list:
//   @param1 s16 degreeX 1.11.4
//   @param2 s16 degreeY 1.11.4
// Remarks(Revision between versions):
//		this = shear * this
//			created by yn 2018.10.16
//-----------------------------
funcStatus NewMatrix::shear(s16 degreeX, s16 degreeY)//1.11.4
{
	NewMatrix matrixShearX, matrixShearY;

	if((degreeX == 0) && (degreeY == 0)) return AHMI_FUNC_SUCCESS; //add by XT, 20200410
	//Positive matrix operations (left multiplication)
	matrixShearX.ShearXToMatrix(degreeX);
	this->ShearXLeftMultiMatrix(&matrixShearX);
	matrixShearY.ShearYToMatrix(degreeY);
	this->ShearYLeftMultiMatrix(&matrixShearY);

	//Inverse matrix operation (right multiplication)
	//matrixShearX.copyInv(&matrixShearXInv);
	//this->RightMultiMatrix(&matrixShearXInv);
	//matrixShearY.copyInv(&matrixShearYInv);
	//this->RightMultiMatrix(&matrixShearYInv);

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function name: project
// project matrix, used by canvas or widget transformation
// param list:
//   @param1 s16 degreeX 1.11.4
//   @param2 s16 degreeY 1.11.4
//   @param3 int TexWidth 
//   @param4 int TexHeight
// Remarks(Revision between versions):
//		this = project * this
//			created by yn 2019.02.26
//-----------------------------
funcStatus NewMatrix::project(s16 degreeX, s16 degreeY, int TexWidth, int TexHeight)
{
	NewMatrix matrixProject,matrix_movetozero,matrix_return,matrix_scalen,matrix_scalep,matrix_ropn,matrix_ropp;
	myMathClass mymath;
	s32 Dx,Dy,Dalpha = 0, Dtheta = 0, temp = 0;

	if(TexWidth != 0 && TexHeight != 0)
	{
		//Equivalent z-axis rotation algorithm
		if((degreeX != 0) && (degreeY != 0))
		{
			Dx = degreeX;
			Dy = degreeY;

			temp = (long long)(((long long)Dx << 20) / Dy);
			Dalpha = mymath.GetAngleFromTan(temp);

			if(Dx < 0)
				Dx = -Dx;

			if(Dy < 0)
				Dy = -Dy;

			if(Dx >= Dy)
			{
				Dtheta = Dx;
			}
			else
			{
				Dtheta = Dy;
			}

			if(degreeY < 0)
				Dtheta = -Dtheta;

			if(Dtheta > 90 * 16)
			{
				while((Dtheta > 90 * 16) || (Dtheta < -90 * 16))
				{
					Dtheta -= 180 * 16;
				}
			}
			if(Dtheta < -90 * 16)
			{
				while((Dtheta > 90 * 16) || (Dtheta < -90 * 16))
				{
					Dtheta += 180 * 16;
				}
			}

			matrixProject.RotateZToMatrix(Dtheta);
		}
		//Unidirectional z-axis projection
		else
		{
			//Angle normalized from -90 to 90
			if(degreeX > 90 * 16)
			{
				while((degreeX > 90 * 16) || (degreeX < -90 * 16))
				{
					degreeX -= 180 * 16;
				}
			}

			if(degreeX < -90 * 16)
			{
				while((degreeX > 90 * 16) || (degreeX < -90 * 16))
				{
					degreeX += 180 * 16;
				}
			}

			if(degreeY > 90 * 16)
			{
				while((degreeY > 90 * 16) || (degreeY < -90 * 16))
				{
					degreeY -= 180 * 16;
				}
			}

			if(degreeY < -90 * 16)
			{
				while((degreeY > 90 * 16) || (degreeY < -90 * 16))
				{
					degreeY += 180 * 16;
				}
			}

			//Making z-projection transformation matrix with larger edges
			if(TexWidth >= TexHeight)
			{
				matrixProject.ShearZToMatrix(degreeX,degreeY,TexWidth,TexWidth);
			}
			else
			{
				matrixProject.ShearZToMatrix(degreeX,degreeY,TexHeight,TexHeight);
			}
		}
	}
	else
	{
		matrixProject.NewMatrix_init();
		matrixProject.NewMatrixInv_init();
	}

	//Calculate the preprocessing transformation matrix
	if((degreeX == 0 * 16) && (degreeY == 0 * 16))
	{
		matrix_movetozero.NewMatrix_init();
		matrix_movetozero.NewMatrixInv_init();
		matrix_return.NewMatrix_init();
		matrix_return.NewMatrixInv_init();
		matrix_ropn.NewMatrix_init();
		matrix_ropn.NewMatrixInv_init();
		matrix_ropp.NewMatrix_init();
		matrix_ropp.NewMatrixInv_init();
		matrix_scalen.NewMatrix_init();
		matrix_scalen.NewMatrixInv_init();
		matrix_scalep.NewMatrix_init();
		matrix_scalep.NewMatrixInv_init();
	}
	else
	{
		matrix_movetozero.NewMatrix_init();
		matrix_movetozero.NewMatrixInv_init();
		matrix_return.NewMatrix_init();
		matrix_return.NewMatrixInv_init();
		matrix_ropn.NewMatrix_init();
		matrix_ropn.NewMatrixInv_init();
		matrix_ropp.NewMatrix_init();
		matrix_ropp.NewMatrixInv_init();
		matrix_scalen.NewMatrix_init();
		matrix_scalen.NewMatrixInv_init();
		matrix_scalep.NewMatrix_init();
		matrix_scalep.NewMatrixInv_init();

		if((degreeX != 0) && (degreeY != 0))
		{
			if(TexWidth >= TexHeight)
			{
				matrix_scalen.ScalenToMatrix(TexWidth,TexWidth);
				matrix_scalep.ScalepToMatrix(TexWidth,TexWidth);
			}
			else
			{
				matrix_scalen.ScalenToMatrix(TexHeight,TexHeight);
				matrix_scalep.ScalepToMatrix(TexHeight,TexHeight);
			}

			matrix_ropn.RotateToMatrix(Dalpha);
			matrix_ropp.RotateToMatrix(-Dalpha);
		}

		matrix_movetozero.TranslateToMatrix(-((TexWidth/2) << 13), -((TexHeight/2) << 13));
		matrix_return.TranslateToMatrix(((TexWidth/2) << 13), ((TexHeight/2) << 13));
	}

	//Calculate Z projection transformation matrix
	this->LeftMultiMatrix(&matrix_movetozero);
	if((degreeX != 0) && (degreeY != 0))
	{
		this->LeftMultiMatrix(&matrix_scalen);
		this->LeftMultiMatrix(&matrix_ropn);
	}
	this->LeftMultiMatrix(&matrixProject);
	if((degreeX != 0) && (degreeY != 0))
	{
		this->LeftMultiMatrix(&matrix_ropp);
		this->LeftMultiMatrix(&matrix_scalep);
	}
	this->LeftMultiMatrix(&matrix_return);

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function name: project
// project matrix, used by texture transformation
// param list:
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
funcStatus NewMatrix::textureProject(s16 degreeX, s16 degreeY, int TexWidth, int TexHeight, int TexOffsetX, int TexOffsetY)
{
	NewMatrix matrixProject,matrix_movetozero,matrix_return;
	if(TexWidth != 0 && TexHeight != 0)
	{
		if(TexWidth >= TexHeight)
		{
			matrixProject.ShearZToMatrix(degreeX,degreeY,TexWidth,TexWidth);
		}
		else
		{
			matrixProject.ShearZToMatrix(degreeX,degreeY,TexHeight,TexHeight);
		}
	}
	else
	{
		matrixProject.NewMatrix_init();
		matrixProject.NewMatrixInv_init();
	}

	if((degreeX == 0 * 16) && (degreeY == 0 * 16))
	{
		matrix_movetozero.NewMatrix_init();
		matrix_movetozero.NewMatrixInv_init();
		matrix_return.NewMatrix_init();
		matrix_return.NewMatrixInv_init();
	}
	else
	{
		matrix_movetozero.NewMatrix_init();
		matrix_movetozero.NewMatrixInv_init();
		matrix_return.NewMatrix_init();
		matrix_return.NewMatrixInv_init();

		if(degreeX != 0 * 16)
		{
			matrix_movetozero.TranslateToMatrix(-(((TexWidth/2) << 13) + (TexOffsetX << 9)), -(((TexHeight/2) << 13) + (TexOffsetY << 9)));
			matrix_return.TranslateToMatrix((((TexWidth/2) << 13) + (TexOffsetX << 9)), (((TexHeight/2) << 13) + (TexOffsetY << 9)));
		}

		if(degreeY != 0 * 16)
		{
			matrix_movetozero.TranslateToMatrix(-(((TexWidth/2) << 13) + (TexOffsetX << 9)), -(((TexHeight/2) << 13) + (TexOffsetY << 9)));
			matrix_return.TranslateToMatrix((((TexWidth/2) << 13) + (TexOffsetX << 9)), (((TexHeight/2) << 13) + (TexOffsetY << 9)));
		}
	}

	this->LeftMultiMatrix(&matrix_movetozero);
	this->LeftMultiMatrix(&matrixProject);
	this->LeftMultiMatrix(&matrix_return);

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function name: Matrix_I_Normalization
// normalization matrix I & Inverse_I
// param list:
// Remarks(Revision between versions):
//			created by yn 2019.03.14
//-----------------------------
#ifdef VERSION_2
funcStatus NewMatrix::Matrix_I_Normalization(void)
{
	#ifdef EMBEDDED
	int64_t temp = 0, temp_matrix = 0;
	//judge whether matrix is illegal
	if((this->I == 0) || (this->Inverse_I == 0))
	{
		return AHMI_FUNC_MATRIX_ILLEGAL;
	}
	//judge whether matrix need normalization	
	if((this->Inverse_I != (1<<13)) && (this->Inverse_I != 0)) //add by XT, 20200410
	{
		temp = ((int64_t)this->Inverse_A) * 8192 ;
		temp_matrix = csi_fix_div64((temp >> 32), temp ,this->Inverse_I);
		//judge whether matrix is illegal
		if((temp_matrix > (long long)0x7FFFFFFF) || (temp_matrix < (long long)0xFFFFFFFF80000000))
		{
			return AHMI_FUNC_MATRIX_ILLEGAL;
		}
		this->Inverse_A = temp_matrix;
		
		temp = ((int64_t)this->Inverse_B) * 8192 ;
		temp_matrix = csi_fix_div64((temp >> 32), temp ,this->Inverse_I);
		//judge whether matrix is illegal
		if((temp_matrix > (long long)0x7FFFFFFF) || (temp_matrix < (long long)0xFFFFFFFF80000000))
		{
			return AHMI_FUNC_MATRIX_ILLEGAL;
		}
		this->Inverse_B = temp_matrix;
		
		temp = ((int64_t)this->Inverse_C) * 8192 ;
		temp_matrix = csi_fix_div64((temp >> 32), temp ,this->Inverse_I);
		//judge whether matrix is illegal
		if((temp_matrix > (long long)0x7FFFFFFF) || (temp_matrix < (long long)0xFFFFFFFF80000000))
		{
			return AHMI_FUNC_MATRIX_ILLEGAL;
		}
		this->Inverse_C = temp_matrix;
		
		temp = ((int64_t)this->Inverse_D) * 8192 ;
		temp_matrix = csi_fix_div64((temp >> 32), temp ,this->Inverse_I);
		//judge whether matrix is illegal
		if((temp_matrix > (long long)0x7FFFFFFF) || (temp_matrix < (long long)0xFFFFFFFF80000000))
		{
			return AHMI_FUNC_MATRIX_ILLEGAL;
		}
		this->Inverse_D = temp_matrix;
		
		temp = ((int64_t)this->Inverse_E) * 8192 ;
		temp_matrix = csi_fix_div64((temp >> 32), temp ,this->Inverse_I);
		//judge whether matrix is illegal
		if((temp_matrix > (long long)0x7FFFFFFF) || (temp_matrix < (long long)0xFFFFFFFF80000000))
		{
			return AHMI_FUNC_MATRIX_ILLEGAL;
		}
		this->Inverse_E = temp_matrix;
		
		temp = ((int64_t)this->Inverse_F) * 8192 ;
		temp_matrix = csi_fix_div64((temp >> 32), temp ,this->Inverse_I);
		//judge whether matrix is illegal
		if((temp_matrix > (long long)0x7FFFFFFF) || (temp_matrix < (long long)0xFFFFFFFF80000000))
		{
			return AHMI_FUNC_MATRIX_ILLEGAL;
		}
		this->Inverse_F = temp_matrix;
		
		temp = ((int64_t)this->Inverse_G) * 8192 ;
		temp_matrix = csi_fix_div64((temp >> 32), temp ,this->Inverse_I);
		//judge whether matrix is illegal
		if((temp_matrix > (long long)0x7FFFFFFF) || (temp_matrix < (long long)0xFFFFFFFF80000000))
		{
			return AHMI_FUNC_MATRIX_ILLEGAL;
		}
		this->Inverse_G = temp_matrix;
		
		temp = ((int64_t)this->Inverse_H) * 8192 ;
		temp_matrix = csi_fix_div64((temp >> 32), temp ,this->Inverse_I);
		//judge whether matrix is illegal
		if((temp_matrix > (long long)0x7FFFFFFF) || (temp_matrix < (long long)0xFFFFFFFF80000000))
		{
			return AHMI_FUNC_MATRIX_ILLEGAL;
		}
		this->Inverse_H = temp_matrix;
		
		temp = ((int64_t)this->Inverse_I) * 8192 ;
		temp_matrix = csi_fix_div64((temp >> 32), temp ,this->Inverse_I);
		//judge whether matrix is illegal
		if((temp_matrix > (long long)0x7FFFFFFF) || (temp_matrix < (long long)0xFFFFFFFF80000000))
		{
			return AHMI_FUNC_MATRIX_ILLEGAL;
		}
		this->Inverse_I = temp_matrix;
	}
	#endif
	
	#ifdef PC_SIM
	//this->A = ((long long)this->A * (s32)(1 << 13)/ this->I);
	//this->B = ((long long)this->B * (s32)(1 << 13)/ this->I);
	//this->C = ((long long)this->C * (s32)(1 << 13)/ this->I);
	//this->D = ((long long)this->D * (s32)(1 << 13)/ this->I);
	//this->E = ((long long)this->E * (s32)(1 << 13)/ this->I);
	//this->F = ((long long)this->F * (s32)(1 << 13)/ this->I);
	//this->G = ((long long)this->G * (s32)(1 << 13)/ this->I);
	//this->H = ((long long)this->H * (s32)(1 << 13)/ this->I);
	//this->I = ((long long)this->I * (s32)(1 << 13)/ this->I);
	this->Inverse_A = ((long long)this->Inverse_A * (s32)(1 << 13)/ this->Inverse_I);
	this->Inverse_B = ((long long)this->Inverse_B * (s32)(1 << 13)/ this->Inverse_I);
	this->Inverse_C = ((long long)this->Inverse_C * (s32)(1 << 13)/ this->Inverse_I);
	this->Inverse_D = ((long long)this->Inverse_D * (s32)(1 << 13)/ this->Inverse_I);
	this->Inverse_E = ((long long)this->Inverse_E * (s32)(1 << 13)/ this->Inverse_I);
	this->Inverse_F = ((long long)this->Inverse_F * (s32)(1 << 13)/ this->Inverse_I);
	this->Inverse_G = ((long long)this->Inverse_G * (s32)(1 << 13)/ this->Inverse_I);
	this->Inverse_H = ((long long)this->Inverse_H * (s32)(1 << 13)/ this->Inverse_I);
	this->Inverse_I = ((long long)this->Inverse_I * (s32)(1 << 13)/ this->Inverse_I);
	#endif

	return AHMI_FUNC_SUCCESS;
}

#endif

ROIClass::ROIClass()
{
	lt.mPointX = 0;
	lt.mPointY = 0;
	rt.mPointX = 0;
	rt.mPointY = 0;
	lb.mPointX = 0;
	lb.mPointY = 0;
	rb.mPointX = 0;
	rb.mPointY = 0;
}

ROIClass::~ROIClass()
{
	
}

#endif
