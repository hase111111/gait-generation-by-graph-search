
//! @file      cassert_define.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.
//! @brief デバッグモードとリリースモードでアサートを有効化・無効化するためのヘッダファイル．
//! @details
//! Visual Studioでは，デバッグモードではアサートを有効化し，
//! リリースモードではアサートを無効化する．
//! @n しかし，このプログラムは処理が重いせいで，
//! 環境によってはそもそもデバッグモードで実行することが難しい．
//! @n そこで，リリースモードでもアサートを有効化するために，
//! 以下のようなヘッダファイルを作成した．
//! @n
//! @n リリースモードでアサートを出したい場合は，以下のリリースモードのところで
//! @n #undef NDEBUG でアサートを有効化すること．
//! @n NDEBUGが defineされているときは，assertを無効化する．
//! @n #undef は defineされているものを無効化する．
//! @n つまり，#undef NDEBUG は，assertを有効化する．
//! @n 逆に，#define NDEBUG は，assertを無効化する．
//! @n これらの処理は，cassertのインクルードより前に行う必要がある．

#ifndef DESIGNLAB_CASSERT_DEFINE_H_
#define DESIGNLAB_CASSERT_DEFINE_H_


#ifndef _DEBUG  // if not define _DEBUG つまり，リリースモードの場合，

// アサートを有効化する場合は，以下の行のコメント( // )をはずすこと
#undef NDEBUG

#endif


#include <cassert>


#endif  // DESIGNLAB_CASSERT_DEFINE_H_
