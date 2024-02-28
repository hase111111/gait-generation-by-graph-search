
//! @file      string_util_test.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#ifndef DESIGNLAB_STRING_UTIL_TEST_H_
#define DESIGNLAB_STRING_UTIL_TEST_H_

#include <doctest.h>

#include <string>
#include <vector>

#include "string_util.h"



TEST_SUITE("string_util::Split")
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

    TEST_CASE("カンマで区切った時，カンマが取り除かれ区切られた文字列に分けられるべき")
    {
        SUBCASE("文字列「a,b,c」をカンマで区切った時，「a」「b」「c」に分けられるべき")
        {
            const std::string str = "a,b,c";
            const std::string delim = ",";

            const std::vector<std::string> ans = { "a", "b", "c" };
            const std::vector<std::string> result = designlab::string_util::Split(str, delim);

            INFO("分割された文字列は次の通りです．" << StringVectorToString(result));

            CHECK(result == ans);
        }

        SUBCASE("文字列「a,b,c,」をカンマで区切った時，「a」「b」「c」に分けられるべき")
        {
            const std::string str = "a,b,c,";
            const std::string delim = ",";

            const std::vector<std::string> ans = { "a", "b", "c" };
            const std::vector<std::string> result = designlab::string_util::Split(str, delim);

            INFO("分割された文字列は次の通りです．" << StringVectorToString(result));

            CHECK(result == ans);
        }

        SUBCASE("文字列「,a,b,c」をカンマで区切った時，「」「a」「b」「c」に分けられるべき")
        {
            const std::string str = ",a,b,c";
            const std::string delim = ",";

            const std::vector<std::string> ans = { "", "a", "b", "c" };
            const std::vector<std::string> result = designlab::string_util::Split(str, delim);

            INFO("分割された文字列は次の通りです．" << StringVectorToString(result));

            CHECK(result == ans);
        }
    }
}

TEST_SUITE("string_util::EnumToStringRemoveTopK")
{
    enum class SampleEnum : int
    {
        kOne,
        knight,
        three,
        FOUR,
        Five
    };

    TEST_CASE("先頭にkがついた列挙体の要素を渡した時，先頭のkを取り除いた要素名が返るべき")
    {
        SUBCASE("「kOne」を渡した時，文字列「One」が返るべき")
        {
            const SampleEnum sample_enum = SampleEnum::kOne;
            const std::string expect = "One";
            const std::string actual = designlab::string_util::EnumToStringRemoveTopK(sample_enum);

            INFO("変換された文字列は次の通りです." << actual);

            CHECK(actual == expect);
        }

        SUBCASE("「knight」を渡した時，文字列「night」が返るべき")
        {
            const SampleEnum sample_enum = SampleEnum::knight;
            const std::string expect = "night";
            const std::string actual = designlab::string_util::EnumToStringRemoveTopK(sample_enum);

            INFO("変換された文字列は次の通りです．" << actual);

            CHECK(actual == expect);
        }
    }

    TEST_CASE("先頭にkがついてない列挙体の要素を渡した時，そのままの要素名が返るべき")
    {
        SUBCASE("「three」を渡した時，そのままの要素名が返るべき")
        {
            const SampleEnum sample_enum = SampleEnum::three;
            const std::string expect = "three";
            const std::string actual = designlab::string_util::EnumToStringRemoveTopK(sample_enum);

            INFO("変換された文字列は次の通りです．" << actual);

            CHECK(actual == expect);
        }

        SUBCASE("「FOUR」を渡した時，そのままの要素名が返るべき")
        {
            const SampleEnum sample_enum = SampleEnum::FOUR;
            const std::string expect = "FOUR";
            const std::string actual = designlab::string_util::EnumToStringRemoveTopK(sample_enum);

            INFO("変換された文字列は次の通りです．" << actual);

            CHECK(actual == expect);
        }

        SUBCASE("「Five」を渡した時，そのままの要素名が返るべき")
        {
            const SampleEnum sample_enum = SampleEnum::Five;
            const std::string expect = "Five";
            const std::string actual = designlab::string_util::EnumToStringRemoveTopK(sample_enum);

            INFO("変換された文字列は次の通りです．" << actual);

            CHECK(actual == expect);
        }
    }
}

TEST_SUITE("string_util::EnumValuesToString")
{
    enum class SampleEnum : int
    {
        kApple,
        kBanana,
        kTomato,
    };

    TEST_CASE("区切り文字をカンマにした時，文字列「kApple,kBanana,kTomato」が返るべき")
    {
        const std::string expect = "kApple,kBanana,kTomato";
        const std::string sep = ",";
        const std::string actual = designlab::string_util::EnumValuesToString<SampleEnum>(sep);

        INFO("変換された文字列は次の通りです．" << actual);

        CHECK(actual == expect);
    }
}

TEST_SUITE("string_util::EnumEntriesToString")
{
    enum class SampleEnum : int
    {
        kApple = 0,
        kBanana = 1,
        kTomato = 5,
    };

    TEST_CASE("区切り文字をカンマにした時，文字列「kApple = 0,kBanana = 1,kTomato = 5」が返るべき")
    {
        const std::string expect = "kApple = 0,kBanana = 1,kTomato = 5";
        const std::string sep = ",";
        const std::string actual = designlab::string_util::EnumEntriesToString<SampleEnum>(sep);

        INFO("変換された文字列は次の通りです．" << actual);

        CHECK(actual == expect);
    }
}

#endif  // DESIGNLAB_STRING_UTIL_TEST_H_
