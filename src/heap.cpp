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

#include "heap.h"

/*
 * Handle managing or allocating memory
 */

void Heap::shutdown()
{
	if(basePtr){
		free(basePtr);
	}
	basePtr = currentPtr = 0;
	baseSize = freeSize = 0;
}

void Heap::squeeze()
{
	size_t usedSize = baseSize - freeSize;
	LPVOID newAddr = realloc(basePtr, usedSize);
	if(newAddr){
		basePtr = newAddr;
		baseSize = usedSize;
		freeSize = 0;
	}
}

BOOL Heap::init(size_t reqSize)
{
	if(basePtr){
		shutdown();
	}
	basePtr = mem_get(reqSize);
	if(basePtr){
		currentPtr = basePtr;
		baseSize = freeSize = reqSize;
		return FALSE;
	}
	return TRUE; // failed to allocate memory
}

LPVOID Heap::get(size_t reqSize)
{
	while(freeSize < reqSize){
		LPVOID newAddr = realloc(basePtr, baseSize + 1024);
		if(!newAddr){
			CHAR szError[150]; // max size of string + three int(s) + extra
			wsprintf(szError, "Aborting due to a heap shortage!\nBase size: %d\nFree size: %d\nRequested size: %d", 
				baseSize, freeSize, reqSize);
			MessageBox(NULL, szError, "Heap Notice!!", MB_OK);
			exit(3);
		}
		basePtr = newAddr;
		currentPtr = LPVOID(size_t(basePtr) + baseSize - freeSize); // fix in case realloc shifts memory
		baseSize += 1024;
		freeSize += 1024;
	}
	LPVOID freeMemAddr = currentPtr;
	freeSize -= reqSize;
	currentPtr = LPVOID(size_t(currentPtr) + reqSize);
	return freeMemAddr;
}
