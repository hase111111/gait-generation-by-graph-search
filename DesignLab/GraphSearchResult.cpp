#include "GraphSearchResult.h"

std::string std::to_string(EGraphSearchResult _result)
{
	switch (_result)
	{
	case EGraphSearchResult::Success:
		return "Success";

	case EGraphSearchResult::Failure:
		return "Failure";

	case EGraphSearchResult::FailureByInitializationFailed:
		return "FailureByInitializationFailed";

	case EGraphSearchResult::FailureByNodeLimitExceeded:
		return "FailureByNodeLimitExceeded";

	case EGraphSearchResult::FailureByNoNode:
		return "FailureByNoNode";

	case EGraphSearchResult::FailureByNotReachedDepth:
		return "FailureByNotReachedDepth";

	case EGraphSearchResult::FailureByLegPathGenerationError:
		return "FailureByLegPathGenerationError";

	case EGraphSearchResult::SuccessByReevaluation:
		return "SuccessByReevaluation";

	case EGraphSearchResult::FailureByReevaluation:
		return "FailureByReevaluation";

	case EGraphSearchResult::FailureByReevaluationAndNodeLimitExceeded:
		return "FailureByReevaluationAndNodeLimitExceeded";

	case EGraphSearchResult::FailureByReevaluationAndNoNode:
		return "FailureByReevaluationAndNoNode";

	case EGraphSearchResult::FailureByReevaluationAndNotReachedDepth:
		return "FailureByReevaluationAndNotReachedDepth";

	case EGraphSearchResult::FailureByReevaluationAndLegPathGenerationError:
		return "FailureByReevaluationAndLegPathGenerationError";

	default:
		break;
	}

	return "Unknown";
}
