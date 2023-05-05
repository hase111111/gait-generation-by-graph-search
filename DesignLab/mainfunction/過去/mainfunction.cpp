#include"mainfunction.h"
#include "..\hexapod\hexapod.h"

myvector::VECTOR LNODEGCOMtoD_L_Pp_comm(myvector::VECTOR VIn){

	return myvector::VGet(VIn.y, VIn.x,VIn.z);

}


//マップ情報を与える関数ダミー
void getMap(myvector::VECTOR *p_mapData3D, int* mapData3D_MAX, myvector::VECTOR legPosi[6], myvector::VECTOR COM){
	//std::cout<<"mapData3D_MAX"<<"\n";

	//*mapData3D_MAX = 500;
	//*mapData3D_MAX = 166;
	*mapData3D_MAX = 1000;
	
	//p_mapData3D = new myvector::VECTOR[ *mapData3D_MAX ];


//	for(int i = 6; i < *mapData3D_MAX-8; i+=8){
//
//	//if(i > 90 && i < 109)continue;		
//		p_mapData3D[i].x =- 150;
//		p_mapData3D[i].y = i * 7 - 300;
//		p_mapData3D[i].z = 0;
//
//		p_mapData3D[i+1].x =- 180;
//		p_mapData3D[i+1].y = i * 7 - 300;
//		p_mapData3D[i+1].z = 0;
//
//		p_mapData3D[i+2].x =- 220;
//		p_mapData3D[i+2].y = i * 7 - 300;
//		p_mapData3D[i+2].z = 0;
//
//		p_mapData3D[i+3].x =- 250;
//		p_mapData3D[i+3].y = i * 7 - 300;
//		p_mapData3D[i+3].z = 0;
//
////	if(i > 90 && i < 112)continue;
//
//		p_mapData3D[i+4].x = 400 - 150;
//		p_mapData3D[i+4].y = i * 7 - 300;
//		p_mapData3D[i+4].z = 0;
//
//		p_mapData3D[i+5].x = 400 - 180;
//		p_mapData3D[i+5].y = i * 7 - 300;
//		p_mapData3D[i+5].z = 0;
//
//		p_mapData3D[i+6].x = 400 - 220;
//		p_mapData3D[i+6].y = i * 7 - 300;
//		p_mapData3D[i+6].z = 0;
//
//		p_mapData3D[i+7].x = 400 - 250;
//		p_mapData3D[i+7].y = i * 7 - 300;
//		p_mapData3D[i+7].z = 0;
//
//	}



	for(int i = 6; i < *mapData3D_MAX; i++){
		//----------------------------ランダム-------------------------------------
		p_mapData3D[i].x = (rand()%1500) - 500;				//脚着地可能点
		p_mapData3D[i].y = (rand()%1500) - 500;				//脚着地可能点
		p_mapData3D[i].z = 0;
		//-------------------------------------------------------------------------
	}


	//for(int i = 6; i < *mapData3D_MAX; i++){
	//	//----------------------------ランダム-------------------------------------
	//	p_mapData3D[i].x = (rand()%4000) - 1500;				//脚着地可能点
	//	p_mapData3D[i].y = (rand()%4000) - 500;				//脚着地可能点
	//	p_mapData3D[i].z = 0;
	//	//-------------------------------------------------------------------------

	//	//p_mapData3D[i].x = 400 * (i%2) - 200;
	//	//p_mapData3D[i].y = i * 30 - 500;
	//	//p_mapData3D[i].z = 0;
	//	
	//	//-------------------------------------------------------------------------

	//}


	

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
	for(int i = 0; i < 6; i++)myvector::VectorOutPut(p_mapData3D[i]);
	for(int i = 0; i < 6; i++)myvector::VectorOutPut(phantomX.showPosition_of_2(i));

}


//マップの平行移動
void recalMap(myvector::VECTOR *p_mapData3D, int mapData3D_MAX, myvector::VECTOR CenterOfGravityMovement){

	for(int i = 0; i < mapData3D_MAX; i ++){
		p_mapData3D[i] = myvector::VSub(p_mapData3D[i],CenterOfGravityMovement);
	}
}




