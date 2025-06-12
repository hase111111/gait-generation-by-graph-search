
//! @file cassert_define.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#ifndef DESIGNLAB_CASSERT_DEFINE_H_
#define DESIGNLAB_CASSERT_DEFINE_H_

// Visual Studioでは,デバッグモードではアサートを有効化し,
// リリースモードではアサートを無効化する.
// しかし,このプログラムは処理が重いせいで,
// 環境によってはそもそもデバッグモードで実行することが難しい.
// そこで,リリースモードでもアサートを有効化するために,
// 以下のようなヘッダファイルを作成した.
//
// リリースモードでアサートを出したい場合は,以下のリリースモードのところで
// #undef NDEBUG でアサートを有効化すること.
// NDEBUGが defineされているときは,assertを無効化する.
// #undef は defineされているものを無効化する.
// つまり,#undef NDEBUG は,assertを有効化する.
// 逆に,#define NDEBUG は,assertを無効化する.
// これらの処理は,cassertのインクルードより前に行う必要がある.

#ifndef _DEBUG  // if not define _DEBUG つまり,リリースモードの場合,

// アサートを有効化する場合は,以下の行のコメント( // )をはずすこと
#undef NDEBUG

#endif  // _DEBUG

#include <cassert>

#endif  // DESIGNLAB_CASSERT_DEFINE_H_
