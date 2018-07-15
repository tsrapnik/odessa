//
// assert.cpp
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
#include <assert.h>
#include <circle/logger.h>
#include <circle/string.h>
#include <circle/sysconfig.h>
#include <circle/debug.h>
#include <circle/types.h>

#ifndef NDEBUG

void assertion_failed (const char *pExpr, const char *pFile, unsigned nLine)
{
	u64 ulStackPtr;
	asm volatile ("mov %0, sp" : "=r" (ulStackPtr));

	CString Source;
	Source.Format ("%s(%u)", pFile, nLine);

	debug_stacktrace ((u64 *) ulStackPtr, Source);
	
	CLogger::Get ()->Write (Source, LogPanic, "assertion failed: %s", pExpr);
}

#endif
