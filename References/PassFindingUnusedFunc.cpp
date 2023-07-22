#include "PassFinding.h"
#include "function_walking_pattern_generator.h"
#include "vectorFunc.h"
#include "Define.h"
#include "LegState.h"

//PassFinding.cpp の中から現在使用されていない関数を整理するためにまとめて持ってきたもの
//2023/04/28現在 PassFinding.cppの中が余裕で3000行ぐらいあるので，正直全く読めない
//致し方なく，使用していない関数は全部こっちに持ってきて整理していこうと思う
//未使用だからと言って不必要ではなく，先輩方の今までの研究成果のソースコードの塊なので
//資料として大変貴重であるといえるだろう

//ノードの脚位置から，comtypeの重心位置が可能な場合1
bool PassFinding::isAbleLegPosComType(SNode* node)
{
	//myvector::SVector ret_4_GCOM[35];
	//int ret_4_COMType[35] = { 0 }, ret_4_transition_kaisou[35] = { 0 }, passnum = 0;

	//myvector::SVector legPosi[6];//重心移動後の脚位置
	//int groundLegInd[6];		//接地している脚は1

	//for (int le = 0; le < 6; le++) {
	//	//groundLegInd[le] = HX2[node->v][le];
	//	groundLegInd[le] = LegState::isGrounded(node->leg_state, le);
	//}

	//myvector::SVector ret_Leg[6][20];
	//searchTransitionCoM(node, ret_Leg, ret_4_GCOM, ret_4_COMType, passnum);	//これだめな気がする180823

	//for (int i = 0; i < passnum; i++) {
	//	//if(ret_4_COMType[i] == node->COM_type){
	//	if (ret_4_COMType[i] == numOfCOMType(node->leg_state)) {
	//		for (int le = 0; le < 6; le++) {
	//			legPosi[le] = myvector::subVec(node->Leg[le], ret_4_GCOM[i]);			//重心移動後の脚位置
	//		}
	//		return 1;
	//	}
	//}
	////if(node->COM_type == 0){
	//if (numOfCOMType(node->leg_state) == 0) {
	//	return 1;
	//}
	//return 0;
	return false;
}

//二次方向移動(旋回)
void PassFinding::calculatePassLegRotation() {

	//int ret_2_transition_v[36][6]={};												//関数に渡す2次方向移動を保存する配列　36通りある　6は適当にきめた ret_2_transition_v[][0]は格納されているルートの長さ
	//int LegGroundablePointNum[HexapodConst::LEG_NUM][3]={};										//脚の前後の脚接地可能点 LegGroundablePointNum[HexapodConst::LEG_NUM][0] が１の位置を表す。
	//myvector::VECTOR LegGroundablePoint[HexapodConst::LEG_NUM][3][100]={};									//複数ある脚位置1，2，3に位置する点群の中の代表する1点
	//int i_F[6]={};																	//i_F[6]は足をつけるかどうか
	//int passnum_2zi;															//関数から帰ってきた2次方向移動の数
	//for(int to = LastNodeNum; to >= 0 ;to--){
	//	if(route[to].debug == 22) continue;	//すでに2回以上連続で脚を上下運動させている場合
	//	if(route[to].node_height <= 0)continue;
	//	if(route[to].node_height < 2){
	//		//std::cout<<"calculatePassLegRotation\n";
	//		SelectLegPoint_Rotation(&route[to], LegGroundablePointNum, LegGroundablePoint);									//脚位置算出//脚接移動点を決定

	//		////////////////上げっぱなしの脚　故障脚//////////////////////////
	//		//LegGroundablePointNum[4][0] = 0;//4番脚は下ろせない
	//		//LegGroundablePointNum[4][1] = 0;//4番脚は下ろせない
	//		//LegGroundablePointNum[4][2] = 0;//4番脚は下ろせない
	//		//////////////////////////////////////////////////////////////////

	//		////////////////脚接地点消去　実験用　脚位置１を認めない//////////
	//		//LegGroundablePointNum[0][0] = 0;
	//		//LegGroundablePointNum[1][0] = 0;
	//		//LegGroundablePointNum[2][0] = 0;
	//		//LegGroundablePointNum[3][0] = 0;
	//		//LegGroundablePointNum[4][0] = 0;
	//		//LegGroundablePointNum[5][0] = 0;
	//		//////////////////////////////////////////////////////////////////

	//		bool visited[36] = { 0 };	//探索済みもしくは探索できないノードは1になる
	//		getGraph(route[to].leg_state, i_F, LegGroundablePointNum, visited);	//マップのデータからグラフを得る//遷移不可能なノードはvisited[i]が1になる

	//		for(int ileg = 0; ileg < 6; ileg ++){
	//			//if (HX2[route[to].v][ileg] == 1 && i_F[ileg] == 0) {//接地脚の脚接地点数が0
	//			if (isGrounded(route[to].leg_state, ileg) && i_F[ileg] == 0) {//接地脚の脚接地点数が0
	//				route[to].node_height = -1;
	//				break;
	//			}
	//		}
	//		//std::cout<<"route[to].v = "<<route[to].v<<"\n";
	//		pass_transitions_2zi(route[to].leg_state, visited, ret_2_transition_v, &passnum_2zi);	//i_F[6]は足をつけるかどうか　ret_2_transition_v[36][6]に結果//階層内最短経路探索
	//		//passnum_2ziは移動可能なノードの数
	//		//ret_2_transition_v[a][b],aは移動可能なノードのノード番号の小さい順,bはルートb=0ルート長さb=1今のノードb=2ひとつ先のノードb=3二つ先のノード…
	//																											
	//		for(int ii = 0; ii < passnum_2zi; ii++){
	//			//std::cout<<"ルート長さ="<<ret_2_transition_v[ii][0]<<std::endl;
	//			//std::string stop; std::cin>>stop;
	//			for(int iii=0; iii<1/*ret_2_transition_v[ii][0]*/; iii++){			//格納されているルートの長さだけ繰り返し
	//				(LastNodeNum)++;
	//				if(LastNodeNum >= m_route_max){std::cout<<"Error Overflow LastNodeNum = "<<LastNodeNum;std::string r;std::cin>>r;}			//オーバーフローする前に緊急停止
	//				//route[LastNodeNum].v = ret_2_transition_v[ii][iii+2];					//vの値を更新
	//				route[LastNodeNum].leg_state = ret_2_transition_v[ii][iii+2];					//vの値を更新
	//				//route[LastNodeNum].kaisou = route[to].kaisou;							//階層は変わらない
	//				route[LastNodeNum].center_of_mass = route[to].center_of_mass;									//重心位置は親ノードと同じ
	//				route[LastNodeNum].global_center_of_mass = route[to].global_center_of_mass;								//重心位置は親ノードと同じ
	//				//route[LastNodeNum].COM_type = route[to].COM_type;						//重心位置タイプは親ノードと同じ
	//				route[LastNodeNum].pitch = route[to].pitch;//ここで旋回は行わない
	//				route[LastNodeNum].roll = route[to].roll;
	//				route[LastNodeNum].yaw = route[to].yaw;
	//				route[LastNodeNum].debug = route[to].debug / 10 + 20;
	//				int legPosiInd[6] = {0};												//LegGroundablePoint[HexapodConst::LEG_NUM][3][100]　に格納されている脚位置のインデックス[100]基本0だが不能な脚位置の場合は順次++1
	//				myvector::VECTOR myCom;
	//				//std::cout<<"v="<<route[LastNodeNum].v<<std::endl;
	//				//					for(;;){
	//				for(int iLeg = 0; iLeg < 6; iLeg++){
	//					int legv = isGrounded(route[LastNodeNum].leg_state, iLeg);
	//					int legkaisou = numOfLegPosi(route[LastNodeNum].leg_state, iLeg);
	//					//std::cout<<"route[LastNodeNum].v = "<<route[LastNodeNum].v<<"\n";
	//					if(legv == 0 ){								//0は遊脚
	//						route[LastNodeNum].Leg[iLeg] = route[to].Leg[iLeg];
	//						route[LastNodeNum].Leg[iLeg].z = -route[LastNodeNum].global_center_of_mass.z + LegHeight;
	//						//if(route[LastNodeNum].Leg[iLeg].z == -100){
	//						//	std::cout<<"ERROR1 route[LastNodeNum].Leg["<<iLeg<<"].z != -100\n";
	//						//std::cout<<"route[LastNodeNum].Leg["<<iLeg<<"].z = "<<route[LastNodeNum].Leg[iLeg].z<<"\n";
	//						//std::string stop;
	//						//std::cin>>stop;
	//						//}

	//					}else if(legkaisou == 2){					//脚位置が2ならLeg2を代入
	//						route[LastNodeNum].Leg[iLeg] = route[to].Leg2[iLeg];					//脚位置を更新
	//						//route[LastNodeNum].Leg[iLeg] = LegGroundablePoint[iLeg][HX4[route[LastNodeNum].kaisou][iLeg] - 1][legPosiInd[iLeg]];
	//						route[LastNodeNum].Leg[iLeg] = LegGroundablePoint[iLeg][legkaisou - 1][legPosiInd[iLeg]];
	//						//if(LegGroundablePointNum[iLeg][HX4[route[LastNodeNum].kaisou][iLeg] - 1]==0)std::cout<<"えっ!!\n";
	//						if(LegGroundablePointNum[iLeg][legkaisou - 1]==0)std::cout<<"えっ!!\n";
	//							if(route[LastNodeNum].Leg[iLeg].z != -110){
	//								std::cout<<"ERROR2 route[LastNodeNum].Leg["<<iLeg<<"].z != -100\n";
	//								std::cout<<"route[LastNodeNum].Leg["<<iLeg<<"].z = "<<route[LastNodeNum].Leg[iLeg].z<<"\n";
	//								std::string stop;
	//								std::cin>>stop;
	//						}

	//					}else{
	//						//route[LastNodeNum].Leg[iLeg] = LegGroundablePoint[iLeg][HX4[route[LastNodeNum].kaisou][iLeg] - 1][legPosiInd[iLeg]];//iLeg番目の脚位置（VECTOR）を代入
	//						//if(LegGroundablePointNum[iLeg][HX4[route[LastNodeNum].kaisou][iLeg] - 1]==0)std::cout<<"えっ!!\n";
	//						route[LastNodeNum].Leg[iLeg] = LegGroundablePoint[iLeg][legkaisou - 1][legPosiInd[iLeg]];//iLeg番目の脚位置（VECTOR）を代入
	//						if (LegGroundablePointNum[iLeg][legkaisou - 1] == 0)std::cout << "えっ!!\n";
	//							if(route[LastNodeNum].Leg[iLeg].z != -110){
	//								std::cout<<"ERROR3 route[LastNodeNum].Leg["<<iLeg<<"].z != -100\n";
	//								std::cout<<"route[LastNodeNum].Leg["<<iLeg<<"].z = "<<route[LastNodeNum].Leg[iLeg].z<<"\n";
	//								std::string stop;
	//								std::cin>>stop;
	//						}
	//					}
	//					route[LastNodeNum].Leg2[iLeg] = route[to].Leg2[iLeg];						//脚位置２は変更しない

	//				}//脚位置代入終了

	//				 //						if(isAbleLegPosComType(&route[LastNodeNum],myCom) )break;						//可能な体勢なら抜ける

	//				 //					}
	//				if(iii == 0){
	//					route[LastNodeNum].parent = &route[to];								//親ノードと子ノードを接続

	//					if(route[LastNodeNum].parent->node_height <= 0){
	//						route[LastNodeNum].node_height = route[LastNodeNum].parent->node_height - 1;
	//						route[LastNodeNum].debug = -1;
	//						std::cout<<"親NoodeHeight_Error"<<std::endl;std::string stop;std::cin>>stop;
	//					}else{
	//						route[LastNodeNum].node_height = 1;								//とりあえず更新可に
	//					}

	//				}else{
	//					route[LastNodeNum].parent = &route[LastNodeNum - 1];				//親ノードと子ノードを接続

	//					if(route[LastNodeNum].parent->node_height <= 0){
	//						route[LastNodeNum].node_height = route[LastNodeNum].parent->node_height - 1;
	//						route[LastNodeNum].debug = -1;
	//						std::cout<<"親NoodeHeight_Error"<<std::endl;std::string stop;std::cin>>stop;
	//					}else{
	//						route[LastNodeNum].node_height = 1;								//とりあえず更新可に
	//						route[LastNodeNum - 1].node_height = 3;								//2次移動の途中のノードなので更新不可
	//					}

	//				}

	//				////////////////////////////////////////////////////////

	//				////ここでC_C_Cを呼ぶ　脚位置渡して重心タイプあるかどうか判断　route[LastNodeNum].COM_type
	//				////無かったら脚位置変える　どうやって？　1本しか脚位置を変更していないなら楽
	//				////まずは重心タイプなかったらノード消去で
	//				//myvector::VECTOR myComPosi, beforeLeg2Posi[6];
	//				//if( isAbleLegPosComType(&route[LastNodeNum], &myComPosi) && route[LastNodeNum].parent->node_height > 0){
	//				//	//route[LastNodeNum].global_center_of_mass = myvector::addVec(myComPosi, route[LastNodeNum].global_center_of_mass);
	//				//	//for(int le = 0;le < 6; le++){
	//				//	//	route[LastNodeNum].Leg[le] = myvector::subVec(route[LastNodeNum].Leg[le], myComPosi) ;			//脚位置を更新
	//				//	//	//beforeLeg2Posi[le] = route[LastNodeNum].Leg2[le];
	//				//	//	route[LastNodeNum].Leg2[le] = myvector::subVec(route[LastNodeNum].Leg2[le], myComPosi) ;			//脚位置を更新
	//				//	//	//route[LastNodeNum].Leg2[le].z = beforeLeg2Posi[le].z;
	//				//	//}
	//				//	route[LastNodeNum].debug = 20;
	//				//}else if( !isAbleLegPosComType(&route[LastNodeNum], &myComPosi) && route[LastNodeNum].parent->node_height > 0){
	//				//	//std::cout<<"/////////////////////////////////////////////////////";
	//				//	route[LastNodeNum].node_height = route[LastNodeNum].node_height -100;		//これ以降の更新不可
	//				//	route[LastNodeNum].debug = -1;
	//				//	
	//				//}else{
	//				//	route[LastNodeNum].node_height = route[LastNodeNum].node_height - 1000;		//これ以降の更新不可
	//				//	route[LastNodeNum].debug = -1;

	//				//}
	//				////////////////////////////////////////////////////////

	//			}
	//		}
	//		route[to].node_height ++;																//親ノードのタイプをプラス1
	//	}
	//	//std::cout<<to<<",,"<<route[to].node_height<<std::endl;
	//	//std::string stop;std::cin>>stop;
	//}

}

//胴体回転の探索
void PassFinding::pass_body_rotation(SNode* node, double thPRY[3][20], myvector::SVector ret_leg_move[HexapodConst::LEG_NUM][20], int* Rotation_passnum)
{
	//int i, Rotation_num;
	//int groundLeg[6];
	//for (i = 0; i < HexapodConst::LEG_NUM; i++) {
	//	//groundLeg[i] = HX2[node->v][i];	//接地か遊脚状態かをコピー
	//	groundLeg[i] = LegState::isGrounded(node->leg_state, i);	//接地か遊脚状態かをコピー
	//}
	//_SearchPossibleBodyRotation.phantomX.setMyPosition(node->global_center_of_mass);					//重心位置
	////_SearchPossibleBodyRotation.phantomX.setMyPosition(myvector::VGet(0,0,100));	
	//_SearchPossibleBodyRotation.phantomX.setTarget(m_target);	//進行方向
	//_SearchPossibleBodyRotation.phantomX.setMyDirection(node->pitch, node->roll, node->yaw);	//姿勢
	//_SearchPossibleBodyRotation.phantomX.setLocalLeg2Pos(node->Leg2);					//脚位置z=接地
	//_SearchPossibleBodyRotation.phantomX.setLocalLegPos(node->Leg);					//脚位置
	//myvector::SVector Rotation_Center = node->global_center_of_mass;	//回転中心
	////double leg_th[6] = {45,0,-45,-135,180,135};

	//Rotation_num = _SearchPossibleBodyRotation.pass_body_rotation(groundLeg);	//胴体回転の探索
	//for (int i = 0; i < Rotation_num; i++) {		//探索した胴体回転量とロボット座標での脚位置を記録
	//	for (int ii = 0; ii < HexapodConst::LEG_NUM; ii++) {
	//		ret_leg_move[ii][i] = _SearchPossibleBodyRotation.Leg_move[ii][i];
	//	}
	//	thPRY[0][i] = _SearchPossibleBodyRotation.C_thPRY[0][i];
	//	thPRY[1][i] = _SearchPossibleBodyRotation.C_thPRY[1][i];
	//	thPRY[2][i] = _SearchPossibleBodyRotation.C_thPRY[2][i];
	//}
	//*Rotation_passnum = Rotation_num;
}
