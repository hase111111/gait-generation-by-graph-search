#include "CreateComCandidate.h"
#include "LegState.h"
#include "ComType.h"

CreateComCandidate::CreateComCandidate()
{
}

void CreateComCandidate::getComMovableArea(const SNode _node, std::vector<my_vec::SLegVector>& _res_leg_pos, std::vector<my_vec::SVector>& _res_com_pos, std::vector<int>& _res_state)
{
	//各種変数を初期化する
	init(_node);

	candidatePointNum = getInsidePolygon();	//脚位置による重心タイプ(多角形要素)の数(7)

	for (int i = 0; i < candidatePointNum; i++) 
	{
		bool _is_able_polygon = false;

		switch (phantomX.getTargetMode()) 
		{
		case ETargetMode::STRAIGHT_VECOTR:
		case ETargetMode::STRAIGHT_POSITION:
			_is_able_polygon = isComInPolygon(&IPolygon[i]);
			break;

		case ETargetMode::TURN_ON_SPOT_DIRECTION:
		case ETargetMode::TURN_ON_SPOT_ANGLE:
			break;

		case ETargetMode::TURN_DIRECTION:
		case ETargetMode::TURN_ANGLE:

			//今の支持脚の組み合わせで移動できないポリゴンは消去。
			if (ComType::isAbleCoM(IPolygon[i].COMtype, m_ground_leg) == true && isComInPolygon_circlingTarget(&IPolygon[i]) == true)
			{
				_is_able_polygon = true;
			}

			break;

		default:
			break;
		}

		//それぞれの重心タイプ(多角形要素)から重心移動できる座標を選定し、その中でzが大きくxが小さいもの,重心タイプごとに安定余裕を考えている
		if (_is_able_polygon == true) 
		{
			// 移動後の脚座標を記録する．ローカル座標coxa
			my_vec::SLegVector _temp_leg;
			for (int j = 0; j < HexapodConst::LEG_NUM; j++)
			{
				_temp_leg.leg[j] = IPolygon[i].leg_pos[j];
			}
			_res_leg_pos.push_back(_temp_leg);

			// 重心の移動量を記録する.
			_res_com_pos.push_back(IPolygon[i].COMPoint);
			
			// 脚状態を記録する.
			_res_state.push_back((_node.leg_state ^ (IPolygon[i].COMtype << leg_state::SHIFT_TO_COM_NUM)) & leg_state::COM_STATE_MASKBIT);
			_res_state.back() = _node.leg_state ^ _res_state.back();

			for (int j = 0; j < HexapodConst::LEG_NUM; ++j)
			{
				//全部の脚位置を4に変更
				leg_state::changeLegStateKeepTopBit(_res_state.back(), j, 4);
			}
		}
	}

	return;
}

void CreateComCandidate::initHexapodJustBeforeSearch(const SNode _node, const STarget _t)
{
	phantomX.setMyDirection(_node.pitch, _node.roll, _node.yaw);	//自機の角度
	phantomX.setMyPosition(my_vec::SVector(0, 0, 0));				//自機の重心位置(0,0,0)
	phantomX.setTarget(_t);											//目標
}

void CreateComCandidate::init(const SNode _node)
{
	//重心から脚先の位置の計算(座標,(x,y,z)→(x,z,y)),node->Legはcoxaからの脚先位置
	my_vec::SVector _temp_leg_rot[HexapodConst::LEG_NUM];

	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		_temp_leg_rot[i] = my_vec::VRot(_node.leg_pos[i], my_vec::SVector(0, 0, 0), phantomX.getGlobalMyDirectionthP(), phantomX.getGlobalMyDirectionthR(), phantomX.getGlobalMyDirectionthY());

		//CreateComCandidateの座標(x,y,z)は，PassFindingにおける(x,z,y)となる
		// CCC ← PF
		m_leg_pos[i].x = _temp_leg_rot[i].x + phantomX.getGlobalCoxaJointPos(i).x;
		m_leg_pos[i].y = _temp_leg_rot[i].z + phantomX.getGlobalCoxaJointPos(i).z;
		m_leg_pos[i].z = _temp_leg_rot[i].y + phantomX.getGlobalCoxaJointPos(i).y;
	}

	//重心位置の設定
	m_global_com = _node.global_center_of_mass;

	//CreateComCandidate の変数にコピー．2次階層の状態コピー(例:101010)
	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		m_ground_leg[i] = leg_state::isGrounded(_node.leg_state, i);
	}
}

int CreateComCandidate::getInsidePolygon() 
{
	//legはロボット座標(yzは入れ替え)
	int _polygon_cnt = 0;
	int _node_cnt = 0;
	my_vec::SVector crosspoint;//ロボット座標系

	for (int i = 0; i < HexapodConst::LEG_NUM; ++i)
	{
		//脚位置を入力　ロボット座標系
		INode[i].Point = m_leg_pos[i];		
	}

	_node_cnt = 6;

	//隣りあう２つの脚位置(i,i+1)に対応する4角形or5角形の領域を求める.
	for (int i = 0; i < HexapodConst::LEG_NUM; ++i) 
	{
		//重心タイプの頂点座標を求める
		IPolygon[_polygon_cnt].nOfVertex = 4;
		
		//四角形の対角線の交点を求める
		lineSegmentHitDetection(INode[i].Point, INode[(i + 3) % 6].Point, INode[(i + 1) % 6].Point, INode[(i + 5) % 6].Point, &crosspoint);
		INode[_node_cnt].Point = crosspoint;//交点　ロボット座標系
		IPolygon[_polygon_cnt].VertexNode[0] = &(INode[_node_cnt]);
		_node_cnt++;
		
		lineSegmentHitDetection(INode[i].Point, INode[(i + 2) % 6].Point, INode[(i + 1) % 6].Point, INode[(i + 5) % 6].Point, &crosspoint);
		INode[_node_cnt].Point = crosspoint;
		IPolygon[_polygon_cnt].VertexNode[1] = &(INode[_node_cnt]);
		_node_cnt++;
		
		lineSegmentHitDetection(INode[i].Point, INode[(i + 2) % 6].Point, INode[(i + 1) % 6].Point, INode[(i + 4) % 6].Point, &crosspoint);
		INode[_node_cnt].Point = crosspoint;
		IPolygon[_polygon_cnt].VertexNode[2] = &(INode[_node_cnt]);
		_node_cnt++;

		lineSegmentHitDetection(INode[i].Point, INode[(i + 3) % 6].Point, INode[(i + 1) % 6].Point, INode[(i + 4) % 6].Point, &crosspoint);
		INode[_node_cnt].Point = crosspoint;
		IPolygon[_polygon_cnt].VertexNode[3] = &(INode[_node_cnt]);
		_node_cnt++;

		if (lineSegmentHitDetection(INode[i].Point, IPolygon[_polygon_cnt].VertexNode[3]->Point, INode[(i + 2) % 6].Point, INode[(i + 5) % 6].Point, &crosspoint) == true) 
		{
			//5角形ならば//線分が交わるとき5角形
			IPolygon[_polygon_cnt].nOfVertex = 5;
			//_node_cnt--;

			lineSegmentHitDetection(INode[i].Point, INode[(i + 3) % 6].Point, INode[(i + 2) % 6].Point, INode[(i + 5) % 6].Point, &crosspoint);
			INode[_node_cnt].Point = crosspoint;
			IPolygon[_polygon_cnt].VertexNode[4] = &(INode[_node_cnt]);
			_node_cnt++;

			lineSegmentHitDetection(INode[i + 1].Point, INode[(i + 4) % 6].Point, INode[(i + 2) % 6].Point, INode[(i + 5) % 6].Point, &crosspoint);
			INode[_node_cnt].Point = crosspoint;
			IPolygon[_polygon_cnt].VertexNode[3] = &(INode[_node_cnt]);
			_node_cnt++;
		}

		//ポリゴンの重心タイプを代入 (1 ～ 6が出力される)
		IPolygon[_polygon_cnt].COMtype = i + 1;

		_polygon_cnt++;
	}

	//中心の3角形の領域を求める
	IPolygon[_polygon_cnt].nOfVertex = 3;

	lineSegmentHitDetection(INode[0].Point, INode[3].Point, INode[1].Point, INode[4].Point, &crosspoint);
	INode[_node_cnt].Point = crosspoint;
	IPolygon[_polygon_cnt].VertexNode[0] = &(INode[_node_cnt]);
	_node_cnt++;

	lineSegmentHitDetection(INode[0].Point, INode[3].Point, INode[2].Point, INode[5].Point, &crosspoint);
	INode[_node_cnt].Point = crosspoint;
	IPolygon[_polygon_cnt].VertexNode[1] = &(INode[_node_cnt]);
	_node_cnt++;

	lineSegmentHitDetection(INode[1].Point, INode[4].Point, INode[2].Point, INode[5].Point, &crosspoint);
	INode[_node_cnt].Point = crosspoint;
	IPolygon[_polygon_cnt].VertexNode[2] = &(INode[_node_cnt]);
	_node_cnt++;

	//ポリゴンの重心タイプを代入 leg[2]から交点の距離で判定
	if (INode[2].Point.distanceFrom(INode[_node_cnt - 1].Point) > INode[2].Point.distanceFrom(INode[_node_cnt - 2].Point))
	{
		//逆三角形
		IPolygon[_polygon_cnt].COMtype = 7;
	}
	else 
	{
		IPolygon[_polygon_cnt].COMtype = 8;
	}

	_polygon_cnt++;

	return _polygon_cnt;
};


//線分と線分のあたり判定．直線の交点となっている可能性あり,xy平面での交点
// s1 :	線分1の始点 グローバル
// e1 :	線分1の終点　グローバル
// s2 :	線分2の始点グローバル
// e2 :	線分2の終点グローバル
// crossPoint:	交点の座標
bool CreateComCandidate::lineSegmentHitDetection(my_vec::SVector s1, my_vec::SVector e1, my_vec::SVector s2, my_vec::SVector e2, my_vec::SVector *crossPoint) 
{
	my_vec::SVector v, v1, v2;
	float t1, t2;
	v1 = e1 - s1;
	v2 = e2 - s2;
	v  = s1 - s2;

	//2つのベクトルが平行//外積が0なら平行,xyz空間で並行でなくてもxy平面で平行な可能性あり
	if ((v1.cross(v2)).lengthSquare() < 0.01)
	{
		//ベクトルの外積の大きさの2乗
		return false;	//2つのアークの始点，終点のどれも一致していない
	}

	//t1=s1s2e2の頂点からなる三角形の面積の2倍/s1s2e1e2の頂点からなる四角形の面積の2倍→四角形の面積における三角形の面積の割合→s1から交点の距離/s1からe2の距離
	t1 = -VCrossXZ(v, v2) / VCrossXZ(v1, v2);
	t2 = -VCrossXZ(v, v1) / VCrossXZ(v1, v2);
	*crossPoint = s1 + v1 * t1;

	//線分同士が交差しているならば
	if ( ContactJudgment(t1, t2) ) 
	{		
		// isEqualVectorの許容誤差の値が 1である理由は分からない．もともとマジックナンバーだった．

		if (my_vec::isEqualVector(s1, s2, 1.0) || my_vec::isEqualVector(s1, e2, 1.0) || my_vec::isEqualVector(s2, e1, 1.0) || my_vec::isEqualVector(e1, e2, 1.0))
		{
			return false;//ポリゴンの頂点が一致しているとき
		}

		return true;
	}

	return false;
}

//水平要素のベクトルの外積→ベクトルで作る平行四辺形を水平面に射影した面積
float CreateComCandidate::VCrossXZ(my_vec::SVector In1, my_vec::SVector In2)
{
	return In1.x * In2.z - In1.z * In2.x;
}

bool CreateComCandidate::ContactJudgment(float t1, float t2) 
{
	const float _err = 0.01f;

	return (-_err <= t1 && t1 < 1.0f + _err && -_err <= t2 && t2 < 1.0f + _err);
}



bool CreateComCandidate::isComInPolygon(IntersectionPolygon* polygon) 
{
	//comCandidate[x][z]
	my_vec::SVector comCandidatePoint[DIVIDE_NUM][DIVIDE_NUM];

	//無効な点は0，有効な点は1
	int adleComCandidate[DIVIDE_NUM][DIVIDE_NUM];

	float xMax, xMin, zMax, zMin, dx, dz;
	//多角形頂点の最大・最少座標を探す,グローバル座標
	xMax = polygon->VertexNode[0]->Point.x;
	xMin = polygon->VertexNode[0]->Point.x;

	zMax = polygon->VertexNode[0]->Point.z;
	zMin = polygon->VertexNode[0]->Point.z;


	for (int i = 1; i < polygon->nOfVertex; i++) 
	{
		if (xMax < polygon->VertexNode[i]->Point.x) 
		{
			xMax = polygon->VertexNode[i]->Point.x;
		}
		else if (xMin > polygon->VertexNode[i]->Point.x) 
		{
			xMin = polygon->VertexNode[i]->Point.x;
		}

		if (zMax < polygon->VertexNode[i]->Point.z) 
		{
			zMax = polygon->VertexNode[i]->Point.z;
		}
		else if (zMin > polygon->VertexNode[i]->Point.z) 
		{
			zMin = polygon->VertexNode[i]->Point.z;
		}
	}

	//DIVIDE_NUM=10等分する
	dx = (xMax - xMin) / float(DIVIDE_NUM);
	dz = (zMax - zMin) / float(DIVIDE_NUM);

	for (int ix = 0; ix< DIVIDE_NUM; ix++) 
	{
		for (int iz = 0; iz< DIVIDE_NUM; iz++) 
		{
			adleComCandidate[ix][iz] = 1;//すべて有効
			comCandidatePoint[ix][iz] = my_vec::SVector(xMin + dx * ix, 0, zMin + dz * iz);//DIVIDE_NUM×DIVIDE_NUMに分割した重心位置候補の座標 190419
		}
	}

	//ポリゴンマージン外の点を無効にする
	my_vec::SVector v1, v1n, vMap;

	//頂点の数だけループ
	for (int i = 0; i < polygon->nOfVertex; i++) 
	{
		v1 = polygon->VertexNode[(i + 1) % polygon->nOfVertex]->Point - polygon->VertexNode[i]->Point;	//多角形の辺
		v1n = v1.normalized();

		for (int ix = 0; ix< DIVIDE_NUM; ix++) 
		{
			for (int iz = 0; iz< DIVIDE_NUM; iz++) 
			{
				//map総当たり
				vMap = comCandidatePoint[ix][iz] - polygon->VertexNode[i]->Point - COMPOSI;		//頂点からの重心のベクトル,composiがy方向にずれたらzとxを変換する必要があり？

				//ポリゴンのマージン外にある場合adleComCandidateを0//辺の単位ベクトル(=1)とvMapからなる面積=辺から重心への垂線の長さ>マージン10.0f
				if (vMap.cross(v1n).y > -m_stability_margin)
				{
					adleComCandidate[ix][iz] = 0;//無効
				}
			}
		}
	}

	my_vec::SVector L_legposi[6];
	my_vec::SVector G_legposi[6];

	for (int ileg = 0; ileg < 6; ileg++) 
	{
		G_legposi[ileg] = phantomX.getGlobalCoxaJointPos(ileg);
	}
	
	//iz(y方向)の大きくx方向の小さいところから探す、見つけたら終了
	for (int iz = DIVIDE_NUM - 1; iz >= 0; iz--) 
	{
		for (int ix = 0; ix< DIVIDE_NUM; ix++) 
		{
			//map総当たり
			if (adleComCandidate[ix][iz] == 0) { continue; }

			for (int ileg = 0; ileg < 6; ileg++) 
			{
				//脚座標//(x,y,z)→(x,y,z)脚の付け根からの座標
				L_legposi[ileg].x = m_leg_pos[ileg].z - comCandidatePoint[ix][iz].z - G_legposi[ileg].y;
				L_legposi[ileg].y = m_leg_pos[ileg].x - comCandidatePoint[ix][iz].x - G_legposi[ileg].x;
				L_legposi[ileg].z = -(m_leg_pos[ileg].y - comCandidatePoint[ix][iz].y - G_legposi[ileg].z);
			}

			//取ることがでる体勢かつ転倒しない体勢
			//脚の可動範囲内かどうか							脚がクロスしないかどうか							
			if (phantomX.isAblePause(L_legposi, m_ground_leg) && phantomX.isAbleCOM(L_legposi, m_ground_leg)) 
			{
				polygon->COMPoint.x = comCandidatePoint[ix][iz].x;
				polygon->COMPoint.y = comCandidatePoint[ix][iz].z;
				polygon->COMPoint.z = comCandidatePoint[ix][iz].y;
				//polygon->COMPoint = comCandidatePoint[ix][iz];

				return true;
			}
		}
	}

	return false;
}

bool CreateComCandidate::isComInPolygon_circlingTarget(IntersectionPolygon* polygon) 
{
	my_vec::SVector comCandidatePoint[DIVIDE_NUM][DIVIDE_NUM] = {};	//comCandidate[x][z]
	int adleComCandidate[DIVIDE_NUM][DIVIDE_NUM] = {};					//無効な点は0，有効な点は1

	//多角形頂点の最大・最少座標を探す,ロボット座標
	float xMax, xMin, zMax, zMin, dx, dz;
	xMax = polygon->VertexNode[0]->Point.x;
	xMin = polygon->VertexNode[0]->Point.x;

	zMax = polygon->VertexNode[0]->Point.z;
	zMin = polygon->VertexNode[0]->Point.z;

	for (int i = 1; i < polygon->nOfVertex; i++) 
	{
		if (xMax < polygon->VertexNode[i]->Point.x) 
		{
			xMax = polygon->VertexNode[i]->Point.x;
		}
		else if (xMin > polygon->VertexNode[i]->Point.x) 
		{
			xMin = polygon->VertexNode[i]->Point.x;
		}

		if (zMax < polygon->VertexNode[i]->Point.z)
		{
			zMax = polygon->VertexNode[i]->Point.z;
		}
		else if (zMin > polygon->VertexNode[i]->Point.z) 
		{
			zMin = polygon->VertexNode[i]->Point.z;
		}
	}

	//半径からの距離で除外するやつ
	dx = (xMax - xMin) / float(DIVIDE_NUM);
	dz = (zMax - zMin) / float(DIVIDE_NUM);

	for (int ix = 0; ix < DIVIDE_NUM; ix++)
	{
		for (int iz = 0; iz < DIVIDE_NUM; iz++)
		{
			//平面内の半径からの距離で除外するやつ
			adleComCandidate[ix][iz] = 1;
			comCandidatePoint[ix][iz] = my_vec::SVector(xMin + dx * ix, 0, zMin + dz * iz);	//DIVIDE_NUM×DIVIDE_NUMに分割した重心位置候補の座標
		}
	}

	//ポリゴンマージン外の点を無効にする
	my_vec::SVector v1, v1n, vMap;

	//ポリゴンごとに、安定余裕を計算。
	for (int i = 0; i < polygon->nOfVertex; i++) 
	{
		//頂点の数だけループ
		v1 = polygon->VertexNode[(i + 1) % polygon->nOfVertex]->Point - polygon->VertexNode[i]->Point;	//多角形の辺
		v1n = v1.normalized();
		
		for (int ix = 0; ix< DIVIDE_NUM; ix++) 
		{
			for (int iz = 0; iz< DIVIDE_NUM; iz++) 
			{
				//map総当たり
				vMap = comCandidatePoint[ix][iz] - polygon->VertexNode[i]->Point - COMPOSI;	//頂点からの重心のベクトル,composiが中心以外ならcomposiの位置を回転する必要ある

				if (vMap.cross(v1n).y > -m_stability_margin)
				{
					//ポリゴンのマージン外にある場合adleComCandidateを0//辺の単位ベクトル(=1)とvMapからなる面積=辺から重心への垂線の長さ>マージン10.0f
					adleComCandidate[ix][iz] = 0;//無効
				}
			}
		}
	}

	my_vec::SVector L_legposi[6];	//hexapodの一部の関数の座標系	胴体前方がx,鉛直下向きにz,右手系でy
									//CCCは　						胴体前方がz,鉛直上向きにy、なぜが左手系でx　本当にやめていただきたい
	my_vec::SVector L_coxa[6];	//PFの座標系　					胴体前方がy,鉛直上向きにz,右手系でx

	for (int ileg = 0; ileg < 6; ileg++) 
	{
		L_coxa[ileg] = phantomX.getLocalCoxaJointPos(ileg);
	}

	float thY = phantomX.getGlobalMyDirectionthY();

	//重心をうごかしたときに、脚が可動範囲外にでない、かつ、脚同士がぶつからないかの判定。
	for (int iz = DIVIDE_NUM - 1; iz >= 0; iz--) 
	{
		for (int ix = 0; ix< DIVIDE_NUM; ix++) 
		{
			//map総当たり
			if (adleComCandidate[ix][iz] == 0) { continue; }

			for (int ileg = 0; ileg < 6; ileg++) 
			{
				//重心からの座標(x,y,z)→(x,y,z)脚の付け根からの座標
				if (!m_ground_leg[ileg]) 
				{
					//遊脚は胴体と一緒に移動
					L_legposi[ileg].x = (m_leg_pos[ileg].z)*cos(thY) - (m_leg_pos[ileg].x)*sin(thY) - L_coxa[ileg].y;
					L_legposi[ileg].y = (m_leg_pos[ileg].z)*sin(thY) + (m_leg_pos[ileg].x)*cos(thY) - L_coxa[ileg].x;

				} else {
					L_legposi[ileg].x = (m_leg_pos[ileg].z - comCandidatePoint[ix][iz].z)*cos(thY) - (m_leg_pos[ileg].x - comCandidatePoint[ix][iz].x)*sin(thY) - L_coxa[ileg].y;
					L_legposi[ileg].y = (m_leg_pos[ileg].z - comCandidatePoint[ix][iz].z)*sin(thY) + (m_leg_pos[ileg].x - comCandidatePoint[ix][iz].x)*cos(thY) - L_coxa[ileg].x;
					
				}

				//z方向の重心移動は行わない
				L_legposi[ileg].z = -m_leg_pos[ileg].y;					
			}

			//取ることができない体勢もしくは，転倒する体勢．脚の可動範囲内かどうかor脚がクロスしないかどうか							
			if (!phantomX.isAblePause(L_legposi, m_ground_leg) || !phantomX.isAbleCOM(L_legposi, m_ground_leg)) 
			{
				adleComCandidate[ix][iz] = 0;
			}
		}
	}

	//suneと脚設置可能点の接触判定．干渉する重心候補点は除外する。

	//かつてはここにSPLPの処理があったが，使用されていなかったので削除した．

	float distance = ALLOW_RADIUS_DIFF, distanceA;
	my_vec::SVector CtoG_can;
	float CtoG_can_radius;
	int x_best=-1, z_best=-1;
	//選択可能な残りの重心候補点に対して、
	//旋回半径といちばん近い場所を重心移動位置とする
	for (int iz = DIVIDE_NUM - 1; iz >= 0; iz--) 
	{
		for (int ix = 0; ix< DIVIDE_NUM; ix++) 
		{
			if (adleComCandidate[ix][iz] == 1) 
			{
				CtoG_can = m_global_com + comCandidatePoint[ix][iz] - phantomX.getRotaionCenter();
				CtoG_can_radius = sqrt(CtoG_can.x*CtoG_can.x + CtoG_can.y*CtoG_can.y);
				distanceA = fabs(CtoG_can_radius - phantomX.getTurningRadius());

				if (distanceA < distance) 
				{
					distance = distanceA;
					x_best = ix;
					z_best = iz;
				}
			}
		}
	}
	
	if (x_best < 0 || z_best < 0) { return false; }

	for (int ileg = 0; ileg < 6; ileg++) 
	{
		//重心からの座標(x,y,z)→(x,y,z)脚の付け根からの座標
		if (!m_ground_leg[ileg]) 
		{
			//遊脚
			polygon->leg_pos[ileg].y = (m_leg_pos[ileg].z)*cos(thY) - (m_leg_pos[ileg].x)*sin(thY) - L_coxa[ileg].y;
			polygon->leg_pos[ileg].x = (m_leg_pos[ileg].z)*sin(thY) + (m_leg_pos[ileg].x)*cos(thY) - L_coxa[ileg].x;
		} 
		else 
		{
			polygon->leg_pos[ileg].y = (m_leg_pos[ileg].z - comCandidatePoint[x_best][z_best].z)*cos(thY) - (m_leg_pos[ileg].x - comCandidatePoint[x_best][z_best].x)*sin(thY) - L_coxa[ileg].y;
			polygon->leg_pos[ileg].x = (m_leg_pos[ileg].z - comCandidatePoint[x_best][z_best].z)*sin(thY) + (m_leg_pos[ileg].x - comCandidatePoint[x_best][z_best].x)*cos(thY) - L_coxa[ileg].x;
		}

		//z方向の重心移動は行わない
		polygon->leg_pos[ileg].z = m_leg_pos[ileg].y;					
	}

	if (distance < ALLOW_RADIUS_DIFF) 
	{
		polygon->COMPoint.x = comCandidatePoint[x_best][z_best].x;
		polygon->COMPoint.y = comCandidatePoint[x_best][z_best].z;
		polygon->COMPoint.z = comCandidatePoint[x_best][z_best].y;
		return true;
	}

	return false;
}