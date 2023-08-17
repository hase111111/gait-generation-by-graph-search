#include "pass_finder_fixed.h"

#include "pass_finder_hato_thread.h"
#include "pass_finder_factory_hato.h"


EGraphSearchResult PassFinderFixed::getNextNodebyGraphSearch(const SNode& current_node, const MapState* const p_map, const STarget& target, SNode& output_node)
{
	bool do_reevaluation = false;

	// 1. í èÌÇÃíTçı
	{
		PassFinderHatoThread pass_finder_hato_thread(std::make_unique<PassFinderFactoryHato>());

		EGraphSearchResult result = pass_finder_hato_thread.getNextNodebyGraphSearch(current_node, p_map, target, output_node);

		if (result == EGraphSearchResult::FailureByLegPathGenerationError)
		{
			do_reevaluation = true;
		}
		else
		{
			return result;
		}
	}

	// 2. çƒï]âø
	{
		if (do_reevaluation)
		{
			PassFinderHatoThread pass_finder_hato_thread(std::make_unique<PassFinderFactoryHato>());

			EGraphSearchResult result = pass_finder_hato_thread.getNextNodebyGraphSearch(current_node, p_map, target, output_node);

			if (result == EGraphSearchResult::FailureByLegPathGenerationError)
			{
				return EGraphSearchResult::FailureByLegPathGenerationError;
			}
		}
	}

	return EGraphSearchResult::Success;
}
