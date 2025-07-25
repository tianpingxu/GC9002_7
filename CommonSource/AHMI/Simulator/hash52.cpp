#include "stc.h"

void bit_transfer_signed(int a, int b)
{
    b >>= 1;
    b |= a & 0x80;
    a >>= 1;
    a &= 0x3F;
    if( (a&0x20)!=0 ) a-=0x40;
}

enum quantization_method
{
	QUANT_2 = 0,
	QUANT_3 = 1,
	QUANT_4 = 2,
	QUANT_5 = 3,
	QUANT_6 = 4,
	QUANT_8 = 5,
	QUANT_10 = 6,
	QUANT_12 = 7,
	QUANT_16 = 8,
	QUANT_20 = 9,
	QUANT_24 = 10,
	QUANT_32 = 11,
	QUANT_40 = 12,
	QUANT_48 = 13,
	QUANT_64 = 14,
	QUANT_80 = 15,
	QUANT_96 = 16,
	QUANT_128 = 17,
	QUANT_160 = 18,
	QUANT_192 = 19,
	QUANT_256 = 20
};

int quantization_mode_table[17][128];

int compute_ise_bitcount(
	int items,
	quantization_method quant
) {
	switch (quant)
	{
	case QUANT_2:
		return items;
	case QUANT_3:
		return (8 * items + 4) / 5;
	case QUANT_4:
		return 2 * items;
	case QUANT_5:
		return (7 * items + 2) / 3;
	case QUANT_6:
		return (13 * items + 4) / 5;
	case QUANT_8:
		return 3 * items;
	case QUANT_10:
		return (10 * items + 2) / 3;
	case QUANT_12:
		return (18 * items + 4) / 5;
	case QUANT_16:
		return items * 4;
	case QUANT_20:
		return (13 * items + 2) / 3;
	case QUANT_24:
		return (23 * items + 4) / 5;
	case QUANT_32:
		return 5 * items;
	case QUANT_40:
		return (16 * items + 2) / 3;
	case QUANT_48:
		return (28 * items + 4) / 5;
	case QUANT_64:
		return 6 * items;
	case QUANT_80:
		return (19 * items + 2) / 3;
	case QUANT_96:
		return (33 * items + 4) / 5;
	case QUANT_128:
		return 7 * items;
	case QUANT_160:
		return (22 * items + 2) / 3;
	case QUANT_192:
		return (38 * items + 4) / 5;
	case QUANT_256:
		return 8 * items;
	default:
		return 100000;
	}
}


void build_quantization_mode_table()
{
	for (int i = 0; i <= 16; i++)
	{
		for (int j = 0; j < 128; j++)
		{
			quantization_mode_table[i][j] = -1;
		}
	}

	for (int i = 0; i < 21; i++)
	{
		for (int j = 1; j <= 16; j++)
		{
			int p = compute_ise_bitcount(2 * j, (quantization_method) i);
			if (p < 128)
			{
				quantization_mode_table[j][p] = i;
			}
		}
	}

	for (int i = 0; i <= 16; i++)
	{
		int largest_value_so_far = -1;
		for (int j = 0; j < 128; j++)
		{
			if (quantization_mode_table[i][j] > largest_value_so_far)
			{
				largest_value_so_far = quantization_mode_table[i][j];
			}
			else
			{
				quantization_mode_table[i][j] = largest_value_so_far;
			}
		}
	}
}
uint32_t hash52( uint32_t p )
{
    p ^= p >> 15;  
	p -= p << 17; 
	p += p << 7;
	p += p <<  4;
    p ^= p >>  5; 
	p += p << 16; 
	p ^= p >> 7;
	p ^= p >> 3;
    p ^= p <<  6; 
	p ^= p >> 17;
    return p;
}

int select_partition(int seed, int x, int y, int z,
                     int partitioncount, int small_block)
{
    if( small_block ){ x <<= 1; y <<= 1; z <<= 1; }
    seed += (partitioncount-1) * 1024;
    uint32_t rnum = hash52(seed);
    uint8_t seed1  =  rnum        & 0xF;
    uint8_t seed2  = (rnum >>  4) & 0xF;
    uint8_t seed3  = (rnum >>  8) & 0xF;
    uint8_t seed4  = (rnum >> 12) & 0xF;
    uint8_t seed5  = (rnum >> 16) & 0xF;
    uint8_t seed6  = (rnum >> 20) & 0xF;
    uint8_t seed7  = (rnum >> 24) & 0xF;
    uint8_t seed8  = (rnum >> 28) & 0xF;
    uint8_t seed9  = (rnum >> 18) & 0xF;
    uint8_t seed10 = (rnum >> 22) & 0xF;
    uint8_t seed11 = (rnum >> 26) & 0xF;
    uint8_t seed12 = ((rnum >> 30) | (rnum << 2)) & 0xF;

    seed1  *= seed1;    seed2  *= seed2;
    seed3  *= seed3;    seed4  *= seed4;
    seed5  *= seed5;    seed6  *= seed6;
    seed7  *= seed7;    seed8  *= seed8;
    seed9  *= seed9;    seed10 *= seed10;
    seed11 *= seed11;   seed12 *= seed12;

    int sh1, sh2, sh3;
    if( seed & 1 )
        { sh1 = (seed&2 ? 4:5); sh2 = (partitioncount==3 ? 6:5); }
    else
        { sh1 = (partitioncount==3 ? 6:5); sh2 = (seed&2 ? 4:5); }
    sh3 = (seed & 0x10) ? sh1 : sh2;

    seed1 >>= sh1; seed2  >>= sh2; seed3  >>= sh1; seed4  >>= sh2;
    seed5 >>= sh1; seed6  >>= sh2; seed7  >>= sh1; seed8  >>= sh2;
    seed9 >>= sh3; seed10 >>= sh3; seed11 >>= sh3; seed12 >>= sh3;

    int a = seed1*x + seed2*y + seed11*z + (rnum >> 14);
    int b = seed3*x + seed4*y + seed12*z + (rnum >> 10);
    int c = seed5*x + seed6*y + seed9 *z + (rnum >>  6);
    int d = seed7*x + seed8*y + seed10*z + (rnum >>  2);

    a &= 0x3F; b &= 0x3F; c &= 0x3F; d &= 0x3F;

    if( partitioncount < 4 ) d = 0;
    if( partitioncount < 3 ) c = 0;

    if( a >= b && a >= c && a >= d ) return 0;
    else if( b >= c && b >= d ) return 1;
    else if( c >= d ) return 2;
    else return 3;
}
