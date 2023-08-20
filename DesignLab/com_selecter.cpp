#include "com_selecter.h"

#include <algorithm>
#include <iostream>


bool ComSelecter::getComFromPolygon(const dl_vec::SPolygon2& polygon, const ComType::EComPattern com_pattren, dl_vec::SVector& output_com) const
{
	std::vector<dl_vec::SVector2> coms;

	//候補点を生成する
	makeComCandidatePoint(polygon, coms);

	//候補点を現在の重心から最も遠くに移動できる順にソートする．第3引数のきもいのはラムダ式，簡単に言うと関数を関数の中で宣言できるやつ．むずいので理解しなくてよい
	//参考：https://qiita.com/kemkemG0/items/76988e8e62c8a2a9c90a

	std::sort(coms.begin(), coms.end(),
		[&](const dl_vec::SVector2& _v1, const dl_vec::SVector2& _v2)
		{
			return (_v1 - m_current_node.global_center_of_mass.projectedXY()).lengthSquare() > (_v2 - m_current_node.global_center_of_mass.projectedXY()).lengthSquare();
		}
	);

	//候補点を順番にチェックし，移動後の重心が安定余裕を満たすならば，その点を重心として採用する．
	for (const auto& i : coms)
	{
		//現在の重心を移動させたものを作成する
		SNode com_change_node = m_current_node;
		dl_vec::SVector next_com = { i.x ,i.y,m_current_node.global_center_of_mass.z };
		com_change_node.changeGlobalCenterOfMass(next_com, false);

		//if (isInMargin(polygon, i) == false) { continue; }	//安定余裕を満たさなければ次の候補点へ

		if (m_calclator.isLegInterfering(com_change_node)) { continue; }	//脚が干渉していれば次の候補点へ

		if (!m_calclator.isAllLegInRange(com_change_node)) { continue; }	//脚が可動範囲外ならば次の候補点へ

		if (!m_calclator.isAblePause(com_change_node)) { continue; }		//姿勢が安定できないならば次の候補点へ

		//ここまで来たら，安定余裕を満たし，脚が干渉しておらず，脚が可動範囲内で，姿勢も安定できるということなので，その点を重心として採用する
		output_com = next_com;
		return true;
	}


	if (DO_DEBUG_PRINT)
	{
		std::cout << ComType::convertComPatternToBit(com_pattren) << "の重心見つからず" << std::endl;
	}

	//該当するものがなければfalseを返す
	return false;
}


void ComSelecter::makeComCandidatePoint(const dl_vec::SPolygon2& polygon, std::vector<dl_vec::SVector2>& coms) const
{
	//波東さんの処理では多角形を囲むような四角形を作るので，まずはそれを作る
	const float kMinX = polygon.getMinX();
	const float kMaxX = polygon.getMaxX();
	const float kMinY = polygon.getMinY();
	const float kMaxY = polygon.getMaxY();

	const float kWidth = kMaxX - kMinX;
	const float kHeight = kMaxY - kMinY;

	const float kDeltaWidth = kWidth / (float)DISCRETIZATION_NUM;
	const float kDeltaHeight = kHeight / (float)DISCRETIZATION_NUM;

	//上記の四角形の中にある点を全て候補に追加する．ただし，多角形の中にある点のみ．
	for (int x = 0; x <= DISCRETIZATION_NUM; ++x)
	{
		for (int y = 0; y <= DISCRETIZATION_NUM; ++y)
		{
			const dl_vec::SVector2 kComPoint{kMinX + kDeltaWidth * x, kMinY + kDeltaHeight * y};

			if (polygon.isInside(kComPoint))
			{
				coms.push_back(kComPoint);
			}
		}
	}

	if (DO_DEBUG_PRINT) { std::cout << "CandidatePointNum is " << coms.size() << std::endl; }
}


bool ComSelecter::isInMargin(const dl_vec::SPolygon2& polygon, const dl_vec::SVector2& com) const
{
	// @todo 動作がおかしいので修正すること

	const int kVertexNum = polygon.getVertexNum();	//頂点数．何度も使用するので，先に計算しておくことで軽くする．

	for (int j = 0; j < kVertexNum; ++j)
	{
		dl_vec::SVector2 v1 = polygon.getVertex((j + 1) % kVertexNum) - polygon.getVertex(j);
		v1 = v1.normalized();

		dl_vec::SVector2 v_map = com - polygon.getVertex(j);

		if (v_map.cross(v1) < STABILITY_MARGIN)
		{
			//安定余裕を満たさないならば候補から削除する．
			return false;
		}
	}

	return true;
}
