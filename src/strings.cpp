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

#include "strings.h"

/*
 * Create and interact with a basic string table
 */

BOOL Strings::init(size_t memSize)
{
	if(isPopulated){
		shutdown();
	}
	if(Heap::init(memSize)){
		return TRUE; // allocation failed
	}
	put("-Nil-");
	isPopulated = TRUE;
	return FALSE; // successful
}

void Strings::shutdown()
{
	Heap::shutdown();
	isPopulated = FALSE;
}

LPCSTR Strings::put(LPCSTR input)
{
	int nLen = strlen(input) + 1;
	LPSTR tableAddr = LPSTR(Heap::get(nLen));
	strcpy_s(tableAddr, nLen, input);
	return tableAddr;
}

LPCSTR Strings::get(LPCSTR input)
{
	if(input > LPCSTR(getBasePtr())){ // checking if ptr is after base
		return (input <= LPSTR(size_t(getBasePtr()) + getBaseSize())) ? input : NULL; // checking ptr is not after end of table
	}
	return LPCSTR(getBasePtr()); // return base
}
