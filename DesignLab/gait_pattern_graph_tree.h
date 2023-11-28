#ifndef DESIGNLAB_GAIT_PATTERN_GRAPH_TREE_H_
#define DESIGNLAB_GAIT_PATTERN_GRAPH_TREE_H_

#include <vector>

#include "cassert_define.h"
#include "robot_state_node.h"


class GaitPatternGraphTree final
{
public:

	GaitPatternGraphTree() = delete;

	//! @brief コンストラクタ．
	//! @n ノード数の最大値を指定する．
	inline GaitPatternGraphTree(const int graph_max_size) : graph_size_(0), graph_max_size_(graph_max_size)
	{
		nodes_.resize(graph_max_size);
	}

	//! @brief グラフのノードの総数を返す．
	//! @return グラフのノードの総数．
	constexpr int GetGraphSize() const { return graph_size_; }

	//! @brief グラフのノードの参照を返す．
	//! @param[in] index 参照したいノードのindex．最大ノード数を超える数を指定するとassertionに引っかかる．
	//! @return indexで指定したノードの参照．
	inline const RobotStateNode& GetNode(const int index) const
	{
		assert(index < graph_size_);
		return nodes_[index];
	}

	//! @brief ノードを追加する．
	//! @n 追加するノードは親ノードのindexと，depthの指定が適切にされている必要がある．
	//! @n また，あらかじめ確保したノード数を超えて追加しようとするとassertionに引っかかる．
	inline void AddNode(const RobotStateNode& node)
	{
		assert(graph_size_ < graph_max_size_);

		nodes_[graph_size_++] = node;

		assert(nodes_[graph_size_ - 1].parent_num <= graph_size_);
		assert(nodes_[graph_size_ - 1].depth - 1 == nodes_[nodes_[graph_size_ - 1].parent_num].depth);
	}

	//! @brief グラフをリセットする．
	constexpr void Reset() { graph_size_ = 0; }

private:

	std::vector<RobotStateNode> nodes_;		//!< グラフのデータ．
	int graph_size_;						//!< グラフのサイズ．push_backが重たいので，あらかじめresizeしておき，実際のサイズはgraph_size_で管理する．
	const int graph_max_size_;				//!< グラフの最大サイズ．
};


#endif	// DESIGNLAB_GAIT_PATTERN_GRAPH_TREE_H_