#include "ComUpDownNodeCreator.h"
#include "HexapodConst.h"
#include "HexapodStateCalculator.h"
#include <cfloat>
#include <algorithm>
#include "MyMath.h"
#include "LegState.h"

void ComUpDownNodeCreator::init(const MapState* const _p_Map)
{
	mp_Map = _p_Map;
}

void ComUpDownNodeCreator::create(const SNode& _current_node, const int _current_num, std::vector<SNode>& _output_graph)
{
	//重心を最も高くあげることのできる位置と，最も低く下げることのできる位置を求める．グローバル座標で Zの位置．
	//マップを確認して地面の最高点を求め，そこからMAX_RANGE，MIN_RANGEの分だけ離す．


	//マップの最大z座標を求める．
	const int _map_x = mp_Map->getDevideMapNumX(_current_node.global_center_of_mass.x);
	const int _map_y = mp_Map->getDevideMapNumY(_current_node.global_center_of_mass.y);
	const float _map_highest_z = mp_Map->getTopZFromDevideMap(_map_x, _map_y);

	//ロボットの重心の最も低く下げることのできるz座標と，高くあげることができるz座標を求める．どちらもグローバル座標．
	float _highest_body_zpos = _map_highest_z + HexapodConst::VERTICAL_MAX_RANGE;
	float _lowest_body_zpos = _map_highest_z + HexapodConst::VERTICAL_MIN_RANGE;


	// 最も高い地点を修正する．
	using my_math::squared;

	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		//接地している脚についてのみ考える．
		if (LegStateEdit::isGrounded(_current_node.leg_state, i) == true)
		{
			//三平方の定理を使って，脚接地地点から重心位置をどれだけ上げられるか考える．
			const float _c = HexapodConst::FEMUR_LENGTH + HexapodConst::TIBIA_LENGTH - MARGIN;
			const float _b = _current_node.leg_pos[i].projectedXY().length() - HexapodConst::COXA_LENGTH;

			const float _a = sqrt(squared(_c) - squared(_b));

			//接地脚の最大重心高さの中から一番小さいものを全体の最大重心位置として記録する．_aは脚からどれだけ上げられるかを表しているので，グローバル座標に変更する．
			_highest_body_zpos = std::min(_a + _current_node.global_center_of_mass.z + _current_node.leg_pos[i].z, _highest_body_zpos);
		}
	}


	//ノードを追加する．
	pushNodeByMaxAndMinPosZ(_current_node, _current_num, _highest_body_zpos, _lowest_body_zpos, _output_graph);
}

void ComUpDownNodeCreator::pushNodeByMaxAndMinPosZ(const SNode& _current_node, const int _current_num, const float _high, const float _low, std::vector<SNode>& _output_graph)
{
	//まずは重心の変化が一切ないものを追加する．
	{
		SNode _same_node = _current_node;
		_same_node.changeNextNode(_current_num, m_next_move);
		_output_graph.push_back(_same_node);
	}


	//重心を変化させたものを追加する．
	{
		//最大と最小の間を分割する．
		const float _div_z = (_high - _low) / (float)DISCRETIZATION;

		//分割した分新しいノードを追加する．
		for (int i = 0; i < DISCRETIZATION + 1; i++)
		{
			SNode _new_node = _current_node;

			const float _dif = (_low + _div_z * i) - _current_node.global_center_of_mass.z;		//現在の重心のz座標と目標のZ座標の差異(difference)を求める．

			_new_node.global_center_of_mass.z += _dif;		//重心位置を更新する．

			//脚位置を更新する．本来脚は移動しないので更新する必要はないのだが，脚位置は脚の付け根を基準位置にしているのであげた分下げないと元の位置にならない．
			for (int l = 0; l < HexapodConst::LEG_NUM; l++)
			{
				//遊脚は胴体と一緒に移動するから，変更しなくて大丈夫．
				if (LegStateEdit::isGrounded(_new_node.leg_state, l) == true)
				{
					_new_node.leg_pos[l].z -= _dif;
				}

				_new_node.Leg2[l].z -= _dif;
			}

			//ノードを追加する．
			_new_node.changeNextNode(_current_num, m_next_move);
			_output_graph.push_back(_new_node);
		}
	}

}
