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

#pragma once
#include "stdafx.h"
#include "heap.h"

/*
 * Create and interact with a basic string table
 */

class Strings: Heap
{
	private:
		BOOL isPopulated; // (+20) -> set to TRUE when table is created

	public:
		// Constructor, other variables are from subclass (Heap)
		Strings(): isPopulated(FALSE) { }

		// Initialize a new string table with specified size from parameter
		// Return TRUE if memory allocation failed, otherwise FALSE
		// LOCATION: 006168F0
		BOOL init(size_t memSize);

		// Shutdown and free up string table memory
		// LOCATION: 00616950
		void shutdown();

		// Store input string to string table
		// Return address of stored string
		// LOCATION: 00616970
		LPCSTR put(LPCSTR input);

		// Check if string is in table by looking at bounds within heap
		// Return address of stored string
		// LOCATION: 006169A0
		LPCSTR get(LPCSTR input);
};
