//
// timer.h
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
#ifndef _circle_timer_h
#define _circle_timer_h

#include <circle/interrupt.h>
#include <circle/string.h>
#include <circle/ptrlist.h>
#include <circle/sysconfig.h>
#include <circle/spinlock.h>
#include <circle/types.h>

#define HZ		100			// ticks per second

#define MSEC2HZ(msec)	((msec) * HZ / 1000)

typedef u64 TKernelTimerHandle;

typedef void TKernelTimerHandler (TKernelTimerHandle hTimer, void *pParam, void *pContext);

class CTimer
{
public:
	CTimer (CInterruptSystem *pInterruptSystem);
	~CTimer (void);

	boolean Initialize (void);

	void SetTime (unsigned nTime);			// Seconds since 1970-01-01 00:00:00

	unsigned GetClockTicks (void) const;		// 1 MHz counter
#define CLOCKHZ	1000000

	unsigned GetTicks (void) const;			// 1/HZ seconds since system boot
	unsigned GetUptime (void) const;		// Seconds since system boot (continous)
	unsigned GetTime (void) const;			// Seconds since system boot  or
							// Seconds since 1970-01-01 00:00:00 (if time was set)

	// "[MMM dD ]HH:MM:SS.ss", 0 if Initialize() was not yet called
	CString *GetTimeString (void);			// CString object must be deleted by caller

	// returns timer handle (0 on failure)
	TKernelTimerHandle StartKernelTimer (unsigned nDelay,		// in HZ units
					     TKernelTimerHandler *pHandler,
					     void *pParam   = 0,
					     void *pContext = 0);
	void CancelKernelTimer (TKernelTimerHandle hTimer);

	// when a CTimer object is available better use these methods
	void MsDelay (unsigned nMilliSeconds);
	void usDelay (unsigned nMicroSeconds);
	
	static CTimer *Get (void);

	// can be used before CTimer is constructed
	static void SimpleMsDelay (unsigned nMilliSeconds);
	static void SimpleusDelay (unsigned nMicroSeconds);

private:
	void PollKernelTimers (void);

	void InterruptHandler (void);
	static void InterruptHandler (void *pParam);

	void TuneMsDelay (void);

public:
	static int IsLeapYear (unsigned nYear);
	static unsigned GetDaysOfMonth (unsigned nMonth, unsigned nYear);

private:
	CInterruptSystem	*m_pInterruptSystem;

	volatile unsigned	 m_nTicks;
	volatile unsigned	 m_nUptime;
	volatile unsigned	 m_nTime;
	CSpinLock		 m_TimeSpinLock;

	CPtrList		 m_KernelTimerList;
	CSpinLock		 m_KernelTimerSpinLock;

	unsigned		 m_nMsDelay;
	unsigned		 m_nusDelay;

	static CTimer *s_pThis;

	static const unsigned s_nDaysOfMonth[12];
	static const char *s_pMonthName[12];
};

#endif
