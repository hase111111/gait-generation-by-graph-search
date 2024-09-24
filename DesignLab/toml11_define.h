﻿
//! @file toml11_define.h
//! @brief ライブラリの toml11の設定を行うためのヘッダファイル．
//! @n #include <toml.hpp>はこのプロジェクトの中では書かず，
//! このファイルをインクルードすること．

// Copyright(c) 2023 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#ifndef DESIGNLAB_TOML11_DEFINE_H_
#define DESIGNLAB_TOML11_DEFINE_H_


//! @def TOML11_PRESERVE_COMMENTS_BY_DEFAULT
//! @brief toml11のコメントを保持するための設定．
//! @n toml.hppをインクルードする前に定義する必要がある．
#define TOML11_PRESERVE_COMMENTS_BY_DEFAULT

#include <toml.hpp>


#endif  // DESIGNLAB_TOML11_DEFINE_H_
