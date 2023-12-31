
//! @file      graph_search_const.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_GRAPH_SEARCH_CONST_H_
#define DESIGNLAB_GRAPH_SEARCH_CONST_H_


namespace designlab
{

//! @class GraphSearchConst
//! @brief グラフ探索の定数をまとめたクラス．
//! @n 先行研究のマクロをまとめたもの．
//! @todo 最終的に消す．
class GraphSearchConst final
{
public:
    // 実体が生成できないようにコンストラクタを privateにする
    GraphSearchConst() = default;
    GraphSearchConst(const GraphSearchConst&) = default;
    GraphSearchConst(GraphSearchConst&&) = default;
    GraphSearchConst& operator=(const GraphSearchConst&) = default;


    static const int kMaxDepth;  //!< グラフ探索の最大深さ．
};

}  // namespace designlab

#endif  // DESIGNLAB_GRAPH_SEARCH_CONST_H_
