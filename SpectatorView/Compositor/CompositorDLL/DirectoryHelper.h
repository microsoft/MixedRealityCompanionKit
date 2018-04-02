// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once
#include "stdafx.h"
#include <string>
#include <fstream>
#include "StringHelper.h"
#include <Shlwapi.h>
#include <Shellapi.h>

#pragma comment(lib, "Shlwapi")

class DirectoryHelper
{
public:
	static BOOL CreateOutputDirectory(std::wstring path)
	{
		return CreateDirectory(path.c_str(), NULL);
	}

	static BOOL IsDirectoryEmpty(std::wstring path)
	{
		return PathIsDirectoryEmpty(path.c_str());
	}

	static void MoveFiles(std::wstring srcPath, std::wstring destPath)
	{
		WIN32_FIND_DATA findFileData;
		std::wstring searchCriteria = srcPath + L"\\*.*";
		HANDLE hFind = FindFirstFile(searchCriteria.c_str(), &findFileData);
		if (hFind == INVALID_HANDLE_VALUE)
		{
			return;
		}

		while (FindNextFile(hFind, &findFileData))
		{
			if (findFileData.cFileName != L"." && findFileData.cFileName != L"..")
			{
				MoveFile((srcPath + findFileData.cFileName).c_str(), (destPath + findFileData.cFileName).c_str());
			}
		}

		FindClose(hFind);
	}

	static BOOL FileExists(std::wstring path)
	{
		std::ifstream infile(path.c_str());
		return infile.good();
	}

	static std::wstring FindUniqueFileName(const std::wstring path, std::wstring fileName, std::wstring extension, int& startingIndex)
	{
		std::wstring tempPath = path + L"\\" + std::to_wstring(startingIndex) + L"_" + fileName + extension;

		while (FileExists(tempPath))
		{
			startingIndex++;
			tempPath = path + L"\\" + std::to_wstring(startingIndex) + L"_" + fileName + extension;
		}

		return tempPath;
	}

	static int NumFiles(std::wstring root, std::wstring extension)
	{
		int numFiles = 0;

		// Find number of files in the output directory
		//https://msdn.microsoft.com/en-us/library/windows/desktop/aa364418(v=vs.85).aspx
		WIN32_FIND_DATA findFileData;
		std::wstring searchCriteria = root + L"*" + extension;
		HANDLE hFind = FindFirstFile(searchCriteria.c_str(), &findFileData);
		if (hFind == INVALID_HANDLE_VALUE)
		{
			return 0;
		}

		numFiles++;
		while (FindNextFile(hFind, &findFileData))
		{
			numFiles++;
		}

		FindClose(hFind);
		return numFiles;
	}

	static void DeleteFiles(std::wstring root, std::wstring extension)
	{
		WIN32_FIND_DATA findFileData;
		std::wstring searchCriteria = root + L"*" + extension;

		HANDLE hFind = FindFirstFile(searchCriteria.c_str(), &findFileData);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				DeleteFileW((root + findFileData.cFileName).c_str());
			} while (FindNextFile(hFind, &findFileData));

			FindClose(hFind);
		}
	}
};

