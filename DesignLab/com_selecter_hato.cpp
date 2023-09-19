#include "com_selecter_hato.h"

#include <algorithm>
#include <iostream>

#include "leg_state.h"


bool ComSelecterHato::getComFromPolygon(const designlab::SPolygon2& polygon, const EDiscreteComPos com_pattren, designlab::Vector3* output_com) const
{
	std::pair<bool, designlab::SVector2> com_candidate[DISCRETIZATION_NUM * DISCRETIZATION_NUM];

	//候補点を生成する
	if (!makeComCandidatePoint(polygon, com_candidate))
	{
		return false;
	}

	//頂点から次の頂点へ向かう辺を正規化したベクトルを作成する
	std::vector<designlab::SVector2> edge_vec;
	edge_vec.resize(polygon.getVertexNum());

	for (int i = 0; i < polygon.getVertexNum(); ++i)
	{
		designlab::SVector2 edge = polygon.getVertex(i) - polygon.getVertex((i + 1) % polygon.getVertexNum());
		edge.normalized();
		edge_vec[i] = edge;
	}

	//候補点を順番にチェックし，移動後の重心が安定余裕を満たすならば，その点を重心として採用する．
	designlab::Vector3 after_move_com;
	designlab::Vector3 after_move_leg_pos[HexapodConst::LEG_NUM];

	for (int i = 0; i < DISCRETIZATION_NUM * DISCRETIZATION_NUM; ++i)
	{
		if (!isInMargin(polygon, edge_vec, com_candidate[i].second))
		{
			//候補点が多角形の外側ならば次の候補点へ
			com_candidate[i].first = false;
			continue;
		}

		//現在の重心を移動させたものを作成する
		after_move_com = { com_candidate[i].second.x, com_candidate[i].second.y, getCurrentNode().global_center_of_mass.z };

		for (int j = 0; j < HexapodConst::LEG_NUM; j++)
		{
			if (dl_leg::isGrounded(getCurrentNode().leg_state, j))
			{
				after_move_leg_pos[j] = getCurrentNode().leg_pos[j] - (after_move_com - getCurrentNode().global_center_of_mass);

				if (!mp_calculator->isLegInRange(j, after_move_leg_pos[j]))
				{
					//脚が可動範囲外ならば次の候補点へ
					com_candidate[i].first = false;
					continue;
				}
			}
		}

		//if (!m_calclator.isAblePause(_temp))
		//{
		//	//姿勢が安定できないならば次の候補点へ
		//	com_candidate[i].first = false;
		//	continue;
		//}
	}

	//候補点の中から現在の重心から最も遠くに移動できるものを選択する

	const designlab::SVector2 k_rotate_center = { -10000,0 };
	const float k_rotate_r = 10000;

	float min_dist = -100000;
	int min_index = -1;

	for (int i = 0; i < DISCRETIZATION_NUM * DISCRETIZATION_NUM; ++i)
	{
		if (com_candidate[i].first)
		{
			const float dist = fabsf((com_candidate[i].second - k_rotate_center).length() - k_rotate_r);

			if (min_dist < dist)
			{
				min_dist = dist;
				min_index = i;
			}
		}
	}

	if (min_index == -1)
	{
		return false;
	}

	//該当するものがなければfalseを返す
	(*output_com).x = com_candidate[min_index].second.x;
	(*output_com).y = com_candidate[min_index].second.y;
	(*output_com).z = getCurrentNode().global_center_of_mass.z;
	return true;
}


bool ComSelecterHato::makeComCandidatePoint(const designlab::SPolygon2& polygon, std::pair<bool, designlab::SVector2> coms[DISCRETIZATION_NUM * DISCRETIZATION_NUM]) const
{
	//波東さんの処理では多角形を囲むような四角形を作るので，まずはそれを作る
	const float kMinX = polygon.getMinX();
	const float kMaxX = polygon.getMaxX();
	const float kMinY = polygon.getMinY();
	const float kMaxY = polygon.getMaxY();

	const float kWidth = kMaxX - kMinX;
	const float kHeight = kMaxY - kMinY;

	if (dl_math::isEqual(kWidth, 0.0f) || dl_math::isEqual(kHeight, 0.0f)) { return false; }

	const float kDeltaWidth = kWidth / (float)DISCRETIZATION_NUM;
	const float kDeltaHeight = kHeight / (float)DISCRETIZATION_NUM;

	//上記の四角形の中にある点を全て候補に追加する．
	for (int x = 0; x < DISCRETIZATION_NUM; ++x)
	{
		for (int y = 0; y < DISCRETIZATION_NUM; ++y)
		{
			coms[x * DISCRETIZATION_NUM + y].first = true;
			coms[x * DISCRETIZATION_NUM + y].second.x = kMinX + kDeltaWidth * x;
			coms[x * DISCRETIZATION_NUM + y].second.y = kMinY + kDeltaHeight * y;
		}
	}

	return true;
}


bool ComSelecterHato::isInMargin(const designlab::SPolygon2& polygon, const std::vector<designlab::SVector2>& edge_vec, const designlab::SVector2& candidate_point) const
{
	for (int i = 0; i < polygon.getVertexNum(); ++i)
	{
		designlab::SVector2 v_map = candidate_point - polygon.getVertex(i);

		if (v_map.cross(edge_vec[i]) > -STABILITY_MARGIN)
		{
			//安定余裕を満たさないならば候補から削除する．
			return false;
		}
	}

	return true;
}
