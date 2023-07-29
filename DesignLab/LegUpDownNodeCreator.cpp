#include "LegUpDownNodeCreator.h"

#include <algorithm>
#include <iostream>

#include "ComType.h"
#include "GraphSearchConst.h"
#include "LegState.h"


LegUpDownNodeCreator::LegUpDownNodeCreator(const MapState* const p_Map, const EHexapodMove next_move) : INodeCreator(p_Map, next_move), mp_map(p_Map)
{
	if (GraphSearchConst::DO_DEBUG_PRINT)
	{
		std::cout << "[NodeCreator] LegUpDownNodeCreator : コンストラクタが呼ばれた．\n";
	}
};

LegUpDownNodeCreator::~LegUpDownNodeCreator()
{
	if (GraphSearchConst::DO_DEBUG_PRINT)
	{
		std::cout << "[NodeCreator] LegUpDownNodeCreator : デストラクタが呼ばれた．\n";
	}
}

void LegUpDownNodeCreator::create(const SNode& _current_node, const int _current_num, std::vector<SNode>* output_graph)
{
	//脚の遊脚・接地によって生じるとりうる重心をcomtypeとして仕分けている．(詳しくはComtype.hを参照)．まずは全てtrueにしておく．
	bool _is_able_type[ComType::COM_TYPE_NUM];

	for (int i = 0; i < ComType::COM_TYPE_NUM; i++)
	{
		_is_able_type[i] = true;
	}

	//重心が現在どこにあるか(前よりか真ん中か...)などのパラメータはこのcom patternで仕分けている．(詳しくはComtype.hを参照)．これを取得する．
	int _com_pattern = LegStateEdit::getComPatternState(_current_node.leg_state);

	//com patternよりとることができないcom typeを全てfalseにする．
	ComType::checkAbleComTypeFromComPattern(_com_pattern, _is_able_type);



	//次に脚が地面に接地可能か調べる．

	bool _is_groundable[HexapodConst::LEG_NUM];			//脚が設置可能ならばtrueになる．既に接地しているならばtrueになる．
	my_vec::SVector _ground_pos[HexapodConst::LEG_NUM];	//脚が接地する座標．

	for (int i = 0; i < HexapodConst::LEG_NUM; i++) { _ground_pos[i] = _current_node.leg_pos[i]; }

	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		if (LegStateEdit::isGrounded(_current_node.leg_state, i) == true)
		{
			//すでに接地している脚は接地可能に決まっているのでtrueにする．
			_is_groundable[i] = true;
			_ground_pos[i] = _current_node.leg_pos[i];
		}
		else
		{
			//現在遊脚中の脚は自身の脚状態で接地できるか検討する．
			my_vec::SVector res_ground_pos;

			if (isGroundableLeg(i, _current_node, &res_ground_pos) == true)
			{
				_is_groundable[i] = true;	//接地可能にする．
				_ground_pos[i] = res_ground_pos;
			}
			else
			{
				_is_groundable[i] = false;	//接地不可能にする．
				ComType::checkAbleComTypeFromNotGroundableLeg(i, _is_able_type);	//接地不可能な脚によって，とれないcom typeを全てけす．
			}
		}
	}


	//子ノードを生成する．
	for (int i = 0; i < ComType::COM_TYPE_NUM; i++)
	{
		//その重心タイプが可能であれば，
		if (_is_able_type[i] == true)
		{
			SNode _res_node = _current_node;
			_res_node.changeNextNode(_current_num, m_next_move);

			//遊脚・接地を書き換える．
			bool _temp_ground[HexapodConst::LEG_NUM] = {};
			ComType::getGroundLegFromComType(i, _temp_ground);

			for (int j = 0; j < HexapodConst::LEG_NUM; j++)
			{
				LegStateEdit::changeGround(_res_node.leg_state, j, _temp_ground[j]);

				if (_temp_ground[j] == true)
				{
					_res_node.leg_pos[j] = _ground_pos[j];
				}
				else
				{
					//_res_node.leg_pos[j].x = 160 * HexapodConst::DEFAULT_LEG_ANGLE_COS[j];
					//_res_node.leg_pos[j].y = 160 * HexapodConst::DEFAULT_LEG_ANGLE_SIN[j];
					_res_node.leg_pos[j].z = -10;
				}
			}

			(*output_graph).push_back(_res_node);
		}

	}

}

bool LegUpDownNodeCreator::isGroundableLeg(const int _leg_num, const SNode& _current_node, my_vec::SVector* output_ground_pos)
{
	//for文の中のcontinueについては http://www9.plala.or.jp/sgwr-t/c/sec06-7.html を参照．ちなみに読みづらくなるので本当は使わないほうがいい．

	using my_vec::SVector;

	if (mp_map == nullptr) { return false; }	//マップがないときはfalseを返す．

	//脚座標がdevide mapでどこに当たるか調べて，そのマスの2つ上と2つ下の範囲内を全て探索する．
	const my_vec::SVector global_legbase_pos = m_calclator.getGlobalLegBasePos(_current_node, _leg_num, false);

	int max_x_dev = mp_map->getDevideMapNumX(global_legbase_pos.x) + 1;
	int min_x_dev = mp_map->getDevideMapNumX(global_legbase_pos.x) - 1;
	int max_y_dev = mp_map->getDevideMapNumY(global_legbase_pos.y) + 1;
	int min_y_dev = mp_map->getDevideMapNumY(global_legbase_pos.y) - 1;

	////値がdevide mapの範囲外にあるときは丸める．
	max_x_dev = (max_x_dev >= MapConst::LP_DIVIDE_NUM) ? MapConst::LP_DIVIDE_NUM - 1 : max_x_dev;
	min_x_dev = (min_x_dev < 0) ? 0 : min_x_dev;
	max_y_dev = (max_y_dev >= MapConst::LP_DIVIDE_NUM) ? MapConst::LP_DIVIDE_NUM - 1 : max_y_dev;
	min_y_dev = (min_y_dev < 0) ? 0 : min_y_dev;


	//devide map内を全探索して，現在の脚位置(離散化した物)に適した脚設置可能点が存在するか調べる．

	my_vec::SVector candidate_pos;		//現在の脚位置に合致する候補座標群．
	bool is_candidate_pos = false;		//候補座標が存在するかどうか．

	//範囲内の点を全て調べる．
	for (int x = min_x_dev; x < max_x_dev; x++)
	{
		for (int y = min_y_dev; y < max_y_dev; y++)
		{
			const int _pos_num = mp_map->getPointNumFromDevideMap(x, y);

			for (int n = 0; n < _pos_num; n++)
			{
				SVector map_point_pos = mp_map->getPosFromDevideMap(x, y, n);	//脚設置可能点の座標を取り出す．
				map_point_pos = m_calclator.convertLocalLegPos(_current_node, map_point_pos, _leg_num, false);

				//脚位置を更新したノードを作成する．
				SNode _new_node = _current_node;

				_new_node.leg_pos[_leg_num] = map_point_pos;


				//前の候補地点と比較して，より良い候補地点の時のみ実行すする
				if (is_candidate_pos == true)
				{
					//反対方向をむいている場合は候補地点として採用しない．
					if (_new_node.leg_base_pos[_leg_num].projectedXY().cross(candidate_pos.projectedXY()) * _new_node.leg_base_pos[_leg_num].projectedXY().cross(map_point_pos.projectedXY()) < 0)
					{
						continue;
					}

					//現在の脚位置と候補地点の間に障害物がある場合は候補地点として採用しない．
					if (map_point_pos.projectedXY().cross(candidate_pos.projectedXY()) * map_point_pos.projectedXY().cross(_new_node.leg_base_pos[_leg_num].projectedXY()) < 0)
					{
						continue;
					}
				}

				LegStateEdit::changeGround(_new_node.leg_state, _leg_num, true);

				if (m_calclator.isLegInRange(_new_node, _leg_num) == false) { continue; }			//脚が範囲外ならば追加せずに続行．

				//if (m_calclator.isLegInterfering(_new_node) == true) { continue; }					//脚が干渉しているならば追加せずに続行．

				//if (m_calclator.isAblePause(_new_node) == false) { continue; }						//脚が地面についているならば追加せずに続行．

				if (isAbleLegPos(_new_node, _leg_num) == false) { continue; }	//候補座標として，適していないならば追加せずに続行．

				is_candidate_pos = true;
				candidate_pos = map_point_pos;
			}
		}
	}


	//候補点を全列挙したのち，候補点が一つもなければfalse
	if (is_candidate_pos == false) { return false; }

	//存在するなら，その中で最も適したものを結果として返し，true
	*output_ground_pos = candidate_pos;

	return true;
}

bool LegUpDownNodeCreator::isAbleLegPos(const SNode& _node, const int _leg_num)
{
	int _leg_state = LegStateEdit::getLegState(_node.leg_state, _leg_num);		//脚位置を取得(1～7)

	//まず最初に脚位置4のところにないか確かめる．
	if ((_node.leg_base_pos[_leg_num] - _node.leg_pos[_leg_num]).lengthSquare() < my_math::squared(LEG_MARGIN))
	{
		if (_leg_state == 4) { return true; }
		else { return false; }
	}
	else
	{
		if (_leg_state == 4) { return false; }
	}

	//脚位置4と比較して前か後ろか
	if (_node.leg_base_pos[_leg_num].projectedXY().cross(_node.leg_pos[_leg_num].projectedXY()) * _node.leg_pos[_leg_num].projectedXY().cross({ 1,0 }) > 0)
	{
		//前

		if (_leg_state == 1 || _leg_state == 2 || _leg_state == 3)
		{
			return false;
		}
	}
	else
	{
		//後ろ

		if (_leg_state == 7 || _leg_state == 6 || _leg_state == 5)
		{
			return false;
		}
	}


	//脚位置4と比較して上か下か
	if (_leg_state == 1 || _leg_state == 5)
	{
		//脚位置4と比較して下
		if (_node.leg_base_pos[_leg_num].z - HIGH_MARGIN >= _node.leg_pos[_leg_num].z)
		{
			return true;
		}
	}
	else if (_leg_state == 3 || _leg_state == 7)
	{
		//脚位置4と比較して上
		if (_node.leg_base_pos[_leg_num].z + HIGH_MARGIN <= _node.leg_pos[_leg_num].z)
		{
			return true;
		}
	}
	else
	{
		//脚位置4と同じくらい
		if (std::abs(_node.leg_base_pos[_leg_num].z - _node.leg_pos[_leg_num].z) <= HIGH_MARGIN)
		{
			return true;
		}
	}

	return false;
}
