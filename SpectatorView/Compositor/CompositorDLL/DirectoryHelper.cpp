// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "stdafx.h"
#include "DirectoryHelper.h"

// Create a directory for the input path.
BOOL DirectoryHelper::CreateOutputDirectory(std::wstring path)
{
    return CreateDirectory(path.c_str(), NULL);
}

BOOL DirectoryHelper::IsDirectoryEmpty(std::wstring path)
{
    return PathIsDirectoryEmpty(path.c_str());
}

void DirectoryHelper::MoveFiles(std::wstring srcPath, std::wstring destPath)
{
    WIN32_FIND_DATA findFileData;
    std::wstring searchCriteria = srcPath + L"\\*.*";
    HANDLE hFind = FindFirstFile(searchCriteria.c_str(), &findFileData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        return ;
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

// Check if the input file exists.
BOOL DirectoryHelper::FileExists(std::wstring path)
{
    std::ifstream infile(path.c_str());
    return infile.good();
}

// Find a filename with a unique leading index.  Return the full unique path and output the leading index that was used.
std::wstring DirectoryHelper::FindUniqueFileName(const std::wstring path, std::wstring fileName, std::wstring extension, int& startingIndex)
{
    std::wstring tempPath = path + L"\\" + std::to_wstring(startingIndex) + L"_" + fileName + extension;

    while(FileExists(tempPath))
    {
        startingIndex++;
        tempPath = path + L"\\" + std::to_wstring(startingIndex) + L"_" + fileName + extension;
    }

    return tempPath;
}

// Get the number of files with the given extension in the input directory.
int DirectoryHelper::NumFiles(std::wstring root, std::wstring extension)
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

// Delete all files with a given extension in the input directory.
void DirectoryHelper::DeleteFiles(std::wstring root, std::wstring extension)
{
    WIN32_FIND_DATA findFileData;
    std::wstring searchCriteria = root + L"*" + extension;

    HANDLE hFind = FindFirstFile(searchCriteria.c_str(), &findFileData);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            DeleteFileW((root + findFileData.cFileName).c_str());
        } 
        while (FindNextFile(hFind, &findFileData));

        FindClose(hFind);
    }
}
