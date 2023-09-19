#include "graphic_const.h"

#include "DxLib.h"


const std::string GraphicConst::WIN_NAME = "HexapodGraphic";

const int GraphicConst::COLOR_BIT = 32;

const int GraphicConst::BACK_COLOR_R = 170;
const int GraphicConst::BACK_COLOR_G = 255;
const int GraphicConst::BACK_COLOR_B = 255;

const float GraphicConst::CAMERA_FAR = 4000.0f;

const float GraphicConst::CAMERA_NEAR = 0.1f;

const float GraphicConst::CAMERA_TO_TARGET_MAX = (GraphicConst::CAMERA_FAR + GraphicConst::CAMERA_NEAR) * 3.0f / 5.0f;

const float GraphicConst::CAMERA_TO_TARGET_MIN = (GraphicConst::CAMERA_FAR + GraphicConst::CAMERA_NEAR) / 10.0f;
