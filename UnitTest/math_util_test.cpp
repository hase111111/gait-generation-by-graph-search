#include "pch.h"

#include "../DesignLab/math_util.h"
#include "../DesignLab/designlab_math_util.cpp"


namespace dlm = designlab::math_util;


namespace designlab::test::common::math
{
	TEST(MathUtilTest, IsEqualTestFloatType)
	{
		//float型のオーバーロード

		//TRUE
		EXPECT_TRUE(dlm::IsEqual(2.0f, 2.0f));
		EXPECT_TRUE(dlm::IsEqual(-9.0f, -9.0f));
		EXPECT_TRUE(dlm::IsEqual(129.0f, 129.0f));
		EXPECT_TRUE(dlm::IsEqual(-9125.0f, -9125.0f));

		//FALSE
		EXPECT_FALSE(dlm::IsEqual(9.0f, 1.0f));
		EXPECT_FALSE(dlm::IsEqual(9.0f, -9.0f));
		EXPECT_FALSE(dlm::IsEqual(25.0f, 64.0f));
		EXPECT_FALSE(dlm::IsEqual(-91.0f, -26.0f));

		//小数点以下3桁までの誤差を許容する．大きい場合
		EXPECT_TRUE(dlm::IsEqual(5.0f, 5.0f + 0.00001f));
		EXPECT_TRUE(dlm::IsEqual(5.0f, 5.0f + 0.0001f));
		//EXPECT_FALSE(dlm::IsEqual(5.0f, 5.0f + 0.001f));
		EXPECT_FALSE(dlm::IsEqual(5.0f, 5.0f + 0.0011f));
		EXPECT_FALSE(dlm::IsEqual(5.0f, 5.0f + 0.01f));
		EXPECT_FALSE(dlm::IsEqual(5.0f, 5.0f + 0.1f));

		//小さい場合
		EXPECT_TRUE(dlm::IsEqual(5.0f, 5.0f - 0.00001f));
		EXPECT_TRUE(dlm::IsEqual(5.0f, 5.0f - 0.0001f));
		//EXPECT_FALSE(dlm::IsEqual(5.0f, 5.0f - 0.001f));
		EXPECT_FALSE(dlm::IsEqual(5.0f, 5.0f - 0.0011f));
		EXPECT_FALSE(dlm::IsEqual(5.0f, 5.0f - 0.01f));
		EXPECT_FALSE(dlm::IsEqual(5.0f, 5.0f - 0.1f));
	}

	TEST(MathUtilTest, IsEqualTestDoubleType)
	{
		//double型のオーバーロード

		//TRUE
		EXPECT_TRUE(dlm::IsEqual(2.0, 2.0));
		EXPECT_TRUE(dlm::IsEqual(-9.0, -9.0));
		EXPECT_TRUE(dlm::IsEqual(129.0, 129.0));
		EXPECT_TRUE(dlm::IsEqual(-628.0, -628.0));
		EXPECT_TRUE(dlm::IsEqual(-9125.0, -9125.0));

		//FALSE
		EXPECT_FALSE(dlm::IsEqual(9.0, 1.0));
		EXPECT_FALSE(dlm::IsEqual(9.0, -9.0));
		EXPECT_FALSE(dlm::IsEqual(25.0, 64.0));
		EXPECT_FALSE(dlm::IsEqual(-91.0, -26.0));
		EXPECT_FALSE(dlm::IsEqual(0.0, -5.0));

		//小数点以下3桁までの誤差を許容する．大きい場合
		EXPECT_TRUE(dlm::IsEqual(5.0, 5.0 + 0.00001));
		EXPECT_TRUE(dlm::IsEqual(5.0, 5.0 + 0.0001));
		EXPECT_FALSE(dlm::IsEqual(5.0, 5.0 + 0.001));
		EXPECT_FALSE(dlm::IsEqual(5.0, 5.0 + 0.01));
		EXPECT_FALSE(dlm::IsEqual(5.0, 5.0 + 0.1));

		//小さい場合
		EXPECT_TRUE(dlm::IsEqual(5.0, 5.0 - 0.00001));
		EXPECT_TRUE(dlm::IsEqual(5.0, 5.0 - 0.0001));
		EXPECT_FALSE(dlm::IsEqual(5.0, 5.0 - 0.001));
		EXPECT_FALSE(dlm::IsEqual(5.0, 5.0 - 0.01));
		EXPECT_FALSE(dlm::IsEqual(5.0, 5.0 - 0.1));
	}

	TEST(MathUtilTest, SquaredTestIntType)
	{
		//int型
		EXPECT_EQ(dlm::Squared(-13), 169);
		EXPECT_EQ(dlm::Squared(4), 16);
		EXPECT_EQ(dlm::Squared(0), 0);
	}

	TEST(MathUtilTest, SquaredTestDoubleType)
	{
		//double型
		EXPECT_TRUE(dlm::IsEqual(dlm::Squared(4.8), 23.04));
		EXPECT_TRUE(dlm::IsEqual(dlm::Squared(125.2), 15675.04));
		EXPECT_TRUE(dlm::IsEqual(dlm::Squared(0.1), 0.01));
	}

	TEST(MathUtilTest, SquaredTestFloatType)
	{
		//float型
		EXPECT_TRUE(dlm::IsEqual(dlm::Squared(4.8f), 23.04f));
		EXPECT_TRUE(dlm::IsEqual(dlm::Squared(125.2f), 15675.04f));
		EXPECT_TRUE(dlm::IsEqual(dlm::Squared(0.1f), 0.01f));
	}

	TEST(MathUtilTest, SquaredTestCharType)
	{
		//char型
		EXPECT_EQ(dlm::Squared<char>(11),	char{ 121 });
		EXPECT_EQ(dlm::Squared<char>(-7),	char{ 49 });
		EXPECT_EQ(dlm::Squared<char>(0),	char{ 0 });
	}

	TEST(MathUtilTest, SquaredTestUnsignedIntType)
	{
		//unsigned int型
		EXPECT_EQ(dlm::Squared<unsigned int>(13),	unsigned int{ 169 });
		EXPECT_EQ(dlm::Squared<unsigned int>(1429), unsigned int{ 2042041 });
		EXPECT_EQ(dlm::Squared<unsigned int>(0),	unsigned int{ 0 });
	}

	TEST(MathUtilTest, SquaredTestSizetType)
	{
		//size_t型
		EXPECT_EQ(dlm::Squared<size_t>(11), size_t{ 121 });
		EXPECT_EQ(dlm::Squared<size_t>(7),	size_t{ 49 });
		EXPECT_EQ(dlm::Squared<size_t>(0),	size_t{ 0 });
	}

	TEST(MathUtilTest, CanMakeTriangleTestFloatTypeTrueCase) 
	{
		//三角形が作れる場合のテスト
		std::vector<std::tuple<float, float, float>> testcase_list
		{
			{1.0f, 1.0f, 1.0f},	{1.0f, 1.0f, 1.5f},	{1.0f, 1.5f, 1.0f},
			{1.5f, 1.0f, 1.0f},	{2.0f, 2.0f, 2.0f},	{2.0f, 2.0f, 3.0f},
			{2.0f, 3.0f, 2.0f},	{3.0f, 2.0f, 2.0f},	{3.0f, 3.0f, 3.0f},
			{3.0f, 3.0f, 4.0f},	{3.0f, 4.0f, 3.0f},	{4.0f, 3.0f, 3.0f},
			{4.0f, 4.0f, 4.0f},	{4.0f, 4.0f, 5.0f},	{4.0f, 5.0f, 4.0f},
			{5.0f, 4.0f, 4.0f},	{5.0f, 5.0f, 5.0f},	{12.f, 13.f, 5.0f}
		};
		
		for (const auto& i : testcase_list)
		{
			const float& a = std::get<0>(i);
			const float& b = std::get<1>(i);
			const float& c = std::get<2>(i);

			std::string error_case_message = std::to_string(a) + ", " + std::to_string(b) + ", " + std::to_string(c) + "の3辺から三角形が作れるはずである．";
			EXPECT_TRUE(dlm::CanMakeTriangle(a, b, c)) << error_case_message;
		}
	}

	TEST(MathUtilTest, CanMakeTriangleTestFloatTypeFalseCase)
	{
		//三角形が作れない場合のテスト
		std::vector<std::tuple<float, float, float>> testcase_list
		{
			{1.0f, 1.0f, 2.0f},	{1.0f, 2.0f, 1.0f},	{2.0f, 1.0f, 1.0f},
			{1.0f, 2.0f, 3.0f},	{1.0f, 3.0f, 2.0f},	{2.0f, 1.0f, 3.0f},
			{2.0f, 3.0f, 1.0f},	{3.0f, 1.0f, 2.0f},	{3.0f, 2.0f, 1.0f},
			{1.0f, 1.0f, 3.0f},	{1.0f, 3.0f, 1.0f},	{3.0f, 1.0f, 1.0f},
			{1.0f, 2.0f, 4.0f},	{1.0f, 4.0f, 2.0f},	{4.0f, 1.0f, 2.0f},
			{2.0f, 1.0f, 45.f},	{2.0f, 40.f, 1.0f}, {100.f, 0.1f, 0.3f},
		};

		for (const auto& i : testcase_list)
		{
			const float& a = std::get<0>(i);
			const float& b = std::get<1>(i);
			const float& c = std::get<2>(i);

			std::string error_case_message = std::to_string(a) + ", " + std::to_string(b) + ", " + std::to_string(c) + "の3辺から三角形が作れないはずである．";
			EXPECT_FALSE(dlm::CanMakeTriangle(a, b, c)) << error_case_message;
		}
	}

	TEST(MathUtilTest, ConvertRadToDegTestFloatType)
	{
		//Rad ,Deg の順番
		std::vector<std::tuple<float, float>> testcase_list
		{
			{0.0f, 0.0f},		{0.5f * dlm::kFloatPi, 90.0f},	{dlm::kFloatPi, 180.0f},
			{1.5f * dlm::kFloatPi, 270.0f},	{2.0f * dlm::kFloatPi, 360.0f},	{3.0f * dlm::kFloatPi, 540.0f},
			{-0.5f * dlm::kFloatPi, -90.0f},	{-dlm::kFloatPi, -180.0f},		{-1.5f * dlm::kFloatPi, -270.0f},
			{-2.0f * dlm::kFloatPi, -360.0f},	{-3.0f * dlm::kFloatPi, -540.0f}
		};

		for (const auto& i : testcase_list)
		{
			const float& rad = std::get<0>(i);
			const float& deg = std::get<1>(i);

			std::string error_case_message = std::to_string(rad) + " [rad] は" + std::to_string(deg) + " [deg]になるはずである．\n_";
			EXPECT_TRUE(dlm::IsEqual(dlm::ConvertRadToDeg(rad), deg)) << error_case_message;
		}
	}

	TEST(MathUtilTest, ConvertRadToDegTestDoubleType)
	{
		//Rad ,Deg の順番
		std::vector<std::tuple<double, double>> testcase_list
		{
			{0.0, 0.0},		{0.5 * dlm::kDoublePi, 90.0},	{dlm::kDoublePi, 180.0},
			{1.5 * dlm::kDoublePi, 270.0},	{2.0 * dlm::kDoublePi, 360.0},	{3.0 * dlm::kDoublePi, 540.0},
			{-0.5 * dlm::kDoublePi, -90.0},	{-dlm::kDoublePi, -180.0},		{-1.5 * dlm::kDoublePi, -270.0},
			{-2.0 * dlm::kDoublePi, -360.0},	{-3.0 * dlm::kDoublePi, -540.0}
		};

		for (const auto& i : testcase_list)
		{
			const double& rad = std::get<0>(i);
			const double& deg = std::get<1>(i);

			std::string error_case_message = std::to_string(rad) + " [rad] は" + std::to_string(deg) + " [deg]になるはずである．\n_";
			EXPECT_TRUE(dlm::IsEqual(dlm::ConvertRadToDeg(rad), deg)) << error_case_message;
		}
	}

	TEST(MathUtilTest, ConvertDegToRadTestFloatType)
	{
		//Deg ,Rad の順番
		std::vector<std::tuple<float, float>> testcase_list
		{
			{0.0f, 0.0f},		{90.0f, 0.5f * dlm::kFloatPi},	{180.0f, dlm::kFloatPi},
			{270.0f, 1.5f * dlm::kFloatPi},	{360.0f, 2.0f * dlm::kFloatPi},	{540.0f, 3.0f * dlm::kFloatPi},
			{-90.0f, -0.5f * dlm::kFloatPi},	{-180.0f, -dlm::kFloatPi},		{-270.0f, -1.5f * dlm::kFloatPi},
			{-360.0f, -2.0f * dlm::kFloatPi},	{-540.0f, -3.0f * dlm::kFloatPi}
		};

		for (const auto& i : testcase_list)
		{
			const float& deg = std::get<0>(i);
			const float& rad = std::get<1>(i);

			std::string error_case_message = std::to_string(deg) + " [deg] は" + std::to_string(rad) + " [rad]になるはずである．\n_";
			EXPECT_TRUE(dlm::IsEqual(dlm::ConvertDegToRad(deg), rad)) << error_case_message;
		}
	}

	TEST(MathUtilTest, ConvertDegToRadTestDoubleType)
	{
		//Deg ,Rad の順番
		std::vector<std::tuple<double, double>> testcase_list
		{
			{0.0, 0.0},		{90.0, 0.5 * dlm::kDoublePi},	{180.0, dlm::kDoublePi},
			{270.0, 1.5 * dlm::kDoublePi},	{360.0, 2.0 * dlm::kDoublePi},	{540.0, 3.0 * dlm::kDoublePi},
			{-90.0, -0.5 * dlm::kDoublePi},	{-180.0, -dlm::kDoublePi},		{-270.0, -1.5 * dlm::kDoublePi},
			{-360.0, -2.0 * dlm::kDoublePi},	{-540.0, -3.0 * dlm::kDoublePi}
		};

		for (const auto& i : testcase_list)
		{
			const double& deg = std::get<0>(i);
			const double& rad = std::get<1>(i);

			std::string error_case_message = std::to_string(deg) + " [deg] は" + std::to_string(rad) + " [rad]になるはずである．\n_";
			EXPECT_TRUE(dlm::IsEqual(dlm::ConvertDegToRad(deg), rad)) << error_case_message;
		}
	}


}	// namespace designlab::test::common::math
