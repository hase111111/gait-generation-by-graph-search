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
	TEST(Vector3Test, EqualityOperatorTest)
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

		for (const auto& [vec1, vec2, expected] : testcase_list)
		{
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

	TEST(Vector3Test, InequalityOperatorTest)
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

		for (const auto& [vec1, vec2, expected] : testcase_list)
		{
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

	TEST(Vector3Test, ComparisonLessThanOperatorTest)
	{
		//比較演算子はベクトルの長さを用いて比較を行う．
		// < less than

		EXPECT_TRUE(MakeVec3(0.f, 0.f, 0.f)		< MakeVec3(5.f, 10.f, -6.f));
		EXPECT_TRUE(MakeVec3(2.f, -6.f, 3.f)	< MakeVec3(56.f, 20.f, -94.f));

		EXPECT_FALSE(MakeVec3(4.0f, 5.6f, -2.0f)	< MakeVec3(4.0f, 5.6f, -2.0f));
		EXPECT_FALSE(MakeVec3(-26.f, 85.f, 120.f)	< MakeVec3(-23, 85, 91));
	}

	TEST(Vector3Test, ComparisonLessThanOrEqualToOperatorTest)
	{
		// <= less than or equal to
		 
		EXPECT_TRUE(MakeVec3(0.f, 0.f, 0.f)		<= MakeVec3(5.f, 10.f, -6.f));
		EXPECT_TRUE(MakeVec3(2.f, -6.f, 3.f)	<= MakeVec3(56.f, 20.f, -94.f));
		EXPECT_TRUE(MakeVec3(4.0f, 5.6f, -2.0f) <= MakeVec3(4.0f, 5.6f, -2.0f));

		EXPECT_FALSE(MakeVec3(-26.f, 85.f, 120.f) <= MakeVec3(-23, 85, 91));
	}

	TEST(Vector3Test, ComparisonGreaterThanOperatorTest)
	{
		// > greater than

		EXPECT_TRUE(MakeVec3(5.f, 10.f, -6.f)	> MakeVec3(0.f, 0.f, 0.f));
		EXPECT_TRUE(MakeVec3(56.f, 20.f, -94.f)	> MakeVec3(2.f, -6.f, 3.f));

		EXPECT_FALSE(MakeVec3(4.0f, 5.6f, -2.0f)	> MakeVec3(4.0f, 5.6f, -2.0f));
		EXPECT_FALSE(MakeVec3(-23, 85, 91)		> MakeVec3(-26.f, 85.f, 120.f));
	}

	TEST(Vector3Test, ComparisonGreaterThanOrEqualToOperatorTest)
	{
		// >= greater than or equal to

		EXPECT_TRUE(MakeVec3(5.f, 10.f, -6.f)	>= MakeVec3(0.f, 0.f, 0.f));
		EXPECT_TRUE(MakeVec3(56.f, 20.f, -94.f)	>= MakeVec3(2.f, -6.f, 3.f));
		EXPECT_TRUE(MakeVec3(4.0f, 5.6f, -2.0f)	>= MakeVec3(4.0f, 5.6f, -2.0f));

		EXPECT_FALSE(MakeVec3(-23, 85, 91)		>= MakeVec3(-26.f, 85.f, 120.f));
	}

}	// namespace designlab::test::common::math
