#include "HexapodConst.h"
#include "MyMath.h"

const float HexapodConst::COXA_LENGTH = 52.0f;

const float HexapodConst::FEMUR_LENGTH = 66.0f;

const float HexapodConst::TIBIA_LENGTH = 130.0f;

const float HexapodConst::BODY_FRONT_WIDTH = 60.0f;

const float HexapodConst::BODY_CENTER_WIDTH = 100.0f;

const float HexapodConst::BODY_REAR_WIDTH = 60.0f;

const float HexapodConst::BODY_FRONT_LENGTH = 120.0f;

const float HexapodConst::BODY_REAR_LENGTH = 120.0f;

const float HexapodConst::BODY_HEIGHT = 40.0f;

const float HexapodConst::VERTICAL_MAX_RANGE = 160.0f;

const float HexapodConst::VERTICAL_MIN_RANGE = 30.0f;

const float HexapodConst::MOVABLE_LEG_RANGE = my_math::convertDegToRad(40.0f);

const float HexapodConst::DEFAULT_LEG_ANGLE[] = {	my_math::convertDegToRad(-45.0f),	my_math::convertDegToRad(-90.0f),	my_math::convertDegToRad(-135.0f),
													my_math::convertDegToRad(135.0f),	my_math::convertDegToRad(90.0f),	my_math::convertDegToRad(45.0f) };
