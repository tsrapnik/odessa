#pragma once

//these definitions should be changed according to the used compiler and target machine. make
//sure ux, ix and fx are each x bits long. ux are unsigned integers, ix are signed integers,
//fx are floating point numbers.
using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long;
using i8 = signed char;
using i16 = signed short;
using i32 = signed int;
using i64 = signed long;
using f32 = float;
using f64 = double;

//standard int sizes. these match the word size of your processor.
using usize = u64;
using isize = i64;

//wx are words. they are considered raw memory. they have the same sizes as their corresponding
//integer types, but you are not allowed to perform arithmetic on them or assign values to them
//except by copying another word to it.
enum class w8 : u8
{};
enum class w16 : u16
{};
enum class w32 : u32
{};
enum class w64 : u64
{};
//todo: use wx in stead of ux for all raw memory usage.

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