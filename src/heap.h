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
#include "general.h" // mem_get()

/*
 * Handle managing or allocating memory
 */

class Heap
{
	private:
		LPVOID basePtr;    // (+4) -> pointer to base memory address
		LPVOID currentPtr; // (+8) -> current memory address position
		size_t baseSize;   // (+12) -> size of total memory
		size_t freeSize;   // (+16) -> size of free available memory

	public:
		// Constructor and Destructor
		Heap(): basePtr(0), currentPtr(0), baseSize(0), freeSize(0) { }
		~Heap() { shutdown(); }

		// Get base total memory size
		size_t getBaseSize() { return baseSize; }
		// Get base memory address pointer
		LPVOID getBasePtr() { return basePtr; }

		// Destroy current heap and zero out all class variables
		// LOCATION: 005D4580
		void shutdown();

		// Deflate heap of any free memory
		// LOCATION: 005D45E0
		void squeeze();

		// Allocate memory based on requested size from parameter
		// Return TRUE if memory allocation failed, otherwise FALSE
		// LOCATION: 005D4620
		BOOL init(size_t reqSize);

		// Check if there is currently enough free memory for requested size from parameter
		// If not, allocate more memory to heap in blocks of 1024 bytes until there is enough
		// Return a memory pointer with address to requested size free
		// LOCATION: 005D4680
		LPVOID get(size_t reqSize);
};
