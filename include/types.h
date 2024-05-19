/*
	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/	
#ifndef __TYPES_H_
#define __TYPES_H_

/*
  define some specific length types
*/

typedef unsigned char u8;
typedef signed char i8;
typedef unsigned short int u16;
typedef signed short int i16;
typedef unsigned int u32;
typedef signed int i32;
typedef unsigned long long int u64;
typedef signed long long int i64;
typedef float f32;
typedef double f64;

// Convienience types
typedef u8 b8;
typedef u8 boolean;
typedef void* Pointer;
typedef unsigned char uchar;
typedef uchar* PChar;
typedef i64 num;

#ifndef __x64__
typedef u32 uint;
typedef u32 size_t;
#else
typedef u64 uint;
typedef u64 size_t;
#endif

#define true 1
#define false 0

#define null ((void*)0)

#define I8_MIN		(-0x80)
#define I8_MAX		0x7F
#define U8_MIN		0
#define U8_MAX		0xFF
#define I16_MIN		(-0x8000)
#define I16_MAX		0x7FFF
#define U16_MIN		0
#define U16_MAX		0xFFFF
#define I32_MIN		(-0x80000000)
#define I32_MAX		0x7FFFFFFF
#define U32_MIN		0
#define U32_MAX		0xFFFFFFFF
#define I64_MIN		(-0x8000000000000000)
#define I64_MAX		0x7FFFFFFFFFFFFFFF
#define U64_MIN		0
#define U64_MAX		0xFFFFFFFFFFFFFFFF
#define INVALID_PTR	I64_MAX

// #define U64_F64_MAX (0x43F0000000000000(F64))
// #define F64_MAX 	(0x7FEFFFFFFFFFFFFF(F64))
// #define F64_MIN 	(0xFFEFFFFFFFFFFFFF(F64))
// #define inf 		(0x7FF0000000000000(F64))
// #define � 			(0x7FF0000000000000(F64))
// #define pi			(0x400921FB54442D18(F64))
// #define � 			(0x400921FB54442D18(F64))
// #define exp_1 		(0x4005BF0A8B145769(F64)) //The number "e"
// #define log2_10 	(0x400A934F0979A371(F64))
// #define log2_e		(0x3FF71547652B82FE(F64))
// #define log10_2 	(0x3FD34413509F79FF(F64))
// #define loge_2		(0x3FE62E42FEFA39EF(F64))
// #define sqrt2 		(0x3FF6A09E667F3BCD(F64))
// #define eps 		(0x3CB0000000000000(F64))

#endif
