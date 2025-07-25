#include <stdio.h> 
#include <stdlib.h>
#include <stdint.h>
#include "string.h"

#ifndef PARSEPARA__H
#define PARSEPARA__H

#ifdef __cplusplus
extern "C" {
#endif
    
#define ParseBufferSize             (6144)


#define MAXSIZEOFTDISPLAYFLASH                  (0xFF8)      //4 * 1024 - 8
#define DisplayParaFlag                         (0x12345678)
#define DisplayParaNumber                       (35)
    
#define MAXSIZEOFHWCONFIGFLASH                  (0xFF8)      //4 * 1024 - 8
#define HWConfigFlag                            (DisplayParaFlag)



#define BINPOLARITY                         (0)
#define BINHPIXEL_VALUE                     (1)
#define BINVLINES_VALUE                     (2)
#define BINHSYNCPW_VALUE                    (3)
#define BINVSYNCPW_VALUE                    (4)
#define BINHFNPRCH_VALUE                    (5)
#define BINVFNPRCH_VALUE                    (6)
#define BINHBKPRCH_VALUE                    (7)
#define BINVBKPRCH_VALUE                    (8)
#define BINCLKINDEVIDE                      (9)
#define BINDISPLAYTNTERFACE                 (10)
#define BINSCREENRAM                  		(11)
#define BINDISPLAYROTATE                    (12)
#define BINRESOLUTIONRATIOTYPE              (13)
#define BINPARA_EXTRA                       (14)
#define BINBACKLIGHTPWMENABLE               (15)
#define BINBACKLIGHTPWMFREQUENCE            (16)
#define BINBACKLIGHTON                      (17)
#define BINBACKLIGHTNORMAL                  (18)
#define BINBACKLIGHTWAIT                    (19)
#define BINBACKLIGHTDURATION                (20)
#define BINBUZZERPWMENABLE                  (21)
#define BINBUZZERFREQUENCE                  (22)
#define BINBUZZERDUTY                       (23)
#define BINBUZZERDUTYDIV                    (24)
#define BINBINBUZZERDURATION                (25)
#define BINTOUCHSCREENTYPE                  (26)
#define BINTOUCHSCREENCHANGEXY              (27)
#define BINTOUCHSCREENROTATE                (28)
#define BINTOUCHSOUND                       (29)
#define BINSYSTEMOPTION                     (30)
#define BINDISPLAYCONTRAST                  (31)
#define BINDISPLAYBRIGHTNESS                (32)
#define BINDISPLAYSATURATION                (33)
#define BINTRADCDELAY                       (34)

typedef struct paraNameTag
{
    uint8_t mName[64];
    uint32_t mSize;
    uint32_t mposition;
}paraName;


uint32_t UpgradeProductPara(uint32_t address, uint32_t* pPara);
#ifdef __cplusplus
}
#endif

#endif

