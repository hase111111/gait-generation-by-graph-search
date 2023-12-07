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

		SUBCASE("文字列 [a,b,c] を std::string型の[c] で区切ると，[a,b,] に分けられる")
		{
			const std::string str = "a,b,c";
			const std::string delim = "c";

			const std::vector<std::string> ans = { "a,b," };
			const std::vector<std::string> result = designlab::string_util::Split(str, delim);

			INFO("分割された文字列は次の通りです→" << StringVectorToString(result));

			CHECK(result == ans);
		}

		SUBCASE("文字列 [a,b,c] を std::string型の[a] で区切ると，[]・[,b,c] に分けられる")
		{
			const std::string str = "a,b,c";
			const std::string delim = "a";

			const std::vector<std::string> ans = { "", ",b,c" };
			const std::vector<std::string> result = designlab::string_util::Split(str, delim);

			INFO("分割された文字列は次の通りです→" << StringVectorToString(result));

			CHECK(result == ans);
		}

		SUBCASE("文字列 [a,b,c] を std::string型の[h] で区切ると，分けられず [a,b,c] のまま")
		{
			const std::string str = "a,b,c";
			const std::string delim = "h";

			const std::vector<std::string> ans = { "a,b,c" };
			const std::vector<std::string> result = designlab::string_util::Split(str, delim);

			INFO("分割された文字列は次の通りです→" << StringVectorToString(result));

			CHECK(result == ans);
		}

		SUBCASE("文字列 [a,b,c,] を std::string型の[,] で区切ると，[a]・[b]・[c] に分けられる")
		{
			const std::string str = "a,b,c,";
			const std::string delim = ",";

			const std::vector<std::string> ans = { "a", "b", "c" };
			const std::vector<std::string> result = designlab::string_util::Split(str, delim);

			INFO("分割された文字列は次の通りです→" << StringVectorToString(result));

			CHECK(result == ans);
		}

		SUBCASE("文字列 [a,b,c, ] を std::string型の[,] で区切ると，[a]・[b]・[c]・[ ] に分けられる")
		{
			const std::string str = "a,b,c, ";
			const std::string delim = ",";

			const std::vector<std::string> ans = { "a", "b", "c", " " };
			const std::vector<std::string> result = designlab::string_util::Split(str, delim);

			INFO("分割された文字列は次の通りです→" << StringVectorToString(result));

			CHECK(result == ans);
		}

		SUBCASE("文字列 [犬,猫andネズミ,私と\nあなた,cap,] を std::string型の[,] で区切ると，[犬]・[猫andネズミ]・[私と\nあなた]・[cap] に分けられる")
		{
			const std::string str = "犬,猫andネズミ,私と\nあなた,cap,";
			const std::string delim = ",";

			const std::vector<std::string> ans = { "犬", "猫andネズミ", "私と\nあなた", "cap" };
			const std::vector<std::string> result = designlab::string_util::Split(str, delim);

			INFO("分割された文字列は次の通りです→" << StringVectorToString(result));

			CHECK(result == ans);
		}

		SUBCASE("文字列 [犬,猫andネズミ,私と\nあなた,cap,] を std::string型の[\n] で区切ると，[犬,猫andネズミ,私と]・[あなた,cap,] に分けられる")
		{
			const std::string str = "犬,猫andネズミ,私と\nあなた,cap,";
			const std::string delim = "\n";

			const std::vector<std::string> ans = { "犬,猫andネズミ,私と", "あなた,cap," };
			const std::vector<std::string> result = designlab::string_util::Split(str, delim);

			INFO("分割された文字列は次の通りです→" << StringVectorToString(result));

			CHECK(result == ans);
		}
	}
}