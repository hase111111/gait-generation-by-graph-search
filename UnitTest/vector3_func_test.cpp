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

	//! @brief 与えられた値からVector2を作成する
	dl::Vector2 MakeVec2(float x, float y)
	{
		dl::Vector2 vec;
		vec.x = x;
		vec.y = y;

		return vec;
	}

	//! @brief Vector3を文字列に変換する
	std::string ToString(const dl::Vector3& vec)
	{
		std::stringstream ss;
		ss << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";

		return ss.str();
	}

	//! @brief Vector2を文字列に変換する
	std::string ToString(const dl::Vector2& vec)
	{
		std::stringstream ss;
		ss << "(" << vec.x << ", " << vec.y << ")";

		return ss.str();
	}
}


namespace designlab::test::common::math
{
	TEST(Vector3Test, GetSquaredLengthTest)
	{
		std::vector<std::tuple<dl::Vector3, float>> testcase_list{
			std::make_tuple(MakeVec3(0,0,0),			0.f),
			std::make_tuple(MakeVec3(1,1,1),			3.f),
			std::make_tuple(MakeVec3(1,1,-1),			3.f),
			std::make_tuple(MakeVec3(-1,-1,-1),			3.f),
			std::make_tuple(MakeVec3(0, 5, -12),		169.f),
			std::make_tuple(MakeVec3(4, -10, 5),		141.f),
			std::make_tuple(MakeVec3(100, -80, -35),	17625.f),
			std::make_tuple(MakeVec3(0.1f, -0.7f, 3.5f),12.75f),
		};

		for (const auto& [vec, length] : testcase_list)
		{
			std::string error_case_message = ToString(vec) + "の長さの二乗は正しくは" + std::to_string(length) + "である．";
			EXPECT_FLOAT_EQ(vec.GetSquaredLength(), length) << error_case_message;
		}
	}

	TEST(Vector3Test, GetLengthTest)
	{
		std::vector<std::tuple<dl::Vector3, float>> testcase_list{
			std::make_tuple(MakeVec3(0,0,0),			0.f),
			std::make_tuple(MakeVec3(1,1,1),			1.7320508f),
			std::make_tuple(MakeVec3(1,1,-1),			1.7320508f),
			std::make_tuple(MakeVec3(-1,-1,-1),			1.7320508f),
			std::make_tuple(MakeVec3(0, 5, -12),		13.f),
			std::make_tuple(MakeVec3(4, -10, 5),		11.874342f),
			std::make_tuple(MakeVec3(100, -80, -35),	132.75919f),
		};

		for (const auto & [vec, length] : testcase_list)
		{
			std::string error_case_message = ToString(vec) + "の長さは正しくは" + std::to_string(length) + "である．";
			EXPECT_FLOAT_EQ(vec.GetLength(), length) << error_case_message;
		}
	}

	TEST(Vector3Test, GetDistanceFromTest)
	{
		std::vector<std::tuple<dl::Vector3, dl::Vector3, float>> testcase_list{
			std::make_tuple(MakeVec3(0,0,0),	MakeVec3(0.f, 0.f, 0.f),		0.f),
			std::make_tuple(MakeVec3(0,0,0),	MakeVec3(8.f, 7.f, -10.f),		14.594519f),
			std::make_tuple(MakeVec3(0,0,0),	MakeVec3(16.f, -30.f, 100.f),	105.621967f),
			std::make_tuple(MakeVec3(0,0,0),	MakeVec3(-4.6f, 0.4f, 34.6f),	34.906732f),
			std::make_tuple(MakeVec3(0,0,0),	MakeVec3(0.1f, -0.7f, 3.5f),	3.570714f),

			std::make_tuple(MakeVec3(10.f, 0, 5.f),			MakeVec3(26.f, -30.f, 105.f),	105.621967f),
			std::make_tuple(MakeVec3(26.f, -30.f, 105.f),	MakeVec3(10.f, 0, 5.f),			105.621967f),
			std::make_tuple(MakeVec3(0.2f, 1.f, -3.f),	MakeVec3(-4.4f, 1.4f, 31.6f),		34.906732f),
		};

		for (const auto & [vec, other, distance] : testcase_list)
		{
			std::string error_case_message = ToString(vec) + "と" + ToString(other) + "の距離は正しくは" + std::to_string(distance) + "である．";
			EXPECT_FLOAT_EQ(vec.GetDistanceFrom(other), distance) << error_case_message;
		}
	}

	TEST(Vector3Test, GetNormalizedTest)
	{
		std::vector<std::tuple<dl::Vector3, dl::Vector3>> testcase_list{
			std::make_tuple(MakeVec3(500.f, 0.f, 0.f),			MakeVec3(1.f, 0.f, 0.f)),
			std::make_tuple(MakeVec3(0.f, 54.10f, 0.f),			MakeVec3(0.f, 1.f, 0.f)),
			std::make_tuple(MakeVec3(0.f, 0.f, 1.f),			MakeVec3(0.f, 0.f, 1.f)),
			std::make_tuple(MakeVec3(-23445.f, 0.f, 0.f),		MakeVec3(-1.f, 0.f, 0.f)),
			std::make_tuple(MakeVec3(5.4f, 5.4f, 5.4f),			MakeVec3(0.57735f, 0.57735f, 0.57735f)),
			std::make_tuple(MakeVec3(-94, -94, -94),			MakeVec3(-0.57735f, -0.57735f, -0.57735f)),
			std::make_tuple(MakeVec3(17, 17, -17),				MakeVec3(0.57735f, 0.57735f, -0.57735f)),
			std::make_tuple(MakeVec3(3, 2, -1),					MakeVec3(0.801784f, 0.534522f, -0.267261f)),
			std::make_tuple(MakeVec3(62.51f, -94.78f, 25.89f),	MakeVec3(0.536788f, -0.813898f, 0.222324f)),
		};

		for (const auto& [vec, normalized_vec] : testcase_list)
		{
			std::string error_case_message = ToString(vec) + "の正規化は正しくは" + ToString(normalized_vec) + "である．";
			EXPECT_EQ(vec.GetNormalized(), normalized_vec) << error_case_message;

			ASSERT_FLOAT_EQ(normalized_vec.GetNormalized().GetLength(), 1.f) << "テストケース" + ToString(vec) + "の答えのベクトルの長さが1ではありません．";
			EXPECT_FLOAT_EQ(vec.GetNormalized().GetLength(), 1.f) << ToString(vec) + "の正規化されたベクトルの長さは1になる．";
		}
	}

	TEST(Vector3Test, GetNormalizedTestZeroVecTransToZeroVec) 
	{
		dl::Vector3 zero_vec = MakeVec3(0.f, 0.f, 0.f);
		EXPECT_EQ(zero_vec.GetNormalized(), zero_vec) << "長さが0のベクトルを渡されると，そのまま0ベクトルを返します．";
	}

	TEST(Vector3Test, IsZeroTest)
	{
		dl::Vector3 zero_vec = MakeVec3(0.f, 0.f, 0.f);
		EXPECT_TRUE(zero_vec.IsZero()) << "長さが0のベクトルを渡されるとtrueになる．";

		std::vector<dl::Vector3> false_data_list{
			MakeVec3(-10.f, 0.f, 0.f),
			MakeVec3(512.f, 0.f, 0.f),
			MakeVec3(0.f, 45.f, 0.f),
			MakeVec3(0.f, -3.f, 0.f),
			MakeVec3(0.f, 0.f, 0.5f),
			MakeVec3(0.f, 0.f, -0.2f),
			MakeVec3(0.f, 100.f, -35.f),
			MakeVec3(67.f, 0.f, 26.8f),
			MakeVec3(-71.f, 0.7f, 0.f),
			MakeVec3(24.f, 67.f, 89.f),
		};

		for (const auto& vec : false_data_list)
		{
			EXPECT_FALSE(vec.IsZero()) << ToString(vec) + "は長さが0ではないのでfalseになる．";
		}
	}

	TEST(Vector3Test, DotTest)
	{
		std::vector<std::tuple<dl::Vector3, dl::Vector3, float>> testcase_list{
			std::make_tuple(MakeVec3(0,0,0),			MakeVec3(0.f, 0.f, 0.f),		0.f),
			std::make_tuple(MakeVec3(1,1,1),			MakeVec3(1.f, 1.f, 1.f),		3.f),
			std::make_tuple(MakeVec3(1,1,-1),			MakeVec3(1.f, 1.f, -1.f),		3.f),
			std::make_tuple(MakeVec3(-1,-1,-1),			MakeVec3(-1.f, -1.f, -1.f),		3.f),
			std::make_tuple(MakeVec3(0, 5, -12),		MakeVec3(0.f, 5.f, -12.f),		169.f),
			std::make_tuple(MakeVec3(4, -10, 5),		MakeVec3(4.f, -10.f, 5.f),		141.f),
			std::make_tuple(MakeVec3(100, -80, -35),	MakeVec3(100.f, -80.f, -35.f),	17625.f),
			std::make_tuple(MakeVec3(0.1f, -0.7f, 3.5f),MakeVec3(0.1f, -0.7f, 3.5f),	12.75f),
		};

		for (const auto& [vec, other, dot] : testcase_list)
		{
			std::string error_case_message = ToString(vec) + "と" + ToString(other) + "の内積は正しくは" + std::to_string(dot) + "である．";
			EXPECT_FLOAT_EQ(vec.Dot(other), dot) << error_case_message;
		}
	}

	TEST(Vector3Test, CrossTest)
	{
		std::vector<std::tuple<dl::Vector3, dl::Vector3, dl::Vector3>> testcase_list{
			std::make_tuple(MakeVec3(0,0,0),			MakeVec3(0.f, 0.f, 0.f),		MakeVec3(0.f, 0.f, 0.f)),
			std::make_tuple(MakeVec3(3.f, 4.f, 1.f),		MakeVec3(3.f, 7.f, 5.f),		MakeVec3(13.f, -12.f, 9.f)),
			std::make_tuple(MakeVec3(3.f, 7.f, 5.f),		MakeVec3(3.f, 4.f, 1.f),		MakeVec3(-13.f, 12.f, -9.f)),
			std::make_tuple(MakeVec3(1.f, 1.f, 1.f),		MakeVec3(3.f, 4.f, 1.f),		MakeVec3(-3.f, 2.f, 1.f)),
		};

		for (const auto& [vec, other, cross] : testcase_list)
		{
			std::string error_case_message = ToString(vec) + "と" + ToString(other) + "の外積は正しくは" + ToString(cross) + "である．";
			EXPECT_EQ(vec.Cross(other), cross) << error_case_message;
		}
	}

	TEST(Vector3Test, ProjectXYTest)
	{
		std::vector<std::tuple<dl::Vector3, dl::Vector2>> testcase_list{
			std::make_tuple(MakeVec3(0,0,0),			MakeVec2(0.f, 0.f)),
			std::make_tuple(MakeVec3(1,1,1),			MakeVec2(1.f, 1.f)),
			std::make_tuple(MakeVec3(1,1,-1),			MakeVec2(1.f, 1.f)),
			std::make_tuple(MakeVec3(-1,10,-1),			MakeVec2(-1.f, 10.f)),
		};

		for (const auto& [vec, projected_vec] : testcase_list)
		{
			std::string error_case_message = ToString(vec) + "をXY平面に射影したベクトルは正しくは" + ToString(projected_vec) + "である．";
			EXPECT_EQ(vec.ProjectedXY(), projected_vec) << error_case_message;
		}
	}

}	// namespace designlab::test::common::math
