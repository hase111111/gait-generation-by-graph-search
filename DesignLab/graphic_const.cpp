#include "graphic_const.h"

#include <Dxlib.h>


const std::string GraphicConst::kWindowName = "HexapodGraphic";

const int GraphicConst::kColorBit = 32;

const int GraphicConst::kBackColorRed = 170;
const int GraphicConst::kBackColorGreen = 255;
const int GraphicConst::kBackColorBlue = 255;

const float GraphicConst::kCameraFar = 4000.0f;

const float GraphicConst::kCameraNear = 0.1f;

const float GraphicConst::kCameraToTargetMax = (GraphicConst::kCameraFar + GraphicConst::kCameraNear) * 3.0f / 5.0f;

const float GraphicConst::kCameraToTargetMin = (GraphicConst::kCameraFar + GraphicConst::kCameraNear) / 10.0f;
