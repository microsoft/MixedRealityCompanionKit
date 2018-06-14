// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once
#include <fstream>
#include "StringHelper.h"
#include <Shlwapi.h>
#include <Shellapi.h>

#pragma comment(lib, "Shlwapi")

class DirectoryHelper
{
public:
    static BOOL CreateOutputDirectory(std::wstring path);
    static BOOL IsDirectoryEmpty(std::wstring path);
    static void MoveFiles(std::wstring srcPath, std::wstring destPath);
    static BOOL FileExists(std::wstring path);
    static std::wstring FindUniqueFileName(const std::wstring path, std::wstring fileName, std::wstring extension, int& startingIndex);
    static int NumFiles(std::wstring root, std::wstring extension);
    static void DeleteFiles(std::wstring root, std::wstring extension);
};

