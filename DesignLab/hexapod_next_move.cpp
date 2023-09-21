#include "hexapod_next_move.h"

#include "cassert_define.h"


std::string std::to_string(const HexapodMove move)
{
	switch (move)
	{
	case HexapodMove::kComMove:
		return "重心の平行移動";

	case HexapodMove::kComUpDown:
		return "重心の上下移動";

	case HexapodMove::kLegHierarchyChange:
		return "脚の階層変更";

	case HexapodMove::kLegUpDown:
		return "脚の上下移動";

	case HexapodMove::kLegUpDownNextComMove:
		return "脚の上下移動→重心の平行移動";

	case HexapodMove::kLegUpDownNextComUpDown:
		return "脚の上下移動→重心の上下移動";

	default:
		assert(false);	//ここに来る場合，メンテナンスする必要がある
		return "未定義動作";
		break;
	}
}
