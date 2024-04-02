
//! @file      test_main.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#ifndef DESIGNLAB_TEST_MAIN_H_
#define DESIGNLAB_TEST_MAIN_H_


// ↓テストを使用しない場合コメントアウトすること．

// #define DESIGNLAB_USE_TEST


#if defined(DESIGNLAB_USE_TEST)

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest.h>


// 以下にテストコードが書かれたhファイルをインクルードすることで登録される．

#include "asyncable_data_test.h"
#include "divided_map_state_test.h"
#include "gait_pattern_graph_tree_test.h"
#include "graph_search_evaluator_test.h"
#include "map_state_test.h"
#include "math_line_segment2_test.h"
#include "math_rotation_matrix_test.h"
#include "math_util_test.h"
#include "math_vector2_test.h"
#include "math_vector3_test.h"
#include "node_creator_leg_hierarchy_test.h"
#include "simulation_map_parameter_validator_test.h"
#include "string_util_test.h"


#endif  // DESIGNLAB_USE_TEST

#endif  // DESIGNLAB_TEST_MAIN_H_
