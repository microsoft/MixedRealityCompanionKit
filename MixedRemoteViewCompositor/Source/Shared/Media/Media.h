// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

// Filters media type's attribute before they can be sent over the network.
HRESULT FilterOutputMediaType(_In_ IMFMediaType* pSourceMediaType, _In_ IMFMediaType* pDestinationMediaType);

// Used to validate media type after receiving it from the network.
HRESULT ValidateInputMediaType(_In_ REFGUID guidMajorType, _In_ REFGUID guidSubtype, _In_ IMFMediaType* pMediaType);
