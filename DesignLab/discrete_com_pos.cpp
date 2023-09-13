#include "discrete_com_pos.h"


std::string std::to_string(EDiscreteComPos com_pos)
{
	switch (com_pos)
	{
	case EDiscreteComPos::FRONT:
		return "FRONT";
	case EDiscreteComPos::FRONT_LEFT:
		return "FRONT_LEFT";
	case EDiscreteComPos::BACK_LEFT:
		return "BACK_LEFT";
	case EDiscreteComPos::BACK:
		return "BACK";
	case EDiscreteComPos::BACK_RIGHT:
		return "BACK_RIGHT";
	case EDiscreteComPos::FRONT_RIGHT:
		return "FRONT_RIGHT";
	case EDiscreteComPos::CENTER_FRONT:
		return "CENTER_FRONT";
	case EDiscreteComPos::CENTER_BACK:
		return "CENTER_BACK";
	case EDiscreteComPos::ERROR_POS:
		return "ERROR_POS";
	default:
		return "ERROR_POS";
	}
}
