#include "pch.h"

#include "../DesignLab/designlab_string_util.h"
#include "../DesignLab/designlab_string_util.cpp"


namespace dlsu = ::designlab::string_util;


namespace designlab::test::common 
{
	TEST(StringUtilTest, SplitTestCharType)
	{
		std::vector<std::tuple<std::string, char, std::vector<std::string>>> testcase_list{
			{"Hello World", ' ', { "Hello", "World" }},
			{"Hello World", 'o', { "Hell", " W", "rld" }},
			{"Hello World", 'd', { "Hello Worl" }},
			{"Hello World", 'H', { "", "ello World" }},
			{"102, 99, 11, true", ',', {"102", " 99", " 11", " true"}},
			{"102, 99, 11, true,", ',', {"102", " 99", " 11", " true"}},
			{", 102, 99, 11, true", ',', { "", " 102", " 99", " 11", " true"}},
			{", 102, 99, 11, true,", ',', { "", " 102", " 99", " 11", " true"}}
		};

		for (const auto & [str, delim, expected] : testcase_list)
		{
			std::vector<std::string> actual = dlsu::Split(str, delim);

			ASSERT_EQ(expected, actual);
		}
	}

	TEST(StringUtilTest, SplitTestStringType)
	{
		std::vector<std::tuple<std::string, std::string, std::vector<std::string>>> testcase_list{
			{"Hello World", " ", {"Hello", "World"}},
			{"Hello World", "o", { "Hell", " W", "rld" }},
			{"Hello World", "d", { "Hello Worl" }},
			{"Hello World", "H", { "", "ello World" }},
			{"102, 99, 11, true", ",", {"102", " 99", " 11", " true"}},
			{"102, 99, 11, true,", ",", {"102", " 99", " 11", " true"}},
			{", 102, 99, 11, true", ",", { "", " 102", " 99", " 11", " true"}},
			{", 102, 99, 11, true,", ",", { "", " 102", " 99", " 11", " true"}},
			{", 102, 99, , 11, true,", ",", { "", " 102", " 99", " "," 11", " true"}}
		};

		for (const auto& [str, delim, expected] : testcase_list)
		{
			std::vector<std::string> actual = dlsu::Split(str, delim);

			ASSERT_EQ(expected, actual) << "delim : " << delim;
		}
	}

} // namespace designlab::test::common