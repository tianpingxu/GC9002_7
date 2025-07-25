#include "stdafx.h"
#include "math.h"

class uint128_t {
public:
    unsigned long long int data[2];
    uint128_t() {
        data[1] = 0;
        data[0] = 0;
    }
    uint128_t operator~() {
        uint128_t out;
        out.data[0] = ~data[0];
        out.data[1] = ~data[1];
        return out;
    }
        uint128_t operator+(const uint128_t& b) {
        uint128_t out;
        out.data[1] = data[1] + b.data[1];
        out.data[0] = data[0] + b.data[0];
        if (b.data[0] > (UINT8_MAX - data[0]))out.data[1]++;
        return out;
    }
    uint128_t operator+(const unsigned long long int& b) {
        uint128_t out;
        out.data[1] = data[1];
        out.data[0] = data[0] + b;
        if (b > (UINT64_MAX - data[0]))out.data[1]++;
        return out;
    }
    uint128_t operator-(const uint128_t& b) {
        uint128_t out;
        out.data[1] = data[1] + ~b.data[1];
        out.data[0] = data[0] + ~b.data[0];
        if (b.data[0] > (UINT8_MAX - data[0]))out.data[1]++;
        out.data[0]++;
        out.data[1]++;
        return out;
    }
    bool operator<(const uint128_t& b) {
        if (data[1] == b.data[1]) {
            return (data[0] < b.data[0]) ? true : false;
        }
        else return (data[1] < b.data[1]) ? true : false;
    }  
        bool operator>=(const uint128_t& b) {
        if (data[1] == b.data[1]) {
            return (data[0] >= b.data[0]) ? true : false;
        }
        else return (data[1] > b.data[1]) ? true : false;
    }
    uint128_t &operator=(const unsigned long long int& b) {
        data[1] = 0;
        data[0] = b;
        return *this;
    }
	uint128_t &operator|=(const unsigned long long int& b) {
        data[1] = b;
        return *this;
    }
    uint128_t& operator=(const uint128_t& b) {
        data[1] = b.data[1];
        data[0] = b.data[0];
        return *this;
    }
	u64 operator>>(const unsigned long long int& b) {
        u64 out;
		u64 temp;
		if(b < 64)
		{
		temp = data[1] - ((data[1] >> b) << b);
        out = (data[0] >> b) + (temp << (64-b));
		}
		else
		{
		out = data[1] >> (b-64);
		}
        return out;
    }
	uint128_t operator||(const unsigned long long int& b) {
        uint128_t out;
        u64 temp;
		if(b < 64)
		{
		temp = data[1] - ((data[1] >> b) << b);
        out.data[0] = (data[0] >> b) + (temp << (64-b));
		out.data[1] = data[1] >> b;
		}
		else
		{
		out.data[0] = data[1] >> (b-64);
		out.data[1] = 0;
		}

        return out;
    }
};

typedef uint128_t  u128;

void stc_rgba_dec(u8 x,u8 y,u128 block_data,u32* out_color);

void stc_rgb_dec(u8 x,u8 y,u128 block_data,u32* out_color);
int select_partition(int seed, int x, int y, int z,
                     int partitioncount, int small_block);