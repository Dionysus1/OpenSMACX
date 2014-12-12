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

#pragma once
#include "stdafx.h"

/*
 * Handle debug specific functions for testing purposes
 */

class Debug
{
	private:
		double timerFreq; // frequency for timer
		LONGLONG counterStart; // counter for start time of timer

	public:
		// Initialize the counter and set the timer frequency
		void StartCounter();

		// Calculates the difference between the start time and current time
		// This can be used to calculate the time it takes for code to finish
		double GetCounter();
};

#endif
