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
#include "heap.h"
#include "filemap.h"

/*
 * Designed to speed up the time it takes to find string resources
 * in text files by creating an index of the file sections prefixed by '#'
 */

class TextIndex: Heap
{
	private:
		char fileName[256]; // name of file to be mapped
		DWORD sectionCount; // number of section entries

	public:
		// Constructor
		TextIndex(): sectionCount(0) { fileName[0] = 0; }

		// Read specified file in from parameter and create an index of 
		// section headers ("#EXAMPLE"). Store the file offset value
		// of said sections for improving search time.
		// LOCATION: 005FDF80
		void make_index(LPCSTR sourceTxt);

		// Search the source text file from parameter 1 for section header in 
		// parameter 2 and return the file offset if found. If not found,
		// return -1.
		// LOCATION: 005FE120
		DWORD search_index(LPCSTR sourceTxt, LPCSTR sectionTxt);

		// Get the total of number of section entries for the text file
		DWORD getSectionCount() { return sectionCount; }

		// Clear existing search index
		void clear() { Heap::shutdown(); sectionCount = 0; }
};
