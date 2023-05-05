#include "CreateComCandidate.h"
#include "pch.h"

CreateComCandidate::CreateComCandidate()
{
}

int CreateComCandidate::getMovementPossibilityArea() 
{
	int getCOG = 0;

	candidatePointNum = getInsidePolygon();//脚位置による重心タイプ(多角形要素)の数(7)

	int trueCandidatePointNum = 0;//マージンを満たさないものを除外した重心移動可能点の数

	for (int i = 0; i < candidatePointNum; i++) 
	{
		switch (phantomX.showTargetType()) 
		{
		case ETargetMode::STRAIGHT_VECOTR:
		case ETargetMode::STRAIGHT_POSITION:
			getCOG = getComInPolygon(&IPolygon[i]);
			break;

		case ETargetMode::TURN_ON_SPOT_DIRECTION:
		case ETargetMode::TURN_ON_SPOT_ANGLE:
			break;

		case ETargetMode::TURN_DIRECTION:
		case ETargetMode::TURN_ANGLE:
			getCOG = getComInPolygon_circlingTarget(&IPolygon[i]);
			break;

		default:
			break;
		}

		if (getCOG) 
		{
			//それぞれの重心タイプ(多角形要素)から重心移動できる座標を選定し、その中でzが大きくxが小さいもの,重心タイプごとに安定余裕を考えている
			p_candidatePointType[trueCandidatePointNum] = IPolygon[i].COMtype;//各ポリゴンの番号を代入
			p_candidatePoint[trueCandidatePointNum] = IPolygon[i].COMPoint;//各ポリゴンへの重心の移動量を代入
			
			for (int j = 0; j < 6; j++) 
			{
				p_candidatePointLeg[j][trueCandidatePointNum] = IPolygon[i].Leg[j];//各ポリゴンへ移動後の脚座標を代入(ローカルcoxa)
			}

			trueCandidatePointNum++;
		}
	}

	return trueCandidatePointNum;
}

//平面グラフのアークが作る多角形の要素をすべてを求める関数
int CreateComCandidate::getInsidePolygon() 
{
	//legはロボット座標(yzは入れ替え)
	int countPolygon = 0;
	int xpointcnt = 0;
	myvector::SVector crosspoint;//ロボット座標系

	for (int i = 0; i < LEGCOUNT; ++i) 
	{
		//脚位置を入力　ロボット座標系
		INode[i].Point = m_leg_pos[i];		
		//myvector::VectorOutPut(INode[i].Point);
	}

	xpointcnt = 6;

	//隣りあう２つの脚位置(i,i+1)に対応する4角形or5角形の領域を求める.
	for (int i = 0; i < LEGCOUNT; ++i) 
	{
		//重心タイプの頂点座標を求める
		IPolygon[countPolygon].nOfVertex = 4;
		
		//四角形の対角線の交点を求める
		lineSegmentHitDetection(INode[i].Point, INode[(i + 3) % 6].Point, INode[(i + 1) % 6].Point, INode[(i + 5) % 6].Point, &crosspoint);
		INode[xpointcnt].Point = crosspoint;//交点　ロボット座標系
		IPolygon[countPolygon].VertexNode[0] = &(INode[xpointcnt]);
		xpointcnt++;
		
		lineSegmentHitDetection(INode[i].Point, INode[(i + 2) % 6].Point, INode[(i + 1) % 6].Point, INode[(i + 5) % 6].Point, &crosspoint);
		INode[xpointcnt].Point = crosspoint;
		IPolygon[countPolygon].VertexNode[1] = &(INode[xpointcnt]);
		xpointcnt++;
		
		lineSegmentHitDetection(INode[i].Point, INode[(i + 2) % 6].Point, INode[(i + 1) % 6].Point, INode[(i + 4) % 6].Point, &crosspoint);
		INode[xpointcnt].Point = crosspoint;
		IPolygon[countPolygon].VertexNode[2] = &(INode[xpointcnt]);
		xpointcnt++;

		lineSegmentHitDetection(INode[i].Point, INode[(i + 3) % 6].Point, INode[(i + 1) % 6].Point, INode[(i + 4) % 6].Point, &crosspoint);
		INode[xpointcnt].Point = crosspoint;
		IPolygon[countPolygon].VertexNode[3] = &(INode[xpointcnt]);
		xpointcnt++;

		if (lineSegmentHitDetection(INode[i].Point, IPolygon[countPolygon].VertexNode[3]->Point, INode[(i + 2) % 6].Point, INode[(i + 5) % 6].Point, &crosspoint)) 
		{
			//5角形ならば//線分が交わるとき5角形
			IPolygon[countPolygon].nOfVertex = 5;
			//xpointcnt--;

			lineSegmentHitDetection(INode[i].Point, INode[(i + 3) % 6].Point, INode[(i + 2) % 6].Point, INode[(i + 5) % 6].Point, &crosspoint);
			INode[xpointcnt].Point = crosspoint;
			IPolygon[countPolygon].VertexNode[4] = &(INode[xpointcnt]);
			xpointcnt++;

			lineSegmentHitDetection(INode[i + 1].Point, INode[(i + 4) % 6].Point, INode[(i + 2) % 6].Point, INode[(i + 5) % 6].Point, &crosspoint);
			INode[xpointcnt].Point = crosspoint;
			IPolygon[countPolygon].VertexNode[3] = &(INode[xpointcnt]);
			xpointcnt++;
		}

		//ポリゴンの重心タイプを代入
		IPolygon[countPolygon].COMtype = i + 1;

		countPolygon++;
	}

	//中心の3角形の領域を求める
	IPolygon[countPolygon].nOfVertex = 3;

	lineSegmentHitDetection(INode[0].Point, INode[3].Point, INode[1].Point, INode[4].Point, &crosspoint);
	INode[xpointcnt].Point = crosspoint;
	IPolygon[countPolygon].VertexNode[0] = &(INode[xpointcnt]);
	xpointcnt++;

	lineSegmentHitDetection(INode[0].Point, INode[3].Point, INode[2].Point, INode[5].Point, &crosspoint);
	INode[xpointcnt].Point = crosspoint;
	IPolygon[countPolygon].VertexNode[1] = &(INode[xpointcnt]);
	xpointcnt++;

	lineSegmentHitDetection(INode[1].Point, INode[4].Point, INode[2].Point, INode[5].Point, &crosspoint);
	INode[xpointcnt].Point = crosspoint;
	IPolygon[countPolygon].VertexNode[2] = &(INode[xpointcnt]);
	xpointcnt++;

	//ポリゴンの重心タイプを代入 leg[2]から交点の距離で判定
	if (myvector::VMag2(INode[2].Point, INode[xpointcnt - 1].Point) > myvector::VMag2(INode[2].Point, INode[xpointcnt - 2].Point)) 
	{
		//逆三角形
		IPolygon[countPolygon].COMtype = 7;
	} 
	else 
	{
		IPolygon[countPolygon].COMtype = 8;
	}

	countPolygon++;

	return countPolygon;
};


//線分と線分のあたり判定．直線の交点となっている可能性あり,xy平面での交点
// s1 :	線分1の始点 グローバル
// e1 :	線分1の終点　グローバル
// s2 :	線分2の始点グローバル
// e2 :	線分2の終点グローバル
// crossPoint:	交点の座標
bool CreateComCandidate::lineSegmentHitDetection(myvector::SVector s1, myvector::SVector e1, myvector::SVector s2, myvector::SVector e2, myvector::SVector *crossPoint) 
{
	myvector::SVector v, v1, v2;
	double t1, t2;
	v1 = myvector::VSub(e1, s1);
	v2 = myvector::VSub(e2, s2);
	v = myvector::VSub(s1, s2);

	//2つのベクトルが平行//外積が0なら平行,xyz空間で並行でなくてもxy平面で平行な可能性あり
	if (myvector::VSquareSize(myvector::VCross(v1, v2)) < 0.01) 
	{
		//ベクトルの外積の大きさの2乗
		return false;	//2つのアークの始点，終点のどれも一致していない
	}

	//t1=s1s2e2の頂点からなる三角形の面積の2倍/s1s2e1e2の頂点からなる四角形の面積の2倍→四角形の面積における三角形の面積の割合→s1から交点の距離/s1からe2の距離
	t1 = -VCrossXZ(v, v2) / VCrossXZ(v1, v2);
	t2 = -VCrossXZ(v, v1) / VCrossXZ(v1, v2);
	*crossPoint = myvector::VAdd(s1, myvector::VScale(v1, t1));

	//線分同士が交差しているならば
	if ( ContactJudgment(t1, t2) ) 
	{		
		if ( VectorEqual(s1, s2) || VectorEqual(s1, e2) || VectorEqual(s2, e1) || VectorEqual(e1, e2) ) 
		{
			return false;//ポリゴンの頂点が一致しているとき
		}

		return true;
	}

	return false;
}

//水平要素のベクトルの外積→ベクトルで作る平行四辺形を水平面に射影した面積
double CreateComCandidate::VCrossXZ(myvector::SVector In1, myvector::SVector In2)
{
	return In1.x * In2.z - In1.z * In2.x;
}

bool CreateComCandidate::ContactJudgment(double t1, double t2) 
{
	return (-HITDETECTIONMARGIN <= t1 && t1 < 1.0 + HITDETECTIONMARGIN && -HITDETECTIONMARGIN <= t2 && t2 < 1.0 + HITDETECTIONMARGIN);
}

//1 2つのベクトルの全ての成分が等しい
//0 2つのベクトルが異なる
bool CreateComCandidate::VectorEqual(myvector::SVector v1, myvector::SVector v2) 
{
	return (abs(v1.x - v2.x) + abs(v1.y - v2.y) + abs(v1.z - v2.z)) < 0.01;
}


int CreateComCandidate::getComInPolygon(IntersectionPolygon* polygon) 
{
	//comCandidate[x][z]
	myvector::SVector comCandidatePoint[DIVIDE_NUM][DIVIDE_NUM];

	//無効な点は0，有効な点は1
	int adleComCandidate[DIVIDE_NUM][DIVIDE_NUM];

	double xMax, xMin, zMax, zMin, dx, dz;
	//多角形頂点の最大・最少座標を探す,グローバル座標
	xMax = polygon->VertexNode[0]->Point.x;
	xMin = polygon->VertexNode[0]->Point.x;

	zMax = polygon->VertexNode[0]->Point.z;
	zMin = polygon->VertexNode[0]->Point.z;


	for (int i = 1; i < polygon->nOfVertex; i++) {
		if (xMax < polygon->VertexNode[i]->Point.x) {
			xMax = polygon->VertexNode[i]->Point.x;
		} else if (xMin > polygon->VertexNode[i]->Point.x) {
			xMin = polygon->VertexNode[i]->Point.x;
		}

		if (zMax < polygon->VertexNode[i]->Point.z) {
			zMax = polygon->VertexNode[i]->Point.z;
		} else if (zMin > polygon->VertexNode[i]->Point.z) {
			zMin = polygon->VertexNode[i]->Point.z;
		}
	}
	//DIVIDE_NUM=10等分する
	dx = (xMax - xMin) / double(DIVIDE_NUM);
	dz = (zMax - zMin) / double(DIVIDE_NUM);

	for (int ix = 0; ix< DIVIDE_NUM; ix++) {
		for (int iz = 0; iz< DIVIDE_NUM; iz++) {
			adleComCandidate[ix][iz] = 1;//すべて有効
			comCandidatePoint[ix][iz] = myvector::VGet(xMin + dx * ix, 0, zMin + dz * iz);//DIVIDE_NUM×DIVIDE_NUMに分割した重心位置候補の座標 190419
		}
	}


	//ポリゴンマージン外の点を無効にする
	myvector::SVector v1, v1n, vMap;
	//std::cout<<"polygon->nOfVertex = "<<polygon->nOfVertex<<"\n";
	for (int i = 0; i < polygon->nOfVertex; i++) {//頂点の数だけループ
		v1 = myvector::VSub(polygon->VertexNode[(i + 1) % polygon->nOfVertex]->Point, polygon->VertexNode[i]->Point);//多角形の辺
		v1n = myvector::VUnit(v1);
		for (int ix = 0; ix< DIVIDE_NUM; ix++) {
			for (int iz = 0; iz< DIVIDE_NUM; iz++) {//map総当たり
				vMap = myvector::VSub(myvector::VSub(comCandidatePoint[ix][iz], polygon->VertexNode[i]->Point), COMPOSI);//頂点からの重心のベクトル,composiがy方向にずれたらzとxを変換する必要があり？

				if (myvector::VCross(vMap, v1n).y > -m_stability_margin) {//ポリゴンのマージン外にある場合adleComCandidateを0//辺の単位ベクトル(=1)とvMapからなる面積=辺から重心への垂線の長さ>マージン10.0f
					adleComCandidate[ix][iz] = 0;//無効
				}
			}
		}
	}

	myvector::SVector L_legposi[6];

	myvector::SVector G_legposi[6];
	for (int ileg = 0; ileg < 6; ileg++) 
	{
		G_legposi[ileg] = phantomX.showGlobalCoxaJointPosi(ileg);
	}

	//for(int ix = 0; ix< DIVIDE_NUM; ix++){
	//	for(int iz = 0; iz< DIVIDE_NUM; iz++)
	
	//iz(y方向)の大きくx方向の小さいところから探す、見つけたら終了
	for (int iz = DIVIDE_NUM - 1; iz >= 0; iz--) {
		for (int ix = 0; ix< DIVIDE_NUM; ix++) {//map総当たり
			if (adleComCandidate[ix][iz] == 0)continue;

			for (int ileg = 0; ileg < 6; ileg++) {
				//脚座標//(x,y,z)→(x,y,z)脚の付け根からの座標
				L_legposi[ileg].x = m_leg_pos[ileg].z - comCandidatePoint[ix][iz].z - G_legposi[ileg].y;
				L_legposi[ileg].y = m_leg_pos[ileg].x - comCandidatePoint[ix][iz].x - G_legposi[ileg].x;
				L_legposi[ileg].z = -(m_leg_pos[ileg].y - comCandidatePoint[ix][iz].y - G_legposi[ileg].z);
			}

			//取ることがでる体勢かつ転倒しない体勢
			//脚の可動範囲内かどうか							脚がクロスしないかどうか							
			if (phantomX.isAblePause(L_legposi, groundingLeg) && phantomX.isAbleCOM(L_legposi, groundingLeg)) {
				polygon->COMPoint.x = comCandidatePoint[ix][iz].x;
				polygon->COMPoint.y = comCandidatePoint[ix][iz].z;
				polygon->COMPoint.z = comCandidatePoint[ix][iz].y;
				//polygon->COMPoint = comCandidatePoint[ix][iz];

				return 1;
			}
		}
	}
	return 0;
}

//各ポリゴン内で、代表点を１つ決める
int CreateComCandidate::getComInPolygon_circlingTarget(IntersectionPolygon* polygon) {

	myvector::SVector comCandidatePoint[DIVIDE_NUM][DIVIDE_NUM] = {};//comCandidate[x][z]
	int adleComCandidate[DIVIDE_NUM][DIVIDE_NUM] = {};//無効な点は0，有効な点は1

	double xMax, xMin, zMax, zMin, dx, dz;
	//多角形頂点の最大・最少座標を探す,ロボット座標
	xMax = polygon->VertexNode[0]->Point.x;
	xMin = polygon->VertexNode[0]->Point.x;

	zMax = polygon->VertexNode[0]->Point.z;
	zMin = polygon->VertexNode[0]->Point.z;


	for (int i = 1; i < polygon->nOfVertex; i++) {
		if (xMax < polygon->VertexNode[i]->Point.x) {
			xMax = polygon->VertexNode[i]->Point.x;
		} else if (xMin > polygon->VertexNode[i]->Point.x) {
			xMin = polygon->VertexNode[i]->Point.x;
		}

		if (zMax < polygon->VertexNode[i]->Point.z) {
			zMax = polygon->VertexNode[i]->Point.z;
		} else if (zMin > polygon->VertexNode[i]->Point.z) {
			zMin = polygon->VertexNode[i]->Point.z;
		}
	}
	//------------------------------------半径からの距離で除外やつ--------------------//
	//myvector::VECTOR CtoGcan;// = myvector::VSub(gcom, phantomX.showTurningCenter);//旋回中心から重心へのベクトル
	//myvector::VECTOR Gadd;
	//double CtoGcandist;// = sqrt(Vtemp1.x*Vtemp1.x + Vtemp1.y*Vtemp1.y + Vtemp1.z*Vtemp1.z);	//重心位置と回転中心との距離
	//double radiusDiff;// = fabs(CtoGdist - phantomX.showTurningRadius);	
	//------------------------------------------------------------------------------------//
	//DIVIDE_NUM=10等分する
	dx = (xMax - xMin) / double(DIVIDE_NUM);
	dz = (zMax - zMin) / double(DIVIDE_NUM);


	//std::cout<<"zMax = "<<zMax<<"\n";
	//std::cout<<"zMin = "<<zMin<<"\n";

	for (int ix = 0; ix< DIVIDE_NUM; ix++) {
		for (int iz = 0; iz< DIVIDE_NUM; iz++) {
		//------------------------------------平面内の半径からの距離で除外やつ--------------------//
			//Gadd = myvector::VGet(xMin + dx * ix, zMin + dz * iz, 0);//グローバルにするとzがy　重心の移動量
			//CtoGcan = myvector::VSub(myvector::VAdd(gcom, Gadd), phantomX.showTurningCenter());
			//CtoGcandist = sqrt(CtoGcan.x*CtoGcan.x + CtoGcan.y*CtoGcan.y);//高さ方向は考えない
			//radiusDiff = fabs(CtoGcandist - phantomX.showTurningRadius());
			//	if (radiusDiff < ALLOW_RADIUS_DIFF) {
					adleComCandidate[ix][iz] = 1;
					comCandidatePoint[ix][iz] = myvector::VGet(xMin + dx * ix, 0, zMin + dz * iz);//DIVIDE_NUM×DIVIDE_NUMに分割した重心位置候補の座標
			//	}
		//------------------------------------------------------------------------------------//
		}
	}


	//ポリゴンマージン外の点を無効にする
	myvector::SVector v1, v1n, vMap;
	//std::cout<<"polygon->nOfVertex = "<<polygon->nOfVertex<<"\n";
	//ポリゴンごとに、安定余裕を計算。
	for (int i = 0; i < polygon->nOfVertex; i++) {//頂点の数だけループ
		v1 = myvector::VSub(polygon->VertexNode[(i + 1) % polygon->nOfVertex]->Point, polygon->VertexNode[i]->Point);//多角形の辺
		v1n = myvector::VUnit(v1);
		for (int ix = 0; ix< DIVIDE_NUM; ix++) {
			for (int iz = 0; iz< DIVIDE_NUM; iz++) {//map総当たり
				vMap = myvector::VSub(myvector::VSub(comCandidatePoint[ix][iz], polygon->VertexNode[i]->Point), COMPOSI);//頂点からの重心のベクトル,composiが中心以外ならcomposiの位置を回転する必要ある

				if (myvector::VCross(vMap, v1n).y > -m_stability_margin) {//ポリゴンのマージン外にある場合adleComCandidateを0//辺の単位ベクトル(=1)とvMapからなる面積=辺から重心への垂線の長さ>マージン10.0f
					adleComCandidate[ix][iz] = 0;//無効
				}
			}
		}
	}

	myvector::SVector L_legposi[6];//hexapodの一部の関数の座標系　胴体前方がx,鉛直下向きにz,右手系でy
									//CCCは　					胴体前方がz,鉛直上向きにy、なぜが左手系でx　本当にやめていただきたい
	myvector::SVector L_coxa[6];//PFの座標系　					胴体前方がy,鉛直上向きにz,右手系でx
	for (int ileg = 0; ileg < 6; ileg++) {
		L_coxa[ileg] = phantomX.showLocalCoxaJointPosi(ileg);
	}

	double thY = phantomX.showGlobalMyDirectionthY();
	//重心をうごかしたときに、脚が可動範囲外にでない、かつ、脚同士がぶつからないかの判定。
	for (int iz = DIVIDE_NUM - 1; iz >= 0; iz--) {
		for (int ix = 0; ix< DIVIDE_NUM; ix++) {//map総当たり
			if (adleComCandidate[ix][iz] == 0)continue;

			for (int ileg = 0; ileg < 6; ileg++) {
				//重心からの座標(x,y,z)→(x,y,z)脚の付け根からの座標
				if (/*leg[ileg].y == -80*/!groundingLeg[ileg]) {//遊脚は胴体と一緒に移動
					L_legposi[ileg].x = (m_leg_pos[ileg].z)*cos(thY) - (m_leg_pos[ileg].x)*sin(thY) - L_coxa[ileg].y;
					L_legposi[ileg].y = (m_leg_pos[ileg].z)*sin(thY) + (m_leg_pos[ileg].x)*cos(thY) - L_coxa[ileg].x;

				} else {
					L_legposi[ileg].x = (m_leg_pos[ileg].z - comCandidatePoint[ix][iz].z)*cos(thY) - (m_leg_pos[ileg].x - comCandidatePoint[ix][iz].x)*sin(thY) - L_coxa[ileg].y;
					L_legposi[ileg].y = (m_leg_pos[ileg].z - comCandidatePoint[ix][iz].z)*sin(thY) + (m_leg_pos[ileg].x - comCandidatePoint[ix][iz].x)*cos(thY) - L_coxa[ileg].x;
					
				}
				L_legposi[ileg].z = -m_leg_pos[ileg].y;					//z方向の重心移動は行わない
			}
			//std::cerr << "comできる" << ix << "," << iz << "," << phantomX.isAblePause(L_legposi, groundingLeg) << std::endl;
			//		取ることができない体勢もしくは，転倒する体勢
			//脚の可動範囲内かどうか							脚がクロスしないかどうか							
			if (!phantomX.isAblePause(L_legposi, groundingLeg) || !phantomX.isAbleCOM(L_legposi, groundingLeg)) {
				adleComCandidate[ix][iz] = 0;
			}
		}
	}



	//suneと脚設置可能点の接触判定 
	//干渉する重心候補点は除外する。
#ifdef COLLISION_CHECK_SHIN
	myvector::SVector L_legposi_buf[6];//PFの座標系 股関節から足先へのベクトル
	myvector::SVector com_buf;//PFの座標系　グローバル座標系から見た重心位置
	for (int iz = DIVIDE_NUM - 1; iz >= 0; iz--) {
		for (int ix = 0; ix < DIVIDE_NUM; ix++) {//map総当たり
			if (adleComCandidate[ix][iz] == 0)continue;
			//重心移動後の重心位置を代入
			com_buf = myvector::VAdd(S_P_L_P2.phantomX.showGlobalMyPosition(), myvector::VGet(comCandidatePoint[ix][iz].x, comCandidatePoint[ix][iz].z, comCandidatePoint[ix][iz].y));
			//重心移動後の脚位置を代入
			for (int i = 0; i < 6; ++i) {
				//重心からの座標(x,y,z)→(x,y,z)脚の付け根からの座標
				if (!groundingLeg[i]) {//遊脚は胴体と一緒に移動
					L_legposi_buf[i] = S_P_L_P2.phantomX.showLocalLegPosition(i);
				}
				else {//支持脚は、重心の移動量だけ後退
					L_legposi_buf[i] = myvector::VSub(S_P_L_P2.phantomX.showLocalLegPosition(i), myvector::VGet(comCandidatePoint[ix][iz].x, comCandidatePoint[ix][iz].z, comCandidatePoint[ix][iz].y));;
				}

			}

			S_P_L_P2.phantomX.setMyPosition(com_buf);//重心位置グローバル セット
			S_P_L_P2.phantomX.setMyLegPosition(L_legposi_buf);//脚先位置ローカル　セット
			//すねと地形の衝突判定
			if (S_P_L_P2.Collision_judgment_with_shin()) {
				adleComCandidate[ix][iz] = 0;//
			}
		}
	}
#endif
		//std::cout << "counter = " << counter << std::endl;




	/*bool flag = 0;
	for (int iz = DIVIDE_NUM - 1; iz >= 0; iz--) {
		for (int ix = 0; ix < DIVIDE_NUM; ix++) {//map総当たり
			if (adleComCandidate[ix][iz] == 1) {
				flag = 1;
				std::cerr << "able," << ix << "," << iz << std::endl;
			}
		}
	}
	std::string wait;
	if (flag == 0) {
		std::cerr << "アウト" << std::endl;
	}
	std::cerr << x;
	std::cin >> wait;*/

	//std::cout<<"polygon->COMtype"<<polygon->COMtype<<"\n";

	//phantomX.showTravelingDirection();----
	//double distance = -1, distanceA;    //変更20200619hato　もともとのやつ一番遠いやつにしてない？←これがすべての元凶だった。。。
	//もはや、ALLOW_RADIUS_DIFFいらなくなった。。。。。。
	double distance = ALLOW_RADIUS_DIFF, distanceA;
	myvector::SVector CtoG_can;
	double CtoG_can_radius;
	int x_best=-1, z_best=-1;
	//選択可能な残りの重心候補点に対して、
	//旋回半径といちばん近い場所を重心移動位置とする
	for (int iz = DIVIDE_NUM - 1; iz >= 0; iz--) {
		for (int ix = 0; ix< DIVIDE_NUM; ix++) {
			//std::cout<<adleComCandidate[ix][iz];
			if (adleComCandidate[ix][iz] == 1) {
				//distanceA = pow((phantomX.showTurningCenter().x - comCandidatePoint[ix][iz].x), 2) + pow((phantomX.showTurningCenter().y - comCandidatePoint[ix][iz].z), 2) - phantomX.showTurningRadius() * phantomX.showTurningRadius();
				CtoG_can = myvector::VSub(myvector::VAdd(m_global_com, comCandidatePoint[ix][iz]), phantomX.showTurningCenter());
				CtoG_can_radius = sqrt(CtoG_can.x*CtoG_can.x + CtoG_can.y*CtoG_can.y);
				distanceA = fabs(CtoG_can_radius - phantomX.showTurningRadius());
			//	distanceA = pow((phantomX.showTurningCenter().x - comCandidatePoint[ix][iz].x), 2) + pow((comCandidatePoint[ix][iz].z), 2) - phantomX.showTurningRadius() * phantomX.showTurningRadius();//これはちょっと仮決め20200619
			//	distanceA = sqrt(distanceA);
				//polygon->COMPoint = comCandidatePoint[ix][iz];
				//if (distanceA > distance) {
				if (distanceA < distance) {
					distance = distanceA;
					x_best = ix;
					z_best = iz;
				}
			}
		}
	}
	
	if (x_best < 0 || z_best < 0)return 0;
	for (int ileg = 0; ileg < 6; ileg++) 
	{
		//重心からの座標(x,y,z)→(x,y,z)脚の付け根からの座標
		if (/*leg[ileg].y == -80*/!groundingLeg[ileg]) {//遊脚
			polygon->Leg[ileg].y = (m_leg_pos[ileg].z)*cos(thY) - (m_leg_pos[ileg].x)*sin(thY) - L_coxa[ileg].y;
			polygon->Leg[ileg].x = (m_leg_pos[ileg].z)*sin(thY) + (m_leg_pos[ileg].x)*cos(thY) - L_coxa[ileg].x;
		} else {
			polygon->Leg[ileg].y = (m_leg_pos[ileg].z - comCandidatePoint[x_best][z_best].z)*cos(thY) - (m_leg_pos[ileg].x - comCandidatePoint[x_best][z_best].x)*sin(thY) - L_coxa[ileg].y;
			//polygon->Leg[ileg].y = comCandidatePoint[x][z].z;
			polygon->Leg[ileg].x = (m_leg_pos[ileg].z - comCandidatePoint[x_best][z_best].z)*sin(thY) + (m_leg_pos[ileg].x - comCandidatePoint[x_best][z_best].x)*cos(thY) - L_coxa[ileg].x;
		}
		polygon->Leg[ileg].z = m_leg_pos[ileg].y;					//z方向の重心移動は行わない
	}

	if (distance < ALLOW_RADIUS_DIFF) 
	{
		polygon->COMPoint.x = comCandidatePoint[x_best][z_best].x;
		polygon->COMPoint.y = comCandidatePoint[x_best][z_best].z;
		polygon->COMPoint.z = comCandidatePoint[x_best][z_best].y;
		return 1;
	}

	return 0;
}

//leginはcoxa座標系における脚先位置のベクトル190419
//このクラスではyzの座標が違うことに注意、重心座標は(0,0,0) phantomXクラスに値を入力してから使う
//重心座標を原点にセットしているから、出力されるのは、ロボット座標系の脚先座標です、現状20200619
void CreateComCandidate::setLegPosition(myvector::SVector legIn[LEGCOUNT])
{
	myvector::SVector rotationLeg[6];

	for (int i = 0; i < 6; i++)
	{
		rotationLeg[i] = myvector::VRot(legIn[i], myvector::VGet(0, 0, 0), phantomX.showGlobalMyDirectionthP(), phantomX.showGlobalMyDirectionthR(), phantomX.showGlobalMyDirectionthY());

		//CreateComCandidateの座標(x,y,z)は，PassFindingにおける(x,z,y)となる
		m_leg_pos[i].x = rotationLeg[i].x + phantomX.showGlobalCoxaJointPosi(i).x;
		m_leg_pos[i].y = rotationLeg[i].z + phantomX.showGlobalCoxaJointPosi(i).z;
		m_leg_pos[i].z = rotationLeg[i].y + phantomX.showGlobalCoxaJointPosi(i).y;
	}
}

void CreateComCandidate::setGroundingLeg(int In_groundingLeg[6]) {

	for (int i = 0; i < 6; i++)
	{
		groundingLeg[i] = In_groundingLeg[i];
		//groundingLeg[i] = 1;
	}
}