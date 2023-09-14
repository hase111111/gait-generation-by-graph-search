#include "discrete_leg_pos.h"

std::string std::to_string(const EDiscreteLegPos leg_pos)
{
	switch (leg_pos)
	{
	case EDiscreteLegPos::FRONT:
		return "FRONT";

	case EDiscreteLegPos::BACK:
		return "BACK";

	case EDiscreteLegPos::CENTER:
		return "CENTER";

	case EDiscreteLegPos::LOWER_BACK:
		return "LOWER_BACK";

	case EDiscreteLegPos::LOWER_FRONT:
		return "LOWER_FRONT";

	case EDiscreteLegPos::UPPER_BACK:
		return "UPPER_BACK";

	case EDiscreteLegPos::UPPER_FRONT:
		return "UPPER_FRONT";

	default:
		return "ERROR_VALUE";
	}
}

