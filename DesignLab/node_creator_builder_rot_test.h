//! @file node_creator_builder_rot_test.h
//! @brief 回転動作のテストようビルダークラス．

#ifndef DESIGNLAB_NODE_CREATOR_BUILDER_ROT_TEST_H_
#define DESIGNLAB_NODE_CREATOR_BUILDER_ROT_TEST_H_

#include "interface_node_creator_builder.h"


class NodeCreatorBuilderRotTest final : public INodeCreatorBuilder
{
public:

	NodeCreatorBuilderRotTest(
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

#endif	// DESIGNLAB_NODE_CREATOR_BUILDER_ROT_TEST_H_