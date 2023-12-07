﻿#include "toml_serialize_macro.h"

namespace designlab
{
	namespace toml_func
	{
		const std::string Toml11Description::NO_TABLE = "no_table_value";


		std::vector<std::string> sjis_to_utf8_vec(const std::vector<std::string>& str_vec)
		{
			std::vector<std::string> ret_vec;
			for (const auto& str : str_vec)
			{
				ret_vec.push_back(sjis_to_utf8(str));
			}
			return ret_vec;
		}
	}
}
