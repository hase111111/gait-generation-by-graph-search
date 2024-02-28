
//! @file      graph_search_evaluator.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2024. All right reserved.

#ifndef DESIGNLAB_GRAPH_SEARCH_EVALUATOR_H_
#define DESIGNLAB_GRAPH_SEARCH_EVALUATOR_H_

#include <map>
#include <vector>


namespace designlab
{

//! @struct GraphSearchEvaluationValue
//! @brief グラフ探索の評価値を格納する構造体．
struct GraphSearchEvaluationValue final
{
    using Tag = int;

    constexpr static float kMaxEvaluationValue = 1.0e+7f;
    constexpr static float kMinEvaluationValue = -1.0e+7f;

    std::map<Tag, float> value;
};


//! @struct GraphSearchResult
//! @brief グラフ探索の評価値を評価するクラス．
class GraphSearchEvaluator final
{
    using Tag = GraphSearchEvaluationValue::Tag;  // 読みやすさのため，別名をつける．

public:
    //! @struct EvaluationMethod
    //! @brief 評価方法を格納する構造体．
    struct EvaluationMethod final
    {
        bool is_lower_better;  //!< 評価値が小さいほど良い場合は true．

        float margin;  //!< この値よりも違いが小さい場合は，評価値が同じとみなす．
    };

    GraphSearchEvaluator() = delete;  //!< デフォルトコンストラクタは禁止．

    //! @brief コンストラクタ．
    //! @param[in] evaluation_method_map 評価方法を格納したマップ．
    //! @param[in] evaluation_priority_list 優先的に評価を行う Tag から順に並べたリスト．
    GraphSearchEvaluator(const std::map<Tag, EvaluationMethod>& evaluation_method_map,
                         const std::vector<Tag>& evaluation_priority_list);

    ~GraphSearchEvaluator() = default;


    //! @brief 2つの評価値を比較する．左側の評価値が良い場合は true を返す．
    //! @param[in] left 左側の評価値．
    //! @param[in] right 右側の評価値．
    //! @param[in] return_true_case_of_equal 左右の評価値が等しい場合に true を返すかどうか．
    //! デフォルトは true．
    bool LeftIsBetter(const GraphSearchEvaluationValue& left,
                      const GraphSearchEvaluationValue& right,
                      bool return_true_case_of_equal = true) const;

    //! @brief 2つの評価値を比較する．左側の評価値が良い場合は true を返す．
    //! @param[in] left 左側の評価値．
    //! @param[in] right 右側の評価値．
    //! @param[in] tag 評価値を比較する際に用いる Tag．
    //! @param[in] return_true_case_of_equal 左右の評価値が等しい場合に true を返すかどうか．
    bool LeftIsBetterWithTag(const GraphSearchEvaluationValue& left,
                             const GraphSearchEvaluationValue& right,
                             Tag tag,
                             bool return_true_case_of_equal = true) const;

    //! @brief 評価値を初期化する．
    //! 自身の持つ評価方法を用いて，評価値を初期化する．
    //! @return 初期化された評価値．
    GraphSearchEvaluationValue InitializeEvaluationValue() const;

private:
    const std::map<Tag, EvaluationMethod> evaluation_method_map_;  //!< Tag に対する評価方法を格納したマップ．

    //! 優先的に評価を行う Tag から順に並べたリスト．
    const std::vector<Tag> evaluation_priority_list_;
};

}  // namespace designlab


#endif  // DESIGNLAB_GRAPH_SEARCH_EVALUATOR_H_
