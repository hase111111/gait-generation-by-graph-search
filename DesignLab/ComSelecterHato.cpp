#include "ComSelecterHato.h"
#include <algorithm>
#include <iostream>

bool ComSelecterHato::getComFromPolygon(const my_vec::SPolygon2& polygon, const ComType::EComPattern com_pattren, my_vec::SVector& output_com) const
{
	std::vector<std::pair<bool, my_vec::SVector2>> com_candidate;

	//候補点を生成する
	makeComCandidatePoint(polygon, &com_candidate);

	//候補点を現在の重心から最も遠くに移動できる順にソートする．第3引数のきもいのはラムダ式，簡単に言うと関数を関数の中で宣言できるやつ．むずいので理解しなくてよい
	//参考：https://qiita.com/kemkemG0/items/76988e8e62c8a2a9c90a

	//std::sort(com_candidate.begin(), com_candidate.end(),
	//	[&](const my_vec::SVector2& _v1, const my_vec::SVector2& _v2)
	//	{
	//		const my_vec::SVector2 _rotate_center = { -10000,0 };
	//		const float _rotate_r = 10000;
	//		return fabsf((_v1 - _rotate_center).length() - _rotate_r) > fabsf((_v2 - _rotate_center).length() - _rotate_r);
	//	}
	//);

	//候補点を順番にチェックし，移動後の重心が安定余裕を満たすならば，その点を重心として採用する．
	for (auto& i : com_candidate)
	{
		if (!isInMargin(polygon, i.second))
		{
			//候補点が多角形の外側ならば次の候補点へ
			i.first = false;
			continue;
		}

		//現在の重心を移動させたものを作成する
		SNode _temp = m_current_node;
		my_vec::SVector next_com = { i.second.x, i.second.y, m_current_node.global_center_of_mass.z };
		_temp.changeGlobalCenterOfMass(next_com, false);

		if (!m_calclator.isAllLegInRange(_temp))
		{
			//脚が可動範囲外ならば次の候補点へ
			i.first = false;
			continue;
		}

		if (!m_calclator.isAblePause(_temp))
		{
			//姿勢が安定できないならば次の候補点へ
			i.first = false;
			continue;
		}

		//ここまで来たら，脚が可動範囲内で，姿勢も安定できるということなので，その点を重心として採用する
		output_com = next_com;
		return true;
	}


	if (DO_DEBUG_PRINT)
	{
		std::cout << ComType::convertComPatternToBit(com_pattren) << "の重心は見つからなかった" << std::endl;
	}

	//該当するものがなければfalseを返す
	return false;
}

void ComSelecterHato::makeComCandidatePoint(const my_vec::SPolygon2& polygon, std::vector<std::pair<bool, my_vec::SVector2>>* coms) const
{
	//波東さんの処理では多角形を囲むような四角形を作るので，まずはそれを作る
	const float min_x = polygon.getMinX();
	const float max_x = polygon.getMaxX();
	const float min_y = polygon.getMinY();
	const float max_y = polygon.getMaxY();

	const float width = max_x - min_x;
	const float height = max_y - min_y;

	const float delta_width = width / (float)DISCRETIZATION_NUM;
	const float delta_height = height / (float)DISCRETIZATION_NUM;

	//上記の四角形の中にある点を全て候補に追加する．
	(*coms).reserve(DISCRETIZATION_NUM * DISCRETIZATION_NUM);

	for (int x = 0; x < DISCRETIZATION_NUM; ++x)
	{
		for (int y = 0; y < DISCRETIZATION_NUM; ++y)
		{
			(*coms).push_back(std::make_pair(true, my_vec::SVector2{ min_x + delta_width * x, min_y + delta_height * y }));
		}
	}

	if (DO_DEBUG_PRINT) { std::cout << "CandidatePointNum is " << (*coms).size() << std::endl; }
}

bool ComSelecterHato::isInMargin(const my_vec::SPolygon2& polygon, const my_vec::SVector2& candidate_point) const
{
	const int k_vertex_num = polygon.getVertexNum();	//頂点数．何度も使用するので，先に計算しておくことで軽くする．

	for (int j = 0; j < k_vertex_num; ++j)
	{
		my_vec::SVector2 v1 = polygon.getVertex((j + 1) % k_vertex_num) - polygon.getVertex(j);
		v1 = v1.normalized() * -1;

		my_vec::SVector2 v_map = candidate_point - polygon.getVertex(j);

		if (v_map.cross(v1) > -STABILITY_MARGIN)
		{
			//安定余裕を満たさないならば候補から削除する．
			return false;
		}
	}

	return true;
}
