
//! @file      gait_pattern_graph_tree.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

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
    //! @return グラフのノードの総数．
    [[nodiscard]] constexpr int GetGraphSize() const { return graph_size_; }

    //! @brief グラフが空かどうかを返す．
    //! @retval true グラフが空．
    //! @retval false グラフが空でない．
    [[nodiscard]] constexpr bool IsEmpty() const { return graph_size_ == 0; }

    //! @brief グラフが根ノードを持つかどうかを返す．
    //! @n 根ノードとは，親を持たないノードのこと．
    //! @n 一番最初に追加するノードは必ず根になるため，
    //! 根を持つかどうかはノードの総数が0でないかどうかで判定できる．
    //! @retval true グラフが根ノードを持つ．
    //! @retval false グラフが根ノードを持たない．
    [[nodiscard]] constexpr bool HasRoot() const
    {
        return graph_size_ != 0;
    }

    //! @brief グラフの根ノードのインデックスを返す．
    //! @n ノードがない場合を考慮していないため，
    //! HasRoot()で根ノードを持つかどうかを確認すること．
    //! @n 一番最初に追加されたノードは必ず根ノードになるため，
    //! 根ノードのインデックスは0で固定される．
    //! @return グラフの根ノードのインデックス．
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
    //! @param[in] index 参照したいノードのインデックス．
    //! 最大ノード数を超える数を指定するとアサーションに引っかかる．
    //! @return インデックスで指定したノードの参照．
    [[nodiscard]] inline const RobotStateNode& GetNode(const int index) const
    {
        assert(0 <= index);
        assert(index < graph_size_);
        return nodes_[index];
    }

    //! @brief 指定したノードの親ノードの参照を返す．depthは親ノードの深さを指定する．
    //! @param[in] index 参照したいノードのインデックス．
    //! 最大ノード数を超える数を指定するとアサーションに引っかかる．
    //! @param[in] depth 親ノードの深さ．
    //! @return インデックスで指定したノードの親ノードの参照．
    [[nodiscard]] const RobotStateNode& GetParentNode(const int index, const int depth) const;

    //! @brief 指定したノードの親ノードの参照を返す．depthは親ノードの深さを指定する．
    //! @param[in] index 参照したいノードのインデックス．
    //! 最大ノード数を超える数を指定するとアサーションに引っかかる．
    //! @param[in] depth 親ノードの深さ．
    //! @return indexで指定したノードの親ノードのインデックス．
    [[nodiscard]] const int GetParentNodeIndex(const int index, const int depth) const;

    //! @brief ノードを追加する．
    //! @n 追加するノードは親ノードのインデックスと，depthの指定が適切にされている必要がある．
    //! @n これらが適切にされていない場合，アサーションに引っかかる．
    //! @n また，あらかじめ確保したノード数を超えて追加しようとするとアサーションに引っかかる．
    //! @n この条件下では一番最初に追加されるノードは必ず根ノード(親を持たず，深さ0)になる，
    //! @param[in] node 追加するノード．
    inline void AddNode(const RobotStateNode& node)
    {
        assert(graph_size_ < kGraphMaxSize);

        nodes_[graph_size_++] = node;

        assert(nodes_[graph_size_ - 1].IsLootNode() ||
               0 <= nodes_[graph_size_ - 1].parent_index);

        assert(nodes_[graph_size_ - 1].parent_index < graph_size_);

        assert(nodes_[graph_size_ - 1].IsLootNode() ||
          nodes_[graph_size_ - 1].depth - 1 == nodes_[nodes_[graph_size_ - 1].parent_index].depth);
    }

    //! @brief グラフをリセットする．
    constexpr void Reset() { graph_size_ = 0; }

    //! @brief 指定したノードの重心の上下移動軌跡を返す．
    //! @param[in] index 参照したいノードのインデックス．
    //! 最大ノード数を超える数を指定するとアサーションに引っかかる．
    //! @return 指定したノードの重心の上下移動軌跡．
    [[nodiscard]]
    std::vector<float> GetCoMVerticalTrajectory(const int index) const
    {
        assert(0 <= index);
        assert(index < graph_size_);
        std::vector<float> com_vertical_trajectory_reverse;

        // 現在のノードから根ノードまでの重心の上下移動軌跡を取得する．
        for (int i = index; i != -1; i = nodes_[i].parent_index)
        {
            if (com_vertical_trajectory_reverse.empty() ||
                com_vertical_trajectory_reverse.back() != nodes_[i].center_of_mass_global_coord.z)
            {
                com_vertical_trajectory_reverse.push_back(nodes_[i].center_of_mass_global_coord.z);
            }
        }

        // 根ノードから現在のノードまでの重心の上下移動軌跡を取得する．
        std::vector<float> com_vertical_trajectory(com_vertical_trajectory_reverse.size());
        for (int i = 0; i < com_vertical_trajectory_reverse.size(); ++i)
        {
            com_vertical_trajectory[i] = com_vertical_trajectory_reverse[com_vertical_trajectory_reverse.size() - 1 - i];
        }

        return com_vertical_trajectory;
    }

private:
    std::vector<RobotStateNode> nodes_;  //!< グラフのデータ．

    //! グラフのサイズ．push_back が重たいので，
    //! あらかじめ resize しておき，実際のサイズはこの変数で管理する．
    int graph_size_;

    const int kGraphMaxSize;    //!< グラフの最大サイズ．
};

}  // namespace designlab


#endif  // DESIGNLAB_GAIT_PATTERN_GRAPH_TREE_H_
