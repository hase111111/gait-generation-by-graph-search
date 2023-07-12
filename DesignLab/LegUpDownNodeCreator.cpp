#include "LegUpDownNodeCreator.h"
#include "NodeEdit.h"
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

	bool _is_groundable[HexapodConst::LEG_NUM];				//脚が設置可能ならばtrueになる．既に接地しているならばtrueになる．
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
			node_edit::changeNextNode(_res_node, _current_num, getNextMove(_current_node.next_move));

			//遊脚・接地を書き換える．
			bool _temp_ground[HexapodConst::LEG_NUM] = {};
			ComType::getGroundLegFromComType(i, _temp_ground);

			for (int l = 0; l < HexapodConst::LEG_NUM; l++)
			{
				LegStateEdit::changeGround(_res_node.leg_state, l, _temp_ground[l]);

				_res_node.leg_pos[l] = _ground_pos[l];

				if (_temp_ground[l] == false) { _res_node.leg_pos[l].z = -40; }
			}

			_output_graph.push_back(_res_node);
		}

	}
}

EHexapodMove LegUpDownNodeCreator::getNextMove(const EHexapodMove& _last_move) const
{
	//重心の上下移動→これ→重心の平行移動
	//脚の平行移動→これ→重心の上下移動
	return EHexapodMove::LEG_HIERARCHY_CHANGE;

	if (_last_move == EHexapodMove::COM_UP_DOWN) { return EHexapodMove::COM_MOVE; }
	else { return EHexapodMove::COM_UP_DOWN; }
}

bool LegUpDownNodeCreator::isGroundableLeg(const int _leg_num, const SNode& _current_node, my_vec::SVector& _output_ground_pos)
{
	if (mp_Map == nullptr) { return false; }

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

	std::vector<my_vec::SVector> _candidate_pos;		//現在の脚位置に合致する候補座標群．
	const my_vec::SVector _leg_pos = m_Calc.getGlobalLeg2Pos(_current_node, _leg_num);		//離散化した時の4の座標をあらかじめ計算しておく
	const my_vec::SVector _coxa_pos = m_Calc.getGlobalCoxaJointPos(_current_node, _leg_num);	//脚の付け根の座標．
	const int _leg_state = LegStateEdit::getLegState(_current_node.leg_state, _leg_num);			//脚位置を取得(1～7)

	//範囲内の点を全て調べる．
	for (int x = _min_x_dev; x < _max_x_dev; x++)
	{
		for (int y = _min_y_dev; y < _max_y_dev; y++)
		{
			const int _pos_num = mp_Map->getPointNumFromDevideMap(x, y);

			for (int n = 0; n < _pos_num; n++)
			{
				//脚設置可能点の座標を取り出す．
				my_vec::SVector _pos = mp_Map->getPosFromDevideMap(x, y, n);

				float _len = (_coxa_pos - _pos).length();	//付け根から脚設置可能点までの長さを取得する．

				//最小より近い，または，最大より遠いならば，追加せずに続行．continueについては http://www9.plala.or.jp/sgwr-t/c/sec06-7.html ちなみに読みづらくなるので本当は使わないほうがいいです．
				if (_len < m_Calc.getMinLegR(_coxa_pos.z - _pos.z) || m_Calc.getMaxLegR(_coxa_pos.z - _pos.z) < _len)
				{
					continue;
				}

				//候補座標として，適していないならば追加せずに続行．
				if (isAbleLegPos(_leg_pos, _pos, _coxa_pos, _leg_state) == false)
				{
					continue;
				}

				_candidate_pos.push_back(_pos);
			}
		}
	}


	//候補点を全列挙したのち，候補点が一つもなければfalse
	if (_candidate_pos.size() == 0) { return false; }

	_output_ground_pos = m_Calc.getLocalLegPos(_current_node, _candidate_pos.front(), _leg_num);

	//存在するなら，その中で最も適したものを結果として返し，true
	return true;
}

bool LegUpDownNodeCreator::isAbleLegPos(const my_vec::SVector& _4pos, const my_vec::SVector& _candiatepos, const my_vec::SVector& _coxapos, const int _leg_state)
{
	//Leg2と比較して，どこにあるかによって以下のように離散化している．
	switch (_leg_state)
	{
	case 1:
		//後ろ，下

		//十分近いならば，離散化した脚位置 4 の場所なので適さない．false
		if (LEG_MARGIN > (_4pos - _candiatepos).length()) { return false; }

		//離散化した脚位置 4 より下にあるか確かめる．
		if (_4pos.z - HIGH_MARGIN > _candiatepos.z)
		{
			//ベクトルの外積a×bはa→bと回転する右ねじの上方向になる．脚位置 4をa，候補点をbとしたときに，下向きになるならば後ろにあるはず．
			if ((_4pos - _coxapos).cross(_candiatepos - _coxapos).z < 0)
			{
				return true;
			}
		}

		break;

	case 2:
		//後ろ，中

		//十分近いならば，離散化した脚位置 4 の場所なので適さない．false
		if (LEG_MARGIN > (_4pos - _candiatepos).length()) { return false; }

		//離散化した脚位置 4 と同じ高さにあるか確かめる．
		if (_4pos.z - HIGH_MARGIN < _candiatepos.z && _candiatepos.z < _4pos.z + HIGH_MARGIN)
		{
			//ベクトルの外積a×bはa→bと回転する右ねじの上方向になる．脚位置 4をa，候補点をbとしたときに，下向きになるならば後ろにあるはず．
			if ((_4pos - _coxapos).cross(_candiatepos - _coxapos).z < 0)
			{
				return true;
			}
		}

		break;

	case 3:
		//後ろ，上

		//十分近いならば，離散化した脚位置 4 の場所なので適さない．false
		if (LEG_MARGIN > (_4pos - _candiatepos).length()) { return false; }

		//離散化した脚位置 4 より上にあるか確かめる．
		if (_4pos.z + HIGH_MARGIN < _candiatepos.z)
		{
			//ベクトルの外積a×bはa→bと回転する右ねじの上方向になる．脚位置 4をa，候補点をbとしたときに，下向きになるならば後ろにあるはず．
			if ((_4pos - _coxapos).cross(_candiatepos - _coxapos).z < 0)
			{
				return true;
			}
		}

		break;

	case 4:
		//真ん中

		//十分近いならば，追加する．
		if (LEG_MARGIN > (_4pos - _candiatepos).length()) { return true; }

		break;

	case 5:
		//　前，下

		//十分近いならば，離散化した脚位置 4 の場所なので適さない．false
		if (LEG_MARGIN > (_4pos - _candiatepos).length()) { return false; }

		//離散化した脚位置 4 より下にあるか確かめる．
		if (_4pos.z - HIGH_MARGIN > _candiatepos.z)
		{
			//ベクトルの外積a×bはa→bと回転する右ねじの上方向になる．脚位置 4をa，候補点をbとしたときに，上向きになるならば前にあるはず．
			if ((_4pos - _coxapos).cross(_candiatepos - _coxapos).z < 0)
			{
				return true;
			}
		}

		break;

	case 6:
		//　前，中

		//十分近いならば，離散化した脚位置 4 の場所なので適さない．false
		if (LEG_MARGIN > (_4pos - _candiatepos).length()) { return false; }

		//離散化した脚位置 4 と同じ高さにあるか確かめる．
		if (_4pos.z - HIGH_MARGIN < _candiatepos.z && _candiatepos.z < _4pos.z + HIGH_MARGIN)
		{
			//ベクトルの外積a×bはa→bと回転する右ねじの上方向になる．脚位置 4をa，候補点をbとしたときに，上向きになるならば前にあるはず．
			if ((_4pos - _coxapos).cross(_candiatepos - _coxapos).z < 0)
			{
				return true;
			}
		}

		break;

	case 7:
		//　前，上

		//十分近いならば，離散化した脚位置 4 の場所なので適さない．false
		if (LEG_MARGIN > (_4pos - _candiatepos).length()) { return false; }

		//離散化した脚位置 4 より上にあるか確かめる．
		if (_4pos.z + HIGH_MARGIN < _candiatepos.z)
		{
			//ベクトルの外積a×bはa→bと回転する右ねじの上方向になる．脚位置 4をa，候補点をbとしたときに，上向きになるならば前にあるはず．
			if ((_4pos - _coxapos).cross(_candiatepos - _coxapos).z < 0)
			{
				return true;
			}
		}

		break;

	default:
		//該当しないならばfalse
		return false;
		break;

	}

	return false;
}
