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
		SUBCASE("文字列 [a,b,c] を [,] で区切ると，[a]・[b]・[c] に分けられる")
		{
			const std::string str = "a,b,c";
			const std::string delim = ",";

			const std::vector<std::string> ans = { "a", "b", "c" };
			const std::vector<std::string> result = designlab::string_util::Split(str, delim);

			INFO("分割された文字列は次の通りです→" << StringVectorToString(result));

			CHECK(result == ans);
		}

		SUBCASE("文字列 [a,b,c] を [c] で区切ると，[a,b,] に分けられる")
		{
			const std::string str = "a,b,c";
			const std::string delim = "c";

			const std::vector<std::string> ans = { "a,b," };
			const std::vector<std::string> result = designlab::string_util::Split(str, delim);

			INFO("分割された文字列は次の通りです→" << StringVectorToString(result));

			CHECK(result == ans);
		}

		SUBCASE("文字列 [a,b,c] を [a] で区切ると，[]・[,b,c] に分けられる")
		{
			const std::string str = "a,b,c";
			const std::string delim = "a";

			const std::vector<std::string> ans = { "", ",b,c" };
			const std::vector<std::string> result = designlab::string_util::Split(str, delim);

			INFO("分割された文字列は次の通りです→" << StringVectorToString(result));

			CHECK(result == ans);
		}

		SUBCASE("文字列 [a,b,c] を [h] で区切ると，分けられず [a,b,c] のまま")
		{
			const std::string str = "a,b,c";
			const std::string delim = "h";

			const std::vector<std::string> ans = { "a,b,c" };
			const std::vector<std::string> result = designlab::string_util::Split(str, delim);

			INFO("分割された文字列は次の通りです→" << StringVectorToString(result));

			CHECK(result == ans);
		}

		SUBCASE("文字列 [a,b,c,] を [,] で区切ると，[a]・[b]・[c] に分けられる")
		{
			const std::string str = "a,b,c,";
			const std::string delim = ",";

			const std::vector<std::string> ans = { "a", "b", "c" };
			const std::vector<std::string> result = designlab::string_util::Split(str, delim);

			INFO("分割された文字列は次の通りです→" << StringVectorToString(result));

			CHECK(result == ans);
		}

		SUBCASE("文字列 [a,b,c, ] を [,] で区切ると，[a]・[b]・[c]・[ ] に分けられる")
		{
			const std::string str = "a,b,c, ";
			const std::string delim = ",";

			const std::vector<std::string> ans = { "a", "b", "c", " " };
			const std::vector<std::string> result = designlab::string_util::Split(str, delim);

			INFO("分割された文字列は次の通りです→" << StringVectorToString(result));

			CHECK(result == ans);
		}

		SUBCASE("文字列 [犬,猫andネズミ,私と\nあなた,cap,] を [,] で区切ると，[犬]・[猫andネズミ]・[私と\nあなた]・[cap] に分けられる")
		{
			const std::string str = "犬,猫andネズミ,私と\nあなた,cap,";
			const std::string delim = ",";

			const std::vector<std::string> ans = { "犬", "猫andネズミ", "私と\nあなた", "cap" };
			const std::vector<std::string> result = designlab::string_util::Split(str, delim);

			INFO("分割された文字列は次の通りです→" << StringVectorToString(result));

			CHECK(result == ans);
		}

		SUBCASE("文字列 [犬,猫andネズミ,私と\nあなた,cap,] を [\n] で区切ると，[犬,猫andネズミ,私と]・[あなた,cap,] に分けられる")
		{
			const std::string str = "犬,猫andネズミ,私と\nあなた,cap,";
			const std::string delim = "\n";

			const std::vector<std::string> ans = { "犬,猫andネズミ,私と", "あなた,cap," };
			const std::vector<std::string> result = designlab::string_util::Split(str, delim);

			INFO("分割された文字列は次の通りです→" << StringVectorToString(result));

			CHECK(result == ans);
		}
	}

	enum class SampleEnum : int
	{
		kOne,
		Two,
		three,
		FOUR,
	};

	TEST_CASE("EnumToStringRemoveTopK")
	{
		SUBCASE("SampleEnum型の値 [kOne] を渡すと，文字列 [One] が返る")
		{
			const SampleEnum sample_enum = SampleEnum::kOne;
			const std::string ans = "One";
			const std::string result = designlab::string_util::EnumToStringRemoveTopK(sample_enum);

			INFO("変換された文字列は次の通りです→" << result);

			CHECK(result == ans);
		}

		SUBCASE("SampleEnum型の値 [Two] を渡すと，文字列 [Two] が返る")
		{
			const SampleEnum sample_enum = SampleEnum::Two;
			const std::string ans = "Two";
			const std::string result = designlab::string_util::EnumToStringRemoveTopK(sample_enum);

			INFO("変換された文字列は次の通りです→" << result);

			CHECK(result == ans);
		}

		SUBCASE("SampleEnum型の値 [three] を渡すと，文字列 [three] が返る")
		{
			const SampleEnum sample_enum = SampleEnum::three;
			const std::string ans = "three";
			const std::string result = designlab::string_util::EnumToStringRemoveTopK(sample_enum);

			INFO("変換された文字列は次の通りです→" << result);

			CHECK(result == ans);
		}

		SUBCASE("SampleEnum型の値 [FOUR] を渡すと，文字列 [FOUR] が返る")
		{
			const SampleEnum sample_enum = SampleEnum::FOUR;
			const std::string ans = "FOUR";
			const std::string result = designlab::string_util::EnumToStringRemoveTopK(sample_enum);

			INFO("変換された文字列は次の通りです→" << result);

			CHECK(result == ans);
		}
	}

	TEST_CASE("EnumValuesToString")
	{
		SUBCASE("SampleEnum型を [,] で区切ると，文字列 [kOne,Two,three,FOUR] が返る")
		{
			const std::string ans = "kOne,Two,three,FOUR";
			const std::string sep = ",";
			const std::string result = designlab::string_util::EnumValuesToString<SampleEnum>(sep);

			INFO("変換された文字列は次の通りです→" << result);

			CHECK(result == ans);
		}
	}
}