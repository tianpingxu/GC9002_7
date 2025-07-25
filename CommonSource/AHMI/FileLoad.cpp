////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:   2015/11/17
// File Name:     FileLoad.c
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2015/11/17 by...
// Revision 2.01 - modify the LoadPage function by Zhou Yuzhi
// Additional Comments:
//    
// 
////////////////////////////////////////////////////////////////////////////////

#include "publicInclude.h"
#include <stdio.h>
//#include "trace.h"
#include "SoundVector.h"
#include "AHMIInterface.h"
#ifdef EMBEDDED
#include "csi_ahmi.h"
#endif

extern u32 StartOfConfigdata;
extern ConfigInfoClass		ConfigData;
extern char* cfgString;
extern s16 screenratio;

#ifdef PC_SIM

extern "C" extern int compress_sig;
extern "C" extern int embedded_platform;
extern "C" extern int decompress_entry(const char * inFilename , const char * outFilename );

FILE *fp;
FILE *TexVideo;
extern u8       VideoTextureBuffer[VIDEO_BUF_SIZE];

//-----------------------------
// name: OpenACFFile
// read acf file
//  
//    void
// addtional comments :
//   2.02 adds to read from two files
//-----------------------------
u32 OpenACFFile()
{
	errno_t err;
	#ifdef VERSION_2
	//compress_sig = 1;
	if ((compress_sig == 1) && ((embedded_platform == EMBEDDED_GC9003) || (embedded_platform == EMBEDDED_GC9005))){
		err = decompress_entry(AHMI_CMPSOURCEFILE_NAME,AHMI_SOURCEFILE_NAME);
		if(err != 0)
		{
			ERROR_PRINT("Decompress Failed.\r\n");
			return (EM_enErrRetModularInit | EM_ErrRetInit_LoadConfigDataErr); 
		}
	}
	err = fopen_s(&fp,AHMI_SOURCEFILE_NAME,"rb");
	if(err != 0)
	{
		ERROR_PRINT("Cannot Open AHMI Source File.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_LoadConfigDataErr); 
	}
	err = fopen_s(&TexVideo,AHMI_TEX_VIDEO_NAME,"rb");
	if(err != 0)
	{
		ERROR_PRINT("Cannot Open TexVideo File.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_LoadConfigDataErr); 
	}
	#endif
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:LoadConfigData
// read configuration information
//  
//    ConfigInfo* mConfigInfo    output the configuration inforamtion
// addtional comments :
//   reads sizeof(ConfigInfo) from file
//   version 2.02:adds version number and compare it
//-----------------------------
int LoadConfigData(ConfigInfoClass* mConfigInfo)
{
	size_t count;
	char text[100];
	fread_s(&StartOfConfigdata, 4, 1, 4, fp);

	fseek(fp,StartOfConfigdata,SEEK_SET);
	count = fread_s(mConfigInfo,sizeof(ConfigInfoClass),sizeof(ConfigInfoClass),1,fp);
	if(count != 1)
	{
		ERROR_PRINT("This is not a AHMI Source File.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_LoadConfigDataErr); 
	}
	if(mConfigInfo->MagicNumber!= 0x494d4841)
	{
		ERROR_PRINT("This is not a AHMI Source File.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_LoadConfigDataErr); 
	}
	//use global param to store the ratio of ellipse correction
	screenratio = mConfigInfo->screenratio;
	WriteText(text,"The version of current file is %04x. The version of current project is %04x.\r\n",mConfigInfo->AHMIInfo.Version,AHMI_CPU1_VERSION);
	ERROR_PRINT(text);
	if(mConfigInfo->AHMIInfo.Version != (u32)AHMI_CPU1_VERSION)
	{
		ERROR_PRINT("The version of the file cannot match the version of the project!!!\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_LoadConfigDataErr); 
	}
	WriteText(text,"%d Pages in this File.\r\n%d Timers in this File.\r\n%d Tags in this File.\r\n",mConfigInfo->NumofPages,mConfigInfo->NumofTimers,mConfigInfo->NumofTags);
	ERROR_PRINT(text);
	WriteText(text,"Total Texture Size is %d.\r\nDynamic Page Space Size is %d.\r\n",mConfigInfo->TotalTextureSize,mConfigInfo->DynamicPageSize);
	ERROR_PRINT(text);

	//version information
	cfgString = (char*)csi_kernel_malloc(sizeof(char)* ConfigData.CfgStringSize, NULL);
	//load config string
	count = fread_s(cfgString,sizeof(char)*ConfigData.CfgStringSize,sizeof(char)*ConfigData.CfgStringSize,1,fp);
	if(count != 1)
	{
		ERROR_PRINT("cannot read config string.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_LoadConfigDataErr); 
	}
	ERROR_PRINT(cfgString);
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ReadPageDataFromFile
// reads page information from file
//  
//    return DynamicPageCfg* the configuration information of page @see DynamicPageCfgClass
// addtional comments :
//-----------------------------
u32 ReadPageDataFromFile(DynamicPageCfgClass* curPage)
{
	size_t count;
	count = fread_s(curPage,sizeof(DynamicPageCfgClass),sizeof(DynamicPageCfgClass),1,fp);
	if(count != 1)
	{
		ERROR_PRINT("There is no enough data to load dynamic page data.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_LoadConfigDataErr);
	}
	curPage->StoreOffset += StartOfConfigdata;
	return AHMI_FUNC_SUCCESS;
}

u32 ReadWaveFilter(WaveFilter* pWF){
	size_t count;
	count = fread_s(pWF,sizeof(WaveFilter),sizeof(WaveFilter),1,fp);
	if(count != 1)
	{
		ERROR_PRINT("There is no enough data to load  wave filter data.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_ReadWaveFilterErr);
	}
	return AHMI_FUNC_SUCCESS;
}

u32 ReadWaveFilterStatus(WaveFilterStatus* pWFS){
	size_t count;
	count = fread_s(pWFS,sizeof(WaveFilterStatus),sizeof(WaveFilterStatus),1,fp);
	if(count != 1)
	{
		ERROR_PRINT("There is no enough data to load  wave filter status data.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_ReadWaveFilterStatusErr);
	}
	return AHMI_FUNC_SUCCESS;
}

u32 ReadHintClass(HintClass* pHC){
	size_t count;
	count = fread_s(pHC,sizeof(HintClass),sizeof(HintClass),1,fp);
	if(count != 1)
	{
		ERROR_PRINT("There is no enough data to load  hint class data.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_ReadHintErr);
	}
	return AHMI_FUNC_SUCCESS;
}

u32 ReadSoundFromFile(SoundClass* pWF){

	size_t count;
	count = fread_s(pWF,sizeof(SoundClass),sizeof(SoundClass),1,fp);
	if(count != 1)
	{
		ERROR_PRINT("There is no enough data to load  soundclass data.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_ReadSoundErr);
	}
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ReadTagDataFromFile
// reads tag information from file
//  
//    @return TagClassPtr The inforamtion of tags
// addtional comments :
//-----------------------------
u32 ReadTagDataFromFile(TagClassPtr pTag, u16 ID)
{
	size_t count;
	TagClassCfg newTagCfg;
	WidgetLinkDataPtr pWidgetLinkData;
	PageLinkDataPtr pPageLinkData;
	CanvasLinkDataPtr pCanvasLinkData;
	u32 size;
	 
	//read config of tag
	count = fread_s(&newTagCfg,sizeof(TagClassCfg),sizeof(TagClassCfg),1,fp);
	if(count != 1)
	{
		ERROR_PRINT("There is no enough data to load tag Configuration data.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_ReadTagErr);
	}

	//binding widget
	size = sizeof(struct WidgetLinkData) * newTagCfg.NumOfWidgetLinker;
	pWidgetLinkData =(WidgetLinkDataPtr) csi_kernel_malloc( size, NULL );
	if(pWidgetLinkData == NULL && size != 0)
	{
		ERROR_PRINT("pWidgetLinkData Initial failed.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_MallocSpaceErr);
	}
	count = fread_s(pWidgetLinkData,size,sizeof(struct WidgetLinkData),newTagCfg.NumOfWidgetLinker,fp);
	if(count != newTagCfg.NumOfWidgetLinker)
	{
		ERROR_PRINT("There is no enough data to load tag data of widgetlinkdata.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_ReadTagErr);
	}

	//binding dynamic page actions
	size = sizeof(struct PageLinkData) * newTagCfg.NumOfDynamicPageAction;
	pPageLinkData =(PageLinkDataPtr) csi_kernel_malloc( size, NULL );
	if(pPageLinkData == NULL && size != 0)
	{
		ERROR_PRINT("pPageLinkData Initial failed.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_MallocSpaceErr);
	}
	count = fread_s(pPageLinkData,size,sizeof(struct PageLinkData),newTagCfg.NumOfDynamicPageAction,fp);
	if(count != newTagCfg.NumOfDynamicPageAction)
	{
		ERROR_PRINT("There is no enough data to load tag data of PageLinkData.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_ReadTagErr);
	}

	//binding canvas 
	size = sizeof(struct CanvasLinkData) * newTagCfg.NumOfCanvasAction;
	pCanvasLinkData =(CanvasLinkDataPtr) csi_kernel_malloc( size, NULL );
	if(pCanvasLinkData == NULL && size != 0)
	{
		ERROR_PRINT("pCanvasLinkData Initial failed.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_MallocSpaceErr);
	}
	count = fread_s(pCanvasLinkData,size,sizeof(struct CanvasLinkData),newTagCfg.NumOfCanvasAction,fp);
	if(count != newTagCfg.NumOfCanvasAction)
	{
		ERROR_PRINT("There is no enough data to load tag data of PageLinkData.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_ReadTagErr);
	}

	//set the binding information to tags
	pTag->initTag(
		newTagCfg.NumOfWidgetLinker,
		newTagCfg.NumOfCanvasAction,
		newTagCfg.NumOfDynamicPageAction,
		ID,
		newTagCfg.mTagType,
		newTagCfg.mRegID,
		newTagCfg.mAddr,
		newTagCfg.mValueLen,
		newTagCfg.mCurArryElemNum,
		newTagCfg.mStoreID,
		pWidgetLinkData,
		pPageLinkData,
		pCanvasLinkData
	);

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ReadStringData
// read string data from file,
// this strings are configured by IDE, fixed size u8 array
//  
//    @return StringClassPtr return the data of string
// addtional comments : create by lx
//   change comment by zuz 20181024
//-----------------------------
u32 ReadStringData(u8* pString, u16 length){
	//read config
	size_t count;
	count = fread_s(pString,sizeof(u8) * length,sizeof(u8),length,fp);
	if(count != length){
		return (EM_enErrRetModularInit | EM_ErrRetInit_ReadStringTagErr);
	}
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ReadStringList
// read string data from file,
// this strings are configured by IDE, fixed size u8 array
//  
//    @return StringClassPtr return the data of string
// addtional comments : create by yn
//   change comment by zuz 20200706
//-----------------------------
funcStatus ReadStringList(StringClassPtr pString)
{
	//read config
	size_t count;
	count = fread_s(pString,sizeof(StringClass),sizeof(StringClass),1,fp);
	if(count!=1){
		return (EM_enErrRetModularInit | EM_ErrRetInit_ReadStringTagErr);
	}
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ReadU8Data
// read U8 data from file,
//-----------------------------
funcStatus ReadU8Data(u8* ptr, u16 length)
{
	//read config
	size_t count;
	count = fread_s(ptr,sizeof(u8) * length,sizeof(u8),length,fp);
	if(count != length){
		return (EM_enErrRetModularInit | EM_ErrRetInit_ReadArryDataErr);
	}
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ReadArrayDataList
// read Array data from file,
//  
//    @return ArrayDataClassPtr return the data of string
//-----------------------------
funcStatus ReadArrayDataList(ArrayDataClassPtr ptr){
	//read config
	size_t count;
	count = fread_s(ptr,sizeof(ArrayDataClass),sizeof(ArrayDataClass),1,fp);
	if(count!=1){
		return (EM_enErrRetModularInit | EM_ErrRetInit_ReadArryDataErr);
	}
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ReadTagStringList
// reads tag linked list
//  
//    @return ptr writes address after read
//	  @return number The number to read
// addtional comments :
//-----------------------------
u32 ReadTagStringList(u8* ptr, u16 number)
{
	size_t count;
	if((ptr == NULL) || (number == NULL))
		return (EM_enErrRetModularInit | EM_ErrRetInit_NULLPointerErr);
		
	count = fread_s(ptr,sizeof(u8) * number,sizeof(u8),number,fp);
	if(count != 1)
	{
		ERROR_PRINT("There is no enough data to load tag Configuration data.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_ReadStringTagErr);
	}
}


//-----------------------------
// name:ReadFontRangeList
// read font range data from file,
//  
//    @return FontRangeClass* return the data of font ranges
//-----------------------------
funcStatus ReadFontRangeList(FontRangeClass* pFontRange){
	//read config
	size_t count;
	count = fread_s(pFontRange,sizeof(FontRangeClass),sizeof(FontRangeClass),1,fp);
	if(count!=1){
		return (EM_enErrRetModularInit | EM_ErrRetInit_ReadFontRangesErr);
	}
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name�� ReadFontRangeRanges
// read ranges data from file,

//  
//    @return EncodingRange* return the data of ranges

//-----------------------------
funcStatus ReadFontRangeRanges(EncodingRange* pRanegs, u16 num)
{
	size_t count;
	count = fread_s(pRanegs,sizeof(EncodingRange) * num,sizeof(EncodingRange),num,fp);
	if(count != num){
		return (EM_enErrRetModularInit | EM_ErrRetInit_ReadFontRangesErr);
	}
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ReadInitString
// read the initialization string
//  
//    @return ptr The address that needs to write to
//	  @return number The number that needs to read 
// addtional comments :
//-----------------------------
u32 ReadInitString(u8* ptr, u16 number)
{
	size_t count;
	if((ptr == NULL) || (number == NULL))
		return (EM_enErrRetModularInit | EM_ErrRetInit_NULLPointerErr);
		
	count = fread_s(ptr,sizeof(u8)*number,sizeof(u8),number,fp);
	if(count != 1)
	{
		ERROR_PRINT("There is no enough data to load tag Configuration data.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_ReadStringTagErr);
	}
}

//-----------------------------
// name:ReadTagValueData
// read string data from file,
// this strings are configured by IDE, fixed size u8 array
//  
//    @return StringClassPtr return the data of string
// addtional comments : create by lx
//   change comment by zuz 20181024
//-----------------------------
u32 ReadTagValueData(u8* pTagValueSpacePtr, u16 length){
	//read config
	size_t count;
	count = fread_s(pTagValueSpacePtr,sizeof(u8) * length,sizeof(u8),length,fp);
	if(count != length){
		return (EM_enErrRetModularInit | EM_ErrRetInit_ReadTagErr);
	}
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:LoadTextureToSDRAM
// read texture data from file, only used in software
//  
//    @param1 void* INbuffer The address of input buffer 
//    @param2 u32   bufsize  The size of buffer
//    @param3 int   size     The size needed to be read
// addtional comments :
//-----------------------------
void LoadTextureToSDRAM	(void* INbuffer,u32 bufsize, int size)
{
	size_t count = 0;
	int i;
	int videoSize = 720*576*2;
	#ifdef VERSION_2
	fseek(fp,0,SEEK_SET);
	#endif
	for(i = 0; i < size; i++)
	{
		#ifdef VERSION_2
			count += fread_s((char *)INbuffer + i,bufsize,sizeof(unsigned char),1,fp);
		#endif
	}

	//read the video texture from file
	fread_s((char *)VideoTextureBuffer, videoSize,1,videoSize,TexVideo);
   
	//generate file for FPGA simulation
#ifdef TEST_VECTOR_GEN
	FILE *fp1;
	if(count!=size)
	{
		ERROR_PRINT("There is no enough data to load texture.\r\n");
	}

	fp1 = fopen("spidata.hex","w");
	#ifdef VERSION_2
	for(i=0;i<(size);i++)
	{
		fprintf(fp1,"%02X\n",((u8*)INbuffer)[i]);
	}
	#endif
	fclose(fp1);
#endif

}
#endif

#ifdef EMBEDDED

uint32_t LoadConfigData(ConfigInfoClass* mConfigInfo)
{
	uint32_t StartOfConfigdata_Pos = 0, align_size = 0;
    
    #if defined(EMBEDDED) && defined(VERSION_2)
    #if defined(CHIP_GC9002)
    align_size = 16*1024;
    StartOfConfigdata_Pos = 0;
    #else
    if(DRAM_CACHE->Gzip_EN)
    {
        align_size = 1024;
        StartOfConfigdata_Pos = 128*1024 - 4;
    }
    else
    {
        align_size = 16*1024;
        StartOfConfigdata_Pos = 0;
    }
    #endif
    csi_ahmi_SeekReadConfigDataAddr(StartOfConfigdata_Pos);
    #endif

	csi_ahmi_ReadConfigData(&StartOfConfigdata, sizeof(uint32_t));
	#if defined(CHIP_GC9002) || defined(CHIP_GC9005)
	//StartOfConfigdata = 0x1488000;//only for test
	#endif
	
	if(StartOfConfigdata % align_size != 0)
	{
		ERROR_PRINT("This is not a AHMI Source File.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_LoadConfigDataErr);
	}
	
#if defined(EMBEDDED) && defined(VERSION_2)   
	csi_ahmi_SeekReadConfigDataAddr(StartOfConfigdata);
#endif
	
	csi_ahmi_ReadConfigData(mConfigInfo,sizeof(ConfigInfoClass));
	
	if(mConfigInfo->MagicNumber!= 0x494d4841)
	{
		ERROR_PRINT("This is not a AHMI Source File.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_LoadConfigDataErr);
	}
	//printf("The version of current file is %04x. The version of current project is %04x.\r\n",mConfigInfo->AHMIInfo.Version,VERSION);

	if(mConfigInfo->AHMIInfo.Version != (u32)AHMI_CPU1_VERSION)
	{
		ERROR_PRINT("The version of the file cannot match the version of the project!!!\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_LoadConfigDataErr);
	}
    
    //use global param to store the ratio of ellipse correction
    screenratio = mConfigInfo->screenratio;
	//version information
	cfgString = (char*)csi_kernel_malloc(sizeof(char)* ConfigData.CfgStringSize, NULL);
	//load config string
	csi_ahmi_ReadConfigData(cfgString, sizeof(char)* ConfigData.CfgStringSize);
	if(cfgString == NULL)
	{
		ERROR_PRINT("cannot read config string.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_NULLPointerErr);
	}

	return AHMI_FUNC_SUCCESS;
}

funcStatus ReadPageData(DynamicPageCfgClass* curPage)
{
	csi_ahmi_ReadConfigData(curPage,sizeof(DynamicPageCfgClass));
	return AHMI_FUNC_SUCCESS;
}

funcStatus ReadWaveFilter(WaveFilter* pWF)
{
    csi_ahmi_ReadConfigData(pWF,sizeof(WaveFilter));
	return AHMI_FUNC_SUCCESS;
}

funcStatus ReadWaveFilterStatus(WaveFilterStatus* pWFS)
{
    csi_ahmi_ReadConfigData(pWFS,sizeof(WaveFilterStatus));
	return AHMI_FUNC_SUCCESS;
}

funcStatus ReadHintClass(HintClass* pHC)
{
	csi_ahmi_ReadConfigData(pHC,sizeof(HintClass));
	return AHMI_FUNC_SUCCESS;
}

funcStatus ReadSoundFromFile(SoundClass* pWFS)
{
    csi_ahmi_ReadConfigData(pWFS,sizeof(SoundClass));
	return AHMI_FUNC_SUCCESS;
}

u32 ReadTagData(TagClassPtr pTag,u16 ID)
{

	TagClassCfg			newTagCfg;
	WidgetLinkDataPtr	pWidgetLinkData;
	PageLinkDataPtr		pPageLinkData;
	CanvasLinkDataPtr	pCanvasLinkData;
	u32					size;

	//read config of tag
	csi_ahmi_ReadConfigData(&newTagCfg,sizeof(TagClassCfg));

	//binding widget
	size = sizeof(struct WidgetLinkData) * newTagCfg.NumOfWidgetLinker;
	pWidgetLinkData =(WidgetLinkDataPtr) csi_kernel_malloc( size, NULL );
	if(pWidgetLinkData == NULL && size != 0)
	{
		ERROR_PRINT("pWidgetLinkData Initial failed.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_MallocSpaceErr);
	}
	csi_ahmi_ReadConfigData(pWidgetLinkData,size);

	//binding dynamic page actions
	size = sizeof(struct PageLinkData) * newTagCfg.NumOfDynamicPageAction;
	pPageLinkData =(PageLinkDataPtr) csi_kernel_malloc( size, NULL );
	if(pPageLinkData == NULL && size != 0)
	{
		ERROR_PRINT("pPageLinkData Initial failed.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_MallocSpaceErr);
	}
	csi_ahmi_ReadConfigData(pPageLinkData,size);

	//binding canvas 
	size = sizeof(struct CanvasLinkData) * newTagCfg.NumOfCanvasAction;
	pCanvasLinkData =(CanvasLinkDataPtr) csi_kernel_malloc( size, NULL );
	if(pCanvasLinkData == NULL && size != 0)
	{
		ERROR_PRINT("pCanvasLinkData Initial failed.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_MallocSpaceErr);
	}
	csi_ahmi_ReadConfigData(pCanvasLinkData,size);

	//set the binding information to tags
	pTag->initTag(
		newTagCfg.NumOfWidgetLinker,
		newTagCfg.NumOfCanvasAction,
		newTagCfg.NumOfDynamicPageAction,
		ID,
		newTagCfg.mTagType,
		newTagCfg.mRegID,
		newTagCfg.mAddr,
		newTagCfg.mValueLen,
		newTagCfg.mCurArryElemNum,
		newTagCfg.mStoreID,
		pWidgetLinkData,
		pPageLinkData,
		pCanvasLinkData
	);

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ReadStringData
// read string data from file,
// this strings are configured by IDE, fixed size u8 array
//  
//    @return StringClassPtr return the data of string
// addtional comments : create by lx
//   change comment by zuz 20181024
//-----------------------------
u32 ReadStringData(u8* pString, u16 length){
	//read config
//	size_t count;
//	StringClass newString;
    
    if(pString == NULL)
        return (EM_enErrRetModularInit | EM_ErrRetInit_NULLPointerErr);
    
    csi_ahmi_ReadConfigData(pString, length);
    
//	pString->initString(newString.id,newString.value);
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ReadStringList
// read string data from file,
// this strings are configured by IDE, fixed size u8 array
//  
//    @return StringClassPtr return the data of string
// addtional comments : create by yn
//   change comment by zuz 20200706
//-----------------------------
u32 ReadStringList(StringClassPtr pString){
	//read config
//	size_t count;
	if(pString == NULL)
        return (EM_enErrRetModularInit | EM_ErrRetInit_NULLPointerErr);
    
    csi_ahmi_ReadConfigData(pString, sizeof(StringClass));
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ReadTagStringList
// reads tag linked list from file
//  
//    @return ptr writes address after read
//	  @return number The number to read
// addtional comments :
//-----------------------------
void ReadTagStringList(u8* ptr, u16 number)
{
	if((ptr == NULL) || (number == 0))
		return;
	csi_ahmi_ReadConfigData(ptr,number);
}

//-----------------------------
// name:ReadU8Data
// read U8 data from file,
//-----------------------------
funcStatus ReadU8Data(u8* ptr, u16 length)
{
	if((ptr == NULL) || (length == 0))
		return (EM_enErrRetModularInit | EM_ErrRetInit_NULLPointerErr);
	
	csi_ahmi_ReadConfigData(ptr, sizeof(u8) * length);
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ReadArrayDataList
// read Array data from file,
//  
//    @return ArrayDataClassPtr return the data of string
//-----------------------------
funcStatus ReadArrayDataList(ArrayDataClassPtr ptr)
{
	if(ptr == NULL)
		return (EM_enErrRetModularInit | EM_ErrRetInit_NULLPointerErr);
	
	csi_ahmi_ReadConfigData(ptr, sizeof(ArrayDataClass));
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ReadFontRangeList
// read font range data from file,
//  
//    @return FontRangeClass* return the data of font ranges
//-----------------------------
funcStatus ReadFontRangeList(FontRangeClass* pFontRange)
{
	if(pFontRange == NULL)
		return (EM_enErrRetModularInit | EM_ErrRetInit_NULLPointerErr);
	
	csi_ahmi_ReadConfigData(pFontRange, sizeof(FontRangeClass));
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name�� ReadFontRangeRanges
// read ranges data from file,

//  
//    @return EncodingRange* return the data of ranges

//-----------------------------
funcStatus ReadFontRangeRanges(EncodingRange* pRanegs, u16 num)
{
	if((pRanegs == NULL) || (num == 0))
		return (EM_enErrRetModularInit | EM_ErrRetInit_NULLPointerErr);
	
	csi_ahmi_ReadConfigData(pRanegs, sizeof(EncodingRange) * num);
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:ReadInitString
// read the initialization string
//  
//    @return ptr The address that needs to write to
//	  @return number The number that needs to read 
// addtional comments :
//-----------------------------
void ReadInitString(u8* ptr, u16 number)
{
	if((ptr == NULL) || (number == 0))
		return;
	csi_ahmi_ReadConfigData(ptr,number);
}

//-----------------------------
// name:ReadTagValueData
// read string data from file,
// this strings are configured by IDE, fixed size u8 array
//  
//    @return StringClassPtr return the data of string
// addtional comments : create by lx
//   change comment by zuz 20181024
//-----------------------------
u32 ReadTagValueData(u8* pTagValueSpacePtr, u16 length)
{
	if((pTagValueSpacePtr == NULL) || (length == 0))
		return (EM_enErrRetModularInit | EM_ErrRetInit_NULLPointerErr);
	
	csi_ahmi_ReadConfigData(pTagValueSpacePtr,length);
	return AHMI_FUNC_SUCCESS;
}

#endif
