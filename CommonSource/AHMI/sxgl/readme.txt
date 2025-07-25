1.接口api

//each time rotate the cube in 15 degree
//then generate the sourcebuffer
//*sourceBufferAddr is the address of sourcebuffer output
//*sourceShift is the input of current sourcebuffer shift
int generate3dSourcebuffer(
	unsigned int *sourceBufferAddr, 
	unsigned int* sourceShift
	);

//set the address and width/height of input texture
void setTexAttr(
	unsigned int texIndex, //0-outside cube, 1-video 2-inside cube
	unsigned int type    , //@ref sourcebuffer texture type
	unsigned int width   , // original texture width
	unsigned int height  , // original texture height
	unsigned int address , //need to set as texture's address
	unsigned int source  , //0-flash, 1-ddr
	unsigned int sbWidth , //extended texture width
	unsigned int sbHeight  //extended texture height
	);

2.使用说明

2.1 将source和include文件夹下的文件添加到工程目录，包含目录设置到include目录下。
2.2 调用setTexAttr设置纹理属性
     其中，
	texIndex -- 0代表设置外围房间的纹理，1代表设置video纹理，2代表设置内部正方体的纹理。
	type - 与sourcebuffer定义一致。
     	address - 为纹理地址，flash纹理需要加偏移量。ddr纹理不需要
     	如果source为0，代表纹理来源于flash；如果source为1，代表纹理来源于ddr，偏移量为0x60000000
	sbWidth - 写入sourcebuffer的纹理宽度
        sbHeight - 写入sourcebuffer的纹理高度
2.3 将* sourceShift赋值为当前sourcebuffer的偏移，调用generate3dSourcebuffer函数生成3d场景的sourcebuffer数据，数据存储于*sourceBufferAddr中。
2.4 将sourcebuffer数据发送AHMI并进行绘制。

