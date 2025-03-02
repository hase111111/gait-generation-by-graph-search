﻿
//! @file model_loader.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include "model_loader.h"

#include <DxLib.h>


namespace designlab
{

int ModelLoader::GetModelHandle(const std::string& file_path)
{
    // すでに読み込み済みのモデルの場合は，ハンドル番号を返す．
    if (model_handle_map_.count(file_path) != 0)
    {
        return model_handle_map_[file_path];
    }

    // モデルを読み込む．
    int handle = MV1LoadModel(file_path.c_str());

    // 読み込みに失敗した場合は-1を返す．
    if (handle == -1)
    {
        return -1;
    }

    // 読み込みに成功した場合は，対応表に追加する．
    model_handle_map_[file_path] = handle;

    return handle;
}

}  // namespace designlab
