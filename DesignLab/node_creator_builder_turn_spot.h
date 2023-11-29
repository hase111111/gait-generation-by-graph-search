#ifndef DESIGNLAB_CREATOR_BUILDER_TURN_SPOT_H_
#define DESIGNLAB_CREATOR_BUILDER_TURN_SPOT_H_

#include "interface_node_creator_builder.h"


class NodeCreatorBuilderTurnSpot final : public INodeCreatorBuilder
{
public:

	NodeCreatorBuilderTurnSpot(
		const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
		const std::shared_ptr<const IHexapodStatePresenter>& presenter_ptr,
		const std::shared_ptr<const IHexapodVaildChecker>& checker_ptr
	);

	void Build(
		const DevideMapState& map,
		std::map<HexapodMove, std::unique_ptr<INodeCreator>>* node_creator) const override;

private:
	const std::shared_ptr<const IHexapodCoordinateConverter> converter_ptr_;
	const std::shared_ptr<const IHexapodStatePresenter> presenter_ptr_;
	const std::shared_ptr<const IHexapodVaildChecker> checker_ptr_;
};


#endif	//	DESIGNLAB_CREATOR_BUILDER_TURN_SPOT_H_