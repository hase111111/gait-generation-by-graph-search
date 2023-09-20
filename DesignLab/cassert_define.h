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

#include <cassert>

#endif // !DESIGNLAB_CASSERT_DEFINE_H_