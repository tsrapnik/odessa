//
// synchronize.h
//
// Circle - A C++ bare metal environment for Raspberry Pi
// Copyright (C) 2014-2016  R. Stange <rsta2@o2online.de>
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
#ifndef _circle_synchronize_h
#define _circle_synchronize_h

#include <circle/macros.h>
#include <circle/types.h>

#ifdef __cplusplus
extern "C" {
#endif

//
// Interrupt control
//
#define	EnableInterrupts()	asm volatile ("msr DAIFClr, #2")
#define	DisableInterrupts()	asm volatile ("msr DAIFSet, #2")

void EnterCritical (void);
void LeaveCritical (void);

//
// Cache control
//
#define InvalidateInstructionCache()	asm volatile ("ic iallu" ::: "memory")
#define FlushPrefetchBuffer()		asm volatile ("isb" ::: "memory")

void InvalidateDataCache (void) MAXOPT;
void InvalidateDataCacheL1Only (void) MAXOPT;
void CleanDataCache (void) MAXOPT;

void InvalidateDataCacheRange (u64 nAddress, u64 nLength) MAXOPT;
void CleanDataCacheRange (u64 nAddress, u64 nLength) MAXOPT;
void CleanAndInvalidateDataCacheRange (u64 nAddress, u64 nLength) MAXOPT;

//
// Barriers
//
#define DataSyncBarrier()	asm volatile ("dsb sy" ::: "memory")
#define DataMemBarrier() 	asm volatile ("dmb sy" ::: "memory")

#define InstructionSyncBarrier() asm volatile ("isb" ::: "memory")
#define InstructionMemBarrier()	asm volatile ("isb" ::: "memory")

#define CompilerBarrier()	asm volatile ("" ::: "memory")

#ifdef __cplusplus
}
#endif

#endif
