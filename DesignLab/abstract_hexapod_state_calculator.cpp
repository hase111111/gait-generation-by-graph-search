#include "abstract_hexapod_state_calculator.h"

#include "cassert_define.h"
#include "leg_state.h"


float AbstractHexapodStateCalculator::CalculateStabilityMargin(const dl_leg::LegStateBit& leg_state, const std::array<designlab::Vector3, HexapodConst::LEG_NUM>& leg_pos) const
{
	// std::min をカッコで囲んでいるのは，マクロの min と被るため．(std::min) と書くと名前が衝突しない

	std::array<designlab::Vector2,HexapodConst::LEG_NUM> ground_leg_pos;	// xy平面に投射した，重心を原点としたローカル(ロボット)座標系で，脚の位置を計算する．
	int ground_leg_pos_num = 0;												// 速度の関係上 vectorでなくarrayを使う．

	//接地脚のみ追加する
	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		if (dl_leg::IsGrounded(leg_state, i))
		{
			ground_leg_pos[ground_leg_pos_num] = leg_pos[i].ProjectedXY() + GetLocalLegBasePosition(i).ProjectedXY();
			ground_leg_pos_num++;
		}
	}


	float min_margin = 0;	// 多角形の辺と重心の距離の最小値
	bool is_first = true;	// 初回かどうか，最初は必ず値を更新する

	for (int i = 0; i < ground_leg_pos_num; i++)
	{
		designlab::Vector2 i_to_i_plus_1 = ground_leg_pos[(i + 1) % ground_leg_pos_num] - ground_leg_pos[i];
		i_to_i_plus_1.Normalize();
		designlab::Vector2 i_to_com = designlab::Vector2{ 0,0 } - ground_leg_pos[i];

		float margin = i_to_com.Cross(i_to_i_plus_1);	// 多角形の辺と重心の距離(静的安定余裕)

		if (is_first) 
		{
			min_margin = margin;
			is_first = false;
		}
		else 
		{
			min_margin = (std::min)(min_margin, margin);
		}
	}

	return min_margin;
}
