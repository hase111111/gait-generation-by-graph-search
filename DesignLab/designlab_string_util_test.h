#pragma once

#include <doctest.h>

#include "designlab_string_util.h"


TEST_SUITE("namespace string_util")
{
	std::string StringVectorToString(const std::vector<std::string>&str_vec)
	{
		std::string str = "[";

		for (const auto& s : str_vec)
		{
			str += s + "]・[";
		}

		str += "]";

		return str;
	}


	TEST_CASE("Split")
	{
		SUBCASE("文字列 [a,b,c] を std::string型の[,] で区切ると，[a]・[b]・[c] に分けられる")
		{
			const std::string str = "a,b,c";
			const std::string delim = ",";

			const std::vector<std::string> ans = { "a", "b", "c" };
			const std::vector<std::string> result = designlab::string_util::Split(str, delim);

			INFO("分割された文字列は次の通りです→" << StringVectorToString(result));

			CHECK(result == ans);
		}

		SUBCASE("文字列 [abc,d, e,f,] を std::string型の[,] で区切ると，[abc]・[d]・[ e]・[f] に分けられる")
		{
			std::string str = "abc,d, e,f,";
			const std::string delim = ",";

			const std::vector<std::string> ans = { "abc", "d", " e", "f" };
			const std::vector<std::string> result = designlab::string_util::Split(str, delim);

			INFO("分割された文字列は次の通りです→" << StringVectorToString(result));

			CHECK(result == ans);
		}

		SUBCASE("文字列 [ ,,,there are ,many empty area,  ,] を std::string型の[,] で区切ると，"
			"[ ]・[]・[]・[there are]・[many empty area]・[  ]に分けられる")
		{
			std::string str = " ,,,there are ,many empty area,  ,";
			const std::string delim = ",";

			const std::vector<std::string> ans = { " ", "", "", "there are ", "many empty area", "  " };
			const std::vector<std::string> result = designlab::string_util::Split(str, delim);

			INFO("分割された文字列は次の通りです→" << StringVectorToString(result));

			CHECK(result == ans);
		}

		SUBCASE("文字列 [埼玉大学は大きく大切で雄大な大桐] を std::string型の[大] で区切ると，[埼玉]・[学は]・[きく]・[切で雄]・[な]・[桐]に分けられる")
		{
			std::string str = "埼玉大学は大きく大切で雄大な大桐";
			const std::string delim = "大";

			const std::vector<std::string> ans = { "埼玉", "学は", "きく", "切で雄", "な", "桐" };
			const std::vector<std::string> result = designlab::string_util::Split(str, delim);

			INFO("分割された文字列は次の通りです→" << StringVectorToString(result));

			CHECK(result == ans);
		}
	}
}