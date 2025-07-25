Display模块可以设置为两种输入模式： master模式和slave模式，以及两种输出模式：主动模式
和被动模式。 Master模式主动从帧缓存中读取数据并显示到屏幕， slave模式则是接受AHMI的输入
并且存储于FIFO。主动模式产生display时序并输出。被动模式通过计数计算输出的坐标，并以tile
为单位依次输出到屏幕。 

Master/Slave相对于输入而言，主动/被动相对于输出而言

LCD没有DDRAM的情况下必须使用master主动模式

注：9002的AHMI sourcebuffer地址被硬件固定为片内RAM地址 0x0200C000

Master主动：通过HAL_DISPLAY_SetFrameBuffer接口设置Display主动读取的FrameBuffer首地址，实时模式获取总数据量，实时模式一次获取的长度
Display在此模式下会主动读取FrameBuffer，并且在读取FrameBuffer完成之后产生垂直同步中断（vs_int）；
产生vs_int中断之后，表明Display将读取的当前base_addr寄存器中的值作为FrameBuffer基地址，
所以需要将完整的FrameBuffer信息在vs_int中断产生之前写入Display寄存器


Display流控制：仅限于Master主动模式

AHMI SourceBuffer [0x0200C000 ~ 0x0200C7FF] 2KB
     FrameBuffer:
     1.Display AHB Slave        0x30000000
     2.Display AHB Master       ①SRAM [0x02008000](Dcache for Code QSPI Flash[0x0800000])
                                ②HyperRAM [0x20000000]
        

Display ISP参数相关调整公式：
亮度： 
unsigned char br,bg,bb;//均为0.1.7定点数
           [br  0   0   0]
           [0   bg  0   0]
M_bright = [0   0   bb  0]
           [0   0   0   1]
对比度：
unsigned char c;//c为0.1.7定点数, 注：128计算时也作为0.1.7定点数
           [c   0   0   128-c]
           [0   c   0   128-c]
M_bright = [0   0   c   128-c]
           [0   0   0   1    ]
最终：M_CCM = M_bright × M_sat;

色相和饱和度：
char hue;//hue为1.1.6定点数
Int θ;//表示旋转角度
           [ hue*cosθ   hue*sinθ]
M_YCT =    [-hue*sinθ   hue*cosθ]

伽马矫正：
Y_out=(Y_in/255)^γ×255; //Y_in∈[0,255]，得到Yout后，填入Y Table