

#include "ParsePara.h"
#include "em_ct.h"
#include "AHMIInterface.h"
#ifdef CHIP_GC9002_NoHyper
#include "csi_rv32_gcc.h"
#endif

const paraName nameArray[DisplayParaNumber] = {{"polaritypara"            ,  12, BINPOLARITY            },
                                {"hpixel_value"         , 12, BINHPIXEL_VALUE         },
                                {"vlines_value"         , 12, BINVLINES_VALUE         },
                                {"hsyncpw_value"        , 13, BINHSYNCPW_VALUE        },
                                {"vsyncpw_value"        , 13, BINVSYNCPW_VALUE        },
                                {"hfnprch_value"        , 13, BINHFNPRCH_VALUE        },
                                {"vfnprch_value"        , 13, BINVFNPRCH_VALUE        },
                                {"hbkprch_value"        , 13, BINHBKPRCH_VALUE        },
                                {"vbkprch_value"        , 13, BINVBKPRCH_VALUE        },
								{"clkindevide"          , 11, BINCLKINDEVIDE          },
                                {"displayinterface"     , 16, BINDISPLAYTNTERFACE     },
                                {"screenram"      		,  9, BINSCREENRAM      	  },
                                {"displayrotate"        , 13, BINDISPLAYROTATE        },
                                {"resolutionratiotype"  , 19, BINRESOLUTIONRATIOTYPE  },
                                {"para_extra"           , 10, BINPARA_EXTRA           },
                                {"backlightpwmenable"   , 18, BINBACKLIGHTPWMENABLE   },
                                {"backlightpwmfrequence", 21, BINBACKLIGHTPWMFREQUENCE},
                                {"backlighton"          , 11, BINBACKLIGHTON          },
                                {"backlightnormal"      , 15, BINBACKLIGHTNORMAL      },
                                {"backlightwait"        , 13, BINBACKLIGHTWAIT        },
                                {"backlightduration"    , 17, BINBACKLIGHTDURATION    },
                                {"buzzerpwmenable"      , 15, BINBUZZERPWMENABLE      },
                                {"buzzerfrequence"      , 15, BINBUZZERFREQUENCE      },
                                {"buzzerduty"           , 10, BINBUZZERDUTY           },
                                {"buzzerdutydiv"        , 13, BINBUZZERDUTYDIV        },
                                {"buzzerduration"       , 14, BINBINBUZZERDURATION    },
                                {"touchscreentype"      , 15, BINTOUCHSCREENTYPE      },
                                {"tpinterchangexy"      , 15, BINTOUCHSCREENCHANGEXY  },
                                {"tprotate"             ,  8, BINTOUCHSCREENROTATE    },
                                {"touchsound"           , 10, BINTOUCHSOUND           },
                                {"systemoption"         , 12, BINSYSTEMOPTION         },
                                {"displaycontrast"      , 15, BINDISPLAYCONTRAST      },
                                {"displaybrightness"    , 17, BINDISPLAYBRIGHTNESS    },
                                {"displaysaturation"    , 17, BINDISPLAYSATURATION    },
                                {"tradcdelay"           , 10, BINTRADCDELAY           }
                                };

                          
uint32_t ParaCompareString(uint8_t* str1, const uint8_t* str2 , uint32_t size)
{
    uint32_t i = 0;
    
    for(i = 0; i < size; i++)
    {
        if(*(str1 + i) != *(str2 + i))
        {
            return 0;
        }
    }
    
    if(((*(str1 + i) != ' ') && (*(str1 + i) != 0)) || (*(str2 + i) != 0))
        return 0;
    
    return 1;
}
                              
                              

uint8_t* GetFirstLineStart(uint8_t* buffer, uint32_t size)
{
    uint32_t i = 0;
    uint8_t* p = buffer;
    
    for(i = 0; i < size; i++)
    { 
        if((*p >= 'A') && (*p <= 'z'))
        {
            return p;
        }
    
        p++;
    }

    return NULL;
}

uint8_t* GetNextLineStart(uint8_t* buffer, uint32_t size)
{
    uint32_t i = 0;
    uint32_t newlineflag = 0;
    uint8_t* p = buffer;
    
    for(i = 0; i < size; i++)
    {
        if(newlineflag == 0)
        {
            if((*p == 0x0D) && ((*(p + 1) == 0x0A)))
            {
                newlineflag = 1;
                
                if((*(p + 2) >= 'A') && ((*(p + 2) <= 'z')))
                {
                    if((p + 2) < (buffer + size))
                    {
                        return (p + 2);
                    }
                    else
                    {
                        return NULL;
                    }
                }
                else
                {
                    p += 1;
                }
            }
        }
        else
        {
            if((*(p + 2) >= 'A') && ((*(p + 2) <= 'z')))
            {
                if((p + 2) < (buffer + size))
                {
                    return (p + 2);
                }
                else
                {
                    return NULL;
                }
            }
            else
            {
                p += 1;
            }
        }
        
        p++;
    }
    
    return NULL;
}

uint8_t* GetLineData(uint8_t* buffer, uint32_t bufferSize, uint8_t* lineBuffer, uint32_t* lineSize)
{
    uint32_t i = 0;
//    uint32_t newlineflag = 0;
    uint8_t* p = buffer;
    
    *lineSize = 0;
    
    for(i = 0; i < bufferSize; i++)
    {
        if(*p != '\r')
        {
            p++;
            (*lineSize)++;
        }
        else
        {
            (*lineSize)++;
            (*lineSize)++;
            break;
        }
    }
    
//    for(i = 0; i < (*lineSize); i++)
//    {
//        *(lineBuffer++) = *(buffer++);
//    }
    
    return NULL;
}

uint32_t CheckLineData(uint8_t* lineBuffer, uint32_t lineSize)
{
    uint32_t i = 0;
    uint32_t colonFlag = 0;
    uint8_t temp = 0;
    
    lineSize -= 2;
    
    for(i = 0; i < lineSize;i++)
    {
        temp = *(lineBuffer++);
        
        if(temp == ':')
        {
            colonFlag = 1;
            continue;
        }
        
        if(colonFlag == 1)
        {
            if(((temp > '9') || (temp < '0')) && (temp != ' '))
            {
                return 0;
            }
        }
    }
    
    return 1;
}

uint8_t* TextPreprogress(uint8_t* address, uint32_t size, uint8_t* buffer, uint32_t* pcounter)
{
    uint8_t* bufferPosi = NULL;
    uint8_t* bufferPosilater = NULL;
    uint8_t* pLine = 0;
    uint32_t lineCnt = 0;
    uint32_t i = 0;
    uint32_t annotationFlag = 0;
    uint32_t LRFlag = 0;
//    uint32_t bufferCnt = 0;
    uint32_t spaceCounter = 0;
    uint32_t colonCounter = 0;
    uint8_t temp = 0;
    uint8_t tempNext = 0;
    
    if((size + 2) > ParseBufferSize)
        return 0;
    
    if(buffer == NULL)
        return 0;
    
    bufferPosi = buffer;
    
    for(i = 0; i < size; i++)
    {
        temp = *(address + i);
        
        if((temp == ' ') || (temp == '\t'))
        {
            if((*(bufferPosi - 1) != ' ') && (*(bufferPosi - 1) != '\n'))
            {
                *(bufferPosi++) = ' ';
            }
            
            continue;
        }
        
        /* windows style. */
        if(temp == '\r')
        {
            tempNext = *(address + i + 1);
            if(tempNext == '\n')
            {
                i++;
                if(LRFlag == 0)
                {
                    if(*(bufferPosi - 1) != ' ')
                    {
                        *(bufferPosi++) = ' ';
                    }
                    
                    *(bufferPosi++) = '\r';
                    *(bufferPosi++) = '\n';
    
                    LRFlag = 1;   
                }
                
                annotationFlag = 0;
                
            }
            continue;
        }
        /* unix style. */
        else if(temp == '\n')
        {
            if(LRFlag == 0)
            {
                if(*(bufferPosi - 1) != ' ')
                {
                    *(bufferPosi++) = ' ';
                }
                
                *(bufferPosi++) = '\r';
                *(bufferPosi++) = '\n';
            
                LRFlag = 1;  
            }
            
            annotationFlag = 0;
            
            continue;
        }
        
        
        
        /* Get annotation flag symbel.*/
        if(temp == '%')
        {
            annotationFlag = 1;
            continue;
        }
        
        /* Ignore annotation.*/
        if(annotationFlag == 1)
        {
            /* windows style. */
            if(temp == '\r')
            {
                tempNext = *(address + i + 1);
                if(tempNext == '\n')
                {
                    
                    if(*(bufferPosi - 1) != ' ')
                    {
                        *(bufferPosi++) = ' ';
                    }
                    
                    *(bufferPosi++) = '\r';
                    *(bufferPosi++) = '\n';
                    
                    i++;
                    
                    annotationFlag = 0;
                    
                    LRFlag = 1;
                }
            }
            /* unix style. */
            else if(temp == '\n')
            {
                if(*(bufferPosi - 1) != ' ')
                {
                    *(bufferPosi++) = ' ';
                }
                    
                *(bufferPosi++) = '\r';
                *(bufferPosi++) = '\n';
                
                annotationFlag = 0;
                
                LRFlag = 1;
            }
//            /* Mac style. */
//            else if(temp != '\r')
//            {
////                *(bufferPosi++) = '\r';
//                *(bufferPosi++) = '\n';
//            }
            
            
            continue;
            
        }
        
        LRFlag = 0;
        
        if(temp == ':')
        {
            *(bufferPosi++) = ' ';
            *(bufferPosi++) = temp;
            *(bufferPosi++) = ' ';
            continue;
        }
        
        if((temp >= 'A') && (temp <= 'Z'))
            temp += 0x20;   
        
        *(bufferPosi++) = temp;
    }
    
    if(*(bufferPosi - 1) != '\n')
    {
        if(*(bufferPosi - 1) != ' ')
        {
            *(bufferPosi++) = ' ';
        }
        *(bufferPosi++) = '\r';
        *(bufferPosi++) = '\n';
    }
    
    *pcounter = bufferPosi - buffer;
    
    bufferPosi = buffer;
    bufferPosilater = buffer;
    
    pLine = bufferPosi;
    lineCnt = 0;
    for(i = 0; i < *pcounter; i++)
    {
        
        temp = *(bufferPosi++);
        lineCnt++;
        
        if(temp == ' ')
        {
            spaceCounter++;
        }
        
        if(temp == ':')
        {
            colonCounter++;
        }
        
        if(temp == '\r')
        {
            if((spaceCounter == 3) && (colonCounter == 1))
            {
                for(i = 0;i < lineCnt;i++)
                {
                    *(bufferPosilater++) = *(pLine++);
                }
                
                
                *(bufferPosilater++) = '\n';   
                
                pLine = bufferPosi;
            }
            
            bufferPosi++;
            pLine = bufferPosi;
            lineCnt = 0;
            spaceCounter = 0;
            colonCounter = 0;
        }
        
    }
    
    *pcounter = bufferPosilater - buffer;
    
    
    for(;bufferPosilater < bufferPosi;)
    {
        *(bufferPosilater++) = 0; 
    }
    
    return buffer;
}


__attribute__((optimize(0))) uint32_t UpgradeProductPara(uint32_t address, uint32_t* pPara)
{
//	uint32_t EraseOfSector;
	uint32_t sizeOfFile = 0;
	uint32_t i;
    uint32_t count1;  
    uint32_t value = 0;  
    uint32_t framesize = 0;  
    uint32_t lineSize = 0; 
    uint32_t stringSize = 0;     
    uint8_t tempLine[64] = {0};

    
//    uint8_t* newPointer = 0;
    uint8_t* oldPointer = 0;
    uint8_t* buffer = 0;
	
	#ifdef CHIP_GC9002_NoHyper
    uint32_t addr = 0;
	#endif
    uint32_t paraCounter = 0;
    
//    uint32_t flashsize = 0;
    uint32_t charCounter = 0;
    

    if(*(uint32_t*)(address) != DisplayParaFlag)
        return (EM_enErrRetModularInit | EM_ErrRetInit_GetProductDataErr);
    
    /*Get file size.*/
	sizeOfFile = *(uint32_t*)(address + 4);
    
    if(sizeOfFile > MAXSIZEOFTDISPLAYFLASH)
		return (EM_enErrRetModularInit | EM_ErrRetInit_GetProductDataErr);
    
    if((*(uint32_t*)(address + 8) == 0xFFFFFFFF) || (*(uint32_t*)(address + 8) == 0))
		return (EM_enErrRetModularInit | EM_ErrRetInit_GetProductDataErr);
    

    if(sizeOfFile <= MAXSIZEOFTDISPLAYFLASH)
    {     

        buffer = malloc(ParseBufferSize);
		#ifdef CHIP_GC9002_NoHyper
		addr = (uint32_t)malloc(4096);
		#endif
		
        if(buffer == 0)
			return (EM_enErrRetModularInit | EM_ErrRetInit_GetProductDataErr);
        
        memset(buffer, 0, ParseBufferSize);
        
        TextPreprogress((uint8_t*)(address + 8), sizeOfFile, buffer, &charCounter);
        
        oldPointer = buffer;
        
        sizeOfFile = charCounter;
        
		#ifdef CHIP_GC9002_NoHyper
		extern void set_sp(uint32_t sp);
		addr += 4096;
		__set_MSCRATCH(__get_SP());
		set_sp(addr);
		//__set_SP(addr);
		#endif
        while(sizeOfFile > 0)
        {
            
            GetLineData(oldPointer, charCounter, tempLine, &lineSize);
        
            if(CheckLineData(oldPointer, lineSize))
            {
                memset(tempLine, 0, 64);
                
                /* Get a parameter from a line.*/
                count1 = sscanf((const char *)oldPointer, "%[^:] : %d ", tempLine, &value);
                
                if(count1 == 2)
                {
                    stringSize = strlen((const char*)tempLine);
                    
                    for(i = 0; i < DisplayParaNumber; i++)
                    {
                        if(nameArray[i].mSize == (stringSize - 1))
                        {
                            if(ParaCompareString(tempLine, nameArray[i].mName, nameArray[i].mSize))
                            {
                                framesize++;
                        
                                /* Write paramemter. */
                                *(pPara + nameArray[i].mposition) = value;
                        
                                paraCounter++;
                                
                                break;
                            }
                        }
         
                    }
                }
            }
        
        
            sizeOfFile -= lineSize; 
        
            oldPointer = oldPointer + lineSize;
            
        }
		#ifdef CHIP_GC9002_NoHyper
		set_sp(__get_MSCRATCH());
		//__set_SP(__get_MSCRATCH());
        addr -= 4096;
		free((uint8_t*)addr);
		#endif
        free(buffer);
              
    }
    
    
    return paraCounter;
}
