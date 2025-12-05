
//! @file toml_serialize_macro.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include "toml_serialize_macro.h"

namespace gaitgen::toml_func
{

const char Toml11Description::kNoTable[] = "no_table_value";

std::vector<std::string> sjis_to_utf8_vec(const std::vector<std::string>& str_vec)
{
    std::vector<std::string> ret_vec;

    for (const auto& str : str_vec)
    {
        ret_vec.push_back(sjis_to_utf8(str));
        // ret_vec.push_back(str);
    }

    return ret_vec;
}


}  // namespace gaitgen::toml_func
