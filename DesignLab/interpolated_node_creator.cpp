#include "interpolated_node_creator.h"

#include "hexapod_const.h"


void InterpolatedNodeCreator::createInterpolatedNode(const SNode& node, const SNode& next_node, std::vector<SNode>* interpolated_node) const
{
	(*interpolated_node).clear();

	dl_vec::SVector dif[HexapodConst::LEG_NUM];

	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		dif[i] = next_node.leg_pos[i] - node.leg_pos[i];
	}


	// 各脚について，現在のノードと次のノードの間を補間する
	for (int i = 0; i < INTERPOLATED_NODE_NUM; i++)
	{
		SNode new_node = node;

		//重心位置を補完する
		new_node.global_center_of_mass = node.global_center_of_mass +
			(next_node.global_center_of_mass - node.global_center_of_mass) * (static_cast<float>(i) + 1.0f) / (static_cast<float>(INTERPOLATED_NODE_NUM) + 1.0f);


		for (int j = 0; j < HexapodConst::LEG_NUM; j++)
		{
			// dif zが0の時は，平行移動のみ
			if (dif[j].z == 0 || dif[j].projectedXY().isZero())
			{
				new_node.leg_pos[j] = node.leg_pos[j] + dif[j] * (static_cast<float>(i) + 1.0f) / (static_cast<float>(INTERPOLATED_NODE_NUM) + 1.0f);
			}
			// dif zが正の時は，脚が上に上がる→平行移動
			else if (dif[j].z > 0)
			{
				if (i < INTERPOLATED_NODE_NUM1)
				{
					// 上昇中
					new_node.leg_pos[j].z = node.leg_pos[j].z + dif[j].z * (i + 1) / (INTERPOLATED_NODE_NUM1 + 1);
				}
				else
				{
					// 平行移動中
					new_node.leg_pos[j].x = node.leg_pos[j].x + dif[j].x * (i - INTERPOLATED_NODE_NUM1 + 1) / (INTERPOLATED_NODE_NUM2 + 1);
					new_node.leg_pos[j].y = node.leg_pos[j].y + dif[j].y * (i - INTERPOLATED_NODE_NUM1 + 1) / (INTERPOLATED_NODE_NUM2 + 1);
					new_node.leg_pos[j].z = node.leg_pos[j].z + dif[j].z;
				}
			}
			// dif zが負の時は，平行移動→脚が下に下がる
			else
			{
				if (i < INTERPOLATED_NODE_NUM1)
				{
					// 平行移動中
					new_node.leg_pos[j].x = node.leg_pos[j].x + dif[j].x * (i + 1) / (INTERPOLATED_NODE_NUM1 + 1);
					new_node.leg_pos[j].y = node.leg_pos[j].y + dif[j].y * (i + 1) / (INTERPOLATED_NODE_NUM1 + 1);
				}
				else
				{
					// 下降中
					new_node.leg_pos[j].x = node.leg_pos[j].x + dif[j].x;
					new_node.leg_pos[j].y = node.leg_pos[j].y + dif[j].y;
					new_node.leg_pos[j].z = node.leg_pos[j].z + dif[j].z * (i - INTERPOLATED_NODE_NUM1 + 1) / (INTERPOLATED_NODE_NUM2 + 1);
				}
			}

		}
		// 結果をpush 
		(*interpolated_node).push_back(new_node);
	}
}
