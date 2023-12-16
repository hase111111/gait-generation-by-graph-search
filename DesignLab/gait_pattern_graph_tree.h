//! @file gait_pattern_graph_tree.h
//! @brief RobotStateNode 構造体をノードとする木構造のグラフのクラス．

#ifndef DESIGNLAB_GAIT_PATTERN_GRAPH_TREE_H_
#define DESIGNLAB_GAIT_PATTERN_GRAPH_TREE_H_

#include <vector>

#include "cassert_define.h"
#include "robot_state_node.h"


namespace designlab
{

//! @class GaitPatternGraphTree
//! @brief RobotStateNode 構造体をノードとする木構造のグラフのクラス．
//! @details 論文でいうところの歩容パターングラフ．
class GaitPatternGraphTree final
{
public:

	GaitPatternGraphTree() = delete;

	//! @brief コンストラクタ．
	//! @n ノード数の最大値を指定する．
	//! @param[in] graph_max_size ノード数の最大値．
	explicit inline GaitPatternGraphTree(const int graph_max_size) :
		graph_size_(0),
		kGraphMaxSize(graph_max_size)
	{
		nodes_.resize(graph_max_size);
	}

	//! @brief グラフのノードの総数を返す．
	//! @return int グラフのノードの総数．
	[[nodiscard]] constexpr int GetGraphSize() const { return graph_size_; }

	//! @brief グラフが空かどうかを返す．
	//! @return bool グラフが空ならtrue．
	[[nodiscard]] constexpr bool IsEmpty() const { return graph_size_ == 0; }

	//! @brief グラフが根ノードを持つかどうかを返す．
	//! @n 根ノードとは，親を持たないノードのこと．
	//! @n 一番最初に追加するノードは必ず親になるため，親を持つかどうかはノードの総数が0でないかどうかで判定できる．
	//! @return bool グラフが親を持つならtrue．
	[[nodiscard]] constexpr bool HasRoot() const
	{
		return graph_size_ != 0;
	}

	//! @brief グラフの根ノードのindexを返す．
	//! @n ノードがない場合を考慮していないため，HasRoot()で根ノードを持つかどうかを確認すること．
	//! @n 一番最初に追加されたノードは必ず根ノードになるため，根ノードのindexは0で固定される．
	//! @return int グラフの根ノードのindex．
	[[nodiscard]] constexpr int GetRootIndex() const
	{
		return 0;
	}

	//! @brief グラフの根ノードの参照を返す．
	//! @return const RobotStateNode& グラフの根ノードの参照．
	[[nodiscard]] inline const RobotStateNode& GetRootNode() const
	{
		return nodes_[0];
	}

	//! @brief グラフのノードの参照を返す．
	//! @param[in] index 参照したいノードのindex．最大ノード数を超える数を指定するとassertionに引っかかる．
	//! @return const RobotStateNode& indexで指定したノードの参照．
	[[nodiscard]] inline const RobotStateNode& GetNode(const int index) const
	{
		assert(0 <= index);
		assert(index < graph_size_);
		return nodes_[index];
	}

	//! @brief 指定したノードの親ノードの参照を返す．depthは親ノードの深さを指定する．
	//! @param[in] index 参照したいノードのindex．最大ノード数を超える数を指定するとassertionに引っかかる．
	//! @param[in] depth 親ノードの深さ．
	//! @return const RobotStateNode& indexで指定したノードの親ノードの参照．
	[[nodiscard]] const RobotStateNode& GetParentNode(const int index, const int depth) const;

	//! @brief 指定したノードの親ノードの参照を返す．depthは親ノードの深さを指定する．
	//! @param[in] index 参照したいノードのindex．最大ノード数を超える数を指定するとassertionに引っかかる．
	//! @param[in] depth 親ノードの深さ．
	//! @return int indexで指定したノードの親ノードのindex．
	[[nodiscard]] const int GetParentNodeIndex(const int index, const int depth) const;

	//! @brief ノードを追加する．
	//! @n 追加するノードは親ノードのindexと，depthの指定が適切にされている必要がある．
	//! @n これらが適切にされていない場合，assertionに引っかかる．
	//! @n また，あらかじめ確保したノード数を超えて追加しようとするとassertionに引っかかる．
	//! @n この条件下では一番最初に追加されるノードは必ず根ノード(親を持たず，深さ0)になる，
	//! @param [in] node 追加するノード．
	inline void AddNode(const RobotStateNode& node)
	{
		assert(graph_size_ < kGraphMaxSize);

		nodes_[graph_size_++] = node;

		assert(nodes_[graph_size_ - 1].IsLootNode() || 0 <= nodes_[graph_size_ - 1].parent_index);
		assert(nodes_[graph_size_ - 1].parent_index < graph_size_);
		assert(nodes_[graph_size_ - 1].IsLootNode() || nodes_[graph_size_ - 1].depth - 1 == nodes_[nodes_[graph_size_ - 1].parent_index].depth);
	}

	//! @brief グラフをリセットする．
	constexpr void Reset() { graph_size_ = 0; }

private:

	std::vector<RobotStateNode> nodes_;		//!< グラフのデータ．
	int graph_size_;						//!< グラフのサイズ．push_backが重たいので，あらかじめresizeしておき，実際のサイズはこの変数で管理する．
	const int kGraphMaxSize;				//!< グラフの最大サイズ．
};

}	// namespace designlab


#endif	// DESIGNLAB_GAIT_PATTERN_GRAPH_TREE_H_