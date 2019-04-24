// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#include <d3d11.h>
#include <stdio.h>

// ppl
// https://msdn.microsoft.com/en-us/library/dd492418.aspx
// https://msdn.microsoft.com/en-us/library/dd492427.aspx
#include <ppl.h>
#include <ppltasks.h>
using namespace concurrency;