#pragma once
#include "..\vectorFunc\vectorFunc.h"

myvector::VECTOR LNODEGCOMtoD_L_Pp_comm(myvector::VECTOR VIn);

void getMap(myvector::VECTOR *p_mapData3D, int* mapData3D_MAX, myvector::VECTOR legPosi[6], myvector::VECTOR COM);

void recalMap(myvector::VECTOR *p_mapData3D, int mapData3D_MAX, myvector::VECTOR CenterOfGravityMovement);