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

constexpr usize word_size = 64;