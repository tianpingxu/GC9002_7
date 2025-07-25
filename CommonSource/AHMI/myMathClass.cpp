////////////////////////////////////////////////////////////////////////////////
// Company:		 Graphichina Inc
//
// Create Date:   2016/03/21
// File Name:     myMathClass.cpp
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2016/03/21 by Zhou Yuzhi 
// Additional Comments:
//    the math function
// 
////////////////////////////////////////////////////////////////////////////////

#include "myMathClass.h"
#include "publicInclude.h"
#if defined(CHIP_GC9002)
#include "csi_ahmi_fix.h"
#endif

#ifdef Use_SinTanTable
//sintable. 1.11.20
//every 0.1 degree
const s32 sinTable[901] = {
	0x00000000,0x00000726,0x00000e4c,0x00001572,0x00001c98,0x000023be,0x00002ae4,0x0000320a,0x00003930,0x00004056,
    0x0000477c,0x00004ea1,0x000055c7,0x00005ced,0x00006412,0x00006b38,0x0000725d,0x00007983,0x000080a8,0x000087cd,
    0x00008ef2,0x00009617,0x00009d3c,0x0000a461,0x0000ab85,0x0000b2aa,0x0000b9ce,0x0000c0f2,0x0000c816,0x0000cf3a,
    0x0000d65e,0x0000dd81,0x0000e4a5,0x0000ebc8,0x0000f2eb,0x0000fa0e,0x00010130,0x00010853,0x00010f75,0x00011697,
    0x00011db8,0x000124da,0x00012bfb,0x0001331c,0x00013a3d,0x0001415e,0x0001487e,0x00014f9e,0x000156be,0x00015dde,
    0x000164fd,0x00016c1c,0x0001733b,0x00017a59,0x00018177,0x00018895,0x00018fb3,0x000196d0,0x00019ded,0x0001a509,
    0x0001ac26,0x0001b341,0x0001ba5d,0x0001c178,0x0001c893,0x0001cfae,0x0001d6c8,0x0001dde2,0x0001e4fb,0x0001ec14,
    0x0001f32d,0x0001fa45,0x0002015d,0x00020874,0x00020f8b,0x000216a2,0x00021db8,0x000224ce,0x00022be4,0x000232f9,
    0x00023a0d,0x00024121,0x00024835,0x00024f48,0x0002565b,0x00025d6d,0x0002647f,0x00026b90,0x000272a1,0x000279b1,
    0x000280c1,0x000287d0,0x00028edf,0x000295ed,0x00029cfb,0x0002a408,0x0002ab15,0x0002b221,0x0002b92d,0x0002c038,
    0x0002c743,0x0002ce4d,0x0002d556,0x0002dc5f,0x0002e368,0x0002ea6f,0x0002f176,0x0002f87d,0x0002ff83,0x00030688,
    0x00030d8d,0x00031491,0x00031b95,0x00032298,0x0003299a,0x0003309c,0x0003379d,0x00033e9d,0x0003459d,0x00034c9c,
    0x0003539b,0x00035a98,0x00036196,0x00036892,0x00036f8e,0x00037689,0x00037d83,0x0003847d,0x00038b76,0x0003926e,
    0x00039966,0x0003a05d,0x0003a753,0x0003ae48,0x0003b53d,0x0003bc31,0x0003c324,0x0003ca16,0x0003d108,0x0003d7f9,
    0x0003dee9,0x0003e5d8,0x0003ecc7,0x0003f3b5,0x0003faa2,0x0004018e,0x00040879,0x00040f64,0x0004164e,0x00041d37,
    0x0004241f,0x00042b06,0x000431ed,0x000438d2,0x00043fb7,0x0004469b,0x00044d7e,0x00045461,0x00045b42,0x00046223,
    0x00046902,0x00046fe1,0x000476bf,0x00047d9c,0x00048478,0x00048b53,0x0004922d,0x00049907,0x00049fdf,0x0004a6b7,
    0x0004ad8d,0x0004b463,0x0004bb38,0x0004c20c,0x0004c8de,0x0004cfb0,0x0004d681,0x0004dd51,0x0004e420,0x0004eaee,
    0x0004f1bb,0x0004f887,0x0004ff52,0x0005061c,0x00050ce6,0x000513ae,0x00051a75,0x0005213b,0x00052800,0x00052ec4,
    0x00053586,0x00053c48,0x00054309,0x000549c9,0x00055088,0x00055745,0x00055e02,0x000564bd,0x00056b78,0x00057231,
    0x000578ea,0x00057fa1,0x00058657,0x00058d0c,0x000593c0,0x00059a73,0x0005a124,0x0005a7d5,0x0005ae84,0x0005b532,
    0x0005bbe0,0x0005c28c,0x0005c936,0x0005cfe0,0x0005d689,0x0005dd30,0x0005e3d6,0x0005ea7b,0x0005f11f,0x0005f7c2,
    0x0005fe63,0x00060503,0x00060ba2,0x00061240,0x000618dd,0x00061f78,0x00062612,0x00062cab,0x00063343,0x000639da,
    0x0006406f,0x00064703,0x00064d96,0x00065427,0x00065ab7,0x00066146,0x000667d4,0x00066e60,0x000674eb,0x00067b75,
    0x000681fe,0x00068885,0x00068f0b,0x00069590,0x00069c13,0x0006a295,0x0006a916,0x0006af95,0x0006b613,0x0006bc90,
    0x0006c30b,0x0006c985,0x0006cffd,0x0006d675,0x0006dceb,0x0006e35f,0x0006e9d2,0x0006f044,0x0006f6b4,0x0006fd23,
    0x00070391,0x000709fd,0x00071068,0x000716d1,0x00071d39,0x000723a0,0x00072a05,0x00073069,0x000736cb,0x00073d2c,
    0x0007438b,0x000749e9,0x00075045,0x000756a0,0x00075cfa,0x00076352,0x000769a9,0x00076ffe,0x00077651,0x00077ca3,
    0x000782f4,0x00078943,0x00078f91,0x000795dd,0x00079c28,0x0007a271,0x0007a8b8,0x0007aefe,0x0007b543,0x0007bb86,
    0x0007c1c7,0x0007c807,0x0007ce45,0x0007d482,0x0007dabd,0x0007e0f7,0x0007e72f,0x0007ed66,0x0007f39a,0x0007f9ce,
    0x0007ffff,0x00080630,0x00080c5e,0x0008128b,0x000818b6,0x00081ee0,0x00082508,0x00082b2f,0x00083153,0x00083777,
    0x00083d98,0x000843b8,0x000849d6,0x00084ff3,0x0008560e,0x00085c27,0x0008623f,0x00086854,0x00086e69,0x0008747b,
    0x00087a8c,0x0008809b,0x000886a9,0x00088cb5,0x000892bf,0x000898c7,0x00089ece,0x0008a4d3,0x0008aad6,0x0008b0d7,
    0x0008b6d7,0x0008bcd5,0x0008c2d1,0x0008c8cc,0x0008cec4,0x0008d4bb,0x0008dab1,0x0008e0a4,0x0008e696,0x0008ec86,
    0x0008f274,0x0008f860,0x0008fe4b,0x00090433,0x00090a1a,0x00090fff,0x000915e3,0x00091bc4,0x000921a4,0x00092782,
    0x00092d5e,0x00093338,0x00093911,0x00093ee7,0x000944bc,0x00094a8f,0x00095060,0x0009562f,0x00095bfc,0x000961c7,
    0x00096791,0x00096d59,0x0009731e,0x000978e2,0x00097ea4,0x00098464,0x00098a23,0x00098fdf,0x00099599,0x00099b52,
    0x0009a108,0x0009a6bd,0x0009ac70,0x0009b220,0x0009b7cf,0x0009bd7c,0x0009c327,0x0009c8d0,0x0009ce77,0x0009d41c,
    0x0009d9bf,0x0009df61,0x0009e500,0x0009ea9d,0x0009f038,0x0009f5d1,0x0009fb69,0x000a00fe,0x000a0691,0x000a0c22,
    0x000a11b2,0x000a173f,0x000a1cca,0x000a2253,0x000a27db,0x000a2d60,0x000a32e3,0x000a3864,0x000a3de3,0x000a4360,
    0x000a48db,0x000a4e54,0x000a53cb,0x000a5940,0x000a5eb2,0x000a6423,0x000a6992,0x000a6efe,0x000a7469,0x000a79d1,
    0x000a7f37,0x000a849b,0x000a89fd,0x000a8f5d,0x000a94bb,0x000a9a17,0x000a9f71,0x000aa4c8,0x000aaa1d,0x000aaf71,
    0x000ab4c2,0x000aba11,0x000abf5e,0x000ac4a8,0x000ac9f1,0x000acf37,0x000ad47b,0x000ad9bd,0x000adefd,0x000ae43b,
    0x000ae977,0x000aeeb0,0x000af3e7,0x000af91c,0x000afe4f,0x000b0380,0x000b08ae,0x000b0dda,0x000b1304,0x000b182c,
    0x000b1d52,0x000b2275,0x000b2796,0x000b2cb5,0x000b31d2,0x000b36ec,0x000b3c04,0x000b411a,0x000b462e,0x000b4b3f,
    0x000b504f,0x000b555c,0x000b5a66,0x000b5f6f,0x000b6475,0x000b6979,0x000b6e7a,0x000b737a,0x000b7877,0x000b7d72,
    0x000b826a,0x000b8760,0x000b8c54,0x000b9145,0x000b9635,0x000b9b22,0x000ba00c,0x000ba4f5,0x000ba9db,0x000baebe,
    0x000bb39f,0x000bb87e,0x000bbd5b,0x000bc235,0x000bc70d,0x000bcbe3,0x000bd0b6,0x000bd587,0x000bda55,0x000bdf22,
    0x000be3eb,0x000be8b3,0x000bed78,0x000bf23a,0x000bf6fb,0x000bfbb8,0x000c0074,0x000c052d,0x000c09e4,0x000c0e98,
    0x000c134a,0x000c17f9,0x000c1ca6,0x000c2151,0x000c25f9,0x000c2a9f,0x000c2f42,0x000c33e3,0x000c3882,0x000c3d1e,
    0x000c41b7,0x000c464e,0x000c4ae3,0x000c4f75,0x000c5405,0x000c5893,0x000c5d1d,0x000c61a6,0x000c662c,0x000c6aaf,
    0x000c6f30,0x000c73af,0x000c782b,0x000c7ca4,0x000c811b,0x000c8590,0x000c8a02,0x000c8e71,0x000c92de,0x000c9749,
    0x000c9bb1,0x000ca016,0x000ca479,0x000ca8da,0x000cad37,0x000cb193,0x000cb5ec,0x000cba42,0x000cbe96,0x000cc2e7,
    0x000cc736,0x000ccb82,0x000ccfcb,0x000cd412,0x000cd857,0x000cdc99,0x000ce0d8,0x000ce515,0x000ce94f,0x000ced86,
    0x000cf1bb,0x000cf5ee,0x000cfa1e,0x000cfe4b,0x000d0275,0x000d069d,0x000d0ac3,0x000d0ee6,0x000d1306,0x000d1724,
    0x000d1b3f,0x000d1f57,0x000d236d,0x000d2780,0x000d2b91,0x000d2f9e,0x000d33aa,0x000d37b2,0x000d3bb8,0x000d3fbc,
    0x000d43bc,0x000d47ba,0x000d4bb6,0x000d4faf,0x000d53a5,0x000d5798,0x000d5b89,0x000d5f77,0x000d6362,0x000d674b,
    0x000d6b31,0x000d6f15,0x000d72f5,0x000d76d4,0x000d7aaf,0x000d7e88,0x000d825d,0x000d8631,0x000d8a01,0x000d8dcf,
    0x000d919a,0x000d9563,0x000d9929,0x000d9cec,0x000da0ac,0x000da46a,0x000da824,0x000dabdd,0x000daf92,0x000db345,
    0x000db6f5,0x000dbaa2,0x000dbe4c,0x000dc1f4,0x000dc599,0x000dc93b,0x000dccdb,0x000dd077,0x000dd411,0x000dd7a9,
    0x000ddb3d,0x000ddecf,0x000de25e,0x000de5ea,0x000de973,0x000decfa,0x000df07d,0x000df3fe,0x000df77d,0x000dfaf8,
    0x000dfe71,0x000e01e7,0x000e055a,0x000e08ca,0x000e0c37,0x000e0fa2,0x000e130a,0x000e166f,0x000e19d1,0x000e1d31,
    0x000e208d,0x000e23e7,0x000e273e,0x000e2a92,0x000e2de3,0x000e3132,0x000e347d,0x000e37c6,0x000e3b0c,0x000e3e4f,
    0x000e4190,0x000e44cd,0x000e4808,0x000e4b3f,0x000e4e74,0x000e51a6,0x000e54d5,0x000e5802,0x000e5b2b,0x000e5e52,
    0x000e6175,0x000e6496,0x000e67b4,0x000e6acf,0x000e6de7,0x000e70fd,0x000e740f,0x000e771f,0x000e7a2b,0x000e7d35,
    0x000e803c,0x000e8340,0x000e8641,0x000e893f,0x000e8c3b,0x000e8f33,0x000e9229,0x000e951b,0x000e980b,0x000e9af8,
    0x000e9de1,0x000ea0c8,0x000ea3ac,0x000ea68d,0x000ea96b,0x000eac47,0x000eaf1f,0x000eb1f4,0x000eb4c7,0x000eb796,
    0x000eba63,0x000ebd2c,0x000ebff3,0x000ec2b7,0x000ec578,0x000ec835,0x000ecaf0,0x000ecda8,0x000ed05d,0x000ed30f,
    0x000ed5be,0x000ed86a,0x000edb13,0x000eddba,0x000ee05d,0x000ee2fd,0x000ee59a,0x000ee835,0x000eeacc,0x000eed60,
    0x000eeff2,0x000ef280,0x000ef50b,0x000ef794,0x000efa19,0x000efc9b,0x000eff1b,0x000f0197,0x000f0411,0x000f0687,
    0x000f08fb,0x000f0b6b,0x000f0dd8,0x000f1043,0x000f12aa,0x000f150f,0x000f1770,0x000f19cf,0x000f1c2a,0x000f1e82,
    0x000f20d8,0x000f232a,0x000f2579,0x000f27c5,0x000f2a0f,0x000f2c55,0x000f2e98,0x000f30d8,0x000f3315,0x000f354f,
    0x000f3787,0x000f39bb,0x000f3bec,0x000f3e19,0x000f4044,0x000f426c,0x000f4491,0x000f46b3,0x000f48d1,0x000f4aed,
    0x000f4d06,0x000f4f1b,0x000f512e,0x000f533d,0x000f554a,0x000f5753,0x000f5959,0x000f5b5c,0x000f5d5c,0x000f5f59,
    0x000f6153,0x000f634a,0x000f653e,0x000f672f,0x000f691d,0x000f6b07,0x000f6cef,0x000f6ed3,0x000f70b5,0x000f7293,
    0x000f746e,0x000f7646,0x000f781b,0x000f79ed,0x000f7bbc,0x000f7d88,0x000f7f51,0x000f8116,0x000f82d9,0x000f8498,
    0x000f8654,0x000f880e,0x000f89c4,0x000f8b77,0x000f8d26,0x000f8ed3,0x000f907d,0x000f9224,0x000f93c7,0x000f9567,
    0x000f9705,0x000f989f,0x000f9a36,0x000f9bca,0x000f9d5a,0x000f9ee8,0x000fa073,0x000fa1fa,0x000fa37e,0x000fa500,
    0x000fa67e,0x000fa7f9,0x000fa970,0x000faae5,0x000fac57,0x000fadc5,0x000faf30,0x000fb098,0x000fb1fe,0x000fb35f,
    0x000fb4be,0x000fb61a,0x000fb772,0x000fb8c8,0x000fba1a,0x000fbb69,0x000fbcb5,0x000fbdfe,0x000fbf43,0x000fc086,
    0x000fc1c5,0x000fc301,0x000fc43b,0x000fc571,0x000fc6a3,0x000fc7d3,0x000fc8ff,0x000fca29,0x000fcb4f,0x000fcc72,
    0x000fcd92,0x000fceaf,0x000fcfc8,0x000fd0de,0x000fd1f2,0x000fd302,0x000fd40f,0x000fd518,0x000fd61f,0x000fd723,
    0x000fd823,0x000fd920,0x000fda1a,0x000fdb11,0x000fdc04,0x000fdcf5,0x000fdde2,0x000fdecc,0x000fdfb3,0x000fe097,
    0x000fe178,0x000fe255,0x000fe32f,0x000fe406,0x000fe4da,0x000fe5ab,0x000fe679,0x000fe743,0x000fe80a,0x000fe8ce,
    0x000fe98f,0x000fea4d,0x000feb08,0x000febbf,0x000fec73,0x000fed24,0x000fedd2,0x000fee7d,0x000fef24,0x000fefc8,
    0x000ff069,0x000ff107,0x000ff1a2,0x000ff23a,0x000ff2ce,0x000ff35f,0x000ff3ed,0x000ff478,0x000ff500,0x000ff584,
    0x000ff605,0x000ff683,0x000ff6fe,0x000ff776,0x000ff7ea,0x000ff85c,0x000ff8ca,0x000ff935,0x000ff99d,0x000ffa01,
    0x000ffa62,0x000ffac1,0x000ffb1c,0x000ffb73,0x000ffbc8,0x000ffc19,0x000ffc68,0x000ffcb3,0x000ffcfb,0x000ffd3f,
    0x000ffd81,0x000ffdbf,0x000ffdfa,0x000ffe32,0x000ffe67,0x000ffe98,0x000ffec6,0x000ffef2,0x000fff1a,0x000fff3e,
    0x000fff60,0x000fff7e,0x000fff99,0x000fffb1,0x000fffc6,0x000fffd8,0x000fffe6,0x000ffff1,0x000ffff9,0x000ffffe,
    0x00100000,
};

//1.11.20
//every 0.25 degree
const s32 tanTable[90*4] = {
	0x00000000,0x000011df,0x000023be,0x0000359e,0x0000477e,0x00005960,0x00006b41,0x00007d24,0x00008f09,0x0000a0ee,
    0x0000b2d5,0x0000c4be,0x0000d6a9,0x0000e896,0x0000fa85,0x00010c77,0x00011e6b,0x00013062,0x0001425c,0x00015459,
    0x0001665a,0x0001785e,0x00018a66,0x00019c72,0x0001ae81,0x0001c095,0x0001d2ae,0x0001e4cb,0x0001f6ec,0x00020913,
    0x00021b3f,0x00022d70,0x00023fa7,0x000251e4,0x00026426,0x0002766f,0x000288be,0x00029b13,0x0002ad6f,0x0002bfd2,
    0x0002d23c,0x0002e4ad,0x0002f726,0x000309a6,0x00031c2e,0x00032ebe,0x00034157,0x000353f8,0x000366a1,0x00037954,
    0x00038c0f,0x00039ed4,0x0003b1a2,0x0003c47a,0x0003d75c,0x0003ea48,0x0003fd3f,0x00041040,0x0004234c,0x00043663,
    0x00044985,0x00045cb2,0x00046feb,0x00048331,0x00049682,0x0004a9e0,0x0004bd4a,0x0004d0c1,0x0004e445,0x0004f7d7,
    0x00050b76,0x00051f23,0x000532de,0x000546a8,0x00055a80,0x00056e67,0x0005825d,0x00059663,0x0005aa78,0x0005be9d,
    0x0005d2d2,0x0005e718,0x0005fb6e,0x00060fd5,0x0006244e,0x000638d8,0x00064d75,0x00066223,0x000676e4,0x00068bb7,
    0x0006a09e,0x0006b598,0x0006caa6,0x0006dfc7,0x0006f4fd,0x00070a48,0x00071fa8,0x0007351d,0x00074aa7,0x00076048,
    0x000775ff,0x00078bcc,0x0007a1b1,0x0007b7ad,0x0007cdc0,0x0007e3ec,0x0007fa30,0x0008108d,0x00082704,0x00083d94,
    0x0008543e,0x00086b02,0x000881e1,0x000898dc,0x0008aff2,0x0008c724,0x0008de73,0x0008f5de,0x00090d67,0x0009250e,
    0x00093cd3,0x000954b7,0x00096cba,0x000984dd,0x00099d20,0x0009b583,0x0009ce08,0x0009e6ae,0x0009ff77,0x000a1862,
    0x000a3170,0x000a4aa2,0x000a63f9,0x000a7d74,0x000a9715,0x000ab0dc,0x000acac9,0x000ae4dd,0x000aff1a,0x000b197f,
    0x000b340c,0x000b4ec4,0x000b69a5,0x000b84b2,0x000b9feb,0x000bbb4f,0x000bd6e1,0x000bf2a0,0x000c0e8e,0x000c2aab,
    0x000c46f8,0x000c6376,0x000c8025,0x000c9d06,0x000cba1b,0x000cd763,0x000cf4e0,0x000d1292,0x000d307b,0x000d4e9b,
    0x000d6cf3,0x000d8b85,0x000daa50,0x000dc956,0x000de899,0x000e0818,0x000e27d5,0x000e47d1,0x000e680e,0x000e888b,
    0x000ea94a,0x000eca4d,0x000eeb94,0x000f0d21,0x000f2ef5,0x000f5111,0x000f7376,0x000f9625,0x000fb920,0x000fdc69,
    0x000fffff,0x001023e6,0x0010481e,0x00106ca9,0x00109188,0x0010b6bc,0x0010dc48,0x0011022d,0x0011286c,0x00114f07,
    0x001175ff,0x00119d58,0x0011c511,0x0011ed2e,0x001215af,0x00123e97,0x001267e8,0x001291a4,0x0012bbcc,0x0012e663,
    0x0013116c,0x00133ce7,0x001368d8,0x00139540,0x0013c223,0x0013ef82,0x00141d60,0x00144bc0,0x00147aa4,0x0014aa0e,
    0x0014da02,0x00150a83,0x00153b93,0x00156d35,0x00159f6d,0x0015d23d,0x001605a9,0x001639b3,0x00166e61,0x0016a3b4,
    0x0016d9b1,0x0017105c,0x001747b7,0x00177fc8,0x0017b891,0x0017f218,0x00182c61,0x00186770,0x0018a349,0x0018dff1,
    0x00191d6e,0x00195bc4,0x00199af8,0x0019db0f,0x001a1c10,0x001a5dff,0x001aa0e3,0x001ae4c1,0x001b29a0,0x001b6f86,
    0x001bb67a,0x001bfe83,0x001c47a7,0x001c91ef,0x001cdd61,0x001d2a05,0x001d77e4,0x001dc706,0x001e1774,0x001e6937,
    0x001ebc57,0x001f10e0,0x001f66da,0x001fbe50,0x0020174e,0x002071dd,0x0020ce0b,0x00212be3,0x00218b71,0x0021ecc3,
    0x00224fe6,0x0022b4e9,0x00231bda,0x002384c8,0x0023efc4,0x00245cde,0x0024cc27,0x00253db2,0x0025b192,0x002627d9,
    0x0026a09e,0x00271bf5,0x002799f4,0x00281ab4,0x00289e4d,0x002924d8,0x0029ae71,0x002a3b35,0x002acb3f,0x002b5eb1,
    0x002bf5aa,0x002c904e,0x002d2ebf,0x002dd124,0x002e77a5,0x002f226d,0x002fd1a7,0x00308582,0x00313e31,0x0031fbe6,
    0x0032beda,0x00338746,0x00345569,0x00352984,0x003603dd,0x0036e4bd,0x0037cc73,0x0038bb52,0x0039b1b2,0x003aaff4,
    0x003bb67a,0x003cc5b2,0x003dde0e,0x003f0009,0x00402c28,0x004162f8,0x0042a511,0x0043f317,0x00454db9,0x0046b5b7,
    0x00482bdc,0x0049b108,0x004b462a,0x004cec47,0x004ea47b,0x00506ffb,0x00525017,0x00544640,0x00565409,0x00587b2d,
    0x005abd91,0x005d1d50,0x005f9cbb,0x00623e62,0x00650520,0x0067f423,0x006b0ef9,0x006e599e,0x0071d88d,0x007590d6,
    0x00798835,0x007dc530,0x00824f3e,0x00872eed,0x008c6e21,0x0092184f,0x00983ad6,0x009ee569,0x00a62a96,0x00ae2072,
    0x00b6e17e,0x00c08dd0,0x00cb4c9d,0x00d74e56,0x00e4cf87,0x00f41ce2,0x01059901,0x0119c4d9,0x01314c55,0x014d19ea,
    0x016e75d2,0x01973a29,0x01ca2e17,0x020baf28,0x026303ed,0x02dd4505,0x0394a3ad,0x04c63d78,0x07296b18,0x0e52e807
};
#endif


//1.11.20
//every 0.25 degree
const s32 tanTable[90*4] = {
	0x00000000,0x000011df,0x000023be,0x0000359e,0x0000477e,0x00005960,0x00006b41,0x00007d24,0x00008f09,0x0000a0ee,
    0x0000b2d5,0x0000c4be,0x0000d6a9,0x0000e896,0x0000fa85,0x00010c77,0x00011e6b,0x00013062,0x0001425c,0x00015459,
    0x0001665a,0x0001785e,0x00018a66,0x00019c72,0x0001ae81,0x0001c095,0x0001d2ae,0x0001e4cb,0x0001f6ec,0x00020913,
    0x00021b3f,0x00022d70,0x00023fa7,0x000251e4,0x00026426,0x0002766f,0x000288be,0x00029b13,0x0002ad6f,0x0002bfd2,
    0x0002d23c,0x0002e4ad,0x0002f726,0x000309a6,0x00031c2e,0x00032ebe,0x00034157,0x000353f8,0x000366a1,0x00037954,
    0x00038c0f,0x00039ed4,0x0003b1a2,0x0003c47a,0x0003d75c,0x0003ea48,0x0003fd3f,0x00041040,0x0004234c,0x00043663,
    0x00044985,0x00045cb2,0x00046feb,0x00048331,0x00049682,0x0004a9e0,0x0004bd4a,0x0004d0c1,0x0004e445,0x0004f7d7,
    0x00050b76,0x00051f23,0x000532de,0x000546a8,0x00055a80,0x00056e67,0x0005825d,0x00059663,0x0005aa78,0x0005be9d,
    0x0005d2d2,0x0005e718,0x0005fb6e,0x00060fd5,0x0006244e,0x000638d8,0x00064d75,0x00066223,0x000676e4,0x00068bb7,
    0x0006a09e,0x0006b598,0x0006caa6,0x0006dfc7,0x0006f4fd,0x00070a48,0x00071fa8,0x0007351d,0x00074aa7,0x00076048,
    0x000775ff,0x00078bcc,0x0007a1b1,0x0007b7ad,0x0007cdc0,0x0007e3ec,0x0007fa30,0x0008108d,0x00082704,0x00083d94,
    0x0008543e,0x00086b02,0x000881e1,0x000898dc,0x0008aff2,0x0008c724,0x0008de73,0x0008f5de,0x00090d67,0x0009250e,
    0x00093cd3,0x000954b7,0x00096cba,0x000984dd,0x00099d20,0x0009b583,0x0009ce08,0x0009e6ae,0x0009ff77,0x000a1862,
    0x000a3170,0x000a4aa2,0x000a63f9,0x000a7d74,0x000a9715,0x000ab0dc,0x000acac9,0x000ae4dd,0x000aff1a,0x000b197f,
    0x000b340c,0x000b4ec4,0x000b69a5,0x000b84b2,0x000b9feb,0x000bbb4f,0x000bd6e1,0x000bf2a0,0x000c0e8e,0x000c2aab,
    0x000c46f8,0x000c6376,0x000c8025,0x000c9d06,0x000cba1b,0x000cd763,0x000cf4e0,0x000d1292,0x000d307b,0x000d4e9b,
    0x000d6cf3,0x000d8b85,0x000daa50,0x000dc956,0x000de899,0x000e0818,0x000e27d5,0x000e47d1,0x000e680e,0x000e888b,
    0x000ea94a,0x000eca4d,0x000eeb94,0x000f0d21,0x000f2ef5,0x000f5111,0x000f7376,0x000f9625,0x000fb920,0x000fdc69,
    0x000fffff,0x001023e6,0x0010481e,0x00106ca9,0x00109188,0x0010b6bc,0x0010dc48,0x0011022d,0x0011286c,0x00114f07,
    0x001175ff,0x00119d58,0x0011c511,0x0011ed2e,0x001215af,0x00123e97,0x001267e8,0x001291a4,0x0012bbcc,0x0012e663,
    0x0013116c,0x00133ce7,0x001368d8,0x00139540,0x0013c223,0x0013ef82,0x00141d60,0x00144bc0,0x00147aa4,0x0014aa0e,
    0x0014da02,0x00150a83,0x00153b93,0x00156d35,0x00159f6d,0x0015d23d,0x001605a9,0x001639b3,0x00166e61,0x0016a3b4,
    0x0016d9b1,0x0017105c,0x001747b7,0x00177fc8,0x0017b891,0x0017f218,0x00182c61,0x00186770,0x0018a349,0x0018dff1,
    0x00191d6e,0x00195bc4,0x00199af8,0x0019db0f,0x001a1c10,0x001a5dff,0x001aa0e3,0x001ae4c1,0x001b29a0,0x001b6f86,
    0x001bb67a,0x001bfe83,0x001c47a7,0x001c91ef,0x001cdd61,0x001d2a05,0x001d77e4,0x001dc706,0x001e1774,0x001e6937,
    0x001ebc57,0x001f10e0,0x001f66da,0x001fbe50,0x0020174e,0x002071dd,0x0020ce0b,0x00212be3,0x00218b71,0x0021ecc3,
    0x00224fe6,0x0022b4e9,0x00231bda,0x002384c8,0x0023efc4,0x00245cde,0x0024cc27,0x00253db2,0x0025b192,0x002627d9,
    0x0026a09e,0x00271bf5,0x002799f4,0x00281ab4,0x00289e4d,0x002924d8,0x0029ae71,0x002a3b35,0x002acb3f,0x002b5eb1,
    0x002bf5aa,0x002c904e,0x002d2ebf,0x002dd124,0x002e77a5,0x002f226d,0x002fd1a7,0x00308582,0x00313e31,0x0031fbe6,
    0x0032beda,0x00338746,0x00345569,0x00352984,0x003603dd,0x0036e4bd,0x0037cc73,0x0038bb52,0x0039b1b2,0x003aaff4,
    0x003bb67a,0x003cc5b2,0x003dde0e,0x003f0009,0x00402c28,0x004162f8,0x0042a511,0x0043f317,0x00454db9,0x0046b5b7,
    0x00482bdc,0x0049b108,0x004b462a,0x004cec47,0x004ea47b,0x00506ffb,0x00525017,0x00544640,0x00565409,0x00587b2d,
    0x005abd91,0x005d1d50,0x005f9cbb,0x00623e62,0x00650520,0x0067f423,0x006b0ef9,0x006e599e,0x0071d88d,0x007590d6,
    0x00798835,0x007dc530,0x00824f3e,0x00872eed,0x008c6e21,0x0092184f,0x00983ad6,0x009ee569,0x00a62a96,0x00ae2072,
    0x00b6e17e,0x00c08dd0,0x00cb4c9d,0x00d74e56,0x00e4cf87,0x00f41ce2,0x01059901,0x0119c4d9,0x01314c55,0x014d19ea,
    0x016e75d2,0x01973a29,0x01ca2e17,0x020baf28,0x026303ed,0x02dd4505,0x0394a3ad,0x04c63d78,0x07296b18,0x0e52e807
};

#ifdef AHMI_CORE

//*****************************
// CORDIC algorithm for absolute value and angle
//  CORDIC (s16 *s32p_COS,   // The X component of initial azimuth and rotation azimuth is equivalent to 1/2048 accuracy after normalization.
//          s16 *s32p_SIN);  // The Y component of initial azimuth and rotation azimuth is equivalent to 1/2048 accuracy after normalization.
//*****************************
void myMathClass::FindAngle(s32* s32p_COS, s32 *s32p_SIN)
{
	s16 s16_para[12] = { 1448, 1295, 1257, 1247, 1244, 1244, 1244, 1244, 1244, 1244, 1244,1244};    //cos@ result * 2048
	s32 s32_x, s32_y;
	s32 s32_count_i;
	if(*s32p_SIN==0)
	{
		*s32p_COS = (*s32p_COS>=0)?*s32p_COS:-*s32p_COS;
		return;
	}
	if(*s32p_COS == 0)
	{
		*s32p_COS = (*s32p_SIN>=0)?*s32p_SIN:-*s32p_SIN;
		return;
	}

	if(*s32p_COS<0 && *s32p_SIN>=0)
	{
		*s32p_COS = -*s32p_COS;
	}
	else if(*s32p_COS<0 && *s32p_SIN<0)
	{
		*s32p_COS = -*s32p_COS;
		*s32p_SIN = -*s32p_SIN;
	}
	else if(*s32p_COS>=0 && *s32p_SIN<0)
	{
		*s32p_SIN = -*s32p_SIN;
	}
	s32_x=*s32p_COS;
	s32_y=*s32p_SIN;
	for (s32_count_i = 0; s32_count_i < 12; s32_count_i++)
	{
		if (*s32p_SIN>0)      //If the target angle is greater than 0, continue to rotate counterclockwise, if the target angle is lower than 0, rotate
		{
			*s32p_COS = (s32_x + (s32_y >> s32_count_i));
			*s32p_SIN = (s32_y - (s32_x >> s32_count_i));
			s32_x = *s32p_COS;
			s32_y = *s32p_SIN;
		}
		else if (*s32p_SIN < 0)
		{
			*s32p_COS = (s32_x - (s32_y >> s32_count_i));
			*s32p_SIN = (s32_y + (s32_x >> s32_count_i));
			s32_x = *s32p_COS;
			s32_y = *s32p_SIN;
		}
		if ((s32_y >-2 && s32_y < 2 )|| s32_count_i ==11)
		{
			*s32p_COS = (s32_x*s16_para[s32_count_i])>>11;
			*s32p_SIN = (s32_y*s16_para[s32_count_i])>>11;
			break;
		}
	}
}

//*****************************
// CORDIC algorithm for trigonometric functions
//  CORDIC (s32 s32_Radian, // Rotation angle, left hand line, clockwise direction, accuracy 1.27.4
//          s16 *s32p_COS,   // The X component of initial azimuth and rotation azimuth is equivalent to 1/2048 accuracy after normalization.
//          s16 *s32p_SIN);  // The Y component of initial azimuth and rotation azimuth is equivalent to 1/2048 accuracy after normalization.
//*****************************
void myMathClass::CORDIC(s32 s32_Radian, s16* s32p_COS, s16 *s32p_SIN)
{
	s16 s16_Angle[12] = { 5760, 3400 ,1797 ,912 , 458, 229, 115, 57, 29, 14, 7,4 };             //cordic Angle precision is 9.7
	s16 s16_para[12] = { 1448, 1295, 1257, 1247, 1244, 1244, 1244, 1244, 1244, 1244, 1244,1244};//cos@result * 2048
	s16 s32_x, s32_y;
	s32 s32_count_i;
	s32 s32_coeffX = 0, s32_coeffY = 0;
	//float f_realAngle = 0;
	s32_x = *s32p_COS;
	s32_y = *s32p_SIN;
	while (s32_Radian >= 360 * 16)
		s32_Radian = s32_Radian - 360 * 16;
	while (s32_Radian < 0)
		s32_Radian = s32_Radian + 360 * 16;
	if (s32_Radian == 0)
	{
		*s32p_COS = s32_x;
		*s32p_SIN = s32_y;
	}
	else if(s32_Radian == 180 * 16)
	{
		*s32p_COS = -s32_x;
		*s32p_SIN = -s32_y;
	}
	else if (s32_Radian == 90 * 16)
	{
		*s32p_COS = -s32_y;
		*s32p_SIN = s32_x;
	}
	else if(s32_Radian == 270 * 16)
	{
		*s32p_COS = s32_y;
		*s32p_SIN = -s32_x;
	}
	else
	{
		if (s32_Radian > 0 && s32_Radian < 90 * 16)
		{
			s32_coeffX = 1, s32_coeffY = 1;
		}
		else if (s32_Radian > 90 * 16 && s32_Radian < 180 * 16) //The rotation angle to the second quadrant mirrors the target angle along the s32_y axis so that the rotation occurs in the first quadrant. After completing the rotation, mirror the value of s32_x again.
		{
			s32_Radian =180 * 16 - s32_Radian ; // Angle along the s32_y axis mirror image
			s32_coeffX = -1, s32_coeffY = 1; //Mirror X value
		}
		else if (s32_Radian > 180 * 16 && s32_Radian < 270 * 16) //The rotation angle to the third quadrant can mirror the target angle along the dot so that the rotation occurs in the first quadrant. After completing the rotation, mirror the value of s32_x and s32_y again.
		{
			s32_Radian = s32_Radian - 180 * 16;// Angle along the origin image
			s32_coeffX = -1, s32_coeffY = -1;//The value of mirror s32_x, s32_y
		}
		else if (s32_Radian > 270 * 16 && s32_Radian < 360 * 16) //The rotation angle to the fourth quadrant can mirror the target angle along the s32_x axis so that the rotation occurs in the first quadrant. After completing the rotation, mirror the value of s32_y again.
		{
			s32_Radian = 360 * 16 - s32_Radian; // Angle along the s32_x axis mirror image
			s32_coeffX = 1, s32_coeffY = -1;//The value of mirror s32_y
		}
		
		#if defined(CHIP_GC9002)
//		s32 tmpRadian = s32_Radian << 20;// Extend 1.27.4 to 1.7.24
//		s32 tmpcos = *s32p_COS;
//		s32 tmpsin = *s32p_SIN;
//		s32 tmpcos_out = 0;
//		s32 tmpsin_out = 0;
//		csi_fix_CORDIC(tmpRadian, &tmpcos, &tmpsin);
//		tmpcos_out = tmpcos*s32_coeffX;
//		tmpsin_out = tmpsin*s32_coeffY;
		s16 tmpCOS = *s32p_COS;
		s16 tmpSIN = *s32p_SIN;
		s32 tmpCOS_s32 = (s32)tmpCOS;
		s32 tmpSIN_s32 = (s32)tmpSIN;
		csi_fix_CORDIC(s32_Radian, &tmpCOS_s32, &tmpSIN_s32);
		*s32p_COS = tmpCOS_s32;
		*s32p_SIN = tmpSIN_s32;
		*s32p_COS = *s32p_COS*s32_coeffX;
		*s32p_SIN = *s32p_SIN*s32_coeffY;
		#else
		s32_Radian = s32_Radian << 3;// Extend 1.11.4 to 1.9.7
		for (s32_count_i = 0; s32_count_i < 12; s32_count_i++)
		{
			if (s32_Radian>0)      //If the target angle is greater than 0, continue to rotate counterclockwise, if the target angle is lower than 0, rotate
			{
				s32_Radian = s32_Radian - s16_Angle[s32_count_i];
				*s32p_COS = (s32_x - (s32_y >> s32_count_i));
				*s32p_SIN = (s32_y + (s32_x >> s32_count_i));
				s32_x = *s32p_COS;
				s32_y = *s32p_SIN;
			}
			else if (s32_Radian < 0)
			{
				s32_Radian = s32_Radian + s16_Angle[s32_count_i];
				*s32p_COS = (s32_x + (s32_y >> s32_count_i));
				*s32p_SIN = (s32_y - (s32_x >> s32_count_i));
				s32_x = *s32p_COS;
				s32_y = *s32p_SIN;
			}
			if ((s32_Radian >-2 && s32_Radian < 2 ) || s32_count_i ==11)
			{
				*s32p_COS = s32_coeffX*(*s32p_COS)*s16_para[s32_count_i] >> 11;
				*s32p_SIN = s32_coeffY*(*s32p_SIN)*s16_para[s32_count_i] >> 11;
				break;
			}
		}
		#endif
	}
}

//*****************************
// CORDIC algorithm for trigonometric functions,32 bit version
//  CORDIC_32 (s32 s32_Radian, // Rotation angle, left hand line, clockwise direction, accuracy 1.27.4
//			   s16 *s32p_COS,   // The X component of initial azimuth and rotation azimuth is equivalent to 1/2048 accuracy after normalization.
//			   s16 *s32p_SIN);  // The Y component of initial azimuth and rotation azimuth is equivalent to 1/2048 accuracy after normalization.
//*****************************
void myMathClass::CORDIC_32(s32 s32_Radian, s32* s32p_COS, s32 *s32p_SIN)
{
	s16 s16_Angle[12] = { 5760, 3400 ,1797 ,912 , 458, 229, 115, 57, 29, 14, 7,4 };             //cordic Angle precision is 9.7
	s16 s16_para[12] = { 1448, 1295, 1257, 1247, 1244, 1244, 1244, 1244, 1244, 1244, 1244,1244};//cos@result * 2048
	s32 s32_x, s32_y;
	s32 s32_count_i;
	s32 s32_coeffX = 0, s32_coeffY = 0;
	//float f_realAngle = 0;
	s32_x = *s32p_COS;
	s32_y = *s32p_SIN;
	unsigned long long temp;
	s16 sign;
	while (s32_Radian >= 360 * 16)
		s32_Radian = s32_Radian - 360 * 16;
	while (s32_Radian < 0)
		s32_Radian = s32_Radian + 360 * 16;
	if (s32_Radian == 0)
	{
		*s32p_COS = s32_x;
		*s32p_SIN = s32_y;
	}
	else if(s32_Radian == 180 * 16)
	{
		*s32p_COS = -s32_x;
		*s32p_SIN = -s32_y;
	}
	else if (s32_Radian == 90 * 16)
	{
		*s32p_COS = -s32_y;
		*s32p_SIN = s32_x;
	}
	else if(s32_Radian == 270 * 16)
	{
		*s32p_COS = s32_y;
		*s32p_SIN = -s32_x;
	}
	else
	{
		if (s32_Radian > 0 && s32_Radian < 90 * 16)
		{
			s32_coeffX = 1, s32_coeffY = 1;
		}
		else if (s32_Radian > 90 * 16 && s32_Radian < 180 * 16) //The rotation angle to the second quadrant mirrors the target angle along the s32_y axis so that the rotation occurs in the first quadrant. After completing the rotation, mirror the value of s32_x again.
		{
			s32_Radian =180 * 16 - s32_Radian ; // Angle along the s32_y axis mirror image
			s32_coeffX = -1, s32_coeffY = 1; //Mirror X value
		}
		else if (s32_Radian > 180 * 16 && s32_Radian < 270 * 16) //The rotation angle to the third quadrant can mirror the target angle along the dot so that the rotation occurs in the first quadrant. After completing the rotation, mirror the value of s32_x and s32_y again.
		{
			s32_Radian = s32_Radian - 180 * 16;// Angle along the origin image
			s32_coeffX = -1, s32_coeffY = -1;//The value of mirror s32_x, s32_y
		}
		else if (s32_Radian > 270 * 16 && s32_Radian < 360 * 16) //The rotation angle to the fourth quadrant can mirror the target angle along the s32_x axis so that the rotation occurs in the first quadrant. After completing the rotation, mirror the value of s32_y again.
		{
			s32_Radian = 360 * 16 - s32_Radian; // Angle along the s32_x axis mirror image
			s32_coeffX = 1, s32_coeffY = -1;//The value of mirror s32_y
		}
		
		#if defined(CHIP_GC9002)
//		s32 tmpRadian = s32_Radian;
//		s32 tmpcos = *s32p_COS;
//		s32 tmpsin = *s32p_SIN;
//		s32 tmpcos_out = 0;
//		s32 tmpsin_out = 0;
//		csi_fix_CORDIC(tmpRadian, &tmpcos, &tmpsin);
//		tmpcos_out = tmpcos*s32_coeffX;
//		tmpsin_out = tmpsin*s32_coeffY;
		csi_fix_CORDIC(s32_Radian, s32p_COS, s32p_SIN);
		*s32p_COS = *s32p_COS*s32_coeffX;
		*s32p_SIN = *s32p_SIN*s32_coeffY;
		#else
		s32_Radian = s32_Radian << 3;// Extend 1.11.4 to 1.9.7
		for (s32_count_i = 0; s32_count_i < 12; s32_count_i++)
		{
			if (s32_Radian>0)      //If the target angle is greater than 0, continue to rotate counterclockwise, if the target angle is lower than 0, rotate
			{
				s32_Radian = s32_Radian - s16_Angle[s32_count_i];
				*s32p_COS = (s32_x - (s32_y >> s32_count_i));
				*s32p_SIN = (s32_y + (s32_x >> s32_count_i));
				s32_x = *s32p_COS;
				s32_y = *s32p_SIN;
			}
			else if (s32_Radian < 0)
			{
				s32_Radian = s32_Radian + s16_Angle[s32_count_i];
				*s32p_COS = (s32_x + (s32_y >> s32_count_i));
				*s32p_SIN = (s32_y - (s32_x >> s32_count_i));
				s32_x = *s32p_COS;
				s32_y = *s32p_SIN;
			}
			if ((s32_Radian >-2 && s32_Radian < 2 ) || s32_count_i ==11)
			{
				temp = (unsigned long long)(*s32p_COS)*s16_para[s32_count_i]; //Prevent symbol bit offside
				if(*s32p_COS < 0)
					sign = -1;
				else 
					sign = 1;
				*s32p_COS = (s32)( s32_coeffX * (sign * temp / 2048));
				
				temp = (unsigned long long)(*s32p_SIN)*s16_para[s32_count_i];
				if(*s32p_SIN < 0)
					sign = -1;
				else 
					sign = 1;
				*s32p_SIN = (s32)( s32_coeffY * (sign * temp / 2048));

				//*s32p_SIN = (s32)((long long)(s32_coeffY*(*s32p_SIN)*s16_para[s32_count_i])>>11);
				break;
			}
		}
		#endif
	}
}

//****************************
//The rotation transformation of the inverse matrix is clockwise in the positive direction and only considers the 2 order part.
//| s32_ipointmatrix[0],s32_ipointmatrix[1] |  \/  | cos(theta),-sin(theth)|
//| s32_ipointmatrix[2],s32_ipointmatrix[3] |  /\  | sin(theta), cos(theta)|
//****************************
void myMathClass::MatrixRotate(s16 s16_degrees,s32 s32_ipointmatrix[])
{
	s32 cos = 1024*1024, sin = 0;u8 i;
	//The value of COS is expanded by 1024*1024 times.
	s32 s32_currentmatrix[9];
	CORDIC_32(s16_degrees, &cos, &sin);
	
	for ( i = 0; i < 4; i++)
		s32_currentmatrix[i] = s32_ipointmatrix[i];
	s32_ipointmatrix[0] = ((long long)s32_currentmatrix[0]*cos + (long long)s32_currentmatrix[1]*sin) >> 20; 
	s32_ipointmatrix[1] = ((long long)s32_currentmatrix[1]*cos - (long long)s32_currentmatrix[0]*sin) >> 20;
	s32_ipointmatrix[2] = ((long long)s32_currentmatrix[2]*cos + (long long)s32_currentmatrix[3]*sin) >> 20;
	s32_ipointmatrix[3] = ((long long)s32_currentmatrix[3]*cos - (long long)s32_currentmatrix[2]*sin) >> 20;


}


//****************************
//The rotation transformation of the inverse matrix takes only 2 orders.
//  | s32_ipointmatrix[0],s32_ipointmatrix[1] | \/ | 1/ScaleX,      0|
//  | s32_ipointmatrix[2],s32_ipointmatrix[3] | /\ |      0,1/ScaleY)|
//The format of ScaleX and ScaleY is 1.6.9, and s32_ipointmatrix format is 1.6.9.
//****************************
void myMathClass::MatrixScaler(s16 ScaleX, s16 ScaleY, s32 s32_ipointmatrix[])
{
	s32 s32_currentmatrix[9];
	int i;
    if((ScaleX == 512) && (ScaleY == 512)) //512 = 1 << 9.
	{
        return;
	}
    
	for (i = 0; i < 4; i++)
		s32_currentmatrix[i] = s32_ipointmatrix[i];
	/*s32_ipointmatrix[0] = ((s32_currentmatrix[0])/ScaleX) << 9 ; 
	s32_ipointmatrix[1] = ((s32_currentmatrix[1])/ScaleX) << 9 ; 
	s32_ipointmatrix[2] = ((s32_currentmatrix[2])/ScaleY) << 9 ; 
	s32_ipointmatrix[3] = ((s32_currentmatrix[3])/ScaleY) << 9 ; */

	s32_ipointmatrix[0] = (((long long)s32_currentmatrix[0] << 9) / ScaleX) ; 
	s32_ipointmatrix[1] = (((long long)s32_currentmatrix[1] << 9) / ScaleX) ; 
	s32_ipointmatrix[2] = (((long long)s32_currentmatrix[2] << 9) / ScaleY) ; 
	s32_ipointmatrix[3] = (((long long)s32_currentmatrix[3] << 9) / ScaleY) ; 
}

//****************************
//The tangent transformation of inverse matrix takes only 2 part (X direction tangent).
//S16_degrees denotes the tan value of the texture tangent angle in the X direction, tan0 denotes no tangent, and tan90 denotes a straight line
//| s32_ipointmatrix[0],s32_ipointmatrix[1] |  \/  |       1      ,      0       |
//| s32_ipointmatrix[2],s32_ipointmatrix[3] |  /\  | -tan(s16_degrees),      1       |
//****************************
void myMathClass::MatrixShearX(s16 s16_degrees,s32 s32_ipointmatrix[])
{
	s16 cos = 1024, sin = 0;
	//The value of COS is expanded by 1024 times.
	s32 shearTan;
	s32 s32_currentmatrix[4];
	u8 i;

	CORDIC(s16_degrees, &cos, &sin);
	if(cos == 0) shearTan = -32768;//Negative maximum
	else shearTan = 512 * sin /cos ;// 1.6.9
	
	for (i = 0; i < 4; i++)
		s32_currentmatrix[i] = s32_ipointmatrix[i];
	s32_ipointmatrix[0] = (long long)s32_currentmatrix[0] - (((long long)s32_currentmatrix[1]*shearTan)>> 9);
	s32_ipointmatrix[2] = (long long)s32_currentmatrix[2] - (((long long)s32_currentmatrix[3]*shearTan)>> 9);

}

//****************************
//The tangent transformation of inverse matrix takes only 2 part (Y direction tangent).
//S16_degrees denotes the tan value of the texture tangent angle in the X direction, tan0 denotes no tangent, and tan90 denotes a straight line
//| s32_ipointmatrix[0],s32_ipointmatrix[1] |  \/  |       1   ,  -tan(s16_degrees)       |
//| s32_ipointmatrix[2],s32_ipointmatrix[3] |  /\  |       0   ,      1                   |
//****************************
void myMathClass::MatrixShearY(s16 s16_degrees,s32 s32_ipointmatrix[])
{
	s16 cos = 1024, sin = 0;
	//The value of COS is expanded by 1024 times.
	s32 shearTan;
	s32 s32_currentmatrix[4];
	u8 i;

	CORDIC(s16_degrees, &cos, &sin);
	if(cos == 0) shearTan = -32768;//Negative maximum
	else shearTan = 512 * sin /cos ;// 1.6.9
	
	for (i = 0; i < 4; i++)
		s32_currentmatrix[i] = s32_ipointmatrix[i];

	s32_ipointmatrix[1] = (long long)s32_currentmatrix[1] - (((long long)s32_currentmatrix[0]*shearTan)>> 9);
	s32_ipointmatrix[3] = (long long)s32_currentmatrix[3] - (((long long)s32_currentmatrix[2]*shearTan)>> 9);
}


//*****************************
// Calculate the angle according to the input s32_x and s32_y coordinates.
//  CORDIC_ANGLE (s32 s32_Radian, // Angle, precision 1.27.4
//				  s16 *s32p_COS,  //  S32_x coordinates accuracy 1.11.4
//				  s16 *s32p_SIN); //  S32_Y coordinates accuracy 1.11.4
//*****************************
void myMathClass::CORDIC_ANGLE(s32* s32_Radian, s16 s32p_COS, s16 s32p_SIN)
{
	s16 s16_Angle[12] = { 5760, 3400 ,1797 ,912 , 458, 229, 115, 57, 29, 14, 7,4 };             //cordic Angle precision is 9.7
//	s16 s16_para[12] = { 1448, 1295, 1257, 1247, 1244, 1244, 1244, 1244, 1244, 1244, 1244,1244};    //cos@ result * 2048
	s16 s32_x, s32_y;
	s32 s32_count_i;
//	s32 s32_coeffX = 0, s32_coeffY = 0;
	s32 offset;
//	float f_realAngle = 0;
	s32_x = s32p_COS;
	s32_y = s32p_SIN;
	if(s32_y == 0 && s32_x >=0) *s32_Radian = 0;
	else if(s32_y == 0 && s32_x <0) *s32_Radian = 180 * 16;
	else if(s32_x == 0 && s32_y > 0) *s32_Radian = 90 * 16;
	else if(s32_x == 0 && s32_y < 0) *s32_Radian = 270 * 16;
	else
	{
		if(s32_x > 0 && s32_y > 0)
			offset = 0;
		else if(s32_x < 0 && s32_y > 0)
		{
			offset = 1;
			s32_x = -s32_x;
		}
		else if(s32_x < 0 && s32_y < 0)
		{
			offset = 2;
			s32_x = -s32_x;
			s32_y = -s32_y;
		}
		else if(s32_x < 0 && s32_y < 0)
		{
			offset = 3;
			s32_y = -s32_y;
		}
		for (s32_count_i = 0; s32_count_i < 12; s32_count_i++)
		{
			if (s32_y>0)
			{
				s32_Radian = s32_Radian + s16_Angle[s32_count_i];
				s32p_COS = (s32_x + (s32_y >> s32_count_i));
				s32p_SIN = (s32_y - (s32_x >> s32_count_i));

				s32_x = s32p_COS;
				s32_y = s32p_SIN;
			}
			else if (s32_y < 0)
			{
				s32_Radian = s32_Radian - s16_Angle[s32_count_i];
				s32p_COS = (s32_x - (s32_y >> s32_count_i));
				s32p_SIN = (s32_y + (s32_x >> s32_count_i));
				s32_x = s32p_COS;
				s32_y = s32p_SIN;
			}
		}
		*s32_Radian = (*s32_Radian) >> 3;
		if(offset == 1)
			*s32_Radian = 180 * 16 - *s32_Radian;
		else if(offset == 2)
			*s32_Radian = 180 * 16 + *s32_Radian;
		else if(offset == 3)
			*s32_Radian = 360 * 16 - *s32_Radian;
	}
}

s32 myMathClass::GetAngleFromTan(s32 angleTan){
	#if defined(CHIP_GC9002)
	return csi_fix_CORDIC_GetAngleFromTan(angleTan);
	#else
	int left = 0;
	int right = 90*4-1;
	int mid = 0;
	bool minus = false;
	if (angleTan < 0)
	{
		minus = true;
		angleTan = -angleTan;
	}
	while(right - left > 1){
		if (angleTan < tanTable[left])
		{
			return (minus?-1:1)*(left << 4)/4;
		}
		if (angleTan > tanTable[right])
		{
			return (minus?-1:1)*(right << 4)/4;
		}
		mid = (left + right)/2;
		if (angleTan > tanTable[mid])
		{
			left = mid;
		}else{
			right = mid;
		}
	}

	return (minus?-1:1)*(left << 4)/4;
	#endif
}

#ifdef Use_SinTanTable
//*****************************
// 通过查找表的方式求解三角函数
//  SinTable (s32 s32_Radian, // 旋转角度，左手系，顺时针为正方向，精度1.11.20
//			  s32 *s32p_COS,   // 通过查找表找到角度对应的COS值
//            s32 *s32p_SIN);  // 通过查找表找到角度对应的SIN值
//*****************************
u32 myMathClass::SinTable(s32 s32_Radian, s32* s32p_COS, s32 *s32p_SIN)
{
	u32 k;
	s32 sinDegree;
	s32 cosDegree;

	while (s32_Radian >= (360 << 20) )
		s32_Radian = s32_Radian - (360 << 20);
	while (s32_Radian < 0)
		s32_Radian = s32_Radian + (360 << 20);

	k = ( 10 * (s32_Radian >> 10) ) >> 10; //trans into int
	//0 - 90 cos(d) = sin(90-d)
	if (s32_Radian >= 0 && s32_Radian < (90 << 20))
	{
		
		sinDegree = sinTable[k];
		cosDegree = sinTable[900-k];
		
	}
	//90-180 , degree - 90 is 0-90. sin degree = sin(180 - degree) cos(d) = sin(90-d) = -sin(d-90)
	else if (s32_Radian >= (90 << 20) && s32_Radian < (180 << 20))
	{
		sinDegree = sinTable[180*10 - k];
		cosDegree = -sinTable[k-90*10];

	}
	//180-270 , sin(d) = -sin(d-180); cos(d) = -sin(270-d)
	else if (s32_Radian >= (180 << 20) && s32_Radian <= (270 << 20) ) 
	{
		sinDegree = -sinTable[k-180*10];
		cosDegree = -sinTable[270*10 - k];
	}
	//270-360 sin(d) = -sin(360-d); cos(d) = sin(d-270)
	else if (s32_Radian >= (270 << 20) && s32_Radian < (360 << 20))
	{
		sinDegree = -sinTable[360*10-k];
		cosDegree = sinTable[k-270*10];
	}
	else
	{
		return (EM_enErrRetModularMyMath | EM_ErrRetMyMath_DegreeOverrun);
	}

	*s32p_COS = cosDegree;
	*s32p_SIN = sinDegree;

	return AHMI_FUNC_SUCCESS;
}

//*****************************
// 通过查找表的方式求解三角函数
//  TanTable (s32 s32_Radian, // 旋转角度，左手系，顺时针为正方向，精度1.11.20
//			  s32 *s32p_TAN)   // 通过查找表找到角度对应的TAN值
//*****************************
u32 myMathClass::TanTable(s32 s32_Radian, s32* s32p_TAN)
{
	u32 kx;
	//cos求出的值为扩大1024倍的值
	s32 shearCtg;
	
	//trans into 0-180
	while(s32_Radian > (90 << 20) )
		s32_Radian -= (180 << 20);
	while(s32_Radian < -(90 << 20) )
		s32_Radian += (90 << 20);

	kx = (4 * (s32_Radian >> 10) >> 10); //trans into int

	//0-90
	//ctgx = tan(90-x)
	if (s32_Radian > 0 && s32_Radian <= (90 << 20))
	{
		shearCtg = tanTable[90*4 - kx];
	}
	//-90 - 0
	//ctgx = -tan(x+90)
	else if (s32_Radian >= -(90 << 20) && s32_Radian < 0)
	{
		shearCtg = -tanTable[90*4 + kx];

	}
	//90, the max ct
	else if(s32_Radian == 0 )
	{
		ERROR_PRINT("error when caculating the shearx matrix : cannot shear to 0 degree! using 0.25 instead");
		shearCtg = tanTable[90*4 - 1];
	}
	else
	{
		ERROR_PRINT("error when caculating the shearx matrix : the angle not leagel");
		return (EM_enErrRetModularMyMath | EM_ErrRetMyMath_DegreeOverrun);
	}

	*s32p_TAN = shearCtg;

	return AHMI_FUNC_SUCCESS;
}
#endif

/////////////////////////////////////////////////
//
//将输入的float变量转换为s1.8.9形式
//参数列表
//     @param1 float f_in float类型输入
//     @param2 s16 s16_out s16类型输出，1.8.9
//
//////////////////////////////////////////////////
funcStatus myMathClass::transferFloatToS16(float f_in, s16* s16_out)
{
	u16 integerPart = (u16)f_in;
	u16 fractionalPart = (u16)((f_in - integerPart) * 512);
	*s16_out = (integerPart << 9) + fractionalPart;
	return AHMI_FUNC_SUCCESS;
}

#endif
