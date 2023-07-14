#include "HexapodNextMove.h"

std::string std::to_string(const EHexapodMove _move)
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
		return "未定義動作";
		break;
	}
}
