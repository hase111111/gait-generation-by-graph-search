#include "MapConst.h"

const std::string MapConst::INPUT_FILE_NAME = "map.csv";
const std::string MapConst::OUTPUT_FILE_NAME = "map.csv";

const unsigned int MapConst::HOLE_RATE = 60;

const double MapConst::STEP_HEIGHT = -70;		//段差高さ[mm]．負の値にすると下りの階段になる．
const double MapConst::STEP_LENGTH = 500;		//階段縦幅[mm]

const double MapConst::SLOPE_ANGLE = 10;		//傾斜角[deg]．
const double MapConst::TILT_ANGLE = 20;			//地形を傾ける角度[deg]．

const double MapConst::ROUGH_MAX_HEIGHT = 30;	//デコボコな地形の最大高さ
const double MapConst::ROUGH_MIN_HEIGHT = -30;	//デコボコな地形の最小高さ
