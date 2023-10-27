#include "pch.h"

#include "../DesignLab/designlab_vector3.h"


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
	TEST(Vector3Test, EqualityOperator)
	{
		std::vector<std::tuple<dl::Vector3, dl::Vector3, bool>> testcase_list{
			{MakeVec3(10.f, -5.f, 0.f),				MakeVec3(10.f, -5.f, 0.f),				true,},
			{MakeVec3(-20.f, 60.f, 10.f),			MakeVec3(-20.f, 60.f, 10.f),			true,},
			{MakeVec3(0.4f, 6.6f, -7.8f),			MakeVec3(0.4f, 6.6f, -7.8f),			true,},
			{MakeVec3(100000.f, -5200.f, 62000.f),	MakeVec3(100000.f, -5200.f, 62000.f),	true,},
			{MakeVec3(10.f, -5.f, 0.f),				MakeVec3(13.f, -5.f, 0.f),				false,},
			{MakeVec3(10.f, -5.f, 0.f),				MakeVec3(10.f, 55.f, 0.f),				false,},
			{MakeVec3(10.f, -5.f, 0.f),				MakeVec3(10.f, -5.f, 8.f),				false,},
			{MakeVec3(4.f, 8.f, -9.f),				MakeVec3(-9.f, 3.f, 1.f),				false,},
		};

		for (const auto& i : testcase_list)
		{
			const dl::Vector3& vec1 = std::get<0>(i);
			const dl::Vector3& vec2 = std::get<1>(i);
			const bool expected = std::get<2>(i);

			if (expected == true)
			{
				std::string error_case_message = ToString(vec1) + "と" + ToString(vec2) + "は等しいはずである．";
				EXPECT_TRUE(vec1 == vec2) << error_case_message;
			}
			else
			{
				std::string error_case_message = ToString(vec1) + "と" + ToString(vec2) + "は等しくないはずである．";
				EXPECT_FALSE(vec1 == vec2) << error_case_message;
			}
		}
	}

	TEST(Vector3Test, InequalityOperator)
	{
		std::vector<std::tuple<dl::Vector3, dl::Vector3, bool>> testcase_list{
			std::make_tuple(MakeVec3(10.f, -5.f, 0.f),				MakeVec3(10.f, -5.f, 0.f),				false),
			std::make_tuple(MakeVec3(-20.f, 60.f, 10.f),			MakeVec3(-20.f, 60.f, 10.f),			false),
			std::make_tuple(MakeVec3(0.4f, 6.6f, -7.8f),			MakeVec3(0.4f, 6.6f, -7.8f),			false),
			std::make_tuple(MakeVec3(100000.f, -5200.f, 62000.f),	MakeVec3(100000.f, -5200.f, 62000.f),	false),
			std::make_tuple(MakeVec3(10.f, -5.f, 0.f),				MakeVec3(13.f, -5.f, 0.f),				true),
			std::make_tuple(MakeVec3(10.f, -5.f, 0.f),				MakeVec3(10.f, 55.f, 0.f),				true),
			std::make_tuple(MakeVec3(10.f, -5.f, 0.f),				MakeVec3(10.f, -5.f, 8.f),				true),
			std::make_tuple(MakeVec3(4.f, 8.f, -9.f),				MakeVec3(-9.f, 3.f, 1.f),				true)
		};

		for (const auto& i : testcase_list)
		{
			const dl::Vector3& vec1 = std::get<0>(i);
			const dl::Vector3& vec2 = std::get<1>(i);
			const bool expected = std::get<2>(i);

			if (expected == true)
			{
				std::string error_case_message = ToString(vec1) + "と" + ToString(vec2) + "は等しくないはずである．";
				EXPECT_TRUE(vec1 != vec2) << error_case_message;
			}
			else
			{
				std::string error_case_message = ToString(vec1) + "と" + ToString(vec2) + "は等しいはずである．";
				EXPECT_FALSE(vec1 != vec2) << error_case_message;
			}
		}
	}

	TEST(Vector3Test, ComparisonOperator)
	{
		//比較演算子のテスト．比較演算子はベクトルの長さを用いて比較を行う．

		EXPECT_TRUE(designlab::Vector3(0, 0, 0) < designlab::Vector3(5, 10, -6));
		EXPECT_TRUE(designlab::Vector3(2, -6, 3) < designlab::Vector3(56, 20, -94));
		EXPECT_TRUE(designlab::Vector3(5, 10, -6) > designlab::Vector3(0, 0, 0));
		EXPECT_TRUE(designlab::Vector3(56, 20, -94) > designlab::Vector3(2, -6, 3));

		EXPECT_TRUE(designlab::Vector3(0, 0, 0) <= designlab::Vector3(5, 10, -6));
		EXPECT_TRUE(designlab::Vector3(2, -6, 3) <= designlab::Vector3(56, 20, -94));
		EXPECT_TRUE(designlab::Vector3(5, 10, -6) >= designlab::Vector3(0, 0, 0));
		EXPECT_TRUE(designlab::Vector3(56, 20, -94) >= designlab::Vector3(2, -6, 3));

		EXPECT_TRUE(designlab::Vector3(-23, 85, 91) <= designlab::Vector3(-23, 85, 91));
		EXPECT_TRUE(designlab::Vector3(-23, 85, 91) >= designlab::Vector3(-23, 85, 91));
	}

}	// namespace designlab::test::common::math
