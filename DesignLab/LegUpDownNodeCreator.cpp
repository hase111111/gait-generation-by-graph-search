#include "LegUpDownNodeCreator.h"
#include "ComType.h"
#include "LegState.h"

void LegUpDownNodeCreator::init(const MapState* const _p_Map)
{
	mp_Map = _p_Map;
}

void LegUpDownNodeCreator::create(const SNode& _current_node, const int _current_num, std::vector<SNode>& _output_graph)
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
			my_vec::SVector _res_ground_pos;

			if (isGroundableLeg(i, _current_node, _res_ground_pos) == true)
			{
				_is_groundable[i] = true;	//接地可能にする．
				_ground_pos[i] = _res_ground_pos;
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
			_res_node.changeNextNode(_current_num, getNextMove(_current_node.next_move));

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
					_res_node.leg_pos[j].x = (HexapodConst::COXA_LENGTH + HexapodConst::FEMUR_LENGTH) * cos(HexapodConst::DEFAULT_LEG_ANGLE[j]);
					_res_node.leg_pos[j].y = (HexapodConst::COXA_LENGTH + HexapodConst::FEMUR_LENGTH) * sin(HexapodConst::DEFAULT_LEG_ANGLE[j]);
					_res_node.leg_pos[j].z = -40;
				}
			}

			_output_graph.push_back(_res_node);
		}

	}

	//出力されるグラフが1つだけならば(自身と同じグラフしか生成されない場合は)グラフを追加しない．
	if (_output_graph.size() == 1) { _output_graph.clear(); }
}

bool LegUpDownNodeCreator::isGroundableLeg(const int _leg_num, const SNode& _current_node, my_vec::SVector& _output_ground_pos)
{
	//for文の中のcontinueについては http://www9.plala.or.jp/sgwr-t/c/sec06-7.html を参照．ちなみに読みづらくなるので本当は使わないほうがいい．

	using my_vec::SVector;

	if (mp_Map == nullptr) { return false; }	//マップがないときはfalseを返す．

	//脚座標がdevide mapでどこに当たるか調べて，そのマスの2つ上と2つ下の範囲内を全て探索する．
	int _max_x_dev = mp_Map->getDevideMapNumX(_current_node.leg_pos[_leg_num].x) + 2;
	int _min_x_dev = mp_Map->getDevideMapNumX(_current_node.leg_pos[_leg_num].x) - 2;
	int _max_y_dev = mp_Map->getDevideMapNumY(_current_node.leg_pos[_leg_num].y) + 2;
	int _min_y_dev = mp_Map->getDevideMapNumY(_current_node.leg_pos[_leg_num].y) - 2;

	//値がdevide mapの範囲外にあるときは丸める．
	_max_x_dev = (_max_x_dev >= MapConst::LP_DIVIDE_NUM) ? MapConst::LP_DIVIDE_NUM - 1 : _max_x_dev;
	_min_x_dev = (_min_x_dev < 0) ? 0 : _min_x_dev;
	_max_y_dev = (_max_y_dev >= MapConst::LP_DIVIDE_NUM) ? MapConst::LP_DIVIDE_NUM - 1 : _max_y_dev;
	_min_y_dev = (_min_y_dev < 0) ? 0 : _min_y_dev;


	//devide map内を全探索して，現在の脚位置(離散化した物)に適した脚設置可能点が存在するか調べる．

	std::vector<SVector> _candidate_pos;		//現在の脚位置に合致する候補座標群．
	const SVector _leg_pos = m_Calc.getGlobalLeg2Pos(_current_node, _leg_num);		//離散化した時の4の座標をあらかじめ計算しておく
	const SVector _coxa_pos = m_Calc.getGlobalCoxaJointPos(_current_node, _leg_num);	//脚の付け根の座標．
	const int _leg_state = LegStateEdit::getLegState(_current_node.leg_state, _leg_num);			//脚位置を取得(1～7)

	//範囲内の点を全て調べる．
	for (int x = _min_x_dev; x < _max_x_dev; x++)
	{
		for (int y = _min_y_dev; y < _max_y_dev; y++)
		{
			const int _pos_num = mp_Map->getPointNumFromDevideMap(x, y);

			for (int n = 0; n < _pos_num; n++)
			{
				SVector _pos = mp_Map->getPosFromDevideMap(x, y, n);	//脚設置可能点の座標を取り出す．

				//脚位置を更新したノードを作成する．
				SNode _new_node = _current_node;

				_new_node.leg_pos[_leg_num] = _pos - _coxa_pos;	//脚設置可能点を脚の付け根に合わせる．

				if (m_Calc.isLegInRange(_new_node, _leg_num) == false) { continue; }			//脚が範囲外ならば追加せずに続行．

				if (m_Calc.isAblePause(_new_node) == false) { continue; }						//転ぶ姿勢ならば追加せずに続行．

				if (m_Calc.isLegInterfering(_new_node) == true) { continue; }					//脚が干渉しているならば追加せずに続行．

				if (isAbleLegPos(_leg_pos, _pos, _coxa_pos, _leg_state) == false) { continue; }	//候補座標として，適していないならば追加せずに続行．


				_candidate_pos.push_back(_pos);
			}
		}
	}


	//候補点を全列挙したのち，候補点が一つもなければfalse
	if (_candidate_pos.size() == 0) { return false; }

	//存在するなら，その中で最も適したものを結果として返し，true
	_output_ground_pos = m_Calc.convertLocalLegPos(_current_node, _candidate_pos.front(), _leg_num);

	return true;
}

bool LegUpDownNodeCreator::isAbleLegPos(const my_vec::SVector& _4pos, const my_vec::SVector& _candiatepos, const my_vec::SVector& _coxapos, const int _leg_state)
{
	//まず最初に脚位置4のところにないか確かめる．
	if ((_4pos - _candiatepos).lengthSquare() < my_math::squared(LEG_MARGIN))
	{
		if (_leg_state == 4) { return true; }
		else { return false; }
	}
	else
	{
		if (_leg_state == 4) { return false; }
	}

	//脚位置4と比較して前か後ろか
	my_vec::SVector2 _front_vec = { 1,0 };	//@todo 回転姿勢を考慮するならば，このベクトルを回転させる必要がある．

	if (_leg_state == 7 || _leg_state == 6 || _leg_state == 5)
	{
		if ((_4pos - _coxapos).projectedXY().cross(_front_vec) * (_candiatepos - _coxapos).projectedXY().cross(_front_vec) > 0) {}
		else { return false; }
	}
	else
	{
		if ((_4pos - _coxapos).projectedXY().cross(_front_vec) * (_candiatepos - _coxapos).projectedXY().cross(_front_vec) < 0) {}
		else { return false; }
	}


	//脚位置4と比較して上か下か
	if (_leg_state == 1 || _leg_state == 5)
	{
		//脚位置4と比較して下
		if (_4pos.z - HIGH_MARGIN >= _candiatepos.z)
		{
			return true;
		}
	}
	else if (_leg_state == 3 || _leg_state == 7)
	{
		//脚位置4と比較して上
		if (_4pos.z + HIGH_MARGIN <= _candiatepos.z)
		{
			return true;
		}
	}
	else
	{
		//脚位置4と同じくらい
		if (std::abs(_4pos.z - _candiatepos.z) <= HIGH_MARGIN)
		{
			return true;
		}
	}

	return false;
}
