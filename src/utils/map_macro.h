/******************************************************************************
 * @file: map_macro.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-11
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: 
 * This is a very ugly macro hack to be able to map a macro to each macro 
 * argument given.
 * We need this to being apply to workaround the fact that C/C++ doesn't
 * have good meta-programming code generation tools (templates don't cut it
 * for explicit code generation).
 * This is the way I found to create some simple features that *should* be
 * part of any decent programming language, like being able to retrieve
 * a string from an enum instance so I CAN READ which one it is.
 ******************************************************************************/

#ifndef SRC_UTILS_MAP_MACRO_H
#define SRC_UTILS_MAP_MACRO_H

/**
 * MAP MACRO
 *
 * A set of macros to be able to map a macro to arguments of another macro
 * Usage:
 * // Example to map
 * #define QUOTE_AND_COMMA(x) #x,
 *
 * // Will output "first", "second", "third",
 * APPLY_ALL(QUOTE_AND_COMMA, first, second, third)
 */

/**
 * This macro will correctly expand the macro arguments being given
 * as in MSVC __VA_ARGS__ get expanded as one paramenter instead of a
 * a lot as this technique expects
 **/
#define EXPAND(x) x

/**
 * The actual numbers here will be pushed to the right
 * by __VA_ARGS__ to the correct position.
 * This way COUNT will return the number of arguments
 */
#define COUNT(...) EXPAND(COUNT_N(__VA_ARGS__,  \
63,62,61,60, \
59,58,57,56,55,54,53,52,51,50, \
49,48,47,46,45,44,43,42,41,40, \
39,38,37,36,35,34,33,32,31,30, \
29,28,27,26,25,24,23,22,21,20, \
19,18,17,16,15,14,13,12,11,10, \
9,8,7,6,5,4,3,2,1,0))

/**
 * This macro relies on __VA_ARGS__ being expanded as a comma-list
 * and pushing the N parameter into the pre established slot
 * where it will be printer.
 * So this requires that COUNT_N and COUNT have the same 
 * amount of "position" parameters
 */
#define COUNT_N( \
_1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
_11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
_21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
_31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
_41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
_51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
_61,_62,_63,N,...) N

/**
 * This function is the entry point for applying a macro to each macro argument
 * Basically, how it works is like this,
 * 1. Count how many arguments where given pass the MACRO to map (in func) using
 *    the COUNT(...) macro
 * 2. With that, by passing by the two REDIRECTORS (we need them because with one pass
 *    MSVC would refuse to understand the passed __VA_ARGS__ as many arguments instead
 *    of one.
 *    The final redirector would append the count number to the MAP_ prefix,
 *    thus creating the correct function to call
 */
#define MAP_ALL(func, ...) REDIRECTOR_1(func, COUNT(__VA_ARGS__), __VA_ARGS__)
#define MAP_ARG_ALL(func, arg, ...) REDIRECTOR_ARG1(func, arg, COUNT(__VA_ARGS__), __VA_ARGS__)

/**
 * Redirectors needed to expand the __VA_ARGS__ into arguments
 */
#define REDIRECTOR_1(func, num_args, ...) REDIRECTOR_2(func, num_args, __VA_ARGS__)
#define REDIRECTOR_ARG1(func, arg, num_args, ...) REDIRECTOR_ARG2(func, arg, num_args, __VA_ARGS__)
#define REDIRECTOR_2(func, num_args, ...) EXPAND(MAP_##num_args(func, __VA_ARGS__))
#define REDIRECTOR_ARG2(func, arg, num_args, ...) EXPAND(MAP_ARG_##num_args(func, arg, __VA_ARGS__))

/**
 * The actual functionts that map the given macro to each argument
 * For now only 16 functions are given, but more can be implemented by just adding one
 * argument to the number, parameters and call
 */
#define MAP_0(f, dummy)
#define MAP_1(f, a1) f(a1)
#define MAP_2(f, a1, a2) f(a1) f(a2)
#define MAP_3(f, a1, a2, a3) f(a1) f(a2) f(a3)
#define MAP_4(f, a1, a2, a3, a4) f(a1) f(a2) f(a3) f(a4)
#define MAP_5(f, a1, a2, a3, a4, a5) f(a1) f(a2) f(a3) f(a4) f(a5)
#define MAP_6(f, a1, a2, a3, a4, a5, a6) f(a1) f(a2) f(a3) f(a4) f(a5) f(a6)
#define MAP_7(f, a1, a2, a3, a4, a5, a6, a7) f(a1) f(a2) f(a3) f(a4) f(a5) f(a6) f(a7)
#define MAP_8(f, a1, a2, a3, a4, a5, a6, a7, a8) f(a1) f(a2) f(a3) f(a4) f(a5) f(a6) f(a7) f(a8)
#define MAP_9(f, a1, a2, a3, a4, a5, a6, a7, a8, a9) f(a1) f(a2) f(a3) f(a4) f(a5) f(a6) f(a7) f(a8) f(a9)
#define MAP_10(f, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) f(a1) f(a2) f(a3) f(a4) f(a5) f(a6) f(a7) f(a8) f(a9) f(a10)
#define MAP_11(f, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) f(a1) f(a2) f(a3) f(a4) f(a5) f(a6) f(a7) f(a8) f(a9) f(a10) f(a11)
#define MAP_12(f, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) f(a1) f(a2) f(a3) f(a4) f(a5) f(a6) f(a7) f(a8) f(a9) f(a10) f(a11) f(a12)
#define MAP_13(f, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13) f(a1) f(a2) f(a3) f(a4) f(a5) f(a6) f(a7) f(a8) f(a9) f(a10) f(a11) f(a12) f(a13)
#define MAP_14(f, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14) f(a1) f(a2) f(a3) f(a4) f(a5) f(a6) f(a7) f(a8) f(a9) f(a10) f(a11) f(a12) f(a13) f(a14)
#define MAP_15(f, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15) f(a1) f(a2) f(a3) f(a4) f(a5) f(a6) f(a7) f(a8) f(a9) f(a10) f(a11) f(a12) f(a13) f(a14) f(a15)
#define MAP_16(f, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16) f(a1) f(a2) f(a3) f(a4) f(a5) f(a6) f(a7) f(a8) f(a9) f(a10) f(a11) f(a12) f(a13) f(a14) f(a15) f(a16)

#define MAP_ARG_0(f, arg, dummy)
#define MAP_ARG_1(f, arg, a1) f(arg, a1)
#define MAP_ARG_2(f, arg, a1, a2) f(arg, a1) f(arg, a2)
#define MAP_ARG_3(f, arg, a1, a2, a3) f(arg, a1) f(arg, a2) f(arg, a3)
#define MAP_ARG_4(f, arg, a1, a2, a3, a4) f(arg, a1) f(arg, a2) f(arg, a3) f(arg, a4)
#define MAP_ARG_5(f, arg, a1, a2, a3, a4, a5) f(arg, a1) f(arg, a2) f(arg, a3) f(arg, a4) f(arg, a5)
#define MAP_ARG_6(f, arg, a1, a2, a3, a4, a5, a6) f(arg, a1) f(arg, a2) f(arg, a3) f(arg, a4) f(arg, a5) f(arg, a6)
#define MAP_ARG_7(f, arg, a1, a2, a3, a4, a5, a6, a7) f(arg, a1) f(arg, a2) f(arg, a3) f(arg, a4) f(arg, a5) f(arg, a6) f(arg, a7)
#define MAP_ARG_8(f, arg, a1, a2, a3, a4, a5, a6, a7, a8) f(arg, a1) f(arg, a2) f(arg, a3) f(arg, a4) f(arg, a5) f(arg, a6) f(arg, a7) f(arg, a8)
#define MAP_ARG_9(f, arg, a1, a2, a3, a4, a5, a6, a7, a8, a9) f(arg, a1) f(arg, a2) f(arg, a3) f(arg, a4) f(arg, a5) f(arg, a6) f(arg, a7) f(arg, a8) f(arg, a9)
#define MAP_ARG_10(f, arg, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) f(arg, a1) f(arg, a2) f(arg, a3) f(arg, a4) f(arg, a5) f(arg, a6) f(arg, a7) f(arg, a8) f(arg, a9) f(arg, a10)
#define MAP_ARG_11(f, arg, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) f(arg, a1) f(arg, a2) f(arg, a3) f(arg, a4) f(arg, a5) f(arg, a6) f(arg, a7) f(arg, a8) f(arg, a9) f(arg, a10) f(arg, a11)
#define MAP_ARG_12(f, arg, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) f(arg, a1) f(arg, a2) f(arg, a3) f(arg, a4) f(arg, a5) f(arg, a6) f(arg, a7) f(arg, a8) f(arg, a9) f(arg, a10) f(arg, a11) f(arg, a12)
#define MAP_ARG_13(f, arg, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13) f(arg, a1) f(arg, a2) f(arg, a3) f(arg, a4) f(arg, a5) f(arg, a6) f(arg, a7) f(arg, a8) f(arg, a9) f(arg, a10) f(arg, a11) f(arg, a12) f(arg, a13)
#define MAP_ARG_14(f, arg, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14) f(arg, a1) f(arg, a2) f(arg, a3) f(arg, a4) f(arg, a5) f(arg, a6) f(arg, a7) f(arg, a8) f(arg, a9) f(arg, a10) f(arg, a11) f(arg, a12) f(arg, a13) f(arg, a14)
#define MAP_ARG_15(f, arg, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15) f(arg, a1) f(arg, a2) f(arg, a3) f(arg, a4) f(arg, a5) f(arg, a6) f(arg, a7) f(arg, a8) f(arg, a9) f(arg, a10) f(arg, a11) f(arg, a12) f(arg, a13) f(arg, a14) f(arg, a15)
#define MAP_ARG_16(f, arg, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16) f(arg, a1) f(arg, a2) f(arg, a3) f(arg, a4) f(arg, a5) f(arg, a6) f(arg, a7) f(arg, a8) f(arg, a9) f(arg, a10) f(arg, a11) f(arg, a12) f(arg, a13) f(arg, a14) f(arg, a15) f(arg, a16)

#endif  // SRC_UTILS_MAP_MACRO_H
