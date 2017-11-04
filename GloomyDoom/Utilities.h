#pragma once
#ifndef UTILITIES_
#define UTILITIES_

template <class T> void SafeRelease(T *pT)
{
	if (pT)
	{
		(pT)->Release();
		pT = NULL;
	}
}
#endif
