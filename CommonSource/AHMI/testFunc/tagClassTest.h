////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:   2017/06/14
// File Name:     animationTest.h
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2017/06/14 by Zhou Yuzhi 
// Additional Comments:
//    the class of animatonTest
// 
////////////////////////////////////////////////////////////////////////////////
#ifndef ANIMATION_TEST__H
#define ANIMATION_TEST__H

#include <fstream>
#include <iostream>


class tagClassTestClass{
public:
	void tagSetValueTest(std::ofstream* pAnimationTestLogFile);
	void tagSetBindingElementTest(std::ofstream* pAnimationTestLogFile);
	void updateAllTagsTest(std::ofstream* pAnimationTestLogFile);
};


#endif