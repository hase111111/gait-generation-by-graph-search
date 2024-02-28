
//! @file      graph_tree_creator.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2024. All right reserved.

#ifndef DESIGNLAB_GRAPH_TREE_CREATOR_H_
#define DESIGNLAB_GRAPH_TREE_CREATOR_H_

#include <map>
#include <memory>
#include <vector>

#include "gait_pattern_graph_tree.h"
#include "graph_search_result_record.h"
#include "interface_node_creator.h"
#include "interface_node_creator_builder.h"


namespace designlab
{

//! @class GraphTreeCreator
//! @brief 歩容パターングラフを作成するクラス．
//! @details 先行研究のプログラムを見ればわかる通り，実際には処理効率をあげるために，
//! @n 複数スレッド同時に処理を行うのだが，このクラスでは単一のスレッドで処理を行う．
class GraphTreeCreator final
{
public:
    explicit GraphTreeCreator(std::unique_ptr<INodeCreatorBuilder>&& node_creator_builder_ptr);

    ~GraphTreeCreator() = default;

    //! @brief ノード生成クラスを初期化する．
    //! @param[in] map_state マップの状態．
    void Init(const DividedMapState& map_state);

    //! @brief 歩容パターングラフを作成する．
    //! @param[in] start_depth 作成するグラフの開始深さ．
    //! @param[in] max_depth 作成するグラフの最大深さ．
    //! @param[out] graph_ptr 作成したグラフを代入するためのポインタ．
    //! @return グラフの作成に成功したかどうか．
    GraphSearchResult CreateGraphTree(int start_depth, int max_depth,
                                      GaitPatternGraphTree* graph_ptr) const;

private:
    //! @brief current_nodeの子ノードを生成して，output_graphに代入する．
    //! @param[in] current_node 現在のノード．
    //! @param[in] current_num 現在のノードのインデックス．
    //! @param[out] output_graph_ptr 生成したノードを代入するベクタへのポインタ．空にしておくこと.
    void MakeNewNodesByCurrentNode(const RobotStateNode& current_node, int current_num,
                                   std::vector<RobotStateNode>* output_graph_ptr) const;

    //! ノード生成クラスのマップ．
    std::map<enums::HexapodMove, std::unique_ptr<INodeCreator> > node_creator_map_;

    //! ノード生成クラスのビルダー．
    const std::unique_ptr<INodeCreatorBuilder> node_creator_builder_ptr_;
};

}  // namespace designlab


#endif  // DESIGNLAB_GRAPH_TREE_CREATOR_H_
