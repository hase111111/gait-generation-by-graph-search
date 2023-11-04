#include "map_const.h"

const std::string MapConst::INPUT_FILE_NAME = "map.csv";
const std::string MapConst::OUTPUT_FILE_NAME = "map.csv";

const unsigned int MapConst::HOLE_RATE = 20;

const float MapConst::STEP_HEIGHT = 100.0f;		//段差高さ[mm]．負の値にすると下りの階段になる．
const float MapConst::STEP_LENGTH = 2400.0f;		//階段縦幅[mm]

const float MapConst::SLOPE_ANGLE = -10.0f;		//傾斜角[deg]．
const float MapConst::TILT_ANGLE = 10.0f;		//地形を傾ける角度[deg]．

const float MapConst::ROUGH_MAX_HEIGHT = 30.0f;	//デコボコな地形の最大高さ
const float MapConst::ROUGH_MIN_HEIGHT = -30.0f;//デコボコな地形の最小高さ
