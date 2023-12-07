#ifndef DESIGNLAB_TEST_REGISTRATION_FORM_H_
#define DESIGNLAB_TEST_REGISTRATION_FORM_H_

//↓テストを使用しない場合コメントアウトすること．
#define DESIGNLAB_USE_TEST

#if defined(DESIGNLAB_USE_TEST)

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>


//以下にテストコードが書かれたhファイルをインクルードすることで登録される．
#include "designlab_string_util_test.h"
#include "gait_pattern_graph_tree_test.h"
#include "map_state_test.h"


#endif	// DESIGNLAB_USE_TEST

#endif	// DESIGNLAB_TEST_REGISTRATION_FORM_H_