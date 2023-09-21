//! @file cassert_define.h
//! @brief デバッグモードとリリースモードでアサートを有効化・無効化するためのヘッダファイル
//! @details デバッグモードではアサートを有効化する．リリースモードではアサートを無効化する．
//! @n このプログラムは処理が重いせいで，環境によってはデバッグモードで実行することが難しい
//! @n なので，リリースモードでアサートを出したい場合は，以下のリリースモードのところで
//! @n #undef NDEBUG でアサートを有効化すること．


#ifndef DESIGNLAB_CASSERT_DEFINE_H_
#define DESIGNLAB_CASSERT_DEFINE_H_

#if defined(_DEBUG)	// デバッグモードの場合

#undef NDEBUG		// アサートを有効化する

#else				// リリースモードの場合

#undef NDEBUG		// アサートを有効化する
//#define NDEBUG	// アサートを無効化する

#endif 


#ifndef NDEBUG

#include <cstdlib>
#include <iostream>

//参考 : https://programming-place.net/ppp/contents/cpp2/main/answer/assert_answer.html#ans4

#define DESIGNLAB_MSG_ASSERT(cond, msg)  \
    if (!(cond)) { \
        std::cerr << "Assertion failed: " << #cond << ", " << __FILE__ << " " << __LINE__ << " " << __func__ << "\n"; \
        std::cerr << (msg) << "\n"; \
        std::abort(); \
    }

#else 

#define DESIGNLAB_MSG_ASSERT(cond,msg) ((void)0)

#endif // NDEBUG




#include <cassert>

#endif // !DESIGNLAB_CASSERT_DEFINE_H_