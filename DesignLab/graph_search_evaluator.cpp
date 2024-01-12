
/// @author    Hasegawa
/// @copyright © 埼玉大学 設計工学研究室 2024. All right reserved.

#include "cassert_define.h"
#include "graph_search_evaluator.h"


namespace designlab
{

GraphSearchEvaluator::GraphSearchEvaluator(const std::map<GraphSearchEvaluationValue::Tag, EvaluationMethod>& evaluation_method_map,
    const std::vector<Tag>& evaluation_priority_list) :
    evaluation_method_map_(evaluation_method_map),
    evaluation_priority_list_(evaluation_priority_list)
{
    // 評価方法のマップの要素数と，優先的に評価を行う Tag のリストの要素数が一致していることを確認する．
    assert(evaluation_method_map_.size() == evaluation_priority_list_.size());

    //! @todo ここで，evaluation_priority_list_ のもつ Tag が，evaluation_method_map_ の Tag と一致していることを確認する．
    //! 面倒であるため，上記の assert で代用する．
}

bool GraphSearchEvaluator::LeftIsBetter(const GraphSearchEvaluationValue& left,
    const GraphSearchEvaluationValue& right,
    const bool return_true_case_of_equal) const
{
    // 優先的に評価を行う Tag から順に比較する．
    for (const auto& tag : evaluation_priority_list_)
    {
        // 可読性のため，変数に格納する．
        const float& left_value = left.value.at(tag);
        const float& right_value = right.value.at(tag);

        const auto& evaluation_method = evaluation_method_map_.at(tag);

        assert(evaluation_method.margin >= 0.0f);  // margin は負の値にはできない．

        // margin 以下の違いは無視する．
        if (std::abs(left_value - right_value) <= evaluation_method.margin)
        {
            // 次の要素を比較する．
            continue;
        }


        if (left_value < right_value)
        {
            return evaluation_method.is_lower_better;
        }
        else if (left_value > right_value)
        {
            return !evaluation_method.is_lower_better;
        }
        else
        {
            continue;
        }
    }

    // ここまで来た場合は，全ての要素が等しいと判断されたということ．
    // その場合は，return_true_case_of_equal に従って返す．
    return return_true_case_of_equal;
}

bool GraphSearchEvaluator::LeftIsBetterWithTag(const GraphSearchEvaluationValue& left, const GraphSearchEvaluationValue& right, Tag tag, bool return_true_case_of_equal) const
{
    // 可読性のため，変数に格納する．
    const float& left_value = left.value.at(tag);
    const float& right_value = right.value.at(tag);

    const auto& evaluation_method = evaluation_method_map_.at(tag);

    assert(evaluation_method.margin >= 0.0f);  // margin は負の値にはできない．

    // margin 以下の違いは無視する．
    if (std::abs(left_value - right_value) <= evaluation_method.margin)
    {
        return return_true_case_of_equal;
    }

    if (left_value < right_value)
    {
        return evaluation_method.is_lower_better;
    }
    else
    {
        return !evaluation_method.is_lower_better;
    }
}

GraphSearchEvaluationValue GraphSearchEvaluator::InitializeEvaluationValue() const
{
    // 最低の評価になるように初期化する．
    GraphSearchEvaluationValue evaluation_value;

    for (const auto& tag : evaluation_priority_list_)
    {
        const EvaluationMethod evaluation_method = evaluation_method_map_.at(tag);

        if (evaluation_method.is_lower_better)
        {
            // 評価値が小さいほど良い場合，最大の評価値を設定する．
            evaluation_value.value[tag] = GraphSearchEvaluationValue::kMaxEvaluationValue;
        }
        else
        {
            // 評価値が大きいほど良い場合，最小の評価値を設定する．
            evaluation_value.value[tag] = GraphSearchEvaluationValue::kMinEvaluationValue;
        }
    }

    return evaluation_value;
}

}  // namespace designlab
