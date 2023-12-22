#include "pch.h"

#include "../DesignLab/math_vector3.h"


namespace dl = ::designlab;


namespace
{
	// テスト用のヘルパー関数

	//! @brief 与えられた値からVector3を作成する
	dl::Vector3 MakeVec3(float x, float y, float z)
	{
		dl::Vector3 vec;
		vec.x = x;
		vec.y = y;
		vec.z = z;

		return vec;
	}

	//! @brief Vector3を文字列に変換する
	std::string ToString(const dl::Vector3& vec)
	{
		std::stringstream ss;
		ss << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";

		return ss.str();
	}
}

namespace designlab::test::common::math
{
	TEST(Vector3Test, PlusOperatorTest)
	{
		std::vector<std::tuple<dl::Vector3, dl::Vector3, dl::Vector3>> testcase_list{
			{MakeVec3(1.f, 1.f, 0.f),		MakeVec3(1.f, 1.f, 2.f),		MakeVec3(2.f, 2.f, 2.f)},
			{MakeVec3(5.f, 0.f, 0.f),		MakeVec3(5.f, 0.f, 0.f),		MakeVec3(10.f, 0.f, 0.f)},
			{MakeVec3(0.f, 3.f, 0.f),		MakeVec3(0.f, 7.f, 0.f),		MakeVec3(0.f, 10.f, 0.f)},
			{MakeVec3(0.f, 0.f, 6.f),		MakeVec3(0.f, 0.f, 4.f),		MakeVec3(0.f, 0.f, 10.f)},
			{MakeVec3(60.f, 120.f, 200.f),	MakeVec3(40.f, -20.f, -100.f),	MakeVec3(100.f, 100.f, 100.f)},
			{MakeVec3(0.2f, 34.8f, 6.78f),	MakeVec3(2.6f, -13.2f, 6.11f),	MakeVec3(2.8f, 21.6f, 12.89f)},
		};

		for (const auto& [vec1, vec2, expected] : testcase_list)
		{
			std::string error_case_message = ToString(vec1) + "と" + ToString(vec2) + "の和は" + ToString(expected) + "である．";
			EXPECT_EQ(vec1 + vec2, expected) << error_case_message;
		}
	}

	TEST(Vector3Test, CompoundAssignmentPlusOperatorTest)
	{
		// += のような 算術と代入を同時に行う演算子を複合代入演算子(compound assignment operator)と呼ぶ．

		std::vector<std::tuple<dl::Vector3, dl::Vector3, dl::Vector3>> testcase_list{
			{MakeVec3(1.f, 1.f, 0.f),		MakeVec3(1.f, 1.f, 2.f),		MakeVec3(2.f, 2.f, 2.f)},
			{MakeVec3(5.f, 0.f, 0.f),		MakeVec3(5.f, 0.f, 0.f),		MakeVec3(10.f, 0.f, 0.f)},
			{MakeVec3(0.f, 3.f, 0.f),		MakeVec3(0.f, 7.f, 0.f),		MakeVec3(0.f, 10.f, 0.f)},
			{MakeVec3(0.f, 0.f, 6.f),		MakeVec3(0.f, 0.f, 4.f),		MakeVec3(0.f, 0.f, 10.f)},
			{MakeVec3(60.f, 120.f, 200.f),	MakeVec3(40.f, -20.f, -100.f),	MakeVec3(100.f, 100.f, 100.f)},
			{MakeVec3(0.2f, 34.8f, 6.78f),	MakeVec3(2.6f, -13.2f, 6.11f),	MakeVec3(2.8f, 21.6f, 12.89f)},
		};

		for (const auto& [vec1, vec2, expected] : testcase_list)
		{
			dl::Vector3 vec1_copy = vec1;
			vec1_copy += vec2;

			std::string error_case_message = ToString(vec1) + "と" + ToString(vec2) + "の和は" + ToString(expected) + "である．";
			EXPECT_EQ(vec1_copy, expected) << error_case_message;
		}
	}

	TEST(Vector3Test, MinusOperatorTest)
	{
		std::vector<std::tuple<dl::Vector3, dl::Vector3, dl::Vector3>> testcase_list{
			{MakeVec3(15.f, 66.f, 1010.f),	MakeVec3(5.f, 60.f, 1000.f), MakeVec3(10.f, 6.f, 10.f)},
			{MakeVec3(0.f, 210.f, -50.f),	MakeVec3(5.f, 60.f, -100.f), MakeVec3(-5.f, 150.f, 50.f)},
			{MakeVec3(15.f, 66.f, 1010.f),	MakeVec3(5.f, 60.f, 1000.f), MakeVec3(10.f, 6.f, 10.f)}
		};

		for (const auto& [vec1, vec2, expected] : testcase_list)
		{
			std::string error_case_message = ToString(vec1) + "と" + ToString(vec2) + "の差は" + ToString(expected) + "である．";
			EXPECT_EQ(vec1 - vec2, expected) << error_case_message;
		}
	}

	TEST(Vector3Test, CompoundAssignmentMinusOperatorTest)
	{
		std::vector<std::tuple<dl::Vector3, dl::Vector3, dl::Vector3>> testcase_list{
			{MakeVec3(15.f, 66.f, 1010.f),	MakeVec3(5.f, 60.f, 1000.f), MakeVec3(10.f, 6.f, 10.f)},
			{MakeVec3(0.f, 210.f, -50.f),	MakeVec3(5.f, 60.f, -100.f), MakeVec3(-5.f, 150.f, 50.f)},
			{MakeVec3(15.f, 66.f, 1010.f),	MakeVec3(5.f, 60.f, 1000.f), MakeVec3(10.f, 6.f, 10.f)}
		};

		for (const auto& [vec1, vec2, expected] : testcase_list)
		{
			dl::Vector3 vec1_copy = vec1;
			vec1_copy -= vec2;

			std::string error_case_message = ToString(vec1) + "と" + ToString(vec2) + "の差は" + ToString(expected) + "である．";
			EXPECT_EQ(vec1_copy, expected) << error_case_message;
		}
	}

	TEST(Vector3Test, MultiplyOperatorTest)
	{
		std::vector<std::tuple<dl::Vector3, float, dl::Vector3>> testcase_list{
			{MakeVec3(10.f, 6.f, 21.f),			10.f,	MakeVec3(100.f, 60.f, 210.f)},
			{MakeVec3(-2.f, 7.f, 100.f),		-4.f,	MakeVec3(8.f, -28.f, -400.f)},
			{MakeVec3(62.f, 28.f, -84.f),		3.7f,	MakeVec3(229.4f, 103.6f, -310.8f)},
			{MakeVec3(0.256f, -34.21f, 6.002f), 25.3f,	MakeVec3(6.4768f, -865.513f, 151.8506f)},
		};

		for (const auto & [vec, scalar, expected] : testcase_list)
		{
			std::string error_case_message = ToString(vec) + "と" + std::to_string(scalar) + "の積は正しくは" + ToString(expected) + "である．";
			EXPECT_EQ(vec * scalar, expected) << error_case_message;
		}
	}

	TEST(Vector3Test, MultiplyOperatorTestScalarFirst)
	{
		// スカラーを先に書いた場合の掛け算のテスト

		std::vector<std::tuple<dl::Vector3, float, dl::Vector3>> testcase_list{
			{MakeVec3(10.f, 6.f, 21.f),			10.f,	MakeVec3(100.f, 60.f, 210.f)},
			{MakeVec3(-2.f, 7.f, 100.f),		-4.f,	MakeVec3(8.f, -28.f, -400.f)},
			{MakeVec3(62.f, 28.f, -84.f),		3.7f,	MakeVec3(229.4f, 103.6f, -310.8f)},
			{MakeVec3(0.256f, -34.21f, 6.002f), 25.3f,	MakeVec3(6.4768f, -865.513f, 151.8506f)},
		};

		for (const auto& [vec, scalar, expected] : testcase_list)
		{
			std::string error_case_message = ToString(vec) + "と" + std::to_string(scalar) + "の積は正しくは" + ToString(expected) + "である．";
			EXPECT_EQ(scalar * vec, expected) << error_case_message;
		}
	}

	TEST(Vector3Test, CompoundAssignmentMultiplyOperatorTest)
	{
		std::vector<std::tuple<dl::Vector3, float, dl::Vector3>> testcase_list{
			{MakeVec3(10.f, 6.f, 21.f),			10.f,	MakeVec3(100.f, 60.f, 210.f)},
			{MakeVec3(-2.f, 7.f, 100.f),		-4.f,	MakeVec3(8.f, -28.f, -400.f)},
			{MakeVec3(62.f, 28.f, -84.f),		3.7f,	MakeVec3(229.4f, 103.6f, -310.8f)},
			{MakeVec3(0.256f, -34.21f, 6.002f), 25.3f,	MakeVec3(6.4768f, -865.513f, 151.8506f)},
		};

		for (const auto& [vec, scalar, expected] : testcase_list)
		{
			dl::Vector3 vec_copy = vec;
			vec_copy *= scalar;

			std::string error_case_message = ToString(vec) + "と" + std::to_string(scalar) + "の積は正しくは" + ToString(expected) + "である．";
			EXPECT_EQ(vec_copy, expected) << error_case_message;
		}
	}

	TEST(Vector3Test, DevideOperatorTest)
	{
		std::vector<std::tuple<dl::Vector3, float, dl::Vector3>> testcase_list{
			{MakeVec3(10.f, 6.f, 21.f),			10.f,	MakeVec3(1.f, 0.6f, 2.1f)},
			{MakeVec3(-2.f, 7.f, 100.f),		-4.f,	MakeVec3(0.5f, -1.75f, -25.f)},
			{MakeVec3(62.f, 28.f, -84.f),		3.7f,	MakeVec3(16.756757f, 7.567567f, -22.702702f)},
			{MakeVec3(0.256f, -34.21f, 6.002f), 25.3f,	MakeVec3(0.010138f, -1.352174f, 0.237425f)},
		};

		for (const auto& [vec, scalar, expected] : testcase_list)
		{
			std::string error_case_message = ToString(vec) + "と" + std::to_string(scalar) + "の商は正しくは" + ToString(expected) + "である．";
			EXPECT_EQ(vec / scalar, expected) << error_case_message;
		}
	}

	TEST(Vector3Test, CompoundAssignmentDevideOperatorTest)
	{
		std::vector<std::tuple<dl::Vector3, float, dl::Vector3>> testcase_list{
			{MakeVec3(10.f, 6.f, 21.f),			10.f,	MakeVec3(1.f, 0.6f, 2.1f)},
			{MakeVec3(-2.f, 7.f, 100.f),		-4.f,	MakeVec3(0.5f, -1.75f, -25.f)},
			{MakeVec3(62.f, 28.f, -84.f),		3.7f,	MakeVec3(16.756757f, 7.567567f, -22.702702f)},
			{MakeVec3(0.256f, -34.21f, 6.002f), 25.3f,	MakeVec3(0.010138f, -1.352174f, 0.237425f)},
		};

		for (const auto& [vec, scalar, expected] : testcase_list)
		{
			dl::Vector3 vec_copy = vec;
			vec_copy /= scalar;

			std::string error_case_message = ToString(vec) + "と" + std::to_string(scalar) + "の商は正しくは" + ToString(expected) + "である．";
			EXPECT_EQ(vec_copy, expected) << error_case_message;
		}
	}
}
