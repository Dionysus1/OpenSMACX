/*
 * Copyright 2013-2014 Brendan Casey.
 *
 * OpenSMACX is free software: you can redistribute it and / or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenSMACX is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenSMACX. If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef _DEBUG

#include "debug.h"

/*
 * Handle debug specific functions for testing purposes
 */

void Debug::StartCounter()
{
	LARGE_INTEGER li;
	if(!QueryPerformanceFrequency(&li)){
		OutputDebugString("QueryPerformanceFrequency failed!\n");
	}
	timerFreq = double(li.QuadPart) / 1000.0;
	QueryPerformanceCounter(&li);
	counterStart = li.QuadPart;
}

double Debug::GetCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return double(li.QuadPart - counterStart) / timerFreq;
}

#endif
