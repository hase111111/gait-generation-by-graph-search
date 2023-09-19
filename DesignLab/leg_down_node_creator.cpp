//#include "leg_down_node_creator.h"
//
//#include "leg_state.h"
//#include "com_type.h"
//
//
//void LegDownNodeCreator::create(const SNode& current_node, const int current_num, std::vector<SNode>* output_graph)
//{
//	//脚の遊脚・接地によって生じるとりうる重心をcomtypeとして仕分けている．(詳しくはComtype.hを参照)．まずは全てtrueにしておく．
//	boost::dynamic_bitset<> is_able_leg_ground_pattern(dl_com::getLegGroundPatternNum());
//
//	is_able_leg_ground_pattern.set();	//全てtrueにする．
//
//
//	//脚が地面に接地可能か調べる．
//
//	bool is_groundable_leg[HexapodConst::LEG_NUM];		//脚が設置可能ならばtrueになる．既に接地しているならばtrueになる．
//	designlab::Vector3 ground_pos[HexapodConst::LEG_NUM];	//脚が接地する座標．
//
//	for (int i = 0; i < HexapodConst::LEG_NUM; i++) { ground_pos[i] = current_node.leg_pos[i]; }
//
//	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
//	{
//		if (dl_leg::isGrounded(current_node.leg_state, i))
//		{
//			//すでに接地している脚は接地可能に決まっているのでtrueにし，座標をそのまま入れる．
//			is_groundable_leg[i] = true;
//			ground_pos[i] = current_node.leg_pos[i];
//
//			//足を下ろす動作をさせるので，接地脚は遊脚不可能．よって，とれないcom typeを全てけす．
//			dl_com::banLegGroundPatternFromNotFreeLeg(i, &is_able_leg_ground_pattern);
//		}
//		else
//		{
//			//現在遊脚中の脚は現在の脚状態で接地できるか検討する．
//			designlab::Vector3 res_ground_pos;
//
//			if (isGroundableLeg(i, current_node, res_ground_pos))
//			{
//				is_groundable_leg[i] = true;	//接地可能にする．
//				ground_pos[i] = res_ground_pos;
//			}
//			else
//			{
//				is_groundable_leg[i] = false;	//接地不可能にする．
//				dl_com::banLegGroundPatternFromNotGroundableLeg(i, &is_able_leg_ground_pattern);
//			}
//		}
//	}
//
//
//	//子ノードを生成する．
//	for (int i = 0; i < dl_com::getLegGroundPatternNum(); ++i)
//	{
//		//その重心タイプを取ることが可能であれば
//		if (is_able_leg_ground_pattern[i])
//		{
//			SNode res_node = current_node;
//
//			res_node.changeNextNode(current_num, m_next_move);
//
//
//			//遊脚・接地を書き換える．
//			dl_leg::LegGroundedBit new_is_ground = dl_com::getLegGroundedBitFromLegGroundPatternIndex(i);
//
//			dl_leg::changeAllLegGround(new_is_ground, &res_node.leg_state);
//
//
//			for (int j = 0; j < HexapodConst::LEG_NUM; ++j)
//			{
//				if (new_is_ground[j])
//				{
//					res_node.leg_pos[j] = ground_pos[j];
//
//					res_node.leg_base_pos[j] = ground_pos[j];
//				}
//			}
//
//			(*output_graph).push_back(res_node);
//		}
//
//	}
//
//	//出力されたノードがないならば，そのままのノードを出力する．
//	if ((*output_graph).size() == 0)
//	{
//		SNode same_node = current_node;
//
//		same_node.changeNextNode(current_num, m_next_move);
//
//		(*output_graph).emplace_back(same_node);
//	}
//}
//
//
//bool LegDownNodeCreator::isGroundableLeg(const int _leg_num, const SNode& _current_node, designlab::Vector3& _output_ground_pos)
//{
//	//for文の中のcontinueについては http://www9.plala.or.jp/sgwr-t/c/sec06-7.html を参照．ちなみに読みづらくなるので本当は使わないほうがいい．
//
//	using designlab::Vector3;
//
//	if (mp_map == nullptr) { return false; }	//マップがないときはfalseを返す．
//
//	//脚座標がdevide mapでどこに当たるか調べて，そのマスの2つ上と2つ下の範囲内を全て探索する．
//	const designlab::Vector3 kGlobalLegBasePos = m_calculator.getGlobalLegBasePos(_current_node, _leg_num, false);
//
//	int max_x_dev = mp_map->getDevideMapNumX(kGlobalLegBasePos.x) + 1;
//	int min_x_dev = mp_map->getDevideMapNumX(kGlobalLegBasePos.x) - 1;
//	int max_y_dev = mp_map->getDevideMapNumY(kGlobalLegBasePos.y) + 1;
//	int min_y_dev = mp_map->getDevideMapNumY(kGlobalLegBasePos.y) - 1;
//
//	////値がdevide mapの範囲外にあるときは丸める．
//	max_x_dev = (max_x_dev >= MapConst::LP_DIVIDE_NUM) ? MapConst::LP_DIVIDE_NUM - 1 : max_x_dev;
//	min_x_dev = (min_x_dev < 0) ? 0 : min_x_dev;
//	max_y_dev = (max_y_dev >= MapConst::LP_DIVIDE_NUM) ? MapConst::LP_DIVIDE_NUM - 1 : max_y_dev;
//	min_y_dev = (min_y_dev < 0) ? 0 : min_y_dev;
//
//	//devide map内を全探索して，現在の脚位置(離散化した物)に適した脚設置可能点が存在するか調べる．
//
//	std::vector<designlab::Vector3> _candidate_pos;		//現在の脚位置に合致する候補座標群．
//
//	//範囲内の点を全て調べる．
//	for (int x = min_x_dev; x < max_x_dev; x++)
//	{
//		for (int y = min_y_dev; y < max_y_dev; y++)
//		{
//			const int kPosNum = mp_map->getPointNumFromDevideMap(x, y);
//
//			for (int n = 0; n < kPosNum; n++)
//			{
//				Vector3 _pos = mp_map->getPosFromDevideMap(x, y, n);	//脚設置可能点の座標を取り出す．
//				_pos = m_calculator.convertLocalLegPos(_current_node, _pos, _leg_num, false);
//
//				//脚位置を更新したノードを作成する．
//				SNode _new_node = _current_node;
//
//				_new_node.leg_pos[_leg_num] = _pos;
//
//
//				//前の候補地点と比較して，より良い候補地点の時のみ実行すする
//				if (!_candidate_pos.empty())
//				{
//					//反対方向をむいている場合は候補地点として採用しない．
//					if (_new_node.leg_base_pos[_leg_num].projectedXY().cross(_candidate_pos.front().projectedXY()) * _new_node.leg_base_pos[_leg_num].projectedXY().cross(_pos.projectedXY()) < 0)
//					{
//						continue;
//					}
//				}
//
//				dl_leg::changeGround(_leg_num, true, &_new_node.leg_state);
//
//				if (!m_calculator.isLegInRange(_new_node, _leg_num)) { continue; }			//脚が範囲外ならば追加せずに続行．
//
//				if (!isAbleLegPos(_new_node, _leg_num)) { continue; }	//候補座標として，適していないならば追加せずに続行．
//
//				_candidate_pos.push_back(_pos);
//			}
//		}
//	}
//
//
//	//候補点を全列挙したのち，候補点が一つもなければfalse
//	if (_candidate_pos.empty()) { return false; }
//
//	_output_ground_pos = _candidate_pos.back();
//
//	return true;
//}
//
//bool LegDownNodeCreator::isAbleLegPos(const SNode& _node, const int _leg_num)
//{
//	EDiscreteLegPos discrete_leg_pos = dl_leg::getLegState(_node.leg_state, _leg_num);		//脚位置を取得(1～7)
//
//	//まず最初に脚位置4のところにないか確かめる．
//	if ((_node.leg_base_pos[_leg_num] - _node.leg_pos[_leg_num]).lengthSquare() < dl_math::squared(LEG_MARGIN))
//	{
//		if (discrete_leg_pos == EDiscreteLegPos::CENTER) { return true; }
//		else { return false; }
//	}
//	else
//	{
//		if (discrete_leg_pos == EDiscreteLegPos::CENTER) { return false; }
//	}
//
//	//脚位置4と比較して前か後ろか
//	if (_node.leg_base_pos[_leg_num].projectedXY().cross(_node.leg_pos[_leg_num].projectedXY()) * _node.leg_pos[_leg_num].projectedXY().cross({ 1,0 }) > 0)
//	{
//		//前
//
//		if (discrete_leg_pos == EDiscreteLegPos::LOWER_FRONT || discrete_leg_pos == EDiscreteLegPos::FRONT || discrete_leg_pos == EDiscreteLegPos::UPPER_FRONT)
//		{
//			return false;
//		}
//	}
//	else
//	{
//		//後ろ
//
//		if (discrete_leg_pos == EDiscreteLegPos::LOWER_BACK || discrete_leg_pos == EDiscreteLegPos::BACK || discrete_leg_pos == EDiscreteLegPos::UPPER_BACK)
//		{
//			return false;
//		}
//	}
//
//
//	//脚位置4と比較して上か下か
//	if (discrete_leg_pos == EDiscreteLegPos::LOWER_BACK || discrete_leg_pos == EDiscreteLegPos::LOWER_FRONT)
//	{
//		//脚位置4と比較して下
//		if (_node.leg_base_pos[_leg_num].z - HIGH_MARGIN >= _node.leg_pos[_leg_num].z)
//		{
//			return true;
//		}
//	}
//	else if (discrete_leg_pos == EDiscreteLegPos::UPPER_BACK || discrete_leg_pos == EDiscreteLegPos::UPPER_FRONT)
//	{
//		//脚位置4と比較して上
//		if (_node.leg_base_pos[_leg_num].z + HIGH_MARGIN <= _node.leg_pos[_leg_num].z)
//		{
//			return true;
//		}
//	}
//	else
//	{
//		//脚位置4と同じくらい
//		if (std::abs(_node.leg_base_pos[_leg_num].z - _node.leg_pos[_leg_num].z) <= HIGH_MARGIN)
//		{
//			return true;
//		}
//	}
//
//	return false;
//}
