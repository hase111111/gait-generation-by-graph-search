#include "NodeEdit.h"
#include "MapConst.h"
#include "MyMath.h"
#include "LegState.h"

void node_edit::initNode(SNode& _node, const bool _do_random)
{
	//脚状態
	_node.leg_state = 0;

	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		// 脚の状態は全て，脚位置 4 で接地している状態 ( つまり bitで 1100 ) に初期化する．
		leg_state::changeLegState(_node.leg_state, i, 0b1100);
	}

	//脚付け根を原点とした，脚先の位置を初期化する．
	const float COM_Z = HexapodConst::VERTICAL_MIN_RANGE + MapConst::MAX_Z_BASE;	// ロボットの重心のZ座標

	_node.Leg[0] = my_vec::SVector(100.0f,	-120.0f,	-COM_Z);
	_node.Leg[1] = my_vec::SVector(0.0f,		-130.0f,	-COM_Z);
	_node.Leg[2] = my_vec::SVector(-100.0f,	-120.0f,	-COM_Z);
	_node.Leg[3] = my_vec::SVector(-100.0f,	120.0f,		-COM_Z);
	_node.Leg[4] = my_vec::SVector(0.0f,		130.0f,		-COM_Z);
	_node.Leg[5] = my_vec::SVector(100.0f,	120.0f,		-COM_Z);

	//脚付け根を原点とした，脚位置4に該当する座標を初期化する．
	_node.Leg2[0] = my_vec::SVector(100.0f,	-120.0f,	-COM_Z);
	_node.Leg2[1] = my_vec::SVector(0.0f,		-130.0f,	-COM_Z);
	_node.Leg2[2] = my_vec::SVector(-100.0f,	-120.0f,	-COM_Z);
	_node.Leg2[3] = my_vec::SVector(-100.0f,	120.0f,		-COM_Z);
	_node.Leg2[4] = my_vec::SVector(0.0f,		130.0f,		-COM_Z);
	_node.Leg2[5] = my_vec::SVector(100.0f,	120.0f,		-COM_Z);

	//グローバル座標の重心位置．グローバル座標(0,0,0)を中心とした，下の変数 _x，_yを半径とする楕円形のなかに重心を移動する．
	const float _angle = _do_random ? my_math::generateRandomNumber(0.0f, 2.0f * my_math::MY_FLT_PI) : 0;
	const float _ex = _do_random ? my_math::generateRandomNumber(0.0f, 1.0f) : 0;

	const float _x = ((float)MapConst::MAP_START_ROUGH - MapConst::MAP_MIN_FORWARD) * 0.25f;
	const float _y = ((float)MapConst::MAP_MAX_HORIZONTAL - MapConst::MAP_MIN_HORIZONTAL) / 2.0f * 0.8f;

	_node.global_center_of_mass = my_vec::SVector(_ex * _x * cos(_angle), _ex * _y * sin(_angle), COM_Z);


	//ロールピッチヨーで回転を表現する．ロボットの重心を中心にして回転する． 
	_node.rot = my_vec::SRotator(0, 0, 0);

	_node.next_move = EHexapodMove::COM_UP_DOWN;
	_node.parent_num = -1;
	_node.depth = 0;


	//以下，もう使う気がないパラメータ．

	_node.roll = 0.0f;			// x軸回転
	_node.pitch = 0.0f;			// y軸回転
	_node.yaw = 0.0f;			// z軸回転

	_node.parent = nullptr;		//親ノードのポインタ
	_node.node_height = 1;		//ノード高さ
	_node.debug = 24;			//現在運動履歴として使用,前回の脚上下ノード(上下運動をした場合)2桁,前回の動作1桁,前々回の動作1桁

	_node.last_node_num = 0;
	_node.time = 0;

	_node.delta_comz = 0.0f;
	_node.target_delta_comz = 0;
}

void node_edit::changeParentNode(SNode& _node)
{
	_node.depth = 0;		//深さを0にする
	_node.parent_num = -1;	//自身が親のため，負の値を代入する．
}

void node_edit::changeNextNode(SNode& _node, const int _parent_num, const EHexapodMove _next_move)
{
	_node.depth += 1;
	_node.parent_num = _parent_num;
	_node.next_move = _next_move;
}

std::string node_edit::getTextHexapodMove(const EHexapodMove _move)
{
	switch (_move)
	{
	case EHexapodMove::COM_MOVE:
		return "重心の平行移動";

	case EHexapodMove::COM_UP_DOWN:
		return "重心の上下移動";

	case EHexapodMove::LEG_HIERARCHY_CHANGE:
		return "脚の階層変更";

	case EHexapodMove::LEG_UP_DOWN:
		return "脚の上下移動";

	default:
		return "定義されてない動作です．NodeEdit.hのgetTextHexapodMove関数に動作を追加してください";
		break;
	}
}
