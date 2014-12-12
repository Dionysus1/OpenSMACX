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

#include "textindex.h"

/*
 * Designed to speed up the time it takes to find string resources
 * in text files by creating an index of the file sections prefixed by '#'
 */

void TextIndex::make_index(LPCSTR sourceTxt)
{
	clear();
	strcpy_s(fileName, 256, sourceTxt);
	if(!strchr(fileName, '.')){
		strcat_s(fileName, 256, ".txt"); // append extension if missing
	}
	Filemap txtFileMap;
	LPVOID txtAddrBase = txtFileMap.open_read(fileName, FALSE);
	if(txtAddrBase){
		LPVOID seekAddr = txtAddrBase;
		LPVOID eofAddr = LPVOID(DWORD(txtAddrBase) + txtFileMap.getSize());
		Heap::init(0x8000); // maximum amount of memory per section index
		while(seekAddr < eofAddr){
			LPVOID foundAddr = memchr(seekAddr, '\n', DWORD(eofAddr) - DWORD(seekAddr));
			if(!foundAddr){
				break; // invalid file, no newlines
			}
			char parseBuffer[512]; // maximum line length
			memcpy_s(parseBuffer, 512, seekAddr, DWORD(foundAddr) - DWORD(seekAddr) + 1);
			kill_newline(parseBuffer);
			trim_spaces(parseBuffer);
			if(parseBuffer[0] == '#' && isupper(parseBuffer[1])){
				DWORD len = strlen(&parseBuffer[1]) + 1;
				LPVOID storeAddr = Heap::get(len + 4); // section name + address
				*(LPDWORD)storeAddr = DWORD(seekAddr) - DWORD(txtAddrBase); // file offset
				strcpy_s(LPSTR(storeAddr) + 4, len, &parseBuffer[1]);
				sectionCount++;
			}
			seekAddr = LPVOID(DWORD(foundAddr) + 1);
		}
		Heap::squeeze();
	}
}

DWORD TextIndex::search_index(LPCSTR sourceTxt, LPCSTR sectionTxt)
{
	char fileNameCheck[256];
	strcpy_s(fileNameCheck, 256, sourceTxt);
	if(!strchr(fileNameCheck, '.')){
		strcat_s(fileNameCheck, 256, ".txt"); // append extension if missing
	}
	if(!_stricmp(fileNameCheck, fileName)){
		if(sectionTxt[0] == '#'){
			sectionTxt++;
		}
		LPSTR cmpAddr = LPSTR(getBasePtr()) + 4;
		for(int i = sectionCount; i; i--){
			if(!_stricmp(cmpAddr, sectionTxt)){
				return *LPDWORD(cmpAddr-4);
			}
			while(*(LPSTR)cmpAddr++);
			cmpAddr+=4;
		}
	}
	return -1;
}
