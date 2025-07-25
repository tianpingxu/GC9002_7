////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:   2015/11/17
// File Name:     FileLoad.h
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2015/11/17 by...
// Additional Comments:
//    
// 
////////////////////////////////////////////////////////////////////////////////

#ifndef  FILELOAD__H
#define  FILELOAD__H

//#include "private.h"
#include "SoundVector.h"
#ifdef PC_SIM
//-----------------------------
// name£º OpenACFFile
// read acf file
//  
//    void
// addtional comments :
//   2.02 adds to read from two files
//-----------------------------
u32  OpenACFFile(void);

//-----------------------------
// name£º LoadConfigData
// read configuration information
//  
//    ConfigInfo* mConfigInfo    output the configuration inforamtion
// addtional comments :
//   reads sizeof(ConfigInfo) from file
//   2.02°æ±¾£º adds version number and compare it
//-----------------------------
int   LoadConfigData(ConfigInfoClass*);

//-----------------------------
// name£º ReadPageDataFromFile
// reads page information from file
//  
//    return DynamicPageCfg* the configuration information of page @see DynamicPageCfgClass
// addtional comments :
//-----------------------------
u32 ReadPageDataFromFile(DynamicPageCfgClass*);



u32 ReadWaveFilter(WaveFilter* pWF);

u32 ReadHintClass(HintClass* pHC);

u32 ReadSoundFromFile(SoundClass* pWF);

u32 ReadWaveFilterStatus(WaveFilterStatus* pWFS);

//-----------------------------
// name£º ReadTagDataFromFile
// reads tag information from file
//  
//    @return TagClassPtr The inforamtion of tags
// addtional comments :
//-----------------------------
u32 ReadTagDataFromFile(TagClassPtr pTag, u16 ID);

//-----------------------------
// name£º ReadStringData
// read string data from file,
// this strings are configured by IDE, fixed size u8 array
//  
//    @return StringClassPtr return the data of string
// addtional comments : create by lx
//   change comment by zuz 20181024
//-----------------------------
u32 ReadStringData(u8* pString, u16 length); 

//-----------------------------
// name:ReadStringList
// read string data from file,
// this strings are configured by IDE, fixed size u8 array
//  
//    @return StringClassPtr return the data of string
// addtional comments : create by yn
//   change comment by zuz 20200706
//-----------------------------
u32 ReadStringList(StringClassPtr pString);

//-----------------------------
// name:ReadU8Data
// read U8 data from file,
//-----------------------------
funcStatus ReadU8Data(u8* ptr, u16 length);

//-----------------------------
// name:ReadArrayDataList
// read Array data from file,
//  
//    @return ArrayDataClassPtr return the data of string
//-----------------------------
funcStatus ReadArrayDataList(ArrayDataClassPtr ptr);

//-----------------------------
// name:ReadFontRangeList
// read font range data from file,
//  
//    @return FontRangeClass* return the data of font ranges
//-----------------------------
funcStatus ReadFontRangeList(FontRangeClass* pFontRange);

//-----------------------------
// name£º ReadFontRangeRanges
// read ranges data from file,

//  
//    @return EncodingRange* return the data of ranges

//-----------------------------
funcStatus ReadFontRangeRanges(EncodingRange* pRanegs, u16 num); 

//-----------------------------
// name£º ReadTagStringList
// reads tag linked list from file
//  
//    @return ptr writes address after read
//	  @return number The number to read
// addtional comments :
//-----------------------------
u32 ReadTagStringList(u8* ptr, u16 number);

//-----------------------------
// name£º ReadInitString
// read the initialization string
//  
//    @return ptr The address that needs to write to
//	  @return number The number that needs to read 
// addtional comments :
//-----------------------------
u32 ReadInitString(u8* ptr, u16 number);

//-----------------------------
// name£º ReadTagValueData
// read string data from file,
// this strings are configured by IDE, fixed size u8 array
//  
//    @return StringClassPtr return the data of string
// addtional comments : create by lx
//   change comment by zuz 20181024
//-----------------------------
u32 ReadTagValueData(u8* pTagValueSpacePtr, u16 length); 

//-----------------------------
// name£º LoadTextureToSDRAM
// read texture data from file, only used in software
//  
//    @param1 void* INbuffer The address of input buffer 
//    @param2 u32   bufsize  The size of buffer
//    @param3 int   size     The size needed to be read
// addtional comments :
//-----------------------------
void   LoadTextureToSDRAM(void*,u32,int size);
#endif

#ifdef EMBEDDED

#if defined(EMBEDDED) && defined(VERSION_2)
void SeekSTMFlashAddr(u32 addr);
u32 GetSTMFlashAddr();

void ReadDataFromSTMFlash_ByQSPI(void* data_addr, u32 len);
void ReadDataFromSTMFlash(void* data_addr, u32 len);
#endif
//-----------------------------
// name£º LoadConfigData
// read configuration information
//  
//    ConfigInfo* mConfigInfo    output the configuration inforamtion
// addtional comments :
//   reads sizeof(ConfigInfo) from file
//   2.02°æ±¾£º adds version number and compare it
//-----------------------------
funcStatus LoadConfigData(ConfigInfoClass* mConfigInfo);

//-----------------------------
// name£º ReadPageDataFromFile
// reads page information from file
//  
//    return DynamicPageCfg* the configuration information of page @see DynamicPageCfgClass
// addtional comments :
//-----------------------------
funcStatus ReadPageData(DynamicPageCfgClass* curPage);

funcStatus ReadWaveFilter(WaveFilter* pWF);

funcStatus ReadWaveFilterStatus(WaveFilterStatus* pWFS);

funcStatus ReadHintClass(HintClass* pHC);

funcStatus ReadSoundFromFile(SoundClass* pWFS);

//-----------------------------
// name£º ReadTagData
// reads tag information from file
//  
//    @return TagClassPtr The inforamtion of tags
// addtional comments :
//-----------------------------
u32 ReadTagData(TagClassPtr pTag, u16 ID);

//-----------------------------
// name£º ReadStringData
// read string data from file,
// this strings are configured by IDE, fixed size u8 array
//  
//    @return StringClassPtr return the data of string
// addtional comments : create by lx
//   change comment by zuz 20181024
//-----------------------------
u32 ReadStringData(u8* pString, u16 length); 

//-----------------------------
// name:ReadStringList
// read string data from file,
// this strings are configured by IDE, fixed size u8 array
//  
//    @return StringClassPtr return the data of string
// addtional comments : create by yn
//   change comment by zuz 20200706
//-----------------------------
u32 ReadStringList(StringClassPtr pString);

//-----------------------------
// name:ReadU8Data
// read U8 data from file,
//-----------------------------
funcStatus ReadU8Data(u8* ptr, u16 length);

//-----------------------------
// name:ReadArrayDataList
// read Array data from file,
//  
//    @return ArrayDataClassPtr return the data of string
//-----------------------------
funcStatus ReadArrayDataList(ArrayDataClassPtr ptr);

//-----------------------------
// name:ReadFontRangeList
// read font range data from file,
//  
//    @return FontRangeClass* return the data of font ranges
//-----------------------------
funcStatus ReadFontRangeList(FontRangeClass* pFontRange);

//-----------------------------
// name£º ReadFontRangeRanges
// read ranges data from file,

//  
//    @return EncodingRange* return the data of ranges

//-----------------------------
funcStatus ReadFontRangeRanges(EncodingRange* pRanegs, u16 num); 

//-----------------------------
// name£º ReadTagStringList
// reads tag linked list
//  
//    @return ptr writes address after read
//	  @return number The number to read
// addtional comments :
//-----------------------------
void ReadTagStringList(u8* ptr, u16 number);

//-----------------------------
// name£º ReadInitString
// read the initialization string
//  
//    @return ptr The address that needs to write to
//	  @return number The number that needs to read 
// addtional comments :
//-----------------------------
void ReadInitString(u8* ptr, u16 number);

//-----------------------------
// name:ReadTagValueData
// read string data from file,
// this strings are configured by IDE, fixed size u8 array
//  
//    @return StringClassPtr return the data of string
// addtional comments : create by lx
//   change comment by zuz 20181024
//-----------------------------
u32 ReadTagValueData(u8* pTagValueSpacePtr, u16 length);
#endif

#endif 
