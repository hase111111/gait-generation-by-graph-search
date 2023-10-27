#include "pch.h"

#include "../DesignLab/designlab_vector3.h"
#include "../DesignLab/designlab_vector3.cpp"	//どこか1つのファイルで，cppファイルをインクルードする必要がある


namespace dl = ::designlab;


namespace designlab::test::common::math
{
	TEST(Vector3Test, DefaultConstructor)
	{
		//デフォルトコンストラクタのテスト
		dl::Vector3 vec;

		std::string str = "デフォルトコンストラクタは(0,0,0)となる";
		EXPECT_FLOAT_EQ(vec.x, 0.f) << str;
		EXPECT_FLOAT_EQ(vec.y, 0.f) << str;
		EXPECT_FLOAT_EQ(vec.z, 0.f) << str;
	}

	TEST(Vector3Test, Constructor)
	{
		//コンストラクタのテスト
		dl::Vector3 vec(1.f, 2.f, 3.f);

		std::string str = "コンストラクタは与えられた値で，Vector3を作成する";
		EXPECT_FLOAT_EQ(vec.x, 1.f) << str;
		EXPECT_FLOAT_EQ(vec.y, 2.f) << str;
		EXPECT_FLOAT_EQ(vec.z, 3.f) << str;
	}

	TEST(Vector3Test, CopyConstructor)
	{
		//コピーコンストラクタのテスト
		dl::Vector3 vec(1.f, 2.f, 3.f);
		dl::Vector3 vec2(vec);

		std::string str = "コピーコンストラクタは与えられた値をコピーして，Vector3を作成する";
		EXPECT_FLOAT_EQ(vec2.x, 1.f) << str;
		EXPECT_FLOAT_EQ(vec2.y, 2.f) << str;
		EXPECT_FLOAT_EQ(vec2.z, 3.f) << str;
	}

	TEST(Vector3Test, AssignmentOperator)
	{
		// Assignment Operator = 代入演算子
		dl::Vector3 vec(1.f, 2.f, 3.f);
		dl::Vector3 vec2 = vec;

		std::string str = "代入演算子は与えられた値を代入して，Vector3を作成する";
		EXPECT_FLOAT_EQ(vec2.x, 1.f) << str;
		EXPECT_FLOAT_EQ(vec2.y, 2.f) << str;
		EXPECT_FLOAT_EQ(vec2.z, 3.f) << str;
	}

}	// namespace designlab::test::common::math
