#pragma once

//these definitions should be changed according to the used compiler and target machine. make
//sure ux, ix and fx are each x bits long. also make sure the word size matches that of the
//target machine.
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;
typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long i64;
typedef float f32;
typedef double f64;

//standard word sizes, in this case for a 64 bit machine.
typedef u64 usize;
typedef i64 isize;

typedef u8 byte;

//static asserts check if the size of the types match the intended type. if one of them should
//fail this is an indication the type definition should get corrected.
static_assert(sizeof(u8) == 1);
static_assert(sizeof(u16) == 2);
static_assert(sizeof(u32) == 4);
static_assert(sizeof(u64) == 8);
static_assert(sizeof(i8) == 1);
static_assert(sizeof(i16) == 2);
static_assert(sizeof(i32) == 4);
static_assert(sizeof(i64) == 8);
static_assert(sizeof(f32) == 4);
static_assert(sizeof(f64) == 8);
static_assert(sizeof(usize) == sizeof(isize));