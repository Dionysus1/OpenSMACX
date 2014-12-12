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
#include "filefind.h" // filefind_get()

/*
 * Handle file IO operations
 */

class Filemap
{
	private:
		// memory map of file, handles and size
		LPVOID mapViewAddr; // (+0)
		HANDLE hFile;       // (+4)
		HANDLE hFileMap;    // (+8)
		DWORD fileSize;     // (+12)

	public:
		// Constructor and Destructor
		Filemap(): mapViewAddr(NULL), hFile(INVALID_HANDLE_VALUE), hFileMap(NULL), fileSize(0) { }
		~Filemap() { close(); }

		// Get file's size
		DWORD getSize() { return fileSize; }
		
		// Open a file with READ access and return memory address of mapped file
		// Second param determines if file is read sequential or with random access
		// LOCATION: 00628430
		LPVOID open_read(LPCSTR pFileName, BOOL isSequential);

		// Open a file with WRITE access and the return memory address of mapped file
		// Second param determines if file is read sequential or with random access
		// LOCATION: 00628540
		LPVOID open(LPCSTR pFileName, BOOL isSequential);

		// Create a new file or overwrite an existing one with specified size 
		// filled with zeros
		// LOCATION: 00628650
		LPVOID create(LPCSTR pFileName, DWORD nSize, BOOL isSequential);

		// Close all handles and map to the file
		// LOCATION: 006283E0
		void close();
		
		// Close file and set end based on difference between mapViewAddr and newAddr
		// This can be used to truncate existing tmp files. Assumes file has WRITE access
		// LOCATION: 00628810
		void close(LPVOID newAddr);
};
