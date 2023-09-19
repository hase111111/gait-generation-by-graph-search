#include "graph_search_result.h"


std::string std::to_string(EGraphSearchResult result)
{
	switch (result)
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
