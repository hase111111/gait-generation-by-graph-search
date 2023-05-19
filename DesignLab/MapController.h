#pragma once
#include "MapConst.h"
#include <vector>
#include "vectorFunc.h"
#include "listFunc.h"

// SPLPで使用．与えられた座標からブロック番号を求める p1:四角形エリアの左下の点 p2:右上の点　　x1:最小　x2:最大　y1:最小　y2:最大
void AreaDivide(const myvector::SVector& p1, const myvector::SVector& p2, int& x1, int& x2, int& y1, int& y2);

//マップの平行移動 main.cpp
void recalMap(myvector::SVector p_mapData3D[MapConst::MAPDATA3D_MAX], const LNODE& _current_condition, const  LNODE& _past_condition);
