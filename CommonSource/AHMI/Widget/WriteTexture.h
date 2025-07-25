#include "stdint.h"
#include "string.h"

#ifdef __cplusplus
 extern "C" {
#endif

typedef struct Point
{
	int   X; 
	int   Y;
}Point;

void add_to_bitarray(uint8_t *bitarr, int32_t num);//num means data to be inset
uint8_t get_from_bitarray(uint8_t *bitarr, int32_t num);
void initDataImageTextureRAM(uint32_t dataImageID, uint32_t width, uint32_t height);
void updataDataImageTexture(uint32_t dataImageID, uint8_t* rawDataArr, uint16_t rawDataNum, uint8_t rawDataByteLen, uint8_t rawDataColorType, uint16_t lineID, uint16_t width, uint16_t height, uint8_t rowOrColumn, uint8_t shouldShift);
void initOscilloscopeTextureRAM(uint32_t OscilloscopeID, uint32_t OscilloscopeWidth, uint32_t OscilloscopeHeight);
void updataOscilloscopeTexture_alpha1(uint32_t OscilloscopeID, uint16_t Data, uint16_t DataNum, uint32_t DataMax, uint32_t DataNumMax, uint8_t showMode, uint8_t linewidth);
void drawPointTexture_alpha1(uint32_t OscilloscopeID, uint16_t x, uint16_t y, uint32_t width, uint32_t height, uint8_t linewidth);
void Bezier(uint32_t OscilloscopeID, int x1, int y1, int x2, int y2, int x3, int y3, uint32_t width, uint32_t height, uint8_t linewidth);
void Bresenham(uint32_t OscilloscopeID, int x1, int y1, int x2, int y2, uint32_t width, uint32_t height, uint8_t linewidth);
Point GetFinalPoint_s16(int16_t t, int n);//t 1.2.13
Point GetCenterPoint(Point a, Point b, Point c);
void GetAllPoint(Point a, Point b, Point c);
void GetAllPoint_ForTouchTrack(Point a, Point b, Point c);// .4
void HandleAllPoint(void);

void writeQRCode_alpha1(uint32_t qrID,uint32_t qrwidth, char *QRSourceString);
	 
#ifdef __cplusplus
}
#endif
