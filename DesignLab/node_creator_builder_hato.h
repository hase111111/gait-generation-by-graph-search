//! @file node_creator_builder_hato.h
//! @brief 波東さんが行った処理と同様の結果が得られるようにするためのクラス

#ifndef DESIGNLAB_NODE_CREATOR_BUILDER_HATO_H_
#define DESIGNLAB_NODE_CREATOR_BUILDER_HATO_H_

#include "interface_node_creator_builder.h"


class NodeCreatorBuilderHato : public INodeCreatorBuilder
{
public:

	void Build(const DevideMapState& map, const std::shared_ptr<const AbstractHexapodStateCalculator>& calculator_ptr,
		std::map<HexapodMove, std::unique_ptr<INodeCreator>>* node_creator) const override;

};


#endif // DESIGNLAB_NODE_CREATOR_BUILDER_HATO_H_