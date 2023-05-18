#include "MapController.h"

void AreaDivide(const myvector::SVector& p1, const myvector::SVector& p2, int& x1, int& x2, int& y1, int& y2)
{
	//脚設置可能点が入っている領域の大さ
	double _lengthX = (MapConst::MAP_X_MAX - MapConst::MAP_X_MIN) / (double)MapConst::LP_DIVIDE_NUM;
	double _lengthY = (MapConst::MAP_Y_MAX - MapConst::MAP_Y_MIN) / (double)MapConst::LP_DIVIDE_NUM;

	//ブロック番号計算
	x1 = (int)((p1.x - MapConst::MAP_X_MIN) / _lengthX);
	x2 = (int)((p2.x - MapConst::MAP_X_MIN) / _lengthX);
	y1 = (int)((p1.y - MapConst::MAP_Y_MIN) / _lengthY);
	y2 = (int)((p2.y - MapConst::MAP_Y_MIN) / _lengthY);

	if (x1 >= MapConst::LP_DIVIDE_NUM) { x1 = MapConst::LP_DIVIDE_NUM - 1; }//想定外の範囲の場合は端っこのブロックに収めるようにする
	if (x1 < 0) { x1 = 0; }
	if (y1 >= MapConst::LP_DIVIDE_NUM) { y1 = MapConst::LP_DIVIDE_NUM - 1; }
	if (y1 < 0) { y1 = 0; }
	if (x2 >= MapConst::LP_DIVIDE_NUM) { x2 = MapConst::LP_DIVIDE_NUM - 1; }	//想定外の範囲の場合は端っこのブロックに収めるようにする
	if (x2 < 0) { x2 = 0; }
	if (y2 >= MapConst::LP_DIVIDE_NUM) { y2 = MapConst::LP_DIVIDE_NUM - 1; }
	if (y2 < 0) { y2 = 0; }
}

void MapSqrtDivide(const myvector::SVector mapData[MapConst::MAPDATA3D_MAX], std::vector< std::vector< std::vector<myvector::SVector> > >& divideMapData, int pointNum[MapConst::LP_DIVIDE_NUM][MapConst::LP_DIVIDE_NUM])
{
	// 0 で初期化する
	for (int i = 0; i < MapConst::LP_DIVIDE_NUM; ++i)
	{
		for (int j = 0; j < MapConst::LP_DIVIDE_NUM; ++j)
		{
			pointNum[i][j] = 0;	
		}
	}

	//脚設置可能点が入っている領域の大さ
	const double _lengthX = (MapConst::MAP_X_MAX - MapConst::MAP_X_MIN) / (double)MapConst::LP_DIVIDE_NUM;
	const double _lengthY = (MapConst::MAP_Y_MAX - MapConst::MAP_Y_MIN) / (double)MapConst::LP_DIVIDE_NUM;

	int x, y, t;
	for (int i = 0; i < MapConst::MAPDATA3D_MAX; ++i)
	{
		//無効なデータならば処理を飛ばす
		if (mapData[i].x == MapConst::INVALID_FOOT_HOLD) { continue; }

		//xy方向のブロック番号をそれぞれ求める
		x = (int)((mapData[i].x - MapConst::MAP_X_MIN) / _lengthX);		
		y = (int)((mapData[i].y - MapConst::MAP_Y_MIN) / _lengthY);		//y方向のブロック番号

		if (x >= MapConst::LP_DIVIDE_NUM) { x = MapConst::LP_DIVIDE_NUM - 1; }	//想定外の範囲の場合は端っこのブロックに収めるようにする
		if (x < 0) { x = 0; }
		if (y >= MapConst::LP_DIVIDE_NUM) { y = MapConst::LP_DIVIDE_NUM - 1; }
		if (y < 0) { y = 0; }

		t = pointNum[x][y];

		if (divideMapData[x][y].size() <= pointNum[x][y])
		{
			//足りなくなったら現在の倍サイズ確保　使いまわすときはswapで小さくする（resizeだとcapacityは変わらないため）
			divideMapData[x][y].resize(divideMapData[x][y].size() * 2);
		}

		divideMapData[x][y][t] = mapData[i];
		++pointNum[x][y];
	}
}

void recalMap(myvector::SVector p_mapData3D[MapConst::MAPDATA3D_MAX], const LNODE& _current_condition, const  LNODE& _past_condition)
{
	for (int i = 0; i < MapConst::MAPDATA3D_MAX; i++)
	{
		//ひとつ前のロボット座標→グローバル座標→現在のロボット座標
		p_mapData3D[i] = myvector::VRot(p_mapData3D[i], _past_condition.global_center_of_mass, _past_condition.pitch, _past_condition.roll, _past_condition.yaw);
		p_mapData3D[i] = p_mapData3D[i] + _past_condition.global_center_of_mass;
		p_mapData3D[i] = p_mapData3D[i] - _current_condition.global_center_of_mass;
		p_mapData3D[i] = myvector::VRot(p_mapData3D[i], _current_condition.global_center_of_mass, -_current_condition.pitch, -_current_condition.roll, -_current_condition.yaw);
	}
}
