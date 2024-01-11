
/// @file      graph_search_evaluator_test.h
/// @author    Hasegawa
/// @copyright © 埼玉大学 設計工学研究室 2024. All right reserved.

#ifndef DESIGNLAB_GRAPH_SEARCH_EVALUATOR_TEST_H_
#define DESIGNLAB_GRAPH_SEARCH_EVALUATOR_TEST_H_

#include <doctest.h>

#include "graph_search_evaluator.h"


TEST_SUITE("GraphSearchEvaluator")
{
    using GraphSearchEvaluator = designlab::GraphSearchEvaluator;
    using GraphSearchEvaluationValue = designlab::GraphSearchEvaluationValue;
    using Tag = GraphSearchEvaluationValue::Tag;

    constexpr Tag kFirstTag = 10;   // 1つ目の評価値のタグ．
    constexpr Tag kSecondTag = 1;  // 2つ目の評価値のタグ．


    TEST_CASE("LeftIsBetterTest_WhenLeftIsBetter_ShouldReturnTrue")
    {
        SUBCASE("LowerValueIsBetter")
        {
            // 評価値が小さいほど良い場合．
            const GraphSearchEvaluator::EvaluationMethod evaluation_method =
            {
                .is_lower_better = true,
                .margin = 0.0f,
            };

            GraphSearchEvaluator evaluator({ { kFirstTag, evaluation_method }, { kSecondTag, evaluation_method } },
                                           { kFirstTag, kSecondTag });


            GraphSearchEvaluationValue left = evaluator.InitializeEvaluationValue();
            left.value.at(kFirstTag) = -1.0f;
            left.value.at(kSecondTag) = -1.0f;
            GraphSearchEvaluationValue right = evaluator.InitializeEvaluationValue();
            right.value.at(kFirstTag) = 1.0f;
            right.value.at(kSecondTag) = 1.0f;

            const bool actual = evaluator.LeftIsBetter(left, right);

            const bool expected = true;

            CHECK(actual == expected);
        }

        SUBCASE("HigherValueIsBetter")
        {
            // 評価値が大きいほど良い場合．
            const GraphSearchEvaluator::EvaluationMethod evaluation_method =
            {
                .is_lower_better = false,
                .margin = 0.0f,
            };

            GraphSearchEvaluator evaluator({ { kFirstTag, evaluation_method }, { kSecondTag, evaluation_method } },
                                                      { kFirstTag, kSecondTag });

            GraphSearchEvaluationValue left = evaluator.InitializeEvaluationValue();
            left.value.at(kFirstTag) = 1.0f;
            left.value.at(kSecondTag) = 1.0f;
            GraphSearchEvaluationValue right = evaluator.InitializeEvaluationValue();
            right.value.at(kFirstTag) = -1.0f;
            right.value.at(kSecondTag) = -1.0f;

            const bool actual = evaluator.LeftIsBetter(left, right);

            const bool expected = true;

            CHECK(actual == expected);
        }

        // 2つ目の評価値が低い評価だとしても，1つ目の評価値が高い評価ならば，そちらを優先する．
        SUBCASE("WhenFirstTagIsBetter")
        {
            // 評価値が小さいほど良い場合．
            const GraphSearchEvaluator::EvaluationMethod evaluation_method =
            {
                .is_lower_better = false,
                .margin = 0.0f,
            };

            GraphSearchEvaluator evaluator({ { kFirstTag, evaluation_method }, { kSecondTag, evaluation_method } },
                                                      { kFirstTag, kSecondTag });

            GraphSearchEvaluationValue left = evaluator.InitializeEvaluationValue();
            left.value.at(kFirstTag) = 1.0f;
            left.value.at(kSecondTag) = 0.0f;
            GraphSearchEvaluationValue right = evaluator.InitializeEvaluationValue();
            right.value.at(kFirstTag) = 0.0f;
            right.value.at(kSecondTag) = -1.0f;

            const bool actual = evaluator.LeftIsBetter(left, right);

            const bool expected = true;

            CHECK(actual == expected);
        }
    }

    TEST_CASE("LeftIsBetterTest_WhenRightIsBetter_ShouldReturnFalse")
    {
        SUBCASE("LowerValueIsBetter")
        {
            const GraphSearchEvaluator::EvaluationMethod evaluation_method =
            {
                .is_lower_better = true,
                .margin = 0.0f,
            };

            GraphSearchEvaluator evaluator({ { kFirstTag, evaluation_method }, { kSecondTag, evaluation_method } },
                                           { kFirstTag, kSecondTag });

            GraphSearchEvaluationValue left = evaluator.InitializeEvaluationValue();
            left.value.at(kFirstTag) = 1.0f;
            left.value.at(kSecondTag) = 1.0f;
            GraphSearchEvaluationValue right = evaluator.InitializeEvaluationValue();
            right.value.at(kFirstTag) = -1.0f;
            right.value.at(kSecondTag) = -1.0f;

            const bool actual = evaluator.LeftIsBetter(left, right);

            const bool expected = false;

            CHECK(actual == expected);
        }

        SUBCASE("HigherValueIsBetter")
        {
            const GraphSearchEvaluator::EvaluationMethod evaluation_method =
            {
                .is_lower_better = false,
                .margin = 0.0f,
            };

            GraphSearchEvaluator evaluator({ { kFirstTag, evaluation_method }, { kSecondTag, evaluation_method } },
                                           { kFirstTag, kSecondTag });

            GraphSearchEvaluationValue left = evaluator.InitializeEvaluationValue();
            left.value.at(kFirstTag) = -1.0f;
            left.value.at(kSecondTag) = -1.0f;
            GraphSearchEvaluationValue right = evaluator.InitializeEvaluationValue();
            right.value.at(kFirstTag) = 1.0f;
            right.value.at(kSecondTag) = 1.0f;

            const bool actual = evaluator.LeftIsBetter(left, right);

            const bool expected = false;

            CHECK(actual == expected);
        }
    }

    TEST_CASE("LeftIsBetterTest_WhenValuesAreEqual_ShouldReturnTrueOrFalse")
    {
        const GraphSearchEvaluator::EvaluationMethod evaluation_method =
        {
            .is_lower_better = true,
            .margin = 1.0f,
        };

        const GraphSearchEvaluator evaluator({ { kFirstTag, evaluation_method }, { kSecondTag, evaluation_method } },
                                             { kFirstTag, kSecondTag });

        GraphSearchEvaluationValue left = evaluator.InitializeEvaluationValue();
        left.value.at(kFirstTag) = 1.0f;
        left.value.at(kSecondTag) = 1.0f;
        GraphSearchEvaluationValue right = evaluator.InitializeEvaluationValue();
        right.value.at(kFirstTag) = 1.0f;
        right.value.at(kSecondTag) = 1.0f;


        // 左側を優先する設定の場合は true を返す．
        SUBCASE("WhenReturnTrueCaseOfEqualIsTrue")
        {
            const bool actual = evaluator.LeftIsBetter(left, right, true);

            const bool expected = true;  // 左側を優先する．

            CHECK(actual == expected);
        }

        // 左側を優先しない設定の場合は false を返す．
        SUBCASE("WhenReturnTrueCaseOfEqualIsFalse")
        {
            const bool actual = evaluator.LeftIsBetter(left, right, false);

            const bool expected = false;  // 左側を優先しない．

            CHECK(actual == expected);
        }
    }

    TEST_CASE("LeftIsBetterTest_WhenValuesAreEqualInMargin_ShouldReturnTrueOrFalse")
    {
        const GraphSearchEvaluator::EvaluationMethod evaluation_method =
        {
            .is_lower_better = true,
            .margin = 1.0f,
        };

        const GraphSearchEvaluator evaluator({ { kFirstTag, evaluation_method }, { kSecondTag, evaluation_method } },
                                                    { kFirstTag, kSecondTag });

        GraphSearchEvaluationValue left = evaluator.InitializeEvaluationValue();
        left.value.at(kFirstTag) = 1.0f;
        left.value.at(kSecondTag) = 1.0f;
        GraphSearchEvaluationValue right = evaluator.InitializeEvaluationValue();
        right.value.at(kFirstTag) = 0.5f;   // 差が margin 以下．
        right.value.at(kSecondTag) = 1.5f;  // 差が margin 以下．

        // 左側を優先する設定の場合は true を返す．
        SUBCASE("WhenLeftIsPreferred_ShouldReturnTrue")
        {
            const bool actual = evaluator.LeftIsBetter(left, right, true);

            const bool expected = true;  // 左側を優先する．

            CHECK(actual == expected);
        }

        // 左側を優先しない設定の場合は false を返す．
        SUBCASE("WhenRightIsPreferred_ShouldReturnFalse")
        {
            const bool actual = evaluator.LeftIsBetter(left, right, false);

            const bool expected = false;  // 左側を優先しない．

            CHECK(actual == expected);
        }
    }

    TEST_CASE("InitializeEvaluationValueTest_ShouldReturnMinEvaluationValue")
    {
        // 初期値は評価が最低となる値あることを確認する．

        const GraphSearchEvaluator::EvaluationMethod evaluation_method_min =
        {
            .is_lower_better = true,
            .margin = 0.0f,
        };

        const GraphSearchEvaluator::EvaluationMethod evaluation_method_max =
        {
            .is_lower_better = false,
            .margin = 0.0f,
        };

        // FirstTag は低い値，SecondTag は高い値を高く評価する．
        GraphSearchEvaluator evaluator({ { kFirstTag, evaluation_method_min }, { kSecondTag, evaluation_method_max } },
                                       { kFirstTag, kSecondTag });

        const GraphSearchEvaluationValue actual = evaluator.InitializeEvaluationValue();

        const GraphSearchEvaluationValue expected =
        {
            .value = { { kFirstTag, GraphSearchEvaluationValue::kMaxEvaluationValue },  // 最低評価となるのは，最も高い値．
                       { kSecondTag, GraphSearchEvaluationValue::kMinEvaluationValue } },  // 最低評価となるのは，最も低い値．
        };

        CHECK(actual.value.at(kFirstTag) == expected.value.at(kFirstTag));
        CHECK(actual.value.at(kSecondTag) == expected.value.at(kSecondTag));

        CHECK(actual.value.size() == expected.value.size());
    }
}


#endif  // DESIGNLAB_GRAPH_SEARCH_EVALUATOR_TEST_H_
