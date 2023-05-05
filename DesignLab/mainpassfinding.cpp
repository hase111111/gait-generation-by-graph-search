#include <boost\thread.hpp>
#include "multiDetectLandingPoint.h"
#include "hexapodGraphic.h"
#include <string>
#include "mainfunction.h"
//#include "../multiPassFinding/multiPassFinding.h"
#include "functionPassFinding.h"
//#include "..\walking_pattern_generator_for_hexapod_robot\passfinding.h"

void getMap(myvector::VECTOR p_mapData3D[1000], myvector::VECTOR legPosi[6], myvector::VECTOR COM);

void main(void){

	std::string stop;

	//現在の体勢
	//LNODE nowNode, PastCondition;
	//nowNode.GCOM = myvector::VGet(0,0,100);
	//nowNode.Leg[0] = myvector::VGet(120,80,-100);
	//nowNode.Leg[1] = myvector::VGet(120,0,-100);
	//nowNode.Leg[2] = myvector::VGet(120,-80,-100);
	//nowNode.Leg[3] = myvector::VGet(-120,-80,-100);
	//nowNode.Leg[4] = myvector::VGet(-120,0,-100);
	//nowNode.Leg[5] = myvector::VGet(-120,80,-100);
	//nowNode.ThP = 0;
	//nowNode.ThR = 0;
	//nowNode.ThY = 0;
	LNODE NewCondition;							//新しいロボットの状態
	LNODE CurrentCondition;						//現在のロボットの状態
	LNODE PastCondition;						//1つ前のロボットの状態
	
	myvector::VECTOR IPosition_of_Leg[6];
	IPosition_of_Leg[0] = myvector::VGet(120,  80, -100);
	IPosition_of_Leg[1] = myvector::VGet(120,   0, -100);
	IPosition_of_Leg[2] = myvector::VGet(120, -80, -100);
	IPosition_of_Leg[3] = myvector::VGet(-120, -80, -100);
	IPosition_of_Leg[4] = myvector::VGet(-120,   0, -100);
	IPosition_of_Leg[5] = myvector::VGet(-120,  80, -100);
	
	//////////////自分の初期状態をCurrentConditionに代入//////////////
	CurrentCondition.COM = 0;
	CurrentCondition.kaisou = 295;				//iHX4[1][1][1][1][1][1] = 295	iHX4[2][2][2][2][2][2] = 729	iHX4[1][2][1][2][1][2]=670
	CurrentCondition.v = 0;

	CurrentCondition.COM_type = 0;
	CurrentCondition.parent = NULL;
	CurrentCondition.NoodeHeight = 1;
	CurrentCondition.GCOM = myvector::VGet(	0,	0,	100	);
	for(int i = 0; i < 6; i++){
		CurrentCondition.Leg[i] = IPosition_of_Leg[i];
		CurrentCondition.Leg2[i] = IPosition_of_Leg[i];
	}
	CurrentCondition.ThP = 0.0;
	CurrentCondition.ThR = 0.0;
	CurrentCondition.ThY = 0.0;






	PastCondition = CurrentCondition;
	PastCondition.v = -1;


	////マルチスレッド仕様のpassFindingクラス
	//multiPassFinding M_P_F;
	myvector::VECTOR mapDataBackUp[1000];

	getMap(mapDataBackUp, CurrentCondition.Leg, CurrentCondition.GCOM);

	//M_P_F.setWalkingDirection(myvector::VGet(0.0, 1.0, 0.0));
	////M_P_F.setPastCondition();
	//M_P_F.FILEOPEN();
	FILEOPEN();


	/////////////////////脚先位置決定部/////////////////
	//for(;;){
	//	std::cin>>stop;			//カメラが座標を取るまで待機
	//	if(stop =="g")break;
	//}
	//{
	////boost::mutex::scoped_lock lk(mtxMapData);			//マップへのアクセス権 長時間アクセス権を離さないと，カメラがエラーする
	////std::cin>>stop;
	//M_P_F.setCurrentCondition(NewCondition);
	//M_P_F.setMap(1000, mapDataBackUp);

	//nowNode = M_P_F.choose_the_most_suitable_solution();
	//std::cout<<"M_P_F.choose_the_most_suitable_solution end";
	//}
	////////////////////////////////////////////////////


	//M_P_F.FILECLOSE();


	for(int iLoopCount = 0; iLoopCount < 10; iLoopCount++){
		std::cout<<"iLoopCount = "<<iLoopCount<<"\n";

		NewCondition = functionPassFinding(CurrentCondition, PastCondition, 1000, mapDataBackUp, myvector::VGet(0.0, 1.0, 0.0));
		PastCondition = CurrentCondition;
		CurrentCondition = NewCondition;

		CurrentCondition.NoodeHeight = 1;
		CurrentCondition.parent = NULL;
		CurrentCondition.debug = 0;

		LNODEOutPut(CurrentCondition);
	}
	std::cin>>stop;

	FILECLOSE();




}


void getMap(myvector::VECTOR p_mapData3D[1000], myvector::VECTOR legPosi[6], myvector::VECTOR COM){
	//std::cout<<"mapData3D_MAX"<<"\n";

	


//	for(int i = 6; i < *mapData3D_MAX-8; i+=8){
//
//	//if(i > 90 && i < 109)continue;		
//		p_mapData3D[i].x =- 150;
//		p_mapData3D[i].y = i * 7 - 500;
//		p_mapData3D[i].z = 0;
//
//		p_mapData3D[i+1].x =- 180;
//		p_mapData3D[i+1].y = i * 7 - 500;
//		p_mapData3D[i+1].z = 0;
//
//		p_mapData3D[i+2].x =- 220;
//		p_mapData3D[i+2].y = i * 7 - 500;
//		p_mapData3D[i+2].z = 0;
//
//		p_mapData3D[i+3].x =- 250;
//		p_mapData3D[i+3].y = i * 7 - 500;
//		p_mapData3D[i+3].z = 0;
//
////	if(i > 90 && i < 112)continue;
//
//		p_mapData3D[i+4].x = 400 - 150;
//		p_mapData3D[i+4].y = i * 7 - 500;
//		p_mapData3D[i+4].z = 0;
//
//		p_mapData3D[i+5].x = 400 - 180;
//		p_mapData3D[i+5].y = i * 7 - 500;
//		p_mapData3D[i+5].z = 0;
//
//		p_mapData3D[i+6].x = 400 - 220;
//		p_mapData3D[i+6].y = i * 7 - 500;
//		p_mapData3D[i+6].z = 0;
//
//		p_mapData3D[i+7].x = 400 - 250;
//		p_mapData3D[i+7].y = i * 7 - 500;
//		p_mapData3D[i+7].z = 0;
//
//	}



	for(int i = 6; i < 1000; i++){
		//----------------------------ランダム-------------------------------------
		p_mapData3D[i].x = (rand()%3000) - 1000;				//脚着地可能点
		p_mapData3D[i].y = (rand()%3000) - 1000;				//脚着地可能点
		p_mapData3D[i].z = 0;
		//-------------------------------------------------------------------------
	}



	

	hexapod phantomX;
	phantomX.setMyPosition(COM);
	phantomX.setTravelingDirection(COM);
	phantomX.setMyDirection(0,0,0);
	phantomX.setPosition_of_2(legPosi);

	p_mapData3D[0] = myvector::VAdd(legPosi[0], phantomX.showGlobalCoxaJointPosi(0));
	p_mapData3D[1] = myvector::VAdd(legPosi[1], phantomX.showGlobalCoxaJointPosi(1));
	p_mapData3D[2] = myvector::VAdd(legPosi[2], phantomX.showGlobalCoxaJointPosi(2));
	p_mapData3D[3] = myvector::VAdd(legPosi[3], phantomX.showGlobalCoxaJointPosi(3));
	p_mapData3D[4] = myvector::VAdd(legPosi[4], phantomX.showGlobalCoxaJointPosi(4));
	p_mapData3D[5] = myvector::VAdd(legPosi[5], phantomX.showGlobalCoxaJointPosi(5));
	//for(int i = 0; i < 6; i++)myvector::VectorOutPut(p_mapData3D[i]);
	//for(int i = 0; i < 6; i++)myvector::VectorOutPut(phantomX.showPosition_of_2(i));

}
