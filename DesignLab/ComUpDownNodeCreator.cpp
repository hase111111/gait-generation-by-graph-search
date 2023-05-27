#include "ComUpDownNodeCreator.h"
#include "HexapodConst.h"
#include "HexapodStateCalculator.h"
#include <cfloat>
#include <algorithm>
#include "MyMath.h"
#include "LegState.h"

void ComUpDownNodeCreator::create(const SNode& _current_node, const int _current_num, std::vector<SNode>& _output_graph)
{
	//重心を最も高くあげることのできる位置と，最も低く下げることのできる位置を求める．グローバル座標で Zの位置．
	float _highest_body_zpos = FLT_MAX;
	float _lowest_pos = FLT_MAX;


	//マップを確認して地面の最高点を求め，そこからMAX_RANGE，MIN_RANGEの分だけ離す．




	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		//接地している脚についてのみ考える．
		if (LegState::isGrounded(_current_node.leg_state, i) == true)
		{
			//三平方の定理を使って，脚接地地点から重心位置をどれだけ上げられるか考える．
			const float _c = HexapodConst::FEMUR_LENGTH + HexapodConst::TIBIA_LENGTH - MARGIN;
			const float _b = my_math::squared(_current_node.Leg[i].x) + my_math::squared(_current_node.Leg[i].y) - HexapodConst::COXA_LENGTH;
			const float _a = sqrt(_c * _c - _b * _b);

			//接地脚の最大重心高さの中から一番小さいものを全体の最大重心位置として記録する．_aは脚からどれだけ上げられるかを表しているので，グローバル座標に変更する．
			_highest_body_zpos = std::min(_a + _current_node.global_center_of_mass.z + _current_node.Leg[i].z, _highest_body_zpos);



		}
	}
}
