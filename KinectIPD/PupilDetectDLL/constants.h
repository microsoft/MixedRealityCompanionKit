// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#ifndef CONSTANTS_H
#define CONSTANTS_H

// Debugging
const bool kPlotVectorField = false;

// Size constants
const int kEyePercentTop = 25;
const int kEyePercentSide = 13;
const int kEyePercentHeight = 30;
const int kEyePercentWidth = 35;

// Preprocessing
const bool kSmoothFaceImage = false;
const float kSmoothFaceFactor = 0.005;


// Algorithm Parameters
//const int kFastEyeWidth = 75;
//const int kWeightBlurSize = 9;
//const bool kEnableWeight = true;
//const float kWeightDivisor = 80.0;
//const double kGradientThreshold = 150.0;

// Postprocessing
//const bool kEnablePostProcess = true;
//const float kPostProcessThreshold = 0.95;// 97;

// Eye Corner
const bool kEnableEyeCorner = false;

#endif