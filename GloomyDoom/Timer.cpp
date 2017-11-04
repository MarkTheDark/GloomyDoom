#include "stdafx.h"

namespace mjh
{
	Timer::Timer()
	{
		lastStampMicro = getTimestampMicro();
		
	}

	// getters
	double Timer::getTimestampMicro()
	{
		LARGE_INTEGER StartingTime, ElapsedMicroseconds;
		LARGE_INTEGER Frequency;

		QueryPerformanceFrequency(&Frequency);
		QueryPerformanceCounter(&StartingTime);
		ElapsedMicroseconds.QuadPart = StartingTime.QuadPart;

		//convert to microseconds
		ElapsedMicroseconds.QuadPart = (LONGLONG)(ElapsedMicroseconds.QuadPart * NANO_TO_MICRO);
		ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

		//save delta
		deltaMicro = ElapsedMicroseconds.QuadPart - lastStampMicro;

		//save last timestamp
		lastStampMicro = (double)ElapsedMicroseconds.QuadPart;

		return (double)ElapsedMicroseconds.QuadPart;
	}

	double Timer::getTimestampMilli()
	{
		return getTimestampMicro() / MICRO_TO_MILLI;
	}

	double Timer::getLastTimestampMicro()
	{
		return lastStampMicro;
	}

	double Timer::getLastTimestampMilli()
	{
		return lastStampMicro / MICRO_TO_MILLI;
	}

	double Timer::getDeltaMicro()
	{
		return deltaMicro;
	}

	double Timer::getDeltaMilli()
	{
		return deltaMicro / MICRO_TO_MILLI;
	}
}