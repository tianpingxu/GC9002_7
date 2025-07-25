////////////////////////////////////////////////////////////////////////////////
// Company:		Graphichina Inc
//
// Create Date:   2016/03/21
// File Name:     DynamicPage.h
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2016/03/21 by Zhou Yuzhi
// Additional Comments:
//    the math function
// 
////////////////////////////////////////////////////////////////////////////////

#ifndef MYMATHCLASS__H
#define MYMATHCLASS__H

#include "publicDefine.h"

#ifdef AHMI_CORE

#define Use_CORDIC
//#define Use_SinTanTable

typedef enum
{
    EM_ErrRetMyMath_DegreeOverrun = 1
   
}EM_ErrRetMyMath;

class myMathClass
{
public:

	//*****************************
	// CORDIC algorithm for trigonometric functions
	//  CORDIC (s32 s32_Radian,  // Rotation angle, left hand line, clockwise direction, accuracy 1.27.4
	//          s16 *s32p_COS,   // The X component of initial azimuth and rotation azimuth is equivalent to 1/2048 accuracy after normalization.
	//          s16 *s32p_SIN);  // The Y component of initial azimuth and rotation azimuth is equivalent to 1/2048 accuracy after normalization.
	//*****************************
	void CORDIC(s32 Radian, s16* COS, s16 *SIN);

	//*****************************
	// Calculate the angle according to the input s32_x and s32_y coordinates.
	//  CORDIC_ANGLE (s32 s32_Radian, // Angle, precision 1.27.4
	//				  s16 *s32p_COS,  //  S32_x coordinates accuracy 1.11.4
	//				  s16 *s32p_SIN); //  S32_Y coordinates accuracy 1.11.4
	//*****************************
	void CORDIC_ANGLE(s32* Radian, s16 COS, s16 SIN);

	//*****************************
	// CORDIC algorithm for trigonometric functions,32 bit version
	//  CORDIC_32 (s32 s32_Radian,  // Rotation angle, left hand line, clockwise direction, accuracy 1.27.4
	//			   s16 *s32p_COS,   // The X component of initial azimuth and rotation azimuth is equivalent to 1/2048 accuracy after normalization.
	//			   s16 *s32p_SIN);  // The Y component of initial azimuth and rotation azimuth is equivalent to 1/2048 accuracy after normalization.
	//*****************************
	void CORDIC_32(s32 s32_Radian, s32* s32p_COS, s32 *s32p_SIN);
	u32 SinTable(s32 s32_Radian, s32* s32p_COS, s32 *s32p_SIN);
	u32 TanTable(s32 s32_Radian, s32* s32p_TAN);

	//****************************
	//The rotation transformation of the inverse matrix is clockwise in the positive direction and only considers the 2 order part.
	//| s32_ipointmatrix[0],s32_ipointmatrix[1] |  \/  | cos(theta),-sin(theth)|
	//| s32_ipointmatrix[2],s32_ipointmatrix[3] |  /\  | sin(theta), cos(theta)|
	//****************************	
	void MatrixRotate(s16 degrees, s32 ipointmatrix[]);

	//****************************
	//The rotation transformation of the inverse matrix takes only 2 orders.
	//  | s32_ipointmatrix[0],s32_ipointmatrix[1] | \/ | 1/ScaleX,    0    |
	//  | s32_ipointmatrix[2],s32_ipointmatrix[3] | /\ |     0   ,1/ScaleY)|
	//The format of ScaleX and ScaleY is 1.6.9, and s32_ipointmatrix format is 1.6.9.
	//****************************
	void MatrixScaler(s16 ScaleX, s16 ScaleY, s32 ipointmatrix[]);

	//****************************
	//The tangent transformation of inverse matrix takes only 2 part (X direction tangent).
	//S16_degrees denotes the tan value of the texture tangent angle in the X direction, tan0 denotes no tangent, and tan90 denotes a straight line
	//| s32_ipointmatrix[0],s32_ipointmatrix[1] |  \/  |       1      ,			 0       |
	//| s32_ipointmatrix[2],s32_ipointmatrix[3] |  /\  | -tan(s16_degrees),      1       |
	//****************************
	void MatrixShearX(s16 degrees,s32 ipointmatrix[]);

	//****************************
	//The tangent transformation of inverse matrix takes only 2 part (Y direction tangent).
	//S16_degrees denotes the tan value of the texture tangent angle in the X direction, tan0 denotes no tangent, and tan90 denotes a straight line
	//| s32_ipointmatrix[0],s32_ipointmatrix[1] |  \/  |       1   ,  -tan(s16_degrees)   |
	//| s32_ipointmatrix[2],s32_ipointmatrix[3] |  /\  |       0   ,          1           |
	//****************************
	void MatrixShearY(s16 degrees,s32 ipointmatrix[]);

	//*****************************
	// CORDIC algorithm for absolute value and angle
	//  CORDIC (s16 *s32p_COS,   // The X component of initial azimuth and rotation azimuth is equivalent to 1/2048 accuracy after normalization.
	//          s16 *s32p_SIN);  // The Y component of initial azimuth and rotation azimuth is equivalent to 1/2048 accuracy after normalization.
	//*****************************
	void FindAngle(s32* x, s32 *y);

	s32 GetAngleFromTan(s32 angleTan);


	//*****************************
	//Convert the input float variable to the s1.8.9 form.
	//parameter list
	//     @param1 float f_in float tpye input
	//     @param2 s16 s16_out s16 type output 1.8.9
	//*****************************
	funcStatus transferFloatToS16(float f_in, s16* s16_out);
};

#endif

#endif
