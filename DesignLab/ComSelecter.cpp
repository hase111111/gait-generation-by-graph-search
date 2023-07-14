#include "ComSelecter.h"
#include <algorithm>
#include <iostream>

bool ComSelecter::getComFromPolygon(const my_vec::SPolygon2& polygon, const ComType::EComPattern _com_pattren, my_vec::SVector& _output_com) const
{
	std::vector<my_vec::SVector2> _coms;

	//候補点を生成する
	makeComCandidatePoint(polygon, _coms);

	//候補点を現在の重心から最も遠くに移動できる順にソートする．第3引数のきもいのはラムダ式，簡単に言うと関数を関数の中で宣言できるやつ．むずいので理解しなくてよい
	//参考：https://qiita.com/kemkemG0/items/76988e8e62c8a2a9c90a

	std::sort(_coms.begin(), _coms.end(),
		[&](const my_vec::SVector2& _v1, const my_vec::SVector2& _v2)
		{
			return (_v1 - m_current_node.global_center_of_mass.projectedXY()).lengthSquare() > (_v2 - m_current_node.global_center_of_mass.projectedXY()).lengthSquare();
		}
	);

	//候補点を順番にチェックし，移動後の重心が安定余裕を満たすならば，その点を重心として採用する．
	for (const auto& i : _coms)
	{
		//現在の重心を移動させたものを作成する
		SNode _temp = m_current_node;
		my_vec::SVector _next_com = { i.x ,i.y,m_current_node.global_center_of_mass.z };
		_temp.changeGlobalCenterOfMass(_next_com);

		//if (isInMargin(polygon, i) == false) { continue; }	//安定余裕を満たさなければ次の候補点へ

		if (m_calclator.isLegInterfering(_temp) == true) { continue; }	//脚が干渉していれば次の候補点へ

		if (m_calclator.isAllLegInRange(_temp) == false) { continue; }	//脚が可動範囲外ならば次の候補点へ

		if (m_calclator.isAblePause(_temp) == false) { continue; }		//姿勢が安定できないならば次の候補点へ

		//ここまで来たら，安定余裕を満たし，脚が干渉しておらず，脚が可動範囲内で，姿勢も安定できるということなので，その点を重心として採用する
		_output_com = _next_com;
		return true;
	}


	if (DO_DEBUG_PRINT)
	{
		std::cout << ComType::convertComPatternToBit(_com_pattren) << "の重心見つからず" << std::endl;
	}

	//該当するものがなければfalseを返す
	return false;
}

void ComSelecter::makeComCandidatePoint(const my_vec::SPolygon2& polygon, std::vector<my_vec::SVector2>& _coms) const
{
	//波東さんの処理では多角形を囲むような四角形を作るので，まずはそれを作る
	const float _min_x = polygon.getMinX();
	const float _max_x = polygon.getMaxX();
	const float _min_y = polygon.getMinY();
	const float _max_y = polygon.getMaxY();

	const float _width = _max_x - _min_x;
	const float _height = _max_y - _min_y;

	const float _delta_width = _width / (float)DISCRETIZATION_NUM;
	const float _delta_height = _height / (float)DISCRETIZATION_NUM;

	//上記の四角形の中にある点を全て候補に追加する．ただし，多角形の中にある点のみ．
	for (int _x = 0; _x <= DISCRETIZATION_NUM; _x++)
	{
		for (int _y = 0; _y <= DISCRETIZATION_NUM; _y++)
		{
			my_vec::SVector2 _temp(_min_x + _delta_width * _x, _min_y + _delta_height * _y);

			if (polygon.isInside(_temp))
			{
				_coms.push_back(_temp);
			}
		}
	}

	if (DO_DEBUG_PRINT == true) { std::cout << "CandidatePointNum is " << _coms.size() << std::endl; }
}

bool ComSelecter::isInMargin(const my_vec::SPolygon2& polygon, const my_vec::SVector2& _com) const
{
	// @todo 動作がおかしいので修正すること

	const int _vertex_num = polygon.getVertexNum();	//頂点数．何度も使用するので，先に計算しておくことで軽くする．

	for (int j = 0; j < _vertex_num; j++)
	{
		my_vec::SVector2 _v1 = polygon.getVertex((j + 1) % _vertex_num) - polygon.getVertex(j);
		_v1 = _v1.normalized();

		my_vec::SVector2 _v_map = _com - polygon.getVertex(j);

		if (_v_map.cross(_v1) < STABILITY_MARGIN)
		{
			//安定余裕を満たさないならば候補から削除する．
			return false;
		}
	}

	return true;
}
