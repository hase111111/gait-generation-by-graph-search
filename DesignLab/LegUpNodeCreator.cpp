#include "LegUpNodeCreator.h"
#include "com_type.h"
#include "LegState.h"


void LegUpNodeCreator::create(const SNode& _current_node, const int _current_num, std::vector<SNode>* output_graph)
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

	//既に遊脚の脚を接地することはできない．
	{
		std::vector<int> _lifted_leg_num;
		LegStateEdit::getLiftedLegNumWithVector(_current_node.leg_state, _lifted_leg_num);

		for (auto& i : _lifted_leg_num)
		{
			ComType::checkAbleComTypeFromNotGroundableLeg(i, _is_able_type);
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

				if (_temp_ground[j] == false)
				{
					_res_node.leg_pos[j].x = 160 * HexapodConst::DEFAULT_LEG_ANGLE_COS[j];
					_res_node.leg_pos[j].y = 160 * HexapodConst::DEFAULT_LEG_ANGLE_SIN[j];
					_res_node.leg_pos[j].z = -10;
				}
			}

			//可能な姿勢ならば，子ノードとして追加する．
			if (m_calculator.isAblePause(_res_node) == true)
			{
				(*output_graph).push_back(_res_node);
			}
		}
	}

	//出力されたノードがないならば，そのままのノードを出力する．
	if ((*output_graph).size() == 0)
	{
		SNode _res_node = _current_node;

		_res_node.changeNextNode(_current_num, m_next_move);

		(*output_graph).push_back(_res_node);
	}

}
