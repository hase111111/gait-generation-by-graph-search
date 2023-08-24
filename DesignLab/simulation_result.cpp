#include "simulation_result.h"


std::string std::to_string(ESimulationResult result)
{
	switch (result)
	{
	case ESimulationResult::SUCCESS:
		return "SUCCESS";
	case ESimulationResult::FAILURE:
		return "FAILURE";
	case ESimulationResult::FAILURE_BY_GRAPH_SEARCH:
		return "FAILURE_BY_GRAPH_SEARCH";
	case ESimulationResult::FAILURE_BY_LOOP_MOTION:
		return "FAILURE_BY_LOOP_MOTION";
	case ESimulationResult::FAILURE_BY_NODE_LIMIT_EXCEEDED:
		return "FAILURE_BY_NODE_LIMIT_EXCEEDED";
	default:
		return "UNKNOWN";
	}
}
