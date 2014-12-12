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

#include "filemap.h"

/*
 * Handle file IO operations
 */

LPVOID Filemap::open_read(LPCSTR pFileName, BOOL isSequential)
{
	close();
	LPCSTR pFilePaths = filefind_get(pFileName);
	if(!pFilePaths){
		pFilePaths = pFileName;
	}

	hFile = CreateFile(pFilePaths, GENERIC_READ, 0, NULL, OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL|(isSequential?FILE_FLAG_SEQUENTIAL_SCAN:FILE_FLAG_RANDOM_ACCESS), NULL);
	if(hFile == INVALID_HANDLE_VALUE){
		GetLastError(); // failed to read file
		return 0;
	}

	hFileMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
	if(hFileMap){
		mapViewAddr = MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 0);
		if(mapViewAddr){
			fileSize = GetFileSize(hFile, NULL);
			return mapViewAddr;
		}
	}
	close(); // clear everything on error
	return 0;
}

LPVOID Filemap::open(LPCSTR pFileName, BOOL isSequential)
{
	close();
	LPCSTR pFilePaths = filefind_get(pFileName);
	if(!pFilePaths){
		pFilePaths = pFileName;
	}

	hFile = CreateFile(pFilePaths, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL|(isSequential?FILE_FLAG_SEQUENTIAL_SCAN:FILE_FLAG_RANDOM_ACCESS), NULL);
	if(hFile == INVALID_HANDLE_VALUE){
		GetLastError(); // failed to read file
		return 0;
	}

	hFileMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
	if(hFileMap){
		mapViewAddr = MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		if(mapViewAddr){
			fileSize = GetFileSize(hFile, NULL);
			return mapViewAddr;
		}
	}
	close(); // clear everything on error
	return 0;
}

LPVOID Filemap::create(LPCSTR pFileName, DWORD nSize, BOOL isSequential)
{
	close();
	fileSize = nSize;

	hFile = CreateFile(pFileName, GENERIC_READ|GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL|(isSequential?FILE_FLAG_SEQUENTIAL_SCAN:FILE_FLAG_RANDOM_ACCESS), NULL);
	if(hFile == INVALID_HANDLE_VALUE){
		GetLastError();
		return 0;
	}

	if(SetFilePointer(hFile, nSize, NULL, FILE_BEGIN) != INVALID_SET_FILE_POINTER){
		SetEndOfFile(hFile);
		if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) != INVALID_SET_FILE_POINTER){
			hFileMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
			if(hFileMap){
				mapViewAddr = MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
				if(mapViewAddr){
					memset(mapViewAddr, 0, nSize);
					return mapViewAddr;
				}
			}
		}
	}
	close(); // clear everything on error
	return 0;
}

void Filemap::close()
{
	if(mapViewAddr){
		UnmapViewOfFile(mapViewAddr);
		mapViewAddr = 0;
	}

	if(hFileMap){
		CloseHandle(hFileMap);
		hFileMap = 0;
	}

	if(hFile != INVALID_HANDLE_VALUE){
		CloseHandle(hFile);
		hFile = 0;
	}

	fileSize = 0;
}

void Filemap::close(LPVOID newAddr)
{
	if(newAddr >= mapViewAddr){
		LONG newSize = LONG(newAddr) - LONG(mapViewAddr);
		if(mapViewAddr){
			UnmapViewOfFile(mapViewAddr);
			mapViewAddr = 0;
		}
		if(hFileMap){
			CloseHandle(hFileMap);
			hFileMap = 0;
		}

		if(SetFilePointer(hFile, newSize, NULL, FILE_BEGIN) != INVALID_SET_FILE_POINTER){
			SetEndOfFile(hFile);
			if(hFile){
				CloseHandle(hFile);
				hFile = 0;
			}
			return;
		}
	}
	close(); // clear everything on error
}
