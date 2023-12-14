//! @file node_creator_builder_rot_test.h
//! @brief 回転動作のテスト用ビルダークラス．

#ifndef DESIGNLAB_NODE_CREATOR_BUILDER_ROT_TEST_H_
#define DESIGNLAB_NODE_CREATOR_BUILDER_ROT_TEST_H_

#include "interface_node_creator_builder.h"


//! @class NodeCreatorBuilderRotTest
//! @brief 回転動作のテスト用ビルダークラス．
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
		std::map<::designlab::enums::HexapodMove, std::unique_ptr<INodeCreator>>* node_creator) const override;

private:
	const std::shared_ptr<const IHexapodCoordinateConverter> converter_ptr_;
	const std::shared_ptr<const IHexapodStatePresenter> presenter_ptr_;
	const std::shared_ptr<const IHexapodVaildChecker> checker_ptr_;
};

#endif	// DESIGNLAB_NODE_CREATOR_BUILDER_ROT_TEST_H_