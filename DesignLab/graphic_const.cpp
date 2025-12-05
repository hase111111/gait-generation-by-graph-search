
//! @file graphic_const.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include "graphic_const.h"

#include <Dxlib.h>

namespace gaitgen {

const char GraphicConst::kWindowName[] = "HexapodGraphic";

const int GraphicConst::kColorBit = 32;

const int GraphicConst::kBackColorRed = 170;
const int GraphicConst::kBackColorGreen = 255;
const int GraphicConst::kBackColorBlue = 255;

const float GraphicConst::kCameraToTargetMax = 10000.0f;

const float GraphicConst::kCameraToTargetMin = 10.0f;

}  // namespace gaitgen
