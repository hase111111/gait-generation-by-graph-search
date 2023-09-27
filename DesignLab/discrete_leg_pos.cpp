#include "discrete_leg_pos.h"

#include "cassert_define.h"


std::string std::to_string(const DiscreteLegPos leg_pos)
{
	switch (leg_pos)
	{
	case DiscreteLegPos::kFront:
		return "kFront";

	case DiscreteLegPos::kBack:
		return "Back";

	case DiscreteLegPos::kCenter:
		return "Center";

	case DiscreteLegPos::kLowerBack:
		return "LowerBack";

	case DiscreteLegPos::kLowerFront:
		return "LowerFront";

	case DiscreteLegPos::kUpperBack:
		return "UpperBack";

	case DiscreteLegPos::kUpperFront:
		return "UpperFront";

	default:
		assert(false);		//‚±‚±‚É‚Í—ˆ‚È‚¢‚Í‚¸

		return "ERROR_VALUE";
	}
}

